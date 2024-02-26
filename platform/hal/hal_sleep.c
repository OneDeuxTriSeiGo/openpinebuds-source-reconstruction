/*
 * Copyright (c) 2021 Bestechnic (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "analog.h"
#include "cmsis.h"
#include "hal_cmu.h"
#include "hal_dma.h"
#include "hal_gpadc.h"
#include "hal_location.h"
#include "hal_norflash.h"
#include "hal_sleep.h"
#include "hal_sysfreq.h"
#include "hal_trace.h"
#include "hal_timer.h"
#include "pmu.h"

//static uint8_t SRAM_STACK_LOC sleep_stack[128];

static HAL_SLEEP_HOOK_HANDLER sleep_hook_handler[HAL_SLEEP_HOOK_USER_QTY];
static HAL_DEEP_SLEEP_HOOK_HANDLER deep_sleep_hook_handler[HAL_DEEP_SLEEP_HOOK_USER_QTY];

static uint32_t cpu_wake_lock_map;
static uint32_t sys_wake_lock_map;
static uint32_t chip_wake_lock_map;

#ifdef SLEEP_STATS_TRACE
static uint32_t stats_trace_interval;
static uint32_t stats_trace_time;
#endif
static uint32_t stats_interval;
static uint32_t stats_start_time;
static uint32_t light_sleep_time;
static uint32_t sys_deep_sleep_time;
static uint32_t chip_deep_sleep_time;
static bool stats_started;
static bool stats_valid;
static uint8_t light_sleep_ratio;
static uint8_t sys_deep_sleep_ratio;
static uint8_t chip_deep_sleep_ratio;

void hal_sleep_start_stats(uint32_t stats_interval_ms, uint32_t trace_interval_ms)
{
    uint32_t lock;

    lock = int_lock();
    if (stats_interval_ms) {
        stats_interval = MS_TO_TICKS(stats_interval_ms);
        stats_start_time = hal_sys_timer_get();
        light_sleep_time = 0;
        sys_deep_sleep_time = 0;
        chip_deep_sleep_time = 0;
        stats_valid = false;
        stats_started = true;
    } else {
        stats_started = false;
    }
    int_unlock(lock);

#ifdef SLEEP_STATS_TRACE
    if (stats_interval_ms && trace_interval_ms) {
        stats_trace_interval = MS_TO_TICKS(trace_interval_ms);
    } else {
        stats_trace_interval = 0;
    }
#endif
}

int hal_sleep_get_stats(struct CPU_USAGE_T *usage)
{
    int ret;
    uint32_t lock;

    lock = int_lock();
    if (stats_valid) {
        usage->light_sleep = light_sleep_ratio;
        usage->sys_deep_sleep = sys_deep_sleep_ratio;
        usage->chip_deep_sleep = chip_deep_sleep_ratio;
        usage->busy = 100 - (light_sleep_ratio + sys_deep_sleep_ratio + chip_deep_sleep_ratio);
        ret = 0;
    } else {
        ret = 1;
    }
    int_unlock(lock);

    return ret;
}



int hal_sleep_set_sleep_hook(enum HAL_SLEEP_HOOK_USER_T user, HAL_SLEEP_HOOK_HANDLER handler)
{
    if (user >= ARRAY_SIZE(sleep_hook_handler)) {
        return 1;
    }
    sleep_hook_handler[user] = handler;
    return 0;
}

static int SRAM_TEXT_LOC hal_sleep_exec_sleep_hook(void)
{
    int i;
    int ret;

    for (i = 0; i < ARRAY_SIZE(sleep_hook_handler); i++) {
        if (sleep_hook_handler[i]) {
            ret = sleep_hook_handler[i]();
            if (ret) {
                return ret;
            }
        }
    }

    return 0;
}

int hal_sleep_set_deep_sleep_hook(enum HAL_DEEP_SLEEP_HOOK_USER_T user, HAL_DEEP_SLEEP_HOOK_HANDLER handler)
{
    if (user >= ARRAY_SIZE(deep_sleep_hook_handler)) {
        return 1;
    }
    deep_sleep_hook_handler[user] = handler;
    return 0;
}

static int SRAM_TEXT_LOC hal_sleep_exec_deep_sleep_hook(void)
{
    int i;
    int ret;

    for (i = 0; i < ARRAY_SIZE(deep_sleep_hook_handler); i++) {
        if (deep_sleep_hook_handler[i]) {
            ret = deep_sleep_hook_handler[i]();
            if (ret) {
                return ret;
            }
        }
    }

    return 0;
}

int SRAM_TEXT_LOC hal_sleep_irq_pending(void)
{
#if defined(__GIC_PRESENT) && (__GIC_PRESENT)
    int i;

    for (i = 0; i < (USER_IRQn_QTY + 31) / 32; i++) {
        if (GICDistributor->ICPENDR[i] & GICDistributor->ISENABLER[i]) {
            return 1;
        }
    }
#else
#if 0
    int i;

    for (i = 0; i < (USER_IRQn_QTY + 31) / 32; i++) {
        if (NVIC->ICPR[i] & NVIC->ISER[i]) {
            return 1;
        }
    }
#else
    // If there is any pending and enabled exception (including sysTick)
    if (SCB->ICSR & SCB_ICSR_VECTPENDING_Msk) {
        return 1;
    }
#endif
#endif

    return 0;
}

int SRAM_TEXT_LOC hal_sleep_specific_irq_pending(const uint32_t *irq, uint32_t cnt)
{
    int i;
    uint32_t check_cnt;

    check_cnt = (USER_IRQn_QTY + 31) / 32;
    if (check_cnt > cnt) {
        check_cnt = cnt;
    }

#if defined(__GIC_PRESENT) && (__GIC_PRESENT)
    for (i = 0; i < check_cnt; i++) {
        if (GICDistributor->ICPENDR[i] & GICDistributor->ISENABLER[i] & irq[i]) {
            return 1;
        }
    }
#else
    for (i = 0; i < check_cnt; i++) {
        if (NVIC->ICPR[i] & NVIC->ISER[i] & irq[i]) {
            return 1;
        }
    }
#endif

    return 0;
}



static enum HAL_SLEEP_STATUS_T SRAM_TEXT_LOC hal_sleep_lowpower_mode(void)
{
    enum HAL_SLEEP_STATUS_T ret;
    enum HAL_CMU_LPU_SLEEP_MODE_T mode;
    uint32_t prev_time = 0;
    uint32_t cur_time;
    uint32_t interval;

    ret = HAL_SLEEP_STATUS_LIGHT;

    // Deep sleep hook
    if (hal_sleep_exec_deep_sleep_hook() || hal_trace_busy()) {
        return ret;
    }

    if (chip_wake_lock_map) {
        mode = HAL_CMU_LPU_SLEEP_MODE_SYS;
    } else {
        mode = HAL_CMU_LPU_SLEEP_MODE_CHIP;
    }

    // Stop modules (except for psram and flash, spi)
    hal_gpadc_sleep();
    if (chip_wake_lock_map == 0) {
        analog_sleep();
        pmu_sleep();
    }
    // End of stopping modules


    hal_psram_sleep();
    hal_norflash_sleep(HAL_FLASH_ID_0);


    if (!hal_sleep_irq_pending()) {
        if (stats_started) {
            prev_time = hal_sys_timer_get();
        }
        hal_cmu_lpu_sleep(mode);
        if (stats_started) {
            cur_time = hal_sys_timer_get();
            interval = cur_time - prev_time;
            if (chip_wake_lock_map) {
                sys_deep_sleep_time += interval;
            } else {
                chip_deep_sleep_time += interval;
            }
        }
        ret = HAL_SLEEP_STATUS_DEEP;
    }

    hal_norflash_wakeup(HAL_FLASH_ID_0);
    hal_psram_wakeup();


    if (chip_wake_lock_map == 0) {
        pmu_wakeup();
        analog_wakeup();
    }

    hal_gpadc_wakeup();

    // End of restoring modules


    return ret;
}

// GCC has trouble in detecting static function usage in embedded ASM statements.
// The following function might be optimized away if there is no explicted call in C codes.
// Specifying "used" (or "noclone") attribute on the function can avoid the mistaken optimization.
static enum HAL_SLEEP_STATUS_T SRAM_TEXT_LOC NOINLINE USED hal_sleep_proc(int light_sleep)
{
    enum HAL_SLEEP_STATUS_T ret;
    uint32_t prev_time = 0;
    uint32_t cur_time;
    uint32_t interval;
    bool lpu_busy = false;
    bool dma_busy = false;
    POSSIBLY_UNUSED bool trace_busy = false;

    ret = HAL_SLEEP_STATUS_LIGHT;

    // Check the sleep conditions in interrupt-locked context
    if (cpu_wake_lock_map || (lpu_busy = hal_cmu_lpu_busy())) {
        // Cannot sleep
    } else {
        // Sleep hook
        if (hal_sleep_exec_sleep_hook()) {
            goto _exit_sleep;
        }

        if (sys_wake_lock_map || hal_sysfreq_busy() || (dma_busy = hal_dma_busy())) {
            // Light sleep

            if (stats_started) {
                prev_time = hal_sys_timer_get();
            }

#ifdef NO_LIGHT_SLEEP
            // WFI during USB ISO transfer might generate very weak (0.1 mV) 1K tone interference ???
            while (!hal_sleep_irq_pending());
#else
#ifndef __ARM_ARCH_ISA_ARM
            SCB->SCR = 0;
#endif
            __DSB();
            __WFI();
#endif

            if (stats_started) {
                cur_time = hal_sys_timer_get();
                light_sleep_time += cur_time - prev_time;
            }
#ifdef DEBUG
        } else if ((trace_busy = hal_trace_busy())) {
            // Light sleep with trace busy only

            if (stats_started) {
                prev_time = hal_sys_timer_get();
            }

            // No irq will be generated when trace becomes idle, so the trace status should
            // be kept polling actively instead of entering WFI
            while (!hal_sleep_irq_pending() && hal_trace_busy());

            if (stats_started) {
                cur_time = hal_sys_timer_get();
                light_sleep_time += cur_time - prev_time;
            }

            if (!hal_sleep_irq_pending()) {
                goto _deep_sleep;
            }
#endif
        } else {
            // Deep sleep

_deep_sleep: POSSIBLY_UNUSED;

            if (light_sleep) {
                ret = HAL_SLEEP_STATUS_DEEP;
            } else {
                ret = hal_sleep_lowpower_mode();
            }
        }
    }

_exit_sleep:
    if (stats_started) {
        cur_time = hal_sys_timer_get();
        interval = cur_time - stats_start_time;
        if (interval >= stats_interval) {
            if (light_sleep_time > UINT32_MAX / 100) {
                light_sleep_ratio = (uint64_t)light_sleep_time * 100 / interval;
            } else {
                light_sleep_ratio = light_sleep_time * 100 / interval;
            }
            if (sys_deep_sleep_time > UINT32_MAX / 100) {
                sys_deep_sleep_ratio = (uint64_t)sys_deep_sleep_time * 100 / interval;
            } else {
                sys_deep_sleep_ratio = sys_deep_sleep_time * 100 / interval;
            }
            if (chip_deep_sleep_time > UINT32_MAX / 100) {
                chip_deep_sleep_ratio = (uint64_t)chip_deep_sleep_time * 100 / interval;
            } else {
                chip_deep_sleep_ratio = chip_deep_sleep_time * 100 / interval;
            }
            stats_valid = true;
            light_sleep_time = 0;
            sys_deep_sleep_time = 0;
            chip_deep_sleep_time = 0;
            stats_start_time = cur_time;
        }
#ifdef SLEEP_STATS_TRACE
        if (stats_valid && stats_trace_interval) {
            if (cur_time - stats_trace_time >= stats_trace_interval) {
                TRACE(0, "CPU USAGE: busy=%d light=%d sys_deep=%d chip_deep=%d",
                    100 - (light_sleep_ratio + sys_deep_sleep_ratio + chip_deep_sleep_ratio),
                    light_sleep_ratio, sys_deep_sleep_ratio, chip_deep_sleep_ratio);
                stats_trace_time = cur_time;
#ifdef DEBUG_SLEEP_USER
                TRACE(0, "SLEEP USER: cpu=0x%X sys=0x%X chip=0x%X irq=0x%08X_%08X",
                    cpu_wake_lock_map, sys_wake_lock_map, chip_wake_lock_map,
                    (NVIC->ICPR[1] & NVIC->ISER[1]), (NVIC->ICPR[0] & NVIC->ISER[0]));
                TRACE(0, "BUSY: LPU=%d DMA=%d TRACE=%d", lpu_busy, dma_busy, trace_busy);
                hal_sysfreq_print_user_freq();
                hal_dma_print_busy_chan();
#endif
            }
        }
#endif
    }

    return ret;
}

#ifndef __ARM_ARCH_ISA_ARM
static enum HAL_SLEEP_STATUS_T SRAM_TEXT_LOC NOINLINE USED NAKED hal_sleep_deep_sleep_wrapper(void)
{
    asm volatile(
        "push {r4, lr} \n"
        // Switch current stack pointer to MSP
        "mrs r4, control \n"
        "bic r4, #2 \n"
        "msr control, r4 \n"
        "isb \n"
        "movs r0, #0 \n"
        "bl hal_sleep_proc \n"
        // Switch current stack pointer back to PSP
        "orr r4, #2 \n"
        "msr control, r4 \n"
        "isb \n"
        "pop {r4, pc} \n"
        );

#ifndef __ARMCC_VERSION
    return HAL_SLEEP_STATUS_LIGHT;
#endif
}
#endif

enum HAL_SLEEP_STATUS_T SRAM_TEXT_LOC hal_sleep_enter_sleep(void)
{
    enum HAL_SLEEP_STATUS_T ret;
    uint32_t lock;

    ret = HAL_SLEEP_STATUS_LIGHT;

#ifdef NO_SLEEP
    return ret;
#endif

    lock = int_lock_global();

#ifndef __ARM_ARCH_ISA_ARM
    if (__get_CONTROL() & 0x02) {
        ret = hal_sleep_deep_sleep_wrapper();
    } else
#endif
    {
        ret = hal_sleep_proc(false);
    }

    int_unlock_global(lock);

    return ret;
}

enum HAL_SLEEP_STATUS_T SRAM_TEXT_LOC hal_sleep_light_sleep(void)
{
    enum HAL_SLEEP_STATUS_T ret;
    uint32_t lock;

    ret = HAL_SLEEP_STATUS_LIGHT;

#ifdef NO_SLEEP
    return ret;
#endif

    lock = int_lock_global();

    ret = hal_sleep_proc(true);

    int_unlock_global(lock);

    return ret;
}

int hal_cpu_wake_lock(enum HAL_CPU_WAKE_LOCK_USER_T user)
{
    uint32_t lock;

    if (user >= HAL_CPU_WAKE_LOCK_USER_QTY) {
        return 1;
    }

    lock = int_lock();
    cpu_wake_lock_map |= (1 << user);
    int_unlock(lock);

    return 0;
}

int hal_cpu_wake_unlock(enum HAL_CPU_WAKE_LOCK_USER_T user)
{
    uint32_t lock;

    if (user >= HAL_CPU_WAKE_LOCK_USER_QTY) {
        return 1;
    }

    lock = int_lock();
    cpu_wake_lock_map &= ~(1 << user);
    int_unlock(lock);

    return 0;
}

int hal_sys_wake_lock(enum HAL_SYS_WAKE_LOCK_USER_T user)
{
    uint32_t lock;

    if (user >= HAL_SYS_WAKE_LOCK_USER_QTY) {
        return 1;
    }

    lock = int_lock();
    sys_wake_lock_map |= (1 << user);
    int_unlock(lock);

    return 0;
}

int hal_sys_wake_unlock(enum HAL_SYS_WAKE_LOCK_USER_T user)
{
    uint32_t lock;

    if (user >= HAL_SYS_WAKE_LOCK_USER_QTY) {
        return 1;
    }

    lock = int_lock();
    sys_wake_lock_map &= ~(1 << user);
    int_unlock(lock);

    return 0;
}

int hal_chip_wake_lock(enum HAL_CHIP_WAKE_LOCK_USER_T user)
{
    uint32_t lock;

    if (user >= HAL_CHIP_WAKE_LOCK_USER_QTY) {
        return 1;
    }

    lock = int_lock();
    chip_wake_lock_map |= (1 << user);
    int_unlock(lock);

    return 0;
}

int hal_chip_wake_unlock(enum HAL_CHIP_WAKE_LOCK_USER_T user)
{
    uint32_t lock;

    if (user >= HAL_CHIP_WAKE_LOCK_USER_QTY) {
        return 1;
    }

    lock = int_lock();
    chip_wake_lock_map &= ~(1 << user);
    int_unlock(lock);

    return 0;
}

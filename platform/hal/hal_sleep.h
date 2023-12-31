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
#ifndef __HAL_SLEEP_H__
#define __HAL_SLEEP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

enum HAL_CPU_WAKE_LOCK_USER_T {
    HAL_CPU_WAKE_LOCK_USER_RTOS,
    HAL_CPU_WAKE_LOCK_USER_EXTERNAL,
    HAL_CPU_WAKE_LOCK_USER_AUDIOFLINGER,
    HAL_CPU_WAKE_LOCK_USER_3,
    HAL_CPU_WAKE_LOCK_USER_4,
    HAL_CPU_WAKE_LOCK_USER_5,
    HAL_CPU_WAKE_LOCK_USER_6,
    HAL_CPU_WAKE_LOCK_USER_7,
    HAL_CPU_WAKE_LOCK_USER_8,
    HAL_CPU_WAKE_LOCK_USER_9,
    HAL_CPU_WAKE_LOCK_USER_10,
    HAL_CPU_WAKE_LOCK_USER_11,
    HAL_CPU_WAKE_LOCK_USER_12,
    HAL_CPU_WAKE_LOCK_USER_13,
    HAL_CPU_WAKE_LOCK_USER_14,
    HAL_CPU_WAKE_LOCK_USER_15,
    HAL_CPU_WAKE_LOCK_USER_16,
    HAL_CPU_WAKE_LOCK_USER_17,
    HAL_CPU_WAKE_LOCK_USER_18,
    HAL_CPU_WAKE_LOCK_USER_19,
    HAL_CPU_WAKE_LOCK_USER_20,
    HAL_CPU_WAKE_LOCK_USER_21,
    HAL_CPU_WAKE_LOCK_USER_22,
    HAL_CPU_WAKE_LOCK_USER_23,
    HAL_CPU_WAKE_LOCK_USER_24,
    HAL_CPU_WAKE_LOCK_USER_25,
    HAL_CPU_WAKE_LOCK_USER_26,
    HAL_CPU_WAKE_LOCK_USER_27,
    HAL_CPU_WAKE_LOCK_USER_28,
    HAL_CPU_WAKE_LOCK_USER_29,
    HAL_CPU_WAKE_LOCK_USER_30,
    HAL_CPU_WAKE_LOCK_USER_31,

    HAL_CPU_WAKE_LOCK_USER_QTY
};

enum HAL_SYS_WAKE_LOCK_USER_T {
    HAL_SYS_WAKE_LOCK_USER_INTERSYS,
    HAL_SYS_WAKE_LOCK_USER_INTERSYS_HCI,
    HAL_SYS_WAKE_LOCK_USER_2,
    HAL_SYS_WAKE_LOCK_USER_3,
    HAL_SYS_WAKE_LOCK_USER_4,
    HAL_SYS_WAKE_LOCK_USER_TRANSQ,
    HAL_SYS_WAKE_LOCK_USER_6,
    HAL_SYS_WAKE_LOCK_USER_7,
    HAL_SYS_WAKE_LOCK_USER_8,
    HAL_SYS_WAKE_LOCK_USER_9,
    HAL_SYS_WAKE_LOCK_USER_10,
    HAL_SYS_WAKE_LOCK_USER_11,
    HAL_SYS_WAKE_LOCK_USER_12,
    HAL_SYS_WAKE_LOCK_USER_13,
    HAL_SYS_WAKE_LOCK_USER_14,
    HAL_SYS_WAKE_LOCK_USER_15,
    HAL_SYS_WAKE_LOCK_USER_16,
    HAL_SYS_WAKE_LOCK_USER_17,
    HAL_SYS_WAKE_LOCK_USER_18,
    HAL_SYS_WAKE_LOCK_USER_19,
    HAL_SYS_WAKE_LOCK_USER_20,
    HAL_SYS_WAKE_LOCK_USER_21,
    HAL_SYS_WAKE_LOCK_USER_22,
    HAL_SYS_WAKE_LOCK_USER_23,
    HAL_SYS_WAKE_LOCK_USER_24,
    HAL_SYS_WAKE_LOCK_USER_25,
    HAL_SYS_WAKE_LOCK_USER_26,
    HAL_SYS_WAKE_LOCK_USER_27,
    HAL_SYS_WAKE_LOCK_USER_28,
    HAL_SYS_WAKE_LOCK_USER_29,
    HAL_SYS_WAKE_LOCK_USER_30,
    HAL_SYS_WAKE_LOCK_USER_31,

    HAL_SYS_WAKE_LOCK_USER_QTY
};

enum HAL_CHIP_WAKE_LOCK_USER_T {
    HAL_CHIP_WAKE_LOCK_USER_ANC,
    HAL_CHIP_WAKE_LOCK_USER_1,
    HAL_CHIP_WAKE_LOCK_USER_2,
    HAL_CHIP_WAKE_LOCK_USER_3,
    HAL_CHIP_WAKE_LOCK_USER_4,
    HAL_CHIP_WAKE_LOCK_USER_5,
    HAL_CHIP_WAKE_LOCK_USER_6,
    HAL_CHIP_WAKE_LOCK_USER_7,
    HAL_CHIP_WAKE_LOCK_USER_8,
    HAL_CHIP_WAKE_LOCK_USER_9,
    HAL_CHIP_WAKE_LOCK_USER_10,
    HAL_CHIP_WAKE_LOCK_USER_11,
    HAL_CHIP_WAKE_LOCK_USER_12,
    HAL_CHIP_WAKE_LOCK_USER_13,
    HAL_CHIP_WAKE_LOCK_USER_14,
    HAL_CHIP_WAKE_LOCK_USER_15,
    HAL_CHIP_WAKE_LOCK_USER_16,
    HAL_CHIP_WAKE_LOCK_USER_17,
    HAL_CHIP_WAKE_LOCK_USER_18,
    HAL_CHIP_WAKE_LOCK_USER_19,
    HAL_CHIP_WAKE_LOCK_USER_20,
    HAL_CHIP_WAKE_LOCK_USER_21,
    HAL_CHIP_WAKE_LOCK_USER_22,
    HAL_CHIP_WAKE_LOCK_USER_23,
    HAL_CHIP_WAKE_LOCK_USER_24,
    HAL_CHIP_WAKE_LOCK_USER_25,
    HAL_CHIP_WAKE_LOCK_USER_26,
    HAL_CHIP_WAKE_LOCK_USER_27,
    HAL_CHIP_WAKE_LOCK_USER_28,
    HAL_CHIP_WAKE_LOCK_USER_29,
    HAL_CHIP_WAKE_LOCK_USER_30,
    HAL_CHIP_WAKE_LOCK_USER_31,

    HAL_CHIP_WAKE_LOCK_USER_QTY
};

enum HAL_SLEEP_HOOK_USER_T {
    HAL_SLEEP_HOOK_USER_NVRECORD = 0,
    HAL_SLEEP_HOOK_USER_OTA,
    HAL_SLEEP_HOOK_NORFLASH_API,
    HAL_SLEEP_HOOK_DUMP_LOG,
    HAL_SLEEP_HOOK_USER_QTY
};

enum HAL_DEEP_SLEEP_HOOK_USER_T {
    HAL_DEEP_SLEEP_HOOK_USER_WDT = 0,
    HAL_DEEP_SLEEP_HOOK_USER_NVRECORD,
    HAL_DEEP_SLEEP_HOOK_USER_OTA,
    HAL_DEEP_SLEEP_HOOK_NORFLASH_API,
    HAL_DEEP_SLEEP_HOOK_DUMP_LOG,
    HAL_DEEP_SLEEP_HOOK_USER_QTY
};

enum HAL_SLEEP_STATUS_T {
    HAL_SLEEP_STATUS_DEEP,
    HAL_SLEEP_STATUS_LIGHT,
};

struct CPU_USAGE_T {
    uint8_t busy;
    uint8_t light_sleep;
    uint8_t sys_deep_sleep;
    uint8_t chip_deep_sleep;
};

typedef int (*HAL_SLEEP_HOOK_HANDLER)(void);
typedef int (*HAL_DEEP_SLEEP_HOOK_HANDLER)(void);

int hal_sleep_irq_pending(void);

int hal_sleep_specific_irq_pending(const uint32_t *irq, uint32_t cnt);

enum HAL_SLEEP_STATUS_T hal_sleep_enter_sleep(void);

enum HAL_SLEEP_STATUS_T hal_sleep_light_sleep(void);

int hal_sleep_set_sleep_hook(enum HAL_SLEEP_HOOK_USER_T user, HAL_SLEEP_HOOK_HANDLER handler);

int hal_sleep_set_deep_sleep_hook(enum HAL_DEEP_SLEEP_HOOK_USER_T user, HAL_DEEP_SLEEP_HOOK_HANDLER handler);

int hal_cpu_wake_lock(enum HAL_CPU_WAKE_LOCK_USER_T user);

int hal_cpu_wake_unlock(enum HAL_CPU_WAKE_LOCK_USER_T user);

int hal_sys_wake_lock(enum HAL_SYS_WAKE_LOCK_USER_T user);

int hal_sys_wake_unlock(enum HAL_SYS_WAKE_LOCK_USER_T user);

int hal_chip_wake_lock(enum HAL_CHIP_WAKE_LOCK_USER_T user);

int hal_chip_wake_unlock(enum HAL_CHIP_WAKE_LOCK_USER_T user);

void hal_sleep_start_stats(uint32_t stats_interval_ms, uint32_t trace_interval_ms);

int hal_sleep_get_stats(struct CPU_USAGE_T *usage);

#ifdef __cplusplus
}
#endif

#endif

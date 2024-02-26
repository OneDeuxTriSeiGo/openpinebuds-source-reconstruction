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
#include "plat_types.h"
#include "hal_sysfreq.h"
#include "hal_location.h"
#include "hal_trace.h"
#include "cmsis.h"

#if !(defined(ROM_BUILD) || defined(CHIP_SUBSYS_SENS) || \
#define PMU_CTRL_ENABLE
#endif

#ifdef PMU_CTRL_ENABLE
#include "pmu.h"
#endif

static uint32_t BOOT_BSS_LOC sysfreq_bundle[(HAL_SYSFREQ_USER_QTY + 3) / 4];

static uint8_t * const sysfreq_per_user = (uint8_t *)&sysfreq_bundle[0];

static enum HAL_SYSFREQ_USER_T BOOT_DATA_LOC top_user = HAL_SYSFREQ_USER_QTY;

static enum HAL_CMU_FREQ_T BOOT_DATA_LOC min_sysfreq = HAL_CMU_FREQ_32K;

static enum HAL_CMU_FREQ_T hal_sysfreq_revise_freq(enum HAL_CMU_FREQ_T freq)
{
    if (freq == HAL_CMU_FREQ_32K) {
        freq = HAL_CMU_FREQ_26M;
    }
    return (freq > min_sysfreq) ? freq : min_sysfreq;
}

void hal_sysfreq_set_min_freq(enum HAL_CMU_FREQ_T freq)
{
    uint32_t lock;

    lock = int_lock();

    if (min_sysfreq < freq) {
        min_sysfreq = freq;
        if (min_sysfreq > hal_sysfreq_get()) {
            hal_cmu_sys_set_freq(min_sysfreq);
        }
    }

    int_unlock(lock);
}

int hal_sysfreq_req(enum HAL_SYSFREQ_USER_T user, enum HAL_CMU_FREQ_T freq)
{
    uint32_t lock;
    enum HAL_CMU_FREQ_T cur_freq;
    enum HAL_CMU_FREQ_T real_freq;
    enum HAL_CMU_FREQ_T real_cur_freq;
    int i;

    if (user >= HAL_SYSFREQ_USER_QTY) {
        return 1;
    }
    if (freq >= HAL_CMU_FREQ_QTY) {
        return 2;
    }

    lock = int_lock();

    cur_freq = hal_sysfreq_get();

    sysfreq_per_user[user] = freq;

    if (freq == cur_freq) {
        top_user = user;
    } else if (freq > cur_freq) {
        top_user = user;
        real_freq = hal_sysfreq_revise_freq(freq);
        real_cur_freq = hal_sysfreq_revise_freq(cur_freq);
        if (real_freq != real_cur_freq) {
#ifdef PMU_CTRL_ENABLE
            pmu_sys_freq_config(real_freq);
#endif
#ifdef ULTRA_LOW_POWER
            // Enable PLL if required
            hal_cmu_low_freq_mode_disable(real_cur_freq, real_freq);
#endif
            hal_cmu_sys_set_freq(real_freq);
    } else /* if (freq < cur_freq) */ {
        if (top_user == user || top_user == HAL_SYSFREQ_USER_QTY) {
            if (top_user == user) {
                freq = sysfreq_per_user[0];
                user = 0;
                for (i = 1; i < HAL_SYSFREQ_USER_QTY; i++) {
                    if (freq < sysfreq_per_user[i]) {
                        freq = sysfreq_per_user[i];
                        user = i;
                    }
                }
            }
            top_user = user;
            if (freq != cur_freq) {
                real_freq = hal_sysfreq_revise_freq(freq);
                real_cur_freq = hal_sysfreq_revise_freq(cur_freq);
#ifdef ULTRA_LOW_POWER
                    // Disable PLL if capable
                    hal_cmu_low_freq_mode_enable(real_cur_freq, real_freq);
#endif
#ifdef PMU_CTRL_ENABLE
                    pmu_sys_freq_config(real_freq);
#endif
            }
        }
    }

    int_unlock(lock);

    return 0;
}

enum HAL_CMU_FREQ_T hal_sysfreq_get(void)
{
    if (top_user < HAL_SYSFREQ_USER_QTY) {
        return sysfreq_per_user[top_user];
    } else {
        return hal_cmu_sys_get_freq();
    }
}

enum HAL_CMU_FREQ_T hal_sysfreq_get_hw_freq(void)
{
    if (top_user < HAL_SYSFREQ_USER_QTY) {
        return hal_sysfreq_revise_freq(sysfreq_per_user[top_user]);
    } else {
        return hal_cmu_sys_get_freq();
    }
}

int hal_sysfreq_busy(void)
{
    int i;

    for (i = 0; i < ARRAY_SIZE(sysfreq_bundle); i++) {
        if (sysfreq_bundle[i] != 0) {
            return 1;
        }
    }

    return 0;
}

void hal_sysfreq_print_user_freq(void)
{
    int i;

    for (i = 0; i < HAL_SYSFREQ_USER_QTY; i++) {
        if (sysfreq_per_user[i] != 0) {
            TRACE(TR_ATTR_NO_TS,"\t[%2u] f=%2u", i, sysfreq_per_user[i]);
        }
    }
    TRACE(TR_ATTR_NO_TS,"\ttop_user=%2u", top_user);
}

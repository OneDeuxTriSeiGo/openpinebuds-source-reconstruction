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
#include "plat_addr_map.h"
#include "hal_pwm.h"
#include "reg_pwm.h"
#include "cmsis.h"
#include "hal_cmu.h"
#include "hal_timer.h"
#include "hal_trace.h"

#define PWM_SLOW_CLOCK                  (CONFIG_SYSTICK_HZ)
#define PWM_FAST_CLOCK                  (hal_cmu_get_crystal_freq() / 2)
#define PWM_MAX_VALUE                   0xFFFF

// Max allowed PWM freqency error in percentage
#define PWM_MAX_FREQ_ERR_PCT            5

static struct PWM_T * const pwm[] = {
    (struct PWM_T *)PWM_BASE,
#if defined(CHIP_BEST2000)
    (struct PWM_T *)PWM1_BASE,
#endif
};

static const enum HAL_CMU_MOD_ID_T pwm_o_mod[] = {
    HAL_CMU_MOD_O_PWM0,
#if defined(CHIP_BEST2000)
    HAL_CMU_MOD_O_PWM4,
#endif
};

static const enum HAL_CMU_MOD_ID_T pwm_p_mod[] = {
    HAL_CMU_MOD_P_PWM,
#if defined(CHIP_BEST2000)
    HAL_CMU_MOD_P_PWM1,
#endif
};

int hal_pwm_enable(enum HAL_PWM_ID_T id, const struct HAL_PWM_CFG_T *cfg)
{
    uint32_t mod_freq;
    uint32_t load;
    uint32_t toggle;
    uint32_t lock;
    uint8_t ratio;
    uint8_t index;
    uint8_t offset;

    if (id >= HAL_PWM_ID_QTY) {
        return 1;
    }
    if (cfg->ratio > 100) {
        return 2;
    }

    if (cfg->inv && (cfg->ratio == 0 || cfg->ratio == 100)) {
        ratio = 100 - cfg->ratio;
    } else {
        ratio = cfg->ratio;
    }

#ifdef PWM_TRY_SLOW_CLOCK
    mod_freq = PWM_SLOW_CLOCK;
#else
    if (cfg->sleep_on) {
        mod_freq = PWM_SLOW_CLOCK;
    } else {
        mod_freq = PWM_FAST_CLOCK;
    }
#endif

    if (ratio == 100) {
        load = PWM_MAX_VALUE;
        toggle = PWM_MAX_VALUE;
    } else if (ratio == 0) {
        load = 0;
        toggle = 0;
    } else {
        load = mod_freq / cfg->freq;
        toggle = load * ratio / 100;
        if (toggle == 0) {
            toggle = 1;
        }
#ifdef PWM_TRY_SLOW_CLOCK
        // Check PWM frequency error in percentage
        if (!cfg->sleep_on && ABS((int)(toggle * 100 - load * ratio)) > load * PWM_MAX_FREQ_ERR_PCT) {
            mod_freq = PWM_FAST_CLOCK;
            load = mod_freq / cfg->freq;
            toggle = load * ratio / 100;
        }
#endif
        load = PWM_MAX_VALUE + 1 - load;
        toggle = PWM_MAX_VALUE - toggle;
    }

#if defined(CHIP_BEST2000)
    if (id < HAL_PWM1_ID_0) {
        index = 0;
        offset = id - HAL_PWM_ID_0;
    } else {
        index = 1;
        offset = id - HAL_PWM1_ID_0;
    }
#else
    index = 0;
    offset = id - HAL_PWM_ID_0;
#endif

    lock = int_lock();

    if ((pwm_map & (1 << id)) == 0) {
        pwm_map |= (1 << id);
        hal_cmu_clock_enable(pwm_o_mod[index] + offset);
        hal_cmu_clock_enable(pwm_p_mod[index]);
        hal_cmu_reset_clear(pwm_o_mod[index] + offset);
        hal_cmu_reset_clear(pwm_p_mod[index]);
    } else {
        pwm[index]->EN &= ~(1 << offset);
    }

    if (ratio == 0) {
        // Output 0 when disabled
        goto _exit;
    }

    hal_cmu_pwm_set_freq(id, (mod_freq == PWM_FAST_CLOCK) ? mod_freq : 0);

    if (offset == 0) {
        pwm[index]->LOAD01 = SET_BITFIELD(pwm[index]->LOAD01, PWM_LOAD01_0, load);
        pwm[index]->TOGGLE01 = SET_BITFIELD(pwm[index]->TOGGLE01, PWM_TOGGLE01_0, toggle);
    } else if (offset == 1) {
        pwm[index]->LOAD01 = SET_BITFIELD(pwm[index]->LOAD01, PWM_LOAD01_1, load);
        pwm[index]->TOGGLE01 = SET_BITFIELD(pwm[index]->TOGGLE01, PWM_TOGGLE01_1, toggle);
    } else if (offset == 2) {
        pwm[index]->LOAD23 = SET_BITFIELD(pwm[index]->LOAD23, PWM_LOAD23_2, load);
        pwm[index]->TOGGLE23 = SET_BITFIELD(pwm[index]->TOGGLE23, PWM_TOGGLE23_2, toggle);
    } else {
        pwm[index]->LOAD23 = SET_BITFIELD(pwm[index]->LOAD23, PWM_LOAD23_3, load);
        pwm[index]->TOGGLE23 = SET_BITFIELD(pwm[index]->TOGGLE23, PWM_TOGGLE23_3, toggle);
    }

    if (cfg->inv) {
        pwm[index]->INV |= (1 << offset);
    } else {
        pwm[index]->INV &= ~(1 << offset);
    }

    pwm[index]->EN |= (1 << offset);

_exit:
    int_unlock(lock);

    return 0;
}

int hal_pwm_disable(enum HAL_PWM_ID_T id)
{
    uint8_t index;
    uint8_t offset;

    if (id >= HAL_PWM_ID_QTY) {
        return 1;
    }

#if defined(CHIP_BEST2000)
    if (id < HAL_PWM1_ID_0) {
        index = 0;
        offset = id - HAL_PWM_ID_0;
    } else {
        index = 1;
        offset = id - HAL_PWM1_ID_0;
    }
#else
    index = 0;
    offset = id - HAL_PWM_ID_0;
#endif

    if (hal_cmu_reset_get_status(pwm_o_mod[index] + offset) == HAL_CMU_RST_SET) {
        hal_cmu_clock_enable(pwm_o_mod[index] + offset);
        hal_cmu_clock_enable(pwm_p_mod[index]);
        hal_cmu_reset_clear(pwm_o_mod[index] + offset);
        hal_cmu_reset_clear(pwm_p_mod[index]);
    } else {
        pwm[index]->EN &= ~(1 << offset);
    }

    return 0;
}

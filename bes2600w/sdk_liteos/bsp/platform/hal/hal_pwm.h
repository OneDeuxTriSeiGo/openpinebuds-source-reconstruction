/*
 * Copyright (c) 2021 bestechnic (Shanghai) Technologies CO., LIMITED.
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
#ifndef __HAL_PWM_H__
#define __HAL_PWM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "hal_cmu.h"

struct HAL_PWM_CFG_T {
    uint32_t freq;
    uint8_t ratio;
    bool inv;
    bool sleep_on;
};

struct HAL_PWM_BR_CFG_T {
    uint16_t off_time_ms;
    uint16_t on_time_ms;
    uint16_t fade_time_ms;
};

int hal_pwm_enable(enum HAL_PWM_ID_T id, const struct HAL_PWM_CFG_T *cfg);

int hal_pwm_disable(enum HAL_PWM_ID_T id);

int hal_pwm_breathing_led_enable(enum HAL_PWM_ID_T id, const struct HAL_PWM_BR_CFG_T *cfg);

int hal_pwm_breathing_led_disable(enum HAL_PWM_ID_T id);

#ifdef __cplusplus
}
#endif

#endif


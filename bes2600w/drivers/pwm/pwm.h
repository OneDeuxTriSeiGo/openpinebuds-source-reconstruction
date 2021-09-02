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
#ifndef __PWM_H__
#define __PWM_H__

#include "hal_pwm.h"
#include "hal_gpio.h"
#include "hal_iomux.h"
#include "pwm_core.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct PwmResource {
    uint32_t pwm_pin;
    uint32_t pwm_id;
};

struct PwmDevice {
    struct IDeviceIoService ioService;
    struct HAL_PWM_CFG_T pwm_cfg;
    struct PwmConfig *cfg;
    struct PwmResource resource;
};

#ifdef __cplusplus
}
#endif
#endif
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

#ifndef _I2C_H__
#define _I2C_H__

#include "hal_i2c.h"
#include "hal_iomux.h"
#include "hal_iomux_best2003.h"
#include "device_resource_if.h"
#include "cmsis_os2.h"
#include "cmsis_os.h"
#ifdef __cplusplus
extern "C" {
#endif

struct I2cResource {
    uint32_t port;
    uint32_t mode;
    uint32_t scl_pin;
    uint32_t sda_pin;
    uint32_t use_sync;
    uint32_t use_dma;
    uint32_t as_master;
    uint32_t speed;
    uint32_t addr_as_slave;
    uint32_t address_width;
};

struct I2cDevice {
    uint16_t dev_addr;      /**< slave device addr */
    uint32_t address_width; /**< Addressing mode: 7 bit or 10 bit */
    osMutexId mutex;
    uint32_t port;
    struct HAL_I2C_CONFIG_T i2c_cfg;
    struct I2cResource resource;
};

void littos_i2c_test(void);

#ifdef __cplusplus
}
#endif

#endif
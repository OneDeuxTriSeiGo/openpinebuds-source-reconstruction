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
#ifndef __HAL_SLAVE_I2C_H__
#define __HAL_SLAVE_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

void hal_slave_i2c_enable(void);

void hal_slave_i2c_disable(void);

uint32_t hal_slave_i2c_get_filter_len(void);

int hal_slave_i2c_set_filter_len(uint32_t len);

uint32_t hal_slave_i2c_get_dev_id(void);

int hal_slave_i2c_set_dev_id(uint32_t dev_id);

void hal_slave_i2c_bypass_timeout(void);

void hal_slave_i2c_restore_timeout(void);

#ifdef __cplusplus
}
#endif

#endif

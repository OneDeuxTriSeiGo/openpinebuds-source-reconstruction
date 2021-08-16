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
#ifndef __HAL_ANALOGIF_H__
#define __HAL_ANALOGIF_H__

#ifdef __cplusplus
extern "C" {
#endif

enum HAL_ANALOGIF_RF_ACCESS_T {
    HAL_ANALOGIF_RF_ACCESS_ALL,
    HAL_ANALOGIF_RF_ACCESS_NO_PAGE,
    HAL_ANALOGIF_RF_ACCESS_NONE,
};

#ifdef NO_ISPI

#include "cmsis.h"

__STATIC_FORCEINLINE int hal_analogif_open(void) { return 0; }

__STATIC_FORCEINLINE int hal_analogif_reg_write(unsigned short reg, unsigned short val) { return 0; }

__STATIC_FORCEINLINE int hal_analogif_reg_read(unsigned short reg, unsigned short *val) { *val = 0; return 0; }

__STATIC_FORCEINLINE enum HAL_ANALOGIF_RF_ACCESS_T hal_analogif_rf_access_config(enum HAL_ANALOGIF_RF_ACCESS_T type) { return HAL_ANALOGIF_RF_ACCESS_ALL; }

#else

int hal_analogif_open(void);

int hal_analogif_reg_write(unsigned short reg, unsigned short val);

int hal_analogif_reg_read(unsigned short reg, unsigned short *val);

enum HAL_ANALOGIF_RF_ACCESS_T hal_analogif_rf_access_config(enum HAL_ANALOGIF_RF_ACCESS_T type);

#endif

#ifdef __cplusplus
}
#endif

#endif


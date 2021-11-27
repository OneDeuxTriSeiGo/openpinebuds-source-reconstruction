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
#ifndef __HAL_MEMSC_H__
#define __HAL_MEMSC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

enum HAL_MEMSC_ID_T {
    HAL_MEMSC_ID_0,
    HAL_MEMSC_ID_1,
    HAL_MEMSC_ID_2,
    HAL_MEMSC_ID_3,

    HAL_MEMSC_ID_QTY
};

#define HAL_MEMSC_ID_TRACE              HAL_MEMSC_ID_0
#define HAL_MEMSC_ID_TRNG               HAL_MEMSC_ID_1

enum HAL_MEMSC_AON_ID_T {
    HAL_MEMSC_AON_ID_0,
    HAL_MEMSC_AON_ID_1,
    HAL_MEMSC_AON_ID_2,
    HAL_MEMSC_AON_ID_3,

    HAL_MEMSC_AON_ID_QTY
};

int hal_memsc_lock(enum HAL_MEMSC_ID_T id);

void hal_memsc_unlock(enum HAL_MEMSC_ID_T id);

bool hal_memsc_avail(enum HAL_MEMSC_ID_T id);

int hal_memsc_aon_lock(enum HAL_MEMSC_AON_ID_T id);

void hal_memsc_aon_unlock(enum HAL_MEMSC_AON_ID_T id);

bool hal_memsc_aon_avail(enum HAL_MEMSC_AON_ID_T id);

#ifdef __cplusplus
}
#endif

#endif


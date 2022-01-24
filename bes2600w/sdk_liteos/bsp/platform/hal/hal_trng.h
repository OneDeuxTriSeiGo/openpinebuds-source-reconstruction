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
#ifndef __HAL_TRNG_H__
#define __HAL_TRNG_H__

#include "plat_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_TRNG_DATA_LEN                   24

enum HAL_TRNG_RND_SRC_SEL
{
    HAL_TRNG_RND_SRC_SHORTEST = 0,
    HAL_TRNG_RND_SRC_SHORT,
    HAL_TRNG_RND_SRC_LONG,
    HAL_TRNG_RND_SRC_LONGEST,
};
typedef unsigned int HAL_TRNG_RND_SRC_SEL_ENUM;

struct HAL_TRNG_CFG_T
{
    uint32_t                    sample_cntr1; // must not be less than 0x11
    HAL_TRNG_RND_SRC_SEL_ENUM   rnd_src_sel;
};

typedef void (*HAL_TRNG_RUN_CB_T)(const uint32_t *buf, uint32_t len, uint32_t error);

int hal_trng_open(const struct HAL_TRNG_CFG_T *cfg);

void hal_trng_close(void);

/*
 * Attention: User random api should add lock around this function, and check the return value
 * If this function return error or timeout, you can increase sample_cntr1
 * TRNG module is stopped in the end of this function
 * Don't close TRNG module between twice hal_get_trngdata, or the result is NOT TRUE RNADOM
 * The power consumption of trng module is about 30uA@3.8V at best2003 after stopping
 */
int hal_get_trngdata(uint8_t *buf, uint32_t buf_len);

int hal_trng_run(HAL_TRNG_RUN_CB_T cb);

void hal_trng_cancel(void);

#ifdef __cplusplus
}
#endif

#endif //__HAL_TRNG_H__

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
#ifndef __REG_TRNG_H__
#define __REG_TRNG_H__

#include "plat_types.h"

struct TRNG_T
{
    uint32_t reserved1[0x40];           //0x0-0xfc
    __IO uint32_t RNG_IMR;              //0x100
    __I  uint32_t RNG_ISR;              //0x104
    __O  uint32_t RNG_ICR;              //0x108
    __IO uint32_t TRNG_CONFIG;          //0x10C
    __I  uint32_t TRNG_VALID;           //0x110
    __I  uint32_t EHR_DATA[6];          //0x114-0x128
    __IO uint32_t RND_SOURCE_ENABLE;    //0x12c
    __IO uint32_t SAMPLE_CNT1;          //0x130
    __IO uint32_t AUTOCORR_STATISTIC;   //0x134
    __I  uint32_t TRNG_DEBUG_CONTROL;   //0x138
    uint32_t reserved2;                 //0x13c
    __O  uint32_t TRNG_SW_RESET;        //0x140
    uint32_t reserved3[0x1D];           //0x144-0x1b4
    __I  uint32_t TRNG_BUSY;            //0x1b8
    __O  uint32_t RST_BITS_COUNTER;     //0x1bc
    uint32_t reserved4[0x8];            //0x1c0-0x1dc
    __I  uint32_t RNG_BIST_CNTR[3];     //0x1e0-0x1e8
};

// REG_110
#define TRNG_VALID_BIT              (1 << 0)

// REG_10C
#define RND_SRC_SEL_MASK            (0x3)

// REG_12C
#define RND_SOURCE_EN               (1 << 0)

// REG_1B8
#define TRNG_BUSY_BIT               (1 << 0)

#endif

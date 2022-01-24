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
#ifndef __REG_SLAVE_I2C_H__
#define __REG_SLAVE_I2C_H__

#include "plat_types.h"

struct SLAVE_I2C_T {
    __IO uint32_t EN;                       // 0x000
    __IO uint32_t ID;                       // 0x004
    __IO uint32_t SMP;                      // 0x008
    __IO uint32_t RESERVED_00C[5];          // 0x00C
    __IO uint32_t TBP;                      // 0x020
};

#define I2C_EN                              (1 << 0)
#define W_FILTERLEN_SHIFT                   7
#define W_FILTERLEN_MASK                    (0xFF << W_FILTERLEN_SHIFT)
#define W_FILTERLEN(n)                      BITFIELD_VAL(W_FILTERLEN, n)
#define R_FILTERLEN_SHIFT                   8
#define R_FILTERLEN_MASK                    (0xFF << R_FILTERLEN_SHIFT)
#define R_FILTERLEN(n)                      BITFIELD_VAL(R_FILTERLEN, n)

#define DEV_ID_SHIFT                        0
#define DEV_ID_MASK                         (0xFF << DEV_ID_SHIFT)
#define DEV_ID(n)                           BITFIELD_VAL(DEV_ID, n)

#define SMP_I2C_SEL_SHIFT                   0
#define SMP_I2C_SEL_MASK                    (0xF << SMP_I2C_SEL_SHIFT)
#define SMP_I2C_SEL(n)                      BITFIELD_VAL(SMP_I2C_SEL, n)

#define TIMEOUT_BYPASS                      (1 << 0)

#endif


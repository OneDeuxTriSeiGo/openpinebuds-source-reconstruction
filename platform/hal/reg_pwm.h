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
#ifndef __REG_PWM_H__
#define __REG_PWM_H__

#include "plat_types.h"

// PWM Registers
struct PWM_T
{
    __I  uint32_t ID;                       // 0x000
    __IO uint32_t EN;                       // 0x004
    __IO uint32_t INV;                      // 0x008
    __IO uint32_t PHASE01;                  // 0x00C
    __IO uint32_t PHASE23;                  // 0x010
    __IO uint32_t LOAD01;                   // 0x014
    __IO uint32_t LOAD23;                   // 0x018
    __IO uint32_t TOGGLE01;                 // 0x01C
    __IO uint32_t TOGGLE23;                 // 0x020
    __IO uint32_t PHASEMOD;                 // 0x024
};

#define PWM_EN_0                            (1 << 0)
#define PWM_EN_1                            (1 << 1)
#define PWM_EN_2                            (1 << 2)
#define PWM_EN_3                            (1 << 3)

#define PWM_INV_0                           (1 << 0)
#define PWM_INV_1                           (1 << 1)
#define PWM_INV_2                           (1 << 2)
#define PWM_INV_3                           (1 << 3)

#define PWM_PHASE01_0(n)                    (((n) & 0xFFFF) << 0)
#define PWM_PHASE01_0_MASK                  (0xFFFF << 0)
#define PWM_PHASE01_0_SHIFT                 (0)
#define PWM_PHASE01_1(n)                    (((n) & 0xFFFF) << 16)
#define PWM_PHASE01_1_MASK                  (0xFFFF << 16)
#define PWM_PHASE01_1_SHIFT                 (16)

#define PWM_PHASE23_2(n)                    (((n) & 0xFFFF) << 0)
#define PWM_PHASE23_2_MASK                  (0xFFFF << 0)
#define PWM_PHASE23_2_SHIFT                 (0)
#define PWM_PHASE23_3(n)                    (((n) & 0xFFFF) << 16)
#define PWM_PHASE23_3_MASK                  (0xFFFF << 16)
#define PWM_PHASE23_3_SHIFT                 (16)

#define PWM_LOAD01_0(n)                     (((n) & 0xFFFF) << 0)
#define PWM_LOAD01_0_MASK                   (0xFFFF << 0)
#define PWM_LOAD01_0_SHIFT                  (0)
#define PWM_LOAD01_1(n)                     (((n) & 0xFFFF) << 16)
#define PWM_LOAD01_1_MASK                   (0xFFFF << 16)
#define PWM_LOAD01_1_SHIFT                  (16)

#define PWM_LOAD23_2(n)                     (((n) & 0xFFFF) << 0)
#define PWM_LOAD23_2_MASK                   (0xFFFF << 0)
#define PWM_LOAD23_2_SHIFT                  (0)
#define PWM_LOAD23_3(n)                     (((n) & 0xFFFF) << 16)
#define PWM_LOAD23_3_MASK                   (0xFFFF << 16)
#define PWM_LOAD23_3_SHIFT                  (16)

#define PWM_TOGGLE01_0(n)                   (((n) & 0xFFFF) << 0)
#define PWM_TOGGLE01_0_MASK                 (0xFFFF << 0)
#define PWM_TOGGLE01_0_SHIFT                (0)
#define PWM_TOGGLE01_1(n)                   (((n) & 0xFFFF) << 16)
#define PWM_TOGGLE01_1_MASK                 (0xFFFF << 16)
#define PWM_TOGGLE01_1_SHIFT                (16)

#define PWM_TOGGLE23_2(n)                   (((n) & 0xFFFF) << 0)
#define PWM_TOGGLE23_2_MASK                 (0xFFFF << 0)
#define PWM_TOGGLE23_2_SHIFT                (0)
#define PWM_TOGGLE23_3(n)                   (((n) & 0xFFFF) << 16)
#define PWM_TOGGLE23_3_MASK                 (0xFFFF << 16)
#define PWM_TOGGLE23_3_SHIFT                (16)

#define PWM_PHASEMOD_0                      (1 << 0)
#define PWM_PHASEMOD_1                      (1 << 1)
#define PWM_PHASEMOD_2                      (1 << 2)
#define PWM_PHASEMOD_3                      (1 << 3)

#endif

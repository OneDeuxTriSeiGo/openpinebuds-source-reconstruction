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
#ifndef __REG_RTC_H__
#define __REG_RTC_H__

#include "plat_types.h"

// PL031 Registers
struct RTC_T
{
    __I  uint32_t RTCDR;            // 0x000
    __IO uint32_t RTCMR;            // 0x004
    __IO uint32_t RTCLR;            // 0x008
    __IO uint32_t RTCCR;            // 0x00C
    __IO uint32_t RTCIMSC;          // 0x010
    __I  uint32_t RTCRIS;           // 0x014
    __I  uint32_t RTCMIS;           // 0x018
    __O  uint32_t RTCICR;           // 0x01C
};

#define RTC_CR_EN    (1 << 0)    /* counter enable bit */

#define RTC_BIT_AI    (1 << 0) /* Alarm interrupt bit */

#endif

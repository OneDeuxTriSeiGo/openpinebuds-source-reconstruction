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
#ifndef __REG_PSC_BEST2300P_H__
#define __REG_PSC_BEST2300P_H__

#include "plat_types.h"

struct AONPSC_T {
    __IO uint32_t REG_000;
    __IO uint32_t REG_004;
    __IO uint32_t REG_008;
    __IO uint32_t REG_00C;
    __IO uint32_t REG_010;
    __IO uint32_t REG_014;
    __IO uint32_t REG_018;
    __IO uint32_t REG_01C;
    __IO uint32_t REG_020;
    __IO uint32_t REG_024;
    __IO uint32_t REG_028;
    __IO uint32_t REG_02C;
    __IO uint32_t REG_030;
    __IO uint32_t REG_034;
    __IO uint32_t REG_038;
    __IO uint32_t REG_03C;
    __IO uint32_t REG_040;
    __IO uint32_t REG_044;
    __IO uint32_t REG_048;
    __IO uint32_t REG_04C;
    __IO uint32_t REG_050;
    __IO uint32_t REG_054;
    __IO uint32_t REG_058;
    __IO uint32_t REG_05C;
    __IO uint32_t REG_060;
    __IO uint32_t REG_064;
    __IO uint32_t REG_068;
    __IO uint32_t REG_06C;
    __IO uint32_t REG_070;
    __IO uint32_t REG_074;
    __IO uint32_t REG_078;
    __IO uint32_t REG_07C;
    __IO uint32_t REG_080;
    __IO uint32_t REG_084;
    __IO uint32_t REG_088;
    __IO uint32_t REG_08C;
    __IO uint32_t REG_090;
    __IO uint32_t REG_094;
    __IO uint32_t REG_098;
    __IO uint32_t REG_09C;
    __IO uint32_t REG_0A0;
    __IO uint32_t REG_0A4;
    __IO uint32_t REG_0A8;
    __IO uint32_t REG_0AC;
    __IO uint32_t REG_0B0;
    __IO uint32_t REG_0B4;
};

// reg_000
#define PSC_AON_MCU_PG_AUTO_EN                              (1 << 0)

// reg_004
#define PSC_AON_MCU_PSW_ACK_VALID                           (1 << 0)
#define PSC_AON_MCU_RESERVED(n)                             (((n) & 0x7F) << 1)
#define PSC_AON_MCU_RESERVED_MASK                           (0x7F << 1)
#define PSC_AON_MCU_RESERVED_SHIFT                          (1)
#define PSC_AON_MCU_MAIN_STATE(n)                           (((n) & 0x3) << 8)
#define PSC_AON_MCU_MAIN_STATE_MASK                         (0x3 << 8)
#define PSC_AON_MCU_MAIN_STATE_SHIFT                        (8)
#define PSC_AON_MCU_POWERDN_STATE(n)                        (((n) & 0x7) << 10)
#define PSC_AON_MCU_POWERDN_STATE_MASK                      (0x7 << 10)
#define PSC_AON_MCU_POWERDN_STATE_SHIFT                     (10)
#define PSC_AON_MCU_POWERUP_STATE(n)                        (((n) & 0x7) << 13)
#define PSC_AON_MCU_POWERUP_STATE_MASK                      (0x7 << 13)
#define PSC_AON_MCU_POWERUP_STATE_SHIFT                     (13)

// reg_008
#define PSC_AON_MCU_POWERDN_TIMER1(n)                       (((n) & 0x3F) << 0)
#define PSC_AON_MCU_POWERDN_TIMER1_MASK                     (0x3F << 0)
#define PSC_AON_MCU_POWERDN_TIMER1_SHIFT                    (0)
#define PSC_AON_MCU_POWERDN_TIMER2(n)                       (((n) & 0x3F) << 6)
#define PSC_AON_MCU_POWERDN_TIMER2_MASK                     (0x3F << 6)
#define PSC_AON_MCU_POWERDN_TIMER2_SHIFT                    (6)
#define PSC_AON_MCU_POWERDN_TIMER3(n)                       (((n) & 0x3F) << 12)
#define PSC_AON_MCU_POWERDN_TIMER3_MASK                     (0x3F << 12)
#define PSC_AON_MCU_POWERDN_TIMER3_SHIFT                    (12)
#define PSC_AON_MCU_POWERDN_TIMER4(n)                       (((n) & 0x3F) << 18)
#define PSC_AON_MCU_POWERDN_TIMER4_MASK                     (0x3F << 18)
#define PSC_AON_MCU_POWERDN_TIMER4_SHIFT                    (18)
#define PSC_AON_MCU_POWERDN_TIMER5(n)                       (((n) & 0xFF) << 24)
#define PSC_AON_MCU_POWERDN_TIMER5_MASK                     (0xFF << 24)
#define PSC_AON_MCU_POWERDN_TIMER5_SHIFT                    (24)

// reg_00c
#define PSC_AON_MCU_POWERUP_TIMER1(n)                       (((n) & 0x3F) << 0)
#define PSC_AON_MCU_POWERUP_TIMER1_MASK                     (0x3F << 0)
#define PSC_AON_MCU_POWERUP_TIMER1_SHIFT                    (0)
#define PSC_AON_MCU_POWERUP_TIMER2(n)                       (((n) & 0xFF) << 6)
#define PSC_AON_MCU_POWERUP_TIMER2_MASK                     (0xFF << 6)
#define PSC_AON_MCU_POWERUP_TIMER2_SHIFT                    (6)
#define PSC_AON_MCU_POWERUP_TIMER3(n)                       (((n) & 0x3F) << 14)
#define PSC_AON_MCU_POWERUP_TIMER3_MASK                     (0x3F << 14)
#define PSC_AON_MCU_POWERUP_TIMER3_SHIFT                    (14)
#define PSC_AON_MCU_POWERUP_TIMER4(n)                       (((n) & 0x3F) << 20)
#define PSC_AON_MCU_POWERUP_TIMER4_MASK                     (0x3F << 20)
#define PSC_AON_MCU_POWERUP_TIMER4_SHIFT                    (20)
#define PSC_AON_MCU_POWERUP_TIMER5(n)                       (((n) & 0x3F) << 26)
#define PSC_AON_MCU_POWERUP_TIMER5_MASK                     (0x3F << 26)
#define PSC_AON_MCU_POWERUP_TIMER5_SHIFT                    (26)

// reg_010
#define PSC_AON_MCU_POWERDN_START                           (1 << 0)

// reg_014
#define PSC_AON_MCU_POWERUP_START                           (1 << 0)

// reg_018
#define PSC_AON_MCU_CLK_STOP_REG                            (1 << 0)
#define PSC_AON_MCU_ISO_EN_REG                              (1 << 1)
#define PSC_AON_MCU_RESETN_ASSERT_REG                       (1 << 2)
#define PSC_AON_MCU_PSW_EN_REG                              (1 << 3)
#define PSC_AON_MCU_CLK_STOP_DR                             (1 << 4)
#define PSC_AON_MCU_ISO_EN_DR                               (1 << 5)
#define PSC_AON_MCU_RESETN_ASSERT_DR                        (1 << 6)
#define PSC_AON_MCU_PSW_EN_DR                               (1 << 7)

// reg_01c
#define PSC_AON_MCU_MAIN_STATE(n)                           (((n) & 0x3) << 0)
#define PSC_AON_MCU_MAIN_STATE_MASK                         (0x3 << 0)
#define PSC_AON_MCU_MAIN_STATE_SHIFT                        (0)
#define PSC_AON_MCU_POWERDN_STATE(n)                        (((n) & 0x7) << 2)
#define PSC_AON_MCU_POWERDN_STATE_MASK                      (0x7 << 2)
#define PSC_AON_MCU_POWERDN_STATE_SHIFT                     (2)
#define PSC_AON_MCU_POWERUP_STATE(n)                        (((n) & 0x7) << 5)
#define PSC_AON_MCU_POWERUP_STATE_MASK                      (0x7 << 5)
#define PSC_AON_MCU_POWERUP_STATE_SHIFT                     (5)
#define PSC_AON_BT_MAIN_STATE(n)                            (((n) & 0x3) << 8)
#define PSC_AON_BT_MAIN_STATE_MASK                          (0x3 << 8)
#define PSC_AON_BT_MAIN_STATE_SHIFT                         (8)
#define PSC_AON_BT_POWERDN_STATE(n)                         (((n) & 0x7) << 10)
#define PSC_AON_BT_POWERDN_STATE_MASK                       (0x7 << 10)
#define PSC_AON_BT_POWERDN_STATE_SHIFT                      (10)
#define PSC_AON_BT_POWERUP_STATE(n)                         (((n) & 0x7) << 13)
#define PSC_AON_BT_POWERUP_STATE_MASK                       (0x7 << 13)
#define PSC_AON_BT_POWERUP_STATE_SHIFT                      (13)
#define PSC_AON_WLAN_MAIN_STATE(n)                          (((n) & 0x3) << 16)
#define PSC_AON_WLAN_MAIN_STATE_MASK                        (0x3 << 16)
#define PSC_AON_WLAN_MAIN_STATE_SHIFT                       (16)
#define PSC_AON_WLAN_POWERDN_STATE(n)                       (((n) & 0x7) << 18)
#define PSC_AON_WLAN_POWERDN_STATE_MASK                     (0x7 << 18)
#define PSC_AON_WLAN_POWERDN_STATE_SHIFT                    (18)
#define PSC_AON_WLAN_POWERUP_STATE(n)                       (((n) & 0x7) << 21)
#define PSC_AON_WLAN_POWERUP_STATE_MASK                     (0x7 << 21)
#define PSC_AON_WLAN_POWERUP_STATE_SHIFT                    (21)
#define PSC_AON_CODEC_MAIN_STATE(n)                         (((n) & 0x3) << 24)
#define PSC_AON_CODEC_MAIN_STATE_MASK                       (0x3 << 24)
#define PSC_AON_CODEC_MAIN_STATE_SHIFT                      (24)
#define PSC_AON_CODEC_POWERDN_STATE(n)                      (((n) & 0x7) << 26)
#define PSC_AON_CODEC_POWERDN_STATE_MASK                    (0x7 << 26)
#define PSC_AON_CODEC_POWERDN_STATE_SHIFT                   (26)
#define PSC_AON_CODEC_POWERUP_STATE(n)                      (((n) & 0x7) << 29)
#define PSC_AON_CODEC_POWERUP_STATE_MASK                    (0x7 << 29)
#define PSC_AON_CODEC_POWERUP_STATE_SHIFT                   (29)

// reg_020
#define PSC_AON_BT_PG_AUTO_EN                               (1 << 0)

// reg_024
#define PSC_AON_BT_PSW_ACK_VALID                            (1 << 0)
#define PSC_AON_BT_RESERVED(n)                              (((n) & 0x7F) << 1)
#define PSC_AON_BT_RESERVED_MASK                            (0x7F << 1)
#define PSC_AON_BT_RESERVED_SHIFT                           (1)
#define PSC_AON_BT_MAIN_STATE(n)                            (((n) & 0x3) << 8)
#define PSC_AON_BT_MAIN_STATE_MASK                          (0x3 << 8)
#define PSC_AON_BT_MAIN_STATE_SHIFT                         (8)
#define PSC_AON_BT_POWERDN_STATE(n)                         (((n) & 0x7) << 10)
#define PSC_AON_BT_POWERDN_STATE_MASK                       (0x7 << 10)
#define PSC_AON_BT_POWERDN_STATE_SHIFT                      (10)
#define PSC_AON_BT_POWERUP_STATE(n)                         (((n) & 0x7) << 13)
#define PSC_AON_BT_POWERUP_STATE_MASK                       (0x7 << 13)
#define PSC_AON_BT_POWERUP_STATE_SHIFT                      (13)

// reg_028
#define PSC_AON_BT_POWERDN_TIMER1(n)                        (((n) & 0x3F) << 0)
#define PSC_AON_BT_POWERDN_TIMER1_MASK                      (0x3F << 0)
#define PSC_AON_BT_POWERDN_TIMER1_SHIFT                     (0)
#define PSC_AON_BT_POWERDN_TIMER2(n)                        (((n) & 0x3F) << 6)
#define PSC_AON_BT_POWERDN_TIMER2_MASK                      (0x3F << 6)
#define PSC_AON_BT_POWERDN_TIMER2_SHIFT                     (6)
#define PSC_AON_BT_POWERDN_TIMER3(n)                        (((n) & 0x3F) << 12)
#define PSC_AON_BT_POWERDN_TIMER3_MASK                      (0x3F << 12)
#define PSC_AON_BT_POWERDN_TIMER3_SHIFT                     (12)
#define PSC_AON_BT_POWERDN_TIMER4(n)                        (((n) & 0x3F) << 18)
#define PSC_AON_BT_POWERDN_TIMER4_MASK                      (0x3F << 18)
#define PSC_AON_BT_POWERDN_TIMER4_SHIFT                     (18)
#define PSC_AON_BT_POWERDN_TIMER5(n)                        (((n) & 0xFF) << 24)
#define PSC_AON_BT_POWERDN_TIMER5_MASK                      (0xFF << 24)
#define PSC_AON_BT_POWERDN_TIMER5_SHIFT                     (24)

// reg_02c
#define PSC_AON_BT_POWERUP_TIMER1(n)                        (((n) & 0x3F) << 0)
#define PSC_AON_BT_POWERUP_TIMER1_MASK                      (0x3F << 0)
#define PSC_AON_BT_POWERUP_TIMER1_SHIFT                     (0)
#define PSC_AON_BT_POWERUP_TIMER2(n)                        (((n) & 0xFF) << 6)
#define PSC_AON_BT_POWERUP_TIMER2_MASK                      (0xFF << 6)
#define PSC_AON_BT_POWERUP_TIMER2_SHIFT                     (6)
#define PSC_AON_BT_POWERUP_TIMER3(n)                        (((n) & 0x3F) << 14)
#define PSC_AON_BT_POWERUP_TIMER3_MASK                      (0x3F << 14)
#define PSC_AON_BT_POWERUP_TIMER3_SHIFT                     (14)
#define PSC_AON_BT_POWERUP_TIMER4(n)                        (((n) & 0x3F) << 20)
#define PSC_AON_BT_POWERUP_TIMER4_MASK                      (0x3F << 20)
#define PSC_AON_BT_POWERUP_TIMER4_SHIFT                     (20)
#define PSC_AON_BT_POWERUP_TIMER5(n)                        (((n) & 0x3F) << 26)
#define PSC_AON_BT_POWERUP_TIMER5_MASK                      (0x3F << 26)
#define PSC_AON_BT_POWERUP_TIMER5_SHIFT                     (26)

// reg_030
#define PSC_AON_BT_POWERDN_START                            (1 << 0)

// reg_034
#define PSC_AON_BT_POWERUP_START                            (1 << 0)

// reg_038
#define PSC_AON_BT_CLK_STOP_REG                             (1 << 0)
#define PSC_AON_BT_ISO_EN_REG                               (1 << 1)
#define PSC_AON_BT_RESETN_ASSERT_REG                        (1 << 2)
#define PSC_AON_BT_PSW_EN_REG                               (1 << 3)
#define PSC_AON_BT_CLK_STOP_DR                              (1 << 4)
#define PSC_AON_BT_ISO_EN_DR                                (1 << 5)
#define PSC_AON_BT_RESETN_ASSERT_DR                         (1 << 6)
#define PSC_AON_BT_PSW_EN_DR                                (1 << 7)

// reg_040
#define PSC_AON_WLAN_PG_AUTO_EN                             (1 << 0)

// reg_044
#define PSC_AON_WLAN_PSW_ACK_VALID                          (1 << 0)
#define PSC_AON_WLAN_RESERVED(n)                            (((n) & 0x7F) << 1)
#define PSC_AON_WLAN_RESERVED_MASK                          (0x7F << 1)
#define PSC_AON_WLAN_RESERVED_SHIFT                         (1)
#define PSC_AON_WLAN_MAIN_STATE(n)                          (((n) & 0x3) << 8)
#define PSC_AON_WLAN_MAIN_STATE_MASK                        (0x3 << 8)
#define PSC_AON_WLAN_MAIN_STATE_SHIFT                       (8)
#define PSC_AON_WLAN_POWERDN_STATE(n)                       (((n) & 0x7) << 10)
#define PSC_AON_WLAN_POWERDN_STATE_MASK                     (0x7 << 10)
#define PSC_AON_WLAN_POWERDN_STATE_SHIFT                    (10)
#define PSC_AON_WLAN_POWERUP_STATE(n)                       (((n) & 0x7) << 13)
#define PSC_AON_WLAN_POWERUP_STATE_MASK                     (0x7 << 13)
#define PSC_AON_WLAN_POWERUP_STATE_SHIFT                    (13)

// reg_048
#define PSC_AON_WLAN_POWERDN_TIMER1(n)                      (((n) & 0x3F) << 0)
#define PSC_AON_WLAN_POWERDN_TIMER1_MASK                    (0x3F << 0)
#define PSC_AON_WLAN_POWERDN_TIMER1_SHIFT                   (0)
#define PSC_AON_WLAN_POWERDN_TIMER2(n)                      (((n) & 0x3F) << 6)
#define PSC_AON_WLAN_POWERDN_TIMER2_MASK                    (0x3F << 6)
#define PSC_AON_WLAN_POWERDN_TIMER2_SHIFT                   (6)
#define PSC_AON_WLAN_POWERDN_TIMER3(n)                      (((n) & 0x3F) << 12)
#define PSC_AON_WLAN_POWERDN_TIMER3_MASK                    (0x3F << 12)
#define PSC_AON_WLAN_POWERDN_TIMER3_SHIFT                   (12)
#define PSC_AON_WLAN_POWERDN_TIMER4(n)                      (((n) & 0x3F) << 18)
#define PSC_AON_WLAN_POWERDN_TIMER4_MASK                    (0x3F << 18)
#define PSC_AON_WLAN_POWERDN_TIMER4_SHIFT                   (18)
#define PSC_AON_WLAN_POWERDN_TIMER5(n)                      (((n) & 0xFF) << 24)
#define PSC_AON_WLAN_POWERDN_TIMER5_MASK                    (0xFF << 24)
#define PSC_AON_WLAN_POWERDN_TIMER5_SHIFT                   (24)

// reg_04c
#define PSC_AON_WLAN_POWERUP_TIMER1(n)                      (((n) & 0x3F) << 0)
#define PSC_AON_WLAN_POWERUP_TIMER1_MASK                    (0x3F << 0)
#define PSC_AON_WLAN_POWERUP_TIMER1_SHIFT                   (0)
#define PSC_AON_WLAN_POWERUP_TIMER2(n)                      (((n) & 0xFF) << 6)
#define PSC_AON_WLAN_POWERUP_TIMER2_MASK                    (0xFF << 6)
#define PSC_AON_WLAN_POWERUP_TIMER2_SHIFT                   (6)
#define PSC_AON_WLAN_POWERUP_TIMER3(n)                      (((n) & 0x3F) << 14)
#define PSC_AON_WLAN_POWERUP_TIMER3_MASK                    (0x3F << 14)
#define PSC_AON_WLAN_POWERUP_TIMER3_SHIFT                   (14)
#define PSC_AON_WLAN_POWERUP_TIMER4(n)                      (((n) & 0x3F) << 20)
#define PSC_AON_WLAN_POWERUP_TIMER4_MASK                    (0x3F << 20)
#define PSC_AON_WLAN_POWERUP_TIMER4_SHIFT                   (20)
#define PSC_AON_WLAN_POWERUP_TIMER5(n)                      (((n) & 0x3F) << 26)
#define PSC_AON_WLAN_POWERUP_TIMER5_MASK                    (0x3F << 26)
#define PSC_AON_WLAN_POWERUP_TIMER5_SHIFT                   (26)

// reg_050
#define PSC_AON_WLAN_POWERDN_START                          (1 << 0)

// reg_054
#define PSC_AON_WLAN_POWERUP_START                          (1 << 0)

// reg_058
#define PSC_AON_WLAN_CLK_STOP_REG                           (1 << 0)
#define PSC_AON_WLAN_ISO_EN_REG                             (1 << 1)
#define PSC_AON_WLAN_RESETN_ASSERT_REG                      (1 << 2)
#define PSC_AON_WLAN_PSW_EN_REG                             (1 << 3)
#define PSC_AON_WLAN_CLK_STOP_DR                            (1 << 4)
#define PSC_AON_WLAN_ISO_EN_DR                              (1 << 5)
#define PSC_AON_WLAN_RESETN_ASSERT_DR                       (1 << 6)
#define PSC_AON_WLAN_PSW_EN_DR                              (1 << 7)

// reg_060
#define PSC_AON_CODEC_PG_AUTO_EN                            (1 << 0)

// reg_064
#define PSC_AON_CODEC_PSW_ACK_VALID                         (1 << 0)
#define PSC_AON_CODEC_RESERVED(n)                           (((n) & 0x7F) << 1)
#define PSC_AON_CODEC_RESERVED_MASK                         (0x7F << 1)
#define PSC_AON_CODEC_RESERVED_SHIFT                        (1)
#define PSC_AON_CODEC_MAIN_STATE(n)                         (((n) & 0x3) << 8)
#define PSC_AON_CODEC_MAIN_STATE_MASK                       (0x3 << 8)
#define PSC_AON_CODEC_MAIN_STATE_SHIFT                      (8)
#define PSC_AON_CODEC_POWERDN_STATE(n)                      (((n) & 0x7) << 10)
#define PSC_AON_CODEC_POWERDN_STATE_MASK                    (0x7 << 10)
#define PSC_AON_CODEC_POWERDN_STATE_SHIFT                   (10)
#define PSC_AON_CODEC_POWERUP_STATE(n)                      (((n) & 0x7) << 13)
#define PSC_AON_CODEC_POWERUP_STATE_MASK                    (0x7 << 13)
#define PSC_AON_CODEC_POWERUP_STATE_SHIFT                   (13)

// reg_068
#define PSC_AON_CODEC_POWERDN_TIMER1(n)                     (((n) & 0x3F) << 0)
#define PSC_AON_CODEC_POWERDN_TIMER1_MASK                   (0x3F << 0)
#define PSC_AON_CODEC_POWERDN_TIMER1_SHIFT                  (0)
#define PSC_AON_CODEC_POWERDN_TIMER2(n)                     (((n) & 0x3F) << 6)
#define PSC_AON_CODEC_POWERDN_TIMER2_MASK                   (0x3F << 6)
#define PSC_AON_CODEC_POWERDN_TIMER2_SHIFT                  (6)
#define PSC_AON_CODEC_POWERDN_TIMER3(n)                     (((n) & 0x3F) << 12)
#define PSC_AON_CODEC_POWERDN_TIMER3_MASK                   (0x3F << 12)
#define PSC_AON_CODEC_POWERDN_TIMER3_SHIFT                  (12)
#define PSC_AON_CODEC_POWERDN_TIMER4(n)                     (((n) & 0x3F) << 18)
#define PSC_AON_CODEC_POWERDN_TIMER4_MASK                   (0x3F << 18)
#define PSC_AON_CODEC_POWERDN_TIMER4_SHIFT                  (18)
#define PSC_AON_CODEC_POWERDN_TIMER5(n)                     (((n) & 0xFF) << 24)
#define PSC_AON_CODEC_POWERDN_TIMER5_MASK                   (0xFF << 24)
#define PSC_AON_CODEC_POWERDN_TIMER5_SHIFT                  (24)

// reg_06c
#define PSC_AON_CODEC_POWERUP_TIMER1(n)                     (((n) & 0x3F) << 0)
#define PSC_AON_CODEC_POWERUP_TIMER1_MASK                   (0x3F << 0)
#define PSC_AON_CODEC_POWERUP_TIMER1_SHIFT                  (0)
#define PSC_AON_CODEC_POWERUP_TIMER2(n)                     (((n) & 0xFF) << 6)
#define PSC_AON_CODEC_POWERUP_TIMER2_MASK                   (0xFF << 6)
#define PSC_AON_CODEC_POWERUP_TIMER2_SHIFT                  (6)
#define PSC_AON_CODEC_POWERUP_TIMER3(n)                     (((n) & 0x3F) << 14)
#define PSC_AON_CODEC_POWERUP_TIMER3_MASK                   (0x3F << 14)
#define PSC_AON_CODEC_POWERUP_TIMER3_SHIFT                  (14)
#define PSC_AON_CODEC_POWERUP_TIMER4(n)                     (((n) & 0x3F) << 20)
#define PSC_AON_CODEC_POWERUP_TIMER4_MASK                   (0x3F << 20)
#define PSC_AON_CODEC_POWERUP_TIMER4_SHIFT                  (20)
#define PSC_AON_CODEC_POWERUP_TIMER5(n)                     (((n) & 0x3F) << 26)
#define PSC_AON_CODEC_POWERUP_TIMER5_MASK                   (0x3F << 26)
#define PSC_AON_CODEC_POWERUP_TIMER5_SHIFT                  (26)

// reg_070
#define PSC_AON_CODEC_POWERDN_START                         (1 << 0)

// reg_074
#define PSC_AON_CODEC_POWERUP_START                         (1 << 0)

// reg_078
#define PSC_AON_CODEC_CLK_STOP_REG                          (1 << 0)
#define PSC_AON_CODEC_ISO_EN_REG                            (1 << 1)
#define PSC_AON_CODEC_RESETN_ASSERT_REG                     (1 << 2)
#define PSC_AON_CODEC_PSW_EN_REG                            (1 << 3)
#define PSC_AON_CODEC_CLK_STOP_DR                           (1 << 4)
#define PSC_AON_CODEC_ISO_EN_DR                             (1 << 5)
#define PSC_AON_CODEC_RESETN_ASSERT_DR                      (1 << 6)
#define PSC_AON_CODEC_PSW_EN_DR                             (1 << 7)

// reg_080
#define PSC_AON_MCU_INTR_MASK(n)                            (((n) & 0xFFFFFFFF) << 0)
#define PSC_AON_MCU_INTR_MASK_MASK                          (0xFFFFFFFF << 0)
#define PSC_AON_MCU_INTR_MASK_SHIFT                         (0)

// reg_084
#define PSC_AON_MCU_INTR_MASK2(n)                           (((n) & 0xFFFF) << 0)
#define PSC_AON_MCU_INTR_MASK2_MASK                         (0xFFFF << 0)
#define PSC_AON_MCU_INTR_MASK2_SHIFT                        (0)

// reg_088
#define PSC_AON_MCU_INTR_MASK_STATUS(n)                     (((n) & 0xFFFFFFFF) << 0)
#define PSC_AON_MCU_INTR_MASK_STATUS_MASK                   (0xFFFFFFFF << 0)
#define PSC_AON_MCU_INTR_MASK_STATUS_SHIFT                  (0)

// reg_08c
#define PSC_AON_MCU_INTR_MASK_STATUS2(n)                    (((n) & 0xFFFF) << 0)
#define PSC_AON_MCU_INTR_MASK_STATUS2_MASK                  (0xFFFF << 0)
#define PSC_AON_MCU_INTR_MASK_STATUS2_SHIFT                 (0)

// reg_090
#define PSC_AON_BT_INTR_MASK(n)                             (((n) & 0xFFFFFFFF) << 0)
#define PSC_AON_BT_INTR_MASK_MASK                           (0xFFFFFFFF << 0)
#define PSC_AON_BT_INTR_MASK_SHIFT                          (0)

// reg_094
#define PSC_AON_BT_INTR_MASK2(n)                            (((n) & 0xFFFF) << 0)
#define PSC_AON_BT_INTR_MASK2_MASK                          (0xFFFF << 0)
#define PSC_AON_BT_INTR_MASK2_SHIFT                         (0)

// reg_098
#define PSC_AON_BT_INTR_MASK_STATUS(n)                      (((n) & 0xFFFFFFFF) << 0)
#define PSC_AON_BT_INTR_MASK_STATUS_MASK                    (0xFFFFFFFF << 0)
#define PSC_AON_BT_INTR_MASK_STATUS_SHIFT                   (0)

// reg_09c
#define PSC_AON_BT_INTR_MASK_STATUS2(n)                     (((n) & 0xFFFF) << 0)
#define PSC_AON_BT_INTR_MASK_STATUS2_MASK                   (0xFFFF << 0)
#define PSC_AON_BT_INTR_MASK_STATUS2_SHIFT                  (0)

// reg_0a0
#define PSC_AON_WLAN_INTR_MASK(n)                           (((n) & 0xFFFFFFFF) << 0)
#define PSC_AON_WLAN_INTR_MASK_MASK                         (0xFFFFFFFF << 0)
#define PSC_AON_WLAN_INTR_MASK_SHIFT                        (0)

// reg_0a4
#define PSC_AON_WLAN_INTR_MASK2(n)                          (((n) & 0xFFFF) << 0)
#define PSC_AON_WLAN_INTR_MASK2_MASK                        (0xFFFF << 0)
#define PSC_AON_WLAN_INTR_MASK2_SHIFT                       (0)

// reg_0a8
#define PSC_AON_WLAN_INTR_MASK_STATUS(n)                    (((n) & 0xFFFFFFFF) << 0)
#define PSC_AON_WLAN_INTR_MASK_STATUS_MASK                  (0xFFFFFFFF << 0)
#define PSC_AON_WLAN_INTR_MASK_STATUS_SHIFT                 (0)

// reg_0ac
#define PSC_AON_WLAN_INTR_MASK_STATUS2(n)                   (((n) & 0xFFFF) << 0)
#define PSC_AON_WLAN_INTR_MASK_STATUS2_MASK                 (0xFFFF << 0)
#define PSC_AON_WLAN_INTR_MASK_STATUS2_SHIFT                (0)

// reg_0b0
#define PSC_AON_INTR_RAW_STATUS(n)                          (((n) & 0xFFFFFFFF) << 0)
#define PSC_AON_INTR_RAW_STATUS_MASK                        (0xFFFFFFFF << 0)
#define PSC_AON_INTR_RAW_STATUS_SHIFT                       (0)

// reg_0b4
#define PSC_AON_INTR_RAW_STATUS2(n)                         (((n) & 0xFFFF) << 0)
#define PSC_AON_INTR_RAW_STATUS2_MASK                       (0xFFFF << 0)
#define PSC_AON_INTR_RAW_STATUS2_SHIFT                      (0)

#endif

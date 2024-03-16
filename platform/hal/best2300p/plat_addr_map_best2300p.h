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
#ifndef __PLAT_ADDR_MAP_BEST2300P_H__
#define __PLAT_ADDR_MAP_BEST2300P_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ROM_BASE                                0x00020000
#define ROMD_BASE                               0x24020000

#ifndef ROM_SIZE
#ifdef FPGA
#define ROM_SIZE                                0x00020000
#else
#define ROM_SIZE                                0x00010000
#endif
#endif

#define RAM0_BASE                               0x20000000
#define RAMX0_BASE                              0x00200000
#define RAM1_BASE                               0x20040000
#define RAMX1_BASE                              0x00240000
#define RAM2_BASE                               0x20080000
#define RAMX2_BASE                              0x00280000
#define RAM3_BASE                               0x20100000
#define RAMX3_BASE                              0x00300000
#define RAM4_BASE                               0x20180000
#define RAMX4_BASE                              0x00380000
#define RAM5_BASE                               0x201C0000
#define RAMX5_BASE                              0x003C0000
#define RAM_BASE                                RAM0_BASE
#define RAMX_BASE                               RAMX0_BASE

#define RAM5_SIZE                               0x00040000

/* total ramv size is 0x30000 */
#define RAMV_BASE                               0x20200000
#ifndef RAMV_SIZE
#define RAMV_SIZE                               0x00000000
#endif

#ifdef __BT_RAMRUN__
#define BT_RAMRUN_BASE                           RAM3_BASE
#define BT_RAMRUNX_BASE                          RAMX3_BASE
#endif

#ifndef CP_FLASH_SIZE
#define CP_FLASH_SIZE 0
#endif

#ifdef CHIP_HAS_CP
#if (CP_FLASH_SIZE > 0)
#define CP_FLASH_BASE                           (FLASH_BASE + FLASH_SIZE - CP_FLASH_SIZE)
#define CP_FLASHX_BASE                          (FLASHX_BASE + FLASH_SIZE - CP_FLASH_SIZE)
#endif

#ifdef LARGE_RAM
/*MCU use RAM0/1/2/3, CPX use RAM4, and CP use RAM5*/
#define RAMCP_TOP                               (RAM5_BASE + RAM5_SIZE)

#ifndef RAMCP_SIZE
#define RAMCP_SIZE                              RAM5_SIZE
#endif

#ifndef RAMCP_BASE
#define RAMCP_BASE                              (RAMCP_TOP - RAMCP_SIZE)
#endif

#ifndef RAMCPX_SIZE
#define RAMCPX_SIZE                             (RAM5_BASE - RAM4_BASE)
#endif

#ifndef RAMCPX_BASE
#define RAMCPX_BASE                             (RAM_TO_RAMX(RAMCP_BASE) - RAMCPX_SIZE)
#endif

#else /*LARGE_RAM*/
/*MCU use RAM0, CP and CPX use RAM1*/
#define RAMCP_TOP                               RAM2_BASE

#ifndef RAMCP_SIZE
#define RAMCP_SIZE                              0x20000
#endif

#ifndef RAMCP_BASE
#define RAMCP_BASE                              (RAMCP_TOP - RAMCP_SIZE)
#endif

#ifndef RAMCPX_SIZE
#define RAMCPX_SIZE                             (RAMX2_BASE - RAMX1_BASE - RAMCP_SIZE)
#endif

#ifndef RAMCPX_BASE
#define RAMCPX_BASE                             (RAM_TO_RAMX(RAMCP_BASE) - RAMCPX_SIZE)
#endif

#endif /*LARGE_RAM*/
#endif

#define RAM_TOTAL_SIZE                          (RAM5_BASE + RAM5_SIZE - RAM0_BASE) // 0x00200000

#if defined(ARM_CMSE) || defined(ARM_CMNS)
#undef RAM_BASE
#undef RAMX_BASE

/*MPC: SRAM block size: 0x8000, FLASH block size 0x40000*/
#ifndef RAM_S_SIZE
#define RAM_S_SIZE                              0x00020000
#endif
#define RAM_NSC_SIZE                            0
#ifndef FLASH_S_SIZE
#define FLASH_S_SIZE                            0x00040000
#endif

#define RAM_NS_BASE                             (RAM0_BASE + RAM_S_SIZE)
#define RAMX_NS_BASE                            (RAMX0_BASE + RAM_S_SIZE)

#if defined(ARM_CMNS)
#define RAM_BASE                                RAM_NS_BASE
#define RAMX_BASE                               RAMX_NS_BASE
#else
#if ((RAM_S_SIZE) & (0x8000-1))
#error "RAM_S_SIZE should be 0x8000 aligned"
#endif
#if (FLASH_S_SIZE & (0x40000-1))
#error "FLASH_S_SIZE should be 0x40000 aligned"
#endif
#define RAM_BASE                                RAM0_BASE
#define RAMX_BASE                               RAMX0_BASE
#define RAM_SIZE                                RAM_S_SIZE
#ifndef NS_APP_START_OFFSET
#define NS_APP_START_OFFSET                     (FLASH_S_SIZE)
#endif
#ifndef FLASH_REGION_SIZE
#define FLASH_REGION_SIZE                       FLASH_S_SIZE
#endif
#endif
#endif /* defined(ARM_CMSE) || defined(ARM_CMNS) */

#ifndef RAM_MCU_SIZE
/* secure and non-secure ram size */
#ifdef LARGE_RAM
#ifdef CHIP_HAS_CP
#define RAM_MCU_SIZE                            (RAMCPX_BASE - RAMX_BASE)
#else
#define RAM_MCU_SIZE                            (RAM5_BASE + RAM5_SIZE - RAM_BASE)
#endif
#else
#ifdef CHIP_HAS_CP
#define RAM_MCU_SIZE                            (RAMCPX_BASE - RAMX_BASE)
#else
#define RAM_MCU_SIZE                            (RAM2_BASE - RAM_BASE) // 0x00080000
#endif
#endif
#endif /*RAM_MCU_SIZE*/

#if defined(ARM_CMSE)
#ifndef RAM_NS_SIZE
#define RAM_NS_SIZE                             (RAM_BASE + RAM_MCU_SIZE - RAM_NS_BASE)
#endif /*RAM_NS_SIZE*/
#endif

#ifndef RAM_SIZE
#define RAM_SIZE                                RAM_MCU_SIZE
#endif

#define FLASH_BASE                              0x2C000000
#define FLASH_NC_BASE                           0x28000000
#define FLASHX_BASE                             0x0C000000
#define FLASHX_NC_BASE                          0x08000000

#define DCACHE_CTRL_BASE                        0x27FFA000
#define ICACHE_CTRL_BASE                        0x27FFC000

#define CMU_BASE                                0x40000000
#define MCU_WDT_BASE                            0x40001000
#define MCU_TIMER0_BASE                         0x40002000
#define MCU_TIMER1_BASE                         0x40003000
#define MCU_TIMER2_BASE                         0x40004000
#define I2C0_BASE                               0x40005000
#define I2C1_BASE                               0x40006000
#define SPI_BASE                                0x40007000
#define SPILCD_BASE                             0x40008000
#define ISPI_BASE                               0x40009000
#define SPIPHY_BASE                             0x4000A000
#define UART0_BASE                              0x4000B000
#define UART1_BASE                              0x4000C000
#define UART2_BASE                              0x4000D000
#define BTPCM_BASE                              0x4000E000
#define I2S0_BASE                               0x4000F000
#define SPDIF0_BASE                             0x40010000
#define I2S1_BASE                               0x40017000
#define SEC_ENG_BASE                            0x40020000

#define AON_CMU_BASE                            0x40080000
#define AON_GPIO_BASE                           0x40081000
#define AON_WDT_BASE                            0x40082000
#define AON_PWM0_BASE                           0x40083000
#define AON_TIMER_BASE                          0x40084000
#define AON_PSC_BASE                            0x40088000
#define AON_IOMUX_BASE                          0x40086000

#define CHECKSUM_BASE                           0x40100000
#define CRC_BASE                                0x40108000
#define SDMMC_BASE                              0x40110000
#define BES2003_AUDMA_BASE                      0x40120000
#define BES2003_GPDMA_BASE                      0x40130000
#define FLASH_CTRL_BASE                         0x40140000
#define BTDUMP_BASE                             0x401E0000
#define I2C_SLAVE_BASE                          0x40160000
#define USB_BASE                                0x40180000
#define SEDMA_BASE                              0x401D0000

#define CODEC_BASE                              0x40380000

#define BT_SUBSYS_BASE                          0xA0000000
#define BT_RAM_BASE                             0xC0000000
#define BT_RAM_SIZE                             0x00008000
#define BT_UART_BASE                            0xD0300000
#define BT_CMU_BASE                             0xD0330000

#define IOMUX_BASE                              AON_IOMUX_BASE
#define GPIO_BASE                               AON_GPIO_BASE
#define PWM_BASE                                AON_PWM0_BASE

#ifdef CHIP_BEST2003_DSP
#define TIMER0_BASE                             DSP_TIMER0_BASE
#else
#define TIMER0_BASE                             MCU_TIMER0_BASE
#endif
#define TIMER1_BASE                             MCU_TIMER1_BASE
#define WDT_BASE                                AON_WDT_BASE

/* For linker scripts */

#define VECTOR_SECTION_SIZE                     360
#define REBOOT_PARAM_SECTION_SIZE               64
#define ROM_BUILD_INFO_SECTION_SIZE             40
#define ROM_EXPORT_FN_SECTION_SIZE              128

/* For boot struct version */
#ifndef SECURE_BOOT_VER
#define SECURE_BOOT_VER                         4
#endif

#ifdef __cplusplus
}
#endif

#endif

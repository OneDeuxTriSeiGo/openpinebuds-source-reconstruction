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
#ifndef __HAL_CMU_BEST2300P_H__
#define __HAL_CMU_BEST2300P_H__

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_CMU_VALID_CRYSTAL_FREQ          { 24000000, 40000000, }

enum HAL_CMU_MOD_ID_T {
    // HCLK/HRST
    HAL_CMU_MOD_H_MCU,          // 0
    HAL_CMU_MOD_H_CACHE0,       // 1
    HAL_CMU_MOD_H_CP,           // 2
    HAL_CMU_MOD_H_CACHE1,       // 3
    HAL_CMU_MOD_H_ADMA,         // 4
    HAL_CMU_MOD_H_GDMA,         // 5
    HAL_CMU_MOD_H_SEC_ENG,      // 6
    HAL_CMU_MOD_H_USBC,         // 7
    HAL_CMU_MOD_H_USBH,         // 8
    HAL_CMU_MOD_H_I2C_SLAVE,    // 9
    HAL_CMU_MOD_H_AX2H_A7,      // 10
    HAL_CMU_MOD_H_AH2H_WF,      // 11
    HAL_CMU_MOD_H_AH2H_BT,      // 12
    HAL_CMU_MOD_H_CODEC,        // 13
    HAL_CMU_MOD_H_AHB1,         // 14
    HAL_CMU_MOD_H_AHB0,         // 15
    HAL_CMU_MOD_H_PSRAM1G,      // 16
    HAL_CMU_MOD_H_PSRAM200,     // 17
    HAL_CMU_MOD_H_FLASH,        // 18
    HAL_CMU_MOD_H_RAM5,         // 19
    HAL_CMU_MOD_H_RAM4,         // 20
    HAL_CMU_MOD_H_RAM3,         // 21
    HAL_CMU_MOD_H_RAM2,         // 22
    HAL_CMU_MOD_H_RAM1,         // 23
    HAL_CMU_MOD_H_RAM0,         // 24
    HAL_CMU_MOD_H_ROM0,         // 25
    HAL_CMU_MOD_H_BT_DUMP,      // 26
    HAL_CMU_MOD_H_WF_DUMP,      // 27
    HAL_CMU_MOD_H_SDMMC,        // 28
    HAL_CMU_MOD_H_CHECKSUM,     // 29
    HAL_CMU_MOD_H_CRC,          // 30
    HAL_CMU_MOD_H_FLASH1,       // 31
    // PCLK/PRST
    HAL_CMU_MOD_P_CMU,          // 0
    HAL_CMU_MOD_P_WDT,          // 1
    HAL_CMU_MOD_P_TIMER0,       // 2
    HAL_CMU_MOD_P_TIMER1,       // 3
    HAL_CMU_MOD_P_TIMER2,       // 4
    HAL_CMU_MOD_P_I2C0,         // 5
    HAL_CMU_MOD_P_I2C1,         // 6
    HAL_CMU_MOD_P_SPI,          // 7
    HAL_CMU_MOD_P_SLCD,         // 8
    HAL_CMU_MOD_P_SPI_ITN,      // 9
    HAL_CMU_MOD_P_SPI_PHY,      // 10
    HAL_CMU_MOD_P_UART0,        // 11
    HAL_CMU_MOD_P_UART1,        // 12
    HAL_CMU_MOD_P_UART2,        // 13
    HAL_CMU_MOD_P_PCM,          // 14
    HAL_CMU_MOD_P_I2S0,         // 15
    HAL_CMU_MOD_P_SPDIF0,       // 16
    HAL_CMU_MOD_P_SEC_ENG,      // 20
    HAL_CMU_MOD_P_I2S1,         // 25
    // OCLK/ORST
    HAL_CMU_MOD_O_SLEEP,        // 0
    HAL_CMU_MOD_O_USB,          // 1
    HAL_CMU_MOD_O_USB32K,       // 2
    HAL_CMU_MOD_O_PSRAM1G,      // 3
    HAL_CMU_MOD_O_PSRAM200,     // 4
    HAL_CMU_MOD_O_FLASH,        // 5
    HAL_CMU_MOD_O_SDMMC,        // 6
    HAL_CMU_MOD_O_WDT,          // 7
    HAL_CMU_MOD_O_TIMER0,       // 8
    HAL_CMU_MOD_O_TIMER1,       // 9
    HAL_CMU_MOD_O_TIMER2,       // 10
    HAL_CMU_MOD_O_I2C0,         // 11
    HAL_CMU_MOD_O_I2C1,         // 12
    HAL_CMU_MOD_O_SPI,          // 13
    HAL_CMU_MOD_O_SLCD,         // 14
    HAL_CMU_MOD_O_SPI_ITN,      // 15
    HAL_CMU_MOD_O_SPI_PHY,      // 16
    HAL_CMU_MOD_O_UART0,        // 17
    HAL_CMU_MOD_O_UART1,        // 18
    HAL_CMU_MOD_O_UART2,        // 19
    HAL_CMU_MOD_O_PCM,          // 20
    HAL_CMU_MOD_O_I2S0,         // 21
    HAL_CMU_MOD_O_SPDIF0,       // 22
    HAL_CMU_MOD_O_I2S1,         // 23
    HAL_CMU_MOD_O_A7,           // 24
    HAL_CMU_MOD_O_TSF,          // 25
    HAL_CMU_MOD_O_WDT_AP,       // 26
    HAL_CMU_MOD_O_TIMER0_AP,    // 27
    HAL_CMU_MOD_O_TIMER1_AP,    // 28
    HAL_CMU_MOD_O_FLASH1,       // 29
    HAL_CMU_MOD_O_I2C2,         // 30
    HAL_CMU_MOD_O_UART3,        // 31

    // AON ACLK/ARST
    HAL_CMU_AON_A_CMU,          // 0
    HAL_CMU_AON_A_GPIO,         // 1
    HAL_CMU_AON_A_GPIO_INT,     // 2
    HAL_CMU_AON_A_WDT,          // 3
    HAL_CMU_AON_A_PWM,          // 4
    HAL_CMU_AON_A_TIMER,        // 5
    HAL_CMU_AON_A_IOMUX,        // 6
    HAL_CMU_AON_A_APBC,         // 8
    HAL_CMU_AON_A_H2H_MCU,      // 9
    HAL_CMU_AON_A_PSC,          // 10
    // AON OCLK/ORST
    HAL_CMU_AON_O_WDT,          // 13
    HAL_CMU_AON_O_TIMER,        // 14
    HAL_CMU_AON_O_GPIO,         // 15
    HAL_CMU_AON_O_PWM0,         // 16
    HAL_CMU_AON_O_PWM1,         // 17
    HAL_CMU_AON_O_PWM2,         // 18
    HAL_CMU_AON_O_PWM3,         // 19
    HAL_CMU_AON_O_IOMUX,        // 20
    HAL_CMU_AON_O_SLP32K,       // 21
    HAL_CMU_AON_O_SLP26M,       // 22
    HAL_CMU_AON_O_SPIDPD,       // 23
    HAL_CMU_AON_O_WLAN32K,      // 24
    HAL_CMU_AON_O_WLAN26M,      // 25
    HAL_CMU_AON_O_BTAON,        // 26
    // AON SUBSYS
    HAL_CMU_AON_MCU,            // 2
    HAL_CMU_AON_CODEC,          // 3
    HAL_CMU_AON_WF,             // 4
    HAL_CMU_AON_BT,             // 5
    HAL_CMU_AON_MCUCPU,         // 6
    HAL_CMU_AON_WFCPU,          // 7
    HAL_CMU_AON_BTCPU,          // 8
    HAL_CMU_AON_GLOBAL,         // 9

    HAL_CMU_MOD_QTY,

    HAL_CMU_MOD_GLOBAL = HAL_CMU_AON_GLOBAL,
    HAL_CMU_MOD_BT = HAL_CMU_AON_BT,
    HAL_CMU_MOD_BTCPU = HAL_CMU_AON_BTCPU,

    HAL_CMU_MOD_P_PWM = HAL_CMU_AON_A_PWM,
    HAL_CMU_MOD_O_PWM0 = HAL_CMU_AON_O_PWM0,
    HAL_CMU_MOD_O_PWM1 = HAL_CMU_AON_O_PWM1,
    HAL_CMU_MOD_O_PWM2 = HAL_CMU_AON_O_PWM2,
    HAL_CMU_MOD_O_PWM3 = HAL_CMU_AON_O_PWM3,
    HAL_CMU_H_DCACHE = HAL_CMU_MOD_H_CACHE0,
    HAL_CMU_H_ICACHE = HAL_CMU_MOD_H_CACHE1,

};

enum HAL_CMU_CLOCK_OUT_ID_T {
    HAL_CMU_CLOCK_OUT_AON_32K           = 0x00,
    HAL_CMU_CLOCK_OUT_AON_OSC           = 0x01,
    HAL_CMU_CLOCK_OUT_AON_OSCX2         = 0x02,
    HAL_CMU_CLOCK_OUT_AON_DIG_OSCX2     = 0x03,
    HAL_CMU_CLOCK_OUT_AON_DIG_OSCX4     = 0x04,
    HAL_CMU_CLOCK_OUT_AON_PER           = 0x05,
    HAL_CMU_CLOCK_OUT_AON_USB           = 0x06,
    HAL_CMU_CLOCK_OUT_AON_DCDC0         = 0x07,
    HAL_CMU_CLOCK_OUT_AON_CHCLK         = 0x08,
    HAL_CMU_CLOCK_OUT_AON_SPDIF0        = 0x09,
    HAL_CMU_CLOCK_OUT_AON_MCU           = 0x0A,
    HAL_CMU_CLOCK_OUT_AON_FLASH         = 0x0B,
    HAL_CMU_CLOCK_OUT_AON_SYS           = 0x17,

    HAL_CMU_CLOCK_OUT_BT_NONE           = 0x40,
    HAL_CMU_CLOCK_OUT_BT_32K            = 0x41,
    HAL_CMU_CLOCK_OUT_BT_SYS            = 0x42,
    HAL_CMU_CLOCK_OUT_BT_OSCX2          = 0x43,
    HAL_CMU_CLOCK_OUT_BT_OSC_2          = 0x44,
    HAL_CMU_CLOCK_OUT_BT_ADC            = 0x45,
    HAL_CMU_CLOCK_OUT_BT_ADCD3          = 0x46,
    HAL_CMU_CLOCK_OUT_BT_DAC            = 0x47,
    HAL_CMU_CLOCK_OUT_BT_DACD2          = 0x48,
    HAL_CMU_CLOCK_OUT_BT_DACD4          = 0x49,

    HAL_CMU_CLOCK_OUT_MCU_32K           = 0x60,
    HAL_CMU_CLOCK_OUT_MCU_SYS           = 0x61,
    HAL_CMU_CLOCK_OUT_MCU_FLASH         = 0x62,
    HAL_CMU_CLOCK_OUT_MCU_USB           = 0x63,
    HAL_CMU_CLOCK_OUT_MCU_PCLK          = 0x64,
    HAL_CMU_CLOCK_OUT_MCU_I2S0          = 0x65,
    HAL_CMU_CLOCK_OUT_MCU_PCM           = 0x66,
    HAL_CMU_CLOCK_OUT_MCU_SPDIF0        = 0x67,
    HAL_CMU_CLOCK_OUT_MCU_SDMMC         = 0x68,
    HAL_CMU_CLOCK_OUT_MCU_SPI2          = 0x69,
    HAL_CMU_CLOCK_OUT_MCU_SPI0          = 0x6A,
    HAL_CMU_CLOCK_OUT_MCU_SPI1          = 0x6B,

    HAL_CMU_CLOCK_OUT_CODEC_ADC_ANA     = 0x80,
    HAL_CMU_CLOCK_OUT_CODEC_CODEC       = 0x81,
    HAL_CMU_CLOCK_OUT_CODEC_IIR         = 0x82,
    HAL_CMU_CLOCK_OUT_CODEC_RS_DAC      = 0x83,
    HAL_CMU_CLOCK_OUT_CODEC_RS_ADC      = 0x84,
    HAL_CMU_CLOCK_OUT_CODEC_HCLK        = 0x85,
};

enum HAL_CMU_I2S_MCLK_ID_T {
    HAL_CMU_I2S_MCLK_PLLCODEC           = 0x00,
    HAL_CMU_I2S_MCLK_CODEC              = 0x01,
    HAL_CMU_I2S_MCLK_PLLIIR             = 0x02,
    HAL_CMU_I2S_MCLK_PLLRS              = 0x03,
    HAL_CMU_I2S_MCLK_PLLSPDIF0          = 0x04,
    HAL_CMU_I2S_MCLK_PLLPCM             = 0x05,
    HAL_CMU_I2S_MCLK_PER                = 0x06,
    HAL_CMU_I2S_MCLK_CLK_OUT            = 0x07,
};

enum HAL_I2S_ID_T {
    HAL_I2S_ID_0 = 0,
    HAL_I2S_ID_1,

    HAL_I2S_ID_QTY,
};
#define HAL_I2S_ID_T                        HAL_I2S_ID_T


};

};


int hal_cmu_fast_timer_offline(void);





uint32_t hal_cmu_get_aon_chip_id(void);

uint32_t hal_cmu_get_aon_revision_id(void);

void hal_cmu_cp_enable(uint32_t sp, uint32_t entry);

void hal_cmu_cp_disable(void);

uint32_t hal_cmu_cp_get_entry_addr(void);

#ifdef __cplusplus
}
#endif

#endif

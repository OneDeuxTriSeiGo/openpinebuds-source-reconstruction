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
#ifndef __HAL_IOMUX_BEST2300P_H__
#define __HAL_IOMUX_BEST2300P_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

#ifndef ROM_BUILD
#define PMU_HAS_LED_PIN
#endif

enum HAL_IOMUX_PIN_T {
    HAL_IOMUX_PIN_P0_0 = 0,
    HAL_IOMUX_PIN_P0_1,
    HAL_IOMUX_PIN_P0_2,
    HAL_IOMUX_PIN_P0_3,
    HAL_IOMUX_PIN_P0_4,
    HAL_IOMUX_PIN_P0_5,
    HAL_IOMUX_PIN_P0_6,
    HAL_IOMUX_PIN_P0_7,

    HAL_IOMUX_PIN_P1_0,
    HAL_IOMUX_PIN_P1_1,
    HAL_IOMUX_PIN_P1_2,
    HAL_IOMUX_PIN_P1_3,
    HAL_IOMUX_PIN_P1_4,
    HAL_IOMUX_PIN_P1_5,
    HAL_IOMUX_PIN_P1_6,
    HAL_IOMUX_PIN_P1_7,

    HAL_IOMUX_PIN_P2_0,
    HAL_IOMUX_PIN_P2_1,
    HAL_IOMUX_PIN_P2_2,
    HAL_IOMUX_PIN_P2_3,
    HAL_IOMUX_PIN_P2_4,
    HAL_IOMUX_PIN_P2_5,
    HAL_IOMUX_PIN_P2_6,
    HAL_IOMUX_PIN_P2_7,

    HAL_IOMUX_PIN_P3_0,
    HAL_IOMUX_PIN_P3_1,
    HAL_IOMUX_PIN_P3_2,
    HAL_IOMUX_PIN_P3_3,
    HAL_IOMUX_PIN_P3_4,
    HAL_IOMUX_PIN_P3_5,
    HAL_IOMUX_PIN_P3_6,
    HAL_IOMUX_PIN_P3_7,

    HAL_IOMUX_PIN_NUM,

    HAL_IOMUX_PIN_LED1 = HAL_IOMUX_PIN_NUM,
    HAL_IOMUX_PIN_LED2,

    HAL_IOMUX_PIN_LED_NUM,
};

enum HAL_GPIO_PIN_T {
    HAL_GPIO_PIN_P0_0 = HAL_IOMUX_PIN_P0_0,
    HAL_GPIO_PIN_P0_1 = HAL_IOMUX_PIN_P0_1,
    HAL_GPIO_PIN_P0_2 = HAL_IOMUX_PIN_P0_2,
    HAL_GPIO_PIN_P0_3 = HAL_IOMUX_PIN_P0_3,
    HAL_GPIO_PIN_P0_4 = HAL_IOMUX_PIN_P0_4,
    HAL_GPIO_PIN_P0_5 = HAL_IOMUX_PIN_P0_5,
    HAL_GPIO_PIN_P0_6 = HAL_IOMUX_PIN_P0_6,
    HAL_GPIO_PIN_P0_7 = HAL_IOMUX_PIN_P0_7,

    HAL_GPIO_PIN_P1_0 = HAL_IOMUX_PIN_P1_0,
    HAL_GPIO_PIN_P1_1 = HAL_IOMUX_PIN_P1_1,
    HAL_GPIO_PIN_P1_2 = HAL_IOMUX_PIN_P1_2,
    HAL_GPIO_PIN_P1_3 = HAL_IOMUX_PIN_P1_3,
    HAL_GPIO_PIN_P1_4 = HAL_IOMUX_PIN_P1_4,
    HAL_GPIO_PIN_P1_5 = HAL_IOMUX_PIN_P1_5,
    HAL_GPIO_PIN_P1_6 = HAL_IOMUX_PIN_P1_6,
    HAL_GPIO_PIN_P1_7 = HAL_IOMUX_PIN_P1_7,

    HAL_GPIO_PIN_P2_0 = HAL_IOMUX_PIN_P2_0,
    HAL_GPIO_PIN_P2_1 = HAL_IOMUX_PIN_P2_1,
    HAL_GPIO_PIN_P2_2 = HAL_IOMUX_PIN_P2_2,
    HAL_GPIO_PIN_P2_3 = HAL_IOMUX_PIN_P2_3,
    HAL_GPIO_PIN_P2_4 = HAL_IOMUX_PIN_P2_4,
    HAL_GPIO_PIN_P2_5 = HAL_IOMUX_PIN_P2_5,
    HAL_GPIO_PIN_P2_6 = HAL_IOMUX_PIN_P2_6,
    HAL_GPIO_PIN_P2_7 = HAL_IOMUX_PIN_P2_7,

    HAL_GPIO_PIN_P3_0 = HAL_IOMUX_PIN_P3_0,
    HAL_GPIO_PIN_P3_1 = HAL_IOMUX_PIN_P3_1,
    HAL_GPIO_PIN_P3_2 = HAL_IOMUX_PIN_P3_2,
    HAL_GPIO_PIN_P3_3 = HAL_IOMUX_PIN_P3_3,
    HAL_GPIO_PIN_P3_4 = HAL_IOMUX_PIN_P3_4,
    HAL_GPIO_PIN_P3_5 = HAL_IOMUX_PIN_P3_5,
    HAL_GPIO_PIN_P3_6 = HAL_IOMUX_PIN_P3_6,
    HAL_GPIO_PIN_P3_7 = HAL_IOMUX_PIN_P3_7,

    HAL_GPIO_PIN_NUM = HAL_IOMUX_PIN_NUM,

    HAL_GPIO_PIN_LED1 = HAL_IOMUX_PIN_LED1,
    HAL_GPIO_PIN_LED2 = HAL_IOMUX_PIN_LED2,

    HAL_GPIO_PIN_LED_NUM = HAL_IOMUX_PIN_LED_NUM,
};

enum HAL_IOMUX_FUNCTION_T {
    HAL_IOMUX_FUNC_NONE = 0,
    HAL_IOMUX_FUNC_GPIO,
    HAL_IOMUX_FUNC_AS_GPIO = HAL_IOMUX_FUNC_GPIO,
    HAL_IOMUX_FUNC_BT_UART_CTS,
    HAL_IOMUX_FUNC_BT_UART_RTS,
    HAL_IOMUX_FUNC_BT_UART_RX,
    HAL_IOMUX_FUNC_BT_UART_TX,
    HAL_IOMUX_FUNC_CLK_32K_IN,
    HAL_IOMUX_FUNC_CLK_REQ_IN,
    HAL_IOMUX_FUNC_CLK_REQ_OUT,
    HAL_IOMUX_FUNC_CLK_OUT,
    HAL_IOMUX_FUNC_I2C_M0_SCL,
    HAL_IOMUX_FUNC_I2C_M0_SDA,
    HAL_IOMUX_FUNC_I2C_M1_SCL,
    HAL_IOMUX_FUNC_I2C_M2_SCL,
    HAL_IOMUX_FUNC_I2C_M2_SDA,
    HAL_IOMUX_FUNC_I2C_M1_SDA,
    HAL_IOMUX_FUNC_I2S_MCLK,
    HAL_IOMUX_FUNC_I2S0_SCK,
    HAL_IOMUX_FUNC_I2S0_SDI0,
    HAL_IOMUX_FUNC_I2S0_SDI1,
    HAL_IOMUX_FUNC_I2S0_SDI2,
    HAL_IOMUX_FUNC_I2S0_SDI3,
    HAL_IOMUX_FUNC_I2S0_SDO0,
    HAL_IOMUX_FUNC_I2S0_SDO1,
    HAL_IOMUX_FUNC_I2S0_SDO2,
    HAL_IOMUX_FUNC_I2S0_SDO3,
    HAL_IOMUX_FUNC_I2S0_WS,
    HAL_IOMUX_FUNC_I2S1_SCK,
    HAL_IOMUX_FUNC_I2S1_SDI0,
    HAL_IOMUX_FUNC_I2S1_SDI1,
    HAL_IOMUX_FUNC_I2S1_SDI2,
    HAL_IOMUX_FUNC_I2S1_SDI3,
    HAL_IOMUX_FUNC_I2S1_SDO0,
    HAL_IOMUX_FUNC_I2S1_SDO1,
    HAL_IOMUX_FUNC_I2S1_SDO2,
    HAL_IOMUX_FUNC_I2S1_SDO3,
    HAL_IOMUX_FUNC_I2S1_WS,
    HAL_IOMUX_FUNC_PCM_CLK,
    HAL_IOMUX_FUNC_PCM_DI,
    HAL_IOMUX_FUNC_PCM_DO,
    HAL_IOMUX_FUNC_PCM_FSYNC,
    HAL_IOMUX_FUNC_PDM0_CK,
    HAL_IOMUX_FUNC_PDM0_D,
    HAL_IOMUX_FUNC_PDM1_CK,
    HAL_IOMUX_FUNC_PDM1_D,
    HAL_IOMUX_FUNC_PDM2_CK,
    HAL_IOMUX_FUNC_PDM2_D,
    HAL_IOMUX_FUNC_PWM0,
    HAL_IOMUX_FUNC_PWM1,
    HAL_IOMUX_FUNC_PWM2,
    HAL_IOMUX_FUNC_PWM3,
    HAL_IOMUX_FUNC_PWM4,
    HAL_IOMUX_FUNC_PWM5,
    HAL_IOMUX_FUNC_PWM6,
    HAL_IOMUX_FUNC_PWM7,
    HAL_IOMUX_FUNC_SDMMC_CLK,
    HAL_IOMUX_FUNC_SDMMC_CMD,
    HAL_IOMUX_FUNC_SDMMC_DATA0,
    HAL_IOMUX_FUNC_SDMMC_DATA1,
    HAL_IOMUX_FUNC_SDMMC_DATA2,
    HAL_IOMUX_FUNC_SDMMC_DATA3,
    HAL_IOMUX_FUNC_SDMMC_DATA4,
    HAL_IOMUX_FUNC_SDMMC_DATA5,
    HAL_IOMUX_FUNC_SDMMC_DATA6,
    HAL_IOMUX_FUNC_SDMMC_DATA7,
    HAL_IOMUX_FUNC_SPDIF0_DI,
    HAL_IOMUX_FUNC_SPDIF0_DO,
    HAL_IOMUX_FUNC_SPI_CLK,//spi0
    HAL_IOMUX_FUNC_SPI_CS0,
    HAL_IOMUX_FUNC_SPI_CS1,
    HAL_IOMUX_FUNC_SPI_CS2,
    HAL_IOMUX_FUNC_SPI_CS3,
    HAL_IOMUX_FUNC_SPI_DCN,
    HAL_IOMUX_FUNC_SPI_DI0,
    HAL_IOMUX_FUNC_SPI_DI1,
    HAL_IOMUX_FUNC_SPI_DI2,
    HAL_IOMUX_FUNC_SPI_DI3,
    HAL_IOMUX_FUNC_SPI_DIO,
    HAL_IOMUX_FUNC_SPILCD_CLK,//spi1
    HAL_IOMUX_FUNC_SPILCD_CS0,
    HAL_IOMUX_FUNC_SPILCD_CS1,
    HAL_IOMUX_FUNC_SPILCD_CS2,
    HAL_IOMUX_FUNC_SPILCD_CS3,
    HAL_IOMUX_FUNC_SPILCD_DCN,
    HAL_IOMUX_FUNC_SPILCD_DI0,
    HAL_IOMUX_FUNC_SPILCD_DI1,
    HAL_IOMUX_FUNC_SPILCD_DI2,
    HAL_IOMUX_FUNC_SPILCD_DI3,
    HAL_IOMUX_FUNC_SPILCD_DIO,
    HAL_IOMUX_FUNC_UART0_RX,
    HAL_IOMUX_FUNC_UART0_TX,
    HAL_IOMUX_FUNC_UART1_CTS,
    HAL_IOMUX_FUNC_UART1_RTS,
    HAL_IOMUX_FUNC_UART1_RX,
    HAL_IOMUX_FUNC_UART1_TX,
    HAL_IOMUX_FUNC_UART2_RX,
    HAL_IOMUX_FUNC_UART2_TX,
    HAL_IOMUX_FUNC_UART2_CTS,
    HAL_IOMUX_FUNC_UART2_RTS,
    HAL_IOMUX_FUNC_UART3_RX,
    HAL_IOMUX_FUNC_UART3_TX,
    HAL_IOMUX_FUNC_UART3_CTS,
    HAL_IOMUX_FUNC_UART3_RTS,
    HAL_IOMUX_FUNC_WF_UART_CTS,
    HAL_IOMUX_FUNC_WF_UART_RTS,
    HAL_IOMUX_FUNC_WF_UART_RX,
    HAL_IOMUX_FUNC_WF_UART_TX,
    HAL_IOMUX_FUNC_WF_FEM_CNTL0,//no ctrl1
    HAL_IOMUX_FUNC_WF_FEM_CNTL2,
    HAL_IOMUX_FUNC_WF_FEM_CNTL3,
    HAL_IOMUX_FUNC_WF_FEM_CNTL4,
    HAL_IOMUX_FUNC_WF_FEM_CNTL5,
    HAL_IOMUX_FUNC_WF_FEM_CNTL6,
    HAL_IOMUX_FUNC_WF_FEM_CNTL7,
    HAL_IOMUX_FUNC_WF_FEM_CNTL8,
    HAL_IOMUX_FUNC_WF_FEM_CNTL9,
    HAL_IOMUX_FUNC_WF_SDIO_CLK,
    HAL_IOMUX_FUNC_WF_SDIO_CMD,
    HAL_IOMUX_FUNC_WF_SDIO_DATA0,
    HAL_IOMUX_FUNC_WF_SDIO_DATA1,
    HAL_IOMUX_FUNC_WF_SDIO_DATA2,
    HAL_IOMUX_FUNC_WF_SDIO_DATA3,
    HAL_IOMUX_FUNC_WF_WAKE_HOST,
    HAL_IOMUX_FUNC_WF_TXON,
    HAL_IOMUX_FUNC_DISPLAY_TE,
    HAL_IOMUX_FUNC_DISPLAY_BL_PWM,
    HAL_IOMUX_FUNC_DISPLAY_BL_EN,
    HAL_IOMUX_FUNC_DISPLAY_SPI_CLK,
    HAL_IOMUX_FUNC_DISPLAY_SPI_CS,
    HAL_IOMUX_FUNC_DISPLAY_SPI_DIO,
    HAL_IOMUX_FUNC_DISPLAY_SPI_DO1_DCN,
    HAL_IOMUX_FUNC_DISPLAY_SPI_DO2,
    HAL_IOMUX_FUNC_DISPLAY_SPI_DO3,
    HAL_IOMUX_FUNC_DISPLAY_SPI_DI,
    HAL_IOMUX_FUNC_IR_RX,
    HAL_IOMUX_FUNC_IR_TX,
    HAL_IOMUX_FUNC_END
};

enum HAL_IOMUX_ISPI_ACCESS_T {
    HAL_IOMUX_ISPI_BT_RF            = (1 << 0),
    HAL_IOMUX_ISPI_BT_PMU           = (1 << 1),
    HAL_IOMUX_ISPI_BT_ANA           = (1 << 2),
    HAL_IOMUX_ISPI_MCU_RF           = (1 << 3),
    HAL_IOMUX_ISPI_MCU_PMU          = (1 << 4),
    HAL_IOMUX_ISPI_MCU_ANA          = (1 << 5),
};

void hal_iomux_set_i2s_mclk(void);

void hal_iomux_set_i2s1(void);

void hal_iomux_set_mcu_clock_out(void);

void hal_iomux_set_bt_clock_out(void);

uint32_t hal_iomux_set_io_drv(enum HAL_IOMUX_PIN_T pin, uint32_t val);

void hal_iomux_set_dsi_te(void);

void hal_iomux_set_wf_fem(int rf_switch);

#ifdef __cplusplus
}
#endif

#endif

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
#ifndef __HAL_DMACFG_BEST2300P_H__
#define __HAL_DMACFG_BEST2300P_H__

#define AUDMA_PERIPH_NUM                        16
#define GPDMA_PERIPH_NUM                        16

#define AUDMA_CHAN_NUM                          8
#define GPDMA_CHAN_NUM                          8

#define AUDMA_CHAN_START                        (0)
#define GPDMA_CHAN_START                        (0)

static const uint32_t gpdma_fifo_addr[AUDMA_PERIPH_NUM] = {
    CODEC_BASE + 0x01C,     // CODEC RX
    CODEC_BASE + 0x01C,     // CODEC TX
    CODEC_BASE + 0x034,     // DSD RX
    CODEC_BASE + 0x034,     // DSD TX
    CODEC_BASE + 0x058,     // CODEC TX2
#ifdef AF_DEVICE_I2S
    I2S1_BASE + 0x200,      // I2S0 RX
    I2S1_BASE + 0x240,      // I2S0 TX
    I2S0_BASE + 0x200,      // I2S0 RX
    I2S0_BASE + 0x240,      // I2S0 TX
#else
    I2C1_BASE + 0x010,      // I2C1 RX
    I2C1_BASE + 0x010,      // I2C1 TX
    SPI_BASE + 0x008,       // SPI RX
    SPI_BASE + 0x008,       // SPI TX
#endif
    SDMMC_BASE + 0x200,     // SDMMC
    BTPCM_BASE + 0x1C0,     // BTPCM RX
    BTPCM_BASE + 0x1C8,     // BTPCM TX
    UART2_BASE + 0x000,     // UART2 RX
    UART2_BASE + 0x000,     // UART2 TX
    UART1_BASE + 0x000,     // UART1 RX
    UART1_BASE + 0x000,     // UART1 TX
};

static const enum HAL_DMA_PERIPH_T gpdma_fifo_periph[AUDMA_PERIPH_NUM] = {
    HAL_AUDMA_CODEC_RX,
    HAL_AUDMA_CODEC_TX,
    HAL_AUDMA_DSD_RX,
    HAL_AUDMA_DSD_TX,
    HAL_DMA_PERIPH_NULL,
#ifdef AF_DEVICE_I2S
    HAL_AUDMA_I2S0_RX,
    HAL_AUDMA_I2S0_TX,
    HAL_AUDMA_I2S1_RX,
    HAL_AUDMA_I2S1_TX,
#else
    HAL_GPDMA_I2C1_RX,
    HAL_GPDMA_I2C1_TX,
    HAL_GPDMA_SPI_RX,
    HAL_GPDMA_SPI_TX,
#endif
    HAL_GPDMA_SDMMC,
    HAL_AUDMA_BTPCM_RX,
    HAL_AUDMA_BTPCM_TX,
    HAL_GPDMA_UART2_RX,
    HAL_GPDMA_UART2_TX,
    HAL_GPDMA_UART1_RX,
    HAL_GPDMA_UART1_TX,
};

static const uint32_t audma_fifo_addr[GPDMA_PERIPH_NUM] = {
    CODEC_BASE + 0x01C,     // CODEC RX
    CODEC_BASE + 0x01C,     // CODEC TX
    IRDA_BASE + 0x000,      // IR RX
    IRDA_BASE + 0x004,      // IR TX
    UART0_BASE + 0x000,     // UART0 RX
    UART0_BASE + 0x000,     // UART0 TX
    UART2_BASE + 0x000,     // UART2 RX
    UART2_BASE + 0x000,     // UART2 TX
#ifndef SPILCD_DMA_ENABLE
    BTDUMP_BASE + 0x34,     // BTDUMP
    0,                      // NULL
#else
    SPILCD_BASE + 0x008,    // SPILCD RX
    SPILCD_BASE + 0x008,    // SPILCD TX
#endif
#ifdef AF_DEVICE_I2S
    I2S0_BASE + 0x200,      // I2S0 RX
    I2S0_BASE + 0x240,      // I2S0 TX
    I2S1_BASE + 0x200,      // I2S1 RX
    I2S1_BASE + 0x240,      // I2S1 TX
#else
    BTPCM_BASE + 0x1C0,     // BTPCM RX
    BTPCM_BASE + 0x1C8,     // BTPCM TX
    UART1_BASE + 0x000,     // UART1 RX
    UART1_BASE + 0x000,     // UART1 TX
#endif
    FLASH_CTRL_BASE + 0x008,// FLASH0
    FLASH1_CTRL_BASE + 0x008,// FLASH0
};

static const enum HAL_DMA_PERIPH_T audma_fifo_periph[GPDMA_PERIPH_NUM] = {
    HAL_AUDMA_CODEC_RX,
    HAL_AUDMA_CODEC_TX,
    HAL_GPDMA_IR_RX,
    HAL_GPDMA_IR_TX,
    HAL_GPDMA_UART0_RX,
    HAL_GPDMA_UART0_TX,
    HAL_GPDMA_UART2_RX,
    HAL_GPDMA_UART2_TX,
#ifndef SPILCD_DMA_ENABLE
    HAL_AUDMA_BTDUMP,
    HAL_DMA_PERIPH_NULL,
#else
    HAL_GPDMA_SPILCD_RX,
    HAL_GPDMA_SPILCD_TX,
#endif
#ifdef AF_DEVICE_I2S
    HAL_AUDMA_I2S0_RX,      // I2S0 RX
    HAL_AUDMA_I2S0_TX,      // I2S0 TX
    HAL_AUDMA_I2S1_RX,
    HAL_AUDMA_I2S1_TX,
#else
    HAL_AUDMA_BTPCM_RX,
    HAL_AUDMA_BTPCM_TX,
    HAL_GPDMA_UART1_RX,
    HAL_GPDMA_UART1_TX,
#endif
    HAL_GPDMA_FLASH0,
    HAL_GPDMA_FLASH1,
};

#endif

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
#ifndef __SPI_H__
#define __SPI_H__

#include "hal_spi.h"
#include "hal_gpio.h"
#include "hal_iomux.h"
#include "hal_iomux_best2003.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum SPI_WORK_MODE
{
    SPI_WORK_MODE_0,                  // CPOL = 0; CPHA = 0
    SPI_WORK_MODE_2,                  // CPOL = 1; CPHA = 0
    SPI_WORK_MODE_1,                  // CPOL = 0; CPHA = 1
    SPI_WORK_MODE_3,                  // CPOL = 1; CPHA = 1
};

enum SPI_TRANSFER_MODE
{
    SPI_TRANSFER_DMA,
    SPI_TRANSFER_NORMAL,
};

struct SpiResource
{
    uint32_t num;
    uint32_t speed;
    enum SPI_TRANSFER_MODE transmode;
    enum SPI_WORK_MODE mode;
    uint32_t data_size;
    uint32_t Csnum;
    uint32_t spi_cs_soft;
    uint32_t spi_clk_pin;
    uint32_t spi_mosi_pin;
    uint32_t spi_miso_pin;
    uint32_t spi_cs_pin;
};

struct SPI_CTX_OBJ_T
{
    enum HAL_IOMUX_PIN_T spi_pin_CS0;
    enum HAL_IOMUX_FUNCTION_T spi_fun_DI0;
    enum HAL_IOMUX_FUNCTION_T spi_fun_CLK;
    enum HAL_IOMUX_FUNCTION_T spi_fun_CS0;
    enum HAL_IOMUX_FUNCTION_T spi_fun_DIO;
    osSemaphoreId spi_dma_semaphore;
    osMutexId spi_mutex_id;
    int (*spi_open)(const struct HAL_SPI_CFG_T *cfg);
    int (*spi_dma_send)(const void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler);
    int (*spi_dma_recv)(const void *cmd, void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler);
    int (*spi_send)(const void *data, uint32_t len);
    int (*spi_recv)(const void *cmd, void *data, uint32_t len);
    void (*spi_dma_irq)(int error);
    int (*spi_close)(uint32_t cs);
};

struct SpiDevice
{
    uint32_t spi_id;
    struct SpiResource resource;
    struct HAL_SPI_CFG_T spidevcfg;
};

void littos_spi_test(void);



#ifdef __cplusplus
}
#endif

#endif
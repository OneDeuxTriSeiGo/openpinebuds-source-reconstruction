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
#ifndef __UART_H__
#define __UART_H__

#include "uart_if.h"
#include "uart_core.h"
#include "hal_uart.h"
#include "kfifo.h"
#include "cmsis_os.h"
#ifdef __cplusplus
extern "C" {
#endif

#define UART_DEV_SERVICE_NAME_PREFIX "HDF_PLATFORM_UART%d"
#define MAX_DEV_NAME_SIZE 32

struct UartResource {
    uint32_t num;      /* UART port num */
    uint32_t baudrate; /* Default baudrate */
    uint32_t wlen;     /* Default word length */
    uint32_t parity;   /* Default parity */
    uint32_t stopBit;  /* Default stop bits */
    bool txDMA;
    bool rxDMA;
};

enum UartDeviceState {
    UART_DEVICE_UNINITIALIZED = 0x0u,
    UART_DEVICE_INITIALIZED = 0x1u,
};

struct UART_CTX_OBJ {
    uint8_t *buffer;
    bool txDMA;
    bool rxDMA;
    bool isblock;
    struct kfifo fifo;
    osSemaphoreId rx_sem;
    osSemaphoreId tx_sem;
    void (*uart_dma_rx_handler)(uint32_t xfer_size, int dma_error, union HAL_UART_IRQ_T status);
    void (*uart_dma_tx_handler)(uint32_t xfer_size, int dma_error);
};

struct UartDevice {
    struct IDeviceIoService ioService;
    struct UartResource resource;
    struct HAL_UART_CFG_T config;
    uint32_t uart_id;
    bool init_flag;
    uint32_t TransMode;
};

enum {
    UART_READ = 0,
    UART_WRITE
};

int32_t UartDispatch(struct HdfDeviceIoClient *client, int cmdId, struct HdfSBuf *data, struct HdfSBuf *reply);

#ifdef __cplusplus
}
#endif

#endif
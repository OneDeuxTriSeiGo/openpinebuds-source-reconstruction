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
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "hal_iomux.h"
#include "hal/hal_timer.h"
#include "device_resource_if.h"
#include "hal_trace.h"
#include "hal_cache.h"
#define HDF_LOG_TAG uart_dev

#define UART_FIFO_MAX_BUFFER   2048
#define UART_DMA_RING_BUFFER_SIZE  256 // mast be 2^n

static __SRAMBSS unsigned char _hal_uart_buf[UART_DMA_RING_BUFFER_SIZE];
static __SRAMBSS unsigned char _hal_uart1_buf[UART_DMA_RING_BUFFER_SIZE];
static __SRAMBSS unsigned char _hal_uart2_buf[UART_DMA_RING_BUFFER_SIZE];
static __SRAMBSS unsigned char _hal_uart3_buf[UART_DMA_RING_BUFFER_SIZE];

static struct UART_CTX_OBJ uart_ctx[4] = {0};
static unsigned char *uart_kfifo_buffer[4]={NULL,NULL,NULL,NULL};
struct HAL_UART_CFG_T low_uart_cfg = { // used for tgdb cli console
    .parity = HAL_UART_PARITY_NONE,
    .stop = HAL_UART_STOP_BITS_1,
    .data = HAL_UART_DATA_BITS_8,
    .flow = HAL_UART_FLOW_CONTROL_NONE,
    .tx_level = HAL_UART_FIFO_LEVEL_7_8,
    .rx_level = HAL_UART_FIFO_LEVEL_1_8,
    .baud = 0,
    .dma_rx = false,
    .dma_tx = false,
    .dma_rx_stop_on_err = false,
};

static void hal_set_uart_iomux(enum HAL_UART_ID_T uart_id)
{
    if (uart_id == 0) {
        hal_iomux_set_uart0();
    } else if (uart_id == 1) {
        hal_iomux_set_uart1();
    } else if (uart_id == 2)
    {
        hal_iomux_set_uart2();
    }else
    {
        hal_iomux_set_uart3();
    }
}

static bool uart_rx_flag = false;

static void hal_uart_rx_start(uint32_t uart_id)
{
    struct HAL_DMA_DESC_T dma_desc_rx;
    unsigned int desc_cnt = 1;

    union HAL_UART_IRQ_T mask;
    mask.reg = 0;
    mask.BE = 0;
    mask.FE = 0;
    mask.OE = 0;
    mask.PE = 0;
    mask.RT = 1;

    hal_uart_dma_recv_mask(uart_id, uart_ctx[uart_id].buffer, UART_DMA_RING_BUFFER_SIZE, &dma_desc_rx, &desc_cnt, &mask);
}

static void uart_rx_handler(enum HAL_UART_ID_T id, union HAL_UART_IRQ_T status)
{
    uart_rx_flag = true;
    osStatus ret;

    if (status.TX) {
        ret = osSemaphoreRelease(&uart_ctx[id].tx_sem);
        ASSERT(ret == osOK, "%s: Failed to release write_sem: %d", __func__, ret);
    }

    if (status.RX || status.RT) {
        ret = osSemaphoreRelease(&uart_ctx[id].rx_sem);
        ASSERT(ret == osOK, "%s: Failed to release read_sem: %d", __func__, ret);
    }
}

static void _uart_dma_rx_handler(uint32_t xfer_size, int dma_error, union HAL_UART_IRQ_T status)
{
    uint32_t len = 0;
    uint32_t uartid = 0;

    len = kfifo_put(&uart_ctx[uartid].fifo, uart_ctx[uartid].buffer, xfer_size);
    if (len < xfer_size) {
        printf("%s ringbuf is full have %d need %d\r", __FUNCTION__, (int)len, (int)xfer_size);
        return;
    }

    memset(uart_ctx[uartid].buffer, 0, UART_DMA_RING_BUFFER_SIZE);
    osSemaphoreRelease(&uart_ctx[uartid].rx_sem);
    hal_uart_rx_start(uartid);
}

static void _uart_dma_tx_handler(uint32_t xfer_size, int dma_error)
{
    osSemaphoreRelease(&uart_ctx[0].tx_sem);
}

static void _uart1_dma_rx_handler(uint32_t xfer_size, int dma_error, union HAL_UART_IRQ_T status)
{
    uint32_t len = 0;
    uint32_t uartid = 1;

    len = kfifo_put(&uart_ctx[uartid].fifo, uart_ctx[uartid].buffer, xfer_size);

    if (len < xfer_size) {
        printf("%s ringbuf is full have %d need %d\r", __FUNCTION__, (int)len, (int)xfer_size);
        return;
    }

    memset(uart_ctx[uartid].buffer, 0, UART_DMA_RING_BUFFER_SIZE);
    osSemaphoreRelease(&uart_ctx[uartid].rx_sem);
    hal_uart_rx_start(uartid);
}

static void _uart1_dma_tx_handler(uint32_t xfer_size, int dma_error)
{
    osSemaphoreRelease(&uart_ctx[1].tx_sem);
}

/*uart2*/
static void _uart2_dma_rx_handler(uint32_t xfer_size, int dma_error, union HAL_UART_IRQ_T status)
{
    uint32_t len = 0;
    uint32_t uartid = 2;
    len = kfifo_put(&uart_ctx[uartid].fifo, uart_ctx[uartid].buffer, xfer_size);

    if (len < xfer_size) {
        printf("%s ringbuf is full have %d need %d\r", __FUNCTION__, (int)len, (int)xfer_size);
        return;
    }

    memset(uart_ctx[uartid].buffer, 0, UART_DMA_RING_BUFFER_SIZE);
    osSemaphoreRelease(&uart_ctx[uartid].rx_sem);
    uart_rx_flag = true;
    hal_uart_rx_start(uartid);
}

static void _uart2_dma_tx_handler(uint32_t xfer_size, int dma_error)
{
    osSemaphoreRelease(&uart_ctx[2].tx_sem);
}

/*uart3*/
static void _uart3_dma_rx_handler(uint32_t xfer_size, int dma_error, union HAL_UART_IRQ_T status)
{
    uint32_t len = 0;
    uint32_t uartid = 3;

    len = kfifo_put(&uart_ctx[uartid].fifo, uart_ctx[uartid].buffer, xfer_size);

    if (len < xfer_size) {
        printf("%s ringbuf is full have %d need %d\r", __FUNCTION__, (int)len, (int)xfer_size);
        return;
    }

    memset(uart_ctx[uartid].buffer, 0, UART_DMA_RING_BUFFER_SIZE);
    osSemaphoreRelease(&uart_ctx[uartid].rx_sem);
    uart_rx_flag = true;
    hal_uart_rx_start(uartid);
}

static void _uart3_dma_tx_handler(uint32_t xfer_size, int dma_error)
{
    osSemaphoreRelease(&uart_ctx[3].tx_sem);
}

static void hal_uart_start_rx(uint32_t uart_id)
{
    union HAL_UART_IRQ_T mask;
    mask.reg = 0;
    mask.RT = 1;
    mask.RX = 1;

    hal_uart_irq_set_mask(uart_id, mask);
    hal_uart_irq_set_handler(uart_id, uart_rx_handler);
}

static int32_t hal_uart_send(uint32_t uart_id, const void *data, uint32_t size, uint32_t timeout)
{
    int32_t ret = HDF_FAILURE;
    struct HAL_DMA_DESC_T dma_desc_tx;
    unsigned int desc_cnt = 1;

    if (NULL == data || 0 == size) {
        printf("%s %d Invalid input \r\n", __FILE__, __LINE__);
        return ret;
    }

    if (uart_id > 3) {
        printf("%s %d Invalid input \r\n", __FILE__, __LINE__);
        return ret;
    }

    hal_uart_dma_send(uart_id,data,size,&dma_desc_tx,&desc_cnt);
    osSemaphoreWait(&uart_ctx[uart_id].tx_sem, timeout);

    return HDF_SUCCESS;
}

static int32_t hal_uart_recv(uint8_t  uart_id, void *data, uint32_t expect_size,
                        uint32_t *recv_size, uint32_t timeout)
{
    int32_t  ret = HDF_FAILURE;
    uint32_t begin_time =0;
    uint32_t now_time = 0;
    uint32_t fifo_pop_len = 0;
    uint32_t recved_len = 0;
    uint32_t expect_len = expect_size;

    if (NULL == data || expect_size == 0) {
        printf("%s %d Invalid input \r\n", __FILE__, __LINE__);
        return ret;
    }

    if (uart_id > 3) {
        printf("%s %d Invalid input \r\n", __FILE__, __LINE__);
        return ret;
    }

    begin_time = TICKS_TO_MS(hal_sys_timer_get());

    do
    {
        fifo_pop_len = kfifo_get(&uart_ctx[uart_id].fifo, (uint8_t *)data + recved_len, expect_len);
        recved_len += fifo_pop_len;
        expect_len -= fifo_pop_len;

        printf("uart have %ld len data\r\n",fifo_pop_len);

        if (recved_len >= expect_size) {
            break;
        }

        /*haven't get any data from fifo */
        if (recved_len == 0) {
            break;
        }

        /*if reaches here, it means need to wait for more data come*/
        osSemaphoreWait(&uart_ctx[uart_id].rx_sem, timeout);

        /*time out break*/
        now_time = TICKS_TO_MS(hal_sys_timer_get());

        if((uint32_t)(now_time - begin_time) >= timeout){
            break;
        }
    } while (1);

    if (recv_size != NULL) {
        *recv_size = recved_len;
    }

    return HDF_SUCCESS;
}

static void hal_uart_handler_init(struct UartDevice *device)
{
    uint32_t uart_id = device->uart_id;
    printf("hal_uart_handler_init %ld\r\n", uart_id);

    if (uart_id == 0) {
        uart_ctx[uart_id].uart_dma_rx_handler = _uart_dma_rx_handler;
        uart_ctx[uart_id].uart_dma_tx_handler = _uart_dma_tx_handler;
        uart_ctx[uart_id].buffer = _hal_uart_buf;
    }

    if (uart_id == 1) {
        uart_ctx[uart_id].uart_dma_rx_handler = _uart1_dma_rx_handler;
        uart_ctx[uart_id].uart_dma_tx_handler = _uart1_dma_tx_handler;
        uart_ctx[uart_id].buffer = _hal_uart1_buf;
    }

    if (uart_id == 2) {
        uart_ctx[uart_id].uart_dma_rx_handler = _uart2_dma_rx_handler;
        uart_ctx[uart_id].uart_dma_tx_handler = _uart2_dma_tx_handler;
        uart_ctx[uart_id].buffer = _hal_uart2_buf;
    }

    if(!uart_kfifo_buffer[uart_id])
    {
        uart_kfifo_buffer[uart_id] = (char *) malloc(UART_FIFO_MAX_BUFFER);
        if(!uart_kfifo_buffer[uart_id])
        {
            printf("kfifo malloc failed!");
            return;
        }
        kfifo_init(&uart_ctx[uart_id].fifo, uart_kfifo_buffer[uart_id], UART_FIFO_MAX_BUFFER);
    }

    osSemaphoreCreate(uart_ctx[uart_id].rx_sem,0);
    osSemaphoreCreate(uart_ctx[uart_id].tx_sem,0);

    if (uart_ctx[uart_id].rxDMA)
    {
        printf("uart %ld start dma rx\r\n", uart_id);
        hal_uart_irq_set_dma_handler(uart_id, uart_ctx[uart_id].uart_dma_rx_handler, uart_ctx[uart_id].uart_dma_tx_handler);
        hal_uart_rx_start(uart_id);
    }else{
        hal_uart_start_rx(uart_id);
    }
}

static void uart_start(struct UartDevice *device)
{
    uint32_t uart_id = device->uart_id;
    struct HAL_UART_CFG_T *uart_cfg;
    uart_cfg = &device->config;
    hal_uart_open(uart_id, uart_cfg);
    printf("%s %ld\r\n",__FUNCTION__, uart_id);
    hal_uart_handler_init(device);
}

/* HdfDriverEntry method definitions */
static int32_t UartDriverBind(struct HdfDeviceObject *device);
static int32_t UartDriverInit(struct HdfDeviceObject *device);
static void UartDriverRelease(struct HdfDeviceObject *device);

/* HdfDriverEntry definitions */
struct HdfDriverEntry g_UartDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_PLATFORM_UART",
    .Bind = UartDriverBind,
    .Init = UartDriverInit,
    .Release = UartDriverRelease,
};

/* Initialize HdfDriverEntry */
HDF_INIT(g_UartDriverEntry);


/* UartHostMethod method definitions */
static int32_t UartHostDevInit(struct UartHost *host);
static int32_t UartHostDevDeinit(struct UartHost *host);
static int32_t UartHostDevWrite(struct UartHost *host, uint8_t *data, uint32_t size);
static int32_t UartHostDevSetBaud(struct UartHost *host, uint32_t baudRate);
static int32_t UartHostDevGetBaud(struct UartHost *host, uint32_t *baudRate);
static int32_t UartHostDevRead(struct UartHost *host, uint8_t *data, uint32_t size);
static int32_t UartHostDevSetAttribute(struct UartHost *host, struct UartAttribute *attribute);
static int32_t UartHostDevGetAttribute(struct UartHost *host, struct UartAttribute *attribute);
static int32_t UartHostDevSetTransMode(struct UartHost *host, enum UartTransMode mode);

/* UartHostMethod definitions */
struct UartHostMethod g_uartHostMethod = {
    .Init = UartHostDevInit,
    .Deinit = UartHostDevDeinit,
    .Read = UartHostDevRead,
    .Write = UartHostDevWrite,
    .SetBaud = UartHostDevSetBaud,
    .GetBaud = UartHostDevGetBaud,
    .SetAttribute = UartHostDevSetAttribute,
    .GetAttribute = UartHostDevGetAttribute,
    .SetTransMode = UartHostDevSetTransMode,
};

static int InitUartDevice(struct UartHost *host)
{
    printf("%s: Enter", __func__);

    if (host == NULL) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct UartDevice *uartDevice;
    struct HAL_UART_CFG_T *uart_cfg;
    uartDevice = (struct UartDevice *)host->priv;
    struct UartResource *resource = &uartDevice->resource;

    uart_cfg = &uartDevice->config;
    uartDevice->uart_id = resource->num;
    uart_cfg->parity = resource->parity;
    uart_cfg->stop = resource->stopBit;
    uart_cfg->data = resource->wlen;
    uart_cfg->flow = HAL_UART_FLOW_CONTROL_NONE;
    uart_cfg->tx_level = HAL_UART_FIFO_LEVEL_1_2;
    uart_cfg->rx_level = HAL_UART_FIFO_LEVEL_1_2;
    uart_cfg->baud = resource->baudrate;
    uart_cfg->dma_rx_stop_on_err = false;
    uart_cfg->dma_rx = resource->rxDMA;
    uart_cfg->dma_tx = resource->txDMA;

    uart_ctx[uartDevice->uart_id].txDMA = resource->txDMA;
    uart_ctx[uartDevice->uart_id].rxDMA = resource->rxDMA;

    if (!uartDevice->init_flag)
    {
        printf("uart %ld device init\r\n",uartDevice->uart_id);
        hal_set_uart_iomux(uartDevice->uart_id);
        uart_start(uartDevice);
        uartDevice->init_flag = true;
    }

    return HDF_SUCCESS;
}


static uint32_t GetUartDeviceResource(
    struct UartDevice *device, const struct DeviceResourceNode *resourceNode)
{
    struct UartResource *resource = &device->resource;
    struct DeviceResourceIface *dri = NULL;

    dri = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);

    if (dri == NULL || dri->GetUint32 == NULL) {
        printf("DeviceResourceIface is invalid");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "num", &resource->num, 0) != HDF_SUCCESS) {
        printf("uart config read num fail");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "baudrate", &resource->baudrate, 0) != HDF_SUCCESS) {
        printf("uart config read baudrate fail");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "parity", &resource->parity, 0) != HDF_SUCCESS) {
        printf("uart config read parity fail");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "stopBit", &resource->stopBit, 0) != HDF_SUCCESS) {
        printf("uart config read stopBit fail");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "data", &resource->wlen, 0) != HDF_SUCCESS) {
        printf("uart config read data fail");
        return HDF_FAILURE;
    }

    resource->txDMA = dri->GetBool(resourceNode, "txDMA");
    resource->rxDMA = dri->GetBool(resourceNode, "rxDMA");

    //copy config
    device->uart_id = resource->num;
    device->config.baud = resource->baudrate;
    device->config.parity = resource->parity;
    device->config.stop = resource->stopBit;
    device->config.data = resource->wlen;
    device->config.dma_rx = resource->rxDMA;
    device->config.dma_tx = resource->txDMA;

    return HDF_SUCCESS;
}

static int32_t AttachUartDevice(struct UartHost *uarthost, struct HdfDeviceObject *device)
{
    int32_t ret;
    struct UartDevice *uartDevice = NULL;

    if (device->property == NULL) {
        printf("%s: property is NULL", __func__);
        return HDF_FAILURE;
    }

    uartDevice = (struct UartDevice *)malloc(sizeof(struct UartDevice));
    if (uartDevice == NULL) {
        printf("%s: OsalMemCalloc uartDevice error", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

    ret = GetUartDeviceResource(uartDevice, device->property);
    if (ret != HDF_SUCCESS) {
        (void)free(uartDevice);
        return HDF_FAILURE;
    }

    uarthost->priv = uartDevice;

    return InitUartDevice(uarthost);
}

static int32_t UartDriverBind(struct HdfDeviceObject *device)
{
    static struct UartHost devService;
    if (device == NULL) {
        printf("Sample device object is null!");
        return -1;
    }

    devService.device = device;
    device->service = &(devService.service);

    return HDF_SUCCESS;
}

static void UartDriverRelease(struct HdfDeviceObject *device)
{
    struct UartHost *host = NULL;

    if (device == NULL) {
        printf("%s: device is NULL", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    printf("Enter %s:", __func__);

    host = UartHostFromDevice(device);
    if (host == NULL) {
        printf("%s: host is NULL", __func__);
        return HDF_FAILURE;
    }

    struct UartDevice *uartDevice = (struct UartDevice *)host->priv;
    uint32_t uart_id = uartDevice->uart_id;
    host->method = NULL;

    osSemaphoreDelete(uart_ctx[uart_id].rx_sem);
    osSemaphoreDelete(uart_ctx[uart_id].tx_sem);
    free(uartDevice);

    return;
}

static int32_t UartDriverInit(struct HdfDeviceObject *device)
{
    int32_t ret;
    struct UartHost *host = NULL;

    if (device == NULL) {
        printf("%s: device is NULL", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    printf("Enter %s:", __func__);

    host = UartHostFromDevice(device);
    if (host == NULL) {
        printf("%s: host is NULL", __func__);
        return HDF_FAILURE;
    }

    ret = AttachUartDevice(host, device);
    if (ret != HDF_SUCCESS) {
        printf("%s: attach error", __func__);
        return HDF_FAILURE;
    }

    host->method = &g_uartHostMethod;

    return ret;
}

/* UartHostMethod implementations */
static int32_t UartHostDevInit(struct UartHost *host)
{
    printf("%s: Enter\r\n", __func__);
    if (host == NULL) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    InitUartDevice(host);
    return HDF_SUCCESS;
}

static int32_t UartHostDevDeinit(struct UartHost *host)
{
    printf("%s: Enter", __func__);

    if (host == NULL) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct UartDevice *uartDevice;
    uartDevice = (struct UartDevice *)host->priv;
    uint32_t uart_id = uartDevice->uart_id;
    uartDevice->init_flag = false;

    hal_uart_close(uart_id);

    return HDF_SUCCESS;
}

static int32_t UartHostDevWrite(struct UartHost *host, uint8_t *data, uint32_t size)
{
    struct UartDevice *device = NULL;
    uint32_t port_id;

    if (host == NULL || data == NULL || size == 0) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    device = (struct UartDevice *)host->priv;
    if (device == NULL) {
        printf("%s: device is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    port_id = device->uart_id;
    if (uart_ctx[port_id].txDMA)
    {
        hal_uart_send(port_id, data, size, 1000);
    }else{
        for (uint32_t idx = 0; idx < size; idx++) {
            if(!uart_ctx[port_id].isblock){
                hal_uart_blocked_putc(port_id, data[idx]);
            }else
            {
                hal_uart_putc(port_id, data[idx]);
            }
        }
    }

    return HDF_SUCCESS;
}

static int32_t UartHostDevRead(struct UartHost *host, uint8_t *data, uint32_t size)
{
    uint32_t recv_size = 0;
    int32_t ret;

    printf("%s: Enter\r\n", __func__);

    if (host == NULL) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct UartDevice *uartDevice;
    uartDevice = (struct UartDevice *)host->priv;
    if (uartDevice == NULL) {
        printf("%s: device is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    uint32_t uart_id = uartDevice->uart_id;
    if (uart_ctx[uart_id].rxDMA)
    {
        ret = hal_uart_recv(uart_id, data, size, &recv_size, 1000);
        if(ret!=HDF_SUCCESS){
            printf("uart %ld recev error\r\n",uart_id);
            return HDF_FAILURE;
        }
        ret = recv_size;
    }else{
        if(!uart_ctx[uart_id].isblock){
            data[0] = hal_uart_blocked_getc(uart_id);
        }else{
            data[0] = hal_uart_getc(uart_id);
        }
        ret = 1;
    }

    return ret;
}

static int32_t UartHostDevSetBaud(struct UartHost *host, uint32_t baudRate)
{
    printf("%s: Enter", __func__);

    if (host == NULL) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct UartDevice *uartDevice;
    struct HAL_UART_CFG_T *uart_cfg;

    uartDevice = (struct UartDevice *)host->priv;
    uint32_t uart_id = uartDevice->uart_id;

    uart_cfg = &uartDevice->config;
    uart_cfg->baud = baudRate;

    hal_uart_reopen(uart_id,uart_cfg);

    return HDF_SUCCESS;
}

static int32_t UartHostDevGetBaud(struct UartHost *host, uint32_t *baudRate)
{
    printf("%s: Enter", __func__);

    if (host == NULL) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct UartDevice *uartDevice;
    struct HAL_UART_CFG_T *uart_cfg;
    uartDevice = (struct UartDevice *)host->priv;
    uint32_t uart_id = uartDevice->uart_id;
    uart_cfg = &uartDevice->config;
    baudRate = &uart_cfg->baud;

    return HDF_SUCCESS;
}

static int32_t UartHostDevSetAttribute(struct UartHost *host, struct UartAttribute *attribute)
{
    printf("%s: Enter", __func__);

    if (host == NULL) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct UartDevice *uartDevice;
    struct HAL_UART_CFG_T *uart_cfg;
    uartDevice = (struct UartDevice *)host->priv;
    uint32_t uart_id = uartDevice->uart_id;
    uart_cfg = &uartDevice->config;

    switch (attribute->dataBits)
    {
    case UART_ATTR_DATABIT_8:
        uart_cfg->data = HAL_UART_DATA_BITS_8;
        break;
    case UART_ATTR_DATABIT_7:
        uart_cfg->data = HAL_UART_DATA_BITS_7;
        break;
    case UART_ATTR_DATABIT_6:
        uart_cfg->data = HAL_UART_DATA_BITS_6;
        break;
    case UART_ATTR_DATABIT_5:
        uart_cfg->data = HAL_UART_DATA_BITS_5;
        break;
    default:
        uart_cfg->data = HAL_UART_DATA_BITS_8;
        break;
    }

    uart_cfg->parity = attribute->parity;

    switch (attribute->stopBits)
    {
    case UART_ATTR_STOPBIT_1:
    case UART_ATTR_STOPBIT_2:
        uart_cfg->stop = attribute->stopBits;
        break;
    default:
        uart_cfg->stop = UART_ATTR_STOPBIT_1;
        break;
    }

    if (attribute->rts && attribute->cts){
        uart_cfg->flow = HAL_UART_FLOW_CONTROL_RTSCTS;
    }else if (attribute->rts && !attribute->cts){
        uart_cfg->flow = HAL_UART_FLOW_CONTROL_RTS;
    }else if (!attribute->rts && attribute->cts){
        uart_cfg->flow = HAL_UART_FLOW_CONTROL_CTS;
    }else{
        uart_cfg->flow = HAL_UART_FLOW_CONTROL_NONE;
    }

    hal_uart_reopen(uart_id,uart_cfg);

    return HDF_SUCCESS;
}

static int32_t UartHostDevGetAttribute(struct UartHost *host, struct UartAttribute *attribute)
{
    printf("%s: Enter", __func__);

    if (host == NULL) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct UartDevice *uartDevice;
    struct HAL_UART_CFG_T *uart_cfg;

    uartDevice = (struct UartDevice *)host->priv;
    uart_cfg = &uartDevice->config;

    switch (uart_cfg->data)
    {
    case HAL_UART_DATA_BITS_8:
        attribute->dataBits = UART_ATTR_DATABIT_8;
        break;
    case HAL_UART_DATA_BITS_7:
        attribute->dataBits = UART_ATTR_DATABIT_7;
        break;
    case HAL_UART_DATA_BITS_6:
        attribute->dataBits = UART_ATTR_DATABIT_6;
        break;
    case HAL_UART_DATA_BITS_5:
        attribute->dataBits = UART_ATTR_DATABIT_5;
        break;
    default:
        attribute->dataBits = UART_ATTR_DATABIT_8;
        break;
    }

    attribute->parity = uart_cfg->parity;
    attribute->stopBits = uart_cfg->stop;

    switch (uart_cfg->flow)
    {
    case HAL_UART_FLOW_CONTROL_NONE:
        attribute->rts = 0;
        attribute->cts = 0;
        break;
    case HAL_UART_FLOW_CONTROL_CTS:
        attribute->rts = 0;
        attribute->cts = 1;
        break;
    case HAL_UART_FLOW_CONTROL_RTS:
        attribute->rts = 1;
        attribute->cts = 0;
        break;
    case HAL_UART_FLOW_CONTROL_RTSCTS:
        attribute->rts = 1;
        attribute->cts = 1;
        break;
    default:
        attribute->rts = 0;
        attribute->cts = 0;
        break;
    }

    return HDF_SUCCESS;
}

static int32_t UartHostDevSetTransMode(struct UartHost *host, enum UartTransMode mode)
{
    printf("%s: Enter", __func__);

    if (host == NULL) {
        printf("%s: invalid parameter", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct UartDevice *uartDevice;
    uartDevice = (struct UartDevice *)host->priv;
    uint32_t uart_id = uartDevice->uart_id;

    if ((UART_MODE_RD_BLOCK & mode) == 0)
    {
        uart_ctx[uart_id].isblock = UART_MODE_RD_BLOCK;
    }else{
        uart_ctx[uart_id].isblock = UART_MODE_RD_NONBLOCK;
    }

    if ((UART_MODE_DMA_RX_EN & mode) == 0)
    {
        uart_ctx[uart_id].rxDMA = UART_MODE_DMA_RX_EN;
    }else{
        uart_ctx[uart_id].rxDMA = UART_MODE_DMA_RX_DIS;
    }

    if ((UART_MODE_DMA_TX_EN & mode) == 0)
    {
        uart_ctx[uart_id].txDMA = UART_MODE_DMA_TX_EN;
    }else{
        uart_ctx[uart_id].txDMA = UART_MODE_DMA_TX_DIS;
    }

    return HDF_SUCCESS;
}

#define UART_TEST
#ifdef UART_TEST

void littos_uart_test(void)
{
    printf("%s\r\n",__FUNCTION__);

    int32_t ret;
    uint32_t send_len;
    int32_t recev_len;
    uint8_t buffer[100];
    struct UartHost *test_uart=NULL;

    test_uart = (struct UartHost *)malloc(sizeof(struct UartHost));
    if (test_uart == NULL) {
        printf("%s: malloc test_uart error", __func__);
        return ;
    }

    struct UartDevice *uartdev = NULL;
    uartdev = (struct UartDevice *)malloc(sizeof(struct UartDevice));
    if (uartdev == NULL) {
        printf("%s: malloc uartdev error", __func__);
        return ;
    }

    uartdev->init_flag = false;
    uartdev->resource.num = 2;
    uartdev->resource.baudrate = 1500000;
    uartdev->resource.parity = HAL_UART_PARITY_NONE;
    uartdev->resource.stopBit = HAL_UART_STOP_BITS_1;
    uartdev->resource.wlen = HAL_UART_DATA_BITS_8;
    uartdev->resource.rxDMA = false;
    uartdev->resource.txDMA = false;
    test_uart->priv = uartdev;

    ret = UartHostDevInit(test_uart);
    if (ret!=HDF_SUCCESS)
    {
        printf("UartHostDevInit error!\r\n");
        free(uartdev);
        free(test_uart);
        return;
    }

    UartHostDevWrite(test_uart,(unsigned char *)"uart test write\r\n",20);
    while (1)
    {
        if(uart_rx_flag){
            memset(buffer,0,100);
            while ((recev_len = UartHostDevRead(test_uart,buffer,100)) > 0)
            {
                buffer[recev_len+1]= '\0';
                UartHostDevWrite(test_uart,buffer,recev_len+1);
            }
            recev_len = 0;
        }
        uart_rx_flag = false;
        osDelay(200);
    }
    free(uartdev);
    free(test_uart);
}
#endif
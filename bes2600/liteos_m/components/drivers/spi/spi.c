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
#include <stdlib.h>
#include <string.h>
#include "spi.h"
#include "spi_core.h"
#include "device_resource_if.h"
#include "hal_cache.h"
#include "cmsis_os.h"
#include "hal_trace.h"
#include "securec.h"

#define SPI_DMA_MAX 4095

osSemaphoreDef(spi0_dma_semaphore);
osSemaphoreDef(spi1_dma_semaphore);

osMutexDef(spi0_mutex);
osMutexDef(spi1_mutex);

static void spi0_dma_irq(int error);
static void spi1_dma_irq(int error);

static struct SPI_CTX_OBJ_T spi_ctx[2] = {
    {
        .spi_pin_CS0 = 0,
        .spi_fun_DI0 = HAL_IOMUX_FUNC_SPI_DI0,
        .spi_fun_CLK = HAL_IOMUX_FUNC_SPI_CLK,
        .spi_fun_CS0 = HAL_IOMUX_FUNC_SPI_CS0,
        .spi_fun_DIO = HAL_IOMUX_FUNC_SPI_DIO,
        .spi_dma_semaphore = NULL,
        .spi_mutex_id = 0,
        .spi_open = hal_spi_open,
        .spi_dma_send = hal_spi_dma_send,
        .spi_dma_recv = hal_spi_dma_recv,
        .spi_send = hal_spi_send,
        .spi_recv = hal_spi_recv,
        .spi_dma_irq = spi0_dma_irq,
        .spi_close = hal_spi_close
    },
    {
        .spi_pin_CS0 = 0,
        .spi_fun_DI0 = HAL_IOMUX_FUNC_SPILCD_DI0,
        .spi_fun_CLK = HAL_IOMUX_FUNC_SPILCD_CLK,
        .spi_fun_CS0 = HAL_IOMUX_FUNC_SPILCD_CS0,
        .spi_fun_DIO = HAL_IOMUX_FUNC_SPILCD_DIO,
        .spi_dma_semaphore = NULL,
        .spi_mutex_id = 0,
        .spi_open = hal_spilcd_open,
        .spi_dma_send = hal_spilcd_dma_send,
        .spi_dma_recv = hal_spilcd_dma_recv,
        .spi_send = hal_spilcd_send,
        .spi_recv = hal_spilcd_recv,
        .spi_dma_irq = spi1_dma_irq,
        .spi_close = hal_spilcd_close
    }
};

static struct HAL_IOMUX_PIN_FUNCTION_MAP pinmux_spi[] = {
    {0, 0, HAL_IOMUX_PIN_VOLTAGE_VIO, HAL_IOMUX_PIN_NOPULL },
    {0, 0, HAL_IOMUX_PIN_VOLTAGE_VIO, HAL_IOMUX_PIN_NOPULL },
    {0, 0, HAL_IOMUX_PIN_VOLTAGE_VIO, HAL_IOMUX_PIN_NOPULL },
    {0, 0, HAL_IOMUX_PIN_VOLTAGE_VIO, HAL_IOMUX_PIN_NOPULL },
};

void spi0_dma_irq(int error)
{
    if (osOK != osSemaphoreRelease(spi_ctx[0].spi_dma_semaphore))
    {
        printf("spi1dmairq osSemaphoreRelease failed!\r\n");
    }
}

void spi1_dma_irq(int error)
{
    if (osOK != osSemaphoreRelease(spi_ctx[1].spi_dma_semaphore))
    {
        printf("spi1dmairq osSemaphoreRelease failed!\r\n");
        return;
    }
}

static void spi_iomux_init(struct SpiDevice *spiDevice)
{
    printf("%s: Enter\r\n", __func__);

    if (spiDevice == NULL)
    {
        printf("%s: invalid parameter\r\n", __func__);
        return;
    }

    struct SpiResource *resource = &spiDevice->resource;
    struct HAL_SPI_CFG_T *spi_cfg;

    spi_cfg = &spiDevice->spidevcfg;
    spi_cfg->rate = resource->speed;

    uint32_t spi_port = spiDevice->spi_id;

    pinmux_spi[0].pin = resource->spi_miso_pin;
    pinmux_spi[1].pin = resource->spi_clk_pin;
    pinmux_spi[2].pin = resource->spi_cs_pin;
    pinmux_spi[3].pin = resource->spi_mosi_pin;

    spi_ctx[spi_port].spi_pin_CS0 = resource->spi_cs_pin;

    pinmux_spi[0].function = spi_ctx[spi_port].spi_fun_DI0;
    pinmux_spi[1].function = spi_ctx[spi_port].spi_fun_CLK;

    if (resource->spi_cs_soft)
    {
        spi_ctx[spi_port].spi_fun_CS0 = HAL_IOMUX_FUNC_AS_GPIO;
    }

    pinmux_spi[2].function = spi_ctx[spi_port].spi_fun_CS0;
    pinmux_spi[3].function = spi_ctx[spi_port].spi_fun_DIO;

    if (spi_cfg->rate > 26000000)
    {
        hal_iomux_set_io_drv(pinmux_spi[1].pin, 3);
        hal_iomux_set_io_drv(pinmux_spi[3].pin, 3);
    }

    hal_iomux_init(pinmux_spi, ARRAY_SIZE(pinmux_spi));
}

/**
 * Spi send
 *
 * @param[in]  spi_id   the spi bus id
 * @param[in]  data     spi send data
 * @param[in]  size     spi send data size
 * @param[in]  timeout  timeout in milisecond, set this value to HAL_WAIT_FOREVER
 *                      if you want to wait forever
 *
 * @return  0 : on success, EIO : if the SPI device could not be initialised
 */

#ifdef HalSpiSend
#undef HalSpiSend
#endif
int32_t HalSpiSend(struct SpiDevice *spidevice, const uint8_t *data, uint16_t size, uint32_t timeout)
{
    int32_t ret = 0;
    uint32_t len = size;

    osStatus status = osErrorOS;

    if (NULL == spidevice || NULL == data || 0 == size)
    {
        printf("spi input para err\r\n");
        return -3;
    }

    uint32_t spi_id = spidevice->spi_id;
    struct SpiResource *resource = &spidevice->resource;

    status = osMutexWait(spi_ctx[spi_id].spi_mutex_id, osWaitForever);
    if (osOK != status)
    {
        printf("%s spi_mutex wait error = 0x%X!\r\n", __func__, status);
        return -2;
    }

    hal_cache_sync_all(HAL_CACHE_ID_D_CACHE);

    if (resource->transmode == SPI_TRANSFER_DMA)
    {
        ret = spi_ctx[spi_id].spi_dma_send(data, len, spi_ctx[spi_id].spi_dma_irq);
        if (osSemaphoreWait(spi_ctx[spi_id].spi_dma_semaphore, timeout) <= 0)
        {
            printf("spi dma send timeout\r\n");
            goto OUT;
        }
    }
    else
    {
        ret = spi_ctx[spi_id].spi_send(data, len);
    }

    if (ret)
    {
        printf("spi tail send fail %d, size %u\r\n", ret, len);
        goto OUT;
    }
OUT:
    osMutexRelease(spi_ctx[spi_id].spi_mutex_id);
    return ret;
}

#ifdef HalSpiRecv
#undef HalSpiRecv
#endif

/**
 * spi_recv
 *
 * @param[in]   spi_id   the spi bus id
 * @param[out]  data     spi recv data
 * @param[in]   size     spi recv data size
 * @param[in]   timeout  timeout in milisecond, set this value to HAL_WAIT_FOREVER
 *                       if you want to wait forever
 *
 * @return  0 : on success, EIO : if the SPI device could not be initialised
 */
int32_t HalSpiRecv(struct SpiDevice *spidevice, uint8_t *data, uint16_t size, uint32_t timeout)
{
    int32_t ret = 0;
    uint32_t len = size;
    uint32_t remainder = 0;
    osStatus status = osOK;
    uint8_t *cmd;

    if (NULL == spidevice || NULL == data || 0 == size)
    {
        printf("spi input para err\r\n");
        return -3;
    }

    uint32_t spi_id = spidevice->spi_id;
    struct SpiResource *resource = &spidevice->resource;

    cmd = (uint8_t *)malloc(len);
    if (cmd == NULL)
    {
        printf("%s malloc size %u error\r\n", __FUNCTION__, len);
        return osErrorNoMemory;
    }

    memset_s(cmd, len, 0, len);

    status = osMutexWait(spi_ctx[spi_id].spi_mutex_id, osWaitForever);
    if (osOK != status)
    {
        printf("%s spi_mutex wait error = 0x%X!\r\n", __func__, status);
        free(cmd);
        return osErrorTimeout;
    }

    hal_cache_sync_all(HAL_CACHE_ID_D_CACHE); //PSRAM must sync cache to memory when used dma

    remainder = len <= SPI_DMA_MAX ? len : SPI_DMA_MAX;

    if (resource->transmode == SPI_TRANSFER_DMA) {
        ret = spi_ctx[spi_id].spi_dma_recv(cmd, data, remainder, spi_ctx[spi_id].spi_dma_irq);
        if (osSemaphoreWait(spi_ctx[spi_id].spi_dma_semaphore, osWaitForever) <= 0) {
            printf("SPI Read timeout!\r\n");
            goto OUT;
        }
    } else {
        ret = spi_ctx[spi_id].spi_recv(cmd, data, remainder);
    }

    len -= remainder;
    data += remainder;

    if (ret) {
        printf("spi tail fail %d, size %u\r\n", ret, len);
        goto OUT;
    }
OUT:
    osMutexRelease(spi_ctx[spi_id].spi_mutex_id);
    free(cmd);
    return ret;
}

#ifdef HalSpiSendRecv
#undef HalSpiSendRecv
#endif
int32_t HalSpiSendRecv(struct SpiDevice *spidevice, uint8_t *tx_data, uint16_t tx_size, uint8_t *rx_data,
                  uint16_t rx_size, uint32_t timeout)
{
    int32_t ret;
    osStatus status;

    if (NULL == spidevice || NULL == tx_data || 0 == tx_size || NULL == rx_data || 0 == rx_size) {
        printf("spi input para err\r\n");
        return -3;
    }
    uint32_t spi_id = spidevice->spi_id;
    struct SpiResource *resource = &spidevice->resource;
    status = osMutexWait(spi_ctx[spi_id].spi_mutex_id, osWaitForever);
    if (osOK != status) {
        printf("%s spi_mutex wait error = 0x%X!\r\n", __func__, status);
        return -2;
    }
    hal_cache_sync_all(HAL_CACHE_ID_D_CACHE); //PSRAM must sync cache to memory when used dma
    if (resource->transmode == SPI_TRANSFER_DMA) {
        ret = spi_ctx[spi_id].spi_dma_recv(tx_data, rx_data, rx_size, spi_ctx[spi_id].spi_dma_irq);
        if (osSemaphoreWait(spi_ctx[spi_id].spi_dma_semaphore, 1000) <= 0) {
            printf("%s:SPI Read timeout!\r\n", __func__);
            goto OUT;
        }
    } else {
        ret = spi_ctx[spi_id].spi_recv(tx_data, rx_data, rx_size);
    }
    if (ret) {
        printf("spi dma tail fail %d\r\n", ret);
        goto OUT;
    }
OUT:
    osMutexRelease(spi_ctx[spi_id].spi_mutex_id);
    return ret;
}

static int32_t InitSpiDevice(struct SpiDevice *spiDevice)
{
    if (spiDevice == NULL)
    {
        printf("%s: invalid parameter\r\n", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct SpiResource *resource = &spiDevice->resource;
    struct HAL_SPI_CFG_T *spi_cfg;
    spi_cfg = &spiDevice->spidevcfg;
    uint32_t spi_port = spiDevice->spi_id;

    spi_iomux_init(spiDevice);

    switch (resource->mode)
    {
    case SPI_WORK_MODE_0:
        spi_cfg->clk_delay_half = false;
        spi_cfg->clk_polarity = false;
        break;
    case SPI_WORK_MODE_1:
        spi_cfg->clk_delay_half = true;
        spi_cfg->clk_polarity = false;
        break;
    case SPI_WORK_MODE_2:
        spi_cfg->clk_delay_half = false;
        spi_cfg->clk_polarity = true;
        break;
    case SPI_WORK_MODE_3:
        spi_cfg->clk_delay_half = true;
        spi_cfg->clk_polarity = true;
        break;
    default:
        spi_cfg->clk_delay_half = true;
        spi_cfg->clk_polarity = true;
    }

    spi_cfg->slave = 0;

    if (resource->transmode == SPI_TRANSFER_DMA)
    {
        spi_cfg->dma_rx = true;
        spi_cfg->dma_tx = true;
    }
    else
    {
        spi_cfg->dma_rx = false;
        spi_cfg->dma_tx = false;
    }

    spi_cfg->cs = 0;
    spi_cfg->rx_bits = resource->data_size;
    spi_cfg->tx_bits = resource->data_size;
    spi_cfg->rx_frame_bits = 0;
    /* spi open move to hdf open api */
    /*if cs use as gpio ,pull up at first*/
    if (spi_ctx[spi_port].spi_fun_CS0 == HAL_IOMUX_FUNC_AS_GPIO)
    {
        hal_gpio_pin_set_dir(spi_ctx[spi_port].spi_pin_CS0, HAL_GPIO_DIR_OUT, 1);
    }


    if (!spi_ctx[spi_port].spi_dma_semaphore)
    {
        if (spi_port == 0)
        {
            spi_ctx[spi_port].spi_dma_semaphore = osSemaphoreCreate(osSemaphore(spi0_dma_semaphore), 0);
        }
        else
        {
            spi_ctx[spi_port].spi_dma_semaphore = osSemaphoreCreate(osSemaphore(spi1_dma_semaphore), 0);
        }
    }

    if (!spi_ctx[spi_port].spi_dma_semaphore)
    {
        printf("spi0_dma_semaphore create failed!\r\n");
        return HDF_FAILURE;
    }

    if (!spi_ctx[spi_port].spi_mutex_id)
    {
        if (spi_port == 0)
        {
            spi_ctx[spi_port].spi_mutex_id = osMutexCreate((osMutex(spi0_mutex)));
        }
        else
        {
            spi_ctx[spi_port].spi_mutex_id = osMutexCreate((osMutex(spi1_mutex)));
        }
    }

    if (!spi_ctx[spi_port].spi_mutex_id)
    {
        printf("spi0_mutex create failed!\r\n");
        return -1;
    }

    return HDF_SUCCESS;
}

/*get spi config from hcs file*/
static int32_t GetSpiDeviceResource(struct SpiDevice *spiDevice, const struct DeviceResourceNode *resourceNode)
{
    struct SpiResource *resource = &spiDevice->resource;
    uint32_t rel_pin;
    struct DeviceResourceIface *dri = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);   // open HDF
    if (dri == NULL || dri->GetUint32 == NULL) {
        printf("DeviceResourceIface is invalid\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "busNum", &resource->num, 0) != HDF_SUCCESS) {
        printf("spi config read num fail\r\n");
        return HDF_FAILURE;
    }

    spiDevice->spi_id = resource->num;
    if (dri->GetUint32(resourceNode, "speed", &resource->speed, 0) != HDF_SUCCESS) {
        printf("spi config read base fail\r\n");
        return HDF_FAILURE;
    }

    uint32_t temp;
    if (dri->GetUint32(resourceNode, "transmode", &temp, 0) != HDF_SUCCESS) {
        printf("spi config read transmode fail\r\n");
        return HDF_FAILURE;
    }
    resource->transmode = (enum SPI_TRANSFER_MODE)temp;

    if (dri->GetUint32(resourceNode, "spi_cs_soft", &resource->spi_cs_soft, 0) != HDF_SUCCESS) {
        printf("spi config read spi_cs_soft fail\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "mode", &temp, 0) != HDF_SUCCESS) {
        printf("spi config read mode fail\r\n");
        return HDF_FAILURE;
    }
    resource->mode = (enum SPI_WORK_MODE)temp;

    if (dri->GetUint32(resourceNode, "data_size", &resource->data_size, 0) != HDF_SUCCESS) {
        printf("spi config read data_size fail\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "Csnum", &resource->Csnum, 0) != HDF_SUCCESS) {
        printf("spi config read Csnum fail\r\n");
        return HDF_FAILURE;
    }

    rel_pin = 0;
    if (dri->GetUint32(resourceNode, "spi_clk_pin", &resource->spi_clk_pin, 0) != HDF_SUCCESS) {
        printf("spi config read spi_clk_pin fail\r\n");
        return HDF_FAILURE;
    }

    rel_pin = (resource->spi_clk_pin / 10) * 8 + (resource->spi_clk_pin % 10);
    resource->spi_clk_pin = rel_pin;

    if (dri->GetUint32(resourceNode, "spi_mosi_pin", &resource->spi_mosi_pin, 0) != HDF_SUCCESS) {
        printf("spi config read spi_mosi_pin fail\r\n");
        return HDF_FAILURE;
    }

    rel_pin = (resource->spi_mosi_pin / 10) * 8 + (resource->spi_mosi_pin % 10);
    resource->spi_mosi_pin = rel_pin;
    if (dri->GetUint32(resourceNode, "spi_miso_pin", &resource->spi_miso_pin, 0) != HDF_SUCCESS) {
        printf("spi config read spi_miso_pin fail\r\n");
        return HDF_FAILURE;
    }

    rel_pin = ((resource->spi_miso_pin / 10) * 8) + (resource->spi_miso_pin % 10);
    resource->spi_miso_pin = rel_pin;

    if (dri->GetUint32(resourceNode, "spi_cs_pin", &resource->spi_cs_pin, 0) != HDF_SUCCESS) {
        printf("spi config read spi_cs_pin fail\r\n");
        return HDF_FAILURE;
    }

    rel_pin = ((resource->spi_cs_pin / 10) * 8) + (resource->spi_cs_pin % 10);
    resource->spi_cs_pin = rel_pin;

    return HDF_SUCCESS;
}

int32_t AttachSpiDevice(struct SpiCntlr *spicntlr, struct HdfDeviceObject *device)
{
    int32_t ret;
    struct SpiDevice *spiDevice = NULL;

    if (device->property == NULL) {
        printf("%s: property is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    spiDevice = (struct SpiDevice *)malloc(sizeof(struct SpiDevice));
    if (spiDevice == NULL) {
        printf("%s: malloc spiDevice error\r\n", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

    ret = GetSpiDeviceResource(spiDevice, device->property);
    if (ret != HDF_SUCCESS)
    {
        (void)free(spiDevice);
        return HDF_FAILURE;
    }

    spicntlr->priv = spiDevice;
    spicntlr->busNum = spiDevice->spi_id;
    return InitSpiDevice(spiDevice);
}
/*SPI Method*/
static int32_t SpiDevGetCfg(struct SpiCntlr *spicntlr, struct SpiCfg *spicfg);
static int32_t SpiDevSetCfg(struct SpiCntlr *spicntlr, struct SpiCfg *spicfg);
static int32_t SpiDevTransfer(struct SpiCntlr *spicntlr, struct SpiMsg *spimsg, uint32_t count);
static int32_t SpiDevOpen(struct SpiCntlr *spicntlr);
static int32_t SpiDevClose(struct SpiCntlr *spicntlr);

struct SpiCntlrMethod g_SpiCntlrMethod = {
    .GetCfg = SpiDevGetCfg,
    .SetCfg = SpiDevSetCfg,
    .Transfer = SpiDevTransfer,
    .Open = SpiDevOpen,
    .Close = SpiDevClose,
};

/* HdfDriverEntry method definitions */
static int32_t SpiDriverBind(struct HdfDeviceObject *device);
static int32_t SpiDriverInit(struct HdfDeviceObject *device);
static void SpiDriverRelease(struct HdfDeviceObject *device);

/* HdfDriverEntry definitions */
struct HdfDriverEntry g_SpiDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_PLATFORM_SPI",
    .Bind = SpiDriverBind,
    .Init = SpiDriverInit,
    .Release = SpiDriverRelease,
};

HDF_INIT(g_SpiDriverEntry);

static int32_t SpiDriverBind(struct HdfDeviceObject *device)
{
    struct SpiCntlr *spicntlr = NULL;
    if (device == NULL) {
        TRACE(0, "Sample device object is null!\r\n");
        return -1;
    }
    TRACE(0, "Enter %s:\r\n", __func__);
    spicntlr = (struct SpiCntlr *)malloc(sizeof(struct SpiCntlr));
    if (spicntlr == NULL) {
        TRACE(0, "%s: host is NULL\r\n", __func__);
        return HDF_FAILURE;
    }
    device->service = &spicntlr->service;
    spicntlr->device = device;
    spicntlr->priv = NULL;
    return HDF_SUCCESS;
}

static int32_t SpiDriverInit(struct HdfDeviceObject *device)
{
    int32_t ret;
    struct SpiCntlr *spicntlr = NULL;

    if (device == NULL) {
        printf("%s: device is NULL\r\n", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    printf("Enter %s:", __func__);
    spicntlr = SpiCntlrFromDevice(device);
    if (spicntlr == NULL) {
        printf("%s: spicntlr is NULL", __func__);
        return HDF_FAILURE;
    }

    ret = AttachSpiDevice(spicntlr, device); //SpiCntlr add SpiDevice to priv
    if (ret != HDF_SUCCESS)
    {
        printf("%s: attach error\r\n", __func__);
        return HDF_FAILURE;
    }

    spicntlr->method = &g_SpiCntlrMethod; //register callback

    return ret;
}

static void SpiDriverRelease(struct HdfDeviceObject *device)
{
    struct SpiCntlr *spicntlr = NULL;
    struct SpiDevice *spiDevice = NULL;

    printf("Enter %s\r\n", __func__);

    if (device == NULL) {
        printf("%s: device is NULL\r\n", __func__);
        return;
    }

    spicntlr = SpiCntlrFromDevice(device);
    if (spicntlr == NULL) {
        printf("%s: spicntlr is NULL\r\n", __func__);
        return;
    }

    spiDevice = (struct SpiDevice *)spicntlr->priv;
    if (spiDevice != NULL) {
        free(spiDevice);
    }
    return;
}

static int32_t SpiDevOpen(struct SpiCntlr *spicntlr)
{
    int ret;
    TRACE(0, "Enter %s\r\n", __func__);
    struct SpiDevice *spiDevice = NULL;
    if (spicntlr == NULL) {
        printf("%s: spicntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }
    spiDevice = (struct SpiDevice *)spicntlr->priv;
    uint32_t spi_port = spiDevice->spi_id;
    struct HAL_SPI_CFG_T *spi_cfg = &spiDevice->spidevcfg;
    if (spi_cfg == NULL) {
        printf("spi %u config is NULL\r\n", spi_port);
        return HDF_FAILURE;
    }
    ret = spi_ctx[spi_port].spi_open(spi_cfg);
    if (ret != 0) {
        printf("spi %u open error %d\r\n", spi_port, ret);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t SpiDevClose(struct SpiCntlr *spicntlr)
{
    int ret;
    struct SpiDevice *spiDevice = NULL;
    if (spicntlr == NULL) {
        printf("%s: spicntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }
    spiDevice = (struct SpiDevice *)spicntlr->priv;
    uint32_t spi_port = spiDevice->spi_id;
    struct HAL_SPI_CFG_T *spi_cfg = &spiDevice->spidevcfg;
    if (spi_cfg == NULL) {
        printf("spi %u config is NULL\r\n", spi_port);
        return HDF_FAILURE;
    }
    ret = spi_ctx[spi_port].spi_close(spi_cfg->cs);
    if (ret != 0) {
        printf("spi %u open error %d\r\n", spi_port, ret);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t SpiDevGetCfg(struct SpiCntlr *spicntlr, struct SpiCfg *spicfg)
{
    struct SpiDevice *spiDevice = NULL;
    if (spicntlr == NULL || spicfg == NULL) {
        printf("%s: spicntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }
    spiDevice = (struct SpiDevice *)spicntlr->priv;
    if (spiDevice == NULL) {
        return -1;
    }
    spicfg->maxSpeedHz = spiDevice->resource.speed;
    spicfg->mode = spiDevice->resource.mode;
    spicfg->transferMode = spiDevice->resource.transmode;
    spicfg->bitsPerWord = spiDevice->resource.data_size;

    return HDF_SUCCESS;
}
static int32_t SpiDevSetCfg(struct SpiCntlr *spicntlr, struct SpiCfg *spicfg)
{
    struct SpiDevice *spiDevice = NULL;
    if (spicntlr == NULL || spicfg == NULL) {
        printf("%s: spicntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }
    spiDevice = (struct SpiDevice *)spicntlr->priv;
    if (spiDevice == NULL) {
        return -1;
    }
    struct HAL_SPI_CFG_T *spi_cfg = &spiDevice->spidevcfg;
    if (spi_cfg == NULL) {
        return -1;
    }
    spiDevice->resource.speed = spicfg->maxSpeedHz;
    spiDevice->resource.mode = spicfg->mode;
    spiDevice->resource.transmode = spicfg->transferMode;
    spiDevice->resource.data_size  = spicfg->bitsPerWord;
    spi_cfg->rate = spicfg->maxSpeedHz;
    return InitSpiDevice(spiDevice);
}

static int32_t SpiDevTransfer(struct SpiCntlr *spicntlr, struct SpiMsg *spimsg, uint32_t count)
{
    struct SpiDevice *spiDevice = NULL;
    if (spicntlr == NULL) {
        printf("%s: spicntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }
    struct SpiMsg *msg;
    spiDevice = (struct SpiDevice *)spicntlr->priv;
    uint32_t spi_id = spiDevice->spi_id;
    printf("%s: %u Enter\r\n", __func__, spi_id);
    for (size_t i = 0; i < count; i++) {
        msg = &spimsg[i];
        /*pull down cs at first*/
        if(spiDevice->resource.spi_cs_soft) {
            hal_gpio_pin_set_dir(spi_ctx[spi_id].spi_pin_CS0, HAL_GPIO_DIR_OUT, 0);
        }

        if ((msg->wbuf != NULL) && (msg->rbuf == NULL)) {
            HalSpiSend(spiDevice, msg->wbuf, msg->len, 1000);
        }
        if ((msg->rbuf != NULL) && (msg->wbuf == NULL)) {
            HalSpiRecv(spiDevice, msg->rbuf, msg->len, 1000);
        }
        if ((msg->wbuf != NULL) && (msg->rbuf != NULL)) {
            HalSpiSendRecv(spiDevice, msg->wbuf, msg->len, msg->rbuf, msg->len, 1000);
        }

        /*pull pull up cs at the end*/
        if (msg->csChange) {
            hal_gpio_pin_set_dir(spi_ctx[spi_id].spi_pin_CS0, HAL_GPIO_DIR_OUT, 1);
        }
        osDelay(msg->delayUs);
    }
    return HDF_SUCCESS;
}
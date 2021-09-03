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
#include "i2c.h"
#include "i2c_core.h"
#include "i2c_if.h"
#include "hdf_device_desc.h"
#include "hdf_platform.h"
#include "hal_iomux_best2003.h"
#include "hal_trace.h"

osMutexDef(i2c0_mutex);
osMutexDef(i2c1_mutex);

#define DEC_NUM 10
#define GROUP_PIN_NUM 8
#define I2C_INVALID_ADDR 0xFFFF

/* HdfDriverEntry method definitions */
static int32_t i2cDriverBind(struct HdfDeviceObject *device);
static int32_t i2cDriverInit(struct HdfDeviceObject *device);
static void i2cDriverRelease(struct HdfDeviceObject *device);

/* HdfDriverEntry definitions */
struct HdfDriverEntry g_i2cDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_PLATFORM_I2C",
    .Bind = i2cDriverBind,
    .Init = i2cDriverInit,
    .Release = i2cDriverRelease,
};

// Initialize HdfDriverEntry
HDF_INIT(g_i2cDriverEntry);

/* I2cHostMethod method definitions */
static int32_t i2cHostTransfer(struct I2cCntlr *cntlr, struct I2cMsg *msgs, int16_t count);

struct I2cMethod g_i2cHostMethod = {
    .transfer = i2cHostTransfer,
};

int32_t InitI2cDevice(struct I2cDevice *device)
{
    int32_t ret = -1;

    if (device == NULL) {
        printf("device is NULL\r\n");
        return HDF_FAILURE;
    }

    struct I2cResource *resource = &device->resource;
    if (resource == NULL) {
        printf("%s %d: invalid parameter\r\n", __func__, __LINE__);
        return HDF_FAILURE;
    }

    struct HAL_I2C_CONFIG_T *i2cConfig = &device->i2c_cfg;
    if (i2cConfig == NULL) {
        printf("%s %d: invalid parameter\r\n", __func__, __LINE__);
        return HDF_FAILURE;
    }

    struct HAL_IOMUX_PIN_FUNCTION_MAP pinmuxI2c[] = {
        {0, 0, HAL_IOMUX_PIN_VOLTAGE_VIO, HAL_IOMUX_PIN_PULLUP_ENABLE},
        {0, 0, HAL_IOMUX_PIN_VOLTAGE_VIO, HAL_IOMUX_PIN_PULLUP_ENABLE},
    };

    device->port = resource->port;
    uint32_t i2cPort = device->port;

    if (i2cPort > HAL_I2C_ID_NUM) {
        printf("i2c port %d not support\r\n", i2cPort);
        return HDF_FAILURE;
    }

    if (i2cPort == 0) {
        device->mutex = osMutexCreate(osMutex(i2c0_mutex));
        pinmuxI2c[0].function = HAL_IOMUX_FUNC_I2C_M0_SCL;
        pinmuxI2c[1].function = HAL_IOMUX_FUNC_I2C_M0_SDA;
    } else {
        device->mutex = osMutexCreate(osMutex(i2c1_mutex));
        pinmuxI2c[0].function = HAL_IOMUX_FUNC_I2C_M1_SCL;
        pinmuxI2c[1].function = HAL_IOMUX_FUNC_I2C_M1_SDA;
    }

    if (device->mutex == NULL) {
        printf("%s %d osMutexCreate fail\r\n", __func__, __LINE__);
        return HDF_FAILURE;
    }

    if (osOK != osMutexWait(device->mutex, osWaitForever)) {
        printf("%s %d osMutexWait fail\r\n", __func__, __LINE__);
        return HDF_FAILURE;
    }

    pinmuxI2c[0].pin = resource->scl_pin;
    pinmuxI2c[1].pin = resource->sda_pin;
    hal_iomux_init(pinmuxI2c, ARRAY_SIZE(pinmuxI2c));

    ret = hal_i2c_open(i2cPort, i2cConfig);
    if (ret) {
        printf("open %ld i2c fail,ret %ld\r\n", i2cPort, ret);
    } else {
        printf("open %ld i2c succ.\r\n", i2cPort);
    }

    osMutexRelease(device->mutex);

    return ret;
}

static int32_t HostRestI2cDevice(struct I2cDevice *device)
{
    int32_t ret = -1;
    if (device == NULL) {
        printf("%s %d device is null\r\n", __func__, __LINE__);
        return -1;
    }
    struct I2cResource *resource = &device->resource;
    if (resource == NULL) {
        printf("%s %d: invalid parameter\r\n", __func__, __LINE__);
        return HDF_FAILURE;
    }
    struct HAL_I2C_CONFIG_T *i2cConfig = &device->i2c_cfg;
    if (i2cConfig == NULL) {
        printf("%s %d: invalid parameter\r\n", __func__, __LINE__);
        return HDF_FAILURE;
    }
    device->port = resource->port;
    uint32_t i2cPort = device->port;
    if (i2cPort > HAL_I2C_ID_NUM) {
        printf("i2c port %d not support\r\n", i2cPort);
        return HDF_FAILURE;
    }

    if (device->mutex == NULL) {
        printf("%s %d osMutexCreate fail\r\n", __func__, __LINE__);
        return -1;
    }
    hal_i2c_close(i2cPort);
    ret = hal_i2c_open(i2cPort, i2cConfig);
    if (ret) {
        printf("open %ld i2c fail, ret %ld\r\n", i2cPort, ret);
    } else {
        printf("open %ld i2c succ.\r\n", i2cPort);
    }
    return ret;
}

static uint32_t GetI2cDeviceResource(struct I2cDevice *device,
                                     const struct DeviceResourceNode *resourceNode)
{
    uint32_t temp_pin = 0;
    if (device == NULL || resourceNode == NULL) {
        printf("device or resourceNode is NULL\r\n");
        return HDF_FAILURE;
    }
    struct I2cResource *resource = &device->resource;
    if (resource == NULL) {
        printf("%s %d: invalid parameter\r\n", __func__, __LINE__);
        return HDF_FAILURE;
    }
    struct HAL_I2C_CONFIG_T *i2cConfig = &device->i2c_cfg;
    if (i2cConfig == NULL) {
        printf("%s %d: invalid parameter\r\n", __func__, __LINE__);
        return HDF_FAILURE;
    }
    struct DeviceResourceIface *dri = NULL;
    dri = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (dri == NULL || dri->GetUint32 == NULL) {
        printf("DeviceResourceIface is invalid\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "port", &resource->port, 0) != HDF_SUCCESS) {
        printf("i2c config port fail\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "scl_pin", &temp_pin, 0) != HDF_SUCCESS) {
        printf("i2c config scl_pin fail\r\n");
        return HDF_FAILURE;
    }
    resource->scl_pin = ((temp_pin / DEC_NUM) * GROUP_PIN_NUM) + (temp_pin % DEC_NUM);

    if (dri->GetUint32(resourceNode, "sda_pin", &temp_pin, 0) != HDF_SUCCESS) {
        printf("i2c config sda_pin fail\r\n");
        return HDF_FAILURE;
    }
    resource->sda_pin = ((temp_pin / DEC_NUM) * GROUP_PIN_NUM) + (temp_pin % DEC_NUM);

    if (dri->GetUint32(resourceNode, "speed", &resource->speed, 0) != HDF_SUCCESS) {
        printf("i2c config speed fail\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "mode", &resource->mode, 0) != HDF_SUCCESS) {
        printf("i2c config mode fail\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "use_dma", &resource->use_dma, 0) != HDF_SUCCESS) {
        printf("i2c config use_dma fail\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "use_sync", &resource->use_sync, 0) != HDF_SUCCESS) {
        printf("i2c config use_sync fail\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "as_master", &resource->as_master, 0) != HDF_SUCCESS) {
        printf("i2c config as_master fail\r\n");
        return HDF_FAILURE;
    }
    device->port = resource->port;
    i2cConfig->mode = resource->mode;
    i2cConfig->use_sync = resource->use_sync;
    i2cConfig->use_dma = resource->use_dma;
    i2cConfig->as_master = resource->as_master;
    i2cConfig->speed = resource->speed;
    i2cConfig->addr_as_slave = 0;
    i2cConfig->rising_time_ns = 0;
    return HDF_SUCCESS;
}

static int32_t AttachI2cDevice(struct I2cCntlr *host, struct HdfDeviceObject *device)
{
    int32_t ret;
    struct I2cDevice *i2cDevice = NULL;
    if (device == NULL || host == NULL) {
        printf("%s: device or host is NULL\r\n", __func__);
        return HDF_FAILURE;
    }
    i2cDevice = (struct I2cDevice *)malloc(sizeof(struct I2cDevice));
    if (i2cDevice == NULL) {
        printf("%s: malloc i2cDevice error\r\n", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }
    (void)memset_s(i2cDevice, sizeof(struct I2cDevice), 0, sizeof(struct I2cDevice));
    ret = GetI2cDeviceResource(i2cDevice, device->property);
    if (ret != HDF_SUCCESS) {
        free(i2cDevice);
        return HDF_FAILURE;
    }
    host->priv = i2cDevice;
    host->busId = i2cDevice->port;

    return InitI2cDevice(i2cDevice);
}

static int32_t i2cDriverInit(struct HdfDeviceObject *device)
{
    int32_t ret;
    struct I2cCntlr *host = NULL;
    if (device == NULL) {
        printf("%s: device is NULL\r\n", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    host = (struct I2cCntlr *)malloc(sizeof(struct I2cCntlr));
    if (host == NULL) {
        printf("%s: host is NULL\r\n", __func__);
        return HDF_FAILURE;
    }
    (void)memset_s(host, sizeof(struct I2cCntlr), 0, sizeof(struct I2cCntlr));
    ret = AttachI2cDevice(host, device);
    if (ret != HDF_SUCCESS) {
        printf("%s: attach error\r\n", __func__);
        i2cDriverRelease(device);
        return HDF_FAILURE;
    }
    host->ops = &g_i2cHostMethod;
    device->priv = (void *)host;
    ret = I2cCntlrAdd(host);
    if (ret != HDF_SUCCESS) {
        i2cDriverRelease(device);
        return HDF_FAILURE;
    }
    return ret;
}

static int32_t i2cDriverBind(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        printf("%s: I2c device object is NULL\r\n", __func__);
        return -1;
    }
    return HDF_SUCCESS;
}

static void i2cDriverRelease(struct HdfDeviceObject *device)
{
    struct I2cCntlr *i2cCntrl = NULL;
    struct I2cDevice *i2cDevice = NULL;
    uint16_t port = 0;

    if (device == NULL) {
        printf("%s: device is NULL\r\n", __func__);
        return;
    }
    i2cCntrl = device->priv;
    if (i2cCntrl == NULL) {
        printf("%s: i2cCntrl is NULL\r\n", __func__);
        return;
    }

    i2cDevice = (struct I2cDevice *)i2cCntrl->priv;
    if (i2cDevice == NULL) {
        printf("%s %d: invalid parameter\r\n", __func__, __LINE__);
        return HDF_FAILURE;
    }

    i2cCntrl->ops = NULL;
    if (i2cDevice->mutex != NULL) {
        osMutexDelete(i2cDevice->mutex);
    }
    free(i2cDevice);
    free(i2cCntrl);
    return;
}

static int32_t i2c_transfer(struct I2cDevice *device, struct I2cMsg *msgs, int16_t count)
{
    int ret;
    struct I2cMsg *msg = NULL;
    struct I2cMsg *msg2 = NULL;
    int32_t i2cPort;
    if (device == NULL || msgs == NULL) {
        printf("%s: device or  msgs is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    i2cPort = (int32_t)device->port;
    if (i2cPort > HAL_I2C_ID_NUM) {
        printf("i2c port %d not support\r\n", i2cPort);
        return HDF_FAILURE;
    }
    if (osOK != osMutexWait(device->mutex, osWaitForever)) {
        printf("%s %d osMutexWait fail\r\n", __func__, __LINE__);
        return -1;
    }
    for (int32_t i = 0; i < count; i++) {
        msg = &msgs[i];
        if (msg->addr == I2C_INVALID_ADDR) {
            HostRestI2cDevice(device);
            osMutexRelease(device->mutex);
            return 1;
        }
        if (msg->flags == I2C_FLAG_READ) {
            ret = hal_i2c_task_recv(i2cPort, msg->addr, msg->buf, 0, msg->buf, msg->len, 0, NULL);
            if (ret) {
                printf("%s:%d,i2c recev fail, dev_addr = 0x%x, ret = %d\r\n", __func__, __LINE__, msg->addr, ret);
                osMutexRelease(device->mutex);
                return i;
            }
        } else if (msg->flags == I2C_FLAG_STOP) {
            i++;
            msg2 = &msgs[i];
            ret = hal_i2c_task_recv(i2cPort, msg->addr, msg->buf, msg->len, msg2->buf, msg2->len, 0, NULL);
            if (ret) {
                printf("%s:%d,i2c recev fail, dev_addr = 0x%x, ret = %d\r\n", __func__, __LINE__, msg->addr, ret);
                osMutexRelease(device->mutex);
                return i;
            }
        } else {
            ret = hal_i2c_task_send(i2cPort, msg->addr, msg->buf, msg->len, 0, NULL);
            if (ret) {
                printf("%s:%d,i2c send fail, dev_addr = 0x%x, ret = %d\r\n", __func__, __LINE__, msg->addr, ret);
                osMutexRelease(device->mutex);
                return i;
            }
        }
    }
    osMutexRelease(device->mutex);
    return count;
}

static int32_t i2cHostTransfer(struct I2cCntlr *cntlr, struct I2cMsg *msgs, int16_t count)
{
    struct I2cDevice *device = NULL;
    if (cntlr == NULL || msgs == NULL) {
        printf("%s: I2cCntlr or msgs is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    device = (struct I2cDevice *)cntlr->priv;
    if (device == NULL) {
        printf("%s: I2cDevice is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    return i2c_transfer(device, msgs, count);
}

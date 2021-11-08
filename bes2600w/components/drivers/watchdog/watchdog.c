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
#include "watchdog.h"
#include <stdlib.h>
#include <stdio.h>
#include "device_resource_if.h"
#include "hdf_device_desc.h"
#include "hal_trace.h"
#include "hal_sleep.h"
#include "watchdog_if.h"

static int watchdog_start;
static int watchdog_timeout;

static int32_t WatchdogDevStart(struct WatchdogCntlr *watchdogcntlr);
static int32_t WatchdogDevStop(struct WatchdogCntlr *watchdogcntlr);
static int32_t WatchdogDevSetTimeout(struct WatchdogCntlr *watchdogcntlr, uint32_t seconds);
static int32_t WatchdogDevGetTimeout(struct WatchdogCntlr *watchdogcntlr, uint32_t *seconds);
static int32_t WatchdogDevGetStatus(struct WatchdogCntlr *watchdogcntlr, uint32_t *status);
static int32_t WatchdogDevFeed(struct WatchdogCntlr *watchdogcntlr);

struct WatchdogMethod g_WatchdogCntlrMethod = {
    .getStatus = WatchdogDevGetStatus,
    .setTimeout = WatchdogDevSetTimeout,
    .getTimeout = WatchdogDevGetTimeout,
    .start  = WatchdogDevStart,
    .stop   = WatchdogDevStop,
    .feed   = WatchdogDevFeed,
    .getPriv = NULL,//WatchdogDevGetPriv
    .releasePriv = NULL,//WatchdogDevReleasePriv
};

static void watchdog_irq_handler(enum HAL_WDT_ID_T id, enum HAL_WDT_EVENT_T event)
{
    printf("%s: id %d event %d\r\n", __func__, id, event);
}

static int InitWatchdogDevice(struct WatchdogDevice *watchdogDevice)
{
    if (watchdogDevice == NULL) {
        printf("%s: invaild parameter\r\n", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct WatchdogResource *resource = &watchdogDevice->resource;
    if (resource == NULL) {
        printf("resource is NULL\r\n");
        return HDF_FAILURE;
    }

    int32_t watchdog_id = resource->watchdog_id;
    hal_wdt_set_irq_callback(watchdog_id, watchdog_irq_handler);
    return HDF_SUCCESS;
}

static uint32_t GetWatchdogDeviceResource(
    struct WatchdogDevice *device, const struct DeviceResourceNode *resourceNode)
{
    if (device == NULL || resourceNode == NULL) {
        printf("resource or device is NULL\r\n");
        return HDF_FAILURE;
    }

    struct WatchdogResource *resource = &device->resource;
    struct DeviceResourceIface *dri = NULL;

    dri = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);

    if (dri == NULL || dri->GetUint32 == NULL) {
        printf("DeviceResourceIface is invalid\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "watchdog_id", &resource->watchdog_id, 0) != HDF_SUCCESS) {
        printf("read watchdog_id fail\r\n");
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t AttachWatchdogDevice(struct WatchdogCntlr *watchdogcntlr, struct HdfDeviceObject *device)
{
    int32_t ret;
    struct WatchdogDevice *watchdogDevice = NULL;

    if (device->property == NULL || device == NULL) {
        printf("%s: param is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    watchdogDevice = (struct WatchdogDevice *)malloc(sizeof(struct WatchdogDevice));
    if (watchdogDevice == NULL) {
        printf("%s: malloc watchdogDevice error\r\n", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

    ret = GetWatchdogDeviceResource(watchdogDevice, device->property);
    if (ret != HDF_SUCCESS) {
        (void)free(watchdogDevice);
        return HDF_FAILURE;
    }

    watchdogcntlr->priv = watchdogDevice;
    watchdogcntlr->wdtId = watchdogDevice->resource.watchdog_id;

    return InitWatchdogDevice(watchdogDevice);
}
/* HdfDriverEntry method definitions */
static int32_t WatchdogDriverBind(struct HdfDeviceObject *device);
static int32_t WatchdogDriverInit(struct HdfDeviceObject *device);
static void WatchdogDriverRelease(struct HdfDeviceObject *device);

/* HdfDriverEntry definitions */
struct HdfDriverEntry g_watchdogDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_PLATFORM_WATCHDOG",
    .Bind = WatchdogDriverBind,
    .Init = WatchdogDriverInit,
    .Release = WatchdogDriverRelease,
};

// Initialize HdfDriverEntry
HDF_INIT(g_watchdogDriverEntry);

static int32_t WatchdogDriverBind(struct HdfDeviceObject *device)
{
    struct WatchdogCntlr *watchdogcntlr = NULL;

    if (device == NULL) {
        printf("hdfDevice object is null!\r\n");
        return HDF_FAILURE;
    }

    watchdogcntlr = (struct WatchdogCntlr *)malloc(sizeof(struct WatchdogCntlr));
    if (watchdogcntlr == NULL) {
        printf("%s: malloc watchdogcntlr error\r\n", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

    printf("Enter %s\r\n", __func__);
    device->service = &watchdogcntlr->service;
    watchdogcntlr->device = device;
    watchdogcntlr->priv = NULL;
    return HDF_SUCCESS;
}

static int32_t WatchdogDriverInit(struct HdfDeviceObject *device)
{
    int32_t ret;
    struct WatchdogCntlr *watchdogcntlr = NULL;

    if (device == NULL) {
        printf("%s: device is NULL\r\n", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    printf("Enter %s:\r\n", __func__);

    watchdogcntlr = WatchdogCntlrFromDevice(device);
    if (watchdogcntlr == NULL) {
        printf("%s: watchdogcntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    ret = AttachWatchdogDevice(watchdogcntlr, device);
    if (ret != HDF_SUCCESS) {
        free(watchdogcntlr);
        printf("%s:attach error\r\n", __func__);
        return HDF_FAILURE;
    }

    watchdogcntlr->ops = &g_WatchdogCntlrMethod;
  
    if (!ret) 
        printf("WatchdogDriverInit success!\r\n");
    return ret;
}

static void WatchdogDriverRelease(struct HdfDeviceObject *device)
{
    struct WatchdogCntlr *watchdogcntlr = NULL;
    struct WatchdogDevice *watchdogDevice = NULL;

    if (device == NULL) {
        printf("device is null\r\n");
        return;
    }

    watchdogcntlr = WatchdogCntlrFromDevice(device);
    if (watchdogcntlr == NULL) {
        printf("%s: watchdogcntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    watchdogDevice = (struct WatchdogDevice *)watchdogcntlr->priv;
    if (watchdogDevice != NULL) {
        free(watchdogDevice);
    }
    return;
}

static int32_t WatchdogDevStart(struct WatchdogCntlr *watchdogcntlr)
{
    struct WatchdogDevice *watchdogDevice = NULL;

    if (watchdogcntlr== NULL) {
        printf("%s: watchdogcntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    watchdogDevice = (struct WatchdogDevice *)watchdogcntlr->priv;
    int32_t watchdog_id = watchdogDevice->resource.watchdog_id;

    //hal_wdt_set_timeout(watchdog_id, seconds);
    hal_wdt_start(watchdog_id);
    //hal_sleep_set_deep_sleep_hook(HAL_DEEP_SLEEP_HOOK_USER_WDT, watchdog_feed);
    watchdog_start = 1;
    return HDF_SUCCESS;
}

static int32_t WatchdogDevStop(struct WatchdogCntlr *watchdogcntlr)
{
    struct WatchdogDevice *watchdogDevice = NULL;

    if (watchdogcntlr== NULL) {
        printf("%s: watchdogcntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    watchdogDevice = (struct WatchdogDevice *)watchdogcntlr->priv;
    int32_t watchdog_id = watchdogDevice->resource.watchdog_id;
    hal_wdt_stop(watchdog_id);
    watchdog_start = 0;
    return HDF_SUCCESS;
}

static int32_t WatchdogDevSetTimeout(struct WatchdogCntlr *watchdogcntlr, uint32_t seconds)
{
    struct WatchdogDevice *watchdogDevice = NULL;

    if (watchdogcntlr== NULL) {
        printf("%s: watchdogcntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    watchdog_timeout = seconds;
    watchdogDevice = (struct WatchdogDevice *)watchdogcntlr->priv;
    int32_t watchdog_id = watchdogDevice->resource.watchdog_id;
    hal_wdt_set_timeout(watchdog_id, seconds);
    return HDF_SUCCESS;
}

static int32_t WatchdogDevGetTimeout(struct WatchdogCntlr *watchdogcntlr, uint32_t *seconds)
{
    *seconds = watchdog_timeout;
    return HDF_SUCCESS;
}

static int32_t WatchdogDevGetStatus(struct WatchdogCntlr *watchdogcntlr, uint32_t *status)
{   
    if(watchdog_start == 1){
        *status = WATCHDOG_START;
    } else{
        *status = WATCHDOG_STOP;
    }
    return HDF_SUCCESS;
}

static int32_t WatchdogDevFeed(struct WatchdogCntlr *watchdogcntlr)
{
    struct WatchdogDevice *watchdogDevice = NULL;

    if (watchdogcntlr== NULL) {
        printf("%s: watchdogcntlr is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    watchdogDevice = (struct WatchdogDevice *)watchdogcntlr->priv;
    int32_t watchdog_id = watchdogDevice->resource.watchdog_id;
    hal_wdt_ping(watchdog_id);
    return HDF_SUCCESS;
}

static int32_t WatchdogDevGetPriv(struct WatchdogCntlr *watchdogcntlr)
{
    
}

static int32_t WatchdogDevReleasePriv(struct WatchdogCntlr *watchdogcntlr)
{
    
}
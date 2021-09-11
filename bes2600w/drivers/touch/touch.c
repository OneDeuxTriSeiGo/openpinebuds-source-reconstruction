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
#include "touch.h"
#include "hdf_platform.h"
#include "hdf_log.h"

#define TOUCH_MAX 1
struct TouchManager {
    struct touch_device *device[TOUCH_MAX];
    uint32_t deviceNum;
};
static struct TouchManager g_touchManager;

int32_t RegisterTouchDevice(struct touch_device *device)
{
    uint32_t deviceNum;

    if (device == NULL) {
        HDF_LOGE("%s: device is null", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    deviceNum = g_touchManager.deviceNum;
    if (deviceNum >= TOUCH_MAX) {
        HDF_LOGE("%s: deviceNum > TOUCH_MAX", __func__);
        return HDF_FAILURE;
    }
    if (device->init == NULL || device->read == NULL || device->irq_enable == NULL) {
        HDF_LOGE("%s: device invalid", __func__);
        return HDF_FAILURE;
    }
    g_touchManager.device[deviceNum] = device;
    g_touchManager.deviceNum++;
    return HDF_SUCCESS;
}

static void touch_task(void *arg)
{
    struct touch_device *dev = (struct touch_device *)arg;
    dev->irq_enable(true); // enable irq first
    while (1) {
        if (osSemaphoreAcquire(dev->sem, osWaitForever) != 0) {
            continue;
        }
        dev->irq_enable(false); // avoid too much irq
        struct touch_msg msg;
        if (dev->read(dev, &msg) == 0) {
            if (msg.event != TOUCH_EVENT_NONE) {
                if (osMessageQueuePut(dev->mq, &msg, 0, 0) != 0) {
                    HDF_LOGW("osMessageQueuePut touch_msg failed");
                }
            }
        }
        dev->irq_enable(true); // enable irq after data process
    }
}

int TouchRead(DevHandle handle, struct touch_msg *msg, uint32_t timeout)
{
    struct touch_device *dev = (struct touch_device *)handle;
    if (!dev || !dev->mq || !msg) {
        HDF_LOGE("%s: arg error", __func__);
        return -1;
    }
    return osMessageQueueGet(dev->mq, msg, NULL, timeout);
}

DevHandle TouchOpen(int id)
{
    if (id < 0 || id >= g_touchManager.deviceNum) {
        HDF_LOGE("%s: arg error", __func__);
        return NULL;
    }
    struct touch_device *dev = g_touchManager.device[id];
    if (dev->init(dev) != 0) {
        HDF_LOGE("%s: touch device init failed", __func__);
        return NULL;
    }

    dev->sem = osSemaphoreNew(1, 0, NULL);
    if (dev->sem == NULL) {
        TouchClose(dev);
        HDF_LOGE("%s: osSemaphoreNew failed", __func__);
        return NULL;
    }
    // Here msg_count = events_count = 4, in case of osMessageQueuePut failed when timeout = 0
    dev->mq = osMessageQueueNew(4, sizeof(struct touch_msg), NULL);
    if (dev->mq == NULL) {
        TouchClose(dev);
        HDF_LOGE("%s: osSemaphoreNew failed", __func__);
        return NULL;
    }

    osThreadAttr_t attr = {0};
    attr.stack_size = 2048;
    attr.priority = osPriorityNormal;
    attr.name = dev->name;
    dev->tid = osThreadNew((osThreadFunc_t)touch_task, dev, &attr);
    if (dev->tid == NULL) {
        TouchClose(dev);
        HDF_LOGE("%s: osThreadNew failed", __func__);
        return NULL;
    }
    return dev;
}

void TouchClose(DevHandle handle)
{
    struct touch_device *dev = (struct touch_device *)handle;
    if (!dev) {
        return;
    }
    if (dev->deinit) {
        dev->deinit();
    }
    if (dev->mq) {
        osMessageQueueDelete(dev->mq);
        dev->mq = NULL;
    }
    if (dev->sem) {
        osSemaphoreDelete(dev->sem);
        dev->sem = NULL;
    }
    if (dev->tid) {
        osThreadTerminate(dev->tid);
        dev->tid = NULL;
    }
}
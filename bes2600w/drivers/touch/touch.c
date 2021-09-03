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
#include "platform.h"

extern struct touch_device g_touch_ztw523;
static struct touch_device *g_touch_device[] = {&g_touch_ztw523};

static void touch_task(void *arg)
{
    DBG_ASSERT(arg != NULL);
    struct touch_device *dev = (struct touch_device *)arg;
    dev->irq_enable(true); // enable irq first
    while (1) {
        if (osSemaphoreAcquire(dev->sem, osWaitForever) != 0) {
            continue;
        }
        dev->irq_enable(false); // avoid too much irq ??
        struct touch_msg msg;
        if (dev->read(dev, &msg) == 0) {
            if (msg.event != TOUCH_EVENT_NONE) {
                if (osMessageQueuePut(dev->mq, &msg, 0, 0) != 0) {
                    LOG_W("osMessageQueuePut touch_msg failed");
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
        LOG_E("arg error");
        return -1;
    }
    return osMessageQueueGet(dev->mq, msg, NULL, timeout);
}

DevHandle TouchOpen(int id)
{
    if (id < 0 || id >= P_ARRAY_SIZE(g_touch_device)) {
        LOG_E("arg error");
        return NULL;
    }
    struct touch_device *dev = g_touch_device[id];
    DBG_ASSERT(dev->init && dev->read && dev->irq_enable);
    if (dev->init(dev) != 0) {
        LOG_E("touch device init failed");
        return NULL;
    }

    dev->sem = osSemaphoreNew(1, 0, NULL);
    DBG_ASSERT(dev->sem != NULL);
    // Here msg_count = events_count = 4, in case of osMessageQueuePut failed when timeout = 0
    dev->mq = osMessageQueueNew(4, sizeof(struct touch_msg), NULL);
    DBG_ASSERT(dev->mq != NULL);

    osThreadAttr_t attr = {0};
    attr.stack_size = 2048;
    attr.priority = osPriorityNormal;
    attr.name = dev->name;
    dev->tid = osThreadNew((osThreadFunc_t)touch_task, dev, &attr);
    DBG_ASSERT(dev->tid != NULL);
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
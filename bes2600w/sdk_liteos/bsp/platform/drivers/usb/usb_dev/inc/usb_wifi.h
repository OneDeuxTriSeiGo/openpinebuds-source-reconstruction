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
#ifndef __USB_WIFI_H__
#define __USB_WIFI_H__
#include "plat_types.h"
#include "hal_usb.h"
#include "stdint.h"

#define EPINT_IN                            1
#define EPBULK_IN                           2
#define EP3BULK_IN                          3
#define EP4BULK_IN                          4 //bluetooth RX

#define EPBULK_OUT                          2
#define EP3BULK_OUT                         3
#define EP4BULK_OUT                         4//bluetooth TX
typedef void (*USB_WIFI_DEVICE_STATUS_CALLBACK)(uint32_t status, uint32_t param);


struct USB_WIFI_CFG_T {
    USB_WIFI_DEVICE_STATUS_CALLBACK status_callback;
};

int usb_wifi_open(const struct USB_WIFI_CFG_T *cfg);
#endif /*__USB_WIFI_H__*/
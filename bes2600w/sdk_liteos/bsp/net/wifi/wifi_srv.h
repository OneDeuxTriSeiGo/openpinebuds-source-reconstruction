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
#ifndef __WIFI_SRV_H__
#define __WIFI_SRV_H__

#include "wifi_agent.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WIFI_SRV_UNINIT = 0,
    WIFI_SRV_READY,
    WIFI_SRV_NET_READY,
} WIFI_SRV_STATUS_T;

int wifi_srv_send2peer(int msg, int data_len, void *data, int sync);
int wifi_srv_init(void);

extern struct wifi_init_param wifi_param;

#ifdef __cplusplus
}
#endif

#endif



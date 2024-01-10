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
#ifndef __APP_DIP_H__
#define __APP_DIP_H__

#ifdef __cplusplus
extern "C" {
#endif

//the mobile connect type
typedef enum {
    MOBILE_CONNECT_IDLE,
    MOBILE_CONNECT_IOS,
    MOBILE_CONNECT_ANDROID,
} MOBILE_CONN_TYPE_E;

typedef struct
{
    uint16_t spec_id;
    uint16_t vend_id;
    uint16_t prod_id;
    uint16_t prod_ver;
    uint8_t  prim_rec;
    uint16_t vend_id_source;
} app_dip_pnp_info_t;

void app_dip_sync_init(void);
void app_dip_init(void);

typedef void (*app_dip_info_queried_callback_func_t)(uint8_t *remote_addr, app_dip_pnp_info_t *pnp_info);

void app_dip_register_dip_info_queried_callback(app_dip_info_queried_callback_func_t func);

void app_dip_get_remote_info(uint8_t device_id);

#ifdef __cplusplus
}
#endif

#endif

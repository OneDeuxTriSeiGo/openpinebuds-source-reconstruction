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
#ifndef __APP_SPP_TOTA_H__
#define __APP_SPP_TOTA_H__

#include "me_api.h"

#define TOTA_SPP_ID					 0
#define TOTA_SPP_MAX_PACKET_SIZE     666
#define TOTA_SPP_MAX_PACKET_NUM      5


#define TOTA_SPP_TX_BUF_SIZE	    (TOTA_SPP_MAX_PACKET_SIZE*TOTA_SPP_MAX_PACKET_NUM)

#define APP_TOTA_DATA_CMD_TIME_OUT_IN_MS	5000

typedef void(*app_spp_tota_tx_done_t)(void);
void app_spp_tota_register_tx_done(app_spp_tota_tx_done_t callback);
void app_spp_tota_init(void);
void app_tota_send_cmd_via_spp(uint8_t* ptrData, uint32_t length);
void app_tota_send_data_via_spp(uint8_t* ptrData, uint32_t length);

uint16_t app_spp_tota_tx_buf_size(void);
void app_spp_tota_init_tx_buf(uint8_t* ptr);

void app_spp_tota_client_open(btif_remote_device_t *remDev);

#endif


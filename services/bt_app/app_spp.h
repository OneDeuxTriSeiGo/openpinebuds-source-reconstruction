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
#ifndef __APP_SPP_H__
#define __APP_SPP_H__

#include "spp_api.h"
#include "sdp_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__3M_PACK__)
#define L2CAP_MTU                           980
#else
#define L2CAP_MTU                           672
#endif

#define SPP_RECV_BUFFER_SIZE   L2CAP_MTU*4
#define SPP_MAX_DATA_PACKET_SIZE    L2CAP_MTU


/*---------------------------------------------------------------------------
 * rfcomm channel number
 *      should be from 1 to 30
 */
enum RFCOMM_CHANNEL_NUM {
    RFCOMM_CHANNEL_GS_CONTROL   = RFCOMM_CHANNEL_1,
    RFCOMM_CHANNEL_GS_AUDIO     = RFCOMM_CHANNEL_2,
    RFCOMM_CHANNEL_TOTA         = RFCOMM_CHANNEL_3,
    RFCOMM_CHANNEL_BES_OTA      = RFCOMM_CHANNEL_4,
    RFCOMM_CHANNEL_AI_VOICE     = RFCOMM_CHANNEL_5,
    RFCOMM_CHANNEL_GREEN        = RFCOMM_CHANNEL_6,
    RFCOMM_CHANNEL_RED          = RFCOMM_CHANNEL_7,
    RFCOMM_CHANNEL_FP           = RFCOMM_CHANNEL_8,
    RFCOMM_CHANNEL_AMA          = RFCOMM_CHANNEL_9,
};

struct spp_device *app_create_spp_device(void);

bt_status_t app_spp_send_data(struct spp_device *osDev_t, uint8_t* ptrData, uint16_t *length);
void app_spp_open(struct spp_device *osDev_t, btif_remote_device_t  *btDevice, btif_sdp_record_param_t *param, osMutexId mid, uint8_t service_id, spp_callback_t callback);

#ifdef __cplusplus
}
#endif

#endif

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
#ifndef __APP_HFP_H__
#define __APP_HFP_H__

#include "bluetooth.h"
#include "btapp.h"

#define HF_VOICE_DISABLE  0
#define HF_VOICE_ENABLE   1

#define HF_SENDBUFF_SIZE (320)
#define HF_SENDBUFF_MEMPOOL_NUM (2)

struct hf_sendbuff_control {
    struct {
        btif_bt_packet_t packet;
        uint8_t buffer[HF_SENDBUFF_SIZE];
    } mempool[HF_SENDBUFF_MEMPOOL_NUM];
    uint8_t index;
};


#ifndef _SCO_BTPCM_CHANNEL_
extern struct hf_sendbuff_control  hf_sendbuff_ctrl;
#endif

#ifdef __cplusplus
extern "C" {
#endif                          /*  */

#if defined(SUPPORT_BATTERY_REPORT) || defined(SUPPORT_HF_INDICATORS)
int app_hfp_battery_report(uint8_t level);
bt_status_t app_hfp_send_at_command(const char *cmd);
#else
static inline int app_hfp_battery_report(uint8_t level) {return 0;}
#endif

uint8_t btapp_hfp_get_call_state(void);

uint8_t btapp_hfp_incoming_calls(void);

bool btapp_hfp_is_call_active(void);

bool btapp_hfp_is_sco_active(void);

void btapp_hfp_report_speak_gain(void);

bool btapp_hfp_mic_need_skip_frame(void);

uint8_t btapp_hfp_need_mute(void);

#ifdef __INTERCONNECTION__
uint8_t ask_is_selfdefined_battery_report_AT_command_support(void);

uint8_t send_selfdefined_battery_report_AT_command(uint8_t device_id);
#endif

bool app_hfp_curr_audio_up(btif_hf_channel_t* hfp_chnl);


#ifdef __cplusplus
}
#endif                          /*  */
#endif /*__APP_HFP_H__*/

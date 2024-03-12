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
#ifndef __BT_IF_H_
#define __BT_IF_H_
#include <stdint.h>
#include "bluetooth.h"
#include "btif_sys_config.h"
#include "me_api.h"

//Application ID,indentify profle app context
#define    BTIF_APP_INVALID_ID              ((uint64_t)(0x0000000000000000))
#define    BTIF_APP_HFP_PROFILE_ID          ((uint64_t)(0x0000000000000001))
#define    BTIF_APP_A2DP_PROFILE_ID         ((uint64_t)(0x0000000000000002))
#define    BTIF_APP_AVRCP_PROFILE_ID        ((uint64_t)(0x0000000000000004))
#define    BTIF_APP_MAP_PROFILE_ID          ((uint64_t)(0X0000000000000008))
#define    BTIF_APP_BTGATT_PROFILE_ID       ((uint64_t)(0x0000000000000010))

#define    BTIF_APP_SPP_SERVER_START_ID     ((uint64_t)(0x0000000000000020))
#define    BTIF_APP_SPP_SERVER_END_ID       ((uint64_t)(BTIF_APP_SPP_SERVER_START_ID << (BTIF_APP_SPP_SERVER_NUM - 1)))
#define    BTIF_APP_SPP_SERVER_OFFSET       (5)
#define    BTIF_APP_SPP_SERVER_NUM          (30)
#define    BTIF_APP_SPP_SERVER_MASK         ((uint64_t)(0x000000003FFFFFFF<<BTIF_APP_SPP_SERVER_OFFSET))

#define    BTIF_APP_SPP_CLIENT_START_ID     ((uint64_t)(BTIF_APP_SPP_SERVER_END_ID << 1))
#define    BTIF_APP_SPP_CLIENT_NUM          (10)
#define    BTIF_APP_SPP_CLIENT_END_ID       ((uint64_t)(BTIF_APP_SPP_CLIENT_START_ID << (BTIF_APP_SPP_CLIENT_NUM - 1)))
#define    BTIF_APP_SPP_OFFSET              (5)
#define    BTIF_APP_SPP_NUM                 (BTIF_APP_SPP_SERVER_NUM+BTIF_APP_SPP_CLIENT_NUM)
#define    BTIF_APP_SPP_MASK                ((uint64_t)(0x000000FFFFFFFFFF<<BTIF_APP_SPP_OFFSET))

#define    BTIF_TWS_LINK_CONNECTED          (1 << 0)
#define    BTIF_TWS_L2CAP_CONNECTED         (1 << 1)


#if defined(IBRT)
uint64_t btif_app_get_app_id_from_spp_flag(uint8_t spp_flag);
uint8_t btif_app_get_spp_flag_from_app_id(uint64_t app_id);
#endif

#ifdef __cplusplus
extern "C" {
#endif                          /*  */

enum pair_event
{
    PAIR_EVENT_NUMERIC_REQ,
    PAIR_EVENT_COMPLETE,
    PAIR_EVENT_FAILED,
};

typedef void (*pairing_callback_t) (enum pair_event evt, const btif_event_t *event);
typedef void (*authing_callback_t) (void);

typedef void (*stack_ready_callback_t) (int status);
int bt_stack_register_ready_callback(stack_ready_callback_t ready_cb);
int bt_stack_initilize(void);
int bt_pairing_init(pairing_callback_t pair_cb);
#ifdef BTIF_SECURITY
int bt_authing_init(authing_callback_t auth_cb);
#else
static inline int bt_authing_init(authing_callback_t auth_cb)
{
    return 0;
}
#endif
int bt_stack_config(const unsigned char *dev_name, uint8_t len);
void btif_update_bt_name(const unsigned char *dev_name, uint8_t len);
int bt_set_local_dev_name(const unsigned char *dev_name, uint8_t len);
void bt_process_stack_events(void);
void bt_generate_ecdh_key_pair(void);
uint8_t bt_get_max_sco_number(void);
void bt_set_max_sco_number(uint8_t sco_num);
void bt_fast_init(uint8_t* bt_addr, uint8_t* ble_addr);

void btif_set_btstack_chip_config(void *config);

void btif_avrcp_ct_register_notification_event(uint8_t device_id, uint8_t event);


#if defined(IBRT)
uint32_t btif_save_app_bt_device_ctx(uint8_t *ctx_buffer,uint8_t psm_context_mask);
uint32_t btif_set_app_bt_device_ctx(uint8_t *ctx_buffer,uint8_t psm_context_mask,uint8_t bt_devices_idx, uint8_t rm_detbl_idx, uint8_t avd_ctx_device_idx);
#endif

bool btif_hf_service_link_is_up(void);
bool btif_hf_audio_link_is_up(void);

void btif_pts_av_create_channel(bt_bdaddr_t *btaddr);
void btif_pts_av_disc_channel(void);
void btif_pts_av_set_sink_delay(void);
void btif_pts_ar_connect(bt_bdaddr_t *btaddr);
void btif_pts_ar_disconnect(void);
void btif_pts_ar_panel_stop(void);
void btif_pts_ar_panel_play(void);
void btif_pts_ar_panel_pause(void);
void btif_pts_ar_panel_forward(void);
void btif_pts_ar_panel_backward(void);
void btif_pts_ar_volume_up(void);
void btif_pts_ar_volume_down(void);
void btif_pts_ar_volume_notify(void);
void btif_pts_ar_volume_change(void);
void btif_pts_ar_set_absolute_volume(void);
void btif_pts_hf_create_service_link(bt_bdaddr_t *btaddr);
void btif_pts_hf_disc_service_link(void);
void btif_pts_hf_create_audio_link(void);
void btif_pts_hf_disc_audio_link(void);
void btif_pts_hf_redial_call(void);
void btif_pts_hf_answer_call(void);
void btif_pts_hf_hangup_call(void);
void btif_pts_hf_vr_enable(void);
void btif_pts_hf_vr_disable(void);
void btif_pts_hf_list_current_calls(void);
void btif_pts_hf_release_active_call_2(void);
void btif_pts_hf_hold_active_call_2(void);
void btif_pts_hf_release_active_call(void);
void btif_pts_hf_send_ind_1(void);
void btif_pts_hf_send_ind_2(void);
void btif_pts_hf_send_ind_3(void);
void btif_pts_hf_update_ind_value(void);
void btif_pts_rfc_register_channel(void);
void btif_pts_rfc_close(void);
void btif_pts_rfc_close_dlci_0(void);
void btif_pts_rfc_send_data(void);
void btif_pts_av_send_discover(void);
void btif_pts_av_send_getcap(void);
void btif_pts_av_send_setconf(void);
void btif_pts_av_send_getconf(void);
void btif_pts_av_send_reconf(void);
void btif_pts_av_send_open(void);
void btif_pts_av_send_close(void);
void btif_pts_av_send_abort(void);
void btif_pts_av_send_getallcap(void);
void btif_pts_av_send_suspend(void);
void btif_pts_av_send_start(void);
void btif_pts_av_create_media_channel(void);
void btif_pts_l2c_disc_channel(void);
void btif_pts_l2c_send_data(void);

#ifdef __cplusplus
}
#endif /*  */

#endif /*__BT_IF_H_*/

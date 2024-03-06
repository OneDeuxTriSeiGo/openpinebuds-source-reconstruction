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
#ifndef __APP_BT_H__
#define __APP_BT_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "me_api.h"
#include "a2dp_api.h"
#include "hfp_api.h"

enum APP_BT_REQ_T {
    APP_BT_REQ_ACCESS_MODE_SET,
    APP_BT_REQ_AUTO_TEST,
    APP_BT_REQ_NUM
};

enum APP_BT_GOLBAL_HANDLE_HOOK_USER_T {
    APP_BT_GOLBAL_HANDLE_HOOK_USER_0 = 0,
    APP_BT_GOLBAL_HANDLE_HOOK_USER_1,
    APP_BT_GOLBAL_HANDLE_HOOK_USER_QTY
};

typedef void (*APP_BT_REQ_CONNECT_PROFILE_FN_T)(void *, void *);
typedef bt_status_t  (*APP_BT_REQ_HF_OP_FN_T)(void *);
typedef void (*APP_BT_GOLBAL_HANDLE_HOOK_HANDLER)(const btif_event_t*Event);

#define app_bt_accessmode_set_req(accmode) do{app_bt_send_request(APP_BT_REQ_ACCESS_MODE_SET, accmode, 0, 0,0);}while(0)

typedef enum
{
    // always in active mode during a2dp streaming
    ACTIVE_MODE_KEEPER_A2DP_STREAMING = 0,
    // always in active mode during ai voice up-streaming
    ACTIVE_MODE_KEEPER_AI_VOICE_STREAM,
    // always in active mode during OTA
    ACTIVE_MODE_KEEPER_OTA,
    // stay in active mode for 15 seconds after role switch
    ACTIVE_MODE_KEEPER_ROLE_SWITCH,
    // stay in active mode during synchronous voice prompt playing
    ACTIVE_MODE_KEEPER_SYNC_VOICE_PROMPT,
    // always in active mode during phone call
    ACTIVE_MODE_KEEPEER_SCO_STREAMING,


} BT_LINK_ACTIVE_MODE_KEEPER_USER_E;

#define MAX_ACTIVE_MODE_MANAGED_LINKS       3
#define UPDATE_ACTIVE_MODE_FOR_ALL_LINKS    MAX_ACTIVE_MODE_MANAGED_LINKS

void app_bt_active_mode_manager_init(void);

void app_bt_active_mode_reset(uint32_t linkIndex);

void app_bt_active_mode_set(BT_LINK_ACTIVE_MODE_KEEPER_USER_E user, uint32_t linkIndex);

void app_bt_active_mode_clear(BT_LINK_ACTIVE_MODE_KEEPER_USER_E user, uint32_t linkIndex);

void app_bt_global_handle(const btif_event_t *Event);
bool is_app_bt_pairing_running(void);

void PairingTransferToConnectable(void);

void app_bt_global_handle_init(void);

void app_bt_opening_reconnect(void);

void app_bt_accessmode_set(  btif_accessible_mode_t mode);

int app_bt_send_request(uint32_t message_id, uint32_t param0, uint32_t param1, uint32_t param2,uint32_t ptr);

void app_bt_init(void);

int app_bt_state_checker(void);

void *app_bt_profile_active_store_ptr_get(uint8_t *bdAddr);


void app_bt_profile_connect_manager_opening_reconnect(void);

BOOL app_bt_profile_connect_openreconnecting(void *ptr);

int app_bt_global_handle_hook_set(enum APP_BT_GOLBAL_HANDLE_HOOK_USER_T user, APP_BT_GOLBAL_HANDLE_HOOK_HANDLER handler);

APP_BT_GOLBAL_HANDLE_HOOK_HANDLER app_bt_global_handle_hook_get(enum APP_BT_GOLBAL_HANDLE_HOOK_USER_T user);

bool app_is_hfp_service_connected(uint8_t device_id);
#if defined(IBRT)
void app_bt_ibrt_reconnect_mobile_profile_flag_set(void);
void app_bt_ibrt_reconnect_mobile_profile_flag_clear(void);
bool app_bt_ibrt_reconnect_mobile_profile_flag_get(void);
#endif







void app_bt_stay_active(uint8_t deviceId);

void app_bt_allow_sniff(uint8_t deviceId);

btif_remote_device_t* app_bt_get_remoteDev(uint8_t deviceId);

void app_bt_stop_sniff(uint8_t deviceId);

void app_bt_sniff_config(btif_remote_device_t *remDev);

void app_bt_allow_sniff_rem_dev(btif_remote_device_t* pRemDev);

void app_bt_stay_active_rem_dev(btif_remote_device_t* pRemDev);

void app_check_pending_stop_sniff_op(void);


uint8_t app_bt_count_connected_device(void);

bool app_bt_is_hfp_connected(uint8_t device_id);

void app_bt_pause_media_player_again(uint8_t deviceId);

bool app_bt_is_music_player_working(uint8_t deviceId);

bool app_bt_pause_music_player(uint8_t deviceId);

void app_bt_resume_music_player(uint8_t deviceId);

bool app_bt_is_a2dp_connected(uint8_t device_id);




bool app_bt_is_a2dp_streaming(uint8_t deviceId);

void app_bt_suspend_a2dp_streaming(uint8_t deviceId);

bool app_bt_get_device_bdaddr(uint8_t deviceId, uint8_t* btAddr);

void fast_pair_enter_pairing_mode_handler(void);




bool app_bt_is_in_reconnecting(void);

bool btapp_hfp_is_dev_sco_connected(uint8_t devId);


bool app_bt_is_in_connecting_profiles_state(void);

void app_bt_clear_connecting_profiles_state(uint8_t devId);

void app_bt_set_connecting_profiles_state(uint8_t devId);




#if defined(__INTERCONNECTION__)
btif_accessible_mode_t app_bt_get_current_access_mode(void);

#endif
bool app_bt_is_hfp_audio_on(void);

btif_remote_device_t* app_bt_get_connected_mobile_device_ptr(void);
void app_bt_set_spp_device_ptr(btif_remote_device_t* device);
btif_remote_device_t* app_bt_get_spp_device_ptr(void);

bool app_is_disconnecting_all_bt_connections(void);

void app_set_disconnecting_all_bt_connections(bool isEnable);

void app_bt_start_search(void);

#if defined(IBRT)
uint32_t app_bt_save_a2dp_app_ctx(btif_remote_device_t *rem_dev, uint8_t *buf, uint32_t buf_len);
uint32_t app_bt_restore_a2dp_app_ctx(bt_bdaddr_t *bdaddr_p, uint8_t *buf, uint32_t buf_len);
uint32_t app_bt_save_avrcp_app_ctx(btif_remote_device_t *rem_dev, uint8_t *buf, uint32_t buf_len);
uint32_t app_bt_restore_avrcp_app_ctx(bt_bdaddr_t *bdaddr_p, uint8_t *buf, uint32_t buf_len);
uint32_t app_bt_save_hfp_app_ctx(btif_remote_device_t *rem_dev, uint8_t *buf, uint32_t buf_len);
uint32_t app_bt_restore_hfp_app_ctx(bt_bdaddr_t *bdaddr_p, uint8_t *buf, uint32_t buf_len);
uint32_t app_bt_save_spp_app_ctx(uint64_t app_id,btif_remote_device_t *rem_dev, uint8_t *buf, uint32_t buf_len);
uint32_t app_bt_restore_spp_app_ctx(bt_bdaddr_t *bdaddr_p, uint8_t *buf, uint32_t buf_len, uint64_t app_id);
#ifdef BT_MAP_SUPPORT
uint32_t app_bt_save_map_app_ctx(btif_remote_device_t *rem_dev, uint8_t *buf, uint32_t buf_len);
uint32_t app_bt_restore_map_app_ctx(bt_bdaddr_t *bdaddr_p, uint8_t *buf, uint32_t buf_len);
#endif
#endif

void app_stop_fast_connectable_ble_adv_timer(void);
int8_t app_bt_get_rssi(void);

#ifdef  TILE_DATAPATH
int8_t app_tile_get_ble_rssi(void);
#endif

#ifdef __cplusplus
}
#endif
#endif /* BESBT_H */

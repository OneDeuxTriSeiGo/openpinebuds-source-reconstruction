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
#ifndef __APP_BT_PBAP_H__
#define __APP_BT_PBAP_H__

#ifdef BT_PBAP_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif

struct btif_pbap_channel_t;

void app_bt_pbap_init(void);

bool app_bt_pbap_is_connected(struct btif_pbap_channel_t *pbap_chan);

void app_bt_reconnect_pbap_profile(bt_bdaddr_t *remote);

void app_bt_disconnect_pbap_profile(struct btif_pbap_channel_t *pbap_chan);

void app_bt_pbap_send_obex_disconnect_req(struct btif_pbap_channel_t *pbap_chan);

void app_bt_pbap_send_abort_req(struct btif_pbap_channel_t *pbap_chan);

void app_bt_pbap_get_phonebook_size(struct btif_pbap_channel_t *pbap_chan, const char* phonebook_object_path_name);

void app_bt_pbap_pull_single_phonebook(struct btif_pbap_channel_t *pbap_chan, const char* phonebook_object_path_name, uint16_t pb_index);

void app_bt_pbap_client_test(void);

void app_bt_pts_pbap_create_channel(void);

void app_bt_pts_pbap_disconnect_channel(void);

void app_bt_pts_pbap_send_obex_disc_req(void);

void app_bt_pts_pbap_send_obex_get_req(void);

void app_bt_pts_pbap_send_auth_conn_req(void);

void app_bt_pts_pbap_send_abort(void);

void app_bt_pts_pbap_pull_phonebook_pb(void);

void app_bt_pts_pbap_get_phonebook_size(void);

void app_bt_pts_pbap_set_path_to_root(void);

void app_bt_pts_pbap_set_path_to_parent(void);

void app_bt_pts_pbap_enter_path_to_telecom(void);

void app_bt_pts_pbap_list_phonebook_pb(void);

void app_bt_pts_pbap_list_phonebook_pb_size(void);

void app_bt_pts_pbap_pull_pb_entry_n_tel(void);

void app_bt_pts_pbap_pull_uid_entry(void);

void app_bt_pts_pbap_set_path_to(const char* name, uint32_t len);

void app_bt_pts_pbap_list_phonebook(const char* name, uint32_t len);

void app_bt_pts_pbap_list_reset_missed_call(const char* name, uint32_t len);

void app_bt_pts_pbap_list_vcard_select(const char* name, uint32_t len);

void app_bt_pts_pbap_list_vcard_search(const char* name, uint32_t len);

void app_bt_pts_pbap_pull_phonebook(const char* name, uint32_t len);

void app_bt_pts_pbap_pull_reset_missed_call(const char* name, uint32_t len);

void app_bt_pts_pbap_pull_vcard_select(const char* name, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* BT_PBAP_SUPPORT */

#endif /* __APP_BT_PBAP_H__ */


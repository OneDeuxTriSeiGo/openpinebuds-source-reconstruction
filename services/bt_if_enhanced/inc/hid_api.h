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
#ifndef __BTIF_HID_API_H__
#define __BTIF_HID_API_H__

#ifdef BT_HID_DEVICE

#ifdef __cplusplus
extern "C" {
#endif

struct hid_control_t;
struct hid_callback_parms_t;

typedef enum {
    HID_DEVICE_ROLE,
    HID_HOST_ROLE,
} hid_role_enum_t;

typedef enum {
    BTIF_HID_EVENT_REMOTE_NOT_SUPPORT = 1,
    BTIF_HID_EVENT_CONN_OPENED,
    BTIF_HID_EVENT_CONN_CLOSED,
} btif_hid_event_t;

typedef struct
{
    btif_hid_event_t event;
    uint8_t error_code;
    bt_bdaddr_t remote;
} btif_hid_callback_param_t;

void btif_hid_init(void (*cb)(uint8_t device_id, struct hid_control_t *hid_ctl, const struct hid_callback_parms_t *info), hid_role_enum_t role);

struct hid_control_t *btif_hid_channel_alloc(void);

bt_status_t btif_hid_connect(bt_bdaddr_t *addr);

void btif_hid_disconnect(struct hid_control_t *hid_ctl);

bool btif_hid_is_connected(struct hid_control_t *hid_ctl);

int btif_hid_get_state(struct hid_control_t *hid_ctl);

void btif_hid_keyboard_input_report(struct hid_control_t *hid_ctl, uint8_t modifier_key, uint8_t key_code);

void btif_hid_keyboard_send_ctrl_key(struct hid_control_t *hid_ctl, uint8_t ctrl_key);

#ifdef __cplusplus
}
#endif

#endif /* BT_HID_DEVICE */

#endif /* __BTIF_HID_API_H__ */


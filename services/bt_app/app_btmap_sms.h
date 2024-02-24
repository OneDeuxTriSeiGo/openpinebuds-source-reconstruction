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
#ifndef __APP_BTMAP_SMS_H__
#define __APP_BTMAP_SMS_H__

#include "bluetooth.h"
#include "btapp.h"

#ifdef __cplusplus
extern "C" {
#endif

void app_btmap_sms_init(void);
void app_btmap_sms_open(BT_DEVICE_ID_T id, bt_bdaddr_t *remote);
void app_btmap_sms_close(BT_DEVICE_ID_T id);
void app_btmap_sms_send(BT_DEVICE_ID_T id, char* telNum, char* msg);
bool app_btmap_check_is_connected(BT_DEVICE_ID_T id);
void app_btmap_sms_save(BT_DEVICE_ID_T id, char* telNum, char* msg);
void app_btmap_sms_resend(void);
bool app_btmap_check_is_idle(BT_DEVICE_ID_T id);


struct SMS_MSG_T{
    char* telNum;
    char* msg;
    uint8_t telNumLen;
    uint32_t msgLen;
};

#ifdef __cplusplus
}
#endif

#endif /*__APP_BTMAP_SMS_H__*/

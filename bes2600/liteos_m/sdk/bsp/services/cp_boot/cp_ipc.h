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
#ifndef __CP_IPC_H__
#define __CP_IPC_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CP_IPC_MSG_TRACE = 0,
#ifdef WIFI_ON_CP
    CP_IPC_MSG_NET_MGMT,
#else
    CP_IPC_MSG_NET_MGMT_AGENT,
    CP_IPC_MSG_NET_MGMT_SRV,
#endif
    CP_IPC_MSG_TEST,
    IPC_MSG_TYPE_NUM
}CP_IPC_MSG_TYPE;

typedef struct{
    uint16_t id;
    uint16_t len;
    void * data;
}CP_IPC_MSG_HDR;

typedef void (*CP_IPC_MSG_CB)(void *param);

int cp_ipc_init();
int cp_ipc_start();
int cp_ipc_send(CP_IPC_MSG_HDR *msg);
int cp_ipc_send_self(CP_IPC_MSG_HDR *msg);
int cp_ipc_cb_set(CP_IPC_MSG_TYPE type, CP_IPC_MSG_CB func, bool tx);

#ifdef __cplusplus
}
#endif
#endif

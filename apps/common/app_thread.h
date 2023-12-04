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
#ifndef __APP_THREAD_H__
#define __APP_THREAD_H__

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

#define APP_MAILBOX_MAX (50)

enum APP_MODUAL_ID_T {
    APP_MODUAL_KEY = 0,
    APP_MODUAL_AUDIO,
    APP_MODUAL_BATTERY,
    APP_MODUAL_BT,
    APP_MODUAL_FM,
    APP_MODUAL_SD,
    APP_MODUAL_LINEIN,
    APP_MODUAL_USBHOST,
    APP_MODUAL_USBDEVICE,
    APP_MODUAL_WATCHDOG,
    APP_MODUAL_AUDIO_MANAGE,
    APP_MODUAL_ANC,
    APP_MODUAL_VOICE_ASSIST,
    APP_MODUAL_SMART_MIC,
#ifdef __PC_CMD_UART__  
    APP_MODUAL_CMD,
#endif
#ifdef TILE_DATAPATH
    APP_MODUAL_TILE,
#endif
    APP_MODUAL_MIC,
#ifdef VOICE_DETECTOR_EN
    APP_MODUAL_VOICE_DETECTOR,
#endif
#ifdef AUDIO_HEARING_COMPSATN
    APP_MODUAL_HEAR_COMP,
#endif
    APP_MODUAL_OHTER,

    APP_MODUAL_NUM
};

typedef struct {
    uint32_t message_id;
    uint32_t message_ptr;
    uint32_t message_Param0;
    uint32_t message_Param1;
    uint32_t message_Param2;
} APP_MESSAGE_BODY;

typedef struct {
    uint32_t src_thread;
    uint32_t dest_thread;
    uint32_t system_time;
    uint32_t mod_id;
    APP_MESSAGE_BODY msg_body;
} APP_MESSAGE_BLOCK;

typedef int (*APP_MOD_HANDLER_T)(APP_MESSAGE_BODY *);

int app_mailbox_put(APP_MESSAGE_BLOCK* msg_src);

int app_mailbox_free(APP_MESSAGE_BLOCK* msg_p);

int app_mailbox_get(APP_MESSAGE_BLOCK** msg_p);

int app_os_init(void);

int app_set_threadhandle(enum APP_MODUAL_ID_T mod_id, APP_MOD_HANDLER_T handler);

void * app_os_tid_get(void);

bool app_is_module_registered(enum APP_MODUAL_ID_T mod_id);

#ifdef __cplusplus
    }
#endif

#endif//__FMDEC_H__

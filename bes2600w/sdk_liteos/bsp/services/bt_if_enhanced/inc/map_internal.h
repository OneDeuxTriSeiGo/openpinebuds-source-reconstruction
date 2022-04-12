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
#ifndef __MAP_INTERNAL_H__
#define __MAP_INTERNAL_H__

#ifdef BT_MAP_SUPPORT

#include "bluetooth.h"

#ifdef __cplusplus
extern "C" {
#endif

//------ STATE MACHINE ------//
typedef unsigned int btif_map_sm_event_t;
#define BTIF_MAP_SM_EVENT_GLOBAL_BASE 0x00000000
#define BTIF_MAP_SM_EVENT_STATE_ENTER (BTIF_MAP_SM_EVENT_GLOBAL_BASE+1)

typedef void (*btif_map_sm_event_handler_t)(void *instance, void *param);

typedef struct {
    union {
    } p;
} btif_map_sm_event_param_t;
typedef struct {
    btif_map_sm_event_t event;
    btif_map_sm_event_handler_t handler;
} btif_map_sm_state_handler_t;
typedef struct {
    btif_map_sm_state_handler_t *handlers;
} btif_map_sm_state_t;

//------ SMS ------//
#define BTIF_MAP_SM_EVENT_SMS_BASE 0x00000000

#ifdef __cplusplus
}
#endif

#endif /* BT_MAP_SUPPORT */

#endif /* __MAP_INTERNAL_H__ */

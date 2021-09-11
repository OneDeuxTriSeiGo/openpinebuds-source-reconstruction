/*
 * Copyright (c) 2021 bestechnic (Shanghai) Technologies CO., LIMITED.
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
#ifndef __HAL_IPC_RMT_TRACE_RECIVER_H__
#define __HAL_IPC_RMT_TRACE_RECIVER_H__
#include "plat_types.h"
#include "hal_ipc.h"
#include "hal_ipc_rmt_trace_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void hal_ipc_rmt_trace_reciver_open(enum HAL_IPC_CORE_TYPE_T core);

#ifdef __cplusplus
}
#endif

#endif

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
#ifndef __HAL_IPC_RMT_TRACE_TYPE_H__
#define __HAL_IPC_RMT_TRACE_TYPE_H__
#include "plat_types.h"
#include "hal_ipc.h"

#ifdef __cplusplus
extern "C" {
#endif

enum IPC_RMT_TRC_MSG_ID_T {
    IPC_RMT_TRC_MSG_ID_TRACE,
    IPC_RMT_TRC_MSG_ID_CRASH_ASSERT_START,
    IPC_RMT_TRC_MSG_ID_CRASH_FAULT_START,
    IPC_RMT_TRC_MSG_ID_CRASH_END,
};

struct IPC_RMT_TRC_CTRL_T {
    const uint8_t *buf_start;
    const uint16_t *entry_start;
    uint16_t buf_size;
    uint16_t entry_cnt;
    uint16_t entry_rpos;
    uint16_t entry_wpos;
    uint16_t discard_cnt_r;
    uint16_t discard_cnt_w;
    uint8_t seq_r;
    uint8_t seq_w;
};

struct IPC_RMT_TRC_MSG_T {
    enum IPC_RMT_TRC_MSG_ID_T id;
    void *param;
};


#ifdef __cplusplus
}
#endif

#endif

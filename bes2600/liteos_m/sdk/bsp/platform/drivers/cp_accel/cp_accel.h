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
#ifndef __CP_ACCEL_H__
#define __CP_ACCEL_H__

#include "plat_types.h"
#include "hal_location.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CP_MSG_NUM                    5

#define HAL_MEMSC_ID_CP                    HAL_MEMSC_ID_1

#define LOCK_CP_PROCESS() \
     do {} while (hal_memsc_lock(HAL_MEMSC_ID_CP) == 0)

#define UNLOCK_CP_PROCESS() \
     hal_memsc_unlock(HAL_MEMSC_ID_CP);

typedef unsigned int (*CP_ACCEL_CP_MAIN)(uint8_t event);

typedef unsigned int (*CP_ACCEL_EVT_HDLR)(uint8_t event);

/// Tasks types definition
enum CP_TASK_EVENT
{
    CP_EVENT_A2DP_DECODE = 0x00,

    CP_EVENT_SCO_PROCESSING = 0x00,

    CP_EVENT_AEC_PROCESSING = 0x0,
    CP_EVENT_AEC_RESAMPLING_PROCESSING = 0x1,

    CP_EVENT_HW_PROCESSING = 0x0,

    CP_EVENT_VOICE_ALGO_PROCESSING = 0x00,

    CP_EVENT_A2DP_ENCODE = 0x00,

    /// Maximum number of event
    CP_EVENT_MAX = 0x2,
};

enum MCU_TASK_EVENT
{
    MCU_EVENT_HW_DETECTED = 0,
    MCU_EVENT_HW_NEW_DATA_AVAILABLE = 1,
    MCU_EVENT_HW_UPSTREAM_DATA = 2,
    MCU_EVENT_HW_START_VAD = 3,
    MCU_EVENT_HW_CP_PROCESS_DONE = 4,

    MCU_EVENT_MAX,
};

/// Tasks types definition
enum CP_TASK_TYPE
{
    CP_TASK_A2DP_DECODE = 0x00,
    CP_TASK_SCO = 0x01,
    CP_TASK_AEC = 0x02,
    CP_TASK_HW = 0x03,
    CP_TASK_VOICE_ALGO = 0x04,
    CP_TASK_POWER_CONSUMPTION_MEASUREMENT = 0x05,
    CP_TASK_A2DP_ENCODE = 0x06,
    /// Maximum number of tasks
    CP_TASK_MAX,
};

typedef enum {
    CP_ACCEL_STATE_CLOSED = 0,
    CP_ACCEL_STATE_OPENING,
    CP_ACCEL_STATE_OPENED,
    CP_ACCEL_STATE_CLOSING,
} CP_ACCEL_STATE_E;

/// Task descriptor grouping all information required.
struct cp_task_desc
{
    CP_ACCEL_STATE_E cp_accel_state;
    CP_ACCEL_CP_MAIN cp_work_main;
    CP_ACCEL_EVT_HDLR cp_evt_hdlr;
    CP_ACCEL_EVT_HDLR mcu_evt_hdlr;
    CP_ACCEL_EVT_HDLR mcu_sys_ctrl_hdlr;
};
/// CP TASK environment structure
struct cp_task_env_tag
{
    struct cp_task_desc p_desc[CP_TASK_MAX];
};
/// cp environment definition
struct cp_env_tag
{
    uint8_t mcu2cp_tx_count;
    uint8_t mcu2cp_tx_pending[MAX_CP_MSG_NUM];
    bool cp_msg_recv;
    uint8_t cp_msg[CP_TASK_MAX][2];
};

struct CP_ACCEL_CPU_USAGE_T {
    uint8_t sleep;
    uint8_t busy;
};

/// Retrieves task type from event id.
#define CP_TASK_ID_GET(cp_event_id) (((cp_event_id) & 0xF0) >> 4)
/// Retrieves event from event id.
#define CP_EVENT_GET(cp_event_id) ((cp_event_id) & 0x0F)
/// Builds the task identifier from the type and the index of that task.
#define CP_BUILD_ID(id, event) ( (uint8_t)(((id) << 4)|(event)) )

//int cp_accel_open(CP_ACCEL_CP_MAIN cp_main, CP_ACCEL_EVT_HDLR cp_hdlr, CP_ACCEL_EVT_HDLR mcu_hdlr);
int cp_accel_open(enum CP_TASK_TYPE task_id, struct cp_task_desc const * p_task_desc);

int cp_accel_close(enum CP_TASK_TYPE task_id);

void cp_accel_init(void);

int cp_accel_init_done(void);

int cp_accel_send_event_mcu2cp(uint8_t event);

int cp_accel_send_event_cp2mcu(uint8_t event);

int cp_accel_get_cpu_usage(struct CP_ACCEL_CPU_USAGE_T *usage);

#ifdef __cplusplus
}
#endif

#endif


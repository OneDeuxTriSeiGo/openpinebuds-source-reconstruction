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
#ifndef __APP_RING_MERGE_H__
#define __APP_RING_MERGE_H__

enum APP_RING_MERGE_PLAY_T {
    APP_RING_MERGE_PLAY_ONESHOT = 0,
    APP_RING_MERGE_PLAY_PERIODIC,

    APP_RING_MERGE_PLAY_QTY =  0xff
};

#define PENDING_TO_STOP_A2DP_STREAMING  0
#define PENDING_TO_STOP_SCO_STREAMING   1

uint32_t app_ring_merge_more_data(uint8_t *buf, uint32_t len);

int app_ring_merge_setup(int16_t *buf, uint32_t len, enum APP_RING_MERGE_PLAY_T play);

int app_ring_merge_init(void);

int app_ring_merge_deinit(void);

int app_ring_merge_start(void);

int app_ring_merge_stop(void);

bool app_ring_merge_isrun(void);

void app_ring_merge_save_pending_start_stream_op(uint8_t pendingStopOp, uint8_t deviceId);


#endif


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
#ifndef __APP_A2DP_H__
#define __APP_A2DP_H__
#include "btapp.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define A2DP_NON_CODEC_TYPE_NON         0
#define A2DP_NON_CODEC_TYPE_LHDC        1
#define A2DP_NON_CODEC_TYPE_LDAC        2
#define A2DP_NON_CODEC_TYPE_SCALABLE    3

uint8_t a2dp_get_current_codec_type(uint8_t *elements);

bool a2dp_is_music_ongoing(void);
int a2dp_volume_set(enum BT_DEVICE_ID_T id, uint8_t vol);
#if defined(A2DP_LDAC_ON)
void app_ibrt_restore_ldac_info(uint8_t sample_freq);
#endif
#ifdef __cplusplus
}
#endif

#endif

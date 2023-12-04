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
#ifndef __A2DP_CODEC_LDAC_H__
#define __A2DP_CODEC_LDAC_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "avdtp_api.h"

#define A2DP_LDAC_OCTET_NUMBER                     (8)
#define A2DP_LDAC_VENDOR_ID                       0x0000012d
#define A2DP_LDAC_CODEC_ID                        0x00aa
//To indicate Sampling Rate.
#define A2DP_LDAC_SR_96000                        0x04
#define A2DP_LDAC_SR_88200                        0x08
#define A2DP_LDAC_SR_48000                        0x10
#define A2DP_LDAC_SR_44100                        0x20
#define A2DP_LDAC_SR_DATA(X)                      (X & (A2DP_LDAC_SR_96000 | A2DP_LDAC_SR_88200 | A2DP_LDAC_SR_48000 | A2DP_LDAC_SR_44100))
#define A2DP_LDAC_CM_DATA(X)                      (X & (A2DP_LDAC_CM_MONO | A2DP_LDAC_CM_DUAL | A2DP_LDAC_CM_STEREO))
//To indicate channel mode(a2dp)
#define A2DP_LDAC_CM_MONO                        0x04
#define A2DP_LDAC_CM_DUAL                        0x02
#define A2DP_LDAC_CM_STEREO                      0x01

#if defined(A2DP_LDAC_ON)
extern btif_avdtp_codec_t a2dp_ldac_avdtpcodec;
extern const unsigned char a2dp_codec_ldac_elements[A2DP_LDAC_OCTET_NUMBER];
bt_status_t a2dp_codec_ldac_init(int index);
void a2dp_codec_ldac_common_init(void);
#endif /* A2DP_LDAC_ON */

#if defined(__cplusplus)
}
#endif

#endif /* __A2DP_CODEC_LDAC_H__ */
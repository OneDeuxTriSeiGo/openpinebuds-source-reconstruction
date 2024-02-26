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
#ifndef __A2DP_CODEC_SCALABLE_H__
#define __A2DP_CODEC_SCALABLE_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "avdtp_api.h"

#define A2DP_SCALABLE_OCTET_NUMBER                (7)
#define A2DP_SCALABLE_VENDOR_ID                   0x00000075
#define A2DP_SCALABLE_CODEC_ID                    0x0103

//To indicate bits per sample.
#define A2DP_SCALABLE_HQ                          0x08
#define A2DP_SCALABLE_ADD_INFO1                   0x00
#define A2DP_SCALABLE_ADD_INFO2                   0x00
#define A2DP_SCALABLE_ADD_INFO3                   0x00
#define A2DP_SCALABLE_INFO(X)                     (X & (A2DP_SCALABLE_HQ | A2DP_SCALABLE_ADD_INFO1| A2DP_SCALABLE_ADD_INFO2 | A2DP_SCALABLE_ADD_INFO3))

//To indicate Sampling Rate.
#define A2DP_SCALABLE_SR_96000                    0x80
#define A2DP_SCALABLE_SR_32000                    0x40
#define A2DP_SCALABLE_SR_44100                    0x20
#define A2DP_SCALABLE_SR_48000                    0x10
#define A2DP_SCALABLE_SR_DATA(X)                  (X & (A2DP_SCALABLE_SR_96000 | A2DP_SCALABLE_SR_32000 | A2DP_SCALABLE_SR_44100 | A2DP_SCALABLE_SR_48000))

//To indicate bits per sample.
#define A2DP_SCALABLE_FMT_24                      0x08
#define A2DP_SCALABLE_FMT_16                      0x00
#define A2DP_SCALABLE_FMT_DATA(X)                 (X & (A2DP_SCALABLE_FMT_24 | A2DP_SCALABLE_FMT_16))

extern btif_avdtp_codec_t a2dp_scalable_avdtpcodec;
extern const unsigned char a2dp_codec_scalable_elements[];
bt_status_t a2dp_codec_scalable_init(int index);

#if defined(__cplusplus)
}
#endif

#endif /* __A2DP_CODEC_SCALABLE_H__ */

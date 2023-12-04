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
#ifndef __A2DP_CODEC_AAC_H__
#define __A2DP_CODEC_AAC_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "avdtp_api.h"
#include "tgt_hardware.h"

#ifndef MAX_AAC_BITRATE
#if defined(CHIP_BEST1400)
#define MAX_AAC_BITRATE (96000)
#else
#define MAX_AAC_BITRATE (264630)
#endif
#endif

#define A2DP_AAC_OCTET0_MPEG2_AAC_LC              0x80
#define A2DP_AAC_OCTET0_MPEG4_AAC_LC              0x40
#define A2DP_AAC_OCTET1_SAMPLING_FREQUENCY_44100  0x01
#define A2DP_AAC_OCTET2_SAMPLING_FREQUENCY_48000  0x80
#define A2DP_AAC_OCTET2_CHANNELS_1                0x08
#define A2DP_AAC_OCTET2_CHANNELS_2                0x04
#define A2DP_AAC_OCTET3_VBR_SUPPORTED             0x80

#define A2DP_AAC_OCTET_NUMBER                     (6)

#if defined(A2DP_AAC_ON)
extern btif_avdtp_codec_t a2dp_aac_avdtpcodec;
extern const unsigned char a2dp_codec_aac_elements[A2DP_AAC_OCTET_NUMBER];
bt_status_t a2dp_codec_aac_init(int index);
void a2dp_codec_aac_common_init(void);
#endif /* A2DP_AAC_ON */

#if defined(__cplusplus)
}
#endif

#endif /* __A2DP_CODEC_AAC_H__ */

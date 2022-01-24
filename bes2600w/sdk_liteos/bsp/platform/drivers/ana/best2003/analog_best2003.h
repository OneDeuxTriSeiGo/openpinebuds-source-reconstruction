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
#ifndef __ANALOG_BEST2003_H__
#define __ANALOG_BEST2003_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ISPI_ANA_REG(reg)                   (((reg) & 0xFFF) | 0x1000)

#define MAX_ANA_MIC_CH_NUM                  3

enum ANA_AUD_PLL_USER_T {
    ANA_AUD_PLL_USER_CODEC      = (1 << 0),
    ANA_AUD_PLL_USER_I2S        = (1 << 1),
    ANA_AUD_PLL_USER_SPDIF      = (1 << 2),
    ANA_AUD_PLL_USER_PCM        = (1 << 3),
    ANA_AUD_PLL_USER_IIR        = (1 << 4),
    ANA_AUD_PLL_USER_RS         = (1 << 5),

    ANA_AUD_PLL_USER_END        = (1 << 6),
};
#define ANA_AUD_PLL_USER_T                  ANA_AUD_PLL_USER_T

uint32_t analog_aud_get_max_dre_gain(void);

int analog_debug_config_vad_mic(bool enable);

void analog_aud_vad_enable(enum AUD_VAD_TYPE_T type, bool en);

void analog_aud_vad_adc_enable(bool en);

void analog_aud_codec_set_dev(int speaker_dev);

int analog_aud_codec_get_dev();

void analog_aud_codec_set_pa(int speaker_pa);

#ifdef __cplusplus
}
#endif

#endif


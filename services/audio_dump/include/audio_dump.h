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
#ifndef __AUDIO_DUMP_H__
#define __AUDIO_DUMP_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef AUDIO_DEBUG
#define AUDIO_DUMP
#endif

void audio_dump_init(int frame_len, int sample_bytes, int channel_num);
void audio_dump_clear_up(void);
void audio_dump_add_channel_data(int channel_id, void *pcm_buf, int pcm_len);
void audio_dump_run(void);

void data_dump_init(void);
void data_dump_run(const char *str, void *data_buf, uint32_t data_len);

#ifdef __cplusplus
}
#endif

#endif

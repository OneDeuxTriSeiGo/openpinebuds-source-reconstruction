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
#ifndef __AUDIO_PCM_PLAYER__
#define __AUDIO_PCM_PLAYER__

/* AUDIO_PCM_PLAYER is a pcm player based on app_bt_stream framework */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct audio_pcm_player_config {
    int bits;
    int sample_rate;
    int channel_num;
    int frame_length_samples;
    int volume;
} audio_pcm_player_config_t;

typedef enum audio_pcm_player_event {
    AUDIO_PCM_PLAYER_EVENT_MORE_DATA = 0,
} audio_pcm_player_event_t;

typedef struct audio_pcm_player_event_param {
    struct audio_pcm_player *player;
    void *player_data;
    union {
        struct {
            unsigned char *buff;
            unsigned int   buff_len;
        } more_data;
    } p;
} audio_pcm_player_event_param_t;

typedef int (*audio_pcm_player_callback_t)(audio_pcm_player_event_t event, audio_pcm_player_event_param_t *param);

typedef struct audio_pcm_player {
    void *player_data;
    audio_pcm_player_callback_t cb;
    audio_pcm_player_config_t config;
} audio_pcm_player_t;

// API
int audio_pcm_player_start(audio_pcm_player_t *player);
int audio_pcm_player_stop(audio_pcm_player_t *player);
int audio_pcm_player_close(audio_pcm_player_t *player);
int audio_pcm_player_setup(audio_pcm_player_t *player, audio_pcm_player_config_t *config);
int audio_pcm_player_open(audio_pcm_player_t *player, audio_pcm_player_callback_t cb, void *player_data);

// app_bt_stream layer
int audio_pcm_player_onoff(char onoff);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_PCM_PLAYER__ */
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

const int16_t RES_AUD_RING_SAMPRATE_8000 [] = {
#include "res/ring/SOUND_RING_8000.txt"
};
#ifdef __BT_WARNING_TONE_MERGE_INTO_STREAM_SBC__

const int16_t RES_AUD_RING_SAMPRATE_16000 [] = {
#include "res/ring/SOUND_RING_16000.txt"
};
const int16_t RES_AUD_RING_SAMPRATE_44100[] = {
#include "res/ring/SOUND_RING_44100.txt"
};

const int16_t RES_AUD_RING_SAMPRATE_48000 [] = {
#include "res/ring/SOUND_RING_48000.txt"
};
#endif

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
#ifndef __CODEC_BEST2300P_H__
#define __CODEC_BEST2300P_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*CODEC_ANC_BOOST_DELAY_FUNC)(uint32_t ms);

void codec_set_anc_boost_delay_func(CODEC_ANC_BOOST_DELAY_FUNC delay_func);

#ifdef __cplusplus
}
#endif

#endif

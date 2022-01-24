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
#ifndef __PLL_BEST2003_H__
#define __PLL_BEST2003_H__

#ifdef __cplusplus
extern "C" {
#endif

void ddr_pll_config();

void dsp_pll_config(uint64_t freq);

void bb_pll_config(uint64_t freq);

void dsi_pll_config(uint64_t freq);

#ifdef __cplusplus
}
#endif

#endif


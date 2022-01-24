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
#ifndef __HAL_WIFITSF_H__
#define __HAL_WIFITSF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define PAC_TIM_TIMER_CONTROL  (0x82C00E00 + 0x34)
#define PAC_TIM_TSF_TIMER_LOW  (0x82C00E00 + 0x38)
#define PAC_TIM_TSF_TIMER_HIGH (0x82C00E00 + 0x3C)


void hal_wifitsf_enable(void);
void hal_wifitsf_disable(void);
uint32_t hal_wifitsf_timer_get();
void hal_wifitsf_timer_set(uint32_t low, uint32_t high);
uint32_t hal_wifitsf_dma_time_get();
uint32_t hal_wifitsf_usb_time_get();
void hal_wifitsf_set_trigger_time(uint32_t val);
void hal_wifitsf_set_usb_tirg_shreshold(uint32_t val);

uint32_t hal_wifitsf_dma_time_get_high();
uint32_t hal_wifitsf_get_tsf_higt();
uint32_t hal_wifitsf_set_tsf_init_value(uint32_t low, uint32_t hight);

#ifdef __cplusplus
}
#endif

#endif

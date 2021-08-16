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
#ifndef __HAL_TIMER_FAST_IRQ_H__
#define __HAL_TIMER_FAST_IRQ_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_timer.h"

void hal_fast_timer_setup(enum HAL_TIMER_TYPE_T type, HAL_TIMER_IRQ_HANDLER_T handler);

void hal_fast_timer_start(uint32_t load);

void hal_fast_timer_stop(void);

void hal_fast_timer_pause(void);

void hal_fast_timer_continue(void);

int hal_fast_timer_is_enabled(void);

void hal_fast_timer_reload(uint32_t load);

uint32_t hal_fast_timer_get_raw_value(void);

int hal_fast_timer_irq_active(void);

int hal_fast_timer_irq_pending(void);

uint32_t hal_fast_timer_get_elapsed_time(void);

uint32_t hal_fast_timer_get_load(void);

uint32_t hal_fast_timer_get_count(void);

void hal_fast_timer_bgload(uint32_t load);

#ifdef __cplusplus
}
#endif

#endif

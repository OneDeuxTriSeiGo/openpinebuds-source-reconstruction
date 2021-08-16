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
#ifndef __APP_CUSTOM_API__
#define __APP_CUSTOM_API__
#include "bt_if.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef IBRT

/*
 * provide the basic interface for develop ibrt ui and add more interface in this file if which is
 * only requested by the special customer.
*/
void app_custom_ui_ctx_init();

void app_custom_ui_notify_bluetooth_enabled(void);

void app_custom_ui_notify_bluetooth_disabled(void);

void app_custom_ui_handler_vender_evevnt(uint8_t evt_type, uint8_t * buffer, uint32_t length);

#endif

#ifdef __cplusplus
}
#endif

#endif

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
#ifndef __HAL_TZ_TRIGGER_H__
#define __HAL_TZ_TRIGGER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"

typedef unsigned int (*HAL_TZ_TRIGGER_HANDLER)(void *param);
int hal_tz_trigger_open(HAL_TZ_TRIGGER_HANDLER rxhandler);

int hal_tz_trigger_close();

int hal_tz_trigger_set();

#ifdef __cplusplus
}
#endif

#endif



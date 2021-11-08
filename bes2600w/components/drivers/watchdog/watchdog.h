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
#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include "hal_wdt.h"
#include "watchdog_core.h"

#ifdef __cplusplus
extern "C" {
#endif

struct WatchdogResource {
    enum HAL_WDT_ID_T watchdog_id;
};

struct WatchdogDevice {
    struct WatchdogResource resource;
};

#ifdef __cplusplus
}
#endif
#endif
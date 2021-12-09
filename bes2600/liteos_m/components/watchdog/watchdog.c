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
#include "watchdog.h"

void watchdog_enable(void)
{
    return;
}

void watchdog_feed(void)
{
    return;
}

void watchdog_disable(void)
{
    return;
}

unsigned int watchdog_set_timeout(unsigned int timeout_ms)
{
    return -1;
}

void IoTWatchDogKick(void)
{
    watchdog_feed();
}
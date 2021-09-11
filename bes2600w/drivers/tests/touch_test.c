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
#include "platform.h"
#include "touch_if.h"

void touch_test()
{
    DevHandle dev = TouchOpen(0);
    DBG_ASSERT(dev != NULL);
    struct touch_msg msg;
    const char *touch_events[] = {"", "TOUCH_EVENT_UP", "TOUCH_EVENT_DOWN", "TOUCH_EVENT_MOVE", "TOUCH_EVENT_EXIST"};
    while (1) {
        memset(&msg, 0, sizeof(msg));
        if (TouchRead(dev, &msg, 100) == 0) {
            LOG_I("%s, x %d, y %d", touch_events[msg.event % 5], msg.x, msg.y);
        }
    }
    TouchClose(dev);
}
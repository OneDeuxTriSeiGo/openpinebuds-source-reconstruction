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
#include "touch_input.h"
#include "gfx_utils/graphic_log.h"

namespace OHOS
{
TouchInput *TouchInput::GetInstance()
{
    static TouchInput instance;
    if (!instance.init) {
        instance.handle = TouchOpen(0);
        if (!instance.handle) {
            GRAPHIC_LOGE("TouchOpen failed");
            return nullptr;
        }
        instance.init = true;
    }
    return &instance;
}

bool TouchInput::Read(DeviceData &data)
{
    struct touch_msg tmp = {0};
    // It shouldn't block here, so timeout = 0.
    if (TouchRead(this->handle, &tmp, 0) == 0) {
        this->msg = tmp;
        // GRAPHIC_LOGI("x %d, y %d, event %d", this->msg.x, this->msg.y, this->msg.event);
    }
    // may use last msg
    data.point.x = this->msg.x;
    data.point.y = this->msg.y;
    data.state = (this->msg.event == TOUCH_EVENT_DOWN || this->msg.event == TOUCH_EVENT_MOVE) ? STATE_PRESS : STATE_RELEASE;
    return false;
}
} // namespace OHOS

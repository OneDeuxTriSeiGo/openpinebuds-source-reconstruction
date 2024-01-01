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
#ifndef __XYZ_MODEM__
#define __XYZ_MODEM__

/*
 * start xmodem transfer
 * param: wait, connect timeout in seconds
 * return, 0 is ok
 *         other value is error;
 */
int xmodem_start_xfer(uint32_t wait);
int xmodem_stop_xfer(void);
int xmodem_send_stream(const uint8_t *data, size_t size, int ascii);

#endif /*__XYZ_MODEM__*/

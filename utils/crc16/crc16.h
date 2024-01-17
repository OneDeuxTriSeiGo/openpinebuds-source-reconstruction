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

#ifndef __CRC16_H
#define __CRC16_H

extern unsigned short const crc16_table[256];

#ifdef __cplusplus
extern "C" {
#endif

unsigned short _crc16(unsigned short crc, const unsigned char *buffer, unsigned int len);

#ifdef __cplusplus
}
#endif

static inline unsigned short crc16_byte(unsigned short crc, const unsigned char data)
{
    return (crc >> 8) ^ crc16_table[(crc ^ data) & 0xff];
}

#endif /* __CRC16_H */

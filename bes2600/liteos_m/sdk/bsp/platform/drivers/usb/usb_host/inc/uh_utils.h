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
#ifndef min
#define min(_a,_b) ((_a) < (_b) ? (_a) : (_b))
#endif

inline int LE16(const u8* d)
{
    return d[0] | (d[1] << 8);
}

inline u32 BE32_array(const u8* d)
{
    return (d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3];
}

inline void BE32(u32 n, u8* d)
{
    d[0] = (u8)(n >> 24);
    d[1] = (u8)(n >> 16);
    d[2] = (u8)(n >> 8);
    d[3] = (u8)n;
}

inline void BE16(u32 n, u8* d)
{
    d[0] = (u8)(n >> 8);
    d[1] = (u8)n;
}

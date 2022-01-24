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
#ifndef PING_H
#define PING_H

#include <stdint.h>
#include <netdb.h>

#ifdef __cplusplus
extern "C" {
#endif

void ping_init(const in_addr_t *ping_dst, uint32_t count, uint32_t delay_time, uint32_t len, uint32_t echo_level);

#ifdef __cplusplus
}
#endif

#endif /* PING_H */

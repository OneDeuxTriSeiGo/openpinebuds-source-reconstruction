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
#ifndef BESBT_STRING_H
#define BESBT_STRING_H

#include "stddef.h"
#include "stdint.h"
#include "plat_types.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t  memcpy_s(void *dst,size_t dstMax,const void *src, size_t srcMax);
size_t  memset_s(void *,size_t,int,size_t);

#ifdef __cplusplus
}
#endif


#endif /* BES_STRING_H */


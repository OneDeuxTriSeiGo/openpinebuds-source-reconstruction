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
#ifndef __NV_SECTION_DBG_H__
#define __NV_SECTION_DBG_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************header include********************************/
#include "hal_trace.h"

/******************************macro defination*****************************/
#define LOG_V(str, ...) TR_VERBOSE(TR_MOD(NV_SEC), str, ##__VA_ARGS__)
#define LOG_D(str, ...) TR_DEBUG(TR_MOD(NV_SEC), str, ##__VA_ARGS__)
#define LOG_I(str, ...) TR_INFO(TR_MOD(NV_SEC), str, ##__VA_ARGS__)
#define LOG_W(str, ...) TR_WARN(TR_MOD(NV_SEC), str, ##__VA_ARGS__)
#define LOG_E(str, ...) TR_ERROR(TR_MOD(NV_SEC), str, ##__VA_ARGS__)

/******************************type defination******************************/

/****************************function declearation**************************/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __NV_SECTION_DBG_H__ */
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
#ifndef __APP_BT_TRACE_H__
#define __APP_BT_TRACE_H__

#include "hal_trace.h"

#ifdef ENABLE_COMPRESS_LOG
#define TRACE_AUD_MGR_D(str, ...) hal_trace_dummy(str, ##__VA_ARGS__)
#define TRACE_AUD_MGR_I(str, ...) TR_INFO(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),     "[AUD][MGR]"str, ##__VA_ARGS__)
#define TRACE_AUD_MGR_W(str, ...) TR_WARN(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),     "[AUD][MGR][WARN]"str, ##__VA_ARGS__)
#define TRACE_AUD_MGR_E(str, ...) TR_ERROR(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),    "[AUD][MGR][ERR]"str, ##__VA_ARGS__)
#else
#define TRACE_AUD_MGR_D(str, ...) hal_trace_dummy(str, ##__VA_ARGS__)
#define TRACE_AUD_MGR_I(str, ...) TR_INFO(TR_MOD(AUDFLG),     str, ##__VA_ARGS__)
#define TRACE_AUD_MGR_W(str, ...) TR_WARN(TR_MOD(AUDFLG),     str, ##__VA_ARGS__)
#define TRACE_AUD_MGR_E(str, ...) TR_ERROR(TR_MOD(AUDFLG),    str, ##__VA_ARGS__)
#endif

#ifdef ENABLE_COMPRESS_LOG
#define TRACE_AUD_HDL_D(str, ...) hal_trace_dummy(str, ##__VA_ARGS__)
#define TRACE_AUD_HDL_I(str, ...) TR_INFO(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),     "[AUD][HDL]"str, ##__VA_ARGS__)
#define TRACE_AUD_HDL_W(str, ...) TR_WARN(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),     "[AUD][HDL][WARN]"str, ##__VA_ARGS__)
#define TRACE_AUD_HDL_E(str, ...) TR_ERROR(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),    "[AUD][HDL][ERR]"str, ##__VA_ARGS__)
#else
#define TRACE_AUD_HDL_D(str, ...) hal_trace_dummy(str, ##__VA_ARGS__)
#define TRACE_AUD_HDL_I(str, ...) TR_INFO(TR_MOD(AUDFLG),     str, ##__VA_ARGS__)
#define TRACE_AUD_HDL_W(str, ...) TR_WARN(TR_MOD(AUDFLG),     str, ##__VA_ARGS__)
#define TRACE_AUD_HDL_E(str, ...) TR_ERROR(TR_MOD(AUDFLG),    str, ##__VA_ARGS__)
#endif

#ifdef ENABLE_COMPRESS_LOG
#define TRACE_AUD_STREAM_D(str, ...) hal_trace_dummy(str, ##__VA_ARGS__)
#define TRACE_AUD_STREAM_I(str, ...) TR_INFO(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),     str, ##__VA_ARGS__)
#define TRACE_AUD_STREAM_W(str, ...) TR_WARN(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),     str, ##__VA_ARGS__)
#define TRACE_AUD_STREAM_E(str, ...) TR_ERROR(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),    str, ##__VA_ARGS__)
#else
#define TRACE_AUD_STREAM_D(str, ...) hal_trace_dummy(str, ##__VA_ARGS__)
#define TRACE_AUD_STREAM_I(str, ...) TR_INFO(TR_MOD(AUDFLG),     str, ##__VA_ARGS__)
#define TRACE_AUD_STREAM_W(str, ...) TR_WARN(TR_MOD(AUDFLG),     str, ##__VA_ARGS__)
#define TRACE_AUD_STREAM_E(str, ...) TR_ERROR(TR_MOD(AUDFLG),    str, ##__VA_ARGS__)
#endif

#ifdef ENABLE_COMPRESS_LOG
#define TRACE_MEDIA_PLAYER_D(str, ...) hal_trace_dummy(str, ##__VA_ARGS__)
#define TRACE_MEDIA_PLAYESTREAM_I(str, ...) TR_INFO(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),     str, ##__VA_ARGS__)
#define TRACE_MEDIA_PLAYESTREAM_W(str, ...) TR_WARN(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),     str, ##__VA_ARGS__)
#define TRACE_MEDIA_PLAYESTREAM_E(str, ...) TR_ERROR(TR_ATTR_ARG_NUM(COUNT_ARG_NUM(unused, ##__VA_ARGS__)),    str, ##__VA_ARGS__)
#else
#define TRACE_MEDIA_PLAYESTREAM_D(str, ...) hal_trace_dummy(str, ##__VA_ARGS__)
#define TRACE_MEDIA_PLAYESTREAM_I(str, ...) TR_INFO(TR_MOD(AUDFLG),     str, ##__VA_ARGS__)
#define TRACE_MEDIA_PLAYESTREAM_W(str, ...) TR_WARN(TR_MOD(AUDFLG),     str, ##__VA_ARGS__)
#define TRACE_MEDIA_PLAYESTREAM_E(str, ...) TR_ERROR(TR_MOD(AUDFLG),    str, ##__VA_ARGS__)
#endif

#endif

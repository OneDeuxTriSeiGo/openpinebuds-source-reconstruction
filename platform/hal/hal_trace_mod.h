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
#ifndef __HAL_TRACE_MOD_H__
#define __HAL_TRACE_MOD_H__

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_MOD(m)                          LOG_ATTR_MOD(LOG_MODULE_ ## m)

#define _LOG_MODULE_DEF_A(p, m)             p ## m
#define _LOG_MODULE_DEF(m)                  _LOG_MODULE_DEF_A(LOG_MODULE_, m)

#define _LOG_MODULE_LIST                    \
    _LOG_MODULE_DEF(NONE),                  \
    _LOG_MODULE_DEF(HAL),                   \
    _LOG_MODULE_DEF(DRVANA),                \
    _LOG_MODULE_DEF(DRVCODEC),              \
    _LOG_MODULE_DEF(DRVBT),                 \
    _LOG_MODULE_DEF(DRVFLS),                \
    _LOG_MODULE_DEF(DRVSEC),                \
    _LOG_MODULE_DEF(DRVUSB),                \
    _LOG_MODULE_DEF(AUDFLG),                \
    _LOG_MODULE_DEF(MAIN),                  \
    _LOG_MODULE_DEF(RT_OS),                 \
    _LOG_MODULE_DEF(BTPRF),                 \
    _LOG_MODULE_DEF(BLEPRF),                \
    _LOG_MODULE_DEF(BTAPP),                 \
    _LOG_MODULE_DEF(BLEAPP),                \
    _LOG_MODULE_DEF(TWSAPP),                \
    _LOG_MODULE_DEF(IBRTAPP),               \
    _LOG_MODULE_DEF(APPMAIN),               \
    _LOG_MODULE_DEF(APPTHREAD),             \
    _LOG_MODULE_DEF(PLAYER),                \
    _LOG_MODULE_DEF(TEST),                  \
    _LOG_MODULE_DEF(AUD),                   \
    _LOG_MODULE_DEF(OTA),                   \
    _LOG_MODULE_DEF(NV_SEC),                \
    _LOG_MODULE_DEF(AI_GVA),                \
    _LOG_MODULE_DEF(AI_AMA),                \
    _LOG_MODULE_DEF(AI_GMA),                \


enum LOG_MODULE_T {
    _LOG_MODULE_LIST

    // Should <= 128
    LOG_MODULE_QTY,
};

const char *hal_trace_get_log_module_desc(enum LOG_MODULE_T module);

#ifdef __cplusplus
}
#endif

#endif

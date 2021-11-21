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
#ifndef __APP_TRACE_RX_H__
#define __APP_TRACE_RX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "hal_trace.h"

#if defined(_AUTO_TEST_)
extern int auto_test_send(char *resp);
#define AUTO_TEST_SEND(str)         auto_test_send((char*)str)
#endif

typedef unsigned int (*APP_TRACE_RX_CALLBACK_T)(unsigned char *buf, unsigned int len);

void app_trace_rx_open(void);

int app_trace_rx_register(const char *name, APP_TRACE_RX_CALLBACK_T callback);

int app_trace_rx_deregister(const char *name);

#ifdef __cplusplus
}
#endif

#endif//__FMDEC_H__

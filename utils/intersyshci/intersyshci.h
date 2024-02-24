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
#ifndef __INTERSYSHCI_H__
#define __INTERSYSHCI_H__

#include "trans_adapt.h"

#if defined(__cplusplus)
extern "C" {
#endif

void BESHCI_BufferAvai(void *packet);
void BESHCI_Open(void);
void BESHCI_Close(void);
void BESHCI_Poll(void);
void BESHCI_LockBuffer(void);
void BESHCI_UNLockBuffer(void);
void BESHCI_SCO_Data_Start(void);
void BESHCI_SCO_Data_Stop(void);
void uartrxtx(void const *argument);

bool BESHCI_Controller_Log_Handler(const unsigned char *p_buff, uint32_t length);
void BESHCI_Dump_A2DP_Seq(const unsigned char *p_buff, uint32_t length);
typedef bool (*intersys_hci_cmd_filter_handler_func)(uint8_t* pbuf, uint32_t length);
void intersys_register_hci_cmd_filter_handler_callback(intersys_hci_cmd_filter_handler_func func);

#if defined(__cplusplus)
}
#endif

#endif /* __INTERSYSHCI_H__ */

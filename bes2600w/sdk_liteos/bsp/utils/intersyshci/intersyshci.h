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
#define INTERSYS_TX_SIGNAL (0x01)

#if defined(__cplusplus)
extern "C" {
#endif

bool BESHCI_Controller_Log_Handler(const unsigned char *p_buff, uint32_t length);
void BESHCI_Open(void);
void BESHCI_Close(void);
void BESHCI_LOCK_TX(void);
void BESHCI_UNLOCK_TX(void);
/***************************************************************************
 *
 * BES intersys RX data API
 * Please return the length of the data read each time
 *
 ****************************************************************************/
unsigned short bes_hci_receive_data( const uint8_t *buf, uint32_t size);

/***************************************************************************
 *
 * BES intersys TX data API
 *
 ****************************************************************************/
int bes_hci_send_data(uint8_t *buf, int size);

typedef bool (*intersys_hci_cmd_filter_handler_func)(uint8_t* pbuf, uint32_t length);
void intersys_register_hci_cmd_filter_handler_callback(intersys_hci_cmd_filter_handler_func func);

typedef void (*intersys_log_report_func)(uint8_t* data);
void intersys_register_log_report_handler_callback(intersys_log_report_func func);

void bes_get_hci_rx_flowctrl_info(void);
void bes_get_hci_tx_flowctrl_info(void);
#if defined(__cplusplus)
}
#endif

#endif /* __INTERSYSHCI_H__ */

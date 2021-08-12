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
#ifndef __HAL_MCU2CP_H__
#define __HAL_MCU2CP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"

enum HAL_MCU2CP_ID_T {
    HAL_MCU2CP_ID_0,
#if !defined(NUTTX_BUILD)
    HAL_MCU2CP_ID_1,
#endif
    HAL_MCU2CP_ID_QTY
};

typedef unsigned int (*HAL_MCU2CP_RX_IRQ_HANDLER)(const unsigned char *data, unsigned int len);
typedef void (*HAL_MCU2CP_TX_IRQ_HANDLER)(const unsigned char *data, unsigned int len);

int hal_mcu2cp_open_mcu(enum HAL_MCU2CP_ID_T id,
                        HAL_MCU2CP_RX_IRQ_HANDLER rxhandler, HAL_MCU2CP_TX_IRQ_HANDLER txhandler, bool rx_flowctrl);
int hal_mcu2cp_open_cp (enum HAL_MCU2CP_ID_T id,
                        HAL_MCU2CP_RX_IRQ_HANDLER rxhandler, HAL_MCU2CP_TX_IRQ_HANDLER txhandler, bool rx_flowctrl);

int hal_mcu2cp_close_mcu(enum HAL_MCU2CP_ID_T id);
int hal_mcu2cp_close_cp (enum HAL_MCU2CP_ID_T id);

int hal_mcu2cp_start_recv_mcu(enum HAL_MCU2CP_ID_T id);
int hal_mcu2cp_start_recv_cp (enum HAL_MCU2CP_ID_T id);

int hal_mcu2cp_stop_recv_mcu(enum HAL_MCU2CP_ID_T id);
int hal_mcu2cp_stop_recv_cp (enum HAL_MCU2CP_ID_T id);

int hal_mcu2cp_send_mcu(enum HAL_MCU2CP_ID_T id,
                    const unsigned char *data, unsigned int len);
int hal_mcu2cp_send_cp (enum HAL_MCU2CP_ID_T id,
                    const unsigned char *data, unsigned int len);

void hal_mcu2cp_rx_done_mcu(enum HAL_MCU2CP_ID_T id);
void hal_mcu2cp_rx_done_cp (enum HAL_MCU2CP_ID_T id);

int hal_mcu2cp_opened_mcu(enum HAL_MCU2CP_ID_T id);
int hal_mcu2cp_opened_cp(enum HAL_MCU2CP_ID_T id);

int hal_mcu2cp_local_irq_pending_mcu(enum HAL_MCU2CP_ID_T id);
int hal_mcu2cp_local_irq_pending_cp(enum HAL_MCU2CP_ID_T id);

void hal_mcu2cp_poll_tx_irq_mcu(enum HAL_MCU2CP_ID_T id);
void hal_mcu2cp_poll_tx_irq_cp(enum HAL_MCU2CP_ID_T id);
void hal_mcu2cp_irq_ack_mcu(enum HAL_MCU2CP_ID_T id);
void hal_mcu2cp_irq_ack_cp(enum HAL_MCU2CP_ID_T id);

const struct HAL_MCU2CP_MSG_T ** hal_mcu2cp_get_send_msg_list_cp(void);
const struct HAL_MCU2CP_MSG_T ** hal_mcu2cp_get_send_msg_list_mcu(void);

void hal_mcu2cp_set_send_msg_list_mcu(void);
void hal_mcu2cp_set_send_msg_list_cp(void);

#ifdef __cplusplus
}
#endif

#endif


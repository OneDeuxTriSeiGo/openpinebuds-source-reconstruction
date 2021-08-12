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
#ifndef __HAL_SYS2BTH_H__
#define __HAL_SYS2BTH_H__

#ifdef __cplusplus
extern "C" {
#endif

enum HAL_SYS2BTH_ID_T {
    HAL_SYS2BTH_ID_0,
    HAL_SYS2BTH_ID_1,

    HAL_SYS2BTH_ID_QTY
};

typedef unsigned int (*HAL_SYS2BTH_RX_IRQ_HANDLER)(const void *data, unsigned int len);

typedef void (*HAL_SYS2BTH_TX_IRQ_HANDLER)(const void *data, unsigned int len);

int hal_sys2bth_open(enum HAL_SYS2BTH_ID_T id, HAL_SYS2BTH_RX_IRQ_HANDLER rxhandler, HAL_SYS2BTH_TX_IRQ_HANDLER txhandler, int rx_flowctrl);

int hal_sys2bth_close(enum HAL_SYS2BTH_ID_T id);

int hal_sys2bth_start_recv(enum HAL_SYS2BTH_ID_T id);

int hal_sys2bth_stop_recv(enum HAL_SYS2BTH_ID_T id);

int hal_sys2bth_send_seq(enum HAL_SYS2BTH_ID_T id, const void *data, unsigned int len, unsigned int *seq);

int hal_sys2bth_send(enum HAL_SYS2BTH_ID_T id, const void *data, unsigned int len);

void hal_sys2bth_rx_done(enum HAL_SYS2BTH_ID_T id);

void hal_sys2bth_tx_irq_run(enum HAL_SYS2BTH_ID_T id);

int hal_sys2bth_tx_active(enum HAL_SYS2BTH_ID_T id, unsigned int seq);

int hal_sys2bth_opened(enum HAL_SYS2BTH_ID_T id);

int hal_sys2bth_rx_irq_pending(enum HAL_SYS2BTH_ID_T id);

#ifdef __cplusplus
}
#endif

#endif

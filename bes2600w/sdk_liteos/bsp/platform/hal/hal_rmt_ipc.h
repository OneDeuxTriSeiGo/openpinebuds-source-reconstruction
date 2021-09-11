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
#ifndef __HAL_RMT_IPC_H__
#define __HAL_RMT_IPC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis.h"
#include "hal_sleep.h"

#define HAL_RMT_IPC_MAX_SEND_REC_CNT            3

enum HAL_RMT_IPC_IRQ_TYPE_T {
    HAL_RMT_IPC_IRQ_SEND_IND,
    HAL_RMT_IPC_IRQ_RECV_DONE,

    HAL_RMT_IPC_IRQ_TYPE_QTY
};

typedef unsigned int (*HAL_RMT_IPC_RX_IRQ_HANDLER)(const void *data, unsigned int len);
typedef void (*HAL_RMT_IPC_TX_IRQ_HANDLER)(const void *data, unsigned int len);

typedef void (*HAL_RMT_IPC_IRQ_INIT)(uint32_t id);
typedef void (*HAL_RMT_IPC_PEER_TX_IRQ_SET)(uint32_t id);
typedef void (*HAL_RMT_IPC_LOCAL_TX_IRQ_CLEAR)(uint32_t id);
typedef void (*HAL_RMT_IPC_RX_IRQ_SUSPEND)(uint32_t id);
typedef void (*HAL_RMT_IPC_RX_IRQ_RESUME)(uint32_t id);
typedef void (*HAL_RMT_IPC_RX_DONE)(uint32_t id);
typedef int (*HAL_RMT_IPC_IRQ_ACTIVE)(uint32_t id, enum HAL_RMT_IPC_IRQ_TYPE_T type);
typedef void (*HAL_RMT_IPC_IRQ_ENTRY)(void);
typedef int (*HAL_RMT_IPC_PM_NOTIF_HANDLER)(enum HAL_PM_STATE_T state);

struct HAL_RMT_IPC_MSG_T {
    struct HAL_RMT_IPC_MSG_T *next;        // pointer to next element in the list
    unsigned int len;                       // message data length in bytes
    const void *data;                       // pointer to the message data
};

struct HAL_RMT_IPC_SEND_RECORD_T {
    struct HAL_RMT_IPC_MSG_T msg;
    bool in_use;
};

struct HAL_RMT_IPC_CH_CFG_T {
    struct HAL_RMT_IPC_MSG_T * send_msg_list_p;
    struct HAL_RMT_IPC_MSG_T * send_pending_list_p;
    struct HAL_RMT_IPC_SEND_RECORD_T send_msgs[HAL_RMT_IPC_MAX_SEND_REC_CNT];

    const struct HAL_RMT_IPC_MSG_T ** recv_msg_list_pp;
    struct HAL_RMT_IPC_MSG_T recv_pending_head;

    HAL_RMT_IPC_RX_IRQ_HANDLER rx_irq_handler;
    HAL_RMT_IPC_TX_IRQ_HANDLER tx_irq_handler;

    bool need_flow_ctrl;
    bool chan_busy;
    bool chan_opened;
};

struct HAL_RMT_IPC_CFG_T {
    const char * name;
    const struct HAL_RMT_IPC_CFG_T ** peer_cfg_pp;
    const struct HAL_RMT_IPC_CFG_T ** local_cfg_pp;

    HAL_RMT_IPC_IRQ_INIT irq_init;
    HAL_RMT_IPC_PEER_TX_IRQ_SET peer_tx_irq_set;
    HAL_RMT_IPC_LOCAL_TX_IRQ_CLEAR local_tx_irq_clear;
    HAL_RMT_IPC_RX_IRQ_SUSPEND rx_irq_suspend;
    HAL_RMT_IPC_RX_IRQ_RESUME rx_irq_resume;
    HAL_RMT_IPC_RX_DONE rx_done;
    HAL_RMT_IPC_IRQ_ACTIVE irq_active;
    HAL_RMT_IPC_IRQ_ENTRY rx_irq_entry;
    HAL_RMT_IPC_IRQ_ENTRY tx_irq_entry;
    HAL_RMT_IPC_PM_NOTIF_HANDLER pm_notif_handler;

    enum HAL_SYS_WAKE_LOCK_USER_T sys_wake_lock;
    uint8_t chan_num;
    struct HAL_RMT_IPC_CH_CFG_T * chan_cfg;
    const IRQn_Type * rx_irq_id;
    const IRQn_Type * tx_irq_id;
    bool * busy_p;
};

void hal_rmt_ipc_rx_irq_handler(const struct HAL_RMT_IPC_CFG_T *cfg);

void hal_rmt_ipc_tx_irq_handler(const struct HAL_RMT_IPC_CFG_T *cfg);

int hal_rmt_ipc_pm_notif_handler(const struct HAL_RMT_IPC_CFG_T *cfg, enum HAL_PM_STATE_T state);

int hal_rmt_ipc_open(const struct HAL_RMT_IPC_CFG_T *cfg, uint32_t id, HAL_RMT_IPC_RX_IRQ_HANDLER rxhandler, HAL_RMT_IPC_TX_IRQ_HANDLER txhandler, int rx_flowctrl);

int hal_rmt_ipc_close(const struct HAL_RMT_IPC_CFG_T *cfg, uint32_t id);

int hal_rmt_ipc_start_recv(const struct HAL_RMT_IPC_CFG_T *cfg, uint32_t id);

int hal_rmt_ipc_stop_recv(const struct HAL_RMT_IPC_CFG_T *cfg, uint32_t id);

int hal_rmt_ipc_send_seq(const struct HAL_RMT_IPC_CFG_T *cfg, uint32_t id, const void *data, unsigned int len, unsigned int *seq);

void hal_rmt_ipc_tx_irq_run(const struct HAL_RMT_IPC_CFG_T *cfg, uint32_t id);

int hal_rmt_ipc_tx_active(const struct HAL_RMT_IPC_CFG_T *cfg, uint32_t id, unsigned int seq);

int hal_rmt_ipc_opened(const struct HAL_RMT_IPC_CFG_T *cfg, uint32_t id);

#ifdef __cplusplus
}
#endif

#endif

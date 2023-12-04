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
#ifndef __HAL_TRANSQ_H__
#define __HAL_TRANSQ_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CHIP_HAS_TRANSQ

#include "plat_types.h"

enum HAL_TRANSQ_ID_T {
    HAL_TRANSQ_ID_0 = 0,
#if (CHIP_HAS_TRANSQ > 1)
    HAL_TRANSQ_ID_1,
#endif

    HAL_TRANSQ_ID_QTY
};

enum HAL_TRANSQ_PRI_T {
    HAL_TRANSQ_PRI_NORMAL = 0,
    HAL_TRANSQ_PRI_HIGH,

    HAL_TRANSQ_PRI_QTY
};

enum HAL_TRANSQ_RET_T {
    HAL_TRANSQ_RET_OK = 0,
    HAL_TRANSQ_RET_BAD_ID,
    HAL_TRANSQ_RET_BAD_PRI,
    HAL_TRANSQ_RET_BAD_CFG,
    HAL_TRANSQ_RET_BAD_SLOT,
    HAL_TRANSQ_RET_BAD_TX_NUM,
    HAL_TRANSQ_RET_BAD_RX_NUM,
    HAL_TRANSQ_RET_BAD_MODE,
    HAL_TRANSQ_RET_RX_EMPTY,
    HAL_TRANSQ_RET_TX_FULL,
};

typedef void (*HAL_TRANSQ_RX_IRQ_HANDLER)(enum HAL_TRANSQ_PRI_T pri);
typedef void (*HAL_TRANSQ_TX_IRQ_HANDLER)(enum HAL_TRANSQ_PRI_T pri, const uint8_t *data, uint32_t len);

struct HAL_TRANSQ_SLOT_NUM_T {
    uint8_t tx_num[HAL_TRANSQ_PRI_QTY];
    uint8_t rx_num[HAL_TRANSQ_PRI_QTY];
};

struct HAL_TRANSQ_CFG_T {
    struct HAL_TRANSQ_SLOT_NUM_T slot;
    uint8_t rx_irq_count;
    HAL_TRANSQ_RX_IRQ_HANDLER rx_handler;
    HAL_TRANSQ_TX_IRQ_HANDLER tx_handler;
};

enum HAL_TRANSQ_RET_T hal_transq_get_rx_status(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, bool *ready);

enum HAL_TRANSQ_RET_T hal_transq_get_tx_status(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, bool *done);

bool hal_transq_tx_busy(enum HAL_TRANSQ_ID_T id);

enum HAL_TRANSQ_RET_T hal_transq_rx_first(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, const uint8_t **data, uint32_t *len);

enum HAL_TRANSQ_RET_T hal_transq_rx_next(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, const uint8_t **data, uint32_t *len);

enum HAL_TRANSQ_RET_T hal_transq_tx(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, const uint8_t *data, uint32_t len);

enum HAL_TRANSQ_RET_T hal_transq_update_num(enum HAL_TRANSQ_ID_T id, const struct HAL_TRANSQ_SLOT_NUM_T *slot);

enum HAL_TRANSQ_RET_T hal_transq_open(enum HAL_TRANSQ_ID_T id, const struct HAL_TRANSQ_CFG_T *cfg);

enum HAL_TRANSQ_RET_T hal_transq_close(enum HAL_TRANSQ_ID_T id);

enum HAL_TRANSQ_RET_T hal_transq_flush(enum HAL_TRANSQ_ID_T id);

#if defined(__MCU_FW_2002__) && defined(CONFIG_SDIO_SUPPORT)
void hal_transq_previous_init(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri);
void hal_transq_clear_previous(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri, int data_toggle);
void hal_transq_previous_toggle(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri);
void hal_transq_int_enable(enum HAL_TRANSQ_ID_T id, enum HAL_TRANSQ_PRI_T pri);
#endif

uint8_t get_rx_irq_count(enum HAL_TRANSQ_ID_T id);

#ifdef __ARM_ARCH_ISA_ARM
#ifdef RTOS
void hal_transq_local_irq_handler(int irq_num, void *irq_data);
#endif
#endif
#endif // CHIP_HAS_TRANSQ

void hal_transq_local_irq_handler_body(enum HAL_TRANSQ_ID_T id);

#ifdef __cplusplus
}
#endif

#endif

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
#ifndef __HAL_SDIO_DEVICE_H__
#define __HAL_SDIO_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

#define SDIO_DEVICE_ADMA_MAX_TX_KB      64
#define SDIO_DEVICE_ADMA_DESC_NUM       (SDIO_DEVICE_ADMA_MAX_TX_KB + 2)  //Each link has a maximum of 1k and a maximum of 65 links,add an nop link
#define SDIO_DEVICE_ADMA_TXRX_LEN       (SDIO_DEVICE_ADMA_MAX_TX_KB * 1024)

enum HAL_SDIO_DEVICE_STATUS {
    HAL_SDIO_DEVICE_NOT_RDY = 0,
    HAL_SDIO_DEVICE_RDY,
    HAL_SDIO_DEVICE_IDLE,
    HAL_SDIO_DEVICE_BUSY,
    HAL_SDIO_DEVICE_TX,
    HAL_SDIO_DEVICE_RX,
};

struct HAL_SDIO_DEVICE_CB_T {
    void (*hal_sdio_device_enum_done)(enum HAL_SDIO_DEVICE_STATUS status);
    void (*hal_sdio_device_rxtx_start)(enum HAL_SDIO_DEVICE_STATUS status);
    void (*hal_sdio_device_send_done)(const uint8_t *buf, uint32_t buf_len);
    void (*hal_sdio_device_recv_done)(uint8_t *buf, uint32_t buf_len);
    void (*hal_sdio_device_int_from_host)(void);
#ifdef __MCU_FW_2002__
    void (*hal_sdio_device_pre_rxtx_start)(enum HAL_SDIO_DEVICE_STATUS status, int toggle);
#endif
};

//Public use
void hal_sdio_device_open(struct HAL_SDIO_DEVICE_CB_T *callback);
void hal_sdio_device_reopen(struct HAL_SDIO_DEVICE_CB_T *callback);
void hal_sdio_device_close(void);
enum HAL_SDIO_DEVICE_STATUS hal_sdio_device_send(const uint8_t *buf, uint32_t buf_len);
void hal_sdio_device_recv(uint8_t *buf, uint32_t buf_len, uint8_t buf_cnt);

//Under normal circumstances, do not call the following two functions
void hal_sdio_device_change_tplmanid_manf(uint16_t new_manf_id);
void hal_sdio_device_change_tplmanid_card(uint16_t new_card_id);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_SDIO_DEVICE_H__ */


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
#ifndef __HAL_CMD_H__
#define __HAL_CMD_H__

#include "stdint.h"

typedef enum {
    HAL_CMD_RX_START,
    HAL_CMD_RX_STOP,
    HAL_CMD_RX_DONE
} hal_cmd_rx_status_t;

typedef int (*hal_cmd_callback_t)(uint8_t *buf, uint32_t  len);
typedef int (*CMD_CALLBACK_HANDLER_T)(hal_cmd_rx_status_t status);

#ifdef __cplusplus
extern "C" {
#endif
int hal_cmd_init(void);
int hal_cmd_open(void);
int hal_cmd_close(void);

int hal_cmd_run(hal_cmd_rx_status_t status);
void hal_cmd_set_callback(CMD_CALLBACK_HANDLER_T handler);
int hal_cmd_register(const char *name, hal_cmd_callback_t callback);

int hal_cmd_list_process(const uint8_t *buf);
void hal_cmd_tx_process(uint8_t **ppbuf, uint16_t *plen);

#ifdef __cplusplus
}
#endif
// hal_cmd_t *hal_cmd_get_ptr(void);

#endif
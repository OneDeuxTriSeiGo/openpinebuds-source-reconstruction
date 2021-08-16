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
#ifndef __APP_UART_DMA_THREAD_H__
#define __APP_UART_DMA_THREAD_H__

#include <stdint.h>
#include <stddef.h>

typedef void (*app_uart_data_received_callback_handler_t)(uint8_t*, uint32_t);

#ifdef __cplusplus
extern "C" {
#endif
    void app_uart_init(void);
    void app_uart_register_data_receiver(app_uart_data_received_callback_handler_t func);    
    void app_uart_send_data(uint8_t* buf, uint32_t len);    
    bool app_uart_start_receiving_data(void);
    void app_uart_close(void);
    void app_uart_stop_uart_rx(void);
    
#ifdef __cplusplus
}
#endif

#endif//__APP_UART_DMA_THREAD_H__

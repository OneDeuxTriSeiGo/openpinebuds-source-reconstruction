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
#ifndef _INTERSYSHCI_PRIV_H__
#define _INTERSYSHCI_PRIV_H__

#if defined(__cplusplus)
extern "C" {
#endif

#define __rx_queue_support__ 1
/*
* use "g_intersys_trace_enable" to enable trace instead of _INTERSYS_DATA_TRACE_
* beshci_set_intersys_trace_enable() will releae to custom
*/
//#define _INTERSYS_DATA_TRACE_
#define _RX_DUMP_LEN_MAX (60)
#define _TX_DUMP_LEN_MAX (200)
#define BT_THREAD_BUFFER_RETRY_CNT (250)
#define ONE_CONTROLLER_LEN         (80)
#define BT_LOG_HCI_DATA_MAX_LEN    (256)

#define INTERSYSHCI_LOC __attribute__((section(".fast_text_sram")))

void beshci_set_intersys_trace_enable(void);
bool beshci_get_intersys_trace_enable(void);
void beshci_set_supv_hci_buff_trace_enable(void);
bool beshci_get_supv_hci_buff_trace_enable(void);
void beshci_set_supv_hci_buff_trace_enable_high(void);
bool beshci_get_supv_hci_buff_trace_enable_high(void);
void beshci_set_a2dp_stream_trace_enable(bool enable);
bool beshci_get_a2dp_stream_trace_enable(void);
void beshci_set_acl_packet_trace_enable(bool enable);
bool beshci_get_acl_packet_trace_enable(void);

bool beshci_is_packet_blocked_to_traceout(uint8_t *packet_buff, uint32_t buff_len);
void beshci_enable_sleep_checker(bool isEnable);
void intersys_sleep_checker_ping(void);
extern osThreadId intersys_tx_thread_id;

#if defined(__cplusplus)
}
#endif

#endif /* _INTERSYSHCI_PRIV_H__ */

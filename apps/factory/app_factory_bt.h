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
#ifndef __APP_FACTORY_BT_H__
#define __APP_FACTORY__BTH__

#include "app_key.h"

enum TEST_MODE_TIMER_TYEP_T{
   BT_NONSIGNALING_TX_TYPE = 0,
   BT_NONSIGNALING_RX_TYPE,
   LE_TEST_TX_TYPE,
   LE_TEST_RX_TYPE,
   TEST_ERROR_TYPE,
};

void app_factorymode_bt_create_connect(void);

void app_factorymode_bt_init_connect(void);

int app_factorymode_bt_xtalcalib_proc(void);

void app_factorymode_bt_xtalrangetest(APP_KEY_STATUS *status, void *param);

void app_factorymode_bt_signalingtest(APP_KEY_STATUS *status, void *param);

void app_factorymode_bt_nosignalingtest(APP_KEY_STATUS *status, void *param);

void app_factorymode_bt_xtalcalib(APP_KEY_STATUS *status, void *param);

void app_factory_enter_le_tx_test(const char* cmd, uint32_t cmd_len);

void app_factory_enter_le_rx_test(const char* cmd, uint32_t cmd_len);

void app_factory_enter_le_continueous_tx_test(const char* cmd, uint32_t cmd_len);

void app_factory_enter_le_continueous_rx_test(const char* cmd, uint32_t cmd_len);

void app_factory_remote_fetch_le_teset_result(void);

void app_factory_terminate_le_test(void);

void app_factory_enter_bt_38chanl_dh5_prbs9_tx_test(void);

void app_factory_enter_bt_38chanl_dh1_prbs9_rx_test(void);

void app_factory_remote_fetch_bt_nonsig_test_result(void);
void app_factory_enter_le_tx_test_v2(int timeout, int tx_channel, int data_len, int pkt_payload, int phy);
void app_factory_enter_le_rx_test_v2(int timeout, int remote_tx_nb, int rx_channel, int phy, int mod_idx);

void app_factorymode_btc_only_mode(APP_KEY_STATUS *status, void *param);

#endif

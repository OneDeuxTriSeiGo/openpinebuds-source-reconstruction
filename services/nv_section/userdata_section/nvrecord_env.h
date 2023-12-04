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
#if defined(NEW_NV_RECORD_ENABLED)

#ifndef NVRECORD_ENV_H
#define NVRECORD_ENV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "me_api.h"
#include "nvrecord_extension.h"

#define NVRAM_ENV_MEDIA_LANGUAGE_DEFAULT (0)
#define NVRAM_ENV_TWS_MODE_DEFAULT (0xff)
#define NVRAM_ENV_FACTORY_TESTER_STATUS_DEFAULT (0xaabbccdd)
#define NVRAM_ENV_FACTORY_TESTER_STATUS_TEST_PASS (0xffffaa55)

int nv_record_env_init(void);
int nv_record_env_get(struct nvrecord_env_t **nvrecord_env);
int nv_record_env_set(struct nvrecord_env_t *nvrecord_env);
void nv_record_update_ibrt_info(uint32_t newMode,bt_bdaddr_t *ibrtPeerAddr);
void nv_record_clean_tws_name(void);
void nvrecord_rebuild_system_env(struct nvrecord_env_t* pSystemEnv);
void nv_record_update_factory_tester_status(uint32_t status);
void nv_record_update_factory_le_rx_test_env(uint32_t to, uint16_t remote_tx_nb,uint8_t test_type,uint8_t rx_channel, uint8_t phy, uint8_t mod_idx);
void nv_record_update_factory_le_tx_test_env(uint32_t to, uint8_t test_type, uint8_t tx_channel, uint8_t data_len, uint8_t pkt_payload, uint8_t phy);
void nv_record_reset_factory_test_env(void);
void nv_record_update_factory_le_rx_test_result(bool test_done, uint16_t result);
void nv_record_factory_get_le_rx_test_result(bool* test_done, uint16_t* result);
void nv_record_update_factory_bt_nonsignaling_test_result(bool test_done, uint16_t result, uint16_t hec_nb,uint16_t crc_nb);
void nv_record_factory_get_bt_nonsignalint_rx_test_result(bool* test_done, uint16_t* result, uint16_t* hec_nb, uint16_t* crc_nb);
void nv_record_update_factory_bt_nonsignaling_env(uint32_t to, uint8_t test_type,
                                uint8_t hopping_mode, uint8_t whitening_mode, uint8_t tx_freq, uint8_t rx_freq, uint8_t power_level,
                                uint8_t lt_addr, uint8_t edr_enabled, uint8_t packet_type, uint8_t payload_pattern, uint16_t payload_length,
                                uint32_t tx_packet_num);

#ifdef __cplusplus
}
#endif
#endif
#endif // #if defined(NEW_NV_RECORD_ENABLED)

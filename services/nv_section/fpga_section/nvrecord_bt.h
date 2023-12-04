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
#if defined(NEW_NV_RECORD_ENALBED)

#ifndef NVRECORD_BT_H
#define NVRECORD_BT_H

#include "nvrecord_extension.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NVRAM_ENV_STREAM_VOLUME_A2DP_VOL_DEFAULT (AUDIO_OUTPUT_VOLUME_DEFAULT)
#define NVRAM_ENV_STREAM_VOLUME_HFP_VOL_DEFAULT (AUDIO_OUTPUT_VOLUME_DEFAULT)

void nv_record_btdevicerecord_set_a2dp_vol(nvrec_btdevicerecord* pRecord, int8_t vol);
void nv_record_btdevicerecord_set_hfp_vol(nvrec_btdevicerecord* pRecord, int8_t vol);
void nv_record_btdevicevolume_set_a2dp_vol(btdevice_volume* device_vol, int8_t vol);
void nv_record_btdevicevolume_set_hfp_vol(btdevice_volume* device_vol, int8_t vol);
void nv_record_btdevicerecord_set_a2dp_profile_active_state(btdevice_profile* device_plf, bool isActive);
void nv_record_btdevicerecord_set_hfp_profile_active_state(btdevice_profile* device_plf, bool isActive);
void nv_record_btdevicerecord_set_hsp_profile_active_state(btdevice_profile* device_plf, bool isActive);
int nv_record_enum_latest_two_paired_dev(btif_device_record_t* record1,btif_device_record_t* record2);
void nv_record_all_ddbrec_print(void);
void nv_record_update_runtime_userdata(void);
void nvrecord_rebuild_paired_bt_dev_info(NV_RECORD_PAIRED_BT_DEV_INFO_T* pPairedBtInfo);
int nv_record_btdevicerecord_find(const bt_bdaddr_t *bd_ddr, nvrec_btdevicerecord **record);
void nv_record_btdevicerecord_set_a2dp_profile_codec(btdevice_profile* device_plf, uint8_t a2dpCodec);
bt_status_t nv_record_ddbrec_delete(const bt_bdaddr_t *bdaddr);
bt_status_t nv_record_enum_dev_records(unsigned short index,btif_device_record_t* record);
bt_status_t nv_record_ddbrec_find(const bt_bdaddr_t *bd_ddr, btif_device_record_t*record);
bt_status_t nv_record_add(SECTIONS_ADP_ENUM type,void *record);
int nv_record_get_paired_dev_count(void);
void ram_record_ddbrec_init(void);
bt_status_t ram_record_ddbrec_find(const bt_bdaddr_t* bd_ddr, nvrec_btdevicerecord **record);
bt_status_t ram_record_ddbrec_add(const nvrec_btdevicerecord* param_rec);
bt_status_t ram_record_ddbrec_delete(const bt_bdaddr_t *bdaddr);
#ifdef __cplusplus
}
#endif
#endif
#endif // #if defined(NEW_NV_RECORD_ENALBED)


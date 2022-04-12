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
#ifndef BESBT_CFG_H
#define BESBT_CFG_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct besbt_cfg_t{
    uint16_t dip_vendor_id;
    uint16_t dip_product_id;
    uint16_t dip_product_version;
    bool apple_hf_at_support;
    bool hf_dont_support_cli_feature;
    bool hf_dont_support_enhanced_call;
    bool hf_support_hf_ind_feature;
    bool sniff;
    bool dont_auto_report_delay_report;
    bool vendor_codec_en;
    bool force_use_cvsd;
    bool support_enre_mode;
    bool bt_hid_cod_enable;
    bool bt_watch_enable;
    bool avdtp_cp_enable;
    bool bt_source_enable;
    bool bt_source_48k;
    bool bt_sink_enable;
    bool a2dp_sink_enable;
    bool lhdc_v3;
    bool hfp_ag_pts_enable;
    bool hfp_ag_pts_ecs_01;
    bool hfp_ag_pts_ecs_02;
    bool hfp_ag_pts_ecc;
    bool source_get_all_cap_flag;
    bool source_suspend_err_flag;
    bool source_unknown_cmd_flag;
    bool acl_tx_flow_debug;
    bool hci_tx_cmd_debug;
    bool dont_play_mute_when_a2dp_stuck;
    bool start_ibrt_reserve_buff;
    bool send_l2cap_echo_req;
    bool a2dp_play_stop_media_first;
    bool disc_acl_after_auth_key_missing;
    bool use_page_scan_repetition_mode_r1;
};
extern struct besbt_cfg_t besbt_cfg;

#ifdef __cplusplus
}
#endif
#endif /* BESBT_H */

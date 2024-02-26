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
#ifndef CO_LLCP_H_
#define CO_LLCP_H_

/**
 ****************************************************************************************
 * @addtogroup CO_BT
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "co_bt.h"
/*
 * DEFINES
 ****************************************************************************************
 */
#define LLCP_OPCODE_MASK      (0xFF)
#define LLCP_OPCODE_POS       (0)
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/// Control packet op_code
enum co_llcp_op_code
{
    LL_CONNECTION_UPDATE_IND_OPCODE = 0x00,
    LL_CHANNEL_MAP_IND_OPCODE       = 0x01,
    LL_TERMINATE_IND_OPCODE         = 0x02,
    LL_ENC_REQ_OPCODE               = 0x03,
    LL_ENC_RSP_OPCODE               = 0x04,
    LL_START_ENC_REQ_OPCODE         = 0x05,
    LL_START_ENC_RSP_OPCODE         = 0x06,
    LL_UNKNOWN_RSP_OPCODE           = 0x07,
    LL_FEATURE_REQ_OPCODE           = 0x08,
    LL_FEATURE_RSP_OPCODE           = 0x09,
    LL_PAUSE_ENC_REQ_OPCODE         = 0x0A,
    LL_PAUSE_ENC_RSP_OPCODE         = 0x0B,
    LL_VERSION_IND_OPCODE           = 0x0C,
    LL_REJECT_IND_OPCODE            = 0x0D,
    LL_SLAVE_FEATURE_REQ_OPCODE     = 0x0E,
    LL_CONNECTION_PARAM_REQ_OPCODE  = 0x0F,
    LL_CONNECTION_PARAM_RSP_OPCODE  = 0x10,
    LL_REJECT_EXT_IND_OPCODE        = 0x11,
    LL_PING_REQ_OPCODE              = 0x12,
    LL_PING_RSP_OPCODE              = 0x13,
    LL_LENGTH_REQ_OPCODE            = 0x14,
    LL_LENGTH_RSP_OPCODE            = 0x15,
    LL_PHY_REQ_OPCODE               = 0x16,
    LL_PHY_RSP_OPCODE               = 0x17,
    LL_PHY_UPDATE_IND_OPCODE        = 0x18,
    LL_MIN_USED_CHANNELS_IND_OPCODE = 0x19,
    /// Opcode length
    LL_OPCODE_MAX_OPCODE,

    LL_OPCODE_DEBUG = 0xFF,
};

/// LLCP PDU lengths (including op_code)
enum co_llcp_length
{
    LL_CONNECTION_UPDATE_IND_LEN     = 12,
    LL_CHANNEL_MAP_IND_LEN           = 8,
    LL_TERMINATE_IND_LEN             = 2,
    LL_ENC_REQ_LEN                   = 23,
    LL_ENC_RSP_LEN                   = 13,
    LL_START_ENC_REQ_LEN             = 1,
    LL_START_ENC_RSP_LEN             = 1,
    LL_UNKNOWN_RSP_LEN               = 2,
    LL_FEATURE_REQ_LEN               = 9,
    LL_FEATURE_RSP_LEN               = 9,
    LL_PAUSE_ENC_REQ_LEN             = 1,
    LL_PAUSE_ENC_RSP_LEN             = 1,
    LL_VERSION_IND_LEN               = 6,
    LL_REJECT_IND_LEN                = 2,
    LL_SLAVE_FEATURE_REQ_LEN         = 9,
    LL_REJECT_EXT_IND_LEN            = 3,
    LL_CONNECTION_PARAM_REQ_LEN      = 24,
    LL_CONNECTION_PARAM_RSP_LEN      = 24,
    LL_PING_REQ_LEN                  = 1,
    LL_PING_RSP_LEN                  = 1,
    LL_LENGTH_REQ_LEN                = 9,
    LL_LENGTH_RSP_LEN                = 9,
    LL_PHY_REQ_LEN                   = 3,
    LL_PHY_RSP_LEN                   = 3,
    LL_PHY_UPDATE_IND_LEN            = 5,
    LL_MIN_USED_CHANNELS_IND_LEN     = 3,
    LL_PDU_LENGTH_MAX                = 35,
};

/// PDU lengths
enum co_pdu_length
{
    PDU_SCAN_REQ_LEN          = 12,
    PDU_CON_REQ_LEN           = 34,
    PDU_CON_RSP_LEN           = 14,
};

/*
 * MESSAGES
 ****************************************************************************************
 */

/// LL_CONNECTION_UPDATE_IND structure.
struct  ll_connection_update_ind
{
    /// op_code
    uint8_t         op_code;
    /// window size (units of 1.25 ms)
    uint8_t         win_size;
    /// window offset (units of 1.25 ms)
    uint16_t        win_off;
    /// interval (units of 1.25 ms)
    uint16_t        interv;
    /// connection latency (unit of connection event)
    uint16_t        latency;
    /// link supervision timeout (unit of 10 ms)
    uint16_t        timeout;
    /// instant (unit of connection event)
    uint16_t        instant;
};

/// LL_CHANNEL_MAP_IND structure.
struct  ll_channel_map_ind
{
    /// op_code
    uint8_t            op_code;
    /// channel mapping
    struct le_chnl_map ch_map;
    /// instant
    uint16_t           instant;
};

/// LL_TERMINATE_IND structure.
struct  ll_terminate_ind
{
    /// op_code
    uint8_t         op_code;
    /// termination code
    uint8_t         err_code;
};

/// LL_ENC_REQ structure.
struct  ll_enc_req
{
    /// op_code
    uint8_t               op_code;
    /// random value
    struct rand_nb        rand;
    /// ediv
    uint16_t              ediv;
    /// skdm
    struct sess_k_div_x   skdm;
    /// ivm
    struct init_vect      ivm;
};

/// LL_ENC_RSP structure.
struct  ll_enc_rsp
{
    /// op_code
    uint8_t             op_code;
    /// skds
    struct sess_k_div_x   skds;
    /// ivs
    struct init_vect    ivs;
};

/// LL_START_ENC_REQ structure.
struct  ll_start_enc_req
{
    /// op_code
    uint8_t             op_code;
};

/// LL_START_ENC_RSP structure.
struct  ll_start_enc_rsp
{
    /// op_code
    uint8_t             op_code;
};

/// LL_UNKNOWN_RSP structure.
struct  ll_unknown_rsp
{
    /// op_code
    uint8_t         op_code;
    /// unknown type
    uint8_t         unk_type;
};

/// LL_FEATURE_REQ structure.
struct  ll_feature_req
{
    /// op_code
    uint8_t             op_code;
    /// le features
    struct le_features  feats;
};


/// LL_FEATURE_RSP structure.
struct  ll_feature_rsp
{
    /// op_code
    uint8_t             op_code;
    /// le features
    struct le_features  feats;
};

/// LL_PAUSE_ENC_REQ structure.
struct  ll_pause_enc_req
{
    /// op_code
    uint8_t             op_code;
};

/// LL_PAUSE_ENC_RSP structure.
struct  ll_pause_enc_rsp
{
    /// op_code
    uint8_t             op_code;
};

/// LL_VERSION_IND structure
struct ll_version_ind
{
    /// op_code
    uint8_t     op_code;
    /// version
    uint8_t     vers;
    /// company id
    uint16_t    compid;
    /// sub version
    uint16_t    subvers;
};

/// LL_REJECT_IND structure.
struct  ll_reject_ind
{
    /// op_code
    uint8_t         op_code;
    /// reject reason
    uint8_t         err_code;
};

/// LL_SLAVE_FEATURE_REQ structure.
struct  ll_slave_feature_req
{
    /// op_code
    uint8_t             op_code;
    /// le features
    struct le_features  feats;
};

/// LL_CONNECTION_PARAM_REQ structure.
struct  ll_connection_param_req
{
    /// op_code
    uint8_t         op_code;
    /// minimum value of connInterval (units of 1.25 ms)
    uint16_t        interval_min;
    /// maximum value of connInterval (units of 1.25 ms)
    uint16_t        interval_max;
    /// connSlaveLatency value (unit of connection event)
    uint16_t        latency;
    /// connSupervisionTimeout value (unit of 10 ms)
    uint16_t        timeout;
    /// preferred periodicity (units of 1.25 ms)
    uint8_t         pref_period;
    /// ReferenceConnEventCount (unit of connection event)
    uint16_t        ref_con_event_count;
    /// Offset0 (units of 1.25 ms)
    uint16_t        offset0;
    /// Offset1 (units of 1.25 ms)
    uint16_t        offset1;
    /// Offset2 (units of 1.25 ms)
    uint16_t        offset2;
    /// Offset3 (units of 1.25 ms)
    uint16_t        offset3;
    /// Offset4 (units of 1.25 ms)
    uint16_t        offset4;
    /// Offset5 (units of 1.25 ms)
    uint16_t        offset5;
};

/// LL_CONNECTION_PARAM_RSP structure.
struct  ll_connection_param_rsp
{
    /// op_code
    uint8_t         op_code;
    /// minimum value of connInterval (units of 1.25 ms)
    uint16_t        interval_min;
    /// maximum value of connInterval (units of 1.25 ms)
    uint16_t        interval_max;
    /// connSlaveLatency value (unit of connection event)
    uint16_t        latency;
    /// connSupervisionTimeout value (unit of 10 ms)
    uint16_t        timeout;
    /// preferred periodicity (units of 1.25 ms)
    uint8_t         pref_period;
    /// ReferenceConnEventCount (unit of connection event)
    uint16_t        ref_con_event_count;
    /// Offset0 (units of 1.25 ms)
    uint16_t        offset0;
    /// Offset1 (units of 1.25 ms)
    uint16_t        offset1;
    /// Offset2 (units of 1.25 ms)
    uint16_t        offset2;
    /// Offset3 (units of 1.25 ms)
    uint16_t        offset3;
    /// Offset4 (units of 1.25 ms)
    uint16_t        offset4;
    /// Offset5 (units of 1.25 ms)
    uint16_t        offset5;
};

/// LL_REJECT_EXT_IND structure.
struct  ll_reject_ext_ind
{
    /// op_code
    uint8_t         op_code;
    /// rejected op_code
    uint8_t         rej_op_code;
    /// reject reason
    uint8_t         err_code;
};

/// LL_PING_REQ structure.
struct  ll_ping_req
{
    /// op_code
    uint8_t         op_code;
};

/// LL_PING_RSP structure.
struct  ll_ping_rsp
{
    /// op_code
    uint8_t         op_code;
};

/// LL_LENGTH_REQ structure.
struct  ll_length_req
{
    /// op_code
    uint8_t     op_code;
    /// The max size in reception (unit of byte)
    uint16_t    max_rx_octets;
    /// The max time in reception (unit of microsecond)
    uint16_t    max_rx_time;
    /// The max size in transmission (unit of byte)
    uint16_t    max_tx_octets;
    /// The max time in transmission (unit of microsecond)
    uint16_t    max_tx_time;
};

/// LL_LENGTH_RSP structure.
struct  ll_length_rsp
{
    /// op_code
    uint8_t     op_code;
    /// The max size in reception (unit of byte)
    uint16_t    max_rx_octets;
    /// The max time in reception (unit of microsecond)
    uint16_t    max_rx_time;
    /// The max size in transmission (unit of byte)
    uint16_t    max_tx_octets;
    /// The max time in transmission (unit of microsecond)
    uint16_t    max_tx_time;
};
/// LL_PHY_REQ structure.
struct  ll_phy_req
{
    /// op_code
    uint8_t    op_code;
    /// Tx phy selection
    uint8_t    tx_phys;
    /// Rx phy selection
    uint8_t    rx_phys;
};

/// LL_PHY_RSP structure.
struct  ll_phy_rsp
{
    /// op_code
    uint8_t    op_code;
    /// Tx phy selection
    uint8_t    tx_phys;
    /// Rx phy selection
    uint8_t    rx_phys;
};

/// LL_PHY_UPDATE_IND structure.
struct  ll_phy_update_ind
{
    /// op_code
    uint8_t    op_code;
    /// master to slave phy selected
    uint8_t    m_to_s_phy;
    /// slave to master phy selected
    uint8_t    s_to_m_phy;
    /// Instant
    uint16_t   instant;
};

/// LL_MIN_USED_CHANNELS_IND structure.
struct  ll_min_used_channels_ind
{
    /// op_code
    uint8_t    op_code;
    /// PHY(s) for which the slave has a minimum number of used channels requirement
    uint8_t    phys;
    /// minimum number of channels to be used on the specified PHY
    uint8_t    min_used_ch;
};

/// LLCP pdu format
union llcp_pdu
{
    /// op_code
    uint8_t  op_code;

    struct ll_connection_update_ind     con_update_ind;
    struct ll_channel_map_ind           channel_map_ind;
    struct ll_terminate_ind             terminate_ind;
    struct ll_enc_req                   enc_req;
    struct ll_enc_rsp                   enc_rsp;
    struct ll_start_enc_req             start_enc_req;
    struct ll_start_enc_rsp             start_enc_rsp;
    struct ll_unknown_rsp               unknown_rsp;
    struct ll_feature_req               feats_req;
    struct ll_feature_rsp               feats_rsp;
    struct ll_pause_enc_req             pause_enc_req;
    struct ll_pause_enc_rsp             pause_enc_rsp;
    struct ll_version_ind               vers_ind;
    struct ll_reject_ind                reject_ind;
    struct ll_slave_feature_req         slave_feature_req;
    struct ll_connection_param_req      con_param_req;
    struct ll_connection_param_rsp      con_param_rsp;
    struct ll_reject_ext_ind            reject_ind_ext;
    struct ll_ping_req                  ping_req;
    struct ll_ping_rsp                  ping_rsp;
    struct ll_length_req                length_req;
    struct ll_length_rsp                length_rsp;
    struct ll_phy_req                   phy_req;
    struct ll_phy_rsp                   phy_rsp;
    struct ll_phy_update_ind            phy_upd_ind;
};

/// @} CO_BT
#endif // CO_LLCP_H_

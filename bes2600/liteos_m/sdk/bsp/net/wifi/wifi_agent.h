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
#ifndef __WIFI_AGENT_H__
#define __WIFI_AGENT_H__

#include "net_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CP_BOOT) && defined(WIFI_ON_CP) && defined(WAGT_SYNC_ENABLE)
#define WAGT_DATA	SRAM_DATA_LOC
#define WAGT_BSS	SRAM_BSS_LOC
extern uint8_t *wagt_sync_ptr;
#define WAGT_SYNC_BUF(buf) wagt_sync_ptr
#define WAGT_SYNC_BUF_OUT(buf, len) {memcpy(wagt_sync_ptr, (buf), (len));}
#define WAGT_SYNC_BUF_IN(buf, len) {memcpy((buf), wagt_sync_ptr, (len));}
#else
#define WAGT_DATA
#define WAGT_BSS
#define WAGT_SYNC_BUF(buf) (buf)
#define WAGT_SYNC_BUF_OUT(buf, len)
#define WAGT_SYNC_BUF_IN(buf, len)
#endif

#define IPC_DEBUG_ENABLE       (0)
#define IPC_DATA_SIZE          (72)
#define IPC_DATA_NUM           (64)
#define WIFI_DATAIN_NUM        (64)
#define WIFI_DATAOUT_NUM       (32)
#define WIFI_DATAOUT_SIZE      (SKB_TX_DATA_SIZE)
#define SEND2PEER_TRY_CNT      (100)
#define WIFI_HEARTBEAT_TIMEOUT (8000) // 8s
#define WIFI_HEARTBEAT_PERIOD  (2000) // 2s
/* elements of ipc_xxx should be the same as CP_IPC_MSG_HDR's */
typedef struct ipc_data {
    uint16_t ipc_id;
    uint16_t ipc_len;
    uint16_t used;
    uint16_t id;
    uint16_t len;
    uint8_t data[IPC_DATA_SIZE];
} IPC_DATA_T;

typedef enum {
    /* management message */
    NET_IPC_MSG_PARAS = 0,
    NET_IPC_MSG_NET_INIT,
    NET_IPC_MSG_EPTA_CONN,
    NET_IPC_MSG_EPTA_TTS,
    NET_IPC_MSG_EPTA_BT,
    NET_IPC_MSG_EPTA_PARAM,
    NET_IPC_MSG_SCAN,
    NET_IPC_MSG_CFG_SCAN,
    NET_IPC_MSG_GET_SCAN_RES,
    NET_IPC_MSG_FLUSH_SCAN_RES,
    NET_IPC_MSG_CONN,
    NET_IPC_MSG_DISC,
    NET_IPC_MSG_GET_SSID,
    NET_IPC_MSG_GET_BSSID,
    NET_IPC_MSG_GET_MAC,
    NET_IPC_MSG_GET_CHANNEL,
    NET_IPC_MSG_GET_RSSI,
    NET_IPC_MSG_GET_CURR_RATE,
    NET_IPC_MSG_GET_LINKINFO,
    NET_IPC_MSG_EN_NETWORK,
    NET_IPC_MSG_EN_NETWORK_ID,
    NET_IPC_MSG_ADD_NETWORK,
    NET_IPC_MSG_MODIFY_NETWORK,
    NET_IPC_MSG_CNT_CFG_SSID,
    NET_IPC_MSG_GET_CFG,
    NET_IPC_MSG_IFA_ADDR,
    NET_IPC_MSG_NV_ADDNET,
    NET_IPC_MSG_NV_GETNET,
    NET_IPC_MSG_NV_FIND_CFG,
    NET_IPC_MSG_DEL_ALLCFG,
    NET_IPC_MSG_DEL_CFG,
    NET_IPC_MSG_PS,
    NET_IPC_MSG_SW_CH,
    NET_IPC_MSG_STAT_EN,
    NET_IPC_MSG_STAT_GET,
    NET_IPC_MSG_STAT_CLR,
    NET_IPC_MSG_AP_UP,
    NET_IPC_MSG_AP_DOWN,
    NET_IPC_MSG_AP_CFG,
    NET_IPC_MSG_AP_SET_STA_NUM,
    NET_IPC_MSG_AP_SET_COUNTRY,
    NET_IPC_MSG_AP_ADD_VENDOR_IE,
    NET_IPC_MSG_AP_GET_STA_INFO,
    NET_IPC_MSG_WPS_WILD_OK,
    NET_IPC_MSG_WPS_PBC,
    NET_IPC_MSG_EVENT_NOTIFY,
    NET_IPC_MSG_SET_FIX_RATE,
    /* debug: not printf the ipc id */
    NET_IPC_MSG_HEARTBEAT,
    NET_IPC_MSG_SNIFF_STT,
    NET_IPC_MSG_SNIFF_STP,
    NET_IPC_MSG_SNIFF_FLT,
    NET_IPC_MSG_SNIFF_CH,
    NET_IPC_MSG_MAC_RECV,
    NET_IPC_MSG_SNIFF_RCV,

    /* data message */
    NET_IPC_MSG_MAC_XMIT,
    NET_IPC_MSG_SEND_MLME,
    NET_IPC_MSG_ALLOC_SKB,
#ifdef CSI_REPORT
    NET_IPC_MSG_CSI_INFO,
#endif
    /* last message */
    NET_IPC_MSG_NUM,
} NET_IPC_MSG_T;

typedef enum {
    INFO_ASYN = 0,
    INFO_SYN = 1,
} NET_IPC_SYN_T;

typedef enum {
    IPC_SEND_ERROR              = -1,
    IPC_STATUS_NOT_READY        = -50,
} IPC_STATUS_T;

typedef enum {
    WIFI_AGENT_UNINIT = 0,
    WIFI_AGENT_RESET,
    WIFI_AGENT_READY,
    WIFI_AGENT_NET_READY,
} WIFI_AGENT_STATUS_T;

struct wifi_data_o {
    uint32     in_use;
    uint16     devnum;
    uint16     data_len;
    uint8      *data;
    uint32     data_ptr;
    uint32     idx;
};

struct wifi_data_i {
    uint32     in_use;
    uint16     devnum;
    uint16     data_len;
    uint8      *data;
    uint8      *skb;
};

uint8 **wifi_agent_get_tx_buf(void);
int wifi_agent_send_tx_buf(uint8 devnum, uint8 **tx_buf, uint16 tx_len);
int wifi_agent_send2peer(int msg, int data_len, void *data, int sync);
int wifi_agent_init(void);
void wifi_agent_pre_reset(void);
void wifi_agent_reset(void);
void wifi_heartbeat_timer_restart(int time);
void wifi_heartbeat_timer_stop(void);
#ifdef __cplusplus
}
#endif

#endif

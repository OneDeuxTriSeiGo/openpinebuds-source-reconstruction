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
#ifndef APP_H_
#define APP_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration
#include "app_ble_mode_switch.h"
#include "app_ble_param_config.h"
#include "co_bt_defines.h"

#ifdef  BLE_APP_PRESENT

#include <stdint.h>          // Standard Integer Definition
#include <co_bt.h>           // Common BT Definitions
#include "arch.h"            // Platform Definitions
#include "gapc_msg.h"       // GAPC Definitions
#include "gapm_msg.h"
#include "gap.h"
#if (NVDS_SUPPORT)
#include "nvds.h"
#endif // (NVDS_SUPPORT)

#if defined(CFG_APP_SEC)
#if defined(CFG_SEC_CON)
#define BLE_AUTHENTICATION_LEVEL        GAP_AUTH_REQ_SEC_CON_BOND
#else
#define BLE_AUTHENTICATION_LEVEL        GAP_AUTH_REQ_MITM_BOND
#endif
#else
#define BLE_AUTHENTICATION_LEVEL        GAP_AUTH_REQ_NO_MITM_NO_BOND
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
/// Maximal length of the Device Name value
#define APP_DEVICE_NAME_MAX_LEN      (24)

// Advertising mode
#define APP_FAST_ADV_MODE   (1)
#define APP_SLOW_ADV_MODE   (2)
#define APP_STOP_ADV_MODE   (3)
#define APP_MAX_TX_OCTETS   251
#define APP_MAX_TX_TIME     2120

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef enum
{
    BLE_CONN_PARAM_MODE_DEFAULT = 0,
    BLE_CONN_PARAM_MODE_AI_STREAM_ON,
    BLE_CONN_PARAM_MODE_A2DP_ON,
    BLE_CONN_PARAM_MODE_HFP_ON,
    BLE_CONN_PARAM_MODE_OTA,
    BLE_CONN_PARAM_MODE_OTA_SLOWER,
    BLE_CONN_PARAM_MODE_SNOOP_EXCHANGE,
    BLE_CONN_PARAM_MODE_NUM,
} BLE_CONN_PARAM_MODE_E;

typedef enum
{
    BLE_CONN_PARAM_PRIORITY_NORMAL = 0,
    BLE_CONN_PARAM_PRIORITY_ABOVE_NORMAL0,
    BLE_CONN_PARAM_PRIORITY_ABOVE_NORMAL1,
    BLE_CONN_PARAM_PRIORITY_ABOVE_NORMAL2,
    BLE_CONN_PARAM_PRIORITY_HIGH,
} BLE_CONN_PARAM_PRIORITY_E;

typedef enum BLE_CONNECT_STATE {
    BLE_DISCONNECTED  = 0,
    BLE_DISCONNECTING = 1,
    BLE_CONNECTED     = 2,
}BLE_CONNECT_STATE_E;

typedef struct
{
    uint8_t     ble_conn_param_mode;
    uint8_t     priority;
    uint16_t    conn_param_interval;    // in the unit of 1.25ms
} BLE_CONN_PARAM_CONFIG_T;


/// Application environment structure
typedef struct {
    /// Connection handle
    uint16_t conhdl;
    BLE_CONNECT_STATE_E connectStatus;
    uint8_t isFeatureExchanged;
    /// Bonding status
    uint8_t bonded;
    uint8_t peerAddrType;
    uint8_t isBdAddrResolvingInProgress;
    uint8_t isGotSolvedBdAddr;
    uint8_t bdAddr[BD_ADDR_LEN];
    uint8_t solvedBdAddr[BD_ADDR_LEN];
    /// ble connection param record
    APP_BLE_CONN_PARAM_T connParam;
    /// ble connection pending param
    APP_BLE_CONN_PARAM_T connPendindParam;
    /// ble connection param update times
    uint8_t conn_param_update_times;

} APP_BLE_CONN_CONTEXT_T;

/// Application environment structure
struct app_env_tag
{
    uint8_t conn_cnt;
    /// Last initialized profile
    uint8_t next_svc;

    /// Device Name length
    uint8_t dev_name_len;
    /// Device Name
    uint8_t dev_name[APP_DEVICE_NAME_MAX_LEN];

    /// Local device IRK
    uint8_t loc_irk[KEY_LEN];

    APP_BLE_CONN_CONTEXT_T context[BLE_CONNECTION_MAX];
};



// max adv data length is 31, and 3 byte is used for adv type flag(0x01)
#define ADV_DATA_MAX_LEN                            (28)

/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */

/// Application environment
extern struct app_env_tag app_env;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize the BLE demo application.
 ****************************************************************************************
 */
void appm_init(void);

/**
 ****************************************************************************************
 * @brief Add a required service in the database
 ****************************************************************************************
 */
bool appm_add_svc(void);

/**
 ****************************************************************************************
 * @brief Put the device in general discoverable and connectable mode
 ****************************************************************************************
 */
void appm_start_advertising(void *param);

/**
 ****************************************************************************************
 * @brief Put the device in non discoverable and non connectable mode
 ****************************************************************************************
 */
void appm_stop_advertising(uint8_t actv_idx);

/**
 ****************************************************************************************
 * @brief Send to request to update the connection parameters
 ****************************************************************************************
 */
void appm_update_param(uint8_t conidx, struct gapc_conn_param *conn_param);

/**
 ****************************************************************************************
 * @brief Send a disconnection request
 ****************************************************************************************
 */
void appm_disconnect(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Retrieve device name
 *
 * @param[out] device name
 *
 * @return name length
 ****************************************************************************************
 */
uint8_t appm_get_dev_name(uint8_t* name);

/**
 ****************************************************************************************
 * @brief Return if the device is currently bonded
 ****************************************************************************************
 */
bool app_sec_get_bond_status(void);

/*---------------------------------------------------------------------------
 *            app_ble_connected_evt_handler
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    callback function of BLE connected event
 *
 * Parameters:
 *    conidx - connection index
 *    pPeerBdAddress - connected BLE device address
 *
 * Return:
 *    void
 */
void app_ble_connected_evt_handler(uint8_t conidx, gap_bdaddr_t *pPeerBdAddress);

/*---------------------------------------------------------------------------
 *            app_ble_disconnected_evt_handler
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    ble disconnect event received callback
 *
 * Parameters:
 *    conidx - connection index
 *
 * Return:
 *    void
 */
void app_ble_disconnected_evt_handler(uint8_t conidx, uint8_t errCode);

void l2cap_update_param(uint8_t  conidx,
                        uint32_t min_interval_in_ms,
                        uint32_t max_interval_in_ms,
                        uint32_t supervision_timeout_in_ms,
                        uint8_t  slaveLatency);

void appm_start_connecting(BLE_INIT_PARAM_T *init_param);

void appm_stop_connecting(void);

void appm_start_scanning(uint16_t intervalInMs, uint16_t windowInMs, uint32_t filtPolicy);

void appm_stop_scanning(void);

void appm_create_advertising(void);

void appm_create_connecting(void);

void app_advertising_stopped(uint8_t actv_idx);

void app_advertising_starting_failed(uint8_t actv_idx, uint8_t err_code);


void app_scanning_stopped(void);

void app_scanning_starting_failed(uint8_t actv_idx, uint8_t err_code);

void app_connecting_stopped(gap_bdaddr_t *peer_addr);

void app_connecting_failed(uint8_t actv_idx, uint8_t err_code);

void appm_exchange_mtu(uint8_t conidx);

void app_ble_system_ready(void);


void appm_set_private_bd_addr(uint8_t* bdAddr);

void appm_add_dev_into_whitelist(struct gap_bdaddr* ptBdAddr);

void app_scanning_started(void);

void app_advertising_started(uint8_t actv_idx);

void app_connecting_stopped(gap_bdaddr_t *peer_addr);

void app_connecting_started(void);

bool appm_resolve_random_ble_addr_from_nv(uint8_t conidx, uint8_t* randomAddr);

void appm_resolve_random_ble_addr_with_sepcific_irk(uint8_t conidx, uint8_t* randomAddr, uint8_t* pIrk);

void appm_random_ble_addr_solved(bool isSolvedSuccessfully, uint8_t* irkUsedForSolving);

uint8_t app_ble_connection_count(void);

bool app_is_arrive_at_max_ble_connections(void);

bool app_is_resolving_ble_bd_addr(void);

void app_enter_fastpairing_mode(void);

bool app_is_in_fastpairing_mode(void);

void app_set_in_fastpairing_mode_flag(bool isEnabled);

uint16_t appm_get_conhdl_from_conidx(uint8_t conidx);

void appm_check_and_resolve_ble_address(uint8_t conidx);

uint8_t* appm_get_current_ble_addr(void);

void app_trigger_ble_service_discovery(uint8_t conidx, uint16_t shl, uint16_t ehl);

uint8_t* appm_get_local_identity_ble_addr(void);

void app_exchange_remote_feature(uint8_t conidx);

void app_ble_update_conn_param_mode_of_specific_connection(uint8_t conidx, BLE_CONN_PARAM_MODE_E mode, bool isEnable);

void app_ble_reset_conn_param_mode_of_specifc_connection(uint8_t conidx);

void app_ble_update_conn_param_mode(BLE_CONN_PARAM_MODE_E mode, bool isEnable);

void app_ble_reset_conn_param_mode(uint8_t conidx);

void appm_refresh_ble_irk(void);

bool app_ble_get_conn_param(uint8_t conidx,  APP_BLE_CONN_PARAM_T* pConnParam);

void app_ble_update_param_successful(uint8_t conidx, APP_BLE_CONN_PARAM_T* pConnParam);
void app_ble_update_param_failed(uint8_t conidx, uint8_t errCode);

void appm_update_adv_data(void *param);

bool gattc_check_if_notification_processing_is_busy(uint8_t conidx);

#ifdef GFPS_ENABLED
void fp_update_ble_connect_param_start(uint8_t ble_conidx);
void fp_update_ble_connect_param_stop(uint8_t ble_conidx);
#endif



#ifdef __cplusplus
}
#endif

/// @} APP

#endif //(BLE_APP_PRESENT)

#endif // APP_H_

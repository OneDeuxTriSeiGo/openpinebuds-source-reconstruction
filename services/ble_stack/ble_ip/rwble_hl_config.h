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
#ifndef RWBLE_HL_CONFIG_H_
#define RWBLE_HL_CONFIG_H_

#include "rwprf_config.h"
#include "rwapp_config.h"

#define CFG_PRF
#define CFG_NB_PRF (BLE_APP_OTA + BLE_APP_GFPS  + BLE_VOICEPATH + BLE_BMS + \
    BLE_ANC_CLIENT + BLE_AMS_CLIENT + BLE_ANCS_PROXY + BLE_AMS_PROXY + \
    BLE_APP_DATAPATH_SERVER + \
    BLE_SMART_VOICE + \
    BLE_APP_TOTA + BLE_APP_TILE \
    )

/**
 ****************************************************************************************
 * @addtogroup ROOT
 * @{
 * @name BLE stack configuration
 * @{
 ****************************************************************************************
 */

#include "rwble_hl_error.h"

/*
 * DEFINES - Mandatory for BLE Host Layers
 ****************************************************************************************
 */
/// Maximum time to remain advertising when in the Limited
/// Discover able mode: TGAP(lim_adv_timeout)
/// required value: 180s: (18000 in 10 ms step)
#define GAP_TMR_LIM_ADV_TIMEOUT                             0x4650 //(18000)

/// Minimum time to perform scanning when performing
/// the General Discovery procedure on 1M PHY: TGAP(gen_disc_scan_min)
/// recommended value: 10.24s: (1024 in 10 ms step)
#define GAP_TMR_GEN_DISC_SCAN_1M                            0x0400 //(1024)

/// Minimum time to perform scanning when performing the
/// Limited Discovery procedure on 1M PHY: TGAP(lim_disc_scan_min)
/// recommended value: 10.24s: (1024 in 10 ms step)
#define GAP_TMR_LIM_DISC_SCAN_1M                            0x0400 //(1024)

/// TGAP(private_addr_int)
/// recommended value: 15 minutes
/// Minimum time interval between private address change
/// Minimum value 1s
#define GAP_TMR_PRIV_ADDR_MIN                             (0x0001)


/// L2CAP Signaling transaction Timer duration in milliseconds
/// TGAP(conn_param_timeout)
/// recommended value: 30 s: (30 000 ms)
#define GAP_SIG_TRANS_TIMEOUT_MS                          0x0BB8 //(30000)

/// SMP L2CAP transaction Timer duration in milliseconds  30 s: (30 000 ms)
#define GAP_SMP_TRANS_TIMEOUT_MS                          0x0BB8 //(30000)


/// Maximal authorized MTU value
#define GAP_LE_MTU_MAX                                     (2048)

/// Maximum GAP device name size
#define GAP_MAX_NAME_SIZE                                  (0x20)




/// 30 seconds transaction timer (30000 ms)
#define GATT_TRANSACTION_TIMEOUT                           (0x0BB8) //(30000)


/// Maximum attribute value length
#define GATT_MAX_VALUE                                     (GAP_LE_MTU_MAX)
/******************************************************************************************/
/* -------------------------   BLE PARTITIONING      -------------------------------------*/
/******************************************************************************************/


/******************************************************************************************/
/* --------------------------   INTERFACES        ----------------------------------------*/
/******************************************************************************************/


#if BLE_APP_PRESENT
#define APP_MAIN_TASK       TASK_APP
#elif AHI_TL_SUPPORT
#define APP_MAIN_TASK       TASK_AHI
#else
#define APP_MAIN_TASK       TASK_NONE
#endif // BLE_APP_PRESENT

// Host Controller Interface (Host side)
#define BLEHL_HCIH_ITF            HCIH_ITF

/******************************************************************************************/
/* --------------------------   COEX SETUP        ----------------------------------------*/
/******************************************************************************************/

///WLAN coex
#define BLEHL_WLAN_COEX          RW_WLAN_COEX
///WLAN test mode
#define BLEHL_WLAN_COEX_TEST     RW_WLAN_COEX_TEST

/******************************************************************************************/
/* --------------------------   HOST MODULES      ----------------------------------------*/
/******************************************************************************************/
#ifdef CFG_PRF
#define BLE_PROFILES      (1)
/// Number of Profile tasks managed by GAP manager.
#define BLE_NB_PROFILES   (CFG_NB_PRF)

#include "rwprf_config.h"
#else
#define BLE_PROFILES      (0)
#define BLE_NB_PROFILES   (0)
#endif // CFG_PRF

#define BLE_GAPM                    1
#if (BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_GAPC                    1
// must be equals to sizeof(gapc_con_t)
#define BLE_GAPC_HEAP_ENV_SIZE      (120 + KE_HEAP_MEM_RESERVED)
#else //(BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_GAPC                    0
#define BLE_GAPC_HEAP_ENV_SIZE      0
#endif //(BLE_CENTRAL || BLE_PERIPHERAL)

#if (BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_L2CAP                   (1)
#define BLE_GATT_CLI                8
#define BLE_GATT                    8
#else //(BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_L2CAP                   0
#define BLE_GATT                    0
#define BLE_GATT_CLI                0
#define BLE_GATT_HEAP_ENV_SIZE      0
#define BLE_L2CAP_HEAP_ENV_SIZE     0
#endif //(BLE_CENTRAL || BLE_PERIPHERAL)



/******************************************************************************************/
/* --------------------------   GATT              ----------------------------------------*/
/******************************************************************************************/


/// Size of the heap
#if (BLE_CENTRAL || BLE_PERIPHERAL)
    /// Can be tuned based on supported profiles
    #define BLEHL_HEAP_DB_SIZE                     (3072)

#define BLEHL_HEAP_MSG_SIZE                    (2048 + 256 * BLE_CONNECTION_MAX)
#else
    #define BLEHL_HEAP_MSG_SIZE                    (256)
    #define BLEHL_HEAP_DB_SIZE                     (0)
#endif /* #if (BLE_CENTRAL || BLE_PERIPHERAL) */




/// Number of BLE HL tasks
#define BLEHL_TASK_SIZE       BLE_HOST_TASK_SIZE + BLE_PRF_TASK_SIZE

/// Size of environment variable needed on BLE Host Stack for one link
#define BLEHL_HEAP_ENV_SIZE ( BLE_GAPC_HEAP_ENV_SIZE          \
                               + BLE_GATT_HEAP_ENV_SIZE       \
                               + BLE_L2CAP_HEAP_ENV_SIZE \
                             )









/// @} BLE stack configuration
/// @} ROOT

#endif // RWBLE_HL_CONFIG_H_

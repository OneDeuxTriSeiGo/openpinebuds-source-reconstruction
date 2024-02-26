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
#ifndef RWIP_CONFIG_H_
#define RWIP_CONFIG_H_

#include "rwapp_config.h"     // Audio Mode 0 configuration
#include "os_api.h"
#include "cmsis.h"
#include "besble_debug.h"
/**
 ****************************************************************************************
 * @addtogroup ROOT
 * @{
 *
 *  Information about RW SW IP options and flags
 *
 *        BT_DUAL_MODE             BT/BLE Dual Mode
 *        BT_STD_MODE              BT Only
 *        BLE_STD_MODE             BLE Only
 *
 *        BT_EMB_PRESENT           BT controller exists
 *        BLE_EMB_PRESENT          BLE controller exists
 *        BLE_HOST_PRESENT         BLE host exists
 *
 * @name RW Stack Configuration
 * @{
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */


#if defined(__IAG_BLE_INCLUDE__)
#ifndef FPGA
#define _BLE_NVDS_
#endif
#define CFG_BLE
#define CFG_HOST
#define CFG_APP


#define CFG_SEC_CON
#endif
#endif

#define GLOBAL_INT_DISABLE() do { uint32_t lock = int_lock();
#define GLOBAL_INT_RESTORE() int_unlock(lock); } while(0);

/******************************************************************************************/
/* -------------------------   STACK PARTITIONING      -----------------------------------*/
/******************************************************************************************/

#if defined(CFG_BT)
#define BT_EMB_PRESENT              1
#else // defined(CFG_BT)
#define BT_EMB_PRESENT              0
#endif // defined(CFG_BT)

#if defined(CFG_BLE) && defined(CFG_EMB)
#define BLE_EMB_PRESENT             1
#else // defined(CFG_BLE) && defined(CFG_EMB)
#define BLE_EMB_PRESENT             0
#endif // defined(CFG_BLE) && defined(CFG_EMB)

#if defined(CFG_BLE) && defined(CFG_HOST)
#define BLE_HOST_PRESENT            1
#else // defined(CFG_BLE) && defined(CFG_HOST)
#define BLE_HOST_PRESENT            0
#endif // defined(CFG_BLE) && defined(CFG_HOST)

#if defined(CFG_BLE) && defined(CFG_APP)
#define BLE_APP_PRESENT             1
#else // defined(CFG_BLE) && defined(CFG_APP)
#define BLE_APP_PRESENT             0
#endif // defined(CFG_BLE) && defined(CFG_APP)

#define BT_DUAL_MODE                (BT_EMB_PRESENT && BLE_EMB_PRESENT)
#define HCI_PRESENT                 1

/// Flag indicating that Mesh is supported
#if defined(CFG_BLE_MESH)
#define BLE_MESH      1
#else  // !defined(CFG_BLE_MESH)
#define BLE_MESH      0
#endif // defined(CFG_BLE_MESH)

/******************************************************************************************/
/* -------------------------   INTERFACES DEFINITIONS      -------------------------------*/
/******************************************************************************************/
#define CFG_AHITL
#define CFG_HCITL

/// Application Host Interface
#if defined(CFG_AHITL)
#define AHI_TL_SUPPORT       1
#else // defined(CFG_AHITL)
#define AHI_TL_SUPPORT       0
#endif // defined(CFG_AHITL)


/// Host Controller Interface Support (defines if HCI parser is present or not)
#if defined(CFG_HCITL)
#define HCI_TL_SUPPORT      1
#else //defined(CFG_HCITL)
#define HCI_TL_SUPPORT      0
#endif //defined(CFG_HCITL)


#if BLE_HOST_PRESENT
#define H4TL_SUPPORT      ((AHI_TL_SUPPORT) + (HCI_TL_SUPPORT))
#else // !BLE_HOST_PRESENT
#define H4TL_SUPPORT      (HCI_TL_SUPPORT)
#endif // BLE_HOST_PRESENT


/******************************************************************************************/
/* --------------------------   BLE COMMON DEFINITIONS      ------------------------------*/
/******************************************************************************************/
/// Kernel Heap memory sized reserved for allocate dynamically connection environment
#define KE_HEAP_MEM_RESERVED        (4)

#define CFG_ALLROLES
#define CFG_PERIPHERAL

#if defined(CFG_BLE)

/// Broadcaster
#if (defined(CFG_BROADCASTER) || defined(CFG_PERIPHERAL) || defined(CFG_ALLROLES))
#define BLE_BROADCASTER      1
#else
#define BLE_BROADCASTER      0
#endif // (defined(CFG_BROADCASTER) || defined(CFG_PERIPHERAL) || defined(CFG_ALLROLES))
/// Observer
#if (defined(CFG_OBSERVER) || defined(CFG_CENTRAL) || defined(CFG_ALLROLES))
#define BLE_OBSERVER      1
#else
#define BLE_OBSERVER      0
#endif // (defined(CFG_OBSERVER) || defined(CFG_CENTRAL) || defined(CFG_ALLROLES))
/// Central
#if (defined(CFG_CENTRAL) || defined(CFG_ALLROLES))
#define BLE_CENTRAL      1
#else
#define BLE_CENTRAL      0
#endif // (defined(CFG_CENTRAL) || defined(CFG_ALLROLES))
/// Peripheral
#if (defined(CFG_PERIPHERAL) || defined(CFG_ALLROLES))
#define BLE_PERIPHERAL      1
#else
#define BLE_PERIPHERAL      0
#endif // (defined(CFG_PERIPHERAL) || defined(CFG_ALLROLES))

#if ((BLE_BROADCASTER+BLE_OBSERVER+BLE_PERIPHERAL+BLE_CENTRAL) == 0)
    #error "No application role defined"
#endif // ((BLE_BROADCASTER+BLE_OBSERVER+BLE_PERIPHERAL+BLE_CENTRAL) == 0)

/// Maximum number of simultaneous BLE activities (scan, connection, advertising, initiating)
#define BLE_ACTIVITY_MAX          (BES_BLE_ACTIVITY_MAX)


/// Max advertising reports before sending the info to the host
#define BLE_ADV_REPORTS_MAX              (1)

#endif //defined(CFG_BLE)


/******************************************************************************************/
/* --------------------------   DISPLAY SETUP        -------------------------------------*/
/******************************************************************************************/

/// Display controller enable/disable
#if defined(CFG_DISPLAY)
#define DISPLAY_SUPPORT      1
#else
#define DISPLAY_SUPPORT      0
#endif //CFG_DISPLAY


/******************************************************************************************/
/* --------------------------      RTC SETUP         -------------------------------------*/
/******************************************************************************************/

/// RTC enable/disable
#if defined(CFG_RTC)
#define RTC_SUPPORT      1
#else
#define RTC_SUPPORT      0
#endif //CFG_RTC

/******************************************************************************************/
/* --------------------------      PS2 SETUP         -------------------------------------*/
/******************************************************************************************/

/// PS2 enable/disable
#if defined(CFG_PS2)
#define PS2_SUPPORT      1
#else
#define PS2_SUPPORT      0
#endif //CFG_PS2

/******************************************************************************************/
/* --------------------------      TRACER SETUP      -------------------------------------*/
/******************************************************************************************/

/// tracer enable/disable
#if defined(CFG_TRC_EN)
    #define TRACER_PRESENT                   1
    #include "dbg_trc_config.h"
#else
    #define TRACER_PRESENT                   0
#endif // CFG_TRC_EN

/******************************************************************************************/
/* -------------------------   DEEP SLEEP SETUP      -------------------------------------*/
/******************************************************************************************/

/// Use 32K Hz Clock if set to 1 else 32,768k is used
#define HZ32000                                     (GAIA_SUPPORT)

/// Time to wake-up Radio Module (in us)
#define SLEEP_RM_WAKEUP_DELAY                       625
/// Time for stabilization of the high frequency oscillator following a sleep-timer expiry (in us)
#define SLEEP_OSC_NORMAL_WAKEUP_DELAY               5000
/// Time for stabilization of the high frequency oscillator following an external wake-up request (in us)
#define SLEEP_OSC_EXT_WAKEUP_DELAY                  5000

/******************************************************************************************/
/* --------------------------   RADIO SETUP       ----------------------------------------*/
/******************************************************************************************/

/// Power control features
#define RF_TXPWR                            1
/// Class of device
#define RF_CLASS1                           0

/******************************************************************************************/
/* -------------------------   SUPPORTED RADIO PHY    ------------------------------------*/
/******************************************************************************************/

#if defined(CFG_RF_ATLAS)
#define BLE_PHY_1MBPS_SUPPORT                       1
#define BLE_PHY_2MBPS_SUPPORT                       1
#define BLE_PHY_CODED_SUPPORT                       1
#else // RIPPLE
#define BLE_PHY_1MBPS_SUPPORT                       1
#define BLE_PHY_2MBPS_SUPPORT                       0
#define BLE_PHY_CODED_SUPPORT                       1
#endif

/******************************************************************************************/
/* -------------------------   COEXISTENCE SETUP      ------------------------------------*/
/******************************************************************************************/

/// WLAN Coexistence
#if defined(CFG_WLAN_COEX)
    #define RW_WLAN_COEX                 1
    #define RW_WLAN_COEX_TEST            (defined(CFG_WLAN_COEX_TEST))
#else
    #define RW_WLAN_COEX                 0
    #define RW_WLAN_COEX_TEST            0
#endif // defined(CFG_WLAN_COEX)

/// MWS Coexistence
#if defined(CFG_MWS_COEX)
    #define RW_MWS_COEX                 1
    #define RW_MWS_COEX_TEST            (defined(CFG_MWS_COEX_TEST))
#else
    #define RW_MWS_COEX                 0
    #define RW_MWS_COEX_TEST            0
#endif // defined(CFG_MWS_COEX)



/******************************************************************************************/
/* --------------------   SECURE CONNECTIONS SETUP  --------------------------------------*/
/******************************************************************************************/
#define SECURE_CONNECTIONS        (1)
#if defined(CFG_ECC_16_BITS_ALGO)
#define ECC_MULT_ALGO_TYPE        (16)
#else // !defined(CFG_ECC_16_BITS_ALGO)
#define ECC_MULT_ALGO_TYPE        (32)
#endif // defined(CFG_ECC_16_BITS_ALGO)
#if defined(CFG_CRYPTO_UT)
#define CRYPTO_UT                 (1)
#else //defined(CFG_CRYPTO_UT)
#define CRYPTO_UT                 (0)
#endif //defined(CFG_CRYPTO_UT)

/******************************************************************************************/
/* --------------------------   DEBUG SETUP       ----------------------------------------*/
/******************************************************************************************/

/// Flag indicating if debug mode is activated or not
#if defined(CFG_DBG)
    #define RW_DEBUG                        ((BLE_EMB_PRESENT) || (BT_EMB_PRESENT) || (BLE_HOST_PRESENT))
#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    #define RW_SWDIAG                       1
#else
    #define RW_SWDIAG                       0
#endif
    #define KE_PROFILING                    1
#else
    #define RW_DEBUG                        0
    #define RW_SWDIAG                       0
    #define KE_PROFILING                    0
#endif /* CFG_DBG */

/// Flag indicating if Read/Write memory commands are supported or not
#if defined(CFG_DBG_MEM)
    #define RW_DEBUG_MEM               1
#else //CFG_DBG_MEM
    #define RW_DEBUG_MEM               0
#endif //CFG_DBG_MEM

/// Flag indicating if Flash debug commands are supported or not
#if defined(CFG_DBG_FLASH)
    #define RW_DEBUG_FLASH                  1
#else //CFG_DBG_FLASH
    #define RW_DEBUG_FLASH                  0
#endif //CFG_DBG_FLASH

/// Flag indicating if CPU stack profiling commands are supported or not
#if defined(CFG_DBG_STACK_PROF)
    #define RW_DEBUG_STACK_PROF             1
#else
    #define RW_DEBUG_STACK_PROF             0
#endif // defined (CFG_DBG_STACK_PROF)


/******************************************************************************************/
/* --------------------------      NVDS SETUP       --------------------------------------*/
/******************************************************************************************/

/// Flag indicating if NVDS feature is supported or not
#if defined(CFG_NVDS)
    #define NVDS_SUPPORT                    1
#else //CFG_DBG_NVDS
    #define NVDS_SUPPORT                    0
#endif //CFG_DBG_NVDS

/******************************************************************************************/
/* --------------------------      MISC SETUP       --------------------------------------*/
/******************************************************************************************/
/// Manufacturer: RivieraWaves SAS
#define RW_COMP_ID                           0x0060


/******************************************************************************************/
/* -------------------------   BT / BLE / BLE HL CONFIG    -------------------------------*/
/******************************************************************************************/

#if (BT_EMB_PRESENT)
#include "rwbt_config.h"    // bt stack configuration
#endif //BT_EMB_PRESENT

#if (BLE_EMB_PRESENT)
#include "rwble_config.h"   // ble stack configuration
#endif //BLE_EMB_PRESENT

#if (BLE_HOST_PRESENT)
#include "rwble_hl_config.h"  // ble Host stack configuration
#endif //BLE_HOST_PRESENT

#if defined(CFG_APP)
#include "rwapp_config.h"     // Application configuration
#endif // defined(CFG_APP)

#define BLE_INVALID_CONNECTION_INDEX    0xFF



#define SV_HIGH_SPEED_BLE_CONNECTION_INTERVAL_MIN_IN_MS        80
#define SV_HIGH_SPEED_BLE_CONNECTION_INTERVAL_MAX_IN_MS        400
#define SV_HIGH_SPEED_BLE_CONNECTION_SUPERVISOR_TIMEOUT_IN_MS    20000

#define SV_LOW_SPEED_BLE_CONNECTION_INTERVAL_MIN_IN_MS            400
#define SV_LOW_SPEED_BLE_CONNECTION_INTERVAL_MAX_IN_MS            1000
#define SV_LOW_SPEED_BLE_CONNECTION_SUPERVISOR_TIMEOUT_IN_MS    20000


/******************************************************************************************/
/* -------------------------   KERNEL SETUP          -------------------------------------*/
/******************************************************************************************/

/// Event types definition
enum KE_EVENT_TYPE
{
    KE_EVENT_KE_TIMER,
    #if (TRACER_PRESENT)
    KE_EVENT_TRC,
    #endif /*(TRACER_PRESENT)*/

    KE_EVENT_KE_MESSAGE,


    KE_EVENT_MAX,
};

/// Tasks types definition
enum KE_TASK_TYPE
{
#if (BLE_APP_PRESENT)
    TASK_APP,
#endif // (BLE_APP_PRESENT)

#if (BLE_HOST_PRESENT)
    TASK_L2CAP,   // L2CAP Task
    TASK_GATT,    // Generic Attribute Profile
    TASK_GAPM,    // Generic Access Profile Manager
    TASK_GAPC,    // Generic Access Profile Controller

    // allocate a certain number of profiles task
    TASK_PRF_MAX = (TASK_GAPC + BLE_NB_PROFILES),

#endif // (BLE_HOST_PRESENT)

    #if (AHI_TL_SUPPORT)
    TASK_AHI,
    #endif // (AHI_TL_SUPPORT)

    /// Maximum number of tasks
    TASK_MAX,

    TASK_NONE = 0xFF,
};

/// Kernel memory heaps types.
enum KE_MEM_HEAP
{
    /// Memory allocated for environment variables
    KE_MEM_ENV,
    #if (BLE_HOST_PRESENT)
    /// Memory allocated for Attribute database
    KE_MEM_ATT_DB,
    #endif // (BLE_HOST_PRESENT)
    /// Memory allocated for kernel messages
    KE_MEM_KE_MSG,
    /// Non Retention memory block
    KE_MEM_NON_RETENTION,
    KE_MEM_BLOCK_MAX,
};



#if (BT_EMB_PRESENT)
#define BT_HEAP_MSG_SIZE_      BT_HEAP_MSG_SIZE
#define BT_HEAP_ENV_SIZE_      BT_HEAP_ENV_SIZE
#else
#define BT_HEAP_MSG_SIZE_      0
#define BT_HEAP_ENV_SIZE_      0
#endif //BT_EMB_PRESENT

#if (BLE_EMB_PRESENT)
#define BLE_HEAP_MSG_SIZE_     BLE_HEAP_MSG_SIZE
#define BLE_HEAP_ENV_SIZE_     BLE_HEAP_ENV_SIZE
#else
#define BLE_HEAP_MSG_SIZE_     0
#define BLE_HEAP_ENV_SIZE_     0
#endif //BLE_EMB_PRESENT

#if (BLE_HOST_PRESENT)
#define BLEHL_HEAP_MSG_SIZE_   BLEHL_HEAP_MSG_SIZE
#define BLEHL_HEAP_ENV_SIZE_   BLEHL_HEAP_ENV_SIZE
#define BLEHL_HEAP_DB_SIZE_    BLEHL_HEAP_DB_SIZE
#else
#define BLEHL_HEAP_MSG_SIZE_   0
#define BLEHL_HEAP_ENV_SIZE_   0
#define BLEHL_HEAP_DB_SIZE_    0
#endif //BLE_HOST_PRESENT

#if (BT_EMB_PRESENT || BLE_EMB_PRESENT)
#define ECC_HEAP_NON_RET_SIZE_   (328*2) // Could only have 2 ECC computations simultaneously
#else // (BT_EMB_PRESENT || BLE_EMB_PRESENT)
#define ECC_HEAP_NON_RET_SIZE_   (0)
#endif // (BT_EMB_PRESENT || BLE_EMB_PRESENT)

/// Kernel Message Heap
#define RWIP_HEAP_MSG_SIZE         (  BT_HEAP_MSG_SIZE_      + \
                                      BLE_HEAP_MSG_SIZE_     + \
                                      BLEHL_HEAP_MSG_SIZE_      )


/// Size of Environment heap
#define RWIP_HEAP_ENV_SIZE         ( BT_HEAP_ENV_SIZE_       + \
                                     BLE_HEAP_ENV_SIZE_      + \
                                     BLEHL_HEAP_ENV_SIZE_       )


/// Size of Attribute database heap
#define RWIP_HEAP_DB_SIZE          (  BLEHL_HEAP_DB_SIZE_  )

/**
 * Size of non-retention heap
 *
 * This heap can be used to split the RAM into 2 parts:
 *    - an always-on part that can handle a certain number of links
 *    - a secondary memory that could be powered-off when not used, and retained only when used
 *
 * With such mechanism, the previous heaps need to be reduced so that they can contain all required data
 * in a light scenario (few connections, few profiles). Then the non-retention heap is sized in order to
 * cover the worst case scenario (max connections, max profiles, etc ...)
 *
 * The current size show what is already known as not needing to be retained during deep sleep.
 */
#define RWIP_HEAP_NON_RET_SIZE    ( ECC_HEAP_NON_RET_SIZE_ )

/// Minimum sleep time to enter in deep sleep (in half slot).
#define RWIP_MINIMUM_SLEEP_TIME                (1)

/******************************************************************************************/
/* -------------------------     CONFIGURABLE PARAMETERS     -----------------------------*/
/******************************************************************************************/

/// List of parameters identifiers
enum PARAM_ID
{
    /// Definition of the tag associated to each parameters
    /// Local Bd Address
    PARAM_ID_BD_ADDRESS                 = 0x01,
    /// Device Name
    PARAM_ID_DEVICE_NAME                = 0x02,
    /// Low Power Clock Drift
    PARAM_ID_LPCLK_DRIFT                = 0x07,
    /// Low Power Clock Jitter
    PARAM_ID_LPCLK_JITTER               = 0x08,
    /// Active Clock Drift
    PARAM_ID_ACTCLK_DRIFT               = 0x09,
    /// External wake-up time
    PARAM_ID_EXT_WAKEUP_TIME            = 0x0D,
    /// Oscillator wake-up time
    PARAM_ID_OSC_WAKEUP_TIME            = 0x0E,
    /// Radio wake-up time
    PARAM_ID_RM_WAKEUP_TIME             = 0x0F,
    /// UART baudrate
    PARAM_ID_UART_BAUDRATE              = 0x10,
    /// Enable sleep mode
    PARAM_ID_SLEEP_ENABLE               = 0x11,
    /// Enable External Wakeup
    PARAM_ID_EXT_WAKEUP_ENABLE          = 0x12,
    /// SP Private Key 192
    PARAM_ID_SP_PRIVATE_KEY_P192        = 0x13,
    /// SP Public Key 192
    PARAM_ID_SP_PUBLIC_KEY_P192         = 0x14,
    /// Synchronous links configuration
    PARAM_ID_SYNC_CONFIG                = 0x2C,
    /// PCM Settings
    PARAM_ID_PCM_SETTINGS               = 0x2D,
    /// Sleep algorithm duration
    PARAM_ID_SLEEP_ALGO_DUR             = 0x2E,

    /// Diagport configuration
    PARAM_ID_DIAG_BT_HW                 = 0x30,
    PARAM_ID_DIAG_BLE_HW                = 0x31,
    PARAM_ID_DIAG_SW                    = 0x32,
    PARAM_ID_DIAG_PLF                   = 0x34,

    /// RSSI threshold tags
    PARAM_ID_RSSI_HIGH_THR              = 0x3A,
    PARAM_ID_RSSI_LOW_THR               = 0x3B,
    PARAM_ID_RSSI_INTERF_THR            = 0x3C,




    PARAM_ID_BT_LINK_KEY_FIRST          = 0x60,
    PARAM_ID_BT_LINK_KEY_LAST           = 0x67,

    PARAM_ID_BLE_LINK_KEY_FIRST         = 0x70,
    PARAM_ID_BLE_LINK_KEY_LAST          = 0x7F,
    /// SC Private Key (Low Energy)
    PARAM_ID_LE_PRIVATE_KEY_P256        = 0x80,
    /// SC Public Key (Low Energy)
    PARAM_ID_LE_PUBLIC_KEY_P256         = 0x81,
    /// SC Debug: Used Fixed Private Key from NVDS (Low Energy)
    PARAM_ID_LE_DBG_FIXED_P256_KEY      = 0x82,
    /// SP Private Key (classic BT)
    PARAM_ID_SP_PRIVATE_KEY_P256        = 0x83,
    /// SP Public Key (classic BT)
    PARAM_ID_SP_PUBLIC_KEY_P256         = 0x84,

    /// Application specific
    PARAM_ID_APP_SPECIFIC_FIRST         = 0x90,
    PARAM_ID_APP_SPECIFIC_LAST          = 0xAF,
};

/// List of parameters lengths
enum PARAM_LEN
{
     // Definition of length associated to each parameters
     /// Local Bd Address
     PARAM_LEN_BD_ADDRESS                 = 6,
     /// Device Name
     PARAM_LEN_DEVICE_NAME                = 248,
     /// Low power clock drift
     PARAM_LEN_LPCLK_DRIFT                = 2,
     /// Low power clock jitter
     PARAM_LEN_LPCLK_JITTER               = 1,
     /// Active clock drift
     PARAM_LEN_ACTCLK_DRIFT               = 1,


     /// External wake-up time
     PARAM_LEN_EXT_WAKEUP_TIME            = 2,
     /// Oscillator wake-up time
     PARAM_LEN_OSC_WAKEUP_TIME            = 2,
     /// Radio wake-up time
     PARAM_LEN_RM_WAKEUP_TIME             = 2,
     /// UART baudrate
     PARAM_LEN_UART_BAUDRATE              = 4,
     /// Enable sleep mode
     PARAM_LEN_SLEEP_ENABLE               = 1,
     /// Enable External Wakeup
     PARAM_LEN_EXT_WAKEUP_ENABLE          = 1,
     /// SP Private Key 192
     PARAM_LEN_SP_PRIVATE_KEY_P192        = 24,
     /// SP Public Key 192
     PARAM_LEN_SP_PUBLIC_KEY_P192         = 48,
     /// Synchronous links configuration
     PARAM_LEN_SYNC_CONFIG                = 2,
     /// PCM Settings
     PARAM_LEN_PCM_SETTINGS               = 8,
     /// Diagport configuration
     PARAM_LEN_DIAG_BT_HW                 = 4,
     PARAM_LEN_DIAG_BLE_HW                = 4,
     PARAM_LEN_DIAG_SW                    = 4,
     PARAM_LEN_DIAG_PLF                   = 4,
     /// RSSI thresholds
     PARAM_LEN_RSSI_THR                   = 1,



     /// Link keys
     PARAM_LEN_BT_LINK_KEY                = 22,
     PARAM_LEN_BLE_LINK_KEY               = 48,

     /// P256
     PARAM_LEN_PRIVATE_KEY_P256           = 32,
     PARAM_LEN_PUBLIC_KEY_P256            = 64,
     PARAM_LEN_DBG_FIXED_P256_KEY         = 1,
};

/******************************************************************************************/
/* -------------------------        BT-BLE COEX        -----------------------------------*/
/******************************************************************************************/

///To let the HW using the default values set in the registers
#define RW_BLE_PTI_PRIO_AUTO    15

#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
/// Enable and diable definition for the PTI
///Enable TX busy signal
#define RWIP_PTI_TXEN           1
///Disable TX busy signal
#define RWIP_PTI_TXDIS          0
/// Tx busy position
#define RWIP_TXBSY_POS          0

///Enable RX busy signal
#define RWIP_PTI_RXEN           1
///Disable RX busy signal
#define RWIP_PTI_RXDIS          0
/// Rx busy position
#define RWIP_RXBSY_POS          1

///Enable do not abort TX
#define RWIP_PTI_DNABORTEN      1
///Disable do not abort TX
#define RWIP_PTI_DNABORTDIS     0
/// Do not abort busy position
#define RWIP_DNABORT_POS        2

/// SAM disabled
#define RWIP_SAM_DIS            0
/// SAM enabled
#define RWIP_SAM_EN             1
/// SAM enable position
#define RWIP_SAMEN_POS          3


/// Bit masking
#define RWIP_COEX_BIT_MASK      1

/// Coex configuration index
enum rwip_coex_config_idx
{
    #if (BT_EMB_PRESENT)
    RWIP_COEX_MSSWITCH_IDX,
    RWIP_COEX_SNIFFATT_IDX,
    RWIP_COEX_PAGE_IDX,
    RWIP_COEX_PSCAN_IDX,
    RWIP_COEX_INQ_IDX,
    RWIP_COEX_INQRES_IDX,
    RWIP_COEX_SCORSVD_IDX,
    RWIP_COEX_BCAST_IDX,
    RWIP_COEX_CONNECT_IDX,
    #endif //#if (BT_EMB_PRESENT)
    #if (BLE_EMB_PRESENT)
    RWIP_COEX_CON_IDX,
    RWIP_COEX_CON_DATA_IDX,
    RWIP_COEX_ADV_IDX,
    RWIP_COEX_SCAN_IDX,
    RWIP_COEX_INIT_IDX,
    #endif // #if (BLE_EMB_PRESENT)
    /// Max configuration index
    RWIP_COEX_CFG_MAX,
};
#endif //(BLE_EMB_PRESENT || BT_EMB_PRESENT)

/******************************************************************************************/
/* -------------------------     BT-BLE PRIORITIES     -----------------------------------*/
/******************************************************************************************/
#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
/// Priority index definition
enum rwip_prio_idx
{
    #if (BT_EMB_PRESENT)
    /// ACL event default priority
    RWIP_PRIO_ACL_DFT_IDX,
    /// ACL event priority with activity
    RWIP_PRIO_ACL_ACT_IDX,
    /// ACL Role Switch event default priority
    RWIP_PRIO_ACL_RSW_IDX,
    /// ACL sniff event default priority
    RWIP_PRIO_ACL_SNIFF_DFT_IDX,
    /// ACL sniff transition event default priority
    RWIP_PRIO_ACL_SNIFF_TRANS_IDX,
    #if MAX_NB_SYNC
    /// SCO event default priority
    RWIP_PRIO_SCO_DFT_IDX,
    #endif //MAX_NB_SYNC
    /// Broadcast ACL event default priority
    RWIP_PRIO_BCST_DFT_IDX,
    /// Broadcast ACL event with LMP activity priority
    RWIP_PRIO_BCST_ACT_IDX,
    /// CSB RX event default priority
    RWIP_PRIO_CSB_RX_DFT_IDX,
    /// CSB TX event default priority
    RWIP_PRIO_CSB_TX_DFT_IDX,
    /// Inquiry event default priority
    RWIP_PRIO_INQ_DFT_IDX,
    /// Inquiry Scan event default priority
    RWIP_PRIO_ISCAN_DFT_IDX,
    /// Page event default priority
    RWIP_PRIO_PAGE_DFT_IDX,
    /// Page event default priority
    RWIP_PRIO_PAGE_1ST_PKT_IDX,
    /// Page first packet event default priority
    RWIP_PRIO_PCA_DFT_IDX,
    /// Page scan event default priority
    RWIP_PRIO_PSCAN_DFT_IDX,
    /// Page scan event priority increment when canceled
    RWIP_PRIO_PSCAN_1ST_PKT_IDX,
    /// Synchronization Scan event default priority
    RWIP_PRIO_SSCAN_DFT_IDX,
    /// Synchronization Train event default priority
    RWIP_PRIO_STRAIN_DFT_IDX,
    #endif //#if (BT_EMB_PRESENT)
    #if (BLE_EMB_PRESENT)
    /// Default priority for scanning events
    RWIP_PRIO_SCAN_IDX,
    /// Default priority for initiating events
    RWIP_PRIO_INIT_IDX,
    /// LE connection events default priority
    RWIP_PRIO_CONNECT_DFT_IDX,
    /// LE connection events priority with activity
    RWIP_PRIO_CONNECT_ACT_IDX,
    /// Default priority for advertising events
    RWIP_PRIO_ADV_IDX,
    /// Default priority for advertising high duty cycle events
    RWIP_PRIO_ADV_HDC_IDX,
    /// Default priority for resolvable private addresses renewal event
    RWIP_PRIO_RPA_RENEW_IDX,
    #endif // #if (BLE_EMB_PRESENT)
    RWIP_PRIO_IDX_MAX
};
/// Default priority value definition
enum rwip_prio_dft
{
    #if (BT_EMB_PRESENT)
    /// ACL event default priority
    RWIP_PRIO_ACL_DFT               = 40,
    /// ACL event priority with activity
    RWIP_PRIO_ACL_ACT               = 80,
    /// ACL Role Switch event default priority
    RWIP_PRIO_ACL_RSW               = 160,
    /// ACL sniff event default priority
    RWIP_PRIO_ACL_SNIFF_DFT         = 120,
    /// ACL sniff transition event default priority
    RWIP_PRIO_ACL_SNIFF_TRANS       = 80,
    #if MAX_NB_SYNC
    /// SCO event default priority
    RWIP_PRIO_SCO_DFT               = 144,
    #endif //MAX_NB_SYNC
    /// Broadcast ACL event default priority
    RWIP_PRIO_BCST_DFT              = 40,
    /// Broadcast ACL event with LMP activity priority
    RWIP_PRIO_BCST_ACT              = 80,
    /// CSB RX event default priority
    RWIP_PRIO_CSB_RX_DFT            = 80,
    /// CSB TX event default priority
    RWIP_PRIO_CSB_TX_DFT            = 80,
    /// Inquiry event default priority
    RWIP_PRIO_INQ_DFT               = 40,
    /// Inquiry Scan event default priority
    RWIP_PRIO_ISCAN_DFT             = 40,
    /// Page event default priority
    RWIP_PRIO_PAGE_DFT              = 64,
    /// Page first packet event default priority
    RWIP_PRIO_PAGE_1ST_PKT          = 160,
    /// PCA event default priority
    RWIP_PRIO_PCA_DFT               = 160,
    /// Page scan event default priority
    RWIP_PRIO_PSCAN_DFT             = 64,
    /// Page scan event priority increment when canceled
    RWIP_PRIO_PSCAN_1ST_PKT         = 160,
    /// Synchronization Scan event default priority
    RWIP_PRIO_SSCAN_DFT             = 80,
    /// Synchronization Train event default priority
    RWIP_PRIO_STRAIN_DFT            = 80,
    #endif //#if (BT_EMB_PRESENT)
    #if (BLE_EMB_PRESENT)
    /// Default priority for scanning events
    RWIP_PRIO_SCAN_DFT              = 40,
    /// Default priority for initiating events
    RWIP_PRIO_INIT_DFT              = 80,
    /// LE connection events default priority
    RWIP_PRIO_CONNECT_DFT           = 112,
    /// LE connection events priority with activity
    RWIP_PRIO_CONNECT_ACT           = 128,
    /// Default priority for advertising events
    RWIP_PRIO_ADV_DFT               = 40,
    /// Default priority for advertising high duty cycle events
    RWIP_PRIO_ADV_HDC_DFT           = 80,
    /// Default priority for resolvable private addresses renewal event
    RWIP_PRIO_RPA_RENEW_DFT         = 80,
    #endif // #if (BLE_EMB_PRESENT)
    /// Max priority
    RWIP_PRIO_MAX                   = 255,
};
/// Default increment value definition
enum rwip_incr_dft
{
    #if (BT_EMB_PRESENT)
    /// ACL event default increment
    RWIP_INCR_ACL_DFT               = 8,
    /// ACL event increment with activity
    RWIP_INCR_ACL_ACT               = 8,
    /// ACL Role Switch event default increment
    RWIP_INCR_ACL_RSW               = 8,
    /// ACL sniff event default increment
    RWIP_INCR_ACL_SNIFF_DFT         = 8,
    /// ACL sniff transition event default increment
    RWIP_INCR_ACL_SNIFF_TRANS       = 8,
    #if MAX_NB_SYNC
    /// SCO event default increment
    RWIP_INCR_SCO_DFT               = 8,
    #endif //MAX_NB_SYNC
    /// Broadcast ACL event default increment
    RWIP_INCR_BCST_DFT              = 8,
    /// Broadcast ACL event with LMP activity increment
    RWIP_INCR_BCST_ACT              = 8,
    /// CSB RX event default increment
    RWIP_INCR_CSB_RX_DFT            = 8,
    /// CSB TX event default increment
    RWIP_INCR_CSB_TX_DFT            = 8,
    /// Inquiry event default increment
    RWIP_INCR_INQ_DFT               = 8,
    /// Inquiry Scan event default increment
    RWIP_INCR_ISCAN_DFT             = 8,
    /// Page event default increment
    RWIP_INCR_PAGE_DFT              = 8,
    /// Page event default increment
    RWIP_INCR_PAGE_1ST_PKT          = 12,
    /// Page first packet event default increment
    RWIP_INCR_PCA_DFT               = 8,
    /// Page scan event default increment
    RWIP_INCR_PSCAN_DFT             = 8,
    /// Page scan event increment increment when canceled
    RWIP_INCR_PSCAN_1ST_PKT         = 8,
    /// Synchronization Scan event default increment
    RWIP_INCR_SSCAN_DFT             = 8,
    /// Synchronization Train event default increment
    RWIP_INCR_STRAIN_DFT            = 8,
    #endif //#if (BT_EMB_PRESENT)
    #if (BLE_EMB_PRESENT)
    /// Default increment for scanning events
    RWIP_INCR_SCAN_DFT              = 8,
    /// Default increment for initiating events
    RWIP_INCR_INIT_DFT              = 8,
    /// LE connection events default increment
    RWIP_INCR_CONNECT_DFT           = 8,
    /// LE connection events increment with activity
    RWIP_INCR_CONNECT_ACT           = 8,
    /// Default increment for advertising events
    RWIP_INCR_ADV_DFT               = 8,
    /// Default increment for advertising high duty cycle events
    RWIP_INCR_ADV_HDC_PRIO_DFT      = 8,
    /// Default increment for resolvable private addresses renewal event
    RWIP_INCR_RPA_RENEW_DFT         = 8,
    #endif // #if (BLE_EMB_PRESENT)
};
#endif //#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
/// @} BT Stack Configuration
/// @} ROOT

#endif //RWIP_CONFIG_H_

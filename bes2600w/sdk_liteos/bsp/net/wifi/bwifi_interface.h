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
#ifndef BWIFI_INTERFACE_H
#define BWIFI_INTERFACE_H

#include "plat_types.h"
#include "cmsis_os.h"

#include "bwifi_sta.h"
#include "bwifi_event.h"
#if LWIP_ETHERNETIF
#include "lwip/netif.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __DUAL_BAND__
#define BWIFI_BAND_DEFAULT              DUAL_BAND //0: 2.4G; 1: 5G; 2: dual_band
#else
#define BWIFI_BAND_DEFAULT              BAND_2G4
#endif
#define BWIFI_MAX_COUNTRY_CODE_SIZE     2
#define BWIFI_CONFIG_VAL_LEN            100
#define WIFI_SCAN_DUMP

struct wifi_init_paras {
    char     band;
    char     country[3];
    uint16_t *tx_power;
    uint16_t *tx_power_5G;
    uint8_t  *wifi_address;
    void     *fac_wifi;
};

typedef enum {
    WIFI_IF_STATION,
    WIFI_IF_SOFTAP,
} BWIFI_INTF_TYPE_T;

typedef enum {
    BWIFI_STATUS_IDLE            = 0,
    BWIFI_STATUS_DISCONNECTING   = 1,
    BWIFI_STATUS_SCANNING        = 2,
    BWIFI_STATUS_CONNECTING      = 3,
    BWIFI_STATUS_WPS_CONNECTING  = 4,
    BWIFI_STATUS_CONNECTED       = 5,
    BWIFI_STATUS_DHCPING         = 6,
    BWIFI_STATUS_GOT_IP          = 7,
    BWIFI_STATUS_ONLINE_SCANNING = 8,
    BWIFI_STATUS_END_DEF,  /* must be the last one */
} BWIFI_STATUS_T;

enum {
    BWIFI_R_OK                  = 0,
    BWIFI_R_COMMON_FAIL         = -1,
    BWIFI_R_INVALID_ARG         = -2,  //invalid argument
    BWIFI_R_INVALID_PASSWORD    = -3,  //invalid password
    BWIFI_R_MEMORY_ERROR        = -4,  //no memory to allocate resource
    BWIFI_R_INIT_FAIL           = -5,  //init wifi fail
    BWIFI_R_NOT_INITED          = -6,  //wifi is not initialized
    BWIFI_R_STATUS_ERROR        = -7,  //request in error STATUS
    BWIFI_R_SCAN_REQ_FAIL       = -8,  //scan fail to start
    BWIFI_R_SCAN_NO_AP_FOUND    = -9,  //scan result is NULL (didn't find any SSID)
    BWIFI_R_NO_SUITABLE_NETWORK = -10, //no suitable network to connect
    BWIFI_R_CONN_REQ_FAIL       = -11, //connect fail to start
    BWIFI_R_CONN_FAIL           = -12, //connect procedure result in fail
    BWIFI_R_CONN_NO_SSID_CONFIG = -13, //no saved SSID config to connect
    BWIFI_R_DISC_FAIL           = -14, //disconnect procedure result in fail
    BWIFI_R_WPS_NOT_FOUND       = -15, //couldn't find WPS AP
    BWIFI_R_WPS_REQ_FAIL        = -16, //WPS fail to start
};

enum {
    WIFI_MFP_DISABLE = 0,   /**< disable mfp capability */
    WIFI_MFP_ENABLE,        /**< have mfp capability, but not require peer to support mfp */
    WIFI_MFP_REQUIRED,      /**< have mfp capability, and require peer to support mfp */
    WIFI_MFP_DEFAULT,       /**< mfp is not specified, setting it based on our capability and network type */
};

typedef enum bwifi_softap_status {
    BWIFI_SOFTAP_STATUS_OFF   = 0,
    BWIFI_SOFTAP_STATUS_ON,
} BWIFI_SOFTAP_STATUS_T;

typedef enum {
    WIFI_USER_EVT_CONN_INTER_STATE,
    WIFI_USER_EVT_CONNECTED,
    WIFI_USER_EVT_GOT_IP,
    WIFI_USER_EVT_DISCONNECTED,
    WIFI_USER_EVT_ROAM_COMPLETE,
    WIFI_USER_EVT_AP_ENABLED,
    WIFI_USER_EVT_AP_DISABLED,
    WIFI_USER_EVT_AP_STA_CONNECTED,
    WIFI_USER_EVT_AP_STA_DISCONNECTED,
    WIFI_USER_EVT_FATAL_ERROR,
    /* mutually exclusive events */
    /* If user layer wants to take over the initiative of wifi reset,
     * register this event for the timing to trigger reset.
     */
    WIFI_USER_EVT_MAC_RESET_REQUEST,
    /* If user layer is only interested in the process of wifi reset,
     * register these events to get notify when reset start/end.
     */
    WIFI_USER_EVT_MAC_RESET_START,
    WIFI_USER_EVT_MAC_RESET_DONE,
    WIFI_USER_EVT_COEX_MODE_CHANGE,
    WIFI_USER_EVT_MAX
} WIFI_USER_EVT_ID;

typedef enum {
    FATAL_ERROR = 1,
    UMAC_CRASH  = 2,
} BWIFI_MAC_RESET_REASON;

typedef enum bwifi_security_type {
    SECURITY_NONE,              /*!< open access point */
    SECURITY_WEP40,             /*!< phrase conforms to WEP */
    SECURITY_WEP104,            /*!< phrase conforms to WEP EXTENDED */
    SECURITY_WPA,               /*!< WPA-PSK */
    SECURITY_WPA2,              /*!< WPA2-PSK */
    SECURITY_WPA_WPA2,          /*!< WPA WPA2 mixed mode */
    SECURITY_WPA3_SAE,          /*!< WPA3-SAE */
    SECURITY_WPA3_SAE_WPA2,     /*!< WPA3-SAE WPA2 mixed mode*/
    SECURITY_ERROR,             /*!< error mode */
} BWIFI_SEC_TYPE_T;

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

struct bwifi_mac_addr {
    u8 mac[ETH_ALEN];
};

struct bwifi_bss_info {
    u8 bssid[ETH_ALEN];
    char ssid[32 + 1];
    u8 channel;
    s8 rssi;
    BWIFI_SEC_TYPE_T security_type;
    u8 *ie; //user program couldn't free(ie);
    u32 ie_length;
};

struct bwifi_ap_info {
    char ssid[32 + 1];
    u8 channel;
    u8 hidden;
    BWIFI_SEC_TYPE_T security;
    int sec_channel_offset;
    int wpa_mode;
    char passwd[64];
};

struct bwifi_ssid {
    struct bwifi_ssid *next;
    u8 ssid[32 + 1]; //null-terminated string
};

struct bwifi_scan_config {
    struct bwifi_ssid *ssids;   /**< List of specified SSIDs */
    int *channels;              /**< Array of channels (zero-terminated) to scan or NULL for all channels */
};

typedef struct bwifi_trans_stat_t {
    uint32_t tx_succ_cnt;
    uint32_t tx_fail_cnt;
    uint32_t tx_retry_cnt;
    uint32_t rx_cnt;
    uint32_t txbps; //tx throughput bit/s
    uint32_t rxbps; //rx throughput bit/s
} bwifi_trans_stat;

typedef struct bwifi_station_linkinfo_t {
    bwifi_trans_stat stat;
    s8 rssi;
    s8 channel;
    /* BIT 31: MCS_FLAG = 1, BTI30:ShortGI ,BIT29-26: BW(0 20M,1 40M), BIT25-0,MCS index
     * BIT 31: MCS_FALG = 0, BIT0-B25 is b/g tx rate
     * */
    int tx_rate;
} bwifi_station_linkinfo;

struct ip_info {
    uint32_t ip;               /**< IP address */
    uint32_t netmask;          /**< netmask */
    uint32_t gw;               /**< gateway */
};

enum wifi_config_item {
    WIFI_CONFIG_ITEM_WAPI_CERT_PRIVATE_KEY = 0, /**< WAPI own private key */
    WIFI_CONFIG_ITEM_WAPI_CERT_AS_CERT,         /**< WAPI AS certification */
    WIFI_CONFIG_ITEM_WAPI_CERT_ASUE_CERT,       /**< WAPI ASUE certification */
    WIFI_CONFIG_ITEM_MAX
};

struct bwifi_config_item {
    int net_index;                      /**< network index */
    int item_index;                     /**< config item index */
    int val_len;                        /**< config item value length */
    uint8_t val[BWIFI_CONFIG_VAL_LEN];  /**< config item value */
};

typedef void (*user_evt_handler_t)(WIFI_USER_EVT_ID evt_id, void *arg);

void bwifi_reg_user_evt_handler(WIFI_USER_EVT_ID evt_id, user_evt_handler_t cb);
void bwifi_unreg_user_evt_handler(WIFI_USER_EVT_ID evt_id);
void bwifi_reg_eth_input_handler(eth_input_handler cb);
void bwifi_unreg_eth_input_handler();

/*
 * wifi record interface
 */
int bwifi_find_record(const char *type, void *record_info);
int bwifi_add_record(void *record_info);
int bwifi_del_record(char *type);
/*
 * Add network config to wpa_supplicant
 * return the allocated network id, or negative on failure.
 */
int bwifi_add_config(struct bwifi_station_config *config);
/*
 * Modify network config item
 * return the result of current operation
 */
int bwifi_modify_config(struct bwifi_config_item *item);
/*
 * Get the number of saved network configs from wpa_supplicant.
 */
int bwifi_count_configured_ssid(void);
/*
 * Get network configs saved in wpa_supplicant
 * return the number of networks
 */
int bwifi_get_current_config(struct bwifi_station_config *config, int count);
/*
 * Save network config to flash (max 8)
 */
int bwifi_save_config(struct bwifi_station_config *config);
/*
 * Get network configs saved in flash
 * return the number of networks
 */
int bwifi_get_saved_config(struct bwifi_station_config *config, int count);
/*
 * Find network config saved in flash who matches with the specified ssid
 */
int bwifi_find_saved_config_by_ssid(const char *ssid, struct bwifi_station_config *config);
/*
 * Delete all network configs saved in wpa and flash
 */
int bwifi_del_all_config(void);
/*
 * Delete network config saved in wpa and flash
 * It will check ssid, passwd, hidden, web_keyid, bssid (if not zero),
 * if config is NULL or config->ssid is zero length, delete all.
 */
int bwifi_del_config(struct bwifi_station_config *config);
/*
 * Scan for wildcard ssid and saved hidden ssids
 * you can get bss list from bwifi_get_scan_result
 * return bss number or error code
 */
int bwifi_scan(void);
/*
 * Scan for specified ssid (if not NULL) on the specified channels (if no 0)
 * you can get bss list from bwifi_get_scan_result
 * return bss number or error code
 */
int bwifi_config_scan(struct bwifi_scan_config *scan_config);
/*
 * Get scan result
 */
int bwifi_get_scan_result(struct bwifi_bss_info *result, int count);
/*
 * Clear saved scan list which is not in use
 */
void bwifi_flush_scan_result(void);
/*
 * Auto connect to an AP saved in wpa_supplicant
 */
int bwifi_auto_connect(void);
/*
 * Connect to an AP assigned by user config
 *
 * This function will add the network config to wpa_supplicant and trigger new connection.
 * If user wants to connect to another AP after connected:
 *   1. bwifi_disconnect()
 *   2. bwifi_connect(config)
 */
int bwifi_connect(struct bwifi_station_config *config);
/*
 * Connect to an AP via WPS PBC
 */
int bwifi_connect_wps_pbc(void);
/*
 * Connect to specified ssid, passwd can be NULL for unencrypted AP.
 */
int bwifi_connect_to_ssid(const char *ssid, const char *passwd, int8 wep_keyid, u8 hidden, u8 *bssid);
/*
 * Connect to specified ssid and mfp, passwd can't be NULL
 */
int bwifi_connect_to_ssid_with_mfp(const char *ssid, const char *passwd, int8 wep_keyid, u8 hidden, u8 *bssid, u8 mfp);
/*
 * Connect to specified bssid, passwd can be NULL for unencrypted AP.
 */
int bwifi_connect_to_bssid(u8 *bssid, const char *passwd);
/*
 * Connect to specified network by network id allocated in wpa_supplicant.
 */
int bwifi_connect_to_nid(int nid);
/*
 * Disconnect to current connected AP or stop connecting to AP
 */
int bwifi_disconnect(void);

/*
 * Quick connect functions
 */
int bwifi_get_quick_connect_config(struct bwifi_quick_connect_config *quick_config);
int bwifi_set_quick_connect_config(struct bwifi_quick_connect_config *quick_config);
int bwifi_del_quick_connect_config(void);
int bwifi_quick_connect(struct bwifi_quick_connect_config *quick_config);

/*
 * Set whether auto reconnect after disconnection
 * default val true
 */
void bwifi_set_reconnect_policy(bool enable);
/*
 * Get current reconnect policy
 */
bool bwifi_get_reconnect_policy(void);
/*
 * Get current connected AP info
 */
int  bwifi_get_current_ssid(char *ssid);
int  bwifi_get_current_bssid(u8 *bssid);
int  bwifi_get_own_mac(u8 *addr);
u8   bwifi_get_current_channel(void);
int8 bwifi_get_current_rssi(void);
int  bwifi_get_current_rate(void);

/**
 * Get station linkinfo statistics which can be used to evaluate network traffic.
 * @info: pointer to a bwifi_station_linkinfo struct to linkinfo statistics
 * Returns: void
 */
void bwifi_get_station_linkinfo(bwifi_station_linkinfo *info);

#if LWIP_SUPPORT
/*
 * Enable or disable using the static IP for subsequent connection.
 *
 * The DHCP clent is enabled by default and collides with the static IP.
 * If this API is callbed with a valid pointer to the ip_info struct,
 * DHCP client will be disabled and the static IP in ip_info will be used;
 * if this API is called with NULL, then DHCP client will be enabled.
 * It depends on the latest configuration.
 */
int bwifi_set_static_ip(struct ip_info *ip);
/*
 * Get current ip addr of wifi station (dhcpc or static ip)
 */
int bwifi_get_current_ip(struct ip_info *ip);

#else

#if LWIP_ETHERNETIF
/*
 * Get netif struct of wifi station or softap
 *
 * This function should only be called when SDK inside LWIP is turned off
 * but ethernet interface is added and initialized by us.
 * return the netif struct to user's LWIP stack for further management.
 */
struct netif *bwifi_get_netif(BWIFI_INTF_TYPE_T type);
#endif

/*
 * Set netif ip addr to wifi mac layer for ARP filter feature
 *
 * This function should only be called when SDK inside LWIP is turned off
 * and DHCP procedure is also taken over by user's LWIP stack,
 * we need user to tell us the assigned local ip addr.
 */
int bwifi_set_ip_addr(BWIFI_INTF_TYPE_T type, struct ip_info *ip);
#endif

void airkiss_notify(uint8 token);

/**
 * Enable or disable the statistics of the frames sent out and received
 * in the last interval_sec time.
 * @en: 0 = disable, 1 = enable
 * @interval_sec: time of the statistics in seconds
 * Returns: void
 *
 * @note    If enabled, the statistics information will be output via uart log.
 */
void bwifi_trans_stat_en(uint8_t en, uint8_t interval_sec);
/**
 * Get trans statistics which can be used to evaluate network traffic.
 * @stat: pointer to a bwifi_trans_stat struct to retrieve trans statistics
 * @clear: 1 = clear, 0 = keep
 * Returns: void
 *
 * @note    Elements in bwifi_trans_stat will be cleared every time this function
 *          is called if clear is set to 1.
 */
void bwifi_trans_stat_get(bwifi_trans_stat *stat, int8 clear);

/*
 * Initialize wifi hardware and interface
 */
int bwifi_init(void);

#ifdef WIFI_ON_CP
/*
 * reset wifi stack on cp
 */
int bwifi_cp_reset(void);
#endif

/*
 * Set country code
 * If not set, we use the default country code CN.
 */
int bwifi_set_country_code(char *country);
/*
 * Get current country code
 */
int bwifi_get_country_code(char *country, int size);
int bwifi_set_band(uint8 band);
int bwifi_get_band(uint8 *band);
void bwifi_band_switch(uint8 band);
void bwifi_set_connecting_status(void);
/*
 * Set powersave mode for legacy Wi-Fi.
 * @ps: 0 = disable, 1 = enable
 * Returns: 0 on success or negtive on failure
 */
int bwifi_set_ps_mode(int ps);

/*
 * swtich channel dynamically
 * @mode: 0 softap mode, 1 sta mode
 * @channel: channel number
 * @snd_offset: secondary channel offset to center freq, 0 - no snd ch, 1 - upper 10M -1 - lower 10M
 * Returns: 0 on success or negtive on failure
 */
int bwifi_switch_channel(uint8_t mode, uint8_t channel, int8_t snd_offset);
#ifdef __AP_MODE__
/*
 * Start softap with previous configuration
 */
int bwifi_softap_start(void);
/*
 * Stop softap
 */
void bwifi_softap_stop(void);
/**
 * Set softap basic configuration
 *
 * @ssid: AP's SSID
 * @channel: Channel number for AP to start on
 * @sec_channel_offset: Secondary channel offset for HT40
 *     0 = HT40 disabled;
 *     -1 = HT40 enabled, secondary channel below primary;
 *     1 = HT40 enabled, secondary channel above primary
 * @hidden: Whether to ignore broadcast SSID
 *     0 = broadcast ssid;
 *     1 = hide ssid with zero length;
 *     2 = hide ssid with zero contents
 * @security: Enum of security type
 * @password: User passphrase
 *
 * Returns: 0 on success, nagtive if invalid arguments
 */
int bwifi_set_softap_config(char *ssid,
                            u8 channel, int sec_channel_offset,
                            u8 hidden,
                            BWIFI_SEC_TYPE_T security,
                            char *passwd);
/*
 * Set softap's maximum client number
 * @sta_num: Maximum number of STAs
 * Returns: void
 */
void bwifi_set_softap_max_sta_num(int sta_num);
/*
 * Set softap's country code
 * @country_code: Two octets string to represent a country code
 * Returns: void
 */
void bwifi_set_softap_country_code(char *country_code);
/**
 * Add vendor elements in softap's Beacon/ProbeRsp
 * @ie: Pointer to raw data of vendor elements
 * @ie_len: Length of vendor elements
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_add_softap_vendor_elements(const u8 *ie, size_t ie_len);
/*
 * Get softap's client list
 * @mac_list: Pointer to buffer to store the mac address list
 * @count: STA count to retrieve, returns the actual connected STA count
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_softap_get_station_list(struct bwifi_mac_addr *mac_list, int *count);

#endif

/*
 * Get current Wi-Fi status
 */
BWIFI_STATUS_T bwifi_get_current_status(void);
/*
 * Get current softap status
 */
BWIFI_SOFTAP_STATUS_T bwifi_get_softap_status(void);
/*
 * Enable or disable Wi-Fi recovery mechanism on fatal error.
 * which is turned off by default for debug purpose.
 */
void bwifi_recovery_enable(bool en);
/**
 * Set fix data rate for Wi-Fi transmission
 * @fix_rate_idx
 *   - 0 ~ 3:   DSSS/CCK rates: 1, 2, 5.5, 11;
 *   - 4 ~ 5:   reserved;
 *   - 6 ~ 13:  legacy OFDM rates: 6, 9, 12, 18, 24, 36, 48, 54;
 *   - 14 ~ 21: 1*1 MCS rates: MCS0 ~ MCS7;
 *   - 0xff:    default value to disable the fix rate function.
 */
void bwifi_set_fix_rate(u32 fix_rate_idx);
/*
 * Get current set fix rate index
 */
u32  bwifi_get_fix_rate(void);

/*
 * Do wifi reset
 * Returns: BWIFI_R_OK on success, others on failure
 */
int bwifi_do_wifi_reset(void);

/*
 * set epta parameters of wifi/bt coex
 * wifi_dur: wifi duration of active window
 * bt_dur: bt duration of active window
 * mode: epta mode
        0 - periodic mode in which the wifi/bt active time is specified by wifi_dur/bt_dur, only support 100ms period
        1 - arbitrate mode by hw itself
        2 - wifi active only mode
*/
void bwifi_set_epta_param(int wifi_dur, int bt_dur, int mode);

#ifdef __cplusplus
}
#endif

#endif /* BWIFI_INTERFACE_H */

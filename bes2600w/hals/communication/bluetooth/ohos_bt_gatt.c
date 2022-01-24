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

#include "stdbool.h"
#include "string.h"
#include "cmsis_os.h"
#include "hal_trace.h"
#include "ohos_bt_gatt.h"
#include "ohos_bt_def.h"
//#include "ohos_bt_gatt_adapter.h"
#include "plat_types.h"
#include "bluetooth.h"
#include "stdlib.h"
#include "hal_timer.h"
#include "app_ble_mode_switch.h"
#include "app_ble_customif.h"
#include "factory_section.h"
#include "app_task.h"
#include "gap.h"



#define BTM_IO_DISPLAY_ONLY   0
#define BTM_IO_DISPLAY_YESNO  1
#define BTM_IO_KEYBOARD_ONLY  2
#define BTM_IO_NO_IO          3
#define ADV_TIMER_IDLE             0
#define ADV_TIMER_RUNNING          1
#define HARMONY_ADV_OCCUPIED_FLAG (0xff)
#define HARMONY_ADV_FREE_FLAG (0x11)
#define HARMONY_ADV_MAX_SIZE (6)

static void adv_enable_timer_handler(void const *param);
osTimerDef(adv_enable_timer, adv_enable_timer_handler);
static osTimerId adv_enable_timer_id = NULL;
static uint32_t adv_enable_para;
uint8_t adv_timer_run_state = ADV_TIMER_IDLE;
static BleScanParams blescanparamter;
static BtGattCallbacks ohos_g_gatt_callback_imp = {0};
BtGattCallbacks * ohos_g_gatt_callback = &ohos_g_gatt_callback_imp;

extern void bes_bt_init(void);
extern void app_sec_set_io_capabilities(uint8_t ioCap);
extern void app_sec_set_auth(uint8_t auth);
extern void app_ble_custom_init(void);
// reference to harmony_ble_gatt.c
typedef struct ble_addr
{
    /// BD Address of device
    uint8_t addr[BTIF_BD_ADDR_SIZE];
    /// Address type of the device 0=public/1=private random
    uint8_t addr_type;
} ohos_ble_addr_t;

typedef struct __ohos_ble_adv_record{
    unsigned char flag;
    unsigned char adv_index;
    int advId;
    ohos_ble_addr_t local_addr;
}ohos_ble_adv_record;

static ohos_ble_adv_record g_ohos_adv_record[HARMONY_ADV_MAX_SIZE];

static void adv_enable_timer_handler(void const *param)
{
    OHOS_BleStopAdv(*(uint8_t*)param);
    adv_timer_run_state = ADV_TIMER_IDLE;
}

static BLE_ADV_TYPE_E ble_adv_type_harmony_change_to_bes(BleAdvType type)
{
    BLE_ADV_TYPE_E adv_type = ADV_TYPE_NON_CONN_NON_SCAN;
    switch(type)
    {
        case OHOS_BLE_ADV_IND:
            adv_type = ADV_TYPE_UNDIRECT;
        break;
        case OHOS_BLE_ADV_DIRECT_IND_HIGH:
            adv_type = ADV_TYPE_DIRECT_HDC;
        break;
        case OHOS_BLE_ADV_SCAN_IND:
            adv_type = ADV_TYPE_NON_CONN_SCAN;
        break;
        case OHOS_BLE_ADV_NONCONN_IND:
            adv_type = ADV_TYPE_NON_CONN_NON_SCAN;
        break;
        case OHOS_BLE_ADV_DIRECT_IND_LOW:
            adv_type = ADV_TYPE_DIRECT_LDC;
        break;
        default:
            break;
    }
    return adv_type;
}

static uint8_t ohos_io_cap_to_bes(BleIoCapMode mode)
{
    uint8_t bes_io_cap;
    switch(mode)
    {
        case OHOS_BLE_IO_CAP_OUT:
            bes_io_cap = BTM_IO_DISPLAY_ONLY;
            break;
        case OHOS_BLE_IO_CAP_IO:
            bes_io_cap = BTM_IO_DISPLAY_YESNO;
            break;
        case OHOS_BLE_IO_CAP_IN:
            bes_io_cap = BTM_IO_KEYBOARD_ONLY;
            break;
        case OHOS_BLE_IO_CAP_NONE:
            bes_io_cap = BTM_IO_NO_IO;
            break;
        defalut :
            bes_io_cap = BTM_IO_NO_IO;
            break;
    }
    return bes_io_cap;
}

static uint8_t ohos_authreq_to_bes(uint8_t mode)
{
    uint8_t bes_auth;
    switch(mode)
    {
        case OHOS_BLE_AUTH_NO_BOND:
            bes_auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;
            break;
        case OHOS_BLE_AUTH_BOND:
            bes_auth = GAP_AUTH_REQ_NO_MITM_BOND;
            break;
        case OHOS_BLE_AUTH_REQ_MITM:
            bes_auth = GAP_AUTH_REQ_MITM_NO_BOND;
            break;
        case OHOS_BLE_AUTH_REQ_SC_ONLY:
            bes_auth = GAP_AUTH_SEC_CON;
            break;
        case OHOS_BLE_AUTH_REQ_SC_BOND:
            bes_auth = GAP_AUTH_SEC_CON | GAP_AUTH_BOND;
            break;
        case OHOS_BLE_AUTH_REQ_SC_MITM:
            bes_auth = GAP_AUTH_REQ_SEC_CON_NO_BOND;
            break;
        case OHOS_BLE_AUTH_REQ_SC_MITM_BOND:
            bes_auth = GAP_AUTH_REQ_SEC_CON_BOND;
            break;
        defalut :
            bes_auth = GAP_AUTH_REQ_NO_MITM_BOND;
            break;
    }
    return bes_auth;
}

static int general_random_ble_address(BdAddr *local_addr)
{
    if (local_addr == NULL){
        return -1;
    }

    srand(GET_CURRENT_MS());
    for(int i = 0;i< OHOS_BD_ADDR_LEN; i++)
        local_addr->addr[i] = rand()%0x100;
    local_addr->addr[5] &= 0x3F;
    return 0;
}

static ohos_ble_adv_record* BleGetFreeAdvRecord(void)
{
    for(int i = 0; i < HARMONY_ADV_MAX_SIZE; i ++){
        if(g_ohos_adv_record[i].flag == HARMONY_ADV_FREE_FLAG){
            g_ohos_adv_record[i].flag = HARMONY_ADV_OCCUPIED_FLAG;
            return &g_ohos_adv_record[i];
        }
    }

    return NULL;
}

static ohos_ble_adv_record* BleGetAdvRecordByAdvId(int advId)
{
    for(int i = 0; i < HARMONY_ADV_MAX_SIZE; i ++){
        if((g_ohos_adv_record[i].flag == HARMONY_ADV_OCCUPIED_FLAG) &&
                (g_ohos_adv_record[i].advId == advId)){

            return &g_ohos_adv_record[i];
        }
    }

    return NULL;
}

static void BleFreeAdvRecord(int advId)
{
    for(int i = 0; i < HARMONY_ADV_MAX_SIZE; i ++){
        if((g_ohos_adv_record[i].flag == HARMONY_ADV_OCCUPIED_FLAG) &&
                (g_ohos_adv_record[i].advId == advId)){
            g_ohos_adv_record[i].flag = HARMONY_ADV_FREE_FLAG;
            break;
        }
    }
}

static void BleInitAdvRecord(void)
{
    TRACE(1,"%S ...",__func__);
     for(int i = 0; i < HARMONY_ADV_MAX_SIZE; i ++){
        g_ohos_adv_record[i].advId = 0;
        g_ohos_adv_record[i].adv_index = i;
        g_ohos_adv_record[i].flag = HARMONY_ADV_FREE_FLAG;
     }
}

extern unsigned char *bt_get_local_address(void);
extern void app_ble_custom_adv_start_3p(int actv_user);
extern void app_ble_custom_adv_stop_3p(int actv_user);
extern void app_ble_custom_set_adv_data(int actv_user,
                                                                            uint8_t *adv_data,
                                                                            uint8_t adv_data_size,
                                                                            uint8_t *scan_rsp_data,
                                                                            uint8_t scan_rsp_data_size);
void app_ble_custome_set_adv_para(int actv_user,
                                                                                uint8_t *local_addr,
                                                                                uint8_t *peer_addr,
                                                                                uint16_t adv_interval,
                                                                                uint8_t adv_type,
                                                                                int8_t tx_power_dbm);

/**
 * @brief Initializes the Bluetooth protocol stack.
 *
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the Bluetooth protocol stack is initialized;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_InitBtStack(void)
{
    if(!app_is_stack_ready())
    {
        bes_bt_init();
    }
    return OHOS_BT_STATUS_SUCCESS;
}

/**
 * @brief Enables the Bluetooth protocol stack.
 *
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the Bluetooth protocol stack is enabled;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_EnableBtStack(void)
{
    return OHOS_BT_STATUS_SUCCESS;
}

/**
 * @brief Disables the Bluetooth protocol stack.
 *
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the Bluetooth protocol stack is disabled;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_DisableBtStack(void)
{
    return OHOS_BT_STATUS_SUCCESS;
}

/**
 * @brief Sets the Bluetooth device name.
 *
 * @param name Indicates the pointer to the name to set.
 * @param len Indicates the length of the name to set.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the Bluetooth device name is set;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_SetDeviceName(const char *name, unsigned int len)
{
    if(name){
        factory_section_set_bt_name(name,len);  //bt name
        return true;
    }
    return false;
}

/**
 * @brief Sets advertising data.
 *
 * @param advId Indicates the advertisement ID, which is allocated by the upper layer of the advertiser.
 * @param data Indicates the pointer to the advertising data. For details, see {@link BleConfigAdvData}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if advertising data is set;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleSetAdvData(int advId, const BleConfigAdvData *data)
{
    if (data == NULL){
        return -1;
    }

    ohos_ble_adv_record * advRecord = NULL;

    advRecord = BleGetFreeAdvRecord();
    if(advRecord == NULL){
        return -1;
    }

    advRecord->advId = advId;
    app_ble_custom_set_adv_data(advRecord->adv_index,
                                                            (uint8_t*)data->advData,
                                                            data->advLength,
                                                            data->scanRspData,
                                                            data->scanRspLength);
    if(ohos_g_gatt_callback){
        ohos_g_gatt_callback->advDataCb(advId,0);
    }
    return 0;
}

/**
 * @brief Starts advertising.
 *
 * @param advId Indicates the advertisement ID, which is allocated by the upper layer of the advertiser.
 * @param param Indicates the pointer to the advertising parameters. For details, see {@link BleAdvParams}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if advertising is started;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleStartAdv(int advId, const BleAdvParams *param)
{
    if (param == NULL){
        return -1;
    }
    uint16_t adv_interval  = 0;
    ohos_ble_adv_record * advRecord = NULL;
    ohos_ble_addr_t peerAddr;
    BdAddr local_addr;
    BLE_ADV_TYPE_E adv_type;

    advRecord = BleGetAdvRecordByAdvId(advId);
    if(advRecord == NULL){
        return -1;
    }
    general_random_ble_address(&local_addr);

    memcpy(advRecord->local_addr.addr,local_addr.addr,OHOS_BD_ADDR_LEN);
    if(param->peerAddr.addr)
        memcpy(peerAddr.addr,param->peerAddr.addr,OHOS_BD_ADDR_LEN);
    peerAddr.addr_type = param->peerAddrType;
    adv_type = ble_adv_type_harmony_change_to_bes(param->advType);

    adv_interval = (param->minInterval + param->maxInterval)/2;
    app_ble_custome_set_adv_para(advRecord->adv_index,
                                                               local_addr.addr,
                                                               &peerAddr,
                                                               adv_interval,
                                                               adv_type,
                                                               param->txPower);
    app_ble_custom_adv_start_3p(advRecord->adv_index);
    if(ohos_g_gatt_callback){
        ohos_g_gatt_callback->advEnableCb(advId, 0);
    }
    return 0;
}

/**
 * @brief Stops advertising.
 *
 * @param advId Indicates the advertisement ID, which is allocated by the upper layer of the advertiser.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if advertising is stopped;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleStopAdv(int advId)
{
    ohos_ble_adv_record * advRecord = NULL;

    advRecord = BleGetAdvRecordByAdvId(advId);
    if(advRecord == NULL){
        return -1;
    }

    app_ble_custom_adv_stop_3p(advRecord->adv_index);
    BleFreeAdvRecord(advId);

    if(ohos_g_gatt_callback){
        ohos_g_gatt_callback->advDisableCb(advId, 0);
    }
    return 0;
}

/**
 * @brief Updates advertising parameters.
 *
 * @param advId Indicates the advertisement ID, which is allocated by the upper layer of the advertiser.
 * @param param Indicates the pointer to the advertising parameters. For details, see {@link BleAdvParams}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if advertising parameters are updated;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleUpdateAdv(int advId, const BleConfigAdvData *data, const BleAdvParams *param)
{
    if (param == NULL){
        return -1;
    }

    uint16_t adv_interval  = 0;
    ohos_ble_adv_record * advRecord = NULL;
    ohos_ble_addr_t peerAddr;
    BdAddr local_addr;
    BLE_ADV_TYPE_E adv_type;

    advRecord = BleGetAdvRecordByAdvId(advId);
    if(advRecord == NULL){
        return -1;
    }

    if(param->peerAddr.addr)
        memcpy(peerAddr.addr,param->peerAddr.addr,OHOS_BD_ADDR_LEN);
    peerAddr.addr_type = param->peerAddrType;
    adv_type = ble_adv_type_harmony_change_to_bes(param->advType);

    adv_interval = (param->minInterval + param->maxInterval)/2;
    app_ble_custome_set_adv_para(advRecord->adv_index,
                                   advRecord->local_addr.addr,
                                   &peerAddr,
                                   adv_interval,
                                   adv_type,
                                   param->txPower);

    app_ble_custom_adv_start_3p(advRecord->adv_index);

    if(ohos_g_gatt_callback){
        ohos_g_gatt_callback->advUpdateCb(advId, 0);
    }
    return 0;
}

/**
 * @brief Sets the secure I/O capability mode.
 *
 * @param mode Indicates the capability mode to set. For details, see {@link BleIoCapMode}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the capability mode is set;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleSetSecurityIoCap(BleIoCapMode mode)
{
    uint8_t bes_io_cap = ohos_io_cap_to_bes(mode);
    app_sec_set_io_capabilities(bes_io_cap);
    return OHOS_BT_STATUS_SUCCESS;
}

/**
 * @brief Sets the authentication mode for secure connection requests.
 *
 * @param mode Indicates the authentication mode to set. For details, see {@link BleAuthReqMode}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the authentication mode is set;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleSetSecurityAuthReq(BleAuthReqMode mode)
{
    uint8_t authreq = ohos_authreq_to_bes(mode);
    app_sec_set_auth(authreq);
    return OHOS_BT_STATUS_SUCCESS;
}

/**
 * @brief Responds to a secure connection request.
 *
 * @param bdAddr Indicates the address of the device that sends the request.
 * @param accept Specifies whether to accept the request. The value <b>true</b> means to accept the request,
 * and <b>false</b> means to reject the request.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the request is responded to;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleGattSecurityRsp(BdAddr bdAddr, bool accept)
{
    app_set_accept_ble_state(accept);
    return OHOS_BT_STATUS_SUCCESS;
}

static void BleConnReqCallbackHandle(int8_t connId, ble_bdaddr_t *bleAddr)
{
    BdAddr bleaddress;
    memcpy(bleaddress.addr,bleAddr->addr,6);
    if(ohos_g_gatt_callback && ohos_g_gatt_callback->securityRespondCb){
        ohos_g_gatt_callback->securityRespondCb(&bleaddress);
    }
}

static void BleConnDoneCallbackHandle(int8_t connId, ble_bdaddr_t *bleAddr)
{
    BdAddr bleaddress;
    memcpy(bleaddress.addr,bleAddr->addr,6);
}

void BleRegisterConnCallback(void)
{
    app_ble_connect_callback_register(BleConnReqCallbackHandle,BleConnDoneCallbackHandle);
}
/**
 * @brief Obtains the device MAC address.
 *
 * @param mac Indicates the pointer to the device MAC address.
 * @param len Indicates the length of the device MAC address.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the device MAC address is obtained;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_ReadBtMacAddr(unsigned char *mac, unsigned int len)
{
    if (NULL == mac)
        return false;

    memcpy(mac,factory_section_get_bt_address(),len);
    return true;
}

/**
 * @brief Sets scan parameters.
 *
 * @param clientId Indicates the client ID, which is obtained during client registration.
 * @param param Indicates the pointer to the scan parameters. For details, see {@link BleScanParams}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the scan parameters are set;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleSetScanParameters(int clientId, BleScanParams *param)
{
    memcpy(&blescanparamter,param,sizeof(BleScanParams));
    if(ohos_g_gatt_callback){
        ohos_g_gatt_callback->scanParamSetCb(clientId,0);
    }
    return OHOS_BT_STATUS_SUCCESS;
}

/**
 * @brief Starts a scan.
 *
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the scan is started;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleStartScan(void)
{
    app_ble_start_scan(blescanparamter.scanFilterPolicy, blescanparamter.scanWindow, blescanparamter.scanInterval);
    return OHOS_BT_STATUS_SUCCESS;
}

/**
 * @brief Stops a scan.
 *
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the scan is stopped;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleStopScan(void)
{
    app_ble_stop_scan();
    return OHOS_BT_STATUS_SUCCESS;
}

static void ble_scan_result_callback(ble_bdaddr_t *bleAddr,int8_t rssi,uint8_t *adv_buf,uint8_t len)
{
    BtScanResultData scanResultdata;
    memcpy(scanResultdata.addr.addr,bleAddr->addr,OHOS_BD_ADDR_LEN);
    scanResultdata.addrType = bleAddr->addr_type;
    scanResultdata.rssi = rssi;
    scanResultdata.advData = adv_buf;
    scanResultdata.advLen = len;
    if(ohos_g_gatt_callback && ohos_g_gatt_callback->scanResultCb){
        ohos_g_gatt_callback->scanResultCb(&scanResultdata);
    }
}

/**
 * @brief Registers GATT callbacks.
 *
 * @param func Indicates the pointer to the callbacks to register. For details, see {@link BtGattCallbacks}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the GATT callbacks are registered;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleGattRegisterCallbacks(BtGattCallbacks *func)
{
    if (func != NULL) {
        if (func->securityRespondCb != NULL) {
            ohos_g_gatt_callback->securityRespondCb = func->securityRespondCb;
        } else {
            if (func->advDataCb)
                ohos_g_gatt_callback->advDataCb = func->advDataCb;
            if (func->advDisableCb)
                ohos_g_gatt_callback->advDisableCb = func->advDisableCb;
            if (func->advEnableCb)
                ohos_g_gatt_callback->advEnableCb = func->advEnableCb;
            if (func->advUpdateCb)
                ohos_g_gatt_callback->advUpdateCb = func->advUpdateCb;
            if (func->scanParamSetCb)
                ohos_g_gatt_callback->scanParamSetCb = func->scanParamSetCb;
            if (func->scanResultCb)
                ohos_g_gatt_callback->scanResultCb = func->scanResultCb;
            BleInitAdvRecord();
            app_ble_custom_init();
            app_ble_adv_report_callback_register(ble_scan_result_callback);
        }
    }
    return 0;
}

/**
 * @brief Sets advertising data and parameters and starts advertising.
 *
 * This function is available for system applications only. \n
 *
 * @param advId Indicates the pointer to the advertisement ID.
 * @param rawData Indicates the advertising data. For details, see {@link StartAdvRawData}.
 * @param advParam Indicates the advertising parameters. For details, see {@link BleAdvParams}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the operation is successful;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int OHOS_BleStartAdvEx(int *advId, const StartAdvRawData rawData, BleAdvParams advParam)
{
    uint32_t duration;
    if (!adv_enable_timer_id) {
        adv_enable_timer_id = osTimerCreate(osTimer(adv_enable_timer), osTimerOnce, &adv_enable_para);
    }
    adv_enable_para = *advId;
    BleConfigAdvData advData;
    advData.advData = rawData.advData;
    advData.advLength = rawData.advDataLen;
    advData.scanRspData = rawData.rspData;
    advData.scanRspLength = rawData.rspDataLen;
    duration = advParam.duration;

    OHOS_BleSetAdvData(*advId, &advData);
    OHOS_BleStartAdv(*advId, &advParam);

    if(duration != 0) {
        if (adv_timer_run_state == ADV_TIMER_IDLE) {
            osTimerStart(adv_enable_timer_id, duration);
            adv_timer_run_state == ADV_TIMER_RUNNING;
        }
        else {
            osTimerStop(adv_enable_timer_id);
            osTimerStart(adv_enable_timer_id, duration);
        }
    }
    else {
        if (adv_timer_run_state == ADV_TIMER_RUNNING) {
            osTimerStop(adv_enable_timer_id);
            adv_timer_run_state == ADV_TIMER_IDLE;
        }
    }
    return OHOS_BT_STATUS_SUCCESS;
}
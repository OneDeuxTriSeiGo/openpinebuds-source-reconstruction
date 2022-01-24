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

#include "string.h"
#include "stdbool.h"
#include "ohos_bt_gap.h"
#include "bt_if.h"
#include "cmsis_os.h"
#include "harmony_utils.h"
#include "ohos_bt_gap.h"
#include "nvrecord_extension.h"
#include "nvrecord_env.h"
#include "nvrecord_bt.h"
#include "me_api.h"
#include "btapp.h"
#include "app_bt.h"
#include "app_bt_func.h"
#ifdef IBRT
#include "app_tws_ibrt.h"
#include "app_ibrt_if.h"
#include "app_tws_ibrt_conn_api.h"
#include "app_ibrt_customif_ui.h"
#include "app_ibrt_conn_evt.h"
#include "app_ibrt_nvrecord.h"
#include "app_ibrt_customif_cmd.h"
#include "app_tws_ctrl_thread.h"
#include "ibrt_mgr.h"
#endif
#include "private_vendor_api.h"
#include "bt_adaptor_dbg.h"
#include "factory_section.h"
#define SCAN_TIMER_IDLE             0
#define SCAN_TIMER_RUNNING          1
extern bool app_is_stack_ready(void);
extern void bes_bt_init(void);
GapBtScanMode bt_scan_mode = OHOS_GAP_SCAN_MODE_NONE;

static unsigned int bt_ble_switch = 0;
extern uint8_t btif_global_addr[OHOS_BD_ADDR_LEN];
BtGapCallBacks *ohos_g_gap_cb_func = NULL;
static GapBtPairedState g_BtPairedState = OHOS_GAP_PAIR_NONE;
static VENDOR_BT_GAP_IF_INFO_T bt_gap_if_env_ctx = {0};
static void scan_mode_timer_handler(void const *param);
osTimerDef(scan_mode_timer, scan_mode_timer_handler);
static osTimerId scan_mode_timer_id = NULL;
uint8_t ohos_timer_run_state = SCAN_TIMER_IDLE;

static uint8_t ohos_scanmode_switch_to_bes(uint8_t scanMode)
{
    uint8_t bes_mode;
    switch(scanMode)
    {
        case OHOS_GAP_SCAN_MODE_NONE:
            bes_mode = BTIF_BAM_NOT_ACCESSIBLE;
        break;
        case OHOS_GAP_SCAN_MODE_CONNECTABLE:
            bes_mode = BTIF_BAM_CONNECTABLE_ONLY;
        break;
        case OHOS_GAP_SCAN_MODE_GENERAL_DISCOVERABLE:
            bes_mode = BTIF_BAM_DISCOVERABLE_ONLY;
        break;
        case OHOS_GAP_SCAN_MODE_LIMITED_DISCOVERABLE:
            bes_mode = BTIF_BAM_DISCOVERABLE_ONLY;
        break;
        case OHOS_GAP_SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE:
            bes_mode = BTIF_BAM_GENERAL_ACCESSIBLE;
        break;
        case OHOS_GAP_SCAN_MODE_CONNECTABLE_LIMITED_DISCOVERABLE:
            bes_mode = BTIF_BAM_GENERAL_ACCESSIBLE;
        break;
        default:
            bes_mode = BTIF_BAM_NOT_ACCESSIBLE;
            break;
    }
    return bes_mode;
}

static uint8_t bes_scanmode_switch_to_ohos(uint8_t scanMode)
{
    uint8_t bes_mode;
    switch(scanMode)
    {
        case BTIF_BAM_NOT_ACCESSIBLE:
            bes_mode = OHOS_GAP_SCAN_MODE_NONE;
        break;
        case BTIF_BAM_CONNECTABLE_ONLY:
            bes_mode = OHOS_GAP_SCAN_MODE_CONNECTABLE;
        break;
        case BTIF_BAM_DISCOVERABLE_ONLY:
            bes_mode = OHOS_GAP_SCAN_MODE_GENERAL_DISCOVERABLE;
        break;
        case BTIF_BAM_GENERAL_ACCESSIBLE:
            bes_mode = OHOS_GAP_SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE;
        break;
        default:
            bes_mode = BTIF_BAM_NOT_ACCESSIBLE;
            break;
    }
    return bes_mode;
}

static void set_bt_scan_limited_mode(int mode, int duration)
{
    static uint8_t timer_init_flag = 0;
    if (!timer_init_flag) {
        timer_init_flag = 1;
        scan_mode_timer_id = osTimerCreate(osTimer(scan_mode_timer), osTimerOnce, NULL);
    }
    switch(mode)
    {
        case OHOS_GAP_SCAN_MODE_LIMITED_DISCOVERABLE:
        case OHOS_GAP_SCAN_MODE_CONNECTABLE_LIMITED_DISCOVERABLE:
            if (scan_mode_timer_id != NULL) {
                if (ohos_timer_run_state == SCAN_TIMER_IDLE){
                    osTimerStart(scan_mode_timer_id, duration);
                    ohos_timer_run_state = SCAN_TIMER_RUNNING;
                }
                else{
                    osTimerStop(scan_mode_timer_id);
                    osTimerStart(scan_mode_timer_id, duration);
                }
            }
        break;
        default:
            if (ohos_timer_run_state == SCAN_TIMER_RUNNING){
                osTimerStop(scan_mode_timer_id);
                ohos_timer_run_state = SCAN_TIMER_IDLE;
            }
        break;
    }
}

/**
 * @brief Enable ble.
 *
 * @return Returns <b>true</b> if the operation is accepted;
 *         returns <b>false</b> if the operation is rejected.
 */
bool OHOS_EnableBle(void)
{
    if (bt_ble_switch == 0)
    {
        if(!app_is_stack_ready())
        {
            bes_bt_init();
        }
        bt_ble_switch = 1;
        return true;
    }
    return false;
}

/**
 * @brief Disable ble.
 *
 * @return Returns <b>true</b> if the operation is accepted;
 *         returns <b>false</b> if the operation is rejected.
 */
bool OHOS_DisableBle(void)
{
    if (bt_ble_switch == 1)
    {
        bt_ble_switch = 0;
        return true;
    }
    return false;
}

/**
 * @brief Enable classic.
 *
 * @return Returns <b>true</b> if the operation is accepted;
 *         returns <b>false</b> if the operation is rejected.
 */
bool OHOS_EnableBt(void)
{
    LOG_I("%s", __func__);
#ifdef IBRT
    ibrt_mgr_update_scan_type_policy(IBRT_OPEN_BOX_EVT);
    SetBtScanMode(OHOS_GAP_SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE, 0);
#endif
    bt_gap_if_env_ctx.ux_bluetooth_enable = true;
    if(ohos_g_gap_cb_func && ohos_g_gap_cb_func->stateChangeCallback){
        ohos_g_gap_cb_func->stateChangeCallback(OHOS_BT_TRANSPORT_DUAL_MONO,OHOS_GAP_STATE_TURN_ON);
    }
    return true;
}

/**
 * @brief Disable classic.
 *
 * @return Returns <b>true</b> if the operation is accepted;
 *         returns <b>false</b> if the operation is rejected.
 */
bool OHOS_DisableBt(void)
{
    LOG_I("%s", __func__);
#ifdef IBRT
    ibrt_mgr_update_scan_type_policy(IBRT_CLOSE_BOX_EVT);
    SetBtScanMode(OHOS_GAP_SCAN_MODE_NONE, 0);
#else
    btif_me_set_accessible_mode(BTIF_BAM_NOT_ACCESSIBLE,NULL);
#endif
    bt_gap_if_env_ctx.ux_bluetooth_enable = false;
    if(ohos_g_gap_cb_func && ohos_g_gap_cb_func->stateChangeCallback){
        ohos_g_gap_cb_func->stateChangeCallback(OHOS_BT_TRANSPORT_DUAL_MONO,OHOS_GAP_STATE_TURN_OFF);
    }
    return true;
}

/**
 * @brief Get classic enable/disable state.
 *
 * @return Returns classic enable/disable state, {@link BtStackState}.
 */
int OHOS_GetBtState()
{
    return bt_ble_switch;
}

/**
 * @brief Get ble enable/disable state.
 *
 * @return Returns <b>true</b> if ble is enabled;
 *         returns <b>false</b> if ble is not enabled.
 */
bool OHOS_IsBleEnabled()
{
    return bt_ble_switch;
}

/**
 * @brief Get local host bluetooth address.
 *
 * @return Returns <b>true</b> if the operation is accepted;
 *         returns <b>false</b> if the operation is rejected.
 */
bool OHOS_GetLocalAddr(unsigned char *mac, unsigned int len)
{
    uint8_t *bt_addr = factory_section_get_bt_address();
    if (!bt_addr) {
        bt_addr = btif_global_addr;
    }
    if (len == OHOS_BD_ADDR_LEN)
    {
        memcpy(mac,bt_addr,len);
        LOG_I("%s",__func__);
        return true;
    }
    else
    {
        return false;
    }

}
/**
 *  @brief Get local host bluetooth name
 *  @param localName actual receiving device name
           length - localName length, initail set length to zero, and call this func to set real length
 *  @return Local host bluetooth name
 */
bool OHOS_GetLocalName(unsigned char *localName, unsigned char *length)
{
    uint8_t* l_name = NULL;
     uint8_t * bt_addr = NULL;
#ifdef IBRT
     ibrt_ctrl_t *p_ibrt_ctrl = app_tws_ibrt_get_bt_ctrl_ctx();
    if (length == NULL || localName == NULL || p_ibrt_ctrl == NULL){
        return false;
    }

    bt_addr = factory_section_get_bt_address();
    if (bt_addr == NULL) {
        LOG_W("factory_section_get_bt_address bt_addr is null");
        return false;
    }
    //factory_address != local_addr in tws mode
    if(memcmp(bt_addr, p_ibrt_ctrl->local_addr.address, 6)){
        struct nvrecord_env_t *nvrecord_env = NULL;
        nv_record_env_get(&nvrecord_env);

        LOG_I("in tws mode get tws_name");
        if(nvrecord_env){
            *length =  nvrecord_env->ibrt_mode.tws_name_len;
            memcpy(localName, nvrecord_env->ibrt_mode.tws_name, *length);
        }else{
            return false;
        }
    }else{
        l_name = factory_section_get_bt_name();
         if(l_name){
            *length = strlen(l_name);
            memcpy(localName, l_name, *length);
        }else
            return false;
    }
#endif
    LOG_I("%s localname:%s", __func__, localName);
    return true;
}
/**
 * @brief Set local device name.
 *
 * @param localName Device name.
 * @param length localName length, The maximum length of the name is {@link OHOS_BD_NAME_LEN}.
 * @return Returns <b>true</b> if the operation is successful;
 *         returns <b>false</b> if the operation fails.
 */
bool OHOS_SetLocalName(unsigned char *localName, unsigned char length)
{
    if (localName == NULL){
        return false;
    }
    LOG_I("%s localname:%s", __func__, localName);
    bt_set_local_dev_name((const unsigned char*)localName, strlen(localName) + 1);
    bt_stack_config((const unsigned char*)localName, strlen(localName) + 1);
    if(ohos_g_gap_cb_func && ohos_g_gap_cb_func->deviceNameChangedCallback){
        ohos_g_gap_cb_func->deviceNameChangedCallback(localName,strlen(localName) + 1);
    }

    factory_section_set_bt_name(localName, length + 1);
#ifdef IBRT
    app_tws_ibrt_sync_tws_name(localName, length + 1);
#endif
    return true;
}
/**
 * @brief Factory reset bluetooth service.
 * @return Returns <b>true</b> if the operation is successful;
 *         returns <b>false</b> if the operation fails.
 */
bool OHOS_BluetoothFactoryReset(void)
{
    LOG_I("%s", __func__);
    bt_bdaddr_t invalid_addr = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    nv_record_update_ibrt_info(STEREO_NONE, &invalid_addr);
    nv_record_clean_tws_name();
    nv_record_ddbrec_clear_all_bt_paired_list();
#ifdef IBRT
    app_ibrt_if_config_keeper_clear();
#endif
    nv_record_flash_flush();
    LOG_I("doing factory finished");
}

/**
 * @brief Set device scan mode.
 * @param mode Scan mode.
 * @param duration Scan time, see details {@link GapBtScanMode}
 * @return special mode
 */
int OHOS_GetBtScanMode(void)
{
    return bt_scan_mode;

}
#ifdef IBRT
void ohos_bt_scan_mode_handler(int scanMode, int duration)
{
    if (scanMode == BTIF_BAM_NOT_ACCESSIBLE)
    {
        ibrt_mgr_update_scan_type_policy(IBRT_EXIT_PAIR_MODE_EVT);
        ibrt_mgr_update_scan_type_policy(IBRT_CLOSE_BOX_EVT);
    }
    else
    {
        ibrt_mgr_update_scan_type_policy(IBRT_OPEN_BOX_EVT);
        if (scanMode == BTIF_BAM_CONNECTABLE_ONLY)
        {
            ibrt_mgr_update_scan_type_policy(IBRT_EXIT_PAIR_MODE_EVT);
        }
        else
        {
            ibrt_mgr_update_scan_type_policy(IBRT_ENTER_PAIR_MODE_EVT);
        }
    }
}
#endif

/**
 * @brief Set device scan mode.
 * @param mode Scan mode, see details {@link GapBtScanMode}.
 * @param duration Scan time.
 * @return Returns <b>true</b> if the operation is successful;
 *         returns <b>false</b> if the operation fails.
 */
bool OHOS_SetBtScanMode(int mode, int duration)
{
    LOG_I("%s mode=%d duration=%d", __func__, mode, duration);
    const btif_access_mode_info_t info = {0x800, 0x12, 0x800, 0x12};
    uint8_t status = BT_STS_SUCCESS;
    uint8_t scanMode = 0;
    scanMode = ohos_scanmode_switch_to_bes(mode);

#ifdef IBRT
    if (app_tws_ibrt_tws_link_connected() && (app_ibrt_conn_get_ui_role() == TWS_UI_SLAVE))
    {
        ibrt_sync_bt_access_mode_t bt_access_mode = {0};
        bt_access_mode.ux_bt_scan_mode = scanMode;
        bt_access_mode.ux_scan_duration = (uint32_t)duration;
        if (tws_ctrl_send_cmd(APP_TWS_CMD_SYNC_SCAN_MODE, (uint8_t*)&bt_access_mode, sizeof(bt_access_mode)) != 0)
        {
            ohos_bt_scan_mode_handler(scanMode, duration);
        }
    }
    else
    {
        ohos_bt_scan_mode_handler(scanMode, duration);
    }
    return true;
#else
    set_bt_scan_limited_mode(mode, duration);
    status = btif_me_set_accessible_mode(scanMode,&info);
    if(status == BT_STS_SUCCESS)
    {
        if(ohos_g_gap_cb_func && ohos_g_gap_cb_func->scanModeChangedCallback)
        {
            ohos_g_gap_cb_func->scanModeChangedCallback(bt_scan_mode);
        }
        return true;
    }
    else
    {
        return (bes_status_to_ohos(status) == OHOS_BT_STATUS_SUCCESS)?true:false;
    }
#endif
}

/*
 * @brief Get paired devices.
 * @param pairList - 按照maxPairNums申请的设备列表数�?          maxPairNums - 指定需要获取的设备列表最大个�?          realPairNums - 实际的配对设备列表个�? * @return Returns <b>true</b> if the operation is successful;
 *         returns <b>false</b> if the operation fails.
 */
bool OHOS_GetPariedDevicesNum(unsigned int *number)
{
    *number = nv_record_get_paired_dev_count();
    LOG_I("%s paried num:%d", __func__, *number);
    return true;
}

bool OHOS_GetPairedDevicesList(PairedDeviceInfo *devInfo, int *number)
{
    LOG_I("%s", __func__);
    btif_device_record_t record;
    bt_status_t status = BT_STS_SUCCESS;
    *number = nv_record_get_paired_dev_count();
    for(int i=0; i<*number; i++)
    {
        status = nv_record_enum_dev_records(i, &record);
        if(status == BT_STS_SUCCESS){
            memcpy(devInfo[i].addr.addr, record.bdAddr.address,OHOS_BD_ADDR_LEN);
#ifdef LINK_KEY_ENCRYPT_BY_CUSTOMER
            memcpy(devInfo[i].deviceName, record.remote_name,OHOS_BD_NAME_LEN);
#endif
        }
    }
    return true;
}

/**
 * @brief Get device pair state.
 * @param device Remote device.
 * @return Returns device pair state. see detail {@link GapBtPairedState}
 */
int OHOS_GetPairState()
{
    return g_BtPairedState;
}

/**
 * @brief Remove pair.
 * @param BdAddr Remote device address.
 * @return Returns <b>true</b> if the operation is successful;
 *         returns <b>false</b> if the operation fails.
 */
bool OHOS_RemovePair(const BdAddr addr)
{
    LOG_I("%s addr:%02x %02x %02x", __func__);
    nv_record_ddbrec_delete((bt_bdaddr_t*)&addr);
    nv_record_flash_flush();
    return true;
}

/**
 * @brief Remove all pairs.
 * @return Returns <b>true</b> if the operation is successful;
 *         returns <b>false</b> if the operation fails.
 */
bool OHOS_RemoveAllPairs(void)
{
    LOG_I("doing factory reset");
    //if ibrt connected, need stop ibrt and disconnect tws link firstly.
#ifdef IBRT
    if (app_tws_ibrt_tws_link_connected())
    {
        ExitStereo();
        osDelay(2000);
    }
#endif
    app_bt_call_func_in_bt_thread(0, 0, 0, 0, (uint32_t)OHOS_BluetoothFactoryReset);
    return true;
}
/**
 * @brief Check if device acl connected.
 * @param addr device address.
 * @return Returns <b>true</b> if device acl connected;
 *         returns <b>false</b> if device does not acl connect.
 */
bool OHOS_IsAclConnected(BdAddr addr)
{
    struct BT_DEVICE_T *curr_device = NULL;
    for(int i=0;i<BT_DEVICE_NUM;i++)
    {
        curr_device = app_bt_get_device(i);
        if (curr_device)
        {
            if(!memcmp(curr_device->remote.address,addr.addr,OHOS_BD_ADDR_LEN))
                return curr_device->acl_is_connected;
        }
    }
    return false;
}

/**
 * @brief disconnect remote device all profile.
 * @param addr device address.
 * @return Returns <b>true</b> if device acl connected;
 *         returns <b>false</b> if device does not acl connect.
 */
bool OHOS_DisconnectRemoteDevice(BdAddr *addr)
{
    if (addr)
    {
        LOG_I("%s addr:%02x %02x %02x", __func__, addr->addr[0], addr->addr[1], addr->addr[2]);
    }
#ifdef IBRT
    app_tws_ibrt_disconnect_mobile((bt_bdaddr_t *)addr);
#endif
    return true;
}

/**
 * @brief Adapter state change function. 开关蓝牙的callback上报
 * @param transport Transport type when state change.
 *        OHOS_BT_TRANSPORT_BR_EDR : classic;
 *        OHOS_BT_TRANSPORT_LE : ble.
 *        detail see {@link BtTransportId}
 * @param state Change to the new state
 *        details see {@link BtStackState}.
 */
static void bt_pair_state_change(bt_bdaddr_t* addr, uint32_t para)
{
    if(ohos_g_gap_cb_func && ohos_g_gap_cb_func->pairStatusChangedCallback){
        ohos_g_gap_cb_func->pairStatusChangedCallback((BdAddr *)addr,para);
    }
    g_BtPairedState = para;
}

int OHOS_GapRegisterCallbacks(BtGapCallBacks *func)
{
    LOG_I("%s", __func__);
    ohos_g_gap_cb_func = func;
    //TODO: move pairing state to event_function_cbs table
    app_bt_pair_callback_register(bt_pair_state_change);
    return 0;
}
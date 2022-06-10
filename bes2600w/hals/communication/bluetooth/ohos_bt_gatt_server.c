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
#include "plat_types.h"
#include "hal_trace.h"
#include "ohos_bt_gatt.h"
#include "ohos_bt_gatt_server.h"
#include "harmony_utils.h"
#include "me_api.h"
#include "cmsis_os.h"
#include "app_ble_mode_switch.h"
#include "app_task.h"
#include "gap.h"
#include "ohos_bt_gatt_server.h"
#include "bleadapter_def.h"
#include "bes_gatts_adapter.h"
#include "bleadapter_utils.h"
#include "ohos_bt_def.h"


#include "app_ble_customif.h"

#define BTM_BONDING_NOT_REQUIRED 0x00
#define BTM_DEDICATED_BONDING    0x02
#define BTM_GENERAL_BONDING      0x04
#define BTM_BONDING_NOT_ALLOWED  0x10

#define BTM_AUTH_MITM_PROTECT_NOT_REQUIRED  0x00
#define BTM_AUTH_MITM_PROTECT_REQUIRED      0x01

#define BTM_IO_DISPLAY_ONLY   0
#define BTM_IO_DISPLAY_YESNO  1
#define BTM_IO_KEYBOARD_ONLY  2
#define BTM_IO_NO_IO          3

#define ADV_TIMER_IDLE             0
#define ADV_TIMER_RUNNING          1

#define GATT_SHORT_UUID_LEN    2
#define MAX_UUID_LEN           16

#define MAX_GATT_ATT_NUM        8
typedef struct {
    /** attribute start hanlde */
    int32_t start_hdl;
    /** attribute end hanlde */
    int32_t end_hdl;
    /** attribute hanlde */
    int32_t handle;
    /** attribute list */
    BleGattAttr attrList;
} BleGattCharact;
static BtGattServerCallbacks * ohos_g_gattServ_cb = NULL;
#define GATT_DECL_INCLUDED_SERVICE       { 0x02, 0x28 }
BleGattCharact ohos_g_gatt_att[10] = {0};
static char g_gatt_connect_flag = 0;
static char g_gattPrimaryService[GATT_SHORT_UUID_LEN] =        { 0x00, 0x28 };
static char g_gattCharacteristicUuid[GATT_SHORT_UUID_LEN] =    { 0x03, 0x28 };
static char g_gattCharacteristicCfgUuid[GATT_SHORT_UUID_LEN] = { 0x02, 0x29 };


static unsigned char BesGattServerSwapProp(int prop)
{
    unsigned char bes_prop = 0;

    if(prop & OHOS_GATT_CHARACTER_PROPERTY_BIT_BROADCAST)
        bes_prop|= ADAPTER_BLE_PROP_BROADCAST;

    if(prop & OHOS_GATT_CHARACTER_PROPERTY_BIT_READ)
        bes_prop|= ADAPTER_BLE_PROP_READ;

    if(prop & OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP)
        bes_prop|= ADAPTER_BLE_PROP_WRITE_NO_RSP;

    if(prop & OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE)
        bes_prop|= ADAPTER_BLE_PROP_WRITE;

    if(prop & OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY)
        bes_prop|= ADAPTER_BLE_PROP_NOTIFY;

    if(prop & OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE)
        bes_prop|= ADAPTER_BLE_PROP_INDICATE;

    if(prop & OHOS_GATT_CHARACTER_PROPERTY_BIT_SIGNED_WRITE)
        bes_prop|= ADAPTER_BLE_PROP_WRITE_SIGNED;

    return bes_prop;
}

static unsigned char BesGattServerSwapPerm(unsigned char perm)
{
    unsigned char bes_perm = 0;

    if(perm & OHOS_GATT_PERMISSION_READ)
        bes_perm |= ADAPTER_BLE_PERM_READ;

    if(perm & OHOS_GATT_PERMISSION_READ_ENCRYPTED)
        bes_perm |= ADAPTER_BLE_PERM_READ_ENCRYPTED;

    if(perm & OHOS_GATT_PERMISSION_READ_ENCRYPTED_MITM)
        bes_perm |= ADAPTER_BLE_PERM_READ_ENCRYPTED_MITM;

    if(perm & OHOS_GATT_PERMISSION_WRITE)
        bes_perm |= ADAPTER_BLE_PERM_WRITE;

    if(perm & OHOS_GATT_PERMISSION_WRITE_ENCRYPTED)
        bes_perm |= ADAPTER_BLE_PERM_WRITE_ENCRYPTED;

    if(perm & OHOS_GATT_PERMISSION_WRITE_ENCRYPTED_MITM)
        bes_perm |= ADAPTER_BLE_PERM_WRITE_ENCRYPTED_MITM;

    return bes_perm;
}
static uint8_t ohos_Encryption_levle_to_bes(BleSecAct secAct)
{
    uint8_t encryptLevel;
    switch(secAct)
    {
        case OHOS_BLE_SEC_NONE:
            encryptLevel = GAP_AUTH_NONE;
            break;
        case OHOS_BLE_SEC_ENCRYPT:
            encryptLevel = GAP_AUTH_SEC_CON;
            break;
        case OHOS_BLE_SEC_ENCRYPT_NO_MITM:
            encryptLevel = (GAP_AUTH_SEC_CON | GAP_AUTH_BOND);
            break;
        case OHOS_BLE_SEC_ENCRYPT_MITM:
            encryptLevel = (GAP_AUTH_SEC_CON | GAP_AUTH_MITM | GAP_AUTH_BOND);
            break;
        default:
            encryptLevel = (GAP_AUTH_SEC_CON | GAP_AUTH_MITM | GAP_AUTH_BOND);
            break;
    }
    return encryptLevel;
}

BleGattAttr *find_attlist_according_handle(int32_t handle)
{
    for(int i=0; i<10; i++)
    {
        if(ohos_g_gatt_att[i].handle == handle){
            BTA_TRACE(2, "%s:ohos_g_gatt_att[%d].attrList:%p\n", __func__, i, ohos_g_gatt_att[i].attrList.func.write);
            return &ohos_g_gatt_att[i].attrList;
        }
    }
    return NULL;
}

int32_t ohos_att_handle_switch_to_bes(int32_t handle)
{
    int32_t offset_val = 1,g_handle;
    for(int i=0; i<10; i++)
    {
        if((ohos_g_gatt_att[i].start_hdl < handle) && (ohos_g_gatt_att[i].end_hdl > handle)){
            g_handle = ohos_g_gatt_att[i].handle - offset_val;
            if(g_handle == handle){
                return ohos_g_gatt_att[i].handle;
            }
            offset_val += 1;
        }
    }
    return 0;
}

#if  1// for test  call back funcs should be define in uper layler
/**
 * @brief Inform the gatt server init result
 * @param[in] serverIf Indicates id of server interface
 * @return void
 */
static void BesGattServerInitCallback(int32_t serverIf)
{
    BTA_TRACE(1, "%s %d", __func__, serverIf);
}

static void BesGattServerConnCreatedCallback(int32_t serverIf,
                                                                            uint8_t* addr,
                                                                            int connId)
{
    BdAddr ble_addr;

    memcpy(&ble_addr, addr, OHOS_BD_ADDR_LEN);
    if (g_gatt_connect_flag == 0)
    {
        g_gatt_connect_flag = 1;
        if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->connectServerCb)) {
            ohos_g_gattServ_cb->connectServerCb(connId,
                                                                    serverIf,
                                                                &   ble_addr);
        }
        BTA_TRACE(1, "%s %d %d", __func__, serverIf, connId);
    }
}

static void BesGattServerDisconnectCallback(int32_t serverIf, uint8_t* addr, int connId)
{
    BdAddr ble_addr;

    memcpy(&ble_addr, addr, OHOS_BD_ADDR_LEN);
    if (g_gatt_connect_flag == 1)
    {
        g_gatt_connect_flag = 0;
        if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->disconnectServerCb)){
            ohos_g_gattServ_cb->disconnectServerCb(connId,
                                              serverIf,
                                              &ble_addr);
        }
        BTA_TRACE(1, "%s %d %d", __func__, serverIf, connId);
    }
}

/**
 * @brief Callback invoked in response to create_service
 * @param[in] serverIf Service parameter
 * @param[in] srvc_handle Service parameter
 * @return void
 */
static void BesGattServerAddServiceCallback(int status, int serverId, BtUuid *uuid, int srvcHandle)
{
    BTA_TRACE(1, "%s %d", __func__, srvcHandle);
    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->serviceAddCb)) {
        ohos_g_gattServ_cb->serviceAddCb(status,serverId,uuid,srvcHandle);
    }
}

/**
 * @brief Callback invoked when a characteristic has been added to a service
 * @param[in] bt_gatts_add_char Characteristic parameter(server if, uuid ,
 *                                      service handle and characteristic handle)
 * @return void
 */
static void BesGattServerAddCharCallback(bes_gatts_add_char_para_t *bt_gatts_add_char )
{
    BTA_TRACE(1, "%s %d", __func__, bt_gatts_add_char->server_if);
}

/**
 * @brief Callback invoked when a descriptor has been added to a characteristic
 * @param[in] bt_gatts_add_desc Descriptor parameter(server if, uuid ,
 *                                      service handle and descriptor handle)
 * @return void
 */
static void BesGattServerAddDescCallback(bes_gatts_add_desc_para_t *bt_gatts_add_desc)
{
    BTA_TRACE(1, "%s %d hd%x", __func__, bt_gatts_add_desc->server_if, bt_gatts_add_desc->srvc_handle);
}

/**
 * @brief Callback invoked when a remote device has requested to write to a characteristic or descriptor
 * @param[in] bt_gatts_req_write (connection id,
 *                              transaction id,
 *                              remote device address,
 *                              attribute handle,
 *                              offset,
 *                              length,
 *                              need response flag,
 *                              data of value)
 * @return void
 */
static uint16_t BesGattServerReqWriteCallback(int32_t server_if,
                                                                    int32_t conn_id,
                                                                    uint16_t task_id,
                                                                    int32_t handle,
                                                                    int32_t offset,
                                                                    int32_t length,
                                                                    uint8_t * data)
{
    uint8_t * ble_addr;
    BtReqWriteCbPara writeCbPara;
    BleGattAttr *gatt_attr;
    bes_adapter_ble_service_db_t* service_db;

    ble_addr = btadapter_ble_get_addr_by_conidx(conn_id);

    writeCbPara.connId = conn_id;
    writeCbPara.bdAddr = ble_addr;
    writeCbPara.attrHandle = handle;
    writeCbPara.isPrep = false;
    writeCbPara.needRsp = true;
    writeCbPara.offset = offset;
    writeCbPara.length = length;
    writeCbPara.value = data;
    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->requestWriteCb)) {
        ohos_g_gattServ_cb->requestWriteCb(writeCbPara);
    }

    gatt_attr = find_attlist_according_handle(handle);
    if(gatt_attr && gatt_attr->func.write){
        gatt_attr->func.write(data,length);
    } else {
        TRACE(1, "%s no write cb", __func__);
    }
    service_db = btadapter_ble_service_get_service_by_if(server_if);
    btadapter_ble_service_send_gatt_write_response(conn_id,
                                                                                        service_db,
                                                                                        BES_ATT_ERR_NO_ERROR);

    BTA_TRACE(1, "%s %d hd:%x", __func__, server_if, handle);
    return BES_BLE_ERR_NO_ERROR;
}

static void BesGattServerNTFCallback(int32_t server_if,
                                                                    int32_t conn_id,
                                                                    uint16_t task_id,
                                                                    uint16_t status)
{
    uint8_t * ble_addr;
    BtReqWriteCbPara writeCbPara;

    ble_addr = btadapter_ble_get_addr_by_conidx(conn_id);

    BTA_TRACE(1, "%s %d conn_id:%d", __func__, server_if, conn_id);
    return BES_BLE_ERR_NO_ERROR;
}


/**
  * @brief Callback invoked when a remote device has requested to read a characteristic or descriptor
  * @param[in] bt_gatts_req_read (connection id,
  *                              transaction id,
  *                              remote device address,
  *                              attribute handle,
  *                              offset,
  *                              is long or not)
  * @return void
  */
 static void BesGattServerReqReadCallback(int32_t server_if,
                                                                    int32_t conn_id,
                                                                    uint16_t task_id,
                                                                    int32_t handle,
                                                                    int32_t offset)
{
    uint8_t * ble_addr;
    BleGattAttr *gatt_attr;
    BtReqReadCbPara readCbPara;
    uint16_t status = BES_ATT_ERR_NO_ERROR;
    bes_adapter_attm_desc_t *att_elem = NULL;
    ble_addr = btadapter_ble_get_addr_by_conidx(conn_id);

    readCbPara.connId = conn_id;
    readCbPara.bdAddr = ble_addr;
    readCbPara.attrHandle = handle;
    readCbPara.offset = offset;
    readCbPara.transId = 1;
    readCbPara.isLong = false;

    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->requestReadCb)) {
        ohos_g_gattServ_cb->requestReadCb(readCbPara);
    }
    bes_adapter_ble_service_db_t * service_db = btadapter_ble_service_get_service_by_if(server_if);
    if(service_db){
        att_elem= btadapter_ble_service_get_att_db_by_handle(service_db, handle);
        if(!att_elem){
            status = BES_BLE_ERR_INSUFFICIENT_RESOURCES;
            btadapter_ble_service_send_gatt_get_response(service_db,
                                                                                                conn_id,
                                                                                                handle,
                                                                                                NULL,
                                                                                                0,
                                                                                                status);        
        }else{
             btadapter_ble_service_send_gatt_get_response(service_db,
                                                                                                conn_id,
                                                                                                handle,
                                                                                                att_elem->data,
                                                                                                2,
                                                                                                status);
        }
    }
    gatt_attr = find_attlist_according_handle(handle);
    if(gatt_attr && gatt_attr->func.read){
        TRACE(1, "%s %p", __func__, gatt_attr);
        gatt_attr->func.read(NULL,NULL);
    } else {
        TRACE(1, "%s no READ cb", __func__);
    }

    BTA_TRACE(2, "%s %d hd:%x", __func__, server_if, handle);
}

/**
 * @brief Callback invoked when GATT Service is started
 * @return void
 */
static void BesGattServerStartServerCallback(int32_t server_if)
{
    BTA_TRACE(1, "%s %d", __func__, server_if);
}

/**
 * @brief Callback invoked when GATT Service is stop
 * @return void
 */
static void BesGattServerStopServerCallback(int32_t server_if)
{
    BTA_TRACE(1, "%s %d", __func__, server_if);
}

/**
 * @brief Callback invoked when GATT Service is delete
 * @return void
 */
static void BesGattServerDeleteServerCallback(int32_t server_if)
{
    BTA_TRACE(1, "%s %d", __func__, server_if);
}

static void BesGattServerMtuExchangeCallback(uint8_t conidx, uint16_t mtu)
{
    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->mtuChangeCb)) {
        ohos_g_gattServ_cb->mtuChangeCb(conidx, mtu);
    }
    BTA_TRACE(1, "%s conidx:%d mtu:%d", __func__, conidx, mtu);
}


static void BesGattServerRegisterCallback(void)
{
    bes_gatts_state_callback_t gattsCb;

    gattsCb.connCreateCb = BesGattServerConnCreatedCallback;
    gattsCb.disconnectCb = BesGattServerDisconnectCallback;
    gattsCb.addCharCb = BesGattServerAddCharCallback;
    gattsCb.addDescCb = BesGattServerAddDescCallback;
    gattsCb.addServiceCb = BesGattServerAddServiceCallback;
    gattsCb.deleteServerCb = BesGattServerDeleteServerCallback;
    gattsCb.initCb = BesGattServerInitCallback;
    gattsCb.reqReadCb = BesGattServerReqReadCallback;
    gattsCb.reqWriteCb = BesGattServerReqWriteCallback;
    gattsCb.sendNtfCb = BesGattServerNTFCallback;
    gattsCb.startServerCb = BesGattServerStartServerCallback;
    gattsCb.stopServerCb = BesGattServerStopServerCallback;
    gattsCb.mtuExchangedCb = BesGattServerMtuExchangeCallback;
    btadapter_ble_register_server_callback(&gattsCb);
}
#endif
/**
 * @brief Registers a GATT server with a specified application UUID.
 *
 * The <b>RegisterServerCallback</b> is invoked to return the GATT server ID.
 *
 * @param appUuid Indicates the UUID of the application for which the GATT server is to be registered.
 * The UUID is defined by the application.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the GATT server is registered;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsRegister(BtUuid appUuid)
{
    int32_t server_if = 0;
    int status  = BT_STS_SUCCESS;
    bes_adapter_ble_service_db_t* service_db = NULL;

    server_if = bes_gatt_server_add_service(-1,
                                                        appUuid.uuid,
                                                        appUuid.uuidLen,
                                                        true,
                                                        8,
                                                        false);

    if(server_if >= 0){
        status = BT_STS_SUCCESS;
    }else{
        status = -1;
    }

    service_db = btadapter_ble_service_get_service_by_if(server_if);
    if(!service_db){
        status = -1;
    }

    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->registerServerCb)) {
        ohos_g_gattServ_cb->registerServerCb(status ,
                                                             server_if,
                                                             &appUuid);
    }

    BTA_TRACE(1, "%s", __func__);
    return bes_status_to_ohos(status);
}

/**
 * @brief Unregisters a GATT server with a specified ID.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the GATT server is unregistered;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
 int BleGattsUnRegister(int serverId)
{
    bes_gatt_server_unregister_service(serverId);
    BTA_TRACE(1, "%s", __func__);
    return OHOS_BT_STATUS_SUCCESS;
}

/**
 * @brief Disconnects the GATT server from the client.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param bdAddr Indicates the address of the client.
 * @param connId Indicates the connection ID, which is returned during the server registration.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the GATT server is disconnected from the client;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsDisconnect(int serverId, BdAddr bdAddr, int connId)
{
    // todo
    int status = OHOS_GATT_SUCCESS;
     if(!bdAddr.addr)
    {
        TRACE(1,"%s() bdAddr is NULL\n",__func__);
        status = OHOS_GATT_INVALID_HANDLE;
        return status;
    }
    TRACE(2,"%s() connId:%d\n",__func__, connId);
    app_ble_start_disconnect(connId);
    return status;
}

/**
 * @brief Adds a service.
 *
 * This function adds the service, its characteristics, and descriptors separately in sequence.\n
 * A service is a collection of data and related behavior that enable a specific capability or feature.\n
 * It consists of a service declaration and one or more included services and characteristics.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param srvcUuid Indicates the UUID of the service.
 * @param isPrimary Specifies whether the service is primary or secondary.
 * Value <b>true</b> indicates that the service is primary, and <b>false</b> indicates that the service is secondary.
 * @param number Indicates the number of attribute handles.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the service is added;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsAddService(int serverId, BtUuid srvcUuid, bool isPrimary, int number)
{
    int status = BT_STS_SUCCESS;
    bes_adapter_attm_item_t att_item;
    bes_adapter_ble_service_db_t* service_db = btadapter_ble_service_get_service_by_if(serverId);

    if(service_db){
        memset(&att_item, 0 ,sizeof(att_item));
        service_db->nattb = number;
        TRACE(2,"%s %d",__func__,service_db->nattb);
        service_db->is_primary = isPrimary;
        memcpy(att_item.uuid, srvcUuid.uuid, srvcUuid.uuidLen);
        att_item.uuid_len = srvcUuid.uuidLen;
        att_item.ext_perm = 0;
        att_item.prop = ADAPTER_BLE_PROP_READ;
        att_item.perm = 0;
        bes_gatt_server_update_att_num(serverId, number);
        status = bes_gatt_server_add_att_item(serverId,
                                                                        service_db->handle_offset ++,
                                                                        &att_item);
        if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->serviceAddCb)) {
               ohos_g_gattServ_cb->serviceAddCb(status ,
                                                               serverId,
                                                               &srvcUuid,
                                                               service_db->start_handle);
        }
    }else
        status = -1;

    return bes_status_to_ohos(status);
}

/**
 * @brief Adds an included service to a specified service.
 *
 * An included service is referenced to define another service on the GATT server.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param srvcHandle Indicates the handle ID of the service.
 * @param includedHandle Indicates the attribute handle ID of the included service.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the included service is added to the service;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsAddIncludedService(int serverId, int srvcHandle, int includedHandle)
{
    uint8_t included_uuid[2] = GATT_DECL_INCLUDED_SERVICE;
    bes_adapter_attm_item_t att_item;
    int status = 0;
    int incl_srvc_handle = 0;

    bes_adapter_ble_service_db_t* service_db = btadapter_ble_service_get_service_by_if(serverId);
    if(service_db){
        memcpy(att_item.uuid, included_uuid, 2);
        att_item.ext_perm = includedHandle;
        att_item.prop = ADAPTER_BLE_PROP_READ;
        att_item.perm = 0;
        incl_srvc_handle = service_db->start_handle + service_db->handle_offset;
        status = bes_gatt_server_add_att_item(serverId,
                                                            service_db->handle_offset ++,
                                                            &att_item);
    }
     if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->includeServiceAddCb)) {
           ohos_g_gattServ_cb->includeServiceAddCb(status,
                                                                    serverId,
                                                                    srvcHandle,
                                                                    incl_srvc_handle);
    }

    if(status == 0)
        return OHOS_BT_STATUS_SUCCESS;
    else
        return OHOS_BT_STATUS_FAIL;
}
/**
 * @brief Adds a characteristic to a specified service.
 *
 * A characteristic consists of data, the data access method, data format, and how the data manifests itself.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param srvcHandle Indicates the handle ID of the service.
 * @param characUuid Indicates the UUID of the characteristic to add.
 * @param properties Indicates the access methods supported by the characteristic,
 * as enumerated in {@link GattCharacteristicProperty}.
 * @param permissions Indicates the access permissions supported by the characteristic,
 * as enumerated in {@link GattAttributePermission}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the characteristic is added to the service;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsAddCharacteristic(int serverId, int srvcHandle, BtUuid characUuid,
                            int properties, int permissions)

{
    bes_adapter_attm_item_t att_item;
    int status = BT_STS_SUCCESS;
    int char_handle = 0;
    bes_adapter_ble_service_db_t* service_db = btadapter_ble_service_get_service_by_if(serverId);

    if(service_db){
        memset(&att_item, 0 ,sizeof(att_item));
        memcpy(att_item.uuid, characUuid.uuid, characUuid.uuidLen);
        att_item.uuid_len = characUuid.uuidLen;
        if(att_item.uuid_len == OHOS_BLE_UUID_MAX_LEN)
            att_item.ext_perm = 256;
        else
            att_item.ext_perm = 0;
        att_item.prop = BesGattServerSwapProp(properties);
        att_item.perm = BesGattServerSwapPerm(permissions);
        char_handle = service_db->start_handle + service_db->handle_offset;
        status = bes_gatt_server_add_att_item(serverId,
                                                            service_db->handle_offset ++,
                                                            &att_item);
    }

     if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->characteristicAddCb)) {
           ohos_g_gattServ_cb->characteristicAddCb(status,
                                                                    serverId,
                                                                    &characUuid,
                                                                    srvcHandle,
                                                                    char_handle);
    }
    return bes_status_to_ohos(status);
}

/**
 * @brief Adds a descriptor to a specified characteristic.
 *
 * A descriptor contains the description, configuration, and format of a characteristic.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param srvcHandle Indicates the handle ID of the service to which the characteristic belongs.
 * @param descUuid Indicates the UUID of the descriptor to add.
 * @param permissions Indicates the access permissions supported by the descriptor,
 * as enumerated in {@link GattAttributePermission}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the descriptor is added to the characteristic;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsAddDescriptor(int serverId, int srvcHandle, BtUuid descUuid, int permissions)
{
    bes_adapter_attm_item_t att_item;
    int status = BT_STS_SUCCESS;
    int descr_handle = 0;
    bes_adapter_ble_service_db_t* service_db = btadapter_ble_service_get_service_by_if(serverId);

    if(service_db){
        memset(&att_item, 0 ,sizeof(att_item));
        memcpy(att_item.uuid, descUuid.uuid, descUuid.uuidLen);
        att_item.uuid_len = descUuid.uuidLen;
        if(att_item.uuid_len == OHOS_BLE_UUID_MAX_LEN)
            att_item.ext_perm = 256;
        else
            att_item.ext_perm = 0;
        att_item.prop = 0;
        att_item.perm = BesGattServerSwapPerm(permissions);
        descr_handle = service_db->start_handle + service_db->handle_offset;
        status = bes_gatt_server_add_att_item(serverId,
                                                                        service_db->handle_offset ++,
                                                                        &att_item);
    }

    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->descriptorAddCb)) {
           ohos_g_gattServ_cb->descriptorAddCb(status,
                                                                serverId,
                                                                &descUuid,
                                                                srvcHandle,
                                                                descr_handle);
    }

    return bes_status_to_ohos(status);
}
/**
 * @brief Starts a service.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param srvcHandle Indicates the handle ID of the service.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the service is started;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsStartService(int serverId, int srvcHandle)
{
    int status = BT_STS_SUCCESS;
    bes_adapter_ble_service_db_t* service_db = btadapter_ble_service_get_service_by_if(serverId);

    if(service_db){
        TRACE(3,"%s %d %d",__func__,__LINE__,service_db->nattb);
        status = bes_gatt_server_start_service(serverId);
        BTA_TRACE(1, "%s start_handle:%d handle_num:%d user_handle_num:%d", __func__, service_db->start_handle, service_db->handle_offset, service_db->nattb);
    }else
        status = -1;

    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->serviceStartCb)) {
           ohos_g_gattServ_cb->serviceStartCb(status,
                                                            serverId,
                                                            srvcHandle);
    }

    return bes_status_to_ohos(status);
}
/**
 * @brief Stops a service.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param srvcHandle Indicates the handle ID of the service.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the service is stopped;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsStopService(int serverId, int srvcHandle)
{
    int status = OHOS_BT_STATUS_FAIL;
    bes_adapter_ble_service_db_t* service_db = btadapter_ble_service_get_service_by_if(serverId);

    if(service_db){
        status = bes_gatt_server_stop_service(serverId);
        BTA_TRACE(1, "%s start_handle:%d handle_num:%d user_handle_num:%d", __func__, service_db->start_handle, service_db->handle_offset, service_db->nattb);
    }else
        status = -1;
    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->serviceStopCb)) {
           ohos_g_gattServ_cb->serviceStopCb(status,
                                                            serverId,
                                                            srvcHandle);
    }

    return status;
}


/**
 * @brief Deletes a service.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param srvcHandle Indicates the handle ID of the service.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the service is deleted;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsDeleteService(int serverId, int srvcHandle)
{
    int status = OHOS_BT_STATUS_FAIL;
    return status;
}
/**
 * @brief Clears all services.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the services are cleared;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsClearServices(int serverId)
{
    int status = OHOS_BT_STATUS_FAIL;
    return status;
}

/**
 * @brief Sends a response to the client from which a read or write request has been received.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param param Indicates the pointer to the response parameters. For details, see {@link GattsSendRspParam}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the response is sent;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsSendResponse(int serverId, GattsSendRspParam *param)
{
    if (param == NULL){
        return -1;
    }

    int status = BT_STS_SUCCESS;

    bes_adapter_ble_service_db_t* service_db = btadapter_ble_service_get_service_by_if(serverId);

    if(service_db){
        status = btadapter_ble_service_send_gatt_get_response(service_db,
                                                                                                        param->connectId,
                                                                                                        param->attrHandle,
                                                                                                        param->value,
                                                                                                        param->valueLen,
                                                                                                        param->status);
    }else
        status = -1;

    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->responseConfirmationCb)) {
        ohos_g_gattServ_cb->responseConfirmationCb(status,
                                                                        param->attrHandle);
    }

    return bes_status_to_ohos(status);
}


/**
 * @brief Sends an indication or notification to the client.
 *
 * The <b>confirm</b> field in <b>param</b> determines whether to send an indication or a notification.
 *
 * @param serverId Indicates the ID of the GATT server.
 * @param param Indicates the pointer to the sending parameters. For details, see {@link GattsSendIndParam}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the indication or notification is sent;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */

int BleGattsSendIndication(int serverId, GattsSendIndParam *param)
{
    if (param == NULL){
        return -1;
    }

    int status = BT_STS_SUCCESS;
    bes_adapter_ble_service_db_t* service_db = btadapter_ble_service_get_service_by_if(serverId);

    if(service_db)
    {
        if(ohos_att_handle_switch_to_bes(param->attrHandle)){
            param->attrHandle = ohos_att_handle_switch_to_bes(param->attrHandle);
        }
        status = bes_gatt_server_send_indication(serverId,param->connectId,param->attrHandle,
            param->confirm,param->value,param->valueLen);
        BTA_TRACE(3, "%s sendLen:%d confirm:%d handlr:0x%x serverId:%d connectId:%d", __func__, param->valueLen,
                    param->confirm, param->attrHandle, serverId, param->connectId);
    } else
        status = BT_STS_FAILED;

    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->indicationSentCb)) {
              ohos_g_gattServ_cb->indicationSentCb(param->connectId,status);
    }

    return bes_status_to_ohos(status);
}

/**
 * @brief Sets the encryption type for the GATT connection.
 *
 * @param bdAddr Indicates the address of the client.
 * @param secAct Indicates the encryption type, as enumerated in {@link BleSecAct}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the encryption type is set;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsSetEncryption(BdAddr bdAddr, BleSecAct secAct)
{
    app_set_authen_level(ohos_Encryption_levle_to_bes(secAct));
    return OHOS_BT_STATUS_SUCCESS;
}

/**
 * @brief Registers GATT server callbacks.
 *
 * @param func Indicates the pointer to the callbacks to register, as enumerated in {@link BtGattServerCallbacks}.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the callbacks are registered;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
extern void BleRegisterConnCallback(void);
int BleGattsRegisterCallbacks(BtGattServerCallbacks *func)
{
    if (func)
    {
        ohos_g_gattServ_cb = func;
        BleRegisterConnCallback();
        BesGattServerRegisterCallback();
        return OHOS_BT_STATUS_SUCCESS;
    }
    return OHOS_BT_STATUS_FAIL;
}

/**
 * @brief Adds a service, its characteristics, and descriptors and starts the service.
 *
 * This function is available for system applications only.
 *
 * @param srvcHandle Indicates the pointer to the handle ID of the service,
 * which is returned by whoever implements this function.
 * @param srvcInfo Indicates the pointer to the service information.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the operation is successful;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */

int BleGattsStartServiceEx(int *srvcHandle, BleGattService *srvcInfo)
{
    int status = OHOS_BT_STATUS_SUCCESS;
    int32_t server_id = 0;
    static int32_t handle_offset = 0;
    static int32_t att_num = 0;

    if (srvcHandle && srvcInfo && srvcInfo->attrList) {
        att_num = srvcInfo->attrNum;
        for (int i=0; i<srvcInfo->attrNum; i++)
        {
            if(srvcInfo->attrList[i].attrType == OHOS_BLE_ATTRIB_TYPE_CHAR)
            {
                if ((srvcInfo->attrList[i].properties & OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY)
                    || (srvcInfo->attrList[i].properties & OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE))
                {
                    att_num = att_num + 2;
                }
                else
                {
                    att_num = att_num + 1;
                }
            }
        }
    }
    if (srvcHandle && srvcInfo && srvcInfo->attrList) {
        BTA_TRACE(4, "%s:srvcHandle:%p srvcInfo:%p srvcIndfo->attrNum:%d\n", __func__, srvcHandle, srvcInfo,srvcInfo->attrNum);
        for (int i=0; i<srvcInfo->attrNum; i++)
        {
            BTA_TRACE(3, "%s:srvcInfo->attrList[%d].attrType=%d\n", __func__, i, srvcInfo->attrList[i].attrType);
            switch(srvcInfo->attrList[i].attrType)
            {
                case OHOS_BLE_ATTRIB_TYPE_SERVICE:
                {
                    BtUuid serUuid;
                    serUuid.uuid = g_gattPrimaryService;
                    serUuid.uuidLen = sizeof(g_gattPrimaryService);
                    server_id = bes_gatt_server_add_service(-1,
                                                            srvcInfo->attrList[i].uuid,
                                                            sizeof(srvcInfo->attrList[i].uuid),
                                                            true,
                                                            att_num,
                                                            false);
                    BTA_TRACE(2, "%s:serUuid:%p \n", __func__, serUuid.uuid);
                    bes_adapter_attm_item_t att_item;
                    bes_adapter_ble_service_db_t* service_db = btadapter_ble_service_get_service_by_if(server_id);
                    if (service_db) {
                        BTA_TRACE(3, "%s:service_db:%p %d\n", __func__, service_db, service_db->nattb);
                        memset(&att_item, 0 ,sizeof(att_item));
                        service_db->nattb = att_num;
                        service_db->is_primary = true;
                        memcpy(att_item.uuid, serUuid.uuid, serUuid.uuidLen);
                        att_item.uuid_len = serUuid.uuidLen;
                        att_item.ext_perm = 0;
                        att_item.prop = OHOS_GATT_CHARACTER_PROPERTY_BIT_READ;
                        att_item.perm = OHOS_GATT_PERMISSION_READ;
                        bes_gatt_server_update_att_num(server_id, att_num);
                        BTA_TRACE(2, "%s:att_item:%p \n", __func__, &att_item);
                        int servstatus = bes_gatt_server_add_att_item(server_id, service_db->handle_offset++, &att_item);
                        if(ohos_g_gattServ_cb && ohos_g_gattServ_cb->serviceAddCb) {
                            BTA_TRACE(2, "%s:serviceAddCb:%p \n", __func__, ohos_g_gattServ_cb->serviceAddCb);
                            ohos_g_gattServ_cb->serviceAddCb(servstatus, server_id,&serUuid, service_db->start_handle);
                        }
                        *srvcHandle = service_db->start_handle;
                        handle_offset = 1;
                        BTA_TRACE(2, "%s:srvcHandle:%d handle_offset:%d\n", __func__, *srvcHandle, handle_offset);
                    }
                    break;
                }
                case OHOS_BLE_ATTRIB_TYPE_CHAR:
                {
                    BtUuid characUuid;
                    characUuid.uuid = g_gattCharacteristicUuid;
                    characUuid.uuidLen = sizeof(g_gattCharacteristicUuid);
                    BleGattsAddCharacteristic(server_id, *srvcHandle, characUuid, OHOS_GATT_CHARACTER_PROPERTY_BIT_READ, OHOS_GATT_PERMISSION_READ);
                    handle_offset ++;
                }
                case OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE:
                {
                    BtUuid characUuid;
                    characUuid.uuid = srvcInfo->attrList[i].uuid;
                    characUuid.uuidLen = OHOS_BLE_UUID_MAX_LEN;
                    BleGattsAddCharacteristic(server_id, *srvcHandle, characUuid, srvcInfo->attrList[i].properties, srvcInfo->attrList[i].permission);
                    ohos_g_gatt_att[att_num].start_hdl = *srvcHandle;
                    ohos_g_gatt_att[att_num].end_hdl = *srvcHandle + MAX_GATT_ATT_NUM;
                    ohos_g_gatt_att[att_num].handle = *srvcHandle + handle_offset;
                    memcpy(&ohos_g_gatt_att[att_num++].attrList, &srvcInfo->attrList[i], sizeof(BleGattAttr));
                    BTA_TRACE(3, "%s:ohos_g_gatt_att[%d].handle:%x srvcInfo->attrList[%d].properties:0x%x srvcInfo->attrList[%d].func.write:%p\n", __func__,
                              att_num, ohos_g_gatt_att[att_num-1].handle, i, srvcInfo->attrList[i].properties, i, srvcInfo->attrList[i].func.write);
                    handle_offset ++;
                    if ( !((srvcInfo->attrList[i].properties & OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY)
                        || (srvcInfo->attrList[i].properties & OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE)))
                    {
                        break;
                    }
                }
                case OHOS_BLE_ATTRIB_TYPE_CHAR_CLIENT_CONFIG:
                {
                    BtUuid characUuid;
                    characUuid.uuid = g_gattCharacteristicCfgUuid;
                    characUuid.uuidLen = sizeof(g_gattCharacteristicCfgUuid);
                    BleGattsAddCharacteristic(server_id, *srvcHandle, characUuid, OHOS_GATT_CHARACTER_PROPERTY_BIT_READ, OHOS_GATT_PERMISSION_READ);
                    handle_offset ++;
                    break;
                }
                case OHOS_BLE_ATTRIB_TYPE_CHAR_USER_DESCR:
                {
                    BtUuid descUuid;
                    descUuid.uuid = srvcInfo->attrList[i].uuid;
                    descUuid.uuidLen = OHOS_BLE_UUID_MAX_LEN;
                    TRACE(2,"%s permission is %d",__func__,srvcInfo->attrList[i].permission);
                    DUMP8("%02x ",descUuid.uuid,OHOS_BLE_UUID_MAX_LEN);
                    BleGattsAddDescriptor(server_id, *srvcHandle, descUuid, srvcInfo->attrList[i].permission);

                    ohos_g_gatt_att[att_num].start_hdl = *srvcHandle;
                    ohos_g_gatt_att[att_num].end_hdl = *srvcHandle + MAX_GATT_ATT_NUM;
                    ohos_g_gatt_att[att_num].handle = *srvcHandle + handle_offset;
                    memcpy(&ohos_g_gatt_att[att_num++].attrList, &srvcInfo->attrList[i], sizeof(BleGattAttr));
                    handle_offset ++;
                    break;
                }

                default:break;
            }
        }
        BleGattsStartService(server_id, *srvcHandle);
    }
    return status;
}

/**
 * @brief Stops a service.
 *
 * This function is available for system applications only.
 *
 * @param srvcHandle Indicates the handle ID of the service.
 * @return Returns {@link OHOS_BT_STATUS_SUCCESS} if the operation is successful;
 * returns an error code defined in {@link BtStatus} otherwise.
 * @since 6
 */
int BleGattsStopServiceEx(int srvcHandle)
{
    if ((ohos_g_gattServ_cb) && (ohos_g_gattServ_cb->serviceStopCb)) {
        ohos_g_gattServ_cb->serviceStopCb(0,0,srvcHandle);
    }

    return OHOS_BT_STATUS_SUCCESS;
}

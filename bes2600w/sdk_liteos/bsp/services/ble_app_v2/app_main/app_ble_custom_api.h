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
#ifndef __APP_BLE_CUSTOM_API_H__
#define __APP_BLE_CUSTOM_API_H__

/*****************************header include********************************/

/******************************macro defination*****************************/

/******************************type defination******************************/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
 *            app_ble_custom_adv_write_data
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    write adv data of customer
 *
 * Parameters:
 *    actv_user: The user of the adv activity
 *    is_custom_adv_flags: If this flag was set, custom can set adv flag by himself
 *    local_addr: The local address of this adv. If local_addr is NULL, then use the ble address from factory section
 *    peer_addr: If adv_type is direct adv, this param is the address of peer ble
 *    adv_interval: Adv interval
 *    adv_type: Adv type
 *    tx_power_dbm: Adv tx power in dbm, range: -3~16
 *    adv_data: Adv data
 *    adv_data_size: Adv data size
 *    scan_rsp_data: Scan response data
 *    scan_rsp_data_size: Scan response data size
 *
 * Return:
 *    None
 */
void app_ble_custom_adv_write_data(BLE_ADV_ACTIVITY_USER_E actv_user,
                    bool is_custom_adv_flags,
                    uint8_t *local_addr,
                    ble_bdaddr_t *peer_addr,
                    uint16_t adv_interval,
                    BLE_ADV_TYPE_E adv_type,
                    int8_t tx_power_dbm,
                    uint8_t *adv_data, uint8_t adv_data_size,
                    uint8_t *scan_rsp_data, uint8_t scan_rsp_data_size);

/*---------------------------------------------------------------------------
 *            app_ble_custom_adv_start
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    for customer to start a adv
 *
 * Parameters:
 *    actv_user: The user of the adv activity
 *
 * Return:
 *    None
 */
void app_ble_custom_adv_start(BLE_ADV_ACTIVITY_USER_E actv_user);

/*---------------------------------------------------------------------------
 *            app_ble_custom_adv_start
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    for customer to stop a adv
 *
 * Parameters:
 *    actv_user: The user of the adv activity
 *
 * Return:
 *    None
 */
void app_ble_custom_adv_stop(BLE_ADV_ACTIVITY_USER_E actv_user);

/*---------------------------------------------------------------------------
 *            app_ble_custom_init
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    initialize app ble custom
 *
 * Parameters:
 *    void
 *
 * Return:
 *    None
 */
void app_ble_custom_init(void);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef __APP_BLE_CUSTOM_API_H__ */


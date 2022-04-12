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
#ifndef __APP_BLE_CUSTOMIF_H__
#define __APP_BLE_CUSTOMIF_H__

/*****************************header include********************************/

/******************************macro defination*****************************/

/******************************type defination******************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_BT_ADAPTER
typedef void (*ble_adv_data_report_cb_t)(ble_bdaddr_t *bleAddr,          int8_t rssi,uint8_t *adv_buf,uint8_t len);
void app_ble_adv_report_callback_register(ble_adv_data_report_cb_t cb);
void app_ble_adv_report_callback_deregister(void);
#endif
/*---------------------------------------------------------------------------
 *            app_ble_customif_init
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    None
 *
 * Parameters:
 *    void
 *
 * Return:
 *    void
 */
void app_ble_customif_init(void);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef __APP_BLE_CUSTOMIF_H__ */


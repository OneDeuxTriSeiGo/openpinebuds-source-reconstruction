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
#ifndef __APP_BLE_CORE_H__
#define __APP_BLE_CORE_H__

/*****************************header include********************************/

/******************************macro defination*****************************/

/******************************type defination******************************/
/**
 * @brief The event type of the ble
 *
 */
typedef enum{
    BLE_CONNECT_EVENT                   = 0,
    BLE_DISCONNECT_EVENT                = 6,
    BLE_CONN_PARAM_UPDATE_REQ_EVENT     = 7,
    BLE_SET_RANDOM_BD_ADDR_EVENT        = 10,

    BLE_EVENT_NUM_MAX,
} ble_evnet_type_e;


/**
 * @brief The event type of other module
 *
 */
typedef enum{
    BLE_CALLBACK_RS_START = 0,
    BLE_CALLBACK_RS_COMPLETE,
    BLE_CALLBACK_ROLE_UPDATE,
    BLE_CALLBACK_IBRT_EVENT_ENTRY,

    BLE_CALLBACK_EVENT_NUM_MAX,
} ble_callback_evnet_type_e;

typedef struct {
    uint8_t conidx;
    ble_bdaddr_t peer_bdaddr;
} connect_handled_t;

typedef struct {
    uint8_t conidx;
    uint8_t errCode;
} disconnect_handled_t;

typedef struct {
    /// Connection interval minimum
    uint16_t intv_min;
    /// Connection interval maximum
    uint16_t intv_max;
    /// Latency
    uint16_t latency;
    /// Supervision timeout
    uint16_t time_out;
} conn_param_update_req_handled_t;

typedef struct {
    uint8_t *new_bdaddr;
} set_random_bd_addr_handled_t;

typedef union {
    connect_handled_t connect_handled;
    disconnect_handled_t disconnect_handled;
    conn_param_update_req_handled_t conn_param_update_req_handled;
    set_random_bd_addr_handled_t set_random_bd_addr_handled;
} ble_event_handled_t;

typedef struct {
    ble_evnet_type_e evt_type;
    ble_event_handled_t p;
} ble_event_t;

typedef struct {
    ble_callback_evnet_type_e evt_type;
    union {
        struct {
            uint8_t newRole;
        } rs_complete_handled;
        struct {
            uint8_t newRole;
        } role_update_handled;
        struct {
            uint8_t event;
        } ibrt_event_entry_handled;
    } p;
} ble_callback_event_t;

typedef void (*APP_BLE_CORE_GLOBAL_HANDLER_FUNC)(ble_event_t *, void *);
typedef void (*APP_BLE_CORE_GLOBAL_CALLBACK_HANDLER_FUNC)(ble_callback_event_t *, void *);

// Element of a message handler table.
typedef struct {
    // evt_type of the ble event
    ble_evnet_type_e evt_type;
    // Pointer to the handler function for the ble event above.
    APP_BLE_CORE_GLOBAL_HANDLER_FUNC func;
} ble_event_handler_t;

// Element of a message handler table.
typedef struct {
    // evt_type of the ble callback event
    ble_callback_evnet_type_e evt_type;
    // Pointer to the handler function for the ble callback event above.
    APP_BLE_CORE_GLOBAL_CALLBACK_HANDLER_FUNC func;
} ble_callback_event_handler_t;

#ifdef __cplusplus
extern "C" {
#endif

/****************************function declearation**************************/
/*---------------------------------------------------------------------------
 *            app_ble_core_register_global_handler_ind
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    to register custom handler function
 *
 * Parameters:
 *    handler -- custom handler function
 *
 * Return:
 *    void
 */
void app_ble_core_register_global_handler_ind(APP_BLE_CORE_GLOBAL_HANDLER_FUNC handler);

/*---------------------------------------------------------------------------
 *            app_ble_core_register_global_callback_handle_ind
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    to register custom callback handler function
 *
 * Parameters:
 *    handler -- custom callback handler function
 *
 * Return:
 *    void
 */
void app_ble_core_register_global_callback_handle_ind(APP_BLE_CORE_GLOBAL_CALLBACK_HANDLER_FUNC handler);

/*---------------------------------------------------------------------------
 *            app_ble_core_global_handle
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    for ble core to handle event
 *
 * Parameters:
 *    *event -- the event ble core need to handle
 *
 * Return:
 *    uint32_t
 */
void app_ble_core_global_handle(ble_event_t *event, void *output);

/*---------------------------------------------------------------------------
 *            app_ble_core_global_callback_event
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    for ble core to handle callback event
 *
 * Parameters:
 *    *event -- the event ble core need to handle
 *
 * Return:
 *    void
 */
void app_ble_core_global_callback_event(ble_callback_event_t *event, void *output);

/*---------------------------------------------------------------------------
 *            app_ble_stub_user_init
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
void app_ble_stub_user_init(void);

/****************************function declearation**************************/
/*---------------------------------------------------------------------------
 *            app_ble_sync_ble_info
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    for tws sync ble info
 *
 * Parameters:
 *    void
 *
 * Return:
 *    void
 */
void app_ble_sync_ble_info(void);

/*---------------------------------------------------------------------------
 *            app_ble_mode_tws_sync_init
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    tws related environment initialization for ble module
 *
 * Parameters:
 *    void
 *
 * Return:
 *    void
 */
void app_ble_mode_tws_sync_init(void);

/*---------------------------------------------------------------------------
 *            app_ble_core_print_ble_state
 *---------------------------------------------------------------------------
 *
 *Synopsis:
 *    print ble state
 *
 * Parameters:
 *    void
 *
 * Return:
 *    void
 */
void app_ble_core_print_ble_state(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __APP_BLE_CORE_H__ */

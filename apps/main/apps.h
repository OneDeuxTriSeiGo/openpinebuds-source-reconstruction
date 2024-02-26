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
#ifndef __APPS_H__
#define __APPS_H__

#include "app_status_ind.h"

#define STACK_READY_BT  0x01
#define STACK_READY_BLE 0x02

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

int app_init(void);

int app_deinit(int deinit_case);

int app_shutdown(void);

int app_reset(void);

int app_status_battery_report(uint8_t level);

int app_voice_report( APP_STATUS_INDICATION_T status,uint8_t device_id);
int app_voice_report_generic(APP_STATUS_INDICATION_T status, uint8_t device_id, uint8_t isMerging);
int app_voice_stop(APP_STATUS_INDICATION_T status, uint8_t device_id);


/*FixME*/
void app_status_set_num(const char* p);

////////////10 second tiemr///////////////
#define APP_FAST_PAIRING_TIMEOUT_IN_SECOND  120

#define APP_PAIR_TIMER_ID       0
#define APP_POWEROFF_TIMER_ID   1
#define APP_FASTPAIR_LASTING_TIMER_ID   2

void app_stop_10_second_timer(uint8_t timer_id);
void app_start_10_second_timer(uint8_t timer_id);

void app_notify_stack_ready(uint8_t ready_flag);

void app_start_postponed_reset(void);

bool app_is_power_off_in_progress(void);

#define CHIP_ID_C     1
#define CHIP_ID_D     2

void app_disconnect_all_bt_connections(void);
bool app_is_stack_ready(void);

extern uint8_t latency_mode_is_open;
bool Curr_Is_Master(void);
bool Curr_Is_Slave(void);
extern uint8_t get_nv_role(void);
extern uint8_t get_curr_role(void);
extern uint8_t  app_poweroff_flag;
extern bool MobileLinkLose_reboot;
extern bool factory_mode_status;
extern uint8_t  app_poweroff_flag;
extern bool MobileLinkLose_reboot;
extern void startclr_info_timer(int ms);
extern void app_enterpairing_timer_start(void);
extern void app_enterpairing_timer_stop(void);
extern void startdelay_report_tone(int ms,APP_STATUS_INDICATION_T status);
extern void box_cmd_app_bt_enter_mono_pairing_mode(void);
extern int app_nvrecord_rebuild(void);
extern void app_bt_power_off_customize();
////////////////////


#ifdef __cplusplus
}
#endif
#endif//__FMDEC_H__

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
#ifndef __BT_XTAL_SYNC_H__
#define __BT_XTAL_SYNC_H__

#ifdef __cplusplus
extern "C" {
#endif

enum BT_XTAL_SYNC_MODE_T {
    BT_XTAL_SYNC_MODE_MUSIC = 1,
    BT_XTAL_SYNC_MODE_VOICE = 2,
    BT_XTAL_SYNC_MODE_WITH_MOBILE = 3,
    BT_XTAL_SYNC_MODE_WITH_TWS = 4,
};

void bt_xtal_sync(enum BT_XTAL_SYNC_MODE_T mode);
#ifdef BT_XTAL_SYNC_NEW_METHOD
typedef struct {
    float Kp;
    float Kd;
}pid_para_t;
void bt_xtal_sync_new(int32_t rxbit, bool fix_rxbit_en, enum BT_XTAL_SYNC_MODE_T mode);
void bt_xtal_sync_new_new(int32_t rxbit, bool fix_rxbit_en, enum BT_XTAL_SYNC_MODE_T mode);

#endif
void bt_init_xtal_sync(enum BT_XTAL_SYNC_MODE_T mode, int range_min, int range_max, int fcap_range);
void bt_term_xtal_sync(bool xtal_term_default);
void bt_term_xtal_sync_default(void);

#ifdef __cplusplus
}
#endif

#endif

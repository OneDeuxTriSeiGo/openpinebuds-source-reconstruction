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
#ifndef __HAL_PSC_BEST2003_H__
#define __HAL_PSC_BEST2003_H__

#ifdef __cplusplus
extern "C" {
#endif

void  hal_psc_dslp_force_on_bt_enable(void);

void  hal_psc_dslp_force_on_bt_disable(void);

void hal_psc_wlan_enable(void);

void hal_psc_wlan_disable(void);

void hal_psc_a7_enable(void);

void hal_psc_a7_disable(void);

#ifdef __cplusplus
}
#endif

#endif


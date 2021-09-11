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
#ifndef __HAL_SEC_BEST2003_H__
#define __HAL_SEC_BEST2003_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

void hal_sec_set_cksum_nonsec(bool nonsec);
void hal_sec_set_crc_nonsec(bool nonsec);
void hal_sec_set_wf2mcu_nonsec(bool nonsec);
void hal_mpc_spy_nonsec_bypass(uint32_t addr, bool bypass);
void hal_page_spy_set(uint32_t spy_base, uint32_t start_addr, uint32_t len, bool write, bool read);

#ifdef __cplusplus
}
#endif

#endif


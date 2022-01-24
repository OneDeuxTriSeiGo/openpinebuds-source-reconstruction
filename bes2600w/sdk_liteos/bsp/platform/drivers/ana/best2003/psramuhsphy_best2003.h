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
#ifndef __PSRAMUHSPHY_BEST2003_H__
#define __PSRAMUHSPHY_BEST2003_H__

#include "plat_types.h"

#define SPIPHY_PSRAMUHS0_CS                 1

#define SPIPHY_PSRAMUHS0_REG(reg)           (((reg) & 0xFFF) | (SPIPHY_PSRAMUHS0_CS << 12))

#ifdef __cplusplus
extern "C" {
#endif

void psramuhs_read_reg(uint32_t reg, uint32_t *val);
void hal_psramuhsip_mc_busy_wait(void);
void hal_psramuhsip_wb_busy_wait(void);
void psramuhsphy_printf_window();

#ifdef __cplusplus
}
#endif

#endif


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
#ifndef __MIPIPHY_H__
#define __MIPIPHY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_addr_map.h"
#include CHIP_SPECIFIC_HDR(mipiphy)
#include "hal_phyif.h"

#ifdef SPIPHY_MIPI_REG
#define SPI_MIPI_REG(reg)           SPIPHY_MIPI_REG(reg)
#else
#define SPI_MIPI_REG(reg)           (reg)
#endif

#define mipiphy_read(reg, val)      hal_phyif_reg_read(SPI_MIPI_REG(reg), val)
#define mipiphy_write(reg, val)     hal_phyif_reg_write(SPI_MIPI_REG(reg), val)

void dsiphy_open(uint32_t clk);

void dsiphy_sleep(void);

void dsiphy_wakeup(void);

void csiphy_open(uint32_t clk);

void csiphy_sleep(void);

void csiphy_wakeup(void);

#ifdef __cplusplus
}
#endif

#endif


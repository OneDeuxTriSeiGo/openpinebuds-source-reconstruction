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
#ifndef SPI_NORFLASH_DRV_H
#define SPI_NORFLASH_DRV_H

#include "plat_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

//---------------------------------------------------------------------------
// API
//---------------------------------------------------------------------------

bool spi_norflash_match_chip(const uint8_t *id, uint32_t len);
int32_t spi_norflash_get_size(uint32_t *total_size, uint32_t *block_size, uint32_t *sector_size, uint32_t *page_size);

int32_t spi_norflash_drv_spi_init(const struct HAL_SPI_CFG_T * spi_cfg);
int32_t spi_norflash_drv_spi_deinit(void);
int32_t spi_norflash_drv_get_id(uint8_t * id, uint32_t len);
int32_t spi_norflash_drv_get_unique_id(uint8_t * id, uint32_t len);
int32_t spi_norflash_drv_read(uint32_t addr, uint8_t *buff, uint32_t len);
int32_t spi_norflash_drv_write(uint32_t addr, uint8_t *buff, uint32_t len);
int32_t spi_norflash_drv_erase(uint32_t addr, uint32_t len);
int32_t spi_norflash_drv_reset(void);

// Error code.
#define SPI_NORFLASH_DRV_RET_OK   0
#define SPI_NORFLASH_DRV_RET_SPI_ERROR 1


#ifdef __cplusplus
}
#endif

#endif /*SPI_NORFLASH_DRV_H*/

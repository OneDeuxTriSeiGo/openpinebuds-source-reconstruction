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
#ifndef _HAL_SDMMC_H_
#define _HAL_SDMMC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

enum HAL_SDMMC_ID_T {
    HAL_SDMMC_ID_0 = 0,
    HAL_SDMMC_ID_NUM,
};

typedef void (*HAL_SDMMC_DELAY_FUNC)(uint32_t ms);

/* hal api */
HAL_SDMMC_DELAY_FUNC hal_sdmmc_set_delay_func(HAL_SDMMC_DELAY_FUNC new_func);
int32_t hal_sdmmc_open(enum HAL_SDMMC_ID_T id);
void hal_sdmmc_info(enum HAL_SDMMC_ID_T id, uint32_t *sector_count, uint32_t *sector_size);
uint32_t hal_sdmmc_read_blocks(enum HAL_SDMMC_ID_T id, uint32_t start_block, uint32_t block_count, uint8_t* dest);
uint32_t hal_sdmmc_write_blocks(enum HAL_SDMMC_ID_T id, uint32_t start_block, uint32_t block_count, uint8_t* src);
void hal_sdmmc_close(enum HAL_SDMMC_ID_T id);
void hal_sdmmc_dump(enum HAL_SDMMC_ID_T id);
int hal_sdmmc_enable_detecter(enum HAL_SDMMC_ID_T id,void (* cb)(uint8_t));

#ifdef __cplusplus
}
#endif

#endif /* _HAL_SDMMC_H_ */

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
#ifndef __LFS_ADAPT_H__
#define __LFS_ADAPT_H__
#ifdef __cplusplus
extern "C"
{
#endif

    int lfs_device_open(void);
    int lfs_device_close(void);
    int lfs_test(void);
    struct lfs_config *lfs_get_config(void);

#ifdef LFS_ON_FLASH
#include "lfs_adapt_flash.h"
#define lfs_block_device_open lfs_flash_open
#define lfs_block_device_close lfs_flash_close
#define lfs_block_device_read lfs_flash_read
#define lfs_block_device_prog lfs_flash_prog
#define lfs_block_device_erase lfs_flash_erase
#define lfs_block_device_sync lfs_flash_sync
#define lfs_block_device_get_read_size lfs_flash_get_read_size
#define lfs_block_device_get_prog_size lfs_flash_get_prog_size
#define lfs_block_device_get_block_size lfs_flash_get_block_size
#define lfs_block_device_get_block_count lfs_flash_get_block_count
#define lfs_block_device_get_block_cycles lfs_flash_get_block_cycles
#define lfs_block_device_get_cache_size lfs_flash_get_cache_size
#define lfs_block_device_get_lookahead_size lfs_flash_get_lookahead_size
#define lfs_block_device_test lfs_flash_test
#endif

#ifdef LFS_ON_SD_CARD
#include "lfs_adapt_sd_card.h"
#define lfs_block_device_open lfs_sd_card_open
#define lfs_block_device_close lfs_sd_card_close
#define lfs_block_device_read lfs_sd_card_read
#define lfs_block_device_prog lfs_sd_card_prog
#define lfs_block_device_erase lfs_sd_card_erase
#define lfs_block_device_sync lfs_sd_card_sync
#define lfs_block_device_get_read_size lfs_sd_card_get_read_size
#define lfs_block_device_get_prog_size lfs_sd_card_get_prog_size
#define lfs_block_device_get_block_size lfs_sd_card_get_block_size
#define lfs_block_device_get_block_count lfs_sd_card_get_block_count
#define lfs_block_device_get_block_cycles lfs_sd_card_get_block_cycles
#define lfs_block_device_get_cache_size lfs_sd_card_get_cache_size
#define lfs_block_device_get_lookahead_size lfs_sd_card_get_lookahead_size
#define lfs_block_device_test lsf_sd_card_test
#endif

#ifdef __cplusplus
}
#endif

#endif /* __LFS_ADAPT_H__ */

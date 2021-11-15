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
#ifndef CACHE_HAL_H
#define CACHE_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

enum HAL_CACHE_ID_T {
    HAL_CACHE_ID_I_CACHE = 0,
    HAL_CACHE_ID_D_CACHE,
    HAL_CACHE_ID_NUM,
};

struct HAL_CACHE_MON_DATA_T {
    uint64_t r_hit;
    uint64_t r_miss;
    uint64_t w_hit;
    uint64_t w_miss;
};

uint8_t hal_cache_enable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_disable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_writebuffer_enable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_writebuffer_disable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_writebuffer_flush(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_writeback_enable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_writeback_disable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_wrap_enable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_wrap_disable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_invalidate_all(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_invalidate(enum HAL_CACHE_ID_T id, uint32_t start_address, uint32_t len);
uint8_t hal_cache_sync_all(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_sync(enum HAL_CACHE_ID_T id, uint32_t start_address, uint32_t len);
uint8_t hal_cache_monitor_enable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_monitor_disable(enum HAL_CACHE_ID_T id);
uint8_t hal_cache_get_monitor_data(enum HAL_CACHE_ID_T id, struct HAL_CACHE_MON_DATA_T *md);
void hal_cache_print_stats(void);
void hal_cache_sleep(void);
void hal_cache_wakeup(void);

uint8_t hal_cachecp_enable(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_disable(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_writebuffer_enable(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_writebuffer_disable(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_writebuffer_flush(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_writeback_enable(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_writeback_disable(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_invalidate(enum HAL_CACHE_ID_T id, uint32_t start_address, uint32_t len);
uint8_t hal_cachecp_sync_all(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_monitor_enable(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_monitor_disable(enum HAL_CACHE_ID_T id);
uint8_t hal_cachecp_get_monitor_data(enum HAL_CACHE_ID_T id, struct HAL_CACHE_MON_DATA_T *md);
void hal_cachecp_print_stats(void);

#ifdef __cplusplus
}
#endif

#endif /* CACHE_HAL_H */

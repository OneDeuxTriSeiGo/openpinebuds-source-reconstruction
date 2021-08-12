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
#ifndef __HAL_SDIO_HOST_H__
#define __HAL_SDIO_HOST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

enum HAL_SDIO_HOST_ID_T {
    HAL_SDIO_HOST_ID_0 = 0,

    HAL_SDIO_HOST_ID_NUM,
};

enum HAL_SDIO_DEVICE_RW {
    HAL_SDIO_DEVICE_READ = 0,
    HAL_SDIO_DEVICE_WRITE,
};

enum HAL_SDIO_ERR {
    HAL_SDIO_NONE = 0,
    HAL_SDIO_CMD3_FAIL,
    HAL_SDIO_CMD5_FAIL,
    HAL_SDIO_CMD7_FAIL,
    HAL_SDIO_CMD52_FAIL,
    HAL_SDIO_CMD53_FAIL,
    HAL_SDIO_RESET_FAIL,
    HAL_SDIO_SET_ABORT_FAIL,
    HAL_SDIO_BLOCK_SIZE_ZERO,
    HAL_SDIO_GET_VERSION_FAIL,
    HAL_SDIO_INVALID_PARAMETER,
    HAL_SDIO_SET_BUS_WIDTH_FAIL,
    HAL_SDIO_GET_BUS_WIDTH_FAIL,
    HAL_SDIO_GET_INT_PENDING_FAIL,
    HAL_SDIO_ENABLE_FUNCTION_FAIL,
    HAL_SDIO_INVALID_FUNCTION_NUM,
    HAL_SDIO_DISABLE_FUNCTION_FAIL,
    HAL_SDIO_ENABLE_MASTER_INT_FAIL,
    HAL_SDIO_DISABLE_MASTER_INT_FAIL,
    HAL_SDIO_ENABLE_FUNCTION_INT_FAIL,
    HAL_SDIO_DISABLE_FUNCTION_INT_FAIL,
    HAL_SDIO_ENABLE_INT_EXT_FAIL,
    HAL_SDIO_DISABLE_INT_EXT_FAIL,
    HAL_SDIO_ENABLE_FUNC1_INT_TO_DEVICE_FAIL,
    HAL_SDIO_GET_LOAD_RDY_FAIL,
    HAL_SDIO_GET_FUNC1_RX_BUF_CFG_FAIL,
};

typedef void (*HAL_SDIO_HOST_INT_HANDLER_T)(uint32_t status);

int32_t hal_sdio_host_open(enum HAL_SDIO_HOST_ID_T id, HAL_SDIO_HOST_INT_HANDLER_T handler);
void hal_sdio_host_close(enum HAL_SDIO_HOST_ID_T id);

enum HAL_SDIO_ERR hal_sdio_reset(enum HAL_SDIO_HOST_ID_T id);
enum HAL_SDIO_ERR hal_sdio_set_func_abort(enum HAL_SDIO_HOST_ID_T id, uint8_t func_num);

int hal_sdio_host_send_cmd(enum HAL_SDIO_HOST_ID_T id, uint8_t cmdindex, uint32_t cmdarg,
                           uint8_t rsp_type, uint32_t *rsp);
int hal_sdio_io_rw_direct(enum HAL_SDIO_HOST_ID_T id, enum HAL_SDIO_DEVICE_RW read0_write1, uint8_t func_num,
                          uint32_t addr, uint8_t data, uint8_t *rsp);
int hal_sdio_io_rw_extended(enum HAL_SDIO_HOST_ID_T id, enum HAL_SDIO_DEVICE_RW read0_write1, uint8_t func_num,
                            uint32_t addr, uint8_t *rsp, uint8_t incr_addr, uint8_t *data_buff, uint32_t data_size);

enum HAL_SDIO_ERR hal_sdio_get_cccr_version(enum HAL_SDIO_HOST_ID_T id, uint8_t *cccr_version);
enum HAL_SDIO_ERR hal_sdio_get_sdio_version(enum HAL_SDIO_HOST_ID_T id, uint8_t *sdio_version);
enum HAL_SDIO_ERR hal_sdio_set_device_bus_width(enum HAL_SDIO_HOST_ID_T id, uint8_t bus_width);
enum HAL_SDIO_ERR hal_sdio_get_bus_width(enum HAL_SDIO_HOST_ID_T id, uint8_t *bus_width);
enum HAL_SDIO_ERR hal_sdio_get_cis_ptr(enum HAL_SDIO_HOST_ID_T id, uint8_t func_num, uint32_t *ptr_addr);
enum HAL_SDIO_ERR hal_sdio_cis_parse(enum HAL_SDIO_HOST_ID_T id, uint8_t func_num, uint32_t cis_ptr);
enum HAL_SDIO_ERR hal_sdio_enable_func(enum HAL_SDIO_HOST_ID_T id, uint8_t func_num);
enum HAL_SDIO_ERR hal_sdio_disable_func(enum HAL_SDIO_HOST_ID_T id, uint8_t func_num);
enum HAL_SDIO_ERR hal_sdio_enable_master_int(enum HAL_SDIO_HOST_ID_T id);
enum HAL_SDIO_ERR hal_sdio_disable_master_int(enum HAL_SDIO_HOST_ID_T id);
enum HAL_SDIO_ERR hal_sdio_enable_func_int(enum HAL_SDIO_HOST_ID_T id, uint8_t func_num);
enum HAL_SDIO_ERR hal_sdio_disable_func_int(enum HAL_SDIO_HOST_ID_T id, uint8_t func_num);
enum HAL_SDIO_ERR hal_sdio_set_block_size(enum HAL_SDIO_HOST_ID_T id, uint8_t func_num, uint16_t block_size);
enum HAL_SDIO_ERR hal_sdio_get_block_size(uint8_t func_num, uint16_t *block_size);
enum HAL_SDIO_ERR hal_sdio_get_int_pending(enum HAL_SDIO_HOST_ID_T id, uint8_t *int_pending);
enum HAL_SDIO_ERR hal_sdio_enable_asyn_int(enum HAL_SDIO_HOST_ID_T id);
enum HAL_SDIO_ERR hal_sdio_disable_asyn_int(enum HAL_SDIO_HOST_ID_T id);
enum HAL_SDIO_ERR hal_sdio_host_gen_int_to_device(enum HAL_SDIO_HOST_ID_T id);
enum HAL_SDIO_ERR hal_sdio_host_check_device_load_ready(enum HAL_SDIO_HOST_ID_T id, uint8_t *status);
enum HAL_SDIO_ERR hal_sdio_host_get_func1_rx_buf_cfg(enum HAL_SDIO_HOST_ID_T id, uint8_t *buf_cnt, uint16_t *buf_len);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_SDIO_HOST_H__ */


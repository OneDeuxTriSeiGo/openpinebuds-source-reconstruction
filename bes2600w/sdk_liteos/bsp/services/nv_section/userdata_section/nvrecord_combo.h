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
#ifndef __NVRECORD_COMBO_H__
#define __NVRECORD_COMBO_H__

#ifdef __cplusplus
extern "C"{
#endif

#ifdef COMBO_CUSBIN_IN_FLASH
/*****************************header include********************************/
#include "nvrecord_extension.h"

/******************************macro defination*****************************/

/******************************type defination******************************/

/****************************function declearation**************************/
void nv_record_combo_bin_rec_init(void);
void nv_record_combo_bin_clear_info(void);
bool nv_record_combo_bin_get_info(uint32_t id, uint32_t* offset, uint32_t* length);
void nv_record_combo_bin_update_info(void* combo_bin_info,uint32_t crc);
void* nv_record_combo_bin_rec_get_ptr(void);
void* nv_record_combo_bin_info_get_ptr(void);
uint16_t nv_record_combo_bin_get_hearlen(void);
uint32_t nv_record_combo_bin_get_content_num(void);
uint32_t nv_record_combo_bin_get_crc32(void);
uint32_t nv_record_combo_bin_get_datalen(uint8_t id);
uint32_t nv_record_combo_bin_get_fw_id(uint8_t id);
#endif

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __NVRECORD_COMBO_H__ */
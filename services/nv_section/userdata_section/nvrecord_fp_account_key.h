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
#if defined(NEW_NV_RECORD_ENABLED)

#ifdef GFPS_ENABLED

#ifndef __NVRECORD_FP_ACCOUNT_KEY_H__
#define __NVRECORD_FP_ACCOUNT_KEY_H__

#include "nvrecord_extension.h"

#ifdef __cplusplus
extern "C" {
#endif

void nv_record_fp_account_key_add(NV_FP_ACCOUNT_KEY_ENTRY_T* param_rec);
void nv_record_fp_account_key_delete();
void nv_record_fp_account_key_init(void);
bool nv_record_fp_account_key_get_by_index(uint8_t index, uint8_t* outputKey);
uint8_t nv_record_fp_account_key_count(void);
void nvrecord_rebuild_fp_account_key(NV_FP_ACCOUNT_KEY_RECORD_T* pFpAccountKey);
void nv_record_fp_update_name(uint8_t* ptrName, uint32_t nameLen);
uint8_t* nv_record_fp_get_name_ptr(uint32_t* ptrNameLen);
NV_FP_ACCOUNT_KEY_RECORD_T* nv_record_get_fp_data_structure_info(void);
void nv_record_update_fp_data_structure(NV_FP_ACCOUNT_KEY_RECORD_T* pFpData);

void nv_record_fp_update_all(uint8_t *info);

#ifdef __cplusplus
}
#endif
#endif

#endif

#endif // #if defined(NEW_NV_RECORD_ENABLED)

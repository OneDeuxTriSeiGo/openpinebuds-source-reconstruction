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
#ifndef __NVRECORD_PROMPT_H__
#define __NVRECORD_PROMPT_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************header include********************************/
#include "nvrecord_extension.h"

/******************************macro defination*****************************/
#define LANGUAGE_ID_MAX 0xFF
#define LANGUAGE_PACKAGE_SIZE (0x25800) //!< 150K for one language
#define DEFAULT_PROMPT_NUM 1
#define PROMPT_INFO_MAX 40

/******************************type defination******************************/
typedef struct
{
    uint32_t id;
    uint32_t offset;
    uint32_t length;
} PROMPT_CONTENT_INFO_T;

typedef struct
{
    uint32_t mainInfo;
    uint32_t version;
    uint32_t contentNum;
    PROMPT_CONTENT_INFO_T info[PROMPT_INFO_MAX];
    uint32_t crc32;
} PROMPT_IMAGE_HEADER_T;

/****************************function declearation**************************/
void nv_record_prompt_rec_init(void);

bool nv_record_prompt_rec_add_new_package(void *pInfo);

uint32_t nv_record_prompt_get_package_addr(uint8_t language, bool addNew, uint32_t newPackageLen);

void nv_record_prompt_get_prompt_info(uint8_t language, uint16_t id, uint8_t **data, uint32_t *length);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __NVRECORD_PROMPT_H__ */
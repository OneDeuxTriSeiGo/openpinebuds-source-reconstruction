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
#ifndef __MPU_H__
#define __MPU_H__

#include "plat_types.h"
#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The MPU region numbers
 */
#ifdef __MPU_REGION_NUM
#define MPU_ID_QTY      __MPU_REGION_NUM
#else
#define MPU_ID_QTY      (8)
#endif

#define MPU_INVALID_ID 0xFF

enum MPU_ATTR_T {
    MPU_ATTR_READ_WRITE_EXEC = 0,
    MPU_ATTR_READ_EXEC,
    MPU_ATTR_EXEC,
    MPU_ATTR_READ_WRITE,
    MPU_ATTR_READ,
    MPU_ATTR_NO_ACCESS,

    MPU_ATTR_QTY,
};

#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_7EM__ENHANCE__)

enum MAIR_ATTR_TYPE_T {
    MAIR_ATTR_FLASH,
    MAIR_ATTR_INT_SRAM,
    MAIR_ATTR_EXT_RAM,
    MAIR_ATTR_DEVICE,
    MAIR_ATTR_NC_MEM,
    MAIR_ATTR_5,
    MAIR_ATTR_6,
    MAIR_ATTR_7,

    MAIR_ATTR_QTY,
};
#endif

typedef struct
{
    uint32_t addr;
    uint32_t len;
    enum MPU_ATTR_T ap_attr;
#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_7EM__ENHANCE__)
    enum MAIR_ATTR_TYPE_T mem_attr;
#endif
} mpu_regions_t;

int mpu_open(void);

int mpu_close(void);

// VALID LENGTH: 32, 64, 128, 256, 512, 1K, 2K, ..., 4G
// ADDR must be aligned to len
int mpu_set(uint8_t id, uint32_t addr, uint32_t len, enum MPU_ATTR_T attr);

int mpu_clear(uint8_t id);

/*mpu setup for mcu */
int mpu_setup(const mpu_regions_t *mpu_table, uint32_t region_num);

/*mpu setup for cp mcu */
int mpu_setup_cp(const mpu_regions_t *mpu_table, uint32_t region_num);

uint8_t mpu_alloc_region(void);

void mpu_free_region(uint8_t id);

void mpu_sleep(void);

void mpu_wakeup(void);

#ifdef __cplusplus
}
#endif

#endif

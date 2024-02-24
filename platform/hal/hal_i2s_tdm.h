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
#ifndef __HAL_I2S_TDM_H__
#define __HAL_I2S_TDM_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "hal_i2s.h"
#include "hal_tdm.h"

#define I2S_TDM_BUF_ALIGN __attribute__((aligned(0x100)))

enum HAL_I2S_TDM_CYCLES_T {
    HAL_I2S_TDM_CYCLES_16   = HAL_TDM_CYCLES_16,
    HAL_I2S_TDM_CYCLES_32   = HAL_TDM_CYCLES_32,
    HAL_I2S_TDM_CYCLES_64   = HAL_TDM_CYCLES_64,
    HAL_I2S_TDM_CYCLES_128  = HAL_TDM_CYCLES_128,
    HAL_I2S_TDM_CYCLES_256  = HAL_TDM_CYCLES_256,
    HAL_I2S_TDM_CYCLES_512  = HAL_TDM_CYCLES_512,
    HAL_I2S_TDM_CYCLES_NUM,
};

enum HAL_I2S_TDM_FS_CYCLES_T {
    HAL_I2S_TDM_FS_CYCLES_ONE_LESS = HAL_TDM_FS_CYCLES_ONE_LESS,
    HAL_I2S_TDM_FS_CYCLES_1    = HAL_TDM_FS_CYCLES_1,
    HAL_I2S_TDM_FS_CYCLES_8   = HAL_TDM_FS_CYCLES_8,
    HAL_I2S_TDM_FS_CYCLES_16  = HAL_TDM_FS_CYCLES_16,
    HAL_I2S_TDM_FS_CYCLES_32  = HAL_TDM_FS_CYCLES_32,
    HAL_I2S_TDM_FS_CYCLES_64  = HAL_TDM_FS_CYCLES_64,
    HAL_I2S_TDM_FS_CYCLES_128 = HAL_TDM_FS_CYCLES_128,
    HAL_I2S_TDM_FS_CYCLES_256 = HAL_TDM_FS_CYCLES_256,
};

enum HAL_I2S_TDM_SLOT_CYCLES_T {
    HAL_I2S_TDM_SLOT_CYCLES_32 = HAL_TDM_SLOT_CYCLES_32,
    HAL_I2S_TDM_SLOT_CYCLES_16 = HAL_TDM_SLOT_CYCLES_16,
};

struct HAL_I2S_TDM_CONFIG_T {
    enum HAL_I2S_TDM_CYCLES_T cycles;
    enum HAL_I2S_TDM_FS_CYCLES_T fs_cycles;
    enum HAL_I2S_TDM_SLOT_CYCLES_T slot_cycles;
    uint32_t data_offset;
};

int32_t hal_i2s_tdm_open(enum HAL_I2S_ID_T i2s_id,enum HAL_I2S_MODE_T mode);
int32_t hal_i2s_tdm_setup(enum HAL_I2S_ID_T i2s_id,
                                 uint32_t sample_rate,
                                 struct HAL_I2S_TDM_CONFIG_T *i2s_tdm_cfg);
int32_t hal_i2s_tdm_start_stream(enum HAL_I2S_ID_T i2s_id);
int32_t hal_i2s_tdm_stop_stream(enum HAL_I2S_ID_T i2s_id);
int32_t hal_i2s_tdm_close(enum HAL_I2S_ID_T i2s_id);
void hal_i2s_tdm_set_config(enum HAL_I2S_ID_T i2s_id,struct HAL_I2S_TDM_CONFIG_T *tdm_cfg);
void hal_i2s_tdm_get_config(enum HAL_I2S_ID_T i2s_id,struct HAL_I2S_TDM_CONFIG_T *tdm_cfg);

#ifdef __cplusplus
}
#endif

#endif // __HAL_I2S_TDM_H__

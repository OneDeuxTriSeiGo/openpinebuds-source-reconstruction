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
#ifndef __HAL_SPDIF_H__
#define __HAL_SPDIF_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CHIP_HAS_SPDIF

#include "plat_types.h"
#include "hal_cmu.h"
#include "hal_aud.h"

struct HAL_SPDIF_CONFIG_T {
    uint32_t bits;
    uint32_t channel_num;
    uint32_t sample_rate;

    uint32_t use_dma;
};

int hal_spdif_open(enum HAL_SPDIF_ID_T id, enum AUD_STREAM_T stream);
int hal_spdif_close(enum HAL_SPDIF_ID_T id, enum AUD_STREAM_T stream);
int hal_spdif_start_stream(enum HAL_SPDIF_ID_T id, enum AUD_STREAM_T stream);
int hal_spdif_stop_stream(enum HAL_SPDIF_ID_T id, enum AUD_STREAM_T stream);
int hal_spdif_setup_stream(enum HAL_SPDIF_ID_T id, enum AUD_STREAM_T stream, struct HAL_SPDIF_CONFIG_T *cfg);
int hal_spdif_send(enum HAL_SPDIF_ID_T id, uint8_t *value, uint32_t value_len);
uint8_t hal_spdif_recv(enum HAL_SPDIF_ID_T id, uint8_t *value, uint32_t value_len);

int hal_spdif_clock_out_enable(enum HAL_SPDIF_ID_T id, uint32_t div);
int hal_spdif_clock_out_disable(enum HAL_SPDIF_ID_T id);

#endif // CHIP_HAS_SPDIF

#ifdef __cplusplus
}
#endif

#endif

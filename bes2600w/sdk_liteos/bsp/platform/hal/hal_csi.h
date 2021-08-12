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
#ifndef __HAL_CSI_H__
#define __HAL_CSI_H__

#ifdef __cplusplus
extern "C" {
#endif

enum CSI_PIX_FMT_T {
    CSI_PIX_FMT_RBB565      = 0,
    CSI_PIX_FMT_RGB888      = 1,
    CSI_PIX_FMT_YUV422      = 2,
    CSI_PIX_FMT_RGBA888     = 3,
};

void hal_csi_init(void);
void hal_csi_start(enum CSI_PIX_FMT_T fmt, uint32_t width, uint32_t height, uint32_t addr);
void hal_csi_stop(void);
void hal_csi_restart(void);
#ifdef __cplusplus
}
#endif

#endif


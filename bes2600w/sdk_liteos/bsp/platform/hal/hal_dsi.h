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
#ifndef __HAL_DSI_H__
#define __HAL_DSI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

struct HAL_DSI_CFG_T {
    uint32_t active_width;
    uint32_t active_height;//screen size
    uint32_t h_front_porch;
    uint32_t h_back_porch;
    uint32_t v_front_porch;
    uint32_t v_back_porch;
    uint32_t total_width;
    uint32_t total_height;

    // pn video
    uint32_t y_mem_pitch;
    uint32_t uv_mem_pitch;
    uint32_t c_mem_pitch;
    uint32_t image_h_sa;
    uint32_t image_v_sa;
    uint32_t image_width;
    uint32_t image_height;
    uint32_t zm_image_width;
    uint32_t zm_image_height;

    // pn graphic
    uint32_t g_mem_pitch;
    uint32_t graphic_h_sa;
    uint32_t graphic_v_sa;
    uint32_t graphic_width;
    uint32_t graphic_height;
    uint32_t zm_graphic_width;
    uint32_t zm_graphic_height;

    // tv graphic
    uint32_t g_tv_mem_pitch;
    uint32_t tvg_h_sa;
    uint32_t tvg_v_sa;
    uint32_t tvg_width;
    uint32_t tvg_height;
    uint32_t zm_tvg_width;
    uint32_t zm_tvg_height;

    // pn cursor
    uint32_t cursor_h_sa;
    uint32_t cursor_v_sa;
    uint32_t cursor_width;
    uint32_t cursor_height;
    int hwc_color1;
    int hwc_color2;

    //background
    int blankcolor;

    // pn viedo setting
    int cos0;
    int sin0;
    int c_mult_s;
    int saturation;
    int brightness;
    int contrast;

    // pn keys
    int cfg_alpha_y;
    int cfg_ckey_y;
    int cfg_ckey_y1;
    int cfg_ckey_y2;
    int cfg_alpha_u;
    int cfg_ckey_u;
    int cfg_ckey_u1;
    int cfg_ckey_u2;
    int cfg_alpha_v;
    int cfg_ckey_v;
    int cfg_ckey_v1;
    int cfg_ckey_v2;
};

void hal_dsi_init(uint16_t h_res);

void hal_dsi_start(void);

void hal_lcdc_init(const struct HAL_DSI_CFG_T *cfg, const uint8_t *layer0,
    const uint8_t *layer1, const uint8_t *layer2);

void hal_lcdc_gamma_enable(const uint8_t * config_R, const uint8_t * config_G, const uint8_t * config_B);

void hal_lcdc_gamma_disable(void);

void hal_lcdc_start(void);

void hal_dsi_send_cmd(uint8_t cmd);
void hal_dsi_read_cmd(uint8_t cmd,uint8_t* read_buf,uint8_t len);
void hal_dsi_send_cmd_data(uint8_t cmd, uint32_t len, uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3);
void hal_dsi_send_long_array(uint32_t len,uint32_t *data);
void hal_dsi_send_cmd_list(unsigned cmd, unsigned char para_count, unsigned char *para_list);

#ifdef __cplusplus
}
#endif

#endif


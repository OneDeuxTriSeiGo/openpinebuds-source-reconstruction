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
#include "mipi_dsi.h"
#include "cmsis_os2.h"
#include "plat_addr_map.h"
#include "hal_cache.h"
#include "hal_dsi.h"
#include "hdf_log.h"
#include "mipi_dsi_core.h"

#ifndef PSRAMUHS_SIZE
#define PSRAMUHS_SIZE 0x800000
#endif

#define WIDTH 454
#define HEIGHT 454
#define BUFSIZE_MAX (1024 * 1024)

struct MipiDsiDevice {
    uint32_t buffers[2];
    uint32_t buf_size;
    volatile uint8_t buf_idx;
    bool te_enable;
    osSemaphoreId_t sem;
    struct HAL_DSI_CFG_T cfg;
};

static struct MipiDsiDevice priv = {
    .buffers = {
        PSRAMUHS_BASE + PSRAMUHS_SIZE - BUFSIZE_MAX,
        PSRAMUHS_BASE + PSRAMUHS_SIZE - BUFSIZE_MAX * 2,
    },
    .buf_size = BUFSIZE_MAX,
    .buf_idx = 0,
    .te_enable = true,
    .cfg = {
        .active_width = WIDTH + 1,
        .active_height = HEIGHT + 1,
        .h_front_porch = 13,
        .h_back_porch = 13,
        .v_front_porch = 13,
        .v_back_porch = 13,
        .total_width = WIDTH + 32,
        .total_height = HEIGHT + 32,

        .y_mem_pitch = WIDTH * 4,
        .uv_mem_pitch = 0,
        .c_mem_pitch = 0,
        .image_h_sa = 1,
        .image_v_sa = 1,
        .image_width = WIDTH,
        .image_height = HEIGHT,
        .zm_image_width = WIDTH,
        .zm_image_height = HEIGHT,

        .g_mem_pitch = WIDTH * 4,
        .graphic_h_sa = 1,
        .graphic_v_sa = 1,
        .graphic_width = WIDTH,
        .graphic_height = HEIGHT,
        .zm_graphic_width = WIDTH,
        .zm_graphic_height = HEIGHT,

        .g_tv_mem_pitch = WIDTH * 4,
        .tvg_h_sa = 1,
        .tvg_v_sa = 1,
        .tvg_width = WIDTH,
        .tvg_height = HEIGHT,
        .zm_tvg_width = WIDTH,
        .zm_tvg_height = HEIGHT,

        .cursor_h_sa = 90,
        .cursor_v_sa = 90,
        .cursor_width = 28,
        .cursor_height = 28,
        .hwc_color1 = 0x0000ff,
        .hwc_color2 = 0x00ff00,

        .blankcolor = 0x104060,

        .cos0 = 0x3fc2,
        .sin0 = 0x05a4,
        .c_mult_s = 0x3000,
        .saturation = 0x5000,
        .brightness = 0xfff8,
        .contrast = 0x5000,

        .cfg_alpha_y = 0x00,
        .cfg_ckey_y = 0x16,
        .cfg_ckey_y1 = 0x0,
        .cfg_ckey_y2 = 0x60,
        .cfg_alpha_u = 0xff,
        .cfg_ckey_u = 0x80,
        .cfg_ckey_u1 = 0x0,
        .cfg_ckey_u2 = 0x80,
        .cfg_alpha_v = 0xff,
        .cfg_ckey_v = 0x80,
        .cfg_ckey_v1 = 0x0,
        .cfg_ckey_v2 = 0x80,
    },
};

static int32_t MipiDsiDevInit()
{
    for (int i = 0; i < HDF_ARRAY_SIZE(priv.buffers); i++) {
        memset((void *)priv.buffers[i], 0, BUFSIZE_MAX);
        hal_cache_sync(HAL_CACHE_ID_D_CACHE, (uint32_t)priv.buffers[i], BUFSIZE_MAX);
        HDF_LOGD("buffer[%d] 0x%x", i, priv.buffers[i]);
    }
    if (priv.te_enable) {
        priv.sem = osSemaphoreNew(1, 1, NULL); // init sem avaliable
        if (priv.sem == NULL) {
            HDF_LOGE("%s: osSemaphoreNew failed", __func__);
            return HDF_FAILURE;
        }
    }
    return HDF_SUCCESS;
}

static int32_t MipiDsiDevSetCntlrCfg(struct MipiDsiCntlr *cntlr)
{
    priv.cfg.active_width = cntlr->cfg.timing.xPixels;
    priv.cfg.active_height = cntlr->cfg.timing.ylines;
    // FIXME
    if (cntlr->cfg.mode == DSI_CMD_MODE) {
        priv.cfg.active_width += 1;
        priv.cfg.active_height += 1;
        hal_dsi_set_mode(DSI_MODE_CMD);
    } else if (cntlr->cfg.mode == DSI_VIDEO_MODE) {
        if (cntlr->cfg.burstMode == VIDEO_BURST_MODE) {
            priv.te_enable = false;
        }
        hal_dsi_set_mode(DSI_MODE_VIDEO);
    }
    priv.cfg.h_back_porch = cntlr->cfg.timing.hbpPixels;
    priv.cfg.v_front_porch = cntlr->cfg.timing.vfpLines;
    priv.cfg.v_back_porch = cntlr->cfg.timing.vbpLines;
    priv.cfg.total_width = cntlr->cfg.timing.hlinePixels;
    priv.cfg.total_height = cntlr->cfg.timing.ylines + cntlr->cfg.timing.vfpLines +
                            cntlr->cfg.timing.vbpLines + cntlr->cfg.timing.vsaLines;

    priv.cfg.y_mem_pitch = cntlr->cfg.timing.xPixels * 4;
    priv.cfg.image_width = cntlr->cfg.timing.xPixels;
    priv.cfg.image_height = cntlr->cfg.timing.ylines;
    priv.cfg.zm_image_width = cntlr->cfg.timing.xPixels;
    priv.cfg.zm_image_height = cntlr->cfg.timing.ylines;

    priv.cfg.g_mem_pitch = cntlr->cfg.timing.xPixels * 4;
    priv.cfg.graphic_width = cntlr->cfg.timing.xPixels;
    priv.cfg.graphic_height = cntlr->cfg.timing.ylines;
    priv.cfg.zm_graphic_width = cntlr->cfg.timing.xPixels;
    priv.cfg.zm_graphic_height = cntlr->cfg.timing.ylines;

    priv.cfg.g_tv_mem_pitch = cntlr->cfg.timing.xPixels * 4;
    priv.cfg.tvg_width = cntlr->cfg.timing.xPixels;
    priv.cfg.tvg_height = cntlr->cfg.timing.ylines;
    priv.cfg.zm_tvg_width = cntlr->cfg.timing.xPixels;
    priv.cfg.zm_tvg_height = cntlr->cfg.timing.ylines;

    HDF_LOGD("%s: width %d, height %d", __func__, priv.cfg.active_width, priv.cfg.active_height);
    /* Init the hardware and clear the display */
    hal_dsi_init(cntlr->cfg.timing.xPixels);
    osDelay(100);
    return HDF_SUCCESS;
}

static int32_t MipiDsiDevSetCmd(struct MipiDsiCntlr *cntlr, struct DsiCmdDesc *cmd)
{
    (void)cntlr;
    if (!cmd || !cmd->payload || cmd->dataLen < 1) {
        return HDF_ERR_INVALID_PARAM;
    }
    if (cmd->dataType == 0x05) {
        hal_dsi_send_cmd(cmd->payload[0]);
    } else {
        hal_dsi_send_cmd_list(cmd->payload[0], cmd->dataLen - 1, &cmd->payload[1]);
    }
    return HDF_SUCCESS;
}

static void MipiDsiDevCallback(uint32_t addr)
{
    osSemaphoreRelease(priv.sem);
}

static void MipiDsiDevToHs(struct MipiDsiCntlr *cntlr)
{
    (void)cntlr;
    hal_lcdc_init(&priv.cfg, NULL, (uint8_t *)priv.buffers[priv.buf_idx], NULL);
    hal_dsi_start();
    if (priv.te_enable) {
        hal_lcdc_set_callback(MipiDsiDevCallback);
    } else {
        hal_lcdc_start();
    }
}

void *MipiDsiDevMmap(uint32_t size)
{
    if (size > BUFSIZE_MAX) {
        HDF_LOGE("%s: invalid size 0x%x", __func__, size);
        return NULL;
    }
    priv.buf_size = size;
    // psram need to sync cache
    hal_cache_sync(HAL_CACHE_ID_D_CACHE, (uint32_t)priv.buffers[priv.buf_idx], size);
    return (void *)(priv.buffers[priv.buf_idx]);
}

void MipiDsiDevFlush(void)
{
    // psram need to sync cache
    hal_cache_sync(HAL_CACHE_ID_D_CACHE, (uint32_t)priv.buffers[priv.buf_idx], priv.buf_size);
    if (priv.te_enable) {
        // wait for last transmission done
        if (osSemaphoreAcquire(priv.sem, 100) != 0) {
            HDF_LOGW("last transmission timeout");
            hal_lcdc_start(); // force to restart
            return;
        }
        // swap buffer
        hal_lcdc_update_addr(NULL, (const void *)priv.buffers[priv.buf_idx], NULL);
        priv.buf_idx = 1 - priv.buf_idx;
        // start transmission
        hal_lcdc_start();
    } else {
        static uint32_t last_time = 0;
        uint32_t now = osKernelGetTickCount();
        uint32_t diff = now - last_time;
        if (diff < 16) {
            HDF_LOGD("wait to finish last transmission");
            osDelay(16 - diff);
        }
        last_time = osKernelGetTickCount();
        // swap buffer
        hal_lcdc_update_addr(NULL, (const void *)priv.buffers[priv.buf_idx], NULL);
        priv.buf_idx = 1 - priv.buf_idx;
    }
}

static int32_t MipiDsiDriverBind(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    return HDF_SUCCESS;
}

static int32_t MipiDsiDriverInit(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        return HDF_ERR_INVALID_OBJECT;
    }

    int ret = MipiDsiDevInit();
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: MipiDsiInit failed", __func__);
        return ret;
    }
    static struct MipiDsiCntlrMethod mipiDsiMethod = {
        .setCntlrCfg = MipiDsiDevSetCntlrCfg,
        .setCmd = MipiDsiDevSetCmd,
        .toHs = MipiDsiDevToHs,
    };
    static struct MipiDsiService mipiDsiService = {
        .flush = MipiDsiDevFlush,
        .mmap = MipiDsiDevMmap,
    };
    static struct MipiDsiCntlr mipiDsiCntlr = {
        .devNo = 0,
        .ops = &mipiDsiMethod,
    };
    ret = MipiDsiRegisterCntlr(&mipiDsiCntlr, device);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: MipiDsiRegisterCntlr failed", __func__);
        return ret;
    }
    mipiDsiCntlr.priv = &mipiDsiService; // must after MipiDsiRegisterCntlr
    return ret;
}

static void MipiDsiDriverRelease(struct HdfDeviceObject *device)
{
    struct MipiDsiCntlr *mipiDsiCntlr = MipiDsiCntlrFromDevice(device);
    if (mipiDsiCntlr) {
        MipiDsiUnregisterCntlr(mipiDsiCntlr);
        mipiDsiCntlr = NULL;
    }
    if (priv.sem) {
        osSemaphoreDelete(priv.sem);
        priv.sem = NULL;
    }
}

static struct HdfDriverEntry g_MipiDsiDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_PLATFORM_MIPI_DSI",
    .Bind = MipiDsiDriverBind,
    .Init = MipiDsiDriverInit,
    .Release = MipiDsiDriverRelease,
};
HDF_INIT(g_MipiDsiDriverEntry);

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
#include "device_resource_if.h"
#include "hal_cache.h"
#include "hal_cmu.h"
#include "hal_dsi.h"
#include "hal_gpio.h"
#include "hal_iomux.h"
#include "hdf_log.h"
#include "mipi_dsi_core.h"

#ifndef PSRAMUHS_SIZE
#define PSRAMUHS_SIZE 0x800000
#endif

#define WIDTH 454
#define HEIGHT 454
#define BUFSIZE_MAX 0xCA000

enum BufferIndexType {
    BUFFER0,
    BUFFER1,
};

struct dsi_priv {
    struct HAL_IOMUX_PIN_FUNCTION_MAP dsi_rst;
#ifdef EVB_V2
    struct HAL_IOMUX_PIN_FUNCTION_MAP dsi_vdd;
    struct HAL_IOMUX_PIN_FUNCTION_MAP dsi_vss;
#endif
    struct HAL_IOMUX_PIN_FUNCTION_MAP dsi_te;
    uint32_t buffers[2];
    uint32_t buf_size;
    volatile enum BufferIndexType buf_idx;
    osSemaphoreId_t sem;
    struct HAL_DSI_CFG_T cfg;
};

static struct dsi_priv priv = {
    .dsi_rst = {
        HAL_GPIO_PIN_P0_3,
        HAL_IOMUX_FUNC_AS_GPIO,
        HAL_IOMUX_PIN_VOLTAGE_VIO,
        HAL_IOMUX_PIN_NOPULL,
    },
#ifdef EVB_V2
    .dsi_vdd = {
        HAL_GPIO_PIN_P1_4,
        HAL_IOMUX_FUNC_AS_GPIO,
        HAL_IOMUX_PIN_VOLTAGE_VIO,
        HAL_IOMUX_PIN_NOPULL,
    },
    .dsi_vss = {
        HAL_GPIO_PIN_P1_5,
        HAL_IOMUX_FUNC_AS_GPIO,
        HAL_IOMUX_PIN_VOLTAGE_VIO,
        HAL_IOMUX_PIN_NOPULL,
    },
#endif
    .dsi_te = {
        HAL_IOMUX_PIN_P2_1,
        HAL_IOMUX_FUNC_DISPLAY_TE,
        HAL_IOMUX_PIN_VOLTAGE_VIO,
        HAL_IOMUX_PIN_NOPULL,
    },
    .buffers = {
        PSRAMUHS_BASE + PSRAMUHS_SIZE - BUFSIZE_MAX * 2,
        PSRAMUHS_BASE + PSRAMUHS_SIZE - BUFSIZE_MAX,
    },
    .buf_size = BUFSIZE_MAX,
    .buf_idx = BUFFER0,
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

#define REAL_PIN(n) (n / 10 * 8 + n % 10)
static uint32_t MipiDsiDevGetResource(struct dsi_priv *priv, const struct DeviceResourceNode *resourceNode)
{
    struct DeviceResourceIface *res = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (res == NULL || res->GetUint32 == NULL) {
        HDF_LOGE("DeviceResourceIface is invalid");
        return HDF_FAILURE;
    }
    uint32_t temp = 0;
    if (res->GetUint32(resourceNode, "rst_pin", &temp, 0) != HDF_SUCCESS) {
        HDF_LOGE("failed to get dsi rst_pin");
        return HDF_FAILURE;
    }
    priv->dsi_rst.pin = REAL_PIN(temp);
#ifdef EVB_V2
    if (res->GetUint32(resourceNode, "vdd_pin", &temp, 0) != HDF_SUCCESS) {
        HDF_LOGE("failed to get dsi vdd_pin");
        return HDF_FAILURE;
    }
    priv->dsi_vdd.pin = REAL_PIN(temp);

    if (res->GetUint32(resourceNode, "vss_pin", &temp, 0) != HDF_SUCCESS) {
        HDF_LOGE("failed to get dsi vss_pin");
        return HDF_FAILURE;
    }
    priv->dsi_vss.pin = REAL_PIN(temp);
#endif
    if (res->GetUint32(resourceNode, "te_pin", &temp, 0) != HDF_SUCCESS) {
        HDF_LOGE("failed to get dsi te_pin");
        return HDF_FAILURE;
    }
    priv->dsi_te.pin = REAL_PIN(temp);
    HDF_LOGD("%s: rst_pin=%d, te_pin=%d", __func__, priv->dsi_rst.pin, priv->dsi_te.pin);
    return HDF_SUCCESS;
}

static void MipiDsiMuxCfg(void)
{
    hal_iomux_init(&priv.dsi_rst, 1);
#ifdef EVB_V2
    hal_iomux_init(&priv.dsi_vdd, 1);
    hal_iomux_init(&priv.dsi_vss, 1);
#endif
    hal_iomux_init(&priv.dsi_te, 1);
    hal_gpio_pin_set_dir(priv.dsi_rst.pin, HAL_GPIO_DIR_OUT, 0);
#ifdef EVB_V2
    hal_gpio_pin_set_dir(priv.dsi_vdd.pin, HAL_GPIO_DIR_OUT, 0);
    hal_gpio_pin_set_dir(priv.dsi_vss.pin, HAL_GPIO_DIR_OUT, 0);
    hal_gpio_pin_set(priv.dsi_vdd.pin);
    osDelay(20);
    hal_gpio_pin_set(priv.dsi_vss.pin);
#endif
    osDelay(20);
    hal_gpio_pin_set(priv.dsi_rst.pin);
    osDelay(20);
}

static int32_t MipiDsiDevInit()
{
    if (priv.sem) {
        return HDF_SUCCESS;
    }
    MipiDsiMuxCfg();
    for (int i = 0; i < HDF_ARRAY_SIZE(priv.buffers); i++) {
        memset((void *)priv.buffers[i], 0, BUFSIZE_MAX);
        hal_cache_sync(HAL_CACHE_ID_D_CACHE, (uint32_t)priv.buffers[i], BUFSIZE_MAX);
        HDF_LOGD("buffer[%d] 0x%x", i, priv.buffers[i]);
    }

    priv.sem = osSemaphoreNew(1, 1, NULL); // init sem avaliable
    if (priv.sem == NULL) {
        HDF_LOGE("%s: osSemaphoreNew failed", __func__);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t MipiDsiDevSetCntlrCfg(struct MipiDsiCntlr *cntlr)
{
    priv.cfg.active_width = cntlr->timing.xPixels + 1;
    priv.cfg.active_height = cntlr->timing.ylines + 1;
    priv.cfg.h_back_porch = cntlr->timing.hbpPixels;
    priv.cfg.v_front_porch = cntlr->timing.vfpLines;
    priv.cfg.v_back_porch = cntlr->timing.vbpLines;
    priv.cfg.total_width = cntlr->timing.hlinePixels;
    priv.cfg.total_height = cntlr->timing.ylines + cntlr->timing.vfpLines + cntlr->timing.vbpLines + cntlr->timing.vsaLines;

    priv.cfg.y_mem_pitch = cntlr->timing.xPixels * 4;
    priv.cfg.image_width = cntlr->timing.xPixels;
    priv.cfg.image_height = cntlr->timing.ylines;
    priv.cfg.zm_image_width = cntlr->timing.xPixels;
    priv.cfg.zm_image_height = cntlr->timing.ylines;

    priv.cfg.g_mem_pitch = cntlr->timing.xPixels * 4;
    priv.cfg.graphic_width = cntlr->timing.xPixels;
    priv.cfg.graphic_height = cntlr->timing.ylines;
    priv.cfg.zm_graphic_width = cntlr->timing.xPixels;
    priv.cfg.zm_graphic_height = cntlr->timing.ylines;

    priv.cfg.g_tv_mem_pitch = cntlr->timing.xPixels * 4;
    priv.cfg.tvg_width = cntlr->timing.xPixels;
    priv.cfg.tvg_height = cntlr->timing.ylines;
    priv.cfg.zm_tvg_width = cntlr->timing.xPixels;
    priv.cfg.zm_tvg_height = cntlr->timing.ylines;

    HDF_LOGD("%s: width %d, height %d", __func__, cntlr->timing.xPixels, cntlr->timing.ylines);
    /* Init the hardware and clear the display */
    hal_dsi_init(cntlr->timing.xPixels);
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
        uint8_t data[5] = {0};
        for (uint16_t i = 0; (i < cmd->dataLen) && (i < sizeof(data)); i++) {
            data[i] = cmd->payload[i];
        }
        hal_dsi_send_cmd_data(data[0], cmd->dataLen - 1, data[1], data[2], data[3], data[4]);
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
    hal_lcdc_set_callback(MipiDsiDevCallback);
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
}

static int32_t MipiDsiDriverBind(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        HDF_LOGE("%s: device is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    static struct MipiDsiService mipiDsiService = {
        .flush = MipiDsiDevFlush,
        .mmap = MipiDsiDevMmap,
    };
    device->service = &mipiDsiService.service;
    return HDF_SUCCESS;
}

static int32_t MipiDsiDriverInit(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        HDF_LOGE("%s: device is NULL", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    if (device->property) {
        if (MipiDsiDevGetResource(&priv, device->property) != HDF_SUCCESS) {
            HDF_LOGE("%s: MipiDsiDevGetResource failed", __func__);
            return HDF_FAILURE;
        }
    }

    int ret = MipiDsiDevInit();
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: MipiDsiInit failed", __func__);
        return ret;
    }

    static struct MipiDsiCntlr mipiDsiCntlr = {
        .devNo = 0,
        .setCntlrCfg = MipiDsiDevSetCntlrCfg,
        .setCmd = MipiDsiDevSetCmd,
        .toHs = MipiDsiDevToHs,
    };
    ret = MipiDsiRegisterCntlr(&mipiDsiCntlr);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: MipiDsiRegisterCntlr failed", __func__);
        return ret;
    }
    return ret;
}

static void MipiDsiDriverRelease(struct HdfDeviceObject *device)
{
    (void)device;
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

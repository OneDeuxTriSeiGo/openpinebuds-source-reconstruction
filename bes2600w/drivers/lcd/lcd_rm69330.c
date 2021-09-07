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
#include "hdf_log.h"
#include "lcd_abs_if.h"
#include "hdf_device_desc.h"

#define WIDTH 454
#define HEIGHT 454
#define FRAME_RATE 60
#define NPLANES 1
#define BPP 32

enum DsiCmdType {
    DSI_SHORT_WRITE = 0x05,
    DSI_SHORT_WRITE_1PAR = 0x15,
    DSI_LONG_WRITE = 0x39
};

static uint8_t g_payLoad0[] = {0xfe, 0x00};
static uint8_t g_payLoad1[] = {0x35, 0x00};
static uint8_t g_payLoad2[] = {0x36, 0x08}; // fix red/blue mirror
static uint8_t g_payLoad3[] = {0x51, 0xcf};
static uint8_t g_payLoad4[] = {0x2a, 0x00, 12, (WIDTH + 11) >> 8, (WIDTH + 11) & 0xff};
static uint8_t g_payLoad5[] = {0x2b, 0x00, 0x00, (HEIGHT - 1) >> 8, (HEIGHT - 1) & 0xff};
static uint8_t g_payLoad6[] = {0x11};
static uint8_t g_payLoad7[] = {0x29};

static struct DsiCmdDesc g_OnCmd[] = {
    {DSI_SHORT_WRITE_1PAR, 0, sizeof(g_payLoad0), g_payLoad0},
    {DSI_SHORT_WRITE_1PAR, 0, sizeof(g_payLoad1), g_payLoad1},
    {DSI_SHORT_WRITE_1PAR, 0, sizeof(g_payLoad2), g_payLoad2},
    {DSI_SHORT_WRITE_1PAR, 0, sizeof(g_payLoad3), g_payLoad3},
    {DSI_LONG_WRITE, 0, sizeof(g_payLoad4), g_payLoad4},
    {DSI_LONG_WRITE, 0, sizeof(g_payLoad5), g_payLoad5},
    {DSI_SHORT_WRITE, 240, sizeof(g_payLoad6), g_payLoad6},
    {DSI_SHORT_WRITE, 150, sizeof(g_payLoad7), g_payLoad7},
};

static struct DsiCmdDesc g_offCmd[] = {};

struct LcdRM69330 {
    struct PanelData panelData;
    struct PanelInfo panelInfo;
    DevHandle mipiHandle;
};

static struct LcdRM69330 priv = {
    .panelInfo = {
        .width = WIDTH,
        .height = HEIGHT,
        .hbp = 13,
        .hfp = 13,
        .vbp = 13,
        .vfp = 13,
        .frameRate = FRAME_RATE,
        .intfType = MIPI_DSI,
        .intfSync = OUTPUT_USER,
        .mipi = {DSI_1_LANES, DSI_VIDEO_MODE, VIDEO_BURST_MODE, FORMAT_YUV422_8_BIT},
    },
};

static int32_t LcdInit(struct PanelData *panel)
{
    if (panel == NULL || panel->info == NULL) {
        return HDF_FAILURE;
    }
    priv.mipiHandle = MipiDsiOpen(0);
    if (priv.mipiHandle == NULL) {
        HDF_LOGE("%s: MipiDsiOpen failed", __func__);
        return HDF_FAILURE;
    }
    HDF_LOGI("%s: width %d, height %d", __func__, WIDTH, HEIGHT);

    struct PanelInfo *panelInfo = panel->info;
    struct MipiCfg cfg = {0};
    cfg.lane = panelInfo->mipi.lane;
    cfg.mode = panelInfo->mipi.mode;
    cfg.format = panelInfo->mipi.format;
    cfg.burstMode = panelInfo->mipi.burstMode;
    cfg.timing.xPixels = panelInfo->width;
    cfg.timing.hsaPixels = panelInfo->hsw;
    cfg.timing.hbpPixels = panelInfo->hbp;
    cfg.timing.hlinePixels = panelInfo->width + panelInfo->hbp + panelInfo->hfp + panelInfo->hsw;
    cfg.timing.vsaLines = panelInfo->vsw;
    cfg.timing.vbpLines = panelInfo->vbp;
    cfg.timing.vfpLines = panelInfo->vfp;
    cfg.timing.ylines = panelInfo->height;

    cfg.pixelClk = CalcPixelClk(panelInfo);
    cfg.phyDataRate = CalcDataRate(panelInfo);
    /* config mipi device */
    if (MipiDsiSetCfg(priv.mipiHandle, &cfg) != HDF_SUCCESS) {
        HDF_LOGE("%s: MipiDsiSetCfg failed", __func__);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t LcdOn(struct PanelData *panel)
{
    (void)panel;
    /* send mipi power on code */
    int32_t count = sizeof(g_OnCmd) / sizeof(g_OnCmd[0]);
    int32_t i;
    for (i = 0; i < count; i++) {
        int32_t ret = MipiDsiTx(priv.mipiHandle, &(g_OnCmd[i]));
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%s: MipiDsiTx failed", __func__);
            return HDF_FAILURE;
        }
    }
    /* set mipi to hs mode */
    MipiDsiSetHsMode(priv.mipiHandle);
    return HDF_SUCCESS;
}

static int32_t LcdOff(struct PanelData *panel)
{
    (void)panel;
    /* send mipi power off code */
    int32_t count = sizeof(g_offCmd) / sizeof(g_offCmd[0]);
    int32_t i;
    for (i = 0; i < count; i++) {
        int32_t ret = MipiDsiTx(priv.mipiHandle, &(g_offCmd[i]));
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%s: MipiDsiTx failed", __func__);
            return HDF_FAILURE;
        }
    }
    MipiDsiSetLpMode(priv.mipiHandle);
    return HDF_SUCCESS;
}

static int32_t LcdSetBacklight(struct PanelData *panel, uint32_t level)
{
    (void)panel;
    uint8_t payload[] = {0x51, level & 0xff};
    struct DsiCmdDesc cmd = {DSI_SHORT_WRITE_1PAR, 0, sizeof(payload), payload};
    if (MipiDsiTx(priv.mipiHandle, &cmd) != HDF_SUCCESS) {
        HDF_LOGE("%s: MipiDsiTx failed", __func__);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t LcdDriverInit(struct HdfDeviceObject *object)
{
    if (object == NULL) {
        HDF_LOGE("%s: object is null", __func__);
        return HDF_FAILURE;
    }
    priv.panelData.info = &priv.panelInfo;
    priv.panelData.init = LcdInit;
    priv.panelData.on = LcdOn;
    priv.panelData.off = LcdOff;
    priv.panelData.setBacklight = LcdSetBacklight;
    priv.panelData.object = object;
    if (RegisterPanel(&priv.panelData) != HDF_SUCCESS) {
        HDF_LOGE("%s: RegisterPanel failed", __func__);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t LcdDriverBind(struct HdfDeviceObject *device)
{
    (void)device;
    HDF_LOGD("%s", __func__);
    return HDF_SUCCESS;
}

static void LcdDriverRelease(struct HdfDeviceObject *device)
{
    (void)device;
    HDF_LOGD("%s", __func__);
}

static struct HdfDriverEntry g_lcdDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_LCD_RM69330",
    .Bind = LcdDriverBind,
    .Init = LcdDriverInit,
    .Release = LcdDriverRelease,
};

HDF_INIT(g_lcdDriverEntry);

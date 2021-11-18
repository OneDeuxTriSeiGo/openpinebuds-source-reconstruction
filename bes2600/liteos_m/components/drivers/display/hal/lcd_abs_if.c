/*
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */
#include "lcd_abs_if.h"
#include "hdf_log.h"

#define TRANSFORM_KILO 1000
#define TRANSFORM_MILL 1000000

static struct PanelManager g_panelManager;

int32_t RegisterPanel(struct PanelData *data)
{
    int32_t panelNum;

    if (data == NULL) {
        HDF_LOGE("%s: panel data is null", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    panelNum = g_panelManager.panelNum;
    if (panelNum >= PANEL_MAX) {
        HDF_LOGE("%s registered panel up PANEL_MAX", __func__);
        return HDF_FAILURE;
    }
    g_panelManager.panel[panelNum] = data;
    g_panelManager.panelNum++;
    HDF_LOGI("%s: register success", __func__);
    return HDF_SUCCESS;
}

struct PanelManager *GetPanelManager(void)
{
    if (g_panelManager.panelNum == 0) {
        return NULL;
    } else {
        return &g_panelManager;
    }
}

struct PanelData *GetPanel(int32_t index)
{
    struct PanelManager *panelManager = NULL;

    panelManager = GetPanelManager();
    if (panelManager == NULL) {
        HDF_LOGE("%s panelManager is null", __func__);
        return NULL;
    }
    if (index >= g_panelManager.panelNum) {
        HDF_LOGE("%s index is greater than g_panelManager.panelNum", __func__);
        return NULL;
    }
    return panelManager->panel[index];
}

int32_t GetBitsPerPixel(enum DsiOutFormat format)
{
    int32_t bpp;

    switch (format) {
    case FORMAT_RGB_16_BIT:
        bpp = 16;
        break;
    case FORMAT_RGB_18_BIT:
        bpp = 18;
        break;
    case FORMAT_RGB_24_BIT:
        bpp = 24;
        break;
    default:
        bpp = 32;
        break;
    }
    return bpp;
}

uint32_t CalcPixelClk(struct PanelInfo *info)
{
    uint16_t hpixel;
    uint16_t vline;

    hpixel = info->width + info->hbp + info->hfp + info->hsw;
    vline = info->height + info->vbp + info->vfp + info->vsw;
    uint32_t pixNum = hpixel * vline * info->frameRate;
    if ((pixNum % TRANSFORM_KILO) == 0) {
        return pixNum / TRANSFORM_KILO;
    }
    return (pixNum / TRANSFORM_KILO + 1);
}

uint32_t CalcDataRate(struct PanelInfo *info)
{
    uint16_t hpixel;
    uint16_t vline;
    uint32_t bitClk;

    hpixel = info->width + info->hbp + info->hfp + info->hsw;
    vline = info->height + info->vbp + info->vfp + info->vsw;
    int32_t bpp = GetBitsPerPixel(info->mipi.format);
    uint32_t bitNum = hpixel * vline * info->frameRate * bpp;
    if ((bitNum % TRANSFORM_MILL) == 0) {
        bitClk = bitNum / TRANSFORM_MILL;
    } else {
        bitClk = bitNum / TRANSFORM_MILL + 1;
    }
    if (info->mipi.lane == 0) {
        info->mipi.lane = 1; // default 1 lane
    }
    if ((bitClk % info->mipi.lane) == 0) {
        return bitClk / info->mipi.lane;
    }
    return bitClk / info->mipi.lane + 1;
}

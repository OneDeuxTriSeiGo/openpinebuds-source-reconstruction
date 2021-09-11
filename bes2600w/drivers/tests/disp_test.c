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
#include "cmsis_os2.h"
#include "disp_hal.h"
#include "platform.h"

static struct DispInfo info = {0};
static void DispFill(void *buffer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color)
{
    int32_t x;
    int32_t y;
    uint32_t *framebuffer = (uint32_t *)buffer;
    for (y = y1; y <= y2; y++) {
        for (x = x1; x <= x2; x++) {
            framebuffer[x + info.width * y] = color;
        }
    }
}

#ifdef DISP_FILL_TEST
void display_test(void)
{
    LOG_I("DISP_FILL_TEST");
    DBG_ASSERT(DispInit(0) == 0);
    DBG_ASSERT(DispOn(0) == 0);
    DBG_ASSERT(DispGetInfo(0, &info) == 0);
    LOG_I("disp init width %d, height %d", info.width, info.height);

    uint32_t now = GetSysTime();
    uint32_t cnt = 0;
    while (1) {
        void *fbAddr = DispMmap(info.width * info.height * 4);
        uint32_t cur = GetSysTime();
        DispFill(fbAddr, 0, 0, info.width - 1, info.height - 1, 0xff);
        LOG_I("fill used %u ms", SysTimeDiff(cur));
        cur = GetSysTime();
        DispFlush();
        LOG_I("flush used %u ms", SysTimeDiff(cur));
        cnt++;
        uint32_t diff = SysTimeDiff(now);
        if (diff >= 1000) {
            LOG_I("%u fps", 1000 * cnt / diff);
            cnt = 0;
            now = GetSysTime();
        }
    }
}
#else
void display_test(void)
{
    int16_t pos_x = 100;
    LOG_I("ANIMATOR_TEST");
    DispInit(0);
    DispOn(0);
    DispGetInfo(0, &info);
    LOG_I("disp init width %d, height %d", info.width, info.height);

    uint32_t now = GetSysTime();
    uint32_t cnt = 0;

    while (1) {
        void *fbAddr = DispMmap(info.width * info.height * 4);
        DispFill(fbAddr, 0, 100, info.width - 1, 200, 0);
        pos_x += 10;
        if (pos_x >= info.width - 100) {
            pos_x = 0;
        }
        uint32_t cur = GetSysTime();
        DispFill(fbAddr, pos_x, 100, pos_x + 100, 200, 0xff00);
        LOG_I("fill used %u ms", SysTimeDiff(cur));
        cur = GetSysTime();
        DispFlush();
        LOG_I("flush used %u ms", SysTimeDiff(cur));
        cnt++;
        uint32_t diff = SysTimeDiff(now);
        if (diff >= 1000) {
            LOG_I("%u fps", 1000 * cnt / diff);
            cnt = 0;
            now = GetSysTime();
        }
    }
}
#endif
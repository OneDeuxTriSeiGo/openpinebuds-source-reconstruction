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
#ifndef __USB_DISKIO__
#define __USB_DISKIO__

#ifdef __cplusplus
extern "C" {
#endif

#include "diskio.h"

DSTATUS usb_disk_status();
DSTATUS usb_disk_initialize();
DRESULT usb_disk_write(const BYTE* buffer, LBA_t block_number, UINT count);
DRESULT usb_disk_read(BYTE* buffer, LBA_t block_number, UINT count);
int     usb_disk_sync();
LBA_t   usb_disk_get_sectors();
DRESULT usb_disk_ioctl(u8 ctrl, void *buff);

#ifdef __cplusplus
}
#endif

#endif

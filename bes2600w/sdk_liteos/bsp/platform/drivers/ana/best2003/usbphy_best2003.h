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
#ifndef __USBPHY_BEST2003_H__
#define __USBPHY_BEST2003_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SPIPHY_USB_CS                       0

#define SPIPHY_USB_REG(reg)                 (((reg) & 0xFFF) | (SPIPHY_USB_CS << 12))

void usbphy_div_reset_set();

void usbphy_div_reset_clear(uint32_t div);

void usbphy_div_set(uint32_t div);

#ifdef __cplusplus
}
#endif

#endif


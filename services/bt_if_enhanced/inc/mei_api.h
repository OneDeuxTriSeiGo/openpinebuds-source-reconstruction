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
#ifndef __MEI_API__H__
#define __MEI_API__H__

/*---------------------------------------------------------------------------
 *
 * Authenticate State of a Remote Device (BtAuthState). The typedef is in
 * mei_api.h
 */
#define BTIF_BAS_NOT_AUTHENTICATED  0x00
#define  BTIF_BAS_START_AUTHENTICATE 0x01
#define  BTIF_BAS_WAITING_KEY_REQ    0x02
#define  BTIF_BAS_SENDING_KEY        0x03
#define  BTIF_BAS_WAITING_FOR_IO     0x04
#define  BTIF_BAS_WAITING_FOR_IO_R   0x05
#define  BTIF_BAS_WAITING_FOR_KEY    0x06
#define  BTIF_BAS_WAITING_FOR_KEY_R  0x07
#define  BTIF_BAS_AUTHENTICATED      0x08

#define BTIF_DBG_SNIFFER_SCO_STOP  0
#define BTIF_DBG_SNIFFER_SCO_START  1
#define BTIF_DBG_SNIFFER_ACL_SWOTCH  2
#define BTIF_DBG_SEND_MAX_SLOT_REQ  3

#endif /* __ME_H */

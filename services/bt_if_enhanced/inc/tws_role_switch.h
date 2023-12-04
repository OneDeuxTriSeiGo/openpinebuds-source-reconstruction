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
#ifndef __TWS_ROLE_SWITCH__H__
#define __TWS_ROLE_SWITCH__H__

#include "bluetooth.h"

enum PSM_CONTEXT_TYPE
{
     PSM_CONTEXT_SDP     = 0x01,  //SDP
     PSM_CONTEXT_RFC     = 0x02,  //RFCOMM MUX,HFP and SPP share
     PSM_CONTEXT_AVDTP   = 0x04,  //A2DP
     PSM_CONTEXT_AVCTP   = 0x08,  //AVRCP
     PSM_CONTEXT_BTGATT  = 0x10,  //GATT OVER BREDR

     PSM_CONTEXT_INVALID = 0x80,
};

#define BT_RPOFILE_FINAL_FLAG   (0x5f)

enum PROFILE_CONTEXT_FLAG
{
    BT_HFP_FLAG = 0x01,
    BT_A2DP_FLAG = 0x02,
    BT_AVRCP_FLAG = 0x04,
    BT_MAP_FLAG =0x08,
    BT_GATT_FLAG = 0x10,
    //add new profile flag here
    
    BT_SPP_FLAG = 0x80, //SPP has multiple app id(total BTIF_APP_SPP_NUM),BT_SPP_FLAG flag should be at high bit
};

#define BT_ALL_CONTEXT_PSM    (PSM_CONTEXT_SDP | PSM_CONTEXT_RFC | PSM_CONTEXT_AVDTP | PSM_CONTEXT_AVCTP | PSM_CONTEXT_BTGATT)  

#define BT_ALL_CONTEXT_FLAG   (BT_HFP_FLAG | BT_A2DP_FLAG  | BT_AVRCP_FLAG | BT_MAP_FLAG | BT_SPP_FLAG | BT_GATT_FLAG)

#define BT_ALL_RFC_APP_ID     BTIF_APP_HFP_PROFILE_ID

#define BT_EARPHONE_BASIC_APP_ID       (BTIF_APP_HFP_PROFILE_ID | BTIF_APP_A2DP_PROFILE_ID | BTIF_APP_AVRCP_PROFILE_ID)

enum TWS_CHANNEL_TYPE
{
     HF_RF_CHANNEL = 0,
     UNKNOWN_CHANNEL,
};

#ifdef __cplusplus
extern "C" {
#endif                          /*  */


#ifdef __cplusplus
}
#endif                          /*  */
#endif                          /* __ME_H */

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
#ifndef PRIVATE_VENDOR_H
#define PRIVATE_VENDOR_H

#include "ohos_bt_def.h"
#include <stdint.h>
#include <stdbool.h>
typedef struct {
    char *stream;
    unsigned int length;
} PcmStreamInfo;

/*
 * @param: void
 * @return: void
 * function: 本地播放器开始播放
 */
void StartLocalMediaPlay(void);

/*
 * @param: void
 * @return: void
 * function: 本地播放器停止播放
 */
void StopLocalMediaPlay(void);

/*
 * @param: void
 * @return: void
 * function: 播放器在非播放的状态下通知应用层有码流过来的callback，目的是应用层决策本地播放器是否能播放
 */
typedef void (*MediaStreamNotifyCallback)(void);

/*
 * @param1: 注册函数地址
 * @return: int 0 - succee, not 0 - failure
 * function: 注册播放器有码流通知的callback函数
 */
int RegisterMediaStreamNotifyCallback(MediaStreamNotifyCallback func);

/*
 * @param1: 流地址
 * @param2: 流长度
 * @return: int 0 - succee, not 0 - failure
 * function: 应用层送HFP处理后的pcm流结果给协议栈
 */
int AppSendHfpPcmStream(PcmStreamInfo *streamInfo);

/*
* @param1: 需要加密的raw data
* @param2: 12字节的加密向量
* @param3: raw data + 16字节的最终加密数据
* @return: int 0 - succee, not 0 - failure
* function: 协议栈采用应用层的hks加密算法加密link key
*/
typedef int (*HksEnDecryptLinkKey)(unsigned char *input, unsigned char* random, unsigned char *output);

/*
* @param1: 应用层加密的函数地址
* @param2: 应用层解密的函数地址
* @return: int 0 - succee, not 0 - failure
* function: 应用层注册加解密函数地址到协议栈
*/
int RegisterEnDecryptCallback(HksEnDecryptLinkKey encryptCb, HksEnDecryptLinkKey decryptCb);
/*
 * 以下接口是立体声部分的接口定义 begin
 */
typedef enum {
    STEREO_MASTER,
    STEREO_SLAVE,
    STEREO_NONE
} StereoRole;

typedef struct {
    BdAddr addr;
    StereoRole role;
} StereoInfo;

typedef enum {
    WIFI_TO_A2DP = 0,
    WIFI_TO_BIS,
    A2DP_TO_BIS,
    UNKNOWN_STATUS
} StereoPlayMode;

typedef enum {
    LEFT_CHANNEL = 0,
    RIGHT_CHANNEL
} StereoChannelSet;

typedef enum {
    STEREO_SETUP_SUCCESS = 0x00,
    STEREO_SETUP_FAILED,
    STEREO_EXIT_SUCCESS,
    STEREO_EXIT_FAILED,
    STEREO_EXIT_REQUEST,
    STEREO_CONNECTION_TIMEOUT = 0x08,
    STEREO_UNKNOWN_REASON = 0xff,
} StereoEvent;

/*
 * @param: void
 * @return: void
 * function: 组建蓝牙立体声
 */
int SetUpStereo(StereoInfo *data);

/*
 * @param: void
 * @return: void
 * function: 拆除蓝牙立体声
 */
void ExitStereo(void);

/*
 * @param: status - used for future
 * @return: void
 * function: 立体声通信中断上报给应用层的接口
 */
typedef void (*StereoStateChangedCallback)(StereoEvent status);

/*
 * @param: StereoStateChangedCallback - 立体声通信中断的处理函数
 * @return: void
 * function: 注册立体声通信中断的函数到bt协议栈
 */
int RegisterStereoNotifyCallback(StereoStateChangedCallback func);

/*
 * @param1: buf - 发送数据的首地址
 * @param2: length - 发送数据的长度
 * @return: int 成功(0)/失败(非0)
 * function: 蓝牙立体声主从音箱发送数据接口
 */
int StereoSendData(unsigned char *buf, unsigned int length);

/*
 * @param1: buf - 接收数据的缓冲区
 * @param2: length - 接收数据的缓冲区大小
 * @return: int 成功(0)/失败(非0)
 * function: 蓝牙立体声主从音箱发送数据接口
 */
typedef int (*StereoReceiveData)(unsigned char *buf, unsigned int length);

/*
 * @param: StereoReceiveData - 发送数据的首地址
 * @return: int 成功(0)/失败(非0)
 * function: 注册蓝牙立体声主从音箱接收数据的处理函数到bt协议栈
 */
int RegisterStereoDateNotifyCallback(StereoReceiveData func);

/*
 * @param: status - 当前播放状态, see details {@link StereoPlayMode}
 * @return: void
 * function: App通知协议栈当前处于哪种播放状态，有以下几种情况:
 *          1、WIFI_TO_A2DP
 *          2、WIFI_TO_BIS
 *          3、A2DP_TO_BIS
 *          4、UNKNOWN_STATUS
 */
void AppNotifyStereoPlayStatus(StereoPlayMode status);

/*
 * @param: addr - 当前连接音箱的手机地址
 *         role - 应用层调用该接口进行主从切换时, 该台音箱当前的角色(master/slave)
 * @return: int 成功(0)/失败(非0)
 * function: 立体声组网过程中, 应用层调用此接口进行主从切换
 */
int StereoMasterSlaveSwitch(BdAddr *addr, StereoRole role);

/*
 * @param: addr - 设置主音箱的声道
 * @return: int 成功(0)/失败(非0)
 * function: 立体声组网成功后, 设置主音箱的左右声道
 */
int StereoChannelSwitch(StereoChannelSet channel);

/*
 * @param: void
 * @return: BdAddr - 获取蓝牙对外呈现的地址
 * function: 1、单箱模式, 获取的是factory的mac地址
 *           2、TWS模式, 获取的是master的mac地址
 */
BdAddr *GetExternalDisplayAddress(void);

/*
 * 以上接口是立体声部分的接口定义 end
 */

void PeriodGetDeviceRssi(void);

typedef enum {
    GAP_DEV_MODE_SINGLE = 0,
    GAP_DEV_MODE_STEREO,
    GAP_DEV_MODE_UNKNOW = 0xFF,
} GAP_DEVICE_MODE_T;

typedef struct
{
    bool                            ux_bluetooth_enable;
    GAP_DEVICE_MODE_T               ux_dev_mode;
    bool                            ux_doing_tws_paring_flag;
    bool                            ux_mobile_connected_before_tws_paring;
    StereoStateChangedCallback      ux_stereo_state_change_cb;
    // workaround solution: fix the case that UX device mode does not match the ibrt statck deive mode.
    bool                            ux_delay_setupstereo;
    uint8_t                         ux_dev_role;
    uint8_t                         ux_peer_addr[6];
} VENDOR_BT_GAP_IF_INFO_T;


#endif

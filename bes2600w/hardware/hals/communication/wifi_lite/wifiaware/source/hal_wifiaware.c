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

#include "wifiaware.h"

int HalWifiSdpInit(const char* ifname)
{
    return 0;
}

unsigned int HalCipherHashSha256(const char* input, unsigned int inputLen, unsigned char* hash, unsigned hashLen)
{
    return 0;
}

int HalWifiSdpStartService(const char* svcName, unsigned char localHandle, RecvCallback recvCB, unsigned char role)
{
    return 0;
}

int HalWifiSdpSend(unsigned char* macAddr, unsigned char peerHandle, unsigned char localHandle,
    unsigned char* msg, int len)
{
    return 0;
}

int HalWifiSdpStopService(unsigned char localHandle, unsigned char role)
{
    return 0;
}

int HalWifiSdpDeinit(void)
{
    return 0;
}

int HalWifiSdpAdjustTxPower(const char *ifname, signed char power)
{
    return 0;
}

int HalWifiSdpRestoreTxPower(const char *ifname)
{
    return 0;
}

int HalWifiSdpBeaconSwitch(const char *ifname, unsigned char enable)
{
    return 0;
}

int HalWifiSdpSetRetryTimes(unsigned int retries)
{
    return 0;
}

int HalWifiSdpGetSyncMode(void)
{
    return 0;
}
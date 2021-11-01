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

#include "flash.h"
#include "hdf_log.h"
#include "hdf_device_desc.h"
#include "device_resource_if.h"
#include "hal_hota_board.h"
#include "hal_update_board.h"
#define PUBKEY_LENGTH 270


static const unsigned char g_pubKeyBuf[PUBKEY_LENGTH] = {
    0x30, 0x82, 0x01, 0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0xBF, 0xAA, 0xA5, 0xB3, 0xC2, 0x78, 0x5E,
    0x63, 0x07, 0x84, 0xCF, 0x37, 0xF0, 0x45, 0xE8, 0xB9, 0x6E, 0xEF, 0x04, 0x88, 0xD3, 0x43, 0x06,
    0x2C, 0xFC, 0x09, 0x8C, 0x84, 0x3B, 0x43, 0x07, 0x89, 0x6B, 0x23, 0x8F, 0xA2, 0xF0, 0x9D, 0x91,
    0xCB, 0xA8, 0x02, 0x60, 0xD8, 0x5C, 0x18, 0xD9, 0x34, 0xF0, 0x3C, 0x49, 0x10, 0x0E, 0xE3, 0xC7,
    0x19, 0xA5, 0x51, 0x93, 0x38, 0xFD, 0xE0, 0x62, 0x64, 0xBA, 0x6D, 0x11, 0x2E, 0xE1, 0x6E, 0x60,
    0x12, 0x16, 0x1B, 0x35, 0xA6, 0x54, 0x3F, 0x0B, 0x5D, 0x54, 0x08, 0xC9, 0x23, 0x51, 0x15, 0xA9,
    0xE2, 0x07, 0xCC, 0xF9, 0xFD, 0x19, 0x8A, 0xB3, 0x7E, 0xCE, 0x69, 0xED, 0x26, 0x34, 0xD2, 0x02,
    0xF3, 0xEB, 0x07, 0x13, 0x69, 0xE3, 0x03, 0x87, 0xB3, 0x6A, 0x3E, 0x91, 0x94, 0xAC, 0x2C, 0xBA,
    0xF6, 0xEE, 0x4C, 0x41, 0x0C, 0x2E, 0xD0, 0xE7, 0x58, 0xA7, 0xE6, 0x7F, 0x1A, 0xC0, 0xB8, 0xE3,
    0x12, 0x18, 0x97, 0x8D, 0x99, 0xAB, 0x35, 0x7B, 0xAD, 0x41, 0xA0, 0xFB, 0xCB, 0x23, 0xF6, 0x51,
    0xE6, 0x94, 0x1F, 0xF7, 0xD8, 0x16, 0xCD, 0x15, 0x67, 0x59, 0x10, 0xA2, 0x40, 0x55, 0xD1, 0x67,
    0xDA, 0x18, 0xCD, 0x63, 0x5B, 0x10, 0xAF, 0x22, 0x99, 0xD8, 0x9D, 0x56, 0x66, 0xCD, 0x80, 0x64,
    0x32, 0xB5, 0xD7, 0xF5, 0xBA, 0x91, 0x4A, 0x8D, 0x97, 0x14, 0x8A, 0xB2, 0xB0, 0x42, 0x4A, 0xE6,
    0x43, 0x22, 0x3B, 0x6E, 0xD6, 0x1E, 0x1F, 0xDA, 0xEC, 0x83, 0xCF, 0x20, 0xFA, 0x02, 0xF3, 0xFB,
    0x6E, 0x09, 0x2A, 0x0D, 0xB7, 0x81, 0x1E, 0xD5, 0x71, 0xDF, 0x80, 0xC5, 0x33, 0x78, 0xE5, 0x41,
    0x33, 0xDF, 0x9A, 0xBD, 0x36, 0x51, 0xAC, 0x96, 0xF4, 0xC6, 0x11, 0xC3, 0x93, 0x78, 0x26, 0x96,
    0x9F, 0x67, 0x05, 0x1D, 0xDF, 0xB3, 0xAA, 0x26, 0x25, 0x02, 0x03, 0x01, 0x00, 0x01
};

static int g_componentTableInit = 0;
static ComponentTableInfo g_componentTable[HAL_PARTITION_MAX] = {0};

int HotaHalInit(void)
{
    if (g_componentTableInit > 0) {
        return HDF_SUCCESS;
    } else {
        return HDF_FAILURE;
    }
}

int HotaHalGetUpdateIndex(unsigned int *index)
{
    return HDF_SUCCESS;
}

int HotaHalDeInit(void)
{
    return HDF_SUCCESS;
}

int HotaHalRead(int partition, unsigned int offset, unsigned int bufLen, unsigned char *buffer)
{
    if ((buffer == NULL) || (bufLen == 0)) {
        return HDF_FAILURE;
    }

    if (ota_flash_read(partition, offset, buffer, bufLen) != HDF_SUCCESS) {
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

int HotaHalWrite(int partition, unsigned char *buffer, unsigned int offset, unsigned int buffLen)
{
    if ((buffer == NULL) || (buffLen == 0)) {
        return HDF_FAILURE;
    }

    if (ota_flash_write(partition, offset, buffer, buffLen) != HDF_SUCCESS) {
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

int HotaHalRestart(void)
{
    return HDF_SUCCESS;
}

int HotaHalSetBootSettings(void)
{
    return HDF_SUCCESS;
}

int HotaHalRollback(void)
{
    return HDF_SUCCESS;
}

const ComponentTableInfo *HotaHalGetPartitionInfo()
{
    return g_componentTable;
}

unsigned char *HotaHalGetPubKey(unsigned int *length)
{
    if (length == NULL) {
        return NULL;
    }

    *length = sizeof(g_pubKeyBuf);
    return (unsigned char *)g_pubKeyBuf;
}


int HotaHalGetUpdateAbility(void)
{
    return ABILITY_PKG_SEARCH | ABILITY_PKG_DLOAD;
}

int HotaHalGetOtaPkgPath(char *path, int len)
{
    return HDF_SUCCESS;
}

int HotaHalIsDeviceCanReboot(void)
{
    return 1;
}

int HotaHalGetMetaData(UpdateMetaData *metaData)
{
    return HDF_SUCCESS;
}

int HotaHalSetMetaData(UpdateMetaData *metaData)
{
    return HDF_SUCCESS;
}

int HotaHalRebootAndCleanUserData(void)
{
    return HDF_SUCCESS;
}

int HotaHalRebootAndCleanCache(void)
{
    return HDF_SUCCESS;
}

int HotaHalIsDevelopMode(void)
{
    return HDF_SUCCESS;
}

int HotaHalCheckVersionValid(const char *currentVersion, const char *pkgVersion, unsigned int pkgVersionLength)
{
    return (strncmp(currentVersion, pkgVersion, pkgVersionLength) == 0) ? 1 : 0;
}

uint32_t HotaGetResource(const struct DeviceResourceNode *resourceNode)
{
    struct DeviceResourceIface *resource = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (resource == NULL) {
        HDF_LOGE("Invalid DeviceResourceIface");
        return HDF_FAILURE;
    }
    int32_t num = resource->GetElemNum(resourceNode, "partitions");
    if (num < 0 || num > HAL_PARTITION_MAX) {
        HDF_LOGE("%s: invalid partitions num %d", __func__, num);
        return HDF_FAILURE;
    }
    for (int32_t i = 0; i < num; i++) {
        uint32_t partition = 0;
        if (resource->GetUint32ArrayElem(resourceNode, "partitions", i, &partition, 0) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get partitions", __func__);
            return HDF_FAILURE;
        }
        if (partition >= HAL_PARTITION_MAX) {
            HDF_LOGE("%s: invalid partition %u", __func__, partition);
            return HDF_FAILURE;
        }

        g_componentTable[partition].id = partition;

        if (resource->GetStringArrayElem(resourceNode, "description", i, &g_componentTable[partition].componentName, NULL) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get mount_points", __func__);
            return HDF_FAILURE;
        }

        g_componentTable[partition].imgPath = "/"; 
    }

    g_componentTableInit = 1;
    return HDF_SUCCESS;
}
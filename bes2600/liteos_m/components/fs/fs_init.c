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
#include <sys/mount.h>
#include "flash.h"
#include "los_fs.h"
#include "los_config.h"
#include "hdf_log.h"
#include "hdf_device_desc.h"
#ifdef LOSCFG_DRIVERS_HDF_CONFIG_MACRO
#include "hcs_macro.h"
#include "hdf_config_macro.h"
#else
#include "device_resource_if.h"
#endif
struct fs_cfg {
    char *mount_point;
    struct PartitionCfg partCfg;
};

static struct fs_cfg fs[LOSCFG_LFS_MAX_MOUNT_SIZE] = {0};
#ifdef LOSCFG_DRIVERS_HDF_CONFIG_MACRO
#define DISPLAY_MISC_FS_LITTLEFS_CONFIG HCS_NODE(HCS_NODE(HCS_NODE(HCS_ROOT, misc), fs_config), littlefs_config)
static uint32_t FsGetResource(struct fs_cfg *fs)
{
    int32_t num = HCS_ARRAYS_SIZE(HCS_NODE(DISPLAY_MISC_FS_LITTLEFS_CONFIG, mount_points));
    if (num < 0 || num > LOSCFG_LFS_MAX_MOUNT_SIZE) {
        HDF_LOGE("%s: invalid mount_points num %d", __func__, num);
        return HDF_FAILURE;
    }
    char * mount_points[] = HCS_ARRAYS(HCS_NODE(DISPLAY_MISC_FS_LITTLEFS_CONFIG, mount_points));
    uint32_t partitions[] = HCS_ARRAYS(HCS_NODE(DISPLAY_MISC_FS_LITTLEFS_CONFIG, partitions));
    uint32_t block_size[] = HCS_ARRAYS(HCS_NODE(DISPLAY_MISC_FS_LITTLEFS_CONFIG, block_size));
    uint32_t block_count[] = HCS_ARRAYS(HCS_NODE(DISPLAY_MISC_FS_LITTLEFS_CONFIG, block_count));
    for (int32_t i = 0; i < num; i++) {
        fs[i].mount_point = mount_points[i];
        fs[i].partCfg.partNo = partitions[i];
        fs[i].partCfg.blockSize = block_size[i];
        fs[i].partCfg.blockCount = block_count[i];

        HDF_LOGD("%s: fs[%d] mount_point=%s, partition=%u, block_size=%u, block_count=%u", __func__, i,
                 fs[i].mount_point, (uint32_t)fs[i].partCfg.partNo, fs[i].partCfg.blockSize, fs[i].partCfg.blockCount);
    }
    return HDF_SUCCESS;
}
#else
static uint32_t FsGetResource(struct fs_cfg *fs, const struct DeviceResourceNode *resourceNode)
{
    struct DeviceResourceIface *resource = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (resource == NULL) {
        HDF_LOGE("Invalid DeviceResourceIface");
        return HDF_FAILURE;
    }
    int32_t num = resource->GetElemNum(resourceNode, "mount_points");
    if (num < 0 || num > LOSCFG_LFS_MAX_MOUNT_SIZE) {
        HDF_LOGE("%s: invalid mount_points num %d", __func__, num);
        return HDF_FAILURE;
    }
    for (int32_t i = 0; i < num; i++) {
        if (resource->GetStringArrayElem(resourceNode, "mount_points", i, &fs[i].mount_point, NULL) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get mount_points", __func__);
            return HDF_FAILURE;
        }
        if (resource->GetUint32ArrayElem(resourceNode, "partitions", i, (uint32_t *)&fs[i].partCfg.partNo, 0) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get partitions", __func__);
            return HDF_FAILURE;
        }
        if (resource->GetUint32ArrayElem(resourceNode, "blockSize", i, &fs[i].partCfg.blockSize, 0) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get blockSize", __func__);
            return HDF_FAILURE;
        }
        if (resource->GetUint32ArrayElem(resourceNode, "blockCount", i, &fs[i].partCfg.blockCount, 0) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get blockCount", __func__);
            return HDF_FAILURE;
        }
        HDF_LOGD("%s: fs[%d] mount_point=%s, partition=%u, blockSize=%u, blockCount=%u", __func__, i,
                 fs[i].mount_point, (uint32_t)fs[i].partCfg.partNo, fs[i].partCfg.blockSize, fs[i].partCfg.blockCount);
    }
    return HDF_SUCCESS;
}
#endif
static int32_t FsDriverInit(struct HdfDeviceObject *object)
{
    if (object == NULL) {
        return HDF_FAILURE;
    }
#ifdef LOSCFG_DRIVERS_HDF_CONFIG_MACRO
    if (FsGetResource(fs) != HDF_SUCCESS) {
        HDF_LOGE("%s: FsGetResource failed", __func__);
        return HDF_FAILURE;
    }
#else
    if (object->property) {
        if (FsGetResource(fs, object->property) != HDF_SUCCESS) {
            HDF_LOGE("%s: FsGetResource failed", __func__);
            return HDF_FAILURE;
        }
    }
#endif

    for (int i = 0; i < sizeof(fs) / sizeof(fs[0]); i++) {
        if (fs[i].mount_point == NULL)
            continue;

        fs[i].partCfg.readFunc = hal_flash_read;
        fs[i].partCfg.writeFunc = hal_flash_write;
        fs[i].partCfg.eraseFunc = hal_flash_erase;
        fs[i].partCfg.readSize = 256; /* 256, lfs read size */
        fs[i].partCfg.writeSize = 256; /* 256, lfs prog size */
        fs[i].partCfg.cacheSize = 256; /* 256, lfs cache size */
        fs[i].partCfg.lookaheadSize = 16; /* 16, lfs lookahead size */
        fs[i].partCfg.blockCycles = 1000; /* 1000, lfs block cycles */

        int ret = mount(NULL, fs[i].mount_point, "littlefs", 0, &fs[i].partCfg);
        HDF_LOGI("%s: mount fs on '%s' %s\n", __func__, fs[i].mount_point, (ret == 0) ? "succeed" : "failed");
    }
    return HDF_SUCCESS;
}

static int32_t FsDriverBind(struct HdfDeviceObject *device)
{
    (void)device;
    return HDF_SUCCESS;
}

static void FsDriverRelease(struct HdfDeviceObject *device)
{
    (void)device;
}

static struct HdfDriverEntry g_fsDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_FS_LITTLEFS",
    .Bind = FsDriverBind,
    .Init = FsDriverInit,
    .Release = FsDriverRelease,
};

HDF_INIT(g_fsDriverEntry);

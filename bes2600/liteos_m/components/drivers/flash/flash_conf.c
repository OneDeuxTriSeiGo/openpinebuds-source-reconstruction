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
//#include "hal_hota_board.h"
//#include "hal_update_board.h"

/* Logic partition on flash devices */
hal_logic_partition_t g_halPartitions[] = {
    [HAL_PARTITION_BOOTLOADER] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "boot1",
        .partition_start_addr = 0x0,
        .partition_length = 0x10000, //64KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_BOOT2A] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "boot2A",
        .partition_start_addr = 0x10000,
        .partition_length = 0x10000, //64KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_BOOT2B] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "boot2B",
        .partition_start_addr = 0x20000,
        .partition_length = 0x10000, //64KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_TRUSTZONEA] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "trustzoneA",
        .partition_start_addr = 0x30000,
        .partition_length = 0x26000, //152KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_TRUSTZONEB] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "trustzoneB",
        .partition_start_addr = 0x56000,
        .partition_length = 0x26000, //152KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_TZ_INFO] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "trustzoneInfo",
        .partition_start_addr = 0x7C000,
        .partition_length = 0x4000, //16KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_CM33_MAIN] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "cm33_main",
        .partition_start_addr = 0x80000,
        .partition_length = 0x7E0000, //8064KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_SYSTEM_MINI] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "system_mini",
        .partition_start_addr = 0x860000,
        .partition_length = 0x300000, //3072KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_RESOURCE] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "littlefs",
        .partition_start_addr = 0xB60000,
        .partition_length = 0x400000, //4096KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_LOG] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "log",
        .partition_start_addr = 0xF60000,
        .partition_length = 0x30000, //192KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_DATA] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "data",
        .partition_start_addr = 0xF90000,
        .partition_length = 0x40000, //256KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_MISC] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "misc",
        .partition_start_addr = 0xFD0000,
        .partition_length = 0xC000, //48KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_USERDATA] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "userdata",
        .partition_start_addr = 0xFDC000,
        .partition_length = 0x4000, //16KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_ENV] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "factory",
        .partition_start_addr = 0xFE0000,
        .partition_length = 0x10000, //64KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [HAL_PARTITION_ENV_REDUND] = {
        .partition_owner = HAL_FLASH_EMBEDDED,
        .partition_description = "factory_backup",
        .partition_start_addr = 0xFF0000,
        .partition_length = 0x10000, //64KB
        .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
};

static uint32_t FlashGetResource(const struct DeviceResourceNode *resourceNode)
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
        if (resource->GetUint32ArrayElem(resourceNode, "owner", i, (uint32_t *)&g_halPartitions[partition].partition_owner, 0) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get owner", __func__);
            return HDF_FAILURE;
        }
        if (resource->GetStringArrayElem(resourceNode, "description", i, &g_halPartitions[partition].partition_description, NULL) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get mount_points", __func__);
            return HDF_FAILURE;
        }
        if (resource->GetUint32ArrayElem(resourceNode, "start_addr", i, &g_halPartitions[partition].partition_start_addr, 0) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get block_size", __func__);
            return HDF_FAILURE;
        }
        if (resource->GetUint32ArrayElem(resourceNode, "length", i, &g_halPartitions[partition].partition_length, 0) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get block_count", __func__);
            return HDF_FAILURE;
        }
        if (resource->GetUint32ArrayElem(resourceNode, "options", i, &g_halPartitions[partition].partition_options, 0) != HDF_SUCCESS) {
            HDF_LOGE("%s: failed to get block_count", __func__);
            return HDF_FAILURE;
        }
        HDF_LOGD("%s: partition[%u] owner=%u, description=%s, start_addr=0x%x, length=0x%x, options=%u", __func__, partition,
                 g_halPartitions[partition].partition_owner, g_halPartitions[partition].partition_description,
                 g_halPartitions[partition].partition_start_addr, g_halPartitions[partition].partition_length,
                 g_halPartitions[partition].partition_options);
    }
    return HDF_SUCCESS;
}

static int32_t FlashDriverInit(struct HdfDeviceObject *object)
{
    if (object == NULL) {
        return HDF_FAILURE;
    }
    if (object->property) {
        if (FlashGetResource(object->property) != HDF_SUCCESS) {
            HDF_LOGE("%s: FlashGetResource failed", __func__);
            return HDF_FAILURE;
        }
    }
    return HDF_SUCCESS;
}

static int32_t FlashDriverBind(struct HdfDeviceObject *device)
{
    (void)device;
    return HDF_SUCCESS;
}

static void FlashDriverRelease(struct HdfDeviceObject *device)
{
    (void)device;
}

static struct HdfDriverEntry g_FlashDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_STORAGE_FLASH",
    .Bind = FlashDriverBind,
    .Init = FlashDriverInit,
    .Release = FlashDriverRelease,
};

HDF_INIT(g_FlashDriverEntry);
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

#include "iot_errno.h"
#include "iot_flash.h"
#include "flash.h"

unsigned int IoTFlashRead(unsigned int flashOffset, unsigned int size, unsigned char *ramData)
{
    unsigned int len = 0;
    int ret = hal_flash_read(HAL_PARTITION_RESOURCE,&flashOffset,ramData,size);
    if (ret == 0) {
        len = flashOffset;
    }
    return len;
}

unsigned int IoTFlashWrite(unsigned int flashOffset, unsigned int size,
                           const unsigned char *ramData, unsigned char doErase)
{
    int ret = -1;
    if (doErase) {
        ret = hal_flash_write(HAL_PARTITION_RESOURCE,&flashOffset, ramData, size);
    } else {
        ret = hal_flash_erase_write(HAL_PARTITION_RESOURCE,&flashOffset, ramData, size);
    }
    return ret;
}

unsigned int IoTFlashErase(unsigned int flashOffset, unsigned int size)
{
    return hal_flash_erase(HAL_PARTITION_RESOURCE,flashOffset, size);
}

unsigned int IoTFlashInit(void)
{
    return 1;
}

unsigned int IoTFlashDeinit(void)
{
    return 1;
}

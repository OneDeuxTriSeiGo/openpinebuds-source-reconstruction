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
#ifndef __OTA_BASIC_H__
#define __OTA_BASIC_H__

// ota data path is exclusive, anytime there is only one ota datapath available
typedef enum
{
    OTA_BASIC_NO_DATAPATH_ENALBED  = 0,
    OTA_BASIC_SPP_DATAPATH_ENABLED = 1,
    OTA_BASIC_BLE_DATAPATH_ENABLED = 2,
} OTA_BASIC_ENABLED_DATAPATH_E;

typedef enum
{
    OTA_BASIC_TWS_INVALID_ROLE   = 0,
    OTA_BASIC_TWS_FREEMAN   = 1,
    OTA_BASIC_TWS_MASTER    = 2,
    OTA_BASIC_TWS_SLAVE     = 3,
    OTA_BASIC_TWS_UNKNOWN   = 4,
} OTA_BASIC_TWS_ROLE_E;

#ifndef NEW_IMAGE_FLASH_OFFSET
#define NEW_IMAGE_FLASH_OFFSET    0x180000
#endif

#ifdef __cplusplus
extern "C" {
#endif

void ota_basic_env_init(void);
bool ota_basic_enable_datapath_status(OTA_BASIC_ENABLED_DATAPATH_E datapath, uint8_t* macAddr);
bool ota_basic_disable_datapath_status(OTA_BASIC_ENABLED_DATAPATH_E datapath, uint8_t* macAddr);
uint8_t* ota_basic_get_mac_addr(void);
OTA_BASIC_TWS_ROLE_E ota_basic_get_tws_role(void);
OTA_BASIC_ENABLED_DATAPATH_E ota_basic_get_datapath(void);
bool ota_basic_is_remap_enabled(void);
uint32_t ota_basic_get_remap_offset(void);
uint32_t ota_basic_get_max_image_size(void);

#ifdef __cplusplus
}
#endif

#endif


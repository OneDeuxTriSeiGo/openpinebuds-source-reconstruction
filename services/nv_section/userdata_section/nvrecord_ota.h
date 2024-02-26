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


#ifndef __NVRECORD_OTA_H__
#define __NVRECORD_OTA_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************header include********************************/
#include "nvrecord_extension.h"

/******************************macro defination*****************************/

/******************************type defination******************************/

/****************************function declearation**************************/
/**
 * @brief Initialize the nvrecord pointer.
 *
 */
void nv_record_ota_init(void);

/**
 * @brief Get the pointer of OTA info saved in flash.
 *
 * NOTE: OTA support mutiple devices(currently APP and hotword model file), so
 * you need to use the device index and the pointer from this function you get
 * to get the device specific OTA info.
 *
 * @param ptr           pointer used to get the flash pointer
 */
void nv_record_ota_get_ptr(void **ptr);

/**
 * @brief Update the break point for OTA progress.
 *
 * @param user          current user
 * @param deviceIndex   current deivce index
 * @param otaOffset     break point to update
 */
void nv_record_ota_update_breakpoint(uint8_t user,
                                     uint8_t deviceIndex,
                                     uint32_t otaOffset);

/**
 * @brief Update the OTA info.
 *
 * @param user          current OTA user
 * @param deviceIndex   current device in OTA progress
 * @param status        OTA status(stage) to update,
 *                      @see OTA_STAGE_E to get more details
 * @param imageSize     image size
 * @param session       OTA version string pointer
 * @return true         OTA info update successfully
 * @return false        OTA info update failed
 */
bool nv_record_ota_update_info(uint8_t user,
                               uint8_t deviceIndex,
                               uint8_t status,
                               uint32_t imageSize,
                               const char *session);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __NVRECORD_OTA_H__ */

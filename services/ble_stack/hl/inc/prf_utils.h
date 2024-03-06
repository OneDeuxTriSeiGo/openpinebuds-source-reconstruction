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
#ifndef _PRF_UTILS_H_
#define _PRF_UTILS_H_

/**
 ****************************************************************************************
 * @addtogroup PRF_UTILS
 * @ingroup Profile
 *
 * @brief Definitions of shared profiles functions that can be used by several profiles
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"
#include "prf_types.h"
#include "gatt.h"
#include "gapc.h"
#include "gapc_msg.h"
#include "prf.h"
#include "ke_task.h"

/*
 * MACROS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Macro used to called the prf_client_get_env function.
 *
 * @param prf_id    Profile Task ID   (In Upper case, ex: HTPC, DISC, ...)
 * @param type      Profile task type (In lower case, ex: htpc, disc, ...)
 ****************************************************************************************
 */
#define PRF_ENV_T(type)           type##_env_t
#define PRF_ENV_TAG(type)         struct type##_env_tag
#define PRF_ENV_GET(prf_id, type) ((type##_env_t *)prf_env_get((PRF_ID_##prf_id)))





/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

#if (BLE_BATT_SERVER)
/**
 ****************************************************************************************
 * @brief Pack Characteristic Presentation Format descriptor value
 ****************************************************************************************
 */
void prf_pack_char_pres_fmt(co_buf_t* p_buf, const prf_char_pres_fmt_t* char_pres_fmt);
#endif // (BLE_BATT_SERVER)

#if (BLE_BATT_CLIENT)
/**
 ****************************************************************************************
 * @brief Unpack Characteristic Presentation Format descriptor value
 ****************************************************************************************
 */
void prf_unpack_char_pres_fmt(co_buf_t* p_buf, prf_char_pres_fmt_t* char_pres_fmt);
#endif // (BLE_BATT_CLIENT)

#if (BLE_CLIENT_PRF)
/**
 ****************************************************************************************
 *
 *
 *
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 *
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 *
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Write peer characteristic using GATT.
 *
 * It will request write modification of peer handle
 *
 * @param[in] conidx        Connection index
 * @param[in] user_lid      GATT Client User Local Identifier
 * @param[in] dummy         Dummy parameter returned in procedure completion
 * @param[in] write_type    GATT Write Type (@see gatt_write_type)
 * @param[in] hdl           Peer handle to modify
 * @param[in] length        Value length
 * @param[in] p_data        Pointer to data value
 *
 * @return Execution status (@see enum hl_err)
 ****************************************************************************************
 */
uint16_t prf_gatt_write(uint8_t conidx, uint8_t user_lid, uint16_t dummy, uint8_t write_type,
                        uint16_t hdl, uint16_t length, const uint8_t* p_data);

/**
 ****************************************************************************************
 *
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Check service characteristic validity
 *
 * For each characteristic in service it verifies handles.
 *
 * If some handles are not present, it checks if they shall be present or they are optional.
 *
 * @param nb_chars      Number of Characteristics in the service
 * @param p_chars       Characteristics values (char handles, val handles, properties)
 * @param p_chars_req   Characteristics requirements.
 *
 * @return Execution status (@see enum hl_err)
 ****************************************************************************************
 */
uint16_t prf_check_svc_char_validity(uint8_t nb_chars, const prf_char_t* p_chars,
                                     const prf_char_def_t* p_chars_req);


/**
 ****************************************************************************************
 * @brief Check service characteristic descriptors validity
 *
 * For each characteristic descriptors in service it verifies handles.
 *
 * If some handles are not present, according to characteristic properties it verify if
 * descriptor is optional or not.
 *
 * @param nb_descs       Number of Characteristic descriptors in the service
 * @param p_descs        Characteristic descriptors values (handles)
 * @param p_descs_req    Characteristics descriptors requirements.
 *
 * @return Execution status (@see enum hl_err)
 ****************************************************************************************
 */
uint16_t prf_check_svc_desc_validity(uint8_t nb_descs, const prf_desc_t* p_descs,
                                     const prf_desc_def_t* p_descs_req, const prf_char_t* p_chars);

/**
 ****************************************************************************************
 * @brief Extract information of the service according to the service description
 *
 * @param[in]     first_hdl    First handle value of following list
 * @param[in]     nb_att       Number of attributes
 * @param[in]     p_atts       Pointer to attribute information present in a service
 * @param[in]     nb_chars     Length of provided arrays (chars and chars_req)
 * @param[in]     p_chars_req    Characteristics requirements
 * @param[in|out] p_chars            Characteristics
 * @param[in]     nb_descs         Length of provided arrays (descs and descs_req)
 * @param[in]     p_descs_req        Descriptors requirements
 * @param[in|out] p_descs            Descriptors
 ****************************************************************************************
 */
void prf_extract_svc_info(uint16_t first_hdl, uint8_t nb_att, const gatt_svc_att_t* p_atts,
        uint8_t nb_chars, const prf_char_def_t* p_chars_req, prf_char_t* p_chars,
        uint8_t nb_descs, const prf_desc_def_t* p_descs_req, prf_desc_t* p_descs);

#endif //(BLE_CLIENT_PRF)



/**
 ****************************************************************************************
 *
 ****************************************************************************************
 */


#if (BLE_SERVER_PRF || BLE_CLIENT_PRF)
/**
 ****************************************************************************************
 * @brief Pack date time value
 *
 * @param[in] p_buf       Pointer to the output buffer
 * @param[in] p_date_time Pointer to structure date time
 ****************************************************************************************
 */
void prf_pack_date_time(co_buf_t*p_buf, const prf_date_time_t* p_date_time);

/**
 ****************************************************************************************
 * @brief Unpack date time value
 *
 * @param[in]  p_buf       Pointer to input buffer
 * @param[out] p_date_time Pointer to structure date time
 ****************************************************************************************
 */
void prf_unpack_date_time(co_buf_t* p_buf, prf_date_time_t* p_date_time);

#endif /* (BLE_SERVER_PRF || BLE_CLIENT_PRF) */

/// @} prf_utils

#endif /* _PRF_UTILS_H_ */

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



#ifndef _PRF_UTILS_128_H_
#define _PRF_UTILS_128_H_

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

#include "ke_task.h"
#include "prf.h"
#include "prf_types.h"
#include "gatt.h"
#include "gapc.h"
#include "gapc_msg.h"

/*
 * MACROS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


#if (BLE_CLIENT_PRF)


/**
 ****************************************************************************************
 *
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Check 128-bit service characteristic validity
 *
 * For each characteristic in service it verifies handles.
 *
 * If some handles are not present, it checks if they shall be present or they are optional.
 *
 * @param nb_chars     Number of Characteristics in the service
 * @param p_chars      Characteristics values (char handles, val handles, properties)
 * @param p_chars_req  Characteristics requirements.
 *
 * @return Execution status (@see enum hl_err)
 ****************************************************************************************
 */

uint16_t prf_check_svc128_char_validity(uint8_t nb_chars,
                                    const prf_char_t* p_chars,
                                    const prf_char128_def_t* p_chars_req);

void prf_extract_svc128_info(uint16_t first_hdl, uint8_t nb_att, const gatt_svc_att_t* p_atts,
        uint8_t nb_chars, const prf_char128_def_t* p_chars_req, prf_char_t* p_chars,
        uint8_t nb_descs, const prf_desc_def_t* p_descs_req, prf_desc_t* p_descs);

#endif //(BLE_CLIENT_PRF)


/// @} prf_utils

#endif /* _PRF_UTILS_128_H_ */

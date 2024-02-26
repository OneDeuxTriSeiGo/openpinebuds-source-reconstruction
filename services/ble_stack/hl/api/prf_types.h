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
#ifndef _PRF_TYPES_H_
#define _PRF_TYPES_H_

/**
 ****************************************************************************************
 * @addtogroup PRF_TYPES
 * @ingroup Profile
 * @brief Definitions of shared profiles types
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "gatt.h"
#include "compiler.h"
#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

/// Characteristic Presentation Format Descriptor Size
#define PRF_CHAR_PRES_FMT_SIZE          (7)


/// Possible values for setting client configuration characteristics
enum prf_cli_conf
{
    /// Stop notification/indication
    PRF_CLI_STOP_NTFIND = 0x0000,
    /// Start notification
    PRF_CLI_START_NTF   = 0x0001,
    /// Start indication
    PRF_CLI_START_IND   = 0x0002,
};

/// Possible values for setting server configuration characteristics
enum prf_srv_conf
{
    /// Stop Broadcast
    PRF_SRV_STOP_BCST  = 0x0000,
    /// Start Broadcast
    PRF_SRV_START_BCST = 0x0001,
};

/// Connection type
enum prf_con_type
{
    /// Discovery type connection
    PRF_CON_DISCOVERY = 0x00,
    /// Normal type connection
    PRF_CON_NORMAL    = 0x01,
};

enum prf_svc_type
{
    PRF_PRIMARY_SERVICE   = 0x00,
    PRF_SECONDARY_SERVICE = 0x01
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/**
 * Characteristic Presentation Format Descriptor structure
 * Packed size is PRF_CHAR_PRES_FMT_SIZE
 */
typedef struct prf_char_pres_fmt
{
    /// Unit (The Unit is a UUID)
    uint16_t unit;
    /// Description
    uint16_t description;
    /// Format
    uint8_t  format;
    /// Exponent
    uint8_t  exponent;
    /// Name space
    uint8_t  name_space;
} prf_char_pres_fmt_t;

/**
 * date and time structure
 * size = 7 bytes
 */
/// Time profile information
typedef struct prf_date_time
{
    /// year time element
    uint16_t year;
    /// month time element
    uint8_t  month;
    /// day time element
    uint8_t  day;
    /// hour time element
    uint8_t  hour;
    /// minute time element
    uint8_t  min;
    /// second time element
    uint8_t  sec;
} prf_date_time_t;

/**
 *  SFLOAT: Short Floating Point Type
 *
 *        +----------+----------+---------+
 *        | Exponent | Mantissa |  Total  |
 * +------+----------+----------+---------+
 * | size |  4 bits  | 12 bits  | 16 bits |
 * +------+----------+----------+---------+
 */
typedef uint16_t prf_sfloat;

/// utf8_s string
typedef struct prf_utf_8
{
    /// value length
    uint16_t length;
    /// Value string in UTF8 format
    uint8_t  str[__ARRAY_EMPTY];
} prf_utf_8_t;

/// Service information structure
typedef struct prf_svc
{
    /// start handle
    uint16_t shdl;
    /// end handle
    uint16_t ehdl;
} prf_svc_t;

/// Included Service information structure
typedef struct prf_incl_svc
{
    /// attribute handle
    uint16_t handle;
    /// included service start handle
    uint16_t start_hdl;
    /// included service  end handle
    uint16_t end_hdl;
    /// UUID length
    uint8_t uuid_len;
    /// UUID
    uint8_t uuid[GATT_UUID_128_LEN];
} prf_incl_svc_t;

/// Characteristic information structure
typedef struct prf_char
{
    /// Value handle
    uint16_t val_hdl;
    /// Characteristic properties
    uint8_t prop;
} prf_char_t;

/// Descriptor information structure
typedef struct prf_desc
{
    /// Descriptor handle
    uint16_t desc_hdl;
} prf_desc_t;

/// Characteristic definition
typedef struct prf_char_def
{
    /// Characteristic UUID
    uint16_t uuid;
    /// Requirement bit field
    uint8_t req_bf;
    /// Mandatory Properties
    uint8_t prop_mand;
} prf_char_def_t;

/// Characteristic Descriptor definition
typedef struct prf_desc_def
{
    /// Characteristic Descriptor uuid
    uint16_t uuid;
    /// Requirement bit field
    uint8_t req_bf;
    /// Corresponding characteristic code
    uint8_t char_code;
} prf_desc_def_t;






/// @} PRF_TYPES

#endif /* _PRF_TYPES_H_ */

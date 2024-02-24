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
#ifndef __BT_STATUS_CONV_H__
#define __BT_STATUS_CONV_H__
#include "bluetooth.h"
#include "btlib_type.h"
#include "hal_trace.h"

extern const bt_status_t status_table[];

#if defined(__cplusplus)
extern "C" {
#endif
char *btifstatus2str(bt_status_t status);
char *btstatus2str(BtStatus status);
#if defined(__cplusplus)
}
#endif

//static inline bt_status_t bt_convert_status(BtStatus stack_status)
//{
//    // TRACE("%s:input=%d%s,output=%d%s", __func__, stack_status, btstatus2str(stack_status), status_table[stack_status], btifstatus2str(status_table[stack_status]));
//
//    if (stack_status <= BT_STATUS_LAST_CODE)
//        return status_table[stack_status];
//    else
//        return BT_STS_LAST_CODE;
//}
//
//#define BT_STATUS(stack_status) bt_convert_status(stack_status)

#endif/*__BT_STATUS_CONV_H__*/

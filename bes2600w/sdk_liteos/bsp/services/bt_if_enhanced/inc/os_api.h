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
#ifndef __OS_API__H__
#define __OS_API__H__

#include "bluetooth.h"

#ifdef __cplusplus
extern "C" {
#endif
    typedef void (*osapi_timer_notify) (void);

    void osapi_stop_hardware(void);

    void osapi_resume_hardware(void);

    void osapi_memcopy(U8 * dest, const U8 * source, U32 numBytes);

    void osapi_lock_stack(void);

    void osapi_unlock_stack(void);

    void osapi_notify_evm(void);
	
	uint8_t osapi_lock_is_exist(void);

#ifdef __cplusplus
}
#endif
#endif/*__OS_API__H__*/

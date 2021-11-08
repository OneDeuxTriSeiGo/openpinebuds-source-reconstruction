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

#ifndef __THREADING_ALT_H__
#define __THREADING_ALT_H__
#ifdef __cplusplus
extern "C" {
#endif


#if !defined(MBEDTLS_CMSIS_FILE)
#include "cmsis_os.h"
#else
#include MBEDTLS_CMSIS_FILE
#endif

typedef struct
{
    osMutexId mutex;
    char is_valid;
} mbedtls_threading_mutex_t;

void mbedtls_threading_set_alt( void ( * mutex_init )( mbedtls_threading_mutex_t * ),
                                       void ( * mutex_free )( mbedtls_threading_mutex_t * ),
                                       int ( * mutex_lock )( mbedtls_threading_mutex_t * ),
                                       int ( * mutex_unlock )( mbedtls_threading_mutex_t * ) );

void mbedtls_threading_free_alt( void );

void mbedtlsThreading_MutexInit(mbedtls_threading_mutex_t* mbedtlsMutex );
void mbedtlsThreading_MutexFree(mbedtls_threading_mutex_t* mbedtlsMutex );
int mbedtlsThreading_MutexLock(mbedtls_threading_mutex_t* mbedtlsMutex );
int mbedtlsThreading_MutexUnlock(mbedtls_threading_mutex_t* mbedtlsMutex );

#ifdef __cplusplus
}
#endif
#endif /* ifndef __THREADING_ALT_H__ */

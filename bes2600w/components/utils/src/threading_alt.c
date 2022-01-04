/***************************************************************************
 *
 * Copyright 2015-2019 BES.
 * All rights reserved. All unpublished rights reserved.
 *
 * No part of this work may be used or reproduced in any form or by any
 * means, or stored in a database or retrieval system, without prior written
 * permission of BES.
 *
 * Use of this work is governed by a license granted by BES.
 * This work contains confidential and proprietary information of
 * BES. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_THREADING_ALT)

#include "threading.h"
#include "threading_alt.h"

osMutexDef(mbedtls_mutex);
void mbedtlsThreading_MutexInit(mbedtls_threading_mutex_t* mbedtlsMutex )
{
   mbedtlsMutex->mutex = osMutexCreate(osMutex(mbedtls_mutex));

   if( mbedtlsMutex->mutex != NULL )
   {
       mbedtlsMutex->is_valid = 1;
   }
   else
   {
       mbedtlsMutex->is_valid = 0;
   }
}

void mbedtlsThreading_MutexFree(mbedtls_threading_mutex_t* mbedtlsMutex )
{
   if( mbedtlsMutex->is_valid == 1 )
   {
       osMutexDelete( mbedtlsMutex->mutex );
       mbedtlsMutex->is_valid = 0;
   }
}

int mbedtlsThreading_MutexLock(mbedtls_threading_mutex_t* mbedtlsMutex )
{
   int ret = MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;

   if( mbedtlsMutex->is_valid == 1 )
   {
       if( osMutexWait(mbedtlsMutex->mutex, osWaitForever) == 0 )
       {
           ret = 0;
       }
       else
       {
           ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
       }
   }

   return ret;
}

int mbedtlsThreading_MutexUnlock(mbedtls_threading_mutex_t* mbedtlsMutex )
{
   int ret = MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;

   if( mbedtlsMutex->is_valid == 1 )
   {
       if( osMutexRelease( mbedtlsMutex->mutex ) == 0 )
       {
           ret = 0;
       }
       else
       {
           ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
       }
   }

   return ret;
}
#endif
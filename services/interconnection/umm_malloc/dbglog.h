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
 * to set the trace level #define DBGLOG_LEVEL x
 *
 * To update which of the DBGLOG macros are compiled in, you must redefine the
 * DBGLOG_LEVEL macro and the inlcude the dbglog.h file again, like this:
 *
 * #undef  DBGLOG_LEVEL
 * #define DBGLOG_LEVEL 6
 * #include "dbglog/dbglog.txt"
 *
 * To handle multiple inclusion, we need to first undefine any macros we define
 * so that the compiler does not warn us that we are changing a macro.
 * ----------------------------------------------------------------------------
 * The DBGLOG_LEVEL and DBGLOG_FUNCTION should be defined BEFORE this
 * file is included or else the following defaults are used:
 *
 * #define DBGLOG_LEVEL    0
 * #define DBGLOG_FUNCTION printf
 * ----------------------------------------------------------------------------
 * There are macros to handle the following decreasing levels of detail:
 *
 * 6 = TRACE
 * 5 = DEBUG
 * 4 = CRITICAL
 * 3 = ERROR
 * 2 = WARNING
 * 1 = INFO
 * 0 = FORCE - The DBGLOG_FUNCTION is always compiled in and is called only when
 *              the first parameter to the macro is non-0
 * ----------------------------------------------------------------------------
 */
#ifndef __DBGLOG_H__
#define __DBGLOG_H__

#include "hal_trace.h"

#undef DBGLOG_TRACE
#undef DBGLOG_DEBUG
#undef DBGLOG_CRITICAL
#undef DBGLOG_ERROR
#undef DBGLOG_WARNING
#undef DBGLOG_INFO
#undef DBGLOG_FORCE

#define DBGLOG_LEVEL 3

#ifndef DBGLOG_LEVEL
#  define DBGLOG_LEVEL 0
#endif

#ifndef DBGLOG_FUNCTION
#  define DBGLOG_FUNCTION TRACE
#endif

/* ------------------------------------------------------------------------- */

#if DBGLOG_LEVEL >= 6
#  define DBGLOG_TRACE(num,format, ...) DBGLOG_FUNCTION(num,format, ## __VA_ARGS__)
#else
#  define DBGLOG_TRACE(format, ...)
#endif

#if DBGLOG_LEVEL >= 5
#  define DBGLOG_DEBUG(num,format, ...) DBGLOG_FUNCTION(num,format, ## __VA_ARGS__)
#else
#  define DBGLOG_DEBUG(format, ...)
#endif

#if DBGLOG_LEVEL >= 4
#  define DBGLOG_CRITICAL(num,format, ...) DBGLOG_FUNCTION(num,format, ## __VA_ARGS__)
#else
#  define DBGLOG_CRITICAL(format, ...)
#endif

#if DBGLOG_LEVEL >= 3
#  define DBGLOG_ERROR(num,format, ...) DBGLOG_FUNCTION(num,format, ## __VA_ARGS__)
#else
#  define DBGLOG_ERROR(format, ...)
#endif

#if DBGLOG_LEVEL >= 2
#  define DBGLOG_WARNING(num,format, ...) DBGLOG_FUNCTION(num,format, ## __VA_ARGS__)
#else
#  define DBGLOG_WARNING(format, ...)
#endif

#if DBGLOG_LEVEL >= 1
#  define DBGLOG_INFO(num,format, ...) DBGLOG_FUNCTION(num,format, ## __VA_ARGS__)
#else
#  define DBGLOG_INFO(format, ...)
#endif

#endif
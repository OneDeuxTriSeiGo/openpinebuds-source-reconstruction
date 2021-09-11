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
#ifndef _TARGET_CONFIG_H
#define _TARGET_CONFIG_H

#include "cmsis.h"
#include "los_compiler.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef   __weak
#define __weak                                 __attribute__((weak))
#endif

/* =============================================================================
                                        System clock module configuration
============================================================================= */
/**
 * @ingroup los_config
 * System clock (unit: HZ)
 */
#if defined(OSTICK_USE_FAST_TIMER)
#define OS_SYS_CLOCK            (1000UL)
#else
#if (CHIP_BEST2003)
extern uint32_t hal_cmu_get_fast_timer_freq(void);
#define CONFIG_FAST_SYSTICK_HZ  hal_cmu_get_fast_timer_freq()
#define OS_SYS_CLOCK            CONFIG_FAST_SYSTICK_HZ
#else
#define OS_SYS_CLOCK            CONFIG_SYSTICK_HZ
#endif
#endif

/**
 * @ingroup los_config
 * Number of Ticks in one second
 */
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                    (1000UL)

#if defined(LOSCFG_BASE_CORE_TICK_PER_SECOND) && \
    ((LOSCFG_BASE_CORE_TICK_PER_SECOND < 1UL) || (LOSCFG_BASE_CORE_TICK_PER_SECOND > 1000000000UL))
    #error "LOSCFG_BASE_CORE_TICK_PER_SECOND SHOULD big than 0, and less than 1000000000UL"
#endif


#if (LOSCFG_BASE_CORE_TICK_PER_SECOND <= 1000UL)
/**
 * @ingroup los_config
 * How much time one tick spent (unit:ms)
 */
#ifndef LOSCFG_BASE_CORE_TICK_PERIOD_MS
#define LOSCFG_BASE_CORE_TICK_PERIOD_MS                     (1000UL / LOSCFG_BASE_CORE_TICK_PER_SECOND)
#endif

#elif (LOSCFG_BASE_CORE_TICK_PER_SECOND <= 1000000UL)
/**
 * @ingroup los_config
 * How much time one tick spent (unit:us)
 */
#ifndef LOSCFG_BASE_CORE_TICK_PERIOD_US
#define LOSCFG_BASE_CORE_TICK_PERIOD_US                     (1000000UL / LOSCFG_BASE_CORE_TICK_PER_SECOND)
#endif

#else
/**
 * @ingroup los_config
 * How much time one tick spent (unit:ns)
 */
#ifndef LOSCFG_BASE_CORE_TICK_PERIOD_NS
#define LOSCFG_BASE_CORE_TICK_PERIOD_NS                     (1000000000UL / LOSCFG_BASE_CORE_TICK_PER_SECOND)
#endif
#endif

#define LOSCFG_BASE_CORE_TICK_HW_TIME                       1

/* =============================================================================
                                        Hardware interrupt module configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration item for hardware interrupt tailoring
 */
#ifndef LOSCFG_PLATFORM_HWI
#define LOSCFG_PLATFORM_HWI                                 1
#endif

#define LOSCFG_USE_SYSTEM_DEFINED_INTERRUPT                 0

#ifndef LOSCFG_PLATFORM_HWI_LIMIT
#define LOSCFG_PLATFORM_HWI_LIMIT                           32
#endif


/* =============================================================================
                                       Task module configuration
============================================================================= */
/**
 * @ingroup los_config
 * Minimum stack size.
 *
 * 0x80 bytes, aligned on a boundary of 8.
 */
#ifndef LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE
#define LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE                 (ALIGN(0x80, 4))
#endif

/**
 * @ingroup los_config
 * Default task priority
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO                   15

/**
 * @ingroup los_config
 * Maximum supported number of tasks except the idle task rather than the number of usable tasks
 */
#define LOSCFG_BASE_CORE_TSK_LIMIT                          150

/**
 * @ingroup los_config
 * Size of the idle task stack
 */
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE                0x1000UL

/**
 * @ingroup los_config
 * Default task stack size
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE             0x1000UL

/**
 * @ingroup los_config
 * Configuration item for task Robin tailoring
 */
#ifndef LOSCFG_BASE_CORE_TIMESLICE
#define LOSCFG_BASE_CORE_TIMESLICE                          1
#endif

/**
 * @ingroup los_config
 * Longest execution time of tasks with the same priorities
 */
#ifndef LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT                  10
#endif

/**
 * @ingroup los_config
 * Configuration item for task (stack) monitoring module tailoring
 */
#ifndef LOSCFG_BASE_CORE_TSK_MONITOR
#define LOSCFG_BASE_CORE_TSK_MONITOR                        1
#endif

/**
 * @ingroup los_config
 * Configuration item for task perf task filter hook
 */
#ifndef LOSCFG_BASE_CORE_EXC_TSK_SWITCH
#define LOSCFG_BASE_CORE_EXC_TSK_SWITCH                     0
#endif

/**
 * @ingroup los_config
 * Define a usable task priority.Highest task priority.
 */
#ifndef LOS_TASK_PRIORITY_HIGHEST
#define LOS_TASK_PRIORITY_HIGHEST                           0
#endif

/**
 * @ingroup los_config
 * Define a usable task priority.Lowest task priority.
 */
#ifndef LOS_TASK_PRIORITY_LOWEST
#define LOS_TASK_PRIORITY_LOWEST                            31
#endif

/**
 * @ingroup los_config
 * Configuration item for task stack independent
 */
#ifndef LOSCFG_BASE_CORE_TASKSTACK_INDEPENDENT
#define LOSCFG_BASE_CORE_TASKSTACK_INDEPENDENT              0
#endif

/**
 * @ingroup los_config
 * SP align size
 */
#ifndef LOSCFG_STACK_POINT_ALIGN_SIZE
#define LOSCFG_STACK_POINT_ALIGN_SIZE                       8
#endif

/* =============================================================================
                                       Semaphore module configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration item for semaphore module tailoring
 */
#ifndef LOSCFG_BASE_IPC_SEM
#define LOSCFG_BASE_IPC_SEM                                 1
#endif

/**
 * @ingroup los_config
 * Maximum supported number of semaphores
 */
#define LOSCFG_BASE_IPC_SEM_LIMIT                           256

/* =============================================================================
                                       Mutex module configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration item for mutex module tailoring
 */
#ifndef LOSCFG_BASE_IPC_MUX
#define LOSCFG_BASE_IPC_MUX                                1
#endif

/**
 * @ingroup los_config
 * Maximum supported number of mutexes
 */
#define LOSCFG_BASE_IPC_MUX_LIMIT                           512

/* =============================================================================
                                       Queue module configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration item for queue module tailoring
 */
#ifndef LOSCFG_BASE_IPC_QUEUE
#define LOSCFG_BASE_IPC_QUEUE                               1
#endif

/**
 * @ingroup los_config
 * Maximum supported number of queues rather than the number of usable queues
 */
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                         256


/* =============================================================================
                                       Software timer module configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration item for software timer module tailoring
 */
#ifndef LOSCFG_BASE_CORE_SWTMR
#define LOSCFG_BASE_CORE_SWTMR                              1
#endif

/**
 * @ingroup los_config
 * Maximum supported number of software timers rather than the number of usable software timers
 */
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                        256

/**
 * @ingroup los_config
 * Software timer task stack size
 */
#ifndef LOSCFG_BASE_CORE_TSK_SWTMR_STACK_SIZE
#define LOSCFG_BASE_CORE_TSK_SWTMR_STACK_SIZE               LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE
#endif

/**
 * @ingroup los_config
 * Configurate item for software timer align tailoring
 */
#ifndef LOSCFG_BASE_CORE_SWTMR_ALIGN
#define LOSCFG_BASE_CORE_SWTMR_ALIGN                        0
#endif

/**
 * @ingroup los_config
 * Maximum size of a software timer queue
 */
#ifndef OS_SWTMR_HANDLE_QUEUE_SIZE
#define OS_SWTMR_HANDLE_QUEUE_SIZE                          (LOSCFG_BASE_CORE_SWTMR_LIMIT + 0)
#endif

/**
 * @ingroup los_config
 * Minimum divisor of software timer multiple alignment
 */
#ifndef LOS_COMMON_DIVISOR
#define LOS_COMMON_DIVISOR                                 10
#endif

/* =============================================================================
                                       Memory module configuration
============================================================================= */
#define LOSCFG_SYS_EXTERNAL_HEAP                            1
extern UINT8 __os_heap_start__[];

/**
 * @ingroup los_config
 * Starting address of the memory
 */
#ifndef LOSCFG_SYS_HEAP_ADDR
#define LOSCFG_SYS_HEAP_ADDR                                (__os_heap_start__)
#endif

/**
 * @ingroup los_config
 * Starting address of the task stack
 */
#ifndef OS_TASK_STACK_ADDR
#define OS_TASK_STACK_ADDR                                  LOSCFG_SYS_HEAP_ADDR
#endif

/**
 * @ingroup los_config
 * Memory size
 */
#define LOSCFG_SYS_HEAP_SIZE                                (OS_HEAP_SIZE)

/**
 * @ingroup los_config
 * Configuration module tailoring of more mempry pool checking
 */
#ifndef LOSCFG_MEM_MUL_POOL
#define LOSCFG_MEM_MUL_POOL                                 1
#endif

/**
 * @ingroup los_config
 * Configuration module tailoring of memory released by task id
 */
#ifndef LOSCFG_MEM_FREE_BY_TASKID
#define LOSCFG_MEM_FREE_BY_TASKID                           0
#endif

/**
 * @ingroup los_config
 * Configuration module tailoring of mem node integrity checking
 */
#ifndef LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK
#define LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK                0
#endif

/**
 * @ingroup los_config
 * Configuration memory leak detection
 */
#ifndef LOSCFG_MEM_LEAKCHECK
#define LOSCFG_MEM_LEAKCHECK                                0
#endif

/**
 * @ingroup los_config
 * Configuration memory leak recorded num
 */
#ifndef LOSCFG_MEM_LEAKCHECK_RECORD_MAX_NUM
#define LOSCFG_MEM_LEAKCHECK_RECORD_MAX_NUM                 1024
#endif

/**
 * @ingroup los_config
 * Configuration of memory pool record memory consumption waterline
 */
#ifndef LOSCFG_MEM_WATERLINE
#define LOSCFG_MEM_WATERLINE                                1
#endif

/**
 * @ingroup los_config
 * Number of memory checking blocks
 */
#ifndef OS_SYS_MEM_NUM
#define OS_SYS_MEM_NUM                                      20
#endif

/**
 * @ingroup los_config
 * Size of unaligned memory
 */
#ifndef OS_SYS_NOCACHEMEM_SIZE
#define OS_SYS_NOCACHEMEM_SIZE                              0x0UL
#endif

/**
 * @ingroup los_config
 * Starting address of the unaligned memory
 */
#if (OS_SYS_NOCACHEMEM_SIZE > 0)
#define OS_SYS_NOCACHEMEM_ADDR                              (&g_sysNoCacheMem0[0])
#endif

/* =============================================================================
                                        Exception module configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration item for exception tailoring
 */
#define LOSCFG_PLATFORM_EXC                                 1

/**
 * @ingroup los_config
 * Configuration of hardware stack protection
 */
#ifndef LOSCFG_EXC_HRADWARE_STACK_PROTECTION
#define LOSCFG_EXC_HRADWARE_STACK_PROTECTION                0
#endif

/* =============================================================================
                                        CPUP configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration item for CPU usage tailoring
 */
#ifndef LOSCFG_BASE_CORE_CPUP
#define LOSCFG_BASE_CORE_CPUP                               1
#endif

/* =============================================================================
                                       Test module configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration test case to open
 */

/**
 * @ingroup los_config
 * Configuration CMSIS_OS_VER
 */
#ifndef CMSIS_OS_VER
#define CMSIS_OS_VER                                       2
#endif

/* =============================================================================
                                       trace configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration liteos trace
 */
#ifndef LOSCFG_KERNEL_TRACE
#define LOSCFG_KERNEL_TRACE                                 0
#endif

/* =============================================================================
                                       printf configuration
============================================================================= */
/**
 * @ingroup los_config
 * Configuration liteos printf
 */
#define LOSCFG_KERNEL_PRINTF                                2

#define LOSCFG_KERNEL_RUNSTOP	1

#define LOSCFG_BACKTRACE_TYPE 1

#define LOSCFG_BASE_CORE_TICK_RESPONSE_MAX 0xFFFFFFFF

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _TARGET_CONFIG_H */

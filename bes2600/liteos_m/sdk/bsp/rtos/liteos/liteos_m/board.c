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
#include "cmsis.h"
#include "securec.h"
#include "cmsis_os.h"
#include "cmsis_nvic.h"
#include "hal_timer.h"
#include "hal_timer_fast_irq.h"
#include "hal_trace.h"
#include "hal_gpio.h"
#include "hal_cache.h"
#include "hal_uart.h"
#include "los_config.h"
#include "string.h"
#include "los_memory.h"
#include "los_context.h"
#include "los_arch_interrupt.h"
#include "los_debug.h"
#include "los_exchook.h"
#include "los_backtrace.h"
#include "los_tick.h"
#include "stdio.h"

#ifndef CP_BUILD
#undef OS_SYS_CLOCK
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
#if defined(CHIP_BEST2002) || defined(CHIP_BEST2003)
#define OS_SYS_CLOCK            (6000000UL)
#else
#define OS_SYS_CLOCK            CONFIG_SYSTICK_HZ
#endif
#endif
#endif

#define SYSTICK_EXTERNAL_CLOCK          1
#ifndef OS_TASK_FLAG_SYSTEM_TASK
#define OS_TASK_FLAG_SYSTEM_TASK                    0x1000U
#endif
extern VOID *LOS_MemAlloc(VOID *pool, UINT32 size);
extern UINT32 LOS_MemFree(VOID *pool, VOID *mem);
extern VOID *LOS_MemRealloc(VOID *pool, VOID *ptr, UINT32 size);
extern int platform_init_step1(int user_init);
extern int platform_init_step0(int release_version);
extern void platform_init_step2(int step1_init);

extern void OsTickHandler(void);
extern void SysTick_Handler(void);
extern UINT32 HalExcContextDump(UINT32 index, UINT32 *excContent);

#ifdef CP_BUILD
#define OS_VECTOR_SET NVIC_SetIrqHandler_cp
#else
#define OS_VECTOR_SET NVIC_SetIrqHandler
#endif

static uint8_t os_exc_occur = 0;
VOID HalConsoleOutput(LogModuleType type, INT32 level, const CHAR *fmt, ...)
{
    static UINT8 buffer[320];
    va_list list;
    if (level > PRINT_LEVEL) {
        return;
    }
    va_start(list, fmt);
    vsnprintf((char*)buffer, sizeof(buffer), fmt, list);
    va_end(list);
    if (os_exc_occur)
        hal_uart_output(buffer,strlen((const char*)buffer));
    else
        hal_trace_output(buffer,strlen((const char*)buffer));
}

#if !defined(CP_BUILD) && (LOSCFG_BACKTRACE_TYPE > 0)
#define OS_EXC_HOOK
#endif

#ifdef OS_EXC_HOOK
extern uint32_t __flashx_text_start__[];
extern uint32_t __flashx_text_end__[];
extern uint32_t __boot_sram_start__[];
extern uint32_t __boot_text_sram_end__[];
extern uint32_t __sram_text_start__[];
extern uint32_t __sram_text_end__[];
extern uint32_t __text_start__[];
extern uint32_t __text_end__[];
extern uint32_t __fast_sram_text_exec_start__[];
extern uint32_t __fast_sram_text_exec_end__[];

#define OS_IS_THUMB_INSTRUCTION(pc)    ((pc & 0x1) == 1)

/* BL or BLX instruction flag. */
#define OS_BL_INS_MASK     0xF800
#define OS_BL_INS_HIGH     0xF800
#define OS_BL_INS_LOW      0xF000
#define OS_BLX_INX_MASK    0xFF00
#define OS_BLX_INX         0x4700
STATIC INLINE BOOL OsInsIsBlOrBlx(UINTPTR addr)
{
    UINT16 ins1 = *((UINT16 *)addr);
    UINT16 ins2 = *((UINT16 *)(addr + 2)); /* 2: Thumb instruction is two bytes. */

    if (((ins2 & OS_BL_INS_MASK) == OS_BL_INS_HIGH) &&
        ((ins1 & OS_BL_INS_MASK) == OS_BL_INS_LOW)) {
        return TRUE;
    } else if ((ins2 & OS_BLX_INX_MASK) == OS_BLX_INX) {
        return TRUE;
    } else {
        return FALSE;
    }
}

STATIC INLINE UINTPTR OsAddrIsValid(UINTPTR sp)
{
    UINTPTR pc;
    BOOL ret;

    /* The stack space pointed to by the current SP may store the LR,
       so need decrease a word to PC. */
    pc = *((UINTPTR *)sp) - sizeof(UINTPTR);

    if (!OS_IS_THUMB_INSTRUCTION(pc)) {
        return 0;
    }

    /* PC in thumb mode is an odd number, fix the PC address by decreasing one byte. */
    pc = *((UINTPTR *)sp) - 1;
    uint32_t * ptr = (uint32_t *)pc;
    if (!(((ptr >= __flashx_text_start__) && (ptr < __flashx_text_end__)) ||
        ((ptr >= __boot_sram_start__) && (ptr < __boot_text_sram_end__)) ||
        ((ptr >= __sram_text_start__) && (ptr < __sram_text_end__)) ||
        ((ptr >= __text_start__) && (ptr < __text_end__)) ||
        ((ptr >= __fast_sram_text_exec_start__) && (ptr < __fast_sram_text_exec_end__))))
    {
        return 0;
    }

    ret = OsInsIsBlOrBlx(pc - sizeof(UINTPTR));
    if (ret == FALSE) {
        return 0;
    }

    return pc;
}

STATIC INLINE UINTPTR HalSpGet(VOID)
{
    UINTPTR sp;
    __asm volatile("mov %0, sp" : "=r" (sp));
    return sp;
}

STATIC INLINE UINTPTR HalPspGet(VOID)
{
    UINTPTR psp;
    __asm volatile("mrs %0, psp" : "=r" (psp));
    return psp;
}

STATIC INLINE UINTPTR HalMspGet(VOID)
{
    UINTPTR msp;
    __asm volatile("mrs %0, msp" : "=r" (msp));
    return msp;
}

STATIC INLINE UINT32 OsStackAddrGet(UINTPTR *stackStart, UINTPTR *stackEnd, UINTPTR SP)
{
    if (SP != 0) {
        *stackStart = SP;
        if ((SP >= CODE_START_ADDR) && (SP < CSTACK_END_ADDR)) {
            *stackEnd = CSTACK_END_ADDR;
        } else {
            UINT32 taskID = LOS_CurTaskIDGet();
            LosTaskCB *taskCB = OS_TCB_FROM_TID(taskID);
            *stackEnd = (UINTPTR)taskCB->topOfStack + taskCB->stackSize;
            if ((SP < (UINTPTR)taskCB->topOfStack) || (SP >= *stackEnd)) {
                PRINT_ERR("msp statck [0x%x, 0x%x], cur task stack [0x%x, 0x%x], cur sp(0x%x) is overflow!\n",
                          CODE_START_ADDR, CSTACK_END_ADDR, (UINTPTR)taskCB->topOfStack, *stackEnd, SP);
                return LOS_NOK;
            }
        }
    } else {
        if (HalSpGet() != HalPspGet()) {
            *stackStart = HalMspGet();
            *stackEnd = CSTACK_END_ADDR;
            if ((*stackStart < CODE_START_ADDR) || (*stackStart >= CSTACK_END_ADDR)) {
                PRINT_ERR("msp stack [0x%x, 0x%x], cur sp(0x%x) is overflow!\n",
                          CODE_START_ADDR, CSTACK_END_ADDR, *stackStart);
                return LOS_NOK;
            }
            PRINTK("msp, start = %x, end = %x\n", *stackStart, *stackEnd);
        } else {
            *stackStart = HalPspGet();
            UINT32 taskID = LOS_CurTaskIDGet();
            LosTaskCB *taskCB = OS_TCB_FROM_TID(taskID);
            *stackEnd = (UINTPTR)taskCB->topOfStack + taskCB->stackSize;
            if ((*stackStart < (UINTPTR)taskCB->topOfStack) || (*stackStart >= *stackEnd)) {
                PRINT_ERR("psp stack [0x%x, 0x%x], cur sp(0x%x) is overflow, cur task id is %d!\n",
                          taskCB->topOfStack, *stackEnd, *stackStart, taskID);
                return LOS_NOK;
            }
            PRINTK("psp, start = %x, end = %x\n", *stackStart, *stackEnd);
        }
    }

    return LOS_OK;
}

VOID os_backtrace_hook(UINTPTR *LR, UINT32 LRSize, UINT32 jumpCount, UINTPTR SP)
{
    UINTPTR stackStart;
    UINTPTR stackEnd;
    UINT32 count = 0;
    UINT32 index = 0;
    UINTPTR sp;
    UINTPTR pc;
    UINT32 ret;

    if (LR == NULL) {
        return;
    }

    ret = OsStackAddrGet(&stackStart, &stackEnd, SP);
    if (ret != LOS_OK) {
        return;
    }

    /* Traverse the stack space and find the LR address. */
    for (sp = stackStart; sp < stackEnd; sp += sizeof(UINTPTR)) {
        pc = OsAddrIsValid(sp);
        if ((pc != 0) && (count < LRSize)) {
            if (index++ < jumpCount) {
                continue;
            }
            LR[count] = pc;
            count++;
            if (count == LRSize) {
                break;
            }
        }
    }
    if (count < LRSize) {
        LR[count] = 0;
    }
}

VOID os_exc_hook(EXC_TYPE excType)
{
    os_exc_occur = 1;
    hal_uart_printf_init();
}
#endif

#ifndef SYSTICK_IRQ_PRIORITY
#define SYSTICK_IRQ_PRIORITY    0xFFU
#endif
static uint8_t PendST;

// Get OS Tick IRQ number.
 __WEAK int32_t  OS_Tick_GetIRQn (void) {
#ifdef OSTICK_USE_FAST_TIMER
    return ((int32_t)TIMER11_IRQn);
#else
    return ((int32_t)SysTick_IRQn);
#endif
}

// Setup OS Tick.
static int32_t OS_Tick_Setup (uint32_t freq) {
  uint32_t load;

  if (freq == 0U) {
    //lint -e{904} "Return statement before end of function"
    return (-1);
  }

  load = OS_SYS_CLOCK/freq - 1U;
  if (load > SysTick_LOAD_RELOAD_Msk) {
    //lint -e{904} "Return statement before end of function"
    return (-1);
  }

  SysTick->CTRL = 0;
  // Set SysTick Interrupt Priority
#if   ((defined(__ARM_ARCH_8M_MAIN__) && (__ARM_ARCH_8M_MAIN__ != 0)) || \
       (defined(__CORTEX_M)           && (__CORTEX_M           == 7U)))
  SCB->SHPR[11] = SYSTICK_IRQ_PRIORITY;
#elif  (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
  SCB->SHPR[1] |= ((uint32_t)SYSTICK_IRQ_PRIORITY << 24);
#elif ((defined(__ARM_ARCH_7M__)      && (__ARM_ARCH_7M__      != 0)) || \
       (defined(__ARM_ARCH_7EM__)     && (__ARM_ARCH_7EM__     != 0)))
  SCB->SHP[11]  = SYSTICK_IRQ_PRIORITY;
#elif  (defined(__ARM_ARCH_6M__)      && (__ARM_ARCH_6M__      != 0))
  SCB->SHP[1]  |= ((uint32_t)SYSTICK_IRQ_PRIORITY << 24);
#else
#error "Unknown ARM Core!"
#endif

#ifdef OSTICK_USE_FAST_TIMER
  hal_fast_timer_setup(HAL_TIMER_TYPE_PERIODIC, (HAL_TIMER_IRQ_HANDLER_T)OsTickHandler);
  NVIC_SetPriority(OS_Tick_GetIRQn(), (1UL << __NVIC_PRIO_BITS) - 1UL);
  hal_fast_timer_start(US_TO_FAST_TICKS(1000000/freq));
  hal_fast_timer_pause();
#else /*OSTICK_USE_FAST_TIMER*/
#if (SYSTICK_EXTERNAL_CLOCK)
  SysTick->CTRL =  SysTick_CTRL_TICKINT_Msk;
#else
  SysTick->CTRL =  SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
#endif
  SysTick->LOAD =  load;
  SysTick->VAL  =  0U;
#endif /*OSTICK_USE_FAST_TIMER*/
  PendST = 0U;

  return (0);
}

#ifdef OSTICK_USE_FAST_TIMER
/// Enable OS Tick.
static void os_tick_enable (void)
{
    hal_fast_timer_continue();
}

/// Disable OS Tick.
static void os_tick_disable (void)
{
    hal_fast_timer_pause();
}
#endif

static uint32_t os_tick_init(HWI_PROC_FUNC tickHandler)
{
    OS_VECTOR_SET(15, tickHandler);
    OS_Tick_Setup(LOSCFG_BASE_CORE_TICK_PER_SECOND);
    return 0;
}

void os_pre_init_hook(void)
{
#ifdef OS_EXC_HOOK
    OS_VECTOR_SET(2, HalExcNMI);
    OS_VECTOR_SET(3, HalExcHardFault);
    OS_VECTOR_SET(4, HalExcMemFault);
    OS_VECTOR_SET(5, HalExcBusFault);
    OS_VECTOR_SET(6, HalExcUsageFault);
    OsBackTraceHookSet(os_backtrace_hook);
#endif
    OS_VECTOR_SET(11, HalSVCHandler);
    OS_VECTOR_SET(14, HalPendSV);
    hal_cache_sync_all(HAL_CACHE_ID_D_CACHE);

    ArchTickTimer tickTimer;
    (VOID)memcpy_s(&tickTimer, sizeof(ArchTickTimer), LOS_SysTickTimerGet(), sizeof(ArchTickTimer));
    tickTimer.freq = OS_SYS_CLOCK;
    tickTimer.irqNum = 15;
    tickTimer.init = os_tick_init;
    LOS_TickTimerRegister(&tickTimer, NULL);
}

extern UINT32 g_swtmrTaskID;
void os_post_init_hook(void)
{
    OS_TCB_FROM_TID(g_swtmrTaskID)->taskStatus &= ~OS_TASK_FLAG_SYSTEM_TASK;
}

#if !defined(MODULE_KERNEL_STUB)
__WEAK void OHOS_SystemInit(void)
{
    printf("%s stub function!\r\n",__FUNCTION__);
}
#ifndef CP_BUILD
__WEAK int OhosSystemAdapterHooks(void)
{
    printf("%s stub function!\r\n",__FUNCTION__);
    return 0;
}
#endif
#else
#ifndef CP_BUILD
extern void OhosSystemAdapterHooks(void);
#endif
#endif

int main(void);
extern void OHOS_SystemInit(void);
extern int platform_io_cfg(uint32_t fem_idx, uint32_t pa_en_idx);
#ifndef __NO_APP__
void board_main(const void * arg)
{
#if defined(RF_TX_CONTROL_IO) || defined(PA_ENABLE_IO)
    uint32_t rf_tx_io = 0;
    uint32_t pa_enable_io = -1;
#endif
    UINTPTR intSave;
    intSave = LOS_IntLock();
#ifdef OS_EXC_HOOK
    LOS_RegExcHook(EXC_INTERRUPT, os_exc_hook);
    //LOS_BackTraceInit();
#endif
    LOS_IntRestore(intSave);
    platform_init_step0(0);
#ifdef RF_TX_CONTROL_IO
    rf_tx_io = RF_TX_CONTROL_IO;
#endif
#ifdef PA_ENABLE_IO
    pa_enable_io = PA_ENABLE_IO;
#endif
#if defined(RF_TX_CONTROL_IO) || defined(PA_ENABLE_IO)
    platform_io_cfg(rf_tx_io, pa_enable_io);
#endif
    int ret = platform_init_step1(0);

    if(!ret) {
#ifndef CP_BUILD
        OhosSystemAdapterHooks();
#endif
        OHOS_SystemInit();
    }
    hal_trace_crash_dump_register(HAL_TRACE_CRASH_DUMP_MODULE_SYS,
                                                          (HAL_TRACE_CRASH_DUMP_CB_T)LOS_TaskInfoMonitor);
    platform_init_step2(ret);
    while (1) {
        osDelay(1000);
        TRACE(0, "main idle");
    }
}
#else
void board_main(const void * arg)
{
    UINTPTR intSave;
    intSave = LOS_IntLock();
#ifdef OS_EXC_HOOK
    LOS_RegExcHook(EXC_INTERRUPT, os_exc_hook);
    OsBackTraceHookSet(os_backtrace_hook);
    //LOS_BackTraceInit();
#endif
    LOS_IntRestore(intSave);
    main();
}
#endif

osThreadDef_t os_thread_def_main = {(board_main),{"main",osThreadDetached,NULL,0U,0,0x4000,(osPriorityHigh),0U,0U}};
#ifdef __NO_STARTFILES__
void _start(void)
{
    os_pre_init_hook();
    osKernelInitialize();
    osThreadCreate(&os_thread_def_main, 0);
    os_post_init_hook();
    osKernelStart();
}
#else
#ifdef LIBC_INIT_AFTER_OS_INIT
__attribute__((naked)) void software_init_hook (void) {
  __asm (
    ".syntax unified\n"
    ".thumb\n"
    "bl   os_pre_init_hook\n"
    "bl   osKernelInitialize\n"
    "movs r0,#0\n"
    "movs r1,#0\n"
    "mov  r4,r0\n"
    "mov  r5,r1\n"
    "ldr  r0,= __libc_fini_array\n"
    "bl   atexit\n"
    "bl   __libc_init_array\n"
    "mov  r0,r4\n"
    "mov  r1,r5\n"
    "ldr  r0,=os_thread_def_main\n"
    "movs r1,#0\n"
    "bl   osThreadCreate\n"
    "bl   os_post_init_hook\n"
    "bl   osKernelStart\n"
    "bl   exit\n"
  );
}
#else
__attribute__((naked)) void software_init_hook (void) {
  __asm (
    ".syntax unified\n"
    ".thumb\n"
    "movs r0,#0\n"
    "movs r1,#0\n"
    "mov  r4,r0\n"
    "mov  r5,r1\n"
    "ldr  r0,= __libc_fini_array\n"
    "bl   atexit\n"
    "bl   __libc_init_array\n"
    "mov  r0,r4\n"
    "mov  r1,r5\n"
    "bl   os_pre_init_hook\n"
    "bl   osKernelInitialize\n"
    "ldr  r0,=os_thread_def_main\n"
    "movs r1,#0\n"
    "bl   osThreadCreate\n"
    "bl   os_post_init_hook\n"
    "bl   osKernelStart\n"
    "bl   exit\n"
  );
}
#endif
#endif


#ifndef OHOS_WRAP_MALLOC
void *__wrap_malloc(size_t size)
{
    return LOS_MemAlloc(OS_SYS_MEM_ADDR, size);
}

void *__wrap__malloc_r(void *reent, size_t nbytes)
{
    return LOS_MemAlloc(OS_SYS_MEM_ADDR, nbytes);
}

void *__wrap__realloc_r(void *reent,
                 void *aptr, size_t nbytes)
{
    ///TODO: Fix bug of LOS_MemRealloc
    //return LOS_MemRealloc(OS_SYS_MEM_ADDR, rmem, newsize);
    void * ptr = NULL;
    if (nbytes) {
        ptr = LOS_MemAlloc(OS_SYS_MEM_ADDR, nbytes);
        if (ptr && aptr) {
            memcpy(ptr, aptr, nbytes);
            LOS_MemFree(OS_SYS_MEM_ADDR, aptr);
        }
    } else
        LOS_MemFree(OS_SYS_MEM_ADDR, aptr);
    return ptr;
}


void *__wrap__calloc_r(struct _reent *reent, size_t nitems, size_t size)
{
    size_t real_size;
    void *ptr = NULL;

    if (nitems == 0 || size == 0) {
        return NULL;
    }

    real_size = (size_t)(nitems * size);
    ptr = LOS_MemAlloc(OS_SYS_MEM_ADDR, real_size);
    if (ptr != NULL) {
        (void)memset(ptr, 0, real_size);
    }
    return ptr;
}

void *__wrap_calloc(size_t count, size_t size)
{
    void * ptr = NULL;
    ptr = LOS_MemAlloc(OS_SYS_MEM_ADDR, count * size);
    if (ptr)
        memset(ptr, 0, count * size);
    return ptr;
}

void *__wrap_realloc(void * rmem, size_t newsize)
{
    ///TODO: Fix bug of LOS_MemRealloc
    //return LOS_MemRealloc(OS_SYS_MEM_ADDR, rmem, newsize);
    void * ptr = NULL;
    if (newsize) {
        ptr = LOS_MemAlloc(OS_SYS_MEM_ADDR, newsize);
        if (ptr && rmem) {
            memcpy(ptr, rmem, newsize);
            LOS_MemFree(OS_SYS_MEM_ADDR, rmem);
        }
    } else
        LOS_MemFree(OS_SYS_MEM_ADDR, rmem);
    return ptr;
}

void __wrap__free_r(struct _reent *reent, void *aptr)
{
    if (aptr == NULL) {
        return;
    }

    LOS_MemFree(OS_SYS_MEM_ADDR, aptr);
}

void __wrap_free(void *rmem)
{
    LOS_MemFree(OS_SYS_MEM_ADDR, rmem);
    return;
}
#endif

void os_heap_stat_print(void)
{
  uint8_t fragment = 0;
  uint8_t usage = 0;
  LOS_MEM_POOL_STATUS poolStatus = {0};
  (VOID) LOS_MemInfoGet(LOSCFG_SYS_HEAP_ADDR, &poolStatus);
  if (poolStatus.totalFreeSize != 0) {
    fragment = 100 - poolStatus.maxFreeNodeSize * 100 / poolStatus.totalFreeSize; /* 100: percent denominator. */
  }
  if (LOS_MemPoolSizeGet(LOSCFG_SYS_HEAP_ADDR) != 0) {
    usage = LOS_MemTotalUsedGet(LOSCFG_SYS_HEAP_ADDR) * 100 / LOS_MemPoolSizeGet(LOSCFG_SYS_HEAP_ADDR); /* 100: percent denominator. */
  }
  TRACE(0, "os heap usage = %d, fragment = %d, maxFreeSize = %d, totalFreeSize = %d\n", usage, fragment,
        poolStatus.maxFreeNodeSize, poolStatus.totalFreeSize);
}

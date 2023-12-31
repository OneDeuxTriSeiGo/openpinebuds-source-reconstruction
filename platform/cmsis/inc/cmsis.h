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
#ifndef MBED_CMSIS_H
#define MBED_CMSIS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_addr_map.h"
#include _TO_STRING(CONCAT_SUFFIX(CHIP_ID_LITERAL, h))

#define IRQ_PRIORITY_REALTIME               0
#define IRQ_PRIORITY_HIGHPLUSPLUS           1
#define IRQ_PRIORITY_HIGHPLUS               2
#define IRQ_PRIORITY_HIGH                   3
#define IRQ_PRIORITY_ABOVENORMAL            4
#define IRQ_PRIORITY_NORMAL                 5
#define IRQ_PRIORITY_BELOWNORMAL            6
#define IRQ_PRIORITY_LOW                    7

#ifdef __ARM_ARCH_ISA_ARM
#define IRQ_LOCK_MASK                       (CPSR_I_Msk | CPSR_F_Msk)
#else
#define NVIC_USER_IRQ_OFFSET                16
#define NVIC_NUM_VECTORS                    (NVIC_USER_IRQ_OFFSET + USER_IRQn_QTY)
#endif

#ifndef __ASSEMBLER__

#ifdef __ARMCC_VERSION
// Stupid armclang
#undef __SSAT
#define __SSAT(ARG1,ARG2) \
__extension__ \
({                          \
  int32_t __RES, __ARG1 = (ARG1); \
  __ASM ("ssat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \
  __RES; \
 })
#endif

__STATIC_FORCEINLINE uint32_t int_lock_global(void)
{
#ifdef __ARM_ARCH_ISA_ARM
    uint32_t cpsr = __get_CPSR();
    uint32_t st = cpsr & IRQ_LOCK_MASK;
    if (st != IRQ_LOCK_MASK) {
        cpsr |= IRQ_LOCK_MASK;
        __set_CPSR(cpsr);
    }
    return st;
#else
	uint32_t pri = __get_PRIMASK();
	if ((pri & 0x1) == 0) {
		__disable_irq();
	}
	return pri;
#endif
}

__STATIC_FORCEINLINE void int_unlock_global(uint32_t pri)
{
#ifdef __ARM_ARCH_ISA_ARM
    if (pri != IRQ_LOCK_MASK) {
        uint32_t cpsr = __get_CPSR();
        cpsr = (cpsr & ~IRQ_LOCK_MASK) | pri;
        __set_CPSR(cpsr);
    }
#else
	if ((pri & 0x1) == 0) {
		__enable_irq();
	}
#endif
}

__STATIC_FORCEINLINE uint32_t int_lock(void)
{
#ifdef INT_LOCK_EXCEPTION
#ifdef __ARM_ARCH_ISA_ARM
    uint32_t mask = GIC_GetInterfacePriorityMask();
    // Only allow IRQs with priority IRQ_PRIORITY_HIGHPLUSPLUS and IRQ_PRIORITY_REALTIME
    GIC_SetInterfacePriorityMask(((IRQ_PRIORITY_HIGHPLUS << (8 - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL));
    return mask;
#else
    uint32_t pri = __get_BASEPRI();
    // Only allow IRQs with priority IRQ_PRIORITY_HIGHPLUSPLUS and IRQ_PRIORITY_REALTIME
    __set_BASEPRI(((IRQ_PRIORITY_HIGHPLUS << (8 - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL));
    return pri;
#endif
#else
    return int_lock_global();
#endif
}

__STATIC_FORCEINLINE void int_unlock(uint32_t pri)
{
#ifdef INT_LOCK_EXCEPTION
#ifdef __ARM_ARCH_ISA_ARM
    GIC_SetInterfacePriorityMask(pri);
#else
    __set_BASEPRI(pri);
#endif
#else
    int_unlock_global(pri);
#endif
}

__STATIC_FORCEINLINE int in_isr(void)
{
#ifdef __ARM_ARCH_ISA_ARM
    uint32_t mode = __get_mode();
    return mode != CPSR_M_USR && mode != CPSR_M_SYS;
#else
    return __get_IPSR() != 0;
#endif
}

__STATIC_FORCEINLINE int32_t ftoi_nearest(float f)
{
    return (f >= 0) ? (int32_t)(f + 0.5) : (int32_t)(f - 0.5);
}

void GotBaseInit(void);

int set_bool_flag(bool *flag);

void clear_bool_flag(bool *flag);

float db_to_float(float db);

uint32_t get_msb_pos(uint32_t val);

uint32_t get_lsb_pos(uint32_t val);

uint32_t integer_sqrt(uint32_t val);

uint32_t integer_sqrt_nearest(uint32_t val);

#endif

#ifdef __cplusplus
}
#endif

#endif

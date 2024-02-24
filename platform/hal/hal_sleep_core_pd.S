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

/*
=========================================================================
    Core Sleep Power Down and Wakeup Power Up Helper Functions
=========================================================================
*/

    #include "cmsis.h"

    .syntax unified

#ifdef CORE_SLEEP_POWER_DOWN

#ifdef ROM_BUILD
    .section .bss.core_sleep_stack
#else
    .section .sram_bss.core_sleep_stack
#endif
    .align    2
core_sleep_stack:
    .space 4

#ifdef ROM_BUILD
    .section .text.hal_sleep_core_power_down, "ax", %progbits
#else
    .section .sram_text.hal_sleep_core_power_down, "ax", %progbits
#endif
    .thumb
    .thumb_func
    .align    2
    .globl    hal_sleep_core_power_down
    .type    hal_sleep_core_power_down, %function
hal_sleep_core_power_down:

    ldr r3, =core_sleep_stack
    push {r4-r12,lr}
#ifdef __ARM_ARCH_8M_MAIN__
    mrs r0, msplim
    push {r0,r1}
#endif
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    ldr     r1, =0xE000EF34
    ldr     r0, [r1]                /* Load FPCCR */
    bic     r0, #1                  /* Clear LSPACT (Lazy state) */
    str     r0, [r1]                /* Store FPCCR */
#endif
    /* TODO: Save FPU registers? */
    str sp, [r3]
    bl hal_psc_core_auto_power_down
.pd_loop:
    b .pd_loop

    .pool
    .size    hal_sleep_core_power_down, . - hal_sleep_core_power_down


#ifdef ROM_BUILD
    .section .text.hal_sleep_core_power_up, "ax", %progbits
#else
    .section .sram_text.hal_sleep_core_power_up, "ax", %progbits
#endif
    .thumb
    .thumb_func
    .align    2
    .globl    hal_sleep_core_power_up
    .type    hal_sleep_core_power_up, %function
hal_sleep_core_power_up:

    ldr r3, =core_sleep_stack
    ldr sp, [r3]
    isb
    /* TODO: Restore FPU registers? */
#ifdef __ARM_ARCH_8M_MAIN__
    pop {r0,r1}
    msr msplim, r0
#endif
    pop {r4-r12,lr}
    bx lr

    .pool
    .size    hal_sleep_core_power_up, . - hal_sleep_core_power_up

#endif

    .end

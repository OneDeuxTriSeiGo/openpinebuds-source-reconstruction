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
#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*NVIC_DEFAULT_FAULT_HANDLER_T)(void);

void NVIC_DisableAllIRQs(void);

void NVIC_InitVectors(void);

void NVIC_SetDefaultFaultHandler(NVIC_DEFAULT_FAULT_HANDLER_T handler);

void NVIC_SetDefaultFaultHandler_cp(NVIC_DEFAULT_FAULT_HANDLER_T handler);

void NVIC_SetResetHandler(NVIC_DEFAULT_FAULT_HANDLER_T handler);

void NVIC_SetIrqHandler(uint16_t irq, NVIC_DEFAULT_FAULT_HANDLER_T handler);

void NVIC_SetIrqHandler_cp(uint16_t irq, NVIC_DEFAULT_FAULT_HANDLER_T handler);

IRQn_Type NVIC_GetCurrentActiveIRQ(void);

void NVIC_PowerDownSleep(uint32_t *buf, uint32_t cnt);

void NVIC_PowerDownWakeup(uint32_t *buf, uint32_t cnt);

uint32_t* NVIC_GetVectorTab();

#ifdef __ARM_ARCH_ISA_ARM
enum EXCEPTION_ID_T {
    EXCEPTION_NONE = -1,
    EXCEPTION_UNDEF = -2,
    EXCEPTION_SVC = -3,
    EXCEPTION_PABT = -4,
    EXCEPTION_DABT = -5,
    EXCEPTION_HYP = -6,
    EXCEPTION_IRQ = -7,
    EXCEPTION_FIQ = -8,
};

struct FAULT_REGS_T {
    uint32_t r[16];
    uint32_t spsr;
};

struct UNDEF_FAULT_INFO_T {
    enum EXCEPTION_ID_T id;
    uint32_t opcode;
    uint32_t state;
};

struct SVC_FAULT_INFO_T {
    enum EXCEPTION_ID_T id;
    uint32_t svc_num;
};

struct PABT_FAULT_INFO_T {
    enum EXCEPTION_ID_T id;
    uint32_t IFSR;
    uint32_t IFAR;
};

struct DABT_FAULT_INFO_T {
    enum EXCEPTION_ID_T id;
    uint32_t DFSR;
    uint32_t DFAR;
};

typedef void (*GIC_FAULT_DUMP_HANDLER_T)(const uint32_t *regs, const uint32_t *extra, uint32_t extra_len);

void GIC_DisableAllIRQs(void);

void GIC_InitVectors(void);

void GIC_SetFaultDumpHandler(GIC_FAULT_DUMP_HANDLER_T handler);

IRQn_Type IRQ_GetCurrentActiveIRQ(void);
#endif

#ifdef __cplusplus
}
#endif

#endif

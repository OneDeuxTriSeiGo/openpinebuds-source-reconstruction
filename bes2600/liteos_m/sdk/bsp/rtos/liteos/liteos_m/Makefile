# Copyright (c) 2021 Bestechnic (Shanghai) Co., Ltd. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
make_file := $(abspath $(lastword $(MAKEFILE_LIST)))
os_path   := $(dir $(make_file))

#LITEOS_SRCS += $(wildcard $(os_path)components/bounds_check/*.c)
LITEOS_SRCS += $(wildcard $(os_path)kal/cmsis/*.c)

ifeq ($(MODULE_KERNEL_STUB),1)
subdir-ccflags-y += -DMODULE_KERNEL_STUB
else
ifeq ($(LPLAYER_NEED_POSIX),1)
LITEOS_SRCS += $(wildcard $(os_path)kal/posix/src/*.c)
ccflags-y += -DLPLAYER_NEED_POSIX
endif
LITEOS_SRCS += $(wildcard $(os_path)utils/*.c)
LITEOS_SRCS += $(wildcard $(os_path)components/bounds_checking_function/src/*.c)
LITEOS_SRCS += $(wildcard $(os_path)components/exchook/*.c)
LITEOS_SRCS += $(wildcard $(os_path)components/cpup/*.c)
LITEOS_SRCS += $(wildcard $(os_path)components/backtrace/*.c)
LITEOS_SRCS += $(wildcard $(os_path)components/power/*.c)
LITEOS_SRCS += $(wildcard $(os_path)kernel/src/*.c)
LITEOS_SRCS += $(wildcard $(os_path)kernel/src/mm/*.c)

# trustzone build
ifeq ($(ARM_CMNS),1)
LITEOS_SRCS += $(wildcard $(os_path)arch/arm/cortex-m33/gcc/TZ/non_secure/*.c)
LITEOS_AS   += $(wildcard $(os_path)arch/arm/cortex-m33/gcc/TZ/non_secure/*.S)
LITEOS_SRCS += $(wildcard $(os_path)arch/arm/cortex-m33/gcc/TZ/secure/*.c)
LITEOS_AS   += $(wildcard $(os_path)arch/arm/cortex-m33/gcc/TZ/secure/*.S)
else
LITEOS_SRCS += $(wildcard $(os_path)arch/arm/cortex-m33/gcc/NTZ/*.c)
LITEOS_AS   += $(wildcard $(os_path)arch/arm/cortex-m33/gcc/NTZ/*.S)
endif
STRIP_SRCS_AS = $(subst $(os_path),,$(LITEOS_AS))
obj-y += $(patsubst %.S, %.o, $(STRIP_SRCS_AS))
endif

STRIP_SRCS = $(subst $(os_path),,$(LITEOS_SRCS))

obj-y += $(patsubst %.c, %.o, $(STRIP_SRCS))
obj-y += board.o

# include extern kernel header
ifeq ($(MODULE_KERNEL_STUB_INC),1)
EXTERN_ROOT_PATH ?= ./../../../../../
${info "EXTERN_ROOT_PATH:$(EXTERN_ROOT_PATH)"}
INC_DIR  = -I$(EXTERN_ROOT_PATH)kernel/liteos_m/kernel/include/
INC_DIR  += -I$(EXTERN_ROOT_PATH)kernel/liteos_m/utils/
INC_DIR  += -I$(EXTERN_ROOT_PATH)kernel/liteos_m/kernel/arch/include/
INC_DIR  += -I$(EXTERN_ROOT_PATH)kernel/liteos_m/arch/include/
INC_DIR  += -I$(EXTERN_ROOT_PATH)kernel/liteos_m/kernel/arch/arm/cortex-m33/gcc/NTZ/
INC_DIR  += -I$(EXTERN_ROOT_PATH)kernel/liteos_m/arch/arm/cortex-m33/gcc/NTZ/
INC_DIR  += -I$(EXTERN_ROOT_PATH)third_party/bounds_checking_function/include/
INC_DIR  += -I$(EXTERN_ROOT_PATH)kernel/liteos_m/components/exchook/
INC_DIR  += -I$(EXTERN_ROOT_PATH)kernel/liteos_m/components/backtrace/
INC_DIR  += -I$(EXTERN_ROOT_PATH)kernel/liteos_m/arch/arm/common/
ccflags-y += $(INC_DIR)
else
INC_DIR  = kernel/include
# trustzone build
ifeq ($(ARM_CMNS),1)
INC_DIR += arch/arm/cortex-m33/gcc/TZ/non_secure/
INC_DIR += arch/arm/cortex-m33/gcc/TZ/secure/
else
INC_DIR += arch/arm/cortex-m33/gcc/NTZ/
endif


INC_DIR += arch/include
INC_DIR += kal
INC_DIR += kal/cmsis
INC_DIR += utils
INC_DIR += components/bounds_checking_function/include
INC_DIR += components/exchook
ifeq ($(LPLAYER_NEED_POSIX),1)
INC_DIR += kal/posix/include/
INC_DIR += ../../../include/rtos/liteos/liteos_m/kal/posix/include
endif
INC_DIR += components/cpup
INC_DIR += components/backtrace
INC_DIR += components/power
ccflags-y += $(addprefix -I${os_path},${INC_DIR})
endif

ccflags-y += -I${os_path}../../../platform/hal/$(CHIP)

AFLAGS_los_dispatch.o += -D__FPU_PRESENT=1 -D__FPU_USED=1
AFLAGS_los_exc.o += -D__FPU_PRESENT=1 -D__FPU_USED=1

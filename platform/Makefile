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
cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))


obj-y := cmsis/ drivers/ hal/

ifneq ($(DUAL_BOOT),1)
ifneq ($(WIFI_RAMRUN),1)
obj-y += main/
endif
endif

ifeq ($(BECO),1)
obj-y += beco/
endif

subdir-ccflags-y += -Iplatform/cmsis/inc -Iplatform/hal -Iutils/hwtimer_list -Iplatform/drivers/wifi/$(CHIP)

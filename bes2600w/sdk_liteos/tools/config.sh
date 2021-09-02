#!/bin/bash
# Copyright (c) 2021 bestechnic (Shanghai) Technologies CO., LIMITED.
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

# == area config a7 liteos bin begin == #
OPT_BEST2600W_LITEOS_A7=" \
HISTEN_PROCESS_EN=0       \
"
# == area config a7 liteos bin end   == #

# == area config liteos main bin begin == #
OPT_BEST2600W_LITEOS_MAIN=" \
RF_TX_CONTROL_IO=12         \
PA_ENABLE_IO=21             \
UART1_IOMUX_INDEX=03        \
PSRAM_XCCELA_MODE=0         \
BT_RAMRUN=0                 \
ARM_CMNS=0                  \
ALSA_AUDIO_PROCESS_RENDER_EN=0 \
"
# == area config liteos main bin end   == #

# == area config liteos cp bin begin == #
OPT_BEST2600W_LITEOS_CP=" \
PSRAM_XCCELA_MODE=0       \
"
# == area config liteos cp bin end   == #


# == area config liteos main mini bin begin == #
OPT_BEST2600W_LITEOS_MAIN_MINI_SE=" \
PSRAM_XCCELA_MODE=0               \
"
# == area config liteos main mini bin end   == #


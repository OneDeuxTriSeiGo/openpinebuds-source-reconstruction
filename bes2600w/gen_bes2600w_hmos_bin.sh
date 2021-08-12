#!/usr/bin/env sh
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

build_product=$1

cd ../../../
WIFIIOT_SDK_EXE_NAME="wifiiot_sdk_bes2600w"
WIFIIOT_SDK_PATH="out/bes2600w/${build_product}/bin/"

if [ -f "$WIFIIOT_SDK_PATH$WIFIIOT_SDK_EXE_NAME.elf" ]; then
    arm-none-eabi-objcopy -O binary "$WIFIIOT_SDK_PATH$WIFIIOT_SDK_EXE_NAME.elf" "$WIFIIOT_SDK_PATH$WIFIIOT_SDK_EXE_NAME.bin"
    echo "build wifiiot_sdk.bin success!"
else
    echo "build wifiiot_sdk.bin failed!"
fi

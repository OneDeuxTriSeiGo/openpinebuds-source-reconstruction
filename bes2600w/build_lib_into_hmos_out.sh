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

build_product=$1
build_trustzone=$2
build_mini_sys=$3
build_type=`echo $4 | tr '[a-z]' '[A-Z]'`

HMOS_OUT_PATH="../../../out/bes2600w/${build_product}/libs/"
SDK_LIB_PATH="sdk_liteos/bsp/out/best2600w_liteos/libbest2600w_liteos.a"
if [ -f "$SDK_LIB_PATH" ]; then
    cp -rf $SDK_LIB_PATH $HMOS_OUT_PATH
    echo "cp libbest2600w_liteos.a to libs success!"
else
    echo "cp libbest2600w_liteos.a to libs failed!"
fi
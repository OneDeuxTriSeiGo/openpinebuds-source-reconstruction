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

# check if build sdk into lib

build_trustzone=$1
build_mini_sys=$2
build_bin_type=$3
product_path=$4

if [ "x$build_trustzone" == "x" ];then
    build_trustzone="false"
fi

rel=" "
rel_branch=" "
build_type="BUILD_TYPE=DEBUG"
if [ "x$build_bin_type" == "xrelease" ];then
    build_type=`echo $build_bin_type | tr '[a-z]' '[A-Z]'`
    build_type="BUILD_TYPE=$build_type"
    rel="-r"
    rel_branch=" branch=soc "
fi

if [ -f ${product_path}/config.sh ]; then
    source ${product_path}/config.sh
else
    source ./tools/config.sh
fi

cd sdk/bsp
#pre-handle options of target
SPACE=" "
function pre_handle_opt()
{
    eval cmd_opt="\${$2}"
#    cmd=`echo $cmd_opt`
   # printf "\n[$1] $cmd\n"
    filter_para=$1
    filter=${filter_para%%=*}
    # printf "\nfilter:$filter\n"
    flag_match=0
    temp_cmd=""
    for input_opt in ${cmd_opt}
    do
        # parse key value by = 
        input_opt_key=${input_opt%%=*}
        # cover same parameter by check key
        if [ "x${input_opt_key}" = "x${filter}" ] ;then
            printf "[MOD][$2]${input_opt} covered by ${filter_para}\n"
            temp_cmd+="$filter_para$SPACE"
            flag_match=1
            continue
         fi

         # keep current parameter not matched
         temp_cmd+="$input_opt$SPACE"
    done

    # handle no matched parameters
    if [ "x${flag_match}" = "x0" ] ;then
        printf "[ADD][$2] add ${filter_para}\n"
        temp_cmd+="$filter_para$SPACE"
    fi
    printf "[GEN][$2] ${temp_cmd} \n"
}

if [ "x${build_trustzone}" == "xtrue" ]; then
    pre_handle_opt ARM_CMNS=1 OPT_BEST2600W_LITEOS_MINI
    OPT_BEST2600W_LITEOS_MINI="${temp_cmd}"
    pre_handle_opt ARM_CMNS=1 OPT_BEST2600W_LITEOS_MAIN
    OPT_BEST2600W_LITEOS_MAIN="${temp_cmd}"
    pre_handle_opt TZ_MAIN_OFFSET=0x30000 OPT_BEST2600W_LITEOS_BOOT2A
    OPT_BEST2600W_LITEOS_BOOT2A="${temp_cmd}"
    pre_handle_opt TZ_MINI_OFFSET=0x56000 OPT_BEST2600W_LITEOS_BOOT2A
    OPT_BEST2600W_LITEOS_BOOT2A="${temp_cmd}"
else
    pre_handle_opt ARM_CMNS=0 OPT_BEST2600W_LITEOS_MINI
    OPT_BEST2600W_LITEOS_MINI="${temp_cmd}"
    pre_handle_opt ARM_CMNS=0 OPT_BEST2600W_LITEOS_MAIN
    OPT_BEST2600W_LITEOS_MAIN="${temp_cmd}"
    pre_handle_opt TZ_MAIN_OFFSET=0x80000 OPT_BEST2600W_LITEOS_BOOT2A
    OPT_BEST2600W_LITEOS_BOOT2A="${temp_cmd}"
    pre_handle_opt TZ_MINI_OFFSET=0xB00000 OPT_BEST2600W_LITEOS_BOOT2A
    OPT_BEST2600W_LITEOS_BOOT2A="${temp_cmd}"
fi


tools/build_best2600w_ohos_into_lib.sh \
-a="$OPT_BEST2600W_LITEOS_A7 $build_type" \
-m="$OPT_BEST2600W_LITEOS_MAIN MODULE_KERNEL_STUB_INC=1 EXTERN_ROOT_PATH=./../../../../../../../ $build_type" \
-c="$OPT_BEST2600W_LITEOS_CP $build_type" \
-s="$OPT_BEST2600W_LITEOS_MAIN_MINI_SE $build_type" \
-l="$OPT_BEST2600W_LITEOS_MINI MODULE_KERNEL_STUB_INC=1 EXTERN_ROOT_PATH=./../../../../../../../ $build_type" \
-x="$OPT_BEST2600W_LITEOS_BOOT1 $build_type" \
-y="$OPT_BEST2600W_LITEOS_BOOT2A $build_type" \
-d=" BUILD_SE=$build_trustzone BUILD_MINI=$build_mini_sys $rel_branch" \
GEN_LIB=1 $rel

cd ../../

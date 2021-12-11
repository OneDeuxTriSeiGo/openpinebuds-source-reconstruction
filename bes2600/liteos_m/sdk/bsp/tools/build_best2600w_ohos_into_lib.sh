#!/bin/bash
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

####### each project need to config following toolchain path #########
TOOLCHAIN_9_2_1="/home/toolchain/9.2.1/gcc-arm-none-eabi-9-2019-q4-major/bin"

####### each project need to config project #########

# Set variables according to real environment
# on ubuntu
# same time support how many guys build
let DEFAULT_BUILD_JOBS=8
let GUYS_BUILD_NUM=5
dev_env=`uname | sed 'y/LINUX/linux/'`
if [ "x$dev_env" == "xlinux" ]; then
    let JOBS=`grep ^processor /proc/cpuinfo|wc -l`
else
    error "Unsupported Build System : $dev_env"
fi
echo -e "machine support JOBS: ${JOBS}"
let JOBS=(JOBS+GUYS_BUILD_NUM-1)/GUYS_BUILD_NUM
if [ $DEFAULT_BUILD_JOBS -gt $JOBS ] ;then
   let JOBS=$DEFAULT_BUILD_JOBS
fi
echo -e "build in machine use JOBS: ${JOBS}"
# build cmd define
# A7_DSP_BIN_NAME=best2300wp_a7
A7_DSP_BIN_NAME=a7_dsp

special_buid_src2lib_dir=( \
"rtos/liteos/liteos_m/" \
"services/cp_boot/" \
)

special_buid_obj_dir=( \
"platform/hal" \
)

buid_m33_arry=("make_best2600w" "make_best2600w_mini")

BUILD_SE="false"
BUILD_MINI="false"
make_boot1="make T=ota_boot1 \
            REMAP_SUPPORT=0 \
            BOOT2A_OFFSET=0x10000 \
            BOOT2B_OFFSET=0x20000 \
            BOOTINFO_OFFSET=0xFD0000 \
            all lst -j${JOBS}"

make_boot2a="make T=ota_boot2a \
            REMAP_SUPPORT=0 \
            all lst -j${JOBS}"

make_boot2b="make T=ota_boot2b \
            TZ_MAIN_OFFSET=0x30000 \
            TZ_MINI_OFFSET=0x56000 \
            BOOTINFO_OFFSET=0xFD0000 \
            all lst -j${JOBS}"

BUILDOPT_M33="CHIP=best2003 A7_DSP_ENABLE=1 DSP_AUDIO_TASK=1 DSP_CODEC_SUPPORT=0\
              PSRAMUHS_ENABLE=1 PSRAM_ENABLE=1 SPEECH_MSG_ENABLE=1 \
              SPEECH_MSG_TEST_CASE=0 A7_PROJECT=dummy"

BUILDOPT_CP="PSRAMCP_SIZE=0x100000 DUAL_BAND=1 AP_MODE_SUPPORT=1"

make_cmcp="make T=cmcp DEBUG_PORT=4 \
           LIB_BIN_IN_SRC_DIR=1 \
           GEN_SDK_LIB_NET=1 \
           ${BUILDOPT_CP} -j${JOBS}"

make_a7="make T=$A7_DSP_BIN_NAME \
         LIB_BIN_IN_SRC_DIR=1 A7_UART=4 \
         $BUILDOPT_M33 \
         KERNEL=LITEOS_A A7_CMSIS_TEST_CASE=0 \
         OPUS_CODEC_SUPPORT=1 -j${JOBS}"

make_best2600w="make T=best2600w_liteos \
                ARM_CMNS=0 \
                LIB_BIN_IN_SRC_DIR=1 \
                MODULE_KERNEL_STUB=1 \
                PSRAM_ENABLE=1 \
                NET_JANSSON_SUPPORT=0 \
                NET_CJSON_SUPPORT=0 \
                RF_TX_CONTROL_IO=12 \
                PA_ENABLE_IO=21 \
                UART1_IOMUX_INDEX=20 \
                NET_FTP_CLIENT_SUPPORT=0 UTILS_BES_KV=1 \
                GEN_SDK_LIB_NET=0 OTA_CODE_OFFSET=0x80000 \
                CP_BOOT=1 \
                FAST_TIMER_COMPENSATE=1 WAPI_SUPPORT=0 \
                USB_AUDIO_FS=0  \
                AI_VOICE=0 LLD_PRINT_ENABLE=0 \
                NET_LWIP_EXTERN=1 NET_LWIP_SUPPORT=0 SYS_TIME_IF=0 \
                ${BUILDOPT_CP} $BUILDOPT_M33 DSP_BIN_NAME=$A7_DSP_BIN_NAME -j${JOBS} "


make_best2600w_mini_se="make T=best2600w_liteos_mini_se all lst -j${JOBS} HUKS_SERVICE=1 LIB_BIN_IN_SRC_DIR=1"

make_best2600w_se="make T=best2600w_liteos_se all lst -j${JOBS} HUKS_SERVICE=1 LIB_BIN_IN_SRC_DIR=1"

make_best2600w_mini="make T=best2600w_liteos_mini \
                ARM_CMNS=0 \
                LIB_BIN_IN_SRC_DIR=1 \
                MODULE_KERNEL_STUB=1 \
                PSRAM_ENABLE=1 \
                NET_LIBCURL_SUPPORT=1 \
                NET_MBEDTLS_SUPPORT=1 \
                NET_JANSSON_SUPPORT=0 \
                NET_CJSON_SUPPORT=0 \
                RF_TX_CONTROL_IO=12 \
                PA_ENABLE_IO=21 \
                UART1_IOMUX_INDEX=20 \
                USE_EXT_CONSOLE=1 REL_AT_C=0 AT_CMD=0  LITTLEFS_ENABLE=0 \
                NET_FTP_CLIENT_SUPPORT=0 UTILS_BES_KV=1 \
                GEN_SDK_LIB_NET=0 OTA_CODE_OFFSET=0xb00000 \
                CP_BOOT=1 \
                MBEDTLS_FS_IO=1 \
                $BUILDOPT_CP -j${JOBS}"

################################ functions list begin #########################
BUILD_INFO_LIST=()
BUILD_INDEX=0;
GLOBAL_OPTS=""
RELEASE_SDK=0
rel_branch=""
SPACE=" "
EQUEL="="

error () {
    set +u
    echo "$0: error: $@" >&2
    exit 1
    set -u
}

build_cmd_echo="\n"
tmp_path=$PATH
function build_result()
{
   eval cmd="\${$1}"
   cmd=`echo $cmd`

   toolchain_path=`which arm-none-eabi-gcc`
   toolchain_info="\n=== build with toolchain [$toolchain_path] ===\n"
   PATH=$tmp_path
   if [ "x$2" != "x1" ];then
      echo -e "$toolchain_info"
      echo -e "\033[41m[build err]\033[0m ${cmd}"
      if [ "x${BUILD_PIECE}" != "xtrue"  ] ;then
         exit 1
      fi
   fi
   build_cmd_echo+=$toolchain_info
   build_cmd_echo+="${cmd}\n\n"
}

#pre-handle option of every target,for now GEN_LIB and SDK
function pre_handle_global_opt()
{
   #printf "pre_handle_global_opt $1 \n"
   GLOBAL_OPTS="$SPACE$1"
}

#pre-handle options of target
function pre_handle_opt()
{
   eval cmd="\${$1}"
   cmd=`echo $cmd`
   # printf "\n[$1] $cmd\n"

   for input_opt in $*
   do
      #strip first para
      if [ "x${1}" == "x${input_opt}" ] ;then
         continue
      fi
      # parse key value by =
      input_opt_key=${input_opt%%=*}

      temp_cmd=""
      flag_match=0
      for cmd_opt in $cmd
      do
         # parse key value by =
         cmd_opt_key=${cmd_opt%%=*}

         # cover same parameter by check key
         if [ "x${input_opt_key}" == "x${cmd_opt_key}" ] ;then
            printf "[$1][opt] ${cmd_opt} covered by ${input_opt}\n"
            temp_cmd+="$input_opt$SPACE"
            flag_match=1
            continue
         fi

         # keep current parameter not matched
         temp_cmd+="$cmd_opt$SPACE"
      done

      # handle no matched parameters
      if [ "x${flag_match}" == "x0" ] ;then
         printf "[$1][opt] target add ${input_opt}\n"
         temp_cmd+="$input_opt$SPACE"
      fi
      # printf "[temp_cmd] ${temp_cmd} \n"
      cmd=$temp_cmd
   done
   # printf "[cmd] ${cmd} \n"
   BUILD_INFO_LIST[BUILD_INDEX]="$1,${cmd}"
   printf "[GEN] ${BUILD_INFO_LIST[BUILD_INDEX]}\n"
   let BUILD_INDEX=BUILD_INDEX+1
}

function pre_handle_build_enable()
{
   for input_opt in $*
   do
      OLD_IFS="$IFS"
      IFS="="
      arry=(${input_opt})
      IFS="$OLD_IFS"
      if [ "x${arry[0]}" == "xBUILD_SE" ] ;then
         BUILD_SE=${arry[1]}
      fi
      if [ "x${arry[0]}" == "xBUILD_MINI" ] ;then
         BUILD_MINI=${arry[1]}
      fi
      if [ "x${arry[0]}" == "xbranch" ] ;then
         rel_branch="_${arry[1]}"
      fi
      if [ "x${arry[0]}" == "xBUILD_PIECE" ] ;then
         BUILD_PIECE="${arry[1]}"
      fi
   done
}

function pre_handle_relase()
{
   RELEASE_SDK=1
}

function build_cmd()
{
   eval cmd="\${$1}"
   build_target=`echo $cmd`

   build_format=" "
   if [ "x${BUILD_PIECE}" != "xtrue"  ] ;then
      for ((index=0; index<${#buid_m33_arry[@]}; index++))
      do
         if [ "x$1" == "x${buid_m33_arry[index]}" ] ;then
            build_format=" lib "
         fi
      done
   fi
   for ((index=0; index<$BUILD_INDEX; index++))
   do
      kv_cmd=${BUILD_INFO_LIST[index]}
      OLD_IFS="$IFS"
      IFS=","
      arr_cmd=(${kv_cmd})
      IFS="$OLD_IFS"
      if [ "x${arr_cmd[0]}" == "x$1" ] ;then
         build_target="${arr_cmd[1]}"
         break;
      fi
   done
   # printf "\n[BUILD][$1] $build_target \n"
   if [ "x${BUILD_INDEX}" == "x0"  ] ;then
      build_target="$build_target"
   fi

   build_target="$build_target$GLOBAL_OPTS${build_format}"
   BUILD_INFO_LIST[index]="$1,${build_target}"
   ### config toolchain ###
   if [ "x$2" == "x" ] ;then
      if [ -d $M33_TOOLCHAIN ] ;then
         export PATH=$M33_TOOLCHAIN:$PATH
      fi
   else
      eval toolchain="\${$2}"
      # printf "\ntoolchain : $toolchain \n"
      if [ -d $toolchain ]; then
         export PATH=$toolchain:$PATH
      fi
   fi
   rm -rf out/log/bsp_build_$1.log
   mkdir -p out/log
   ${build_target} 2>&1 | tee out/log/bsp_build_$1.log
   cat out/log/bsp_build_$1.log | awk 'END {print}' | grep "Error" > /dev/null
   rc=$?
   # echo "rc = $rc"
   echo "${build_target}" >> out/log/bsp_build_$1.log
   build_result build_target $rc
}

function print_all_cmds()
{
   echo -e "\n~~~ build success! show all build command list ~~~\n"
   echo -e "${build_cmd_echo}" 2>&1 | tee out/log/bsp_build_cmd.log
   echo -e "~~~ build success! show all build command over  ~~~\n"
}

function realse_sdk()
{
   if [ "x$RELEASE_SDK" == "x1" ]; then
      relase_cmd="tools/relsw_$1 $1 WORKING"
      echo -e "[realse_sdk] $relase_cmd\n"
      $relase_cmd
   fi
}

usage ()
{
cat<<EOF
Usage:

[       -c,     -a,     -m,    -s,       -l,        -x,         -y,     -h,        -r ]
[  --cmcp=,  --a7=, --m33=, --se=, --lmini=, --xboot1=, --yboot2a=, --help, --release ]

EOF
exit 1
}
################################ functions list end   #########################


# total_param=$@
GETOPT_ARGS=`getopt -o c:a:m:s:l:x:y:z:d:hr -al cmcp:,a7:,m33:,se:,lmini:,xboot1:,yboot2a:,zboot2b:,defineEx:,help,release -- "$@"`

eval set -- "$GETOPT_ARGS"

while [ -n "$1" ]
do
   #  printf "1:$1 2:$2 total_param:$total_param\n"
    case "$1" in
        -c|--cmcp)      pre_handle_opt make_cmcp $2;              shift 2;;
        -a|--a7)        pre_handle_opt make_a7  $2;               shift 2;;
        -m|--m33)       pre_handle_opt make_best2600w $2;         shift 2;;
        -s|--se)        pre_handle_opt make_best2600w_se $2;
                        pre_handle_opt make_best2600w_mini_se $2;
                        shift 2;;
        -l|--lmini)     pre_handle_opt make_best2600w_mini $2;    shift 2;;
        -x|--xboot1)    pre_handle_opt make_boot1 $2;             shift 2;;
        -y|--yboot2a)   pre_handle_opt make_boot2a $2;            shift 2;;
        -d|--defineEx)  pre_handle_build_enable $2;               shift 2;;
        -h|--help)      usage;                                    shift 1;;
        -r|--realse)    pre_handle_relase;                        shift 1;;
        --)             pre_handle_global_opt $2;break;                  ;;
        *)  break ;;
    esac
done


if [ "x${BUILD_PIECE}" != "xtrue"  ] ;then

   # for fit build increate
   BUILD_SDK=0
   for arg in $*
   do
      if [ $arg == "SDK=1" ];then
         BUILD_SDK=1
         echo "find SDK = 1"
      fi
   done
   if [ $arg == "SDK=1" ];then
      echo "~~~~~ not clear bsp, decide in app script ~~~~~"
   else
      echo "~~~~~ clear bsp all now ~~~~~~"
      rm -rf out/cmcp
      rm -rf out/${A7_DSP_BIN_NAME}
      rm -rf out/best2600w_liteos
   fi
fi

# clear out/
rm -rf out/log
if [ "x${BUILD_PIECE}" == "xtrue"  ] ;then
   for ((index=0; index<${#special_buid_src2lib_dir[@]}; index++))
   do
      pack_lib_dir=${special_buid_src2lib_dir[index]}
      if [ -d out/best2600w_liteos/${pack_lib_dir} ] ;then
         echo "del out/best2600w_liteos/$pack_lib_dir"
         rm -rf out/best2600w_liteos/$pack_lib_dir
      fi
      rm -rf out/best2600w_liteos/bsp.mri
      rm -rf out/best2600w_liteos/libbspbest2600w_liteos.a
   done
fi

if [ "x${BUILD_PIECE}" != "xtrue"  ] ;then
   rm -rf out/ota_boot1
   rm -rf out/ota_boot2a
   rm -rf out/cmcp
   rm -rf out/${A7_DSP_BIN_NAME}
   rm -rf out/best2600w_liteos
   rm -rf out/best2600w_liteos_se
   rm -rf out/best2600w_liteos_mini
   rm -rf out/best2600w_liteos_mini_se
   rm -rf out/prebuild

   mkdir -p out/prebuild

# make ota boot
   build_cmd make_boot1
   build_cmd make_boot2a

# make_a7
   build_cmd make_a7 TOOLCHAIN_9_2_1

# compress dsp bin
   tools/lzma e out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin.lzma
   mv out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin.lzma out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin

   if [ "x$BUILD_SDK" == "x1" ]; then
      # use prebuild cp bin
      echo "=============mkdir out for cmcp==============="
      mkdir -p out/cmcp/
      cp prebuild/cmcp.bin out/cmcp/cmcp.bin
      ls out/cmcp/
      echo "=============mkdir out for cmcp==============="
   else
#build cp
      build_cmd make_cmcp
# compress cp bin
      tools/lzma e out/cmcp/cmcp.bin out/cmcp/cmcp.bin.lzma

      if [ "x$RELEASE_SDK" == "x1" ]; then
 
         rm -rf out/cmcp_16/
         mkdir -p out/cmcp_16/
         mv out/cmcp/cmcp.bin.lzma out/cmcp_16/cmcp_16.bin

         make_cmcp_32="${make_cmcp} PSRAM_XCCELA_MODE=1"
         build_cmd make_cmcp_32

         tools/lzma e out/cmcp/cmcp.bin out/cmcp/cmcp.bin.lzma
         rm -rf out/cmcp_32/
         mkdir -p out/cmcp_32/
         mv out/cmcp/cmcp.bin.lzma out/cmcp_32/cmcp_32.bin
      else
         mv out/cmcp/cmcp.bin.lzma out/cmcp/cmcp.bin
      fi
   fi

# compress prebuild bins
# bt ramrun
   cp platform/drivers/bt/best2003/best2003_ramrun_fw.bin out/prebuild/.
   tools/lzma e out/prebuild/best2003_ramrun_fw.bin out/prebuild/best2003_ramrun_fw.lzma
   mv out/prebuild/best2003_ramrun_fw.lzma out/prebuild/best2003_ramrun_fw.bin


# make_best2600w TrustZone App
   if [ "x$BUILD_SE" == "xtrue" ]; then
      build_cmd make_best2600w_se
   fi

# make_best2600w TrustZone App
   if [[ "x$BUILD_SE" == "xtrue" ]] && [[ "x$BUILD_MINI" == "xtrue" ]]; then
      build_cmd make_best2600w_mini_se
   fi
fi
# make_best2600w_mini
if [ "x$BUILD_MINI" == "xtrue" ]; then
   pre_build_into_lib_cmd make_best2600w_mini
   build_cmd make_best2600w_mini
fi

# make_best2600w
build_cmd make_best2600w

cd out/best2600w_liteos/
if [ "x${BUILD_PIECE}" == "xtrue"  ] ;then
   mri_path="bsp.mri"
   touch $mri_path
   echo "create libbspbest2600w_liteos.a" >> $mri_path
   echo "addlib libbest2600w_liteos.a" >> $mri_path
   for ((index=0; index<${#special_buid_src2lib_dir[@]}; index++))
   do
      pack_lib_dir=${special_buid_src2lib_dir[index]}
      if [ -f $pack_lib_dir/built-in.a ]; then
         echo "addlib $pack_lib_dir/built-in.a" >> $mri_path
      else
         echo "WARNING $pack_lib_dir/built-in.a not generate!"
      fi
   done
   for ((index=0; index<${#special_buid_obj_dir[@]}; index++))
   do
      pack_obj_dir=${special_buid_obj_dir[index]}
      find_obj=`find $pack_obj_dir -name "*.o"`
      for obj in $find_obj
      do
         echo "addmod $obj"
         echo "addmod $obj" >> $mri_path
      done
   done

   echo "save" >> $mri_path
   echo "end" >> $mri_path

   ar -M <bsp.mri
   echo " "
   echo "gen out/best2600w_liteos/libbspbest2600w_liteos.a success"
else
   cp -rf libbest2600w_liteos.a libbspbest2600w_liteos.a
fi
cd ../../
# release script
realse_sdk best2600w_ohos_sig${rel_branch}

# print all make cmds
print_all_cmds

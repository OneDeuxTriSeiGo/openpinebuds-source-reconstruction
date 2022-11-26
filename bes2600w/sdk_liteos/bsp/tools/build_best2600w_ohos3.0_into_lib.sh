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
TOOLCHAIN_10_3_1="/home/toolchain/gcc-arm-none-eabi-10.3-2021.10/bin"
TOOLCHAIN_5_4_1="/home/toolchain/ali_5.4.1/gcc-arm-none-eabi/Linux64/bin"

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


buid_m33_arry=("make_best2600w" "make_best2600w_32" "make_best2600w_mini")

BUILD_SE="false"
BUILD_MINI="false"
make_boot1="make T=ota_boot1 \
            REMAP_SUPPORT=1 \
            BOOT2A_OFFSET=0x10000 \
            BOOT2B_OFFSET=0x20000 \
            BOOTINFO_OFFSET=0x1FC0000 \
            all lst -j${JOBS}"

make_boot2a="make T=ota_boot2a \
            REMAP_SUPPORT=1 \
            BOOTINFO_OFFSET=0x1FC0000
            all lst -j${JOBS}"

make_boot2b="make T=ota_boot2b \
            TZ_MAIN_OFFSET=0x30000 \
            TZ_MINI_OFFSET=0x56000 \
            BOOTINFO_OFFSET=0x1FC0000 \
            all lst -j${JOBS}"

BUILDOPT_M33="CHIP=best2003 A7_DSP_ENABLE=1 DSP_AUDIO_TASK=1 DSP_CODEC_SUPPORT=0\
              PSRAMUHS_ENABLE=1 PSRAM_ENABLE=1 SPEECH_MSG_ENABLE=1 \
              USE_PERIPH_PLL_FREQ=1 SPILCD_DMA_ENABLE=1 SPILCD_IOMUX_INDEX=34 \
              SPEECH_MSG_TEST_CASE=0 A7_PROJECT=dummy I2C_PARAMETER_CONFIG_BY_USER=1"

BUILDOPT_CP="PSRAMCP_SIZE=0x100000 DUAL_BAND=1 AP_MODE_SUPPORT=1"

make_cmcp="make T=cmcp DEBUG_PORT=4 \
           LIB_BIN_IN_SRC_DIR=1 \
           GEN_SDK_LIB_NET=1 \
           RF_TX_CONTROL_IO=100 \
           UART1_IOMUX_INDEX=02 \
           WIFI_BT_COEX_FDD=0 \
           WIFI_BT_COEX_HYBRID=0 \
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
                RF_TX_CONTROL_IO=100 \
                PA_ENABLE_IO=21 \
                UART1_IOMUX_INDEX=02 \
                WIFI_BT_COEX_FDD=0 \
                WIFI_BT_COEX_HYBRID=0 \
                NET_FTP_CLIENT_SUPPORT=0 UTILS_BES_KV=1 \
                GEN_SDK_LIB_NET=0 OTA_CODE_OFFSET=0x80000 \
                CP_BOOT=1 LITTLEFS_PARTITION_SUPPORT=0 LITTLEFS_ENABLE=0 \
                FAST_TIMER_COMPENSATE=1 WAPI_SUPPORT=0 \
                USB_AUDIO_FS=0 DEBUG_SPP_LOG=1 \
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
                NET_JANSSON_SUPPORT=0 \
                NET_CJSON_SUPPORT=0 \
                RF_TX_CONTROL_IO=100 \
                PA_ENABLE_IO=21 \
                UART1_IOMUX_INDEX=02 \
                WIFI_BT_COEX_FDD=0 \
                WIFI_BT_COEX_HYBRID=0 \
                NET_FTP_CLIENT_SUPPORT=0 UTILS_BES_KV=1 \
                GEN_SDK_LIB_NET=0 OTA_CODE_OFFSET=0xC80000 \
                CP_BOOT=1 LITTLEFS_PARTITION_SUPPORT=0 LITTLEFS_ENABLE=0 \
                FAST_TIMER_COMPENSATE=1 WAPI_SUPPORT=0 \
                USB_AUDIO_FS=0 DEBUG_SPP_LOG=1 \
                AI_VOICE=0 LLD_PRINT_ENABLE=0 \
                NET_LWIP_EXTERN=1 NET_LWIP_SUPPORT=0 SYS_TIME_IF=0 \
                ${BUILDOPT_CP} $BUILDOPT_M33 DSP_BIN_NAME=$A7_DSP_BIN_NAME -j${JOBS} "

################################ functions list begin #########################
BUILD_INFO_LIST=()
BUILD_INDEX=0;
GLOBAL_OPTS=""
RELEASE_SDK=0

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
      exit 1
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
   for ((index=0; index<${#buid_m33_arry[@]}; index++))
   do
      if [ "x$1" == "x${buid_m33_arry[index]}" ] ;then
          build_format=" lib "
      fi
   done

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

function pre_build_into_lib_cmd()
{
   eval cmd="\${$1}"
   # printf "$build_target \n"

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

   build_target=`echo $cmd`
   for ((index=0; index<$BUILD_INDEX; index++))
   do
      kv_cmd=${BUILD_INFO_LIST[index]}
      OLD_IFS="$IFS"
      IFS=","
      arr_cmd=(${kv_cmd})
      IFS="$OLD_IFS"
      if [ "x${arr_cmd[0]}" == "x$1" ] ;then
         build_target=${arr_cmd[1]}
         break;
      fi
   done

   for ((index=0; index<${#special_buid_src2lib_dir[@]}; index++))
   do
      pack_lib_dir=${special_buid_src2lib_dir[index]}
      printf "\n[pack dir] ${pack_lib_dir}"
      build_pre="$build_target $pack_lib_dir GEN_LIB=1"
      printf "\n[BUILD][$1] $build_pre \n\n"
      ${build_pre} 2>&1 | tee out/log/bsp_build_$1_${index}.log
   done
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
   done
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
GETOPT_ARGS=`getopt -o c:a:m:s:l:x:y:z:d:hr -al cmcp:,a7:,m33:,se:,lmini:,xboot1:,yboot2a:,zboot2b:,disable:,help,release -- "$@"`

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
        -z|--zboot2b)   pre_handle_opt make_boot2b $2;            shift 2;;
        -d|--disable)   pre_handle_build_enable $2;               shift 2;;
        -h|--help)      usage;                                    shift 1;;
        -r|--realse)    pre_handle_relase;                        shift 1;;
        --)             pre_handle_global_opt $2;break;                  ;;
        *)  break ;;
    esac
done


# for fit build increate
BUILD_SDK=0
BUILD_FLASH_SIZE=16
for arg in $*
do
   if [ $arg == "SDK=1" ];then
      BUILD_SDK=1
      echo "find SDK = 1"
   fi
   if [ $arg == "flash_size=32" ];then
      echo "************************** FLASH_SIZE=0x2000000 ************************"
      BUILD_FLASH_SIZE=32
   fi
done
if [ $arg == "SDK=1" ];then
   echo "~~~~~ not clear bsp, decide in app script ~~~~~"
else
   echo "~~~~~ clear bsp all now ~~~~~~"
   rm -rf out/cmcp
   rm -rf out/${A7_DSP_BIN_NAME}
   rm -rf out/best2600w_liteos
   rm -rf out/ota_boot1
   rm -rf out/ota_boot2a
fi


# clear out/
rm -rf out/log
if [ "x$BUILD_SDK" != "x1" ]; then
   rm -rf out/ota_boot1;
   rm -rf out/ota_boot2a;
fi
rm -rf out/cmcp
rm -rf out/${A7_DSP_BIN_NAME}
rm -rf out/best2600w_liteos
rm -rf out/best2600w_liteos_se
rm -rf out/best2600w_liteos_mini
rm -rf out/best2600w_liteos_mini_se
rm -rf out/prebuild
rm -rf out/ota_boot1_$BUILD_FLASH_SIZE
rm -rf out/ota_boot2a_$BUILD_FLASH_SIZE
rm -rf out/best2600w_liteos_$BUILD_FLASH_SIZE
rm -rf out/best2600w_liteos_mini_$BUILD_FLASH_SIZE
rm -rf out/cmcp_$BUILD_FLASH_SIZE

mkdir -p out/prebuild
mkdir -p out/ota_boot1_$BUILD_FLASH_SIZE
mkdir -p out/ota_boot2a_$BUILD_FLASH_SIZE
mkdir -p out/best2600w_liteos_$BUILD_FLASH_SIZE
mkdir -p out/best2600w_liteos_mini_$BUILD_FLASH_SIZE
mkdir -p out/cmcp_$BUILD_FLASH_SIZE

# make ota boot
if [ "x$BUILD_SDK" != "x1" ]; then
   build_cmd make_boot1;
   build_cmd make_boot2a;
   cp -rf out/ota_boot1/ota_boot1.bin out/ota_boot1_$BUILD_FLASH_SIZE/
   cp -rf out/ota_boot2a/ota_boot2a.bin out/ota_boot2a_$BUILD_FLASH_SIZE/
fi

# make_a7
build_cmd make_a7 TOOLCHAIN_10_3_1

# compress dsp bin
tools/lzma e out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin.lzma
mv out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin.lzma out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin


# compress prebuild bins
# bt ramrun
cp platform/drivers/bt/best2003/best2003_ramrun_fw.bin out/prebuild/.
tools/lzma e out/prebuild/best2003_ramrun_fw.bin out/prebuild/best2003_ramrun_fw.lzma
mv out/prebuild/best2003_ramrun_fw.lzma out/prebuild/best2003_ramrun_fw.bin


if [ "x$RELEASE_SDK" == "x1" ]; then

mkdir -p out/ota_boot1_16/
cp -rf out/ota_boot1/ota_boot1.bin out/ota_boot1_16/

#build boot1 32M
make_ota_boot1_32="${make_boot1}  PSRAM_XCCELA_MODE=1 FLASH_SIZE=0x2000000"
build_cmd make_ota_boot1_32
mkdir -p out/ota_boot1_32/
cp -rf out/ota_boot1/ota_boot1.bin out/ota_boot1_32/

mkdir -p out/ota_boot2a_16/
cp -rf out/ota_boot2a/ota_boot2a.bin out/ota_boot2a_16/

#build boot2a 32M
make_ota_boot2a_32="${make_boot2a}  PSRAM_XCCELA_MODE=1 FLASH_SIZE=0x2000000"
build_cmd make_ota_boot2a_32
mkdir -p out/ota_boot2a_32/
cp -rf out/ota_boot2a/ota_boot2a.bin out/ota_boot2a_32/

#build cp 16M
build_cmd make_cmcp
# compress cp bin
tools/lzma e out/cmcp/cmcp.bin out/cmcp/cmcp.bin.lzma
mv out/cmcp/cmcp.bin.lzma out/cmcp/cmcp.bin
mkdir -p out/cmcp_16/
cp -rf out/cmcp/cmcp.bin out/cmcp_16/
cp -rf out/cmcp/_cmcp.lds out/cmcp_16/

build_cmd make_best2600w
mkdir -p out/best2600w_liteos_16/
cp -rf out/best2600w_liteos/libbest2600w_liteos.a out/best2600w_liteos_16/
cp -rf out/best2600w_liteos/_best2001.lds out/best2600w_liteos_16/

rm -rf out/cmcp
rm -rf out/best2600w_liteos

#build cp 32M
make_cmcp_32="${make_cmcp}  PSRAM_XCCELA_MODE=1 FLASH_SIZE=0x2000000"
build_cmd make_cmcp_32
# compress cp bin
tools/lzma e out/cmcp/cmcp.bin out/cmcp/cmcp.bin.lzma
mv out/cmcp/cmcp.bin.lzma out/cmcp/cmcp.bin
mkdir -p out/cmcp_32/
cp -rf out/cmcp/cmcp.bin out/cmcp_32/
cp -rf out/cmcp/_cmcp.lds out/cmcp_32/

make_best2600w_32="${make_best2600w}  PSRAM_XCCELA_MODE=1 FLASH_SIZE=0x2000000"
build_cmd make_best2600w_32
mkdir -p out/best2600w_liteos_32/
cp -rf out/best2600w_liteos/libbest2600w_liteos.a out/best2600w_liteos_32/
cp -rf out/best2600w_liteos/_best2001.lds out/best2600w_liteos_32/

else


#build cp 16M
build_cmd make_cmcp
# compress cp bin
tools/lzma e out/cmcp/cmcp.bin out/cmcp/cmcp.bin.lzma
mv out/cmcp/cmcp.bin.lzma out/cmcp/cmcp.bin
# make_best2600w
build_cmd make_best2600w
# make_best2600w_mini
build_cmd make_best2600w_mini

cp -rf out/cmcp/cmcp.bin out/cmcp_$BUILD_FLASH_SIZE/
cp -rf out/best2600w_liteos/libbest2600w_liteos.a out/best2600w_liteos_$BUILD_FLASH_SIZE/
cp -rf out/best2600w_liteos/_best2001.lds out/best2600w_liteos_$BUILD_FLASH_SIZE/
cp -rf out/best2600w_liteos_mini/libbest2600w_liteos.a out/best2600w_liteos_mini_$BUILD_FLASH_SIZE/
cp -rf out/best2600w_liteos_mini/_best2001.lds out/best2600w_liteos_mini_$BUILD_FLASH_SIZE/

fi

# release script
realse_sdk best2600w_ohos_3.0

# print all make cmds
print_all_cmds

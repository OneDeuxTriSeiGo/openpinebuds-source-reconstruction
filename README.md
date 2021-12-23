# device_bestechnic

#### 介绍

##### 目录

```
device/soc/
├── bes2600                               # 芯片SOC名称
├── bes2x00                               # 芯片SOC名称
├── ...                                   # 芯片SOC名称
|
├── hals                                  # hals适配目录
│   └── communication                     # 连接类接口适配目录
│       └── wifi_lite                     # 轻量级wifi适配目录
├── Kconfig.liteos_m.defconfig            # kconfig 默认宏配置
├── Kconfig.liteos_m.series               # bes系列soc配置宏
└── Kconfig.liteos_m.soc                  # soc kconfig配置宏
```

整个编译框架如下：

```
#                                    +-----------------+
#                                    | topographic map |
#                                    +-----------------+
# topographic map descripe how to generate write_flash_gui.tar.gz, according to depends rules.
#
#                               +-----------------------------+
#                       +-----> |    copy_${exe_bin}_to_gui   |
#                       |       +-----------------------------+
#                       |       +-----------------------------+
#                       +-----> |   copy_${fs_name}_to_gui    |
# +---------------+     |       +-----------------------------+
# | pack_flash_gui|---> +
# +---------------+     |       +-----------------------------+
#                       +-----> | copy_${bsp_bin_name}_to_gui |
#                       |       +-----------------------------+
#                       |       +-----------------------------+     +-----------------+
#                       +-----> | update_${product_name}_yaml | --> | init_burn_tools |
#                               +-----------------------------+     +-----------------+
#                                                                                             +--------------------+     +---------+
#                                                                                             | gen_bin_${exe_bin} | --> | exe_bin |
# excutable bin topographic map:                                                              +--------------------+     +---------+
#                             +--------+                  +-------------------------------+   /                                |
#                             | no sig | ---------------> | copy_${exe_bin}_to_relase_bin | -+                                \|/
#                             +--------+                  +-------------------------------+   \                                V
# +------------------------+   /                                ^                              +-----------------+  +------------------------+
# | copy_${exe_bin}_to_gui | -+                                /|\                             | init_burn_tools |  | gen_bsp_lib_${exe_bin} |
# +------------------------+   \                                |                              +-----------------+  +------------------------+
#                             +--------+     +--------------------------+                                                       |
#                             |  sig   | --> | gen_bin_${burn_name}_sig |                                                      \|/
#                             +--------+     +--------------------------+                                                       V
#                                                                                                                    +--------------------+
#                                                                                                                    | build_sdk_into_lib |
#                                                                                                                    +--------------------+
#
# fs bin topographic map:
#                             +--------+         +------------------+   +-----------------+
#                             | no sig | ------> | genfs_${fs_name} |-->| init_burn_tools |
#                             +--------+         +------------------+   +-----------------+
# +------------------------+   /                         ^
# | copy_${fs_name}_to_gui | -+                         /|\
# +------------------------+   \                         |
#                             +--------+     +----------------------+
#                             |  sig   | --> | genfs_${fs_name}_sig |
#                             +--------+     +----------------------+
#
# bsp bin topographic map:
#                                                                                               +--------------------+
#                                                                                               | build_sdk_into_lib |
#                                                                                               +--------------------+
#                                  +--------+         +------------------------------------+   /
#                                  | no sig | ------> | copy_${bsp_bin_name}_to_relase_bin | -+
#                                  +--------+         +------------------------------------+   \
# +------------------------------+   /                         ^                                +-----------------+
# | copy_${bsp_bin_name}_to_gui  | -+                         /|\                               | init_burn_tools |
# +------------------------------+   \                         |                                +-----------------+
#                                  +--------+     +------------------------------+
#                                  |  sig   | --> | gen_bsp_bin_${burn_name}_sig |
#                                  +--------+     +------------------------------+
```

以下内容步骤参考[quickstart-lite-env-setup-linux](https://gitee.com/openharmony/docs/blob/master/zh-cn/device-dev/quick-start/quickstart-lite-env-setup-linux.md)。

仓库包含编译构建脚本和打包镜像工具。

系统要求： Ubuntu18.04 64位系统版本。

编译环境搭建包含如下几步：

1. 获取源码
2. 安装的库和工具
3. 安装python3
4. 安装hb
5. 安装arm-none-eabi-gcc
6. 编译流程
7. 烧录打印

## 获取源码

```shell
mkdir openharmony_bestechnic

cd openharmony_bestechnic

repo init -u https://gitee.com/openharmony/manifest --no-repo-verify

repo sync -c

repo forall -c 'git lfs pull'
```

## 安装的库和工具

> - 通常系统默认安装samba、vim等常用软件。

> - 使用如下apt-get命令安装下面的库和工具：

```
sudo apt-get install build-essential gcc g++ make zlib* libffi-dev e2fsprogs pkg-config flex bison perl bc openssl libssl-dev libelf-dev libc6-dev-amd64 binutils binutils-dev libdwarf-dev u-boot-tools mtd-utils gcc-arm-linux-gnueabi
```

## 安装Python3

1. 打开Linux编译服务器终端。
2. 输入如下命令，查看python版本号：

   ```
   python3 --version
   ```
   1. 运行如下命令，查看Ubuntu版本：

   ```
   cat /etc/issue
   ```

   2. ubuntu 18安装python。
   ```
   sudo apt-get install python3.8
   ```

3. 设置python和python3软链接为python3.8。

   ```
   sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.8 1
   sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.8 1
   ```
4. 安装并升级Python包管理工具（pip3），任选如下一种方式。

   - **命令行方式：**

     ```
     sudo apt-get install python3-setuptools python3-pip -y
     sudo pip3 install --upgrade pip
     ```
   - **安装包方式：**

     ```
     curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
     python get-pip.py
     ```

## 安装hb
### 安装方法

1. 运行如下命令安装hb

   ```
   pip3 uninstall ohos-build # 如果安装了hb,先卸载
   pip3 install build/lite
   ```
2. 设置环境变量

   ```
   vim ~/.bashrc
   ```

   将以下命令拷贝到.bashrc文件的最后一行，保存并退出。

   ```
   export PATH=~/.local/bin:$PATH
   ```

   执行如下命令更新环境变量。

   ```
   source ~/.bashrc
   ```
3. 执行"hb -h"，有打印以下信息即表示安装成功：

   ```
   usage: hb

   OHOS build system

   positional arguments:
     {build,set,env,clean}
       build               Build source code
       set                 OHOS build settings
       env                 Show OHOS build env
       clean               Clean output

   optional arguments:
     -h, --help            show this help message and exit
   ```

## 安装arm-none-eabi-gcc

1. 打开Linux编译服务器终端。
2. 下载[arm-none-eabi-gcc 编译工具下载](https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2)
3. 解压 [gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2](https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2) 安装包至\~/toolchain/路径下。

   ```shell
   mkdir -p ~/toolchain/
   tar -jxvf gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2 -C ~/toolchain/
   ```
4. 设置环境变量。

   ```
   vim ~/.bashrc
   ```

   将以下命令拷贝到.bashrc文件的最后一行，保存并退出。

   ```
   export PATH=~/toolchain/gcc-arm-none-eabi-10.3-2021.10/bin:$PATH
   ```
5. 生效环境变量。

   ```
   source ~/.bashrc
   ```

## 编译流程

[编译构建使用指](https://gitee.com/openharmony/docs/blob/master/zh-cn/device-dev/subsystems/subsys-build-mini-lite.md)

```shell
hb set -root .
hb set -p

bestechnic
 > display_demo
   iotlink_demo
   xts_demo

选择display_demo

hb build -f
```

## 烧录打印

1. windows10 安装uart 串口驱动 [CP2102usbqd](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)。
2. 把 out/bes2600w/`<product name>`/write_flash_gui-`<timestamp>`.tar.gz 拷贝到windows 环境解压。
3. 双击 write_flash_gui-`<timestamp>`/write_flash_gui/Wifi_download_main.exe。
4. 点击串口列表 ![fileset](https://gitee.com/besyangjian/images/raw/master/fileset.png) 点击 list 按钮勾选弹出来的串口，然后点击ok。
5. 点击开始烧录 ![start](https://gitee.com/besyangjian/images/raw/master/start.png) 等待烧录。
6. 模组 单击 reset按钮（详见板子的按钮）进行烧录,烧录成功图形界面会显示success,反之显示fail。
7. 打开串口工具，reset 或者power 按键（详见板子的按钮） 启动板子，查看log。

# 相关仓

* [device/soc/bestechnic](https://gitee.com/openharmony/device_soc_bestechnic)

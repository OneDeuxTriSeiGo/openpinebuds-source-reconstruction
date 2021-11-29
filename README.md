# device_bestechnic

#### 介绍

以下内容步骤参考[quickstart-lite-env-setup-linux](https://gitee.com/openharmony/docs/blob/master/zh-cn/device-dev/quick-start/quickstart-lite-env-setup-linux.md)

仓库包含编译构建脚本和打包镜像工具

系统要求：Ubuntu16.04 和 Ubuntu18.04 64位系统版本。

编译环境搭建包含如下几步：

1. 获取源码
2. 安装必要的库和工具
3. 安装python3
4. 安装arm-none-eabi-gcc
5. 安装hb
6. 编译流程
7. 烧录

## 获取代码流程

```shell
mkdir openharmony_bestechnic

cd openharmony_bestechnic

repo init -u https://gitee.com/openharmony-sig/manifest.git -m devboard_bestechnic.xml --no-repo-verify

repo sync -c

repo forall -c 'git lfs pull'
```

## 安装必要的库和工具

> - 通常系统默认安装samba、vim等常用软件，需要做适当适配以支持Linux服务器与Windows工作台之间的文件共享。

> - 使用如下apt-get命令安装编译所需的必要的库和工具：

```
sudo apt-get install build-essential gcc g++ make zlib* libffi-dev e2fsprogs pkg-config flex bison perl bc openssl libssl-dev libelf-dev libc6-dev-amd64 binutils binutils-dev libdwarf-dev u-boot-tools mtd-utils gcc-arm-linux-gnueabi
```

## 安装Python3`<a name="section1238412211211"></a>`

1. 打开Linux编译服务器终端。
2. 输入如下命令，查看python版本号：

   ```
   python3 --version
   ```

   如果低于python3.7版本，不建议直接升级，请按照如下步骤重新安装。以python3.8为例，按照以下步骤安装python。

   1. 运行如下命令，查看Ubuntu版本：

   ```
   cat /etc/issue
   ```

   1. 根据Ubuntu不同版本，安装python。
      - 如果Ubuntu 版本为18+，运行如下命令。

        ```
        sudo apt-get install python3.8
        ```
      - 如果Ubuntu版本为16。

        a. 安装依赖包

        ```
        sudo apt update && sudo apt install software-properties-common
        ```

        b. 添加deadsnakes PPA 源，然后按回车键确认安装。

        ```
        sudo add-apt-repository ppa:deadsnakes/ppa
        ```

        c. 安装python3.8

        ```
        sudo apt upgrade && sudo apt install python3.8
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

## 安装hb`<a name="section15794154618411"></a>`

### 前提条件`<a name="section1083283711515"></a>`

请先安装Python 3.7.4及以上版本，请见[安装Python3](#section1238412211211)。

### 安装方法`<a name="section11518484814"></a>`

1. 运行如下命令安装hb

   ```
   python3 -m pip install --user ohos-build
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

## 安装arm-none-eabi-gcc `<a name="section12202192215415"></a>`

1. 打开Linux编译服务器终端。
2. 下载[arm-none-eabi-gcc 编译工具下载](https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2)
3. 解压 [gcc-arm-bi-9-2019-q4-major-x86_64-linux.tar.bz2](https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2) 安装包至\~/toolchain/路径下。

   ```shell
   mkdir -p ~/toolchain/
   tar -zxvf gcc-arm-bi-9-2019-q4-major-x86_64-linux.tar.bz2 -C ~/toolchain/
   ```
4. 设置环境变量。

   ```
   vim ~/.bashrc
   ```

   将以下命令拷贝到.bashrc文件的最后一行，保存并退出。

   ```
   export PATH=~/toolchain/gcc-arm-none-eabi-9-2019-q4-major/bin:$PATH
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

选择display_demo

hb build -f --patch
```

## 烧录打印

1. windows10 安装uart 串口驱动 [CP2102usbqd](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
2. 把 out/bes2600w/`<product name>`/write_flash_gui-`<timestamp>`.tar.gz 拷贝到windows 环境解压
3. 双击 write_flash_gui-`<timestamp>`/write_flash_gui/Wifi_download_main.exe
4. 点击串口列表![](./bes2600/burn_tools/write_flash_gui/images/fileset.png) 点击 list 按钮勾选弹出来的串口，然后点击ok
5. 点击开始烧录![](./bes2600/burn_tools/write_flash_gui/images/start.png) 等待烧录
6. 模组 单击 reset 或者 power 进行烧录
7. 打开串口工具，reset 或者power 按键启动板子，查看log

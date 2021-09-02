#!/usr/bin/python
# -*- coding: UTF-8 -*-
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

import os
import sys
import time
import string
import subprocess
import os,os.path

src_flash_yaml = sys.argv[1]
dest_flash_yaml = sys.argv[2]
temp_path = sys.argv[3]

debug_info = "\r\n\r\n pls run:\r\n\r\n \
cd device/bestechnic/bes2600w/burn_tools \r\n\r\n \
python tools/gen_yaml_signle.py ./tools/wifi_Download_cfg.yaml ${addr} ${burn_name} ${size} ${temp_yaml} \r\n\r\n \
${addr} ${burn_name} ${size} configed in vendor/bestechnic/product_name/config.json\r\n\r\n \
check temp dir ${burn_name}.yaml \r\n\r\n"


if not os.path.exists(temp_path):
    raise Exception('no path : {} {}'.format(temp_path,debug_info))

src_yaml_fd = open(src_flash_yaml,"r")
dest_yaml_fd = open(dest_flash_yaml,'w')

while True:
    line = src_yaml_fd.readline()
    if not line:
        break

    if line.startswith("- "):
        print(line)
        label_addr = src_yaml_fd.readline()
        label_file_name = src_yaml_fd.readline()
        name_arry = label_file_name.split(": ")
        temp_file_name = temp_path+name_arry[1].replace('\n','').replace('\r','').replace(".bin",".yaml")
        if os.path.exists(temp_file_name):
            temp_yaml_fd = open(temp_file_name,'r')
            while True:
                temp_line = temp_yaml_fd.readline()
                if not temp_line:
                    break
                print(line)
                dest_yaml_fd.write(temp_line)
            temp_yaml_fd.close()
        else:
            dest_yaml_fd.write(line)
            dest_yaml_fd.write(label_addr)
            dest_yaml_fd.write(label_file_name)
            while True:
                src_line = src_yaml_fd.readline()
                dest_yaml_fd.write(src_line)
                if src_line.strip().split(": ")[0] == "SWITCH" or not src_line:
                    break
    else:
        dest_yaml_fd.write(line)

dest_yaml_fd.close()
src_yaml_fd.close()

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

dest_flash_yaml = sys.argv[1]
addr = sys.argv[2]
file_name = sys.argv[3]+".bin"
file_size = sys.argv[4]
temp_path = sys.argv[5]

if not os.path.exists(temp_path):
    os.mkdir(temp_path)

temp_file = temp_path+file_name.replace(".bin",".yaml")

dest_yaml_fd = open(dest_flash_yaml,'r')
temp_yaml_fd = open(temp_file,'w+')

while True:
    line = dest_yaml_fd.readline()
    if not line:
        break

    if line.startswith("- "):
        print (line)
        # read addr
        bin_addr = dest_yaml_fd.readline()
        # read name
        bin_name = dest_yaml_fd.readline()
        name_arry = bin_name.split(": ")
        if name_arry[1].replace('\n','').replace('\r','') == file_name:
            addr_arr = bin_addr.split(": ")
            if (name_arry[1].replace('\n','') == "ota_boot1.bin") and (addr == "0"):
                addr = "null"
            bin_addr = addr_arr[0]+": "+addr+"\n"

            temp_yaml_fd.write(line)
            print (bin_addr)
            temp_yaml_fd.write(bin_addr)
            print (bin_name)
            temp_yaml_fd.write(bin_name)

            while True:
                line = dest_yaml_fd.readline()
                arr = line.strip().split(": ")
                if arr[0] == "SIZE":
                    line = "    "+arr[0]+": "+file_size+"\n"
                print (line)
                temp_yaml_fd.write(line)
                if arr[0] == "SWITCH":
                    break
temp_yaml_fd.close()
dest_yaml_fd.close()
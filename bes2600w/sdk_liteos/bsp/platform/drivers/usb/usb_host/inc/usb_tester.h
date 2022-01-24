/*
 * Copyright (c) 2021 Bestechnic (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef USB_TESTER_H
#define USB_TESTER_H

#include "FATFileSystem.h"
#include <stdint.h>


class USBFileSystem : public FATFileSystem
{

public:
    USBFileSystem();
    
    void SetDevice(int device);

    int GetDevice(void);

    virtual int disk_initialize();
    
    virtual int disk_write(const uint8_t * buffer, uint64_t sector, uint8_t count);
    
    virtual int disk_read(uint8_t * buffer, uint64_t sector, uint8_t count);
        
    virtual uint64_t disk_sectors();

protected:	
	int _device;
	u32 _blockSize;
	u32 _blockCount;
	
};


#endif

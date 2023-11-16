/*
Copyright (c) 2010 Peter Barrett

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "mbed.h"
#include "USBHost.h"
#include "Utils.h"
#include "FATFileSystem.h"

int MassStorage_ReadCapacity(int device, u32* blockCount, u32* blockSize);
int MassStorage_Read(int device, u32 blockAddr, u32 blockCount, u8* dst, u32 blockSize);
int MassStorage_Write(int device, u32 blockAddr, u32 blockCount, u8* dst, u32 blockSize);

class USBFileSystem : public FATFileSystem
{
    int _device;
    u32 _blockSize;
    u32 _blockCount;
    
public:
    USBFileSystem() : FATFileSystem("usb"),_device(0),_blockSize(0),_blockCount(0)
    {
    }
    
    void SetDevice(int device)
    {
        _device = device;
    }
    
    virtual int disk_initialize()
    {
        return MassStorage_ReadCapacity(_device,&_blockCount,&_blockSize);
    }
    
    virtual int disk_write(const char *buffer, int block_number)
    {
        return MassStorage_Write(_device,block_number,1,(u8*)buffer,_blockSize);
    }
    
    virtual int disk_read(char *buffer, int block_number)
    {
        return MassStorage_Read(_device,block_number,1,(u8*)buffer,_blockSize);
    }
        
    virtual int disk_sectors()
    {
        return _blockCount;
    }
};

Serial pc(USBTX, USBRX);

void DumpFS(int depth, int count)
{
    DIR *d = opendir("/usb");
    if (!d)
    {
        printf("USB file system borked\n");
        return;
    }

    printf("\nDumping root dir\n");
    struct dirent *p;
    while ((p = readdir(d)))
    {
        int len = sizeof( dirent);
        printf("%s %d\n", p->d_name, len);
    }
    closedir(d);
}

int OnDiskInsert(int device)
{
    USBFileSystem fs;
    fs.SetDevice(device);
    DumpFS(0,0);
    return 0;
}

/*
    Simple test shell to exercise mouse,keyboard,mass storage and hubs.
    Add 2 15k pulldown resistors between D+/D- and ground, attach a usb socket and have at it.
*/

int main()
{
    pc.baud(460800);
    printf("USBShell\nNow get a bunch of usb things and plug them in\n");
    USBInit();
    for (;;)
        USBLoop();
}

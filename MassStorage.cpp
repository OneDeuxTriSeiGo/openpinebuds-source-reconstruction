
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

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "Utils.h"
#include "USBHost.h"


int MassStorage_ReadCapacity(int device, u32* blockCount, u32* blockSize);
int MassStorage_ReadBlock(int device, u32 block, u8* dst);
int MassStorage_WriteBlock(int device, u32 block, const u8* dst);


#define ERR_BAD_CSW_SIGNATURE -200

#define CBW_SIGNATURE  0x43425355
#define CSW_SIGNATURE  0x53425355

//  Command Block
typedef struct
{
    u32 Signature;
    u32 Tag;
    u32 TransferLength;
    u8 Flags;
    u8 LUN;
    u8 CBLength;
    u8 CB[16];   // only 6 really
} CBW;

//  Status block
typedef struct
{
    u32 Signature;
    u32 Tag;
    u32 DataResidue;
    u8 Status;
} CSW;

int SCSIRequestSense(int device);

int DoSCSI(int device, const u8* cmd, int cmdLen, int flags, u8* data, u32 transferLen)
{
    CBW cbw;
    cbw.Signature = CBW_SIGNATURE;
    cbw.Tag = 0;
    cbw.TransferLength = transferLen;
    cbw.Flags = flags;
    cbw.LUN = 0;
    cbw.CBLength = cmdLen;
    memset(cbw.CB,0,sizeof(cbw.CB));
    memcpy(cbw.CB,cmd,cmdLen);

    int r;
    r = USBBulkTransfer(device,0x01,(u8*)&cbw,31);   // Send the command
    if (r < 0)
        return r;

    if (data)
    {
        r = USBBulkTransfer(device,flags | 1,data,transferLen);
        if (r < 0)
            return r;
    }

    CSW csw;
    csw.Signature = 0;
    r = USBBulkTransfer(device,0x81,(u8*)&csw,13);
    if (r < 0)
        return r;

    if (csw.Signature != CSW_SIGNATURE)
        return ERR_BAD_CSW_SIGNATURE;

    // ModeSense?
    if (csw.Status == 1 && cmd[0] != 3)
        return SCSIRequestSense(device);

    return csw.Status;
}

int SCSITestUnitReady(int device)
{
    u8 cmd[6];
    memset(cmd,0,6);
    return DoSCSI(device,cmd,6,DEVICE_TO_HOST,0,0);
}

int SCSIRequestSense(int device)
{
    u8 cmd[6] = {0x03,0,0,0,18,0};
    u8 result[18];
    int r = DoSCSI(device,cmd,6,DEVICE_TO_HOST,result,18);
    return r;
}

int SCSIInquiry(int device)
{
    u8 cmd[6] = {0x12,0,0,0,36,0};
    u8 result[36+2];
    result[36] = '\n';
    result[37] = 0;
    int r = DoSCSI(device,cmd,6,DEVICE_TO_HOST,result,36);
    if (r == 0)
        printf((const char*)result + 8);
    return r;
}

int SCSIReadCapacity(int device, u32* blockCount, u32* blockSize)
{
    u8 cmd[10] = {0x25,0,0,0,8,0,0,0,0,0};
    u8 result[8];
    *blockSize = 0;
    *blockCount = 0;
    int r = DoSCSI(device,cmd,10,DEVICE_TO_HOST,result,8);
    if (r == 0)
    {
        *blockCount = BE32(result);
        *blockSize = BE32(result+4);
    }
    return r;
}

int SCSITransfer(int device, u32 blockAddr, u32 blockCount, u8* dst, u32 blockSize, int direction)
{
    //  USB hardware will only do 4k per transfer
    while (blockCount*blockSize > 4096)
    {
        int count = 4096/blockSize;
        int r = SCSITransfer(device,blockAddr,count,dst,blockSize,direction);
        dst += count*blockSize;
        blockAddr += count;
        blockCount -= count;
    }

    u8 cmd[10];
    memset(cmd,0,10);
    cmd[0] = (direction == DEVICE_TO_HOST) ? 0x28 : 0x2A;
    BE32(blockAddr,cmd+2);
    BE16(blockCount,cmd+7);
    return DoSCSI(device,cmd,10,direction,dst,blockSize*blockCount);
}

int MassStorage_ReadCapacity(int device, u32* blockCount, u32* blockSize)
{
    return SCSIReadCapacity(device,blockCount,blockSize);
}

int MassStorage_Read(int device, u32 blockAddr, u32 blockCount, u8* dst, u32 blockSize = 512)
{
    return SCSITransfer(device,blockAddr,blockCount,dst,blockSize,DEVICE_TO_HOST);
}

int MassStorage_Write(int device, u32 blockAddr, u32 blockCount, u8* dst, u32 blockSize = 512)
{
    return SCSITransfer(device,blockAddr,blockCount,dst,blockSize,HOST_TO_DEVICE);
}


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

#define AUTOEVT(_class,_subclass,_protocol) (((_class) << 16) | ((_subclass) << 8) | _protocol)
#define AUTO_KEYBOARD AUTOEVT(CLASS_HID,1,1)
#define AUTO_MOUSE AUTOEVT(CLASS_HID,1,2)

u8 auto_mouse[4];       // buttons,dx,dy,scroll
u8 auto_keyboard[8];    // modifiers,reserved,keycode1..keycode6
u8 auto_joystick[4];    // x,y,buttons,throttle

void AutoEventCallback(int device, int endpoint, int status, u8* data, int len, void* userData)
{
    int evt = (int)userData;
    switch (evt)
    {
        case AUTO_KEYBOARD:
            printf("AUTO_KEYBOARD ");
            break;
        case AUTO_MOUSE:
            printf("AUTO_MOUSE ");
            break;
        default:
            printf("HUH ");
    }
    printfBytes("data",data,len);
    USBInterruptTransfer(device,endpoint,data,len,AutoEventCallback,userData);
}

//  Establish transfers for interrupt events
void AddAutoEvent(int device, InterfaceDescriptor* id, EndpointDescriptor* ed)
{
    if ((ed->bmAttributes & 3) != ENDPOINT_INTERRUPT || !(ed->bEndpointAddress & 0x80))
        return;
    
    // Make automatic interrupt enpoints for known devices
    u32 evt = AUTOEVT(id->bInterfaceClass,id->bInterfaceSubClass,id->bInterfaceProtocol);
    u8* dst = 0;
    int len;
    switch (evt)
    {
        case AUTO_MOUSE:
            dst = auto_mouse;
            len = sizeof(auto_mouse);
            break;
        case AUTO_KEYBOARD:
            dst = auto_keyboard;
            len = sizeof(auto_keyboard);
            break;
        default:
            printf("Interrupt endpoint %02X %08X\n",ed->bEndpointAddress,evt);
            break;
    }
    if (dst)
    {
        printf("Auto Event for %02X %08X\n",ed->bEndpointAddress,evt);
        USBInterruptTransfer(device,ed->bEndpointAddress,dst,len,AutoEventCallback,(void*)evt);
    }
}

void PrintString(int device, int i)
{
    u8 buffer[256];
    int le = GetDescriptor(device,DESCRIPTOR_TYPE_STRING,i,buffer,255);
    if (le < 0)
         return;
    char* dst = (char*)buffer;
    for (int j = 2; j < le; j += 2)
        *dst++ = buffer[j];
    *dst = 0;
    printf("%d:%s\n",i,(const char*)buffer);
 }
 
//  Walk descriptors and create endpoints for a given device
int StartAutoEvent(int device, int configuration, int interfaceNumber)
{
    u8 buffer[255];
    int err = GetDescriptor(device,DESCRIPTOR_TYPE_CONFIGURATION,0,buffer,255);
    if (err < 0)
        return err;

    int len = buffer[2] | (buffer[3] << 8);
    u8* d = buffer;
    u8* end = d + len;
    while (d < end)
    {
        if (d[1] == DESCRIPTOR_TYPE_INTERFACE)
        {
            InterfaceDescriptor* id = (InterfaceDescriptor*)d;
            if (id->bInterfaceNumber == interfaceNumber)
            {
                 d += d[0];
                while (d < end && d[1] != DESCRIPTOR_TYPE_INTERFACE)
                {
                    if (d[1] == DESCRIPTOR_TYPE_ENDPOINT)
                        AddAutoEvent(device,id,(EndpointDescriptor*)d);
                    d += d[0];
                }
            }
        }
        d += d[0];
    }
    return 0;
}

//  Implemented in main.cpp
int OnDiskInsert(int device);

void OnLoadDevice(int device, DeviceDescriptor* deviceDesc, InterfaceDescriptor* interfaceDesc)
{
    printf("LoadDevice %d %02X:%02X:%02X\n",device,interfaceDesc->bInterfaceClass,interfaceDesc->bInterfaceSubClass,interfaceDesc->bInterfaceProtocol);
    char s[128];
    for (int i = 1; i < 3; i++)
    {
        if (GetString(device,i,s,sizeof(s)) < 0)
            break;
        printf("%d: %s\n",i,s);
    }
    
    switch (interfaceDesc->bInterfaceClass)
    {
        case CLASS_MASS_STORAGE:
            if (interfaceDesc->bInterfaceSubClass == 0x06 && interfaceDesc->bInterfaceProtocol == 0x50)
                OnDiskInsert(device);    // it's SCSI!
            break;
        default:
            StartAutoEvent(device,1,0);
            break;
    }
}
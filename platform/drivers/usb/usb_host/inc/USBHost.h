/*
 * Copyright (c) 2021 bestechnic (Shanghai) Technologies CO., LIMITED.
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
#ifndef USBHOST_H
#define USBHOST_H

#include "plat_types.h"

#define ENDPOINT_CONTROL 0
#define ENDPOINT_ISOCRONOUS 1
#define ENDPOINT_BULK 2
#define ENDPOINT_INTERRUPT 3

#define  DESCRIPTOR_TYPE_DEVICE			1
#define  DESCRIPTOR_TYPE_CONFIGURATION	2
#define  DESCRIPTOR_TYPE_STRING			3
#define  DESCRIPTOR_TYPE_INTERFACE		4
#define  DESCRIPTOR_TYPE_ENDPOINT		5

#define DESCRIPTOR_TYPE_HID         0x21
#define DESCRIPTOR_TYPE_REPORT      0x22
#define DESCRIPTOR_TYPE_PHYSICAL    0x23
#define DESCRIPTOR_TYPE_HUB         0x29

enum USB_CLASS_CODE
{
	CLASS_DEVICE,
	CLASS_AUDIO,
	CLASS_COMM_AND_CDC_CONTROL,
	CLASS_HID,
	CLASS_PHYSICAL = 0x05,
	CLASS_STILL_IMAGING,
	CLASS_PRINTER,
	CLASS_MASS_STORAGE,
	CLASS_HUB,
	CLASS_CDC_DATA,
	CLASS_SMART_CARD,
	CLASS_CONTENT_SECURITY = 0x0D,
	CLASS_VIDEO = 0x0E,
	CLASS_DIAGNOSTIC_DEVICE = 0xDC,
	CLASS_WIRELESS_CONTROLLER = 0xE0,
	CLASS_MISCELLANEOUS = 0xEF,
	CLASS_APP_SPECIFIC = 0xFE,
	CLASS_VENDOR_SPECIFIC = 0xFF
};

#define  DEVICE_TO_HOST         0x80
#define  HOST_TO_DEVICE         0x00
#define  REQUEST_TYPE_CLASS     0x20
#define  RECIPIENT_DEVICE       0x00
#define  RECIPIENT_INTERFACE    0x01
#define  RECIPIENT_ENDPOINT		0x02
#define  RECIPIENT_OTHER		0x03

#define  GET_STATUS				0
#define  CLEAR_FEATURE			1
#define  SET_FEATURE			3
#define  SET_ADDRESS			5
#define  GET_DESCRIPTOR			6
#define  SET_DESCRIPTOR			7
#define  GET_CONFIGURATION		8
#define  SET_CONFIGURATION		9
#define  GET_INTERFACE			10
#define  SET_INTERFACE			11
#define  SYNCH_FRAME			11

//		-5 is nak
/*
0010 ACK Handshake
1010 NAK Handshake
1110 STALL Handshake
0110 NYET (No Response Yet)
*/

#define IO_PENDING -100
#define ERR_ENDPOINT_NONE_LEFT -101
#define ERR_ENDPOINT_NOT_FOUND -102
#define ERR_DEVICE_NOT_FOUND -103
#define ERR_DEVICE_NONE_LEFT -104
#define ERR_HUB_INIT_FAILED -105
#define ERR_INTERFACE_NOT_FOUND -106

typedef struct
{
	u8	bLength;
	u8	bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize;
	u16 idVendor;
	u16 idProduct;
	u16 bcdDevice;	// version
	u8 iManufacturer;
	u8 iProduct;
	u8 iSerialNumber;
	u8 bNumConfigurations;
} DeviceDescriptor;	// 16 bytes

typedef struct
{
	u8	bLength;
	u8	bDescriptorType;
	u16	wTotalLength;
	u8	bNumInterfaces;
	u8	bConfigurationValue;	// Value to use as an argument to select this configuration
	u8	iConfiguration;			// Index of String Descriptor describing this configuration
	u8	bmAttributes;			// Bitmap D7 Reserved, set to 1. (USB 1.0 Bus Powered),D6 Self Powered,D5 Remote Wakeup,D4..0 = 0
	u8	bMaxPower;				// Maximum Power Consumption in 2mA units
} ConfigurationDescriptor;

typedef struct
{
	u8	bLength;
	u8	bDescriptorType;
	u8  bInterfaceNumber;
	u8	bAlternateSetting;
	u8	bNumEndpoints;
	u8	bInterfaceClass;
	u8	bInterfaceSubClass;
	u8	bInterfaceProtocol;
	u8	iInterface;				// Index of String Descriptor Describing this interface
} InterfaceDescriptor;

typedef struct
{
	u8	bLength;
	u8	bDescriptorType;
	u8	bEndpointAddress;	// Bits 0:3 endpoint, Bits 7 Direction 0 = Out, 1 = In (Ignored for Control Endpoints)
	u8	bmAttributes;		// Bits 0:1 00 = Control, 01 = Isochronous, 10 = Bulk, 11 = Interrupt
	u16 wMaxPacketSize;
	u8	bInterval;			// Interval for polling endpoint data transfers.
} EndpointDescriptor;

typedef struct {
  u8    bLength;
  u8    bDescriptorType;
  u16   bcdHID;
  u8    bCountryCode;
  u8    bNumDescriptors;
  u8    bDescriptorType2;
  u16   wDescriptorLength;
} HIDDescriptor;

//============================================================================
//============================================================================


void USBInit();
void USBLoop();
u8* USBGetBuffer(u32* len);

//	Optional callback for transfers, called at interrupt time
typedef void (*USBCallback)(int device, int endpoint, int status, u8* data, int len, void* userData);

//	Transfers
int USBControlTransfer(int device, int request_type, int request, int value, int index, u8* data, int length, USBCallback callback = 0, void* userData = 0);
int USBInterruptTransfer(int device, int ep, u8* data, int length, USBCallback callback = 0, void* userData = 0);
int USBBulkTransfer(int device, int ep, u8* data, int length, USBCallback callback = 0, void* userData = 0);

//	Standard Device methods
int GetDescriptor(int device, int descType, int descIndex, u8* data, int length);
int GetString(int device, int index, char* dst, int length);
int SetAddress(int device, int new_addr);
int SetConfiguration(int device, int configNum);
int SetInterface(int device, int ifNum, int altNum);

//	Implemented to notify app of the arrival of a device
void OnLoadDevice(int device, DeviceDescriptor* deviceDesc, InterfaceDescriptor* interfaceDesc, u8 *itfDescEnd);
void OnUnloadDevice(int device);

#endif

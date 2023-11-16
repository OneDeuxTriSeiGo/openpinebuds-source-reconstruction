
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

//	Config (default uses x bytes)
#define MAX_DEVICES 8				// Max number of devices
#define MAX_ENDPOINTS_TOTAL 16		// Max number of endpoints total
#define MAX_ENDPOINTS_PER_DEVICE 8	// Max number of endpoints for any one device

#define  USBLOG 1
#if USBLOG
#define  LOG(...)       printf(__VA_ARGS__)
#else 
#define  LOG(...)       do {} while(0)
#endif

// USB host structures

#define USB_RAM_SIZE 16*1024	// AHB SRAM block 1 TODO MACHINE DEPENDENT
#define USB_RAM_BASE 0x2007C000

#define TOKEN_SETUP 0
#define TOKEN_IN  1
#define TOKEN_OUT 2

//	Status flags from hub
#define PORT_CONNECTION 0
#define PORT_ENABLE  1
#define PORT_SUSPEND  2
#define PORT_OVER_CURRENT 3
#define PORT_RESET 4
#define PORT_POWER 8
#define PORT_LOW_SPEED 9

#define C_PORT_CONNECTION 16
#define C_PORT_ENABLE 17
#define C_PORT_SUSPEND 18
#define C_PORT_OVER_CURRENT 19
#define C_PORT_RESET 20

typedef struct {
	u8 bm_request_type;
	u8 b_request;
	u16 w_value;
	u16 w_index;
	u16 w_length;
} Setup;


//	Hub stuff is kept private just to keep api simple
int SetPortFeature(int device, int feature, int index);
int ClearPortFeature(int device, int feature, int index);
int SetPortPower(int device, int port);
int SetPortReset(int device, int port);
int GetPortStatus(int device, int port, u32* status);

//===================================================================
//===================================================================
//	Hardware defines

//	HcControl
#define PeriodicListEnable	0x00000004
#define	IsochronousEnable	0x00000008
#define	ControlListEnable	0x00000010
#define	BulkListEnable		0x00000020
#define	OperationalMask		0x00000080
#define	HostControllerFunctionalState	0x000000C0

//	HcCommandStatus
#define HostControllerReset	0x00000001
#define ControlListFilled	0x00000002
#define BulkListFilled		0x00000004

//	HcInterruptStatus Register
#define	WritebackDoneHead		0x00000002
#define	StartofFrame			0x00000004
#define ResumeDetected			0x00000008
#define UnrecoverableError		0x00000010
#define FrameNumberOverflow		0x00000020
#define RootHubStatusChange		0x00000040
#define OwnershipChange			0x00000080
#define MasterInterruptEnable	0x80000000

//	HcRhStatus
#define SetGlobalPower			0x00010000
#define DeviceRemoteWakeupEnable	0x00008000

//	HcRhPortStatus (hub 0, port 1)
#define CurrentConnectStatus	0x00000001
#define	PortEnableStatus		0x00000002
#define PortSuspendStatus		0x00000004
#define PortOverCurrentIndicator	0x00000008
#define PortResetStatus			0x00000010

#define PortPowerStatus			0x00000100
#define LowspeedDevice			0x00000200
#define HighspeedDevice			0x00000400

#define ConnectStatusChange	(CurrentConnectStatus << 16)
#define PortResetStatusChange	(PortResetStatus << 16)


#define  TD_ROUNDING		(u32)0x00040000
#define  TD_SETUP			(u32)0x00000000
#define  TD_IN				(u32)0x00100000
#define  TD_OUT				(u32)0x00080000
#define  TD_DELAY_INT(x)	(u32)((x) << 21)
#define  TD_TOGGLE_0		(u32)0x02000000
#define  TD_TOGGLE_1		(u32)0x03000000
#define  TD_CC				(u32)0xF0000000

//	HostController EndPoint Descriptor
typedef struct {
	volatile u32	Control;
	volatile u32	TailTd;
	volatile u32	HeadTd;
	volatile u32	Next;
} HCED;

// HostController Transfer Descriptor
typedef struct {
	volatile u32	Control;
	volatile u32	CurrBufPtr;
	volatile u32	Next;
	volatile u32	BufEnd;
} HCTD;

// Host Controller Communication Area
typedef struct {
	volatile u32	InterruptTable[32];
	volatile u16	FrameNumber;
	volatile u16	FrameNumberPad;
	volatile u32	DoneHead;
	volatile u8		Reserved[120];
} HCCA;

//====================================================================================
//====================================================================================

class HostController;
class Endpoint;
class Device;

//	  must be 3*16 bytes long
class Endpoint
{
public:
	HCED	EndpointDescriptor;    // Pointer to EndpointDescriptor == Pointer to Endpoint
	HCTD	TDHead;

	enum State
	{
		Free,
		NotQueued,
		Idle,
		SetupQueued,
		DataQueued,
		StatusQueued,
		CallbackPending
	};
	
	volatile u8 CurrentState;
	u8		Flags;			// 0x80 In, 0x03 mask endpoint type

	u16		Length;
	u8*		Data;
	USBCallback Callback;	 // Must be a multiple of 16 bytes long
	void*  UserData;
  
	int Address()
	{
		int ep = (EndpointDescriptor.Control >> 7) & 0xF;
		if (ep)
			ep |= Flags & 0x80;
		return ep;
	}
	
	int Device()
	{
		return EndpointDescriptor.Control & 0x7F;
	}

	int Status()
	{
		return (TDHead.Control >> 28) & 0xF;
	}

	u32 Enqueue(u32 head)
	{
		if (CurrentState == NotQueued)
		{
			EndpointDescriptor.Next = head;
			head = (u32)&EndpointDescriptor;
			CurrentState = Idle;
		}
		return head;
	}
};

class Device
{
public:
	u8	_endpointMap[MAX_ENDPOINTS_PER_DEVICE*2];
	u8	Hub;
	u8	Port;
	u8	Addr;
	u8	Pad;

	//	Only if this device is a hub
	u8	HubPortCount;	// nonzero if this is a hub
	u8	HubInterruptData;
	u8	HubMap;
	u8	HubMask;

	int Flags;		// 1 = Disconnected

	Setup	SetupBuffer;

	// Allocate endpoint zero
	int Init(DeviceDescriptor* d, int hub, int port, int addr, int lowSpeed)
	{
		Hub = hub;
		Port = port;
		Addr = addr;
		Flags = lowSpeed;
		memset(_endpointMap,0xFF,sizeof(_endpointMap));
		return 0;
	}

	int SetEndpointIndex(int ep, int endpointIndex)
	{
		for (int i = 0; i < MAX_ENDPOINTS_PER_DEVICE*2; i += 2)
		{
			if (_endpointMap[i] == 0xFF)	// Add endpoint to map
			{
				_endpointMap[i] = ep;
				_endpointMap[i+1] = endpointIndex;
				return 0;
			}
		}
		return ERR_ENDPOINT_NONE_LEFT;
	}

	int GetEndpointIndex(int ep)
	{
		for (int i = 0; i < MAX_ENDPOINTS_PER_DEVICE*2; i += 2)
		{
			if (_endpointMap[i] == ep)
				return _endpointMap[i+1];
			if (_endpointMap[i] == 0xFF)
				break;
		}
		return -1;
	}
};

class HostController
{
public:
	HCCA		CommunicationArea;
	Endpoint	Endpoints[MAX_ENDPOINTS_TOTAL];    // Multiple of 16
	
	Endpoint	EndpointZero;						// For device enumeration
	HCTD		_commonTail;
	Setup		_setupZero;
	
	Device	Devices[MAX_DEVICES];
	u32	_frameNumber;			// 32 bit ms counter

	u8	_callbacksPending;		//	Endpoints with callbacks are pending, set from ISR via ProcessDoneQueue
	u8	_rootHubStatusChange;	//	Root hub status has changed, set from ISR
	u8	_unused0;
	u8	_unused1;

	u8	_connectPending;	//	Reset has initiated a connect
	u8	_connectCountdown;	//	Number of ms left after reset before we can connect
	u8	_connectHub;		//	Will connect on this hub
	u8	_connectPort;		//	... and this port

	u8	SRAM[0];			// Start of free SRAM

	void Loop()
	{
		u16 elapsed = CommunicationArea.FrameNumber - (u16)_frameNumber;	// extend to 32 bits
		_frameNumber += elapsed;

		// Do callbacks, if any
		while (_callbacksPending)
		{
			for (int i = 0; i < MAX_ENDPOINTS_TOTAL; i++)
			{
				Endpoint* endpoint = Endpoints + i;
				if (endpoint->CurrentState == Endpoint::CallbackPending)
				{
					_callbacksPending--;
					endpoint->CurrentState = Endpoint::Idle;
					endpoint->Callback(endpoint->Device(),endpoint->Address(),endpoint->Status(),endpoint->Data,endpoint->Length,endpoint->UserData);
				}
			}
		}

		//	Deal with changes on the root hub
		if (_rootHubStatusChange)
		{
			u32 status = LPC_USB->HcRhPortStatus1;
			_rootHubStatusChange = 0;
			if (status >> 16)
			{
				HubStatusChange(0,1,status);
				LPC_USB->HcRhPortStatus1 = status & 0xFFFF0000;	// clear status changes
			}
		}

		//	Connect after reset timeout
		if (_connectCountdown)
		{
			if (elapsed >= _connectCountdown)
			{
				_connectCountdown = 0;
				Connect(_connectHub,_connectPort & 0x7F,_connectPort & 0x80);
			} else
				_connectCountdown -= elapsed;
		}
	}

	//	HubInterrupt - bitmap in dev->HubInterruptData
	void HubInterrupt(int device)
	{
		Device* dev = &Devices[device-1];
		for (int i = 0; i < dev->HubPortCount; i++)
		{
			int port = i+1;
			if (dev->HubInterruptData & (1 << port))
			{
				u32 status = 0;
				GetPortStatus(device,port,&status);
				if (status >> 16)
				{
					if (_connectPending && (status & ConnectStatusChange))
						continue;	// Don't connect again until previous device has been added and addressed

					HubStatusChange(device,port,status);
					if (status & ConnectStatusChange)
						ClearPortFeature(device,C_PORT_CONNECTION,port);
					if (status & PortResetStatusChange)
						ClearPortFeature(device,C_PORT_RESET,port);
				}
			}
		}
	}

	static void HubInterruptCallback(int device, int endpoint, int status, u8* data, int len, void* userData)
	{
		HostController* controller = (HostController*)userData;
		if (status == 0)
			controller->HubInterrupt(device);
		USBInterruptTransfer(device,endpoint,data,1,HubInterruptCallback,userData);
	}

	int InitHub(int device)
	{
		u8 buf[16];
		int r= USBControlTransfer(device,DEVICE_TO_HOST | REQUEST_TYPE_CLASS | RECIPIENT_DEVICE,GET_DESCRIPTOR,(DESCRIPTOR_TYPE_HUB << 8),0,buf,sizeof(buf));
		if (r < 0)
			return ERR_HUB_INIT_FAILED;
	    
		//	turn on power on the hubs ports
		Device* dev = &Devices[device-1];
		int ports = buf[2];
		dev->HubPortCount = ports;
		for (int i = 0; i < ports; i++)
			SetPortPower(device,i+1);
	    
		// Enable hub change interrupts
		return USBInterruptTransfer(device,0x81,&dev->HubInterruptData,1,HubInterruptCallback,this);
	}
	
	int AddEndpoint(int device, int ep, int attributes, int maxPacketSize, int interval)
	{
		LOG("AddEndpoint D:%02X A:%02X T:%02X P:%04X I:%02X\n",device,ep,attributes,maxPacketSize,interval);
		Device* dev = &Devices[device-1];
		Endpoint* endpoint = AllocateEndpoint(device,ep,attributes,maxPacketSize);
		if (!endpoint)
			return ERR_ENDPOINT_NONE_LEFT;
		dev->SetEndpointIndex(ep,endpoint - Endpoints);
		endpoint->EndpointDescriptor.Control |= dev->Flags; // Map in slow speed
		return 0;  // TODO ed->bInterval
	}
	
	int AddEndpoint(int device, EndpointDescriptor* ed)
	{
		return AddEndpoint(device,ed->bEndpointAddress,ed->bmAttributes,ed->wMaxPacketSize,ed->bInterval);
	}

	//	  allocate a endpoint
	Endpoint* AllocateEndpoint(int device, int endpointAddress, int type, int maxPacketSize)
	{
		for (int i = 0; i < MAX_ENDPOINTS_TOTAL; i++)
		{
			Endpoint* ep = &Endpoints[i];
			if (ep->CurrentState == 0)
			{
				//LOG("Allocated endpoint %d to %02X:%02X\n",i,device,endpointAddress);
				ep->Flags = (endpointAddress & 0x80) | (type & 3);
				ep->CurrentState = Endpoint::NotQueued;
				ep->EndpointDescriptor.Control = (maxPacketSize << 16) | ((endpointAddress & 0x7F) << 7) | device;
				return ep;
			}
		}
		return 0;
	}

	Endpoint* GetEndpoint(int device, int ep)
	{
		if (device == 0)
		{
			//printf("WARNING: USING DEVICE 0\n");
			return &EndpointZero;
		}
		if (device > MAX_DEVICES)
			return 0;
		int i = Devices[device-1].GetEndpointIndex(ep);
		if (i == -1)
			return 0;
		return Endpoints + i;
	}

	int Transfer(Endpoint* endpoint, int token, u8* data, int len, int state)
	{
		//LOG("Transfer %02X T:%d Len:%d S:%d\n",endpoint->Address(),token,len,state);
	
		int toggle = 0;
		if (endpoint->Address() == 0)
			toggle = (token == TOKEN_SETUP) ? TD_TOGGLE_0 : TD_TOGGLE_1;

		if (token != TOKEN_SETUP)
			token = (token == TOKEN_IN ? TD_IN : TD_OUT);

		HCTD* head = &endpoint->TDHead;
		HCTD* tail = &_commonTail;

		head->Control = TD_ROUNDING | token | TD_DELAY_INT(0) | toggle | TD_CC; 
		head->CurrBufPtr = (u32)data;
		head->BufEnd = (u32)(data + len - 1);
		head->Next = (u32)tail;

		HCED* ed = &endpoint->EndpointDescriptor;
		ed->HeadTd = (u32)head | (ed->HeadTd & 0x00000002);    // carry toggle
		ed->TailTd = (u32)tail;
		
		//HCTD* td = head;
		//LOG("%04X TD %08X %08X %08X Next:%08X\n",CommunicationArea.FrameNumber,td->Control,td->CurrBufPtr,td->BufEnd,td->Next);
		//LOG("%04X ED %08X %08X %08X\n",CommunicationArea.FrameNumber,ed->Control,ed->HeadTd,ed->TailTd);
		
		switch (endpoint->Flags & 3)
		{
			case ENDPOINT_CONTROL:
				LPC_USB->HcControlHeadED = endpoint->Enqueue(LPC_USB->HcControlHeadED);    // May change state NotQueued->Idle
				endpoint->CurrentState = state;											   // Get in before an int
				LPC_USB->HcCommandStatus = LPC_USB->HcCommandStatus | ControlListFilled;
				LPC_USB->HcControl = LPC_USB->HcControl | ControlListEnable;
				break;

			case ENDPOINT_BULK:
				LPC_USB->HcBulkHeadED = endpoint->Enqueue(LPC_USB->HcBulkHeadED);
				endpoint->CurrentState = state;
				LPC_USB->HcCommandStatus = LPC_USB->HcCommandStatus | BulkListFilled;
				LPC_USB->HcControl = LPC_USB->HcControl | BulkListEnable;
				break;

			case ENDPOINT_INTERRUPT:
				CommunicationArea.InterruptTable[0] = endpoint->Enqueue(CommunicationArea.InterruptTable[0]);
				endpoint->CurrentState = state;
				LPC_USB->HcControl |= PeriodicListEnable;
				break;
		}
		return 0;
	}
	
	//	Remove an endpoint from an active queue
	bool Remove(HCED* ed, volatile HCED** queue)
	{
		if (*queue == 0)
			return false;
		if (*queue == (volatile HCED*)ed)
		{
			*queue = (volatile HCED*)ed->Next;	// At head of queue
			return true;
		}

		volatile HCED* head = *queue;
		while (head)
		{
			if (head->Next == (u32)ed)
			{
				head->Next = ed->Next;
				return true;
			}
			head = (volatile HCED*)head->Next;
		}
		return false;
	}

	void Release(Endpoint* endpoint)
	{
		if (endpoint->CurrentState == Endpoint::NotQueued)
		{
			// Never event used it, nothing to do
		}
		else
		{
			HCED* ed = (HCED*)endpoint;
			ed->Control |= 0x4000;	// SKIP
			switch (endpoint->Flags & 0x03)
			{
				case ENDPOINT_CONTROL:
					Remove(ed,(volatile HCED**)&LPC_USB->HcControlHeadED);
					break;
				case ENDPOINT_BULK:
					Remove(ed,(volatile HCED**)&LPC_USB->HcBulkHeadED);
					break;
				case ENDPOINT_INTERRUPT:
					for (int i = 0; i < 32; i++)
						Remove(ed,(volatile HCED**)&CommunicationArea.InterruptTable[i]);
					break;
			}

			u16 fn = CommunicationArea.FrameNumber;
			while (fn == CommunicationArea.FrameNumber)
				;	// Wait for next frame

		}

		//	In theory, the endpoint is now dead.
		//	TODO: Will Callbacks ever be pending? BUGBUG
		memset(endpoint,0,sizeof(Endpoint));
	}

	//	  Pop the last TD from the list
	HCTD* Reverse(HCTD* current) 
	{ 
		HCTD *result = NULL,*temp; 
		while (current) 
		{ 
			temp = (HCTD*)current->Next; 
			current->Next = (u32)result;
			result = current;
			current = temp;
		}
		return result;
	}

	//	  Called from interrupt...
	//	  Control endpoints use a state machine to progress through the transfers
	void ProcessDoneQueue(u32 tdList)
	{
		HCTD* list = Reverse((HCTD*)tdList);
		while (list)
		{
			Endpoint* endpoint = (Endpoint*)(list-1);
			list = (HCTD*)list->Next;
			int ep = endpoint->Address();
			bool in = endpoint->Flags & 0x80;
			int status = (endpoint->TDHead.Control >> 28) & 0xF;

			//LOG("ProcessDoneQueue %02X %08X\n",ep,endpoint->TDHead.Control);

			if (status != 0)
			{
				LOG("ProcessDoneQueue status %02X %d\n",ep,status);
				endpoint->CurrentState = Endpoint::Idle;
			} else {
				switch (endpoint->CurrentState)
				{
					case Endpoint::SetupQueued:
						if (endpoint->Length == 0)
							Transfer(endpoint,in ? TOKEN_OUT : TOKEN_IN,0,0,Endpoint::StatusQueued);	// Skip Data Phase
						else
							Transfer(endpoint,in ? TOKEN_IN : TOKEN_OUT,endpoint->Data,endpoint->Length, Endpoint::DataQueued);    // Setup is done, now Data
						break;

					case Endpoint::DataQueued:
						if (endpoint->TDHead.CurrBufPtr)
							endpoint->Length = endpoint->TDHead.CurrBufPtr - (u32)endpoint->Data;

						if (ep == 0)
							Transfer(endpoint,in ? TOKEN_OUT : TOKEN_IN,0,0,Endpoint::StatusQueued);	// Data is done, now Status, Control only
						else
							endpoint->CurrentState = Endpoint::Idle;
						break;

					case Endpoint::StatusQueued:	// Transaction is done
						endpoint->CurrentState = Endpoint::Idle;
						break;
				}
			}

			//	  Complete, flag if we need a callback
			if (endpoint->Callback && endpoint->CurrentState == Endpoint::Idle)
			{
				endpoint->CurrentState = Endpoint::CallbackPending;
				_callbacksPending++;
			}
		}
	}

	//	Hack to reset devices that don't want to connect
	int AddDevice(int hub, int port, bool isLowSpeed)
	{
		int device = AddDeviceCore(hub,port,isLowSpeed);
        if (device < 0)
		{
			LOG("========RETRY ADD DEVICE========\n");	// This will go for ever.. TODO power cycle root?
            Disconnect(hub,port);	// Could not read descriptor at assigned address, reset this port and try again
            ResetPort(hub,port);	// Cheap bluetooth dongles often need this on a hotplug
            return -1;
        }
		return device;
	}

	int AddDeviceCore(int hub, int port, bool isLowSpeed)
	{
		int lowSpeed = isLowSpeed ? 0x2000 : 0;
		DeviceDescriptor desc;
		EndpointZero.EndpointDescriptor.Control = (8 << 16) | lowSpeed;			   // MaxPacketSize == 8
		int r = GetDescriptor(0,DESCRIPTOR_TYPE_DEVICE,0,(u8*)&desc,8);
		if (r < 0)
		{
			LOG("FAILED TO LOAD DESCRIPTOR FOR DEVICE 0\n");
			return r;
		}

		EndpointZero.EndpointDescriptor.Control = (desc.bMaxPacketSize << 16) | lowSpeed;	 // Actual MaxPacketSize
		r = GetDescriptor(0,DESCRIPTOR_TYPE_DEVICE,0,(u8*)&desc,sizeof(desc));
		if (r < 0)
			return r;

		LOG("\nClass %02X found %04X:%04X\n\n",desc.bDeviceClass,desc.idVendor,desc.idProduct);

		//	  Now assign the device an address, move off EndpointZero
		int device = 0;
		for (int i = 0; i < MAX_DEVICES; i++)
		{
			if (Devices[i].Port == 0)
			{
				device = i+1;
				break;
			}
		}
		if (!device)
			return ERR_DEVICE_NONE_LEFT;

		r = SetAddress(0,device);
		if (r)
			return r;
		DelayMS(2);
		
		// Now at a nonzero address, create control endpoint
		Device* dev = &Devices[device-1];
		dev->Init(&desc,hub,port,device,lowSpeed);
		AddEndpoint(device,0,ENDPOINT_CONTROL,desc.bMaxPacketSize,0);
		_connectPending = 0;

		//	Verify this all works
		r = GetDescriptor(device,DESCRIPTOR_TYPE_DEVICE,0,(u8*)&desc,sizeof(desc));
		if (r < 0)
			return r;

		//	Set to interface 0 by default
		//	Calls LoadDevice if interface is found
		r = SetConfigurationAndInterface(device,1,0,&desc);

		if (desc.bDeviceClass == CLASS_HUB)
			InitHub(device);			// Handle hubs in this code

		return device;
	}

	// Walk descriptors and create endpoints for a given device
	// TODO configuration !=1, alternate settings etc.
	int SetConfigurationAndInterface(int device, int configuration, int interfaceNumber, DeviceDescriptor* desc)
	{
		u8 buffer[255];
		int err = GetDescriptor(device,DESCRIPTOR_TYPE_CONFIGURATION,0,buffer,sizeof(buffer));
		if (err < 0)
			return err;

		err = SetConfiguration(device,configuration);
		if (err < 0)
			return err;

		//	Add the endpoints for this interface
		int len = buffer[2] | (buffer[3] << 8);
		u8* d = buffer;
		u8* end = d + len;
		InterfaceDescriptor* found = 0;
		while (d < end)
		{
			if (d[1] == DESCRIPTOR_TYPE_INTERFACE)
			{
				InterfaceDescriptor* id = (InterfaceDescriptor*)d;
				if (id->bInterfaceNumber == interfaceNumber)
				{
					found = id;
					d += d[0];
					while (d < end && d[1] != DESCRIPTOR_TYPE_INTERFACE)
					{
						switch (d[1])
						{
							case DESCRIPTOR_TYPE_ENDPOINT:
								AddEndpoint(device,(EndpointDescriptor*)d);
								break;
							default:
								LOG("Skipping descriptor %02X (%d bytes)\n",d[1],d[0]);
						}
						d += d[0];
					}
				}
			}
			d += d[0];
		}

		if (!found)
			return ERR_INTERFACE_NOT_FOUND;
		OnLoadDevice(device,desc,found);
		return 0;
	}

	void Init()
	{
		LOG("USB INIT (Controller is %d bytes)\n",sizeof(*this));
		memset(this,0,sizeof(HostController));
		EndpointZero.CurrentState = Endpoint::NotQueued;
		HWInit(&CommunicationArea);
		DelayMS(10);
	}

	void ResetPort(int hub, int port)
	{
		LOG("ResetPort Hub:%d Port:%d\n",hub,port);
		_connectPending++;			// Only reset/add 1 device at a time
		if (hub == 0)
			LPC_USB->HcRhPortStatus1 = PortResetStatus;	// Reset Root Hub, port 1
		else
			SetPortReset(hub,port);	// or reset other hub
	}

	void Disconnect(int hub, int port)
	{
		LOG("Disconnect Hub:%d Port:%d\n",hub,port);	// Mark a device for destruction
		for (int i = 0; i < MAX_DEVICES; i++)
		{
			Device* dev = Devices + i;
			if (dev->Port == port && dev->Hub == hub)
			{
				//	Disconnect everything that is attached to this device if it is a hub
				for (int p = 0; p < dev->HubPortCount; p++)
					Disconnect(i+1,p+1);

				//	Now release endpoints
				for (int j = 1; j < MAX_ENDPOINTS_PER_DEVICE*2; j += 2)
				{
					u8 endpointIndex = dev->_endpointMap[j];
					if (endpointIndex != 0xFF)
						Release(Endpoints + endpointIndex);
				}
				dev->Port = 0;	// Device is now free
				dev->Flags = 0;
				return;
			}
		}
	}

	// called after reset
	void Connect(int hub, int port, bool lowspeed)
	{
		LOG("Connect Hub:%d Port:%d %s\n",hub,port,lowspeed ? "slow" : "full");
		AddDevice(hub,port,lowspeed);
	}

	// Called from interrupt
	void HubStatusChange(int hub, int port, u32 status)
	{
		LOG("HubStatusChange Hub:%d Port:%d %08X\n",hub,port,status);
		if (status & ConnectStatusChange)
		{
			if (status & CurrentConnectStatus)	// Connecting
				ResetPort(hub,port);			// Reset to initiate connect (state machine?)
			else
				Disconnect(hub,port);
		}

		if (status & PortResetStatusChange)
		{
			if (!(status & PortResetStatus))
			{
				_connectCountdown = 200;		// Schedule a connection in 200ms
				if (status & LowspeedDevice)
					port |= 0x80;
				_connectHub = hub;
				_connectPort = port;
			}
		}
	}

	#define HOST_CLK_EN		(1<<0)
	#define PORTSEL_CLK_EN	(1<<3)
	#define AHB_CLK_EN		(1<<4)
	#define CLOCK_MASK		(HOST_CLK_EN | PORTSEL_CLK_EN | AHB_CLK_EN)

	#define  FRAMEINTERVAL		(12000-1)	// 1ms
	#define  DEFAULT_FMINTERVAL	((((6 * (FRAMEINTERVAL - 210)) / 7) << 16) | FRAMEINTERVAL)

	void DelayMS(int ms)
	{
		u16 f = ms + CommunicationArea.FrameNumber;
		while (f != CommunicationArea.FrameNumber)
			;
	}

	static void HWInit(HCCA* cca)
	{
		NVIC_DisableIRQ(USB_IRQn);
		
		// turn on power for USB
		LPC_SC->PCONP		|= (1UL<<31);
		// Enable USB host clock, port selection and AHB clock
		LPC_USB->USBClkCtrl |= CLOCK_MASK;
		// Wait for clocks to become available
		while ((LPC_USB->USBClkSt & CLOCK_MASK) != CLOCK_MASK)
			;
		
		//	We are a Host
		LPC_USB->OTGStCtrl |= 1;
		LPC_USB->USBClkCtrl &= ~PORTSEL_CLK_EN;				// we don't need port selection clock until we do OTG
		
		// configure USB pins
		LPC_PINCON->PINSEL1 &= ~((3<<26)|(3<<28));	
		LPC_PINCON->PINSEL1 |=	((1<<26)|(1<<28));			// USB D+/D-
			
		LPC_PINCON->PINSEL3 &= ~((3 << 6) | (3 << 22));		// USB_PPWR, USB_OVRCR
		LPC_PINCON->PINSEL3 |= ((2 << 6) | (2 << 22));
		
		LPC_PINCON->PINSEL4 &= ~(3 << 18);					// USB_CONNECT
		LPC_PINCON->PINSEL4 |= (1 << 18);

		//	Reset OHCI block
		LPC_USB->HcControl		 = 0;
		LPC_USB->HcControlHeadED = 0;
		LPC_USB->HcBulkHeadED	 = 0;
		
		LPC_USB->HcCommandStatus = HostControllerReset;
		LPC_USB->HcFmInterval	 = DEFAULT_FMINTERVAL;
		LPC_USB->HcPeriodicStart = FRAMEINTERVAL*90/100;

		LPC_USB->HcControl	= (LPC_USB->HcControl & (~HostControllerFunctionalState)) | OperationalMask;
		LPC_USB->HcRhStatus = SetGlobalPower;
		
		LPC_USB->HcHCCA = (u32)cca;
		LPC_USB->HcInterruptStatus |= LPC_USB->HcInterruptStatus;
		LPC_USB->HcInterruptEnable = MasterInterruptEnable | WritebackDoneHead | RootHubStatusChange | FrameNumberOverflow;

		NVIC_SetPriority(USB_IRQn, 0);
		NVIC_EnableIRQ(USB_IRQn);
		while (cca->FrameNumber < 10)
			;	// 10ms delay before diving in
	}
};

//====================================================================================
//====================================================================================
//	  Host controller instance and Interrupt handler

static HostController _controller __attribute__((at(USB_RAM_BASE)));

extern "C" void USB_IRQHandler(void) __irq;
void USB_IRQHandler (void) __irq
{
	u32 int_status = LPC_USB->HcInterruptStatus;

	if (int_status & RootHubStatusChange)	//	Root hub status change
		_controller._rootHubStatusChange++;	//	Just flag the controller, will be processed in USBLoop

	u32 head = 0;
	if (int_status & WritebackDoneHead)
	{
		head = _controller.CommunicationArea.DoneHead;		// Writeback Done
		_controller.CommunicationArea.DoneHead = 0;
	}			 
	LPC_USB->HcInterruptStatus = int_status;

	if (head)
	   _controller.ProcessDoneQueue(head);	 // TODO - low bit can be set BUGBUG
}

//====================================================================================
//====================================================================================
//	  API Methods

void USBInit()
{
	return _controller.Init();
}

void USBLoop()
{
	return _controller.Loop();
}

u8* USBGetBuffer(u32* len)
{
	*len = USB_RAM_SIZE - sizeof(HostController);
	return _controller.SRAM;
}

static Setup* GetSetup(int device)
{
	if (device == 0)
		return &_controller._setupZero;
	
	if (device < 1 || device > MAX_DEVICES)
		return 0;
	return &_controller.Devices[device-1].SetupBuffer;
}

//	Loop until IO on endpoint is complete
static int WaitIODone(Endpoint* endpoint)
{
	if (endpoint->CurrentState == Endpoint::NotQueued)
		return 0;
	while (endpoint->CurrentState != Endpoint::Idle)
		USBLoop();	// May generate callbacks, mount or unmount devices etc
	int status = endpoint->Status();
	if (status == 0)
		return endpoint->Length;
	return -status;
}

int USBTransfer(int device, int ep, u8 flags, u8* data, int length, USBCallback callback, void* userData)
{
	Endpoint* endpoint = _controller.GetEndpoint(device,ep);
	if (!endpoint)
		return ERR_ENDPOINT_NOT_FOUND;
		
	WaitIODone(endpoint);
	endpoint->Flags = flags;
	endpoint->Data = data;
	endpoint->Length = length;
	endpoint->Callback = callback;
	endpoint->UserData = userData;
	if (ep == 0)
		_controller.Transfer(endpoint,TOKEN_SETUP,(u8*)GetSetup(device),8,Endpoint::SetupQueued);
	else
		_controller.Transfer(endpoint,flags & 0x80 ? TOKEN_IN : TOKEN_OUT,data,length,Endpoint::DataQueued);
	if (callback)
		return IO_PENDING;
	return WaitIODone(endpoint);
}

int USBControlTransfer(int device, int request_type, int request, int value, int index, u8* data, int length, USBCallback callback, void * userData)
{
	Setup* setup = GetSetup(device);
	if (!setup)
		return ERR_DEVICE_NOT_FOUND;
		
	// Async control calls may overwrite setup buffer of previous call, so we need to wait before setting up next call
	WaitIODone(_controller.GetEndpoint(device,0));
	
	setup->bm_request_type = request_type;
	setup->b_request = request;
	setup->w_value = value;
	setup->w_index = index;
	setup->w_length = length;
	return USBTransfer(device,0,request_type & DEVICE_TO_HOST,data,length,callback,userData);
}

int  USBInterruptTransfer(int device, int ep, u8* data, int length, USBCallback callback, void* userData)
{
	return USBTransfer(device,ep,(ep & 0x80) | ENDPOINT_INTERRUPT,data,length,callback,userData);
}

int  USBBulkTransfer(int device, int ep, u8* data, int length, USBCallback callback, void* userData)
{
	return USBTransfer(device,ep,(ep & 0x80) | ENDPOINT_BULK,data,length,callback,userData);
}

int GetDescriptor(int device, int descType,int descIndex, u8* data, int length)
{
	return USBControlTransfer(device,DEVICE_TO_HOST | RECIPIENT_DEVICE, GET_DESCRIPTOR,(descType << 8)|(descIndex), 0, data, length, 0);
}

int GetString(int device, int index, char* dst, int length)
{
    u8 buffer[255];
    int le = GetDescriptor(device,DESCRIPTOR_TYPE_STRING,index,buffer,sizeof(buffer));
    if (le < 0)
		return le;
	if (length < 1)
		return -1;
	length <<= 1;
	if (le > length)
		le = length;
    for (int j = 2; j < le; j += 2)
        *dst++ = buffer[j];
    *dst = 0;
	return (le>>1)-1;
}

int SetAddress(int device, int new_addr)
{
	return USBControlTransfer(device,HOST_TO_DEVICE | RECIPIENT_DEVICE, SET_ADDRESS, new_addr, 0, 0, 0, 0);
}

int SetConfiguration(int device, int configNum)
{
	return USBControlTransfer(device,HOST_TO_DEVICE | RECIPIENT_DEVICE, SET_CONFIGURATION, configNum, 0, 0, 0, 0);
}

int SetInterface(int device, int ifNum, int altNum)
{
	return USBControlTransfer(device,HOST_TO_DEVICE | RECIPIENT_INTERFACE, SET_INTERFACE, altNum, ifNum, 0, 0, 0);
}

//	HUB stuff
int SetPortFeature(int device, int feature, int index)
{
    return USBControlTransfer(device,HOST_TO_DEVICE | REQUEST_TYPE_CLASS | RECIPIENT_OTHER,SET_FEATURE,feature,index,0,0);
}

int ClearPortFeature(int device, int feature, int index)
{
    return USBControlTransfer(device,HOST_TO_DEVICE | REQUEST_TYPE_CLASS | RECIPIENT_OTHER,CLEAR_FEATURE,feature,index,0,0);
}

int SetPortPower(int device, int port)
{
    int r = SetPortFeature(device,PORT_POWER,port);
    _controller.DelayMS(20);	// 80ms to turn on a hubs power... DESCRIPTOR? todo
    return r;
}

int SetPortReset(int device, int port)
{
    return SetPortFeature(device,PORT_RESET,port);
}

int GetPortStatus(int device, int port, u32* status)
{
    return USBControlTransfer(device,DEVICE_TO_HOST | REQUEST_TYPE_CLASS | RECIPIENT_OTHER,GET_STATUS,0,port,(u8*)status,4);
}
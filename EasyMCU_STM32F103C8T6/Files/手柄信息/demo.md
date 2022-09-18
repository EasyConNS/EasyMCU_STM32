| **Connection Status**     | Device connected |
| ------------------------- | ---------------- |
| **Current Configuration** | 1                |
| **Speed**                 | Full (12 Mbit/s) |
| **Device Address**        | 6                |
| **Number Of Open Pipes**  | 1                |

**Device Descriptor**  NANcoin

| Offset | Field              | Size | Value | Description                   |
| ------ | ------------------ | ---- | ----- | ----------------------------- |
| 0      | bLength            | 1    | 12h   |                               |
| 1      | bDescriptorType    | 1    | 01h   | Device                        |
| 2      | bcdUSB             | 2    | 0200h | USB Spec 2.0                  |
| 4      | bDeviceClass       | 1    | 00h   | Class info in Ifc Descriptors |
| 5      | bDeviceSubClass    | 1    | 00h   |                               |
| 6      | bDeviceProtocol    | 1    | 00h   |                               |
| 7      | bMaxPacketSize0    | 1    | 40h   | 64 bytes                      |
| 8      | idVendor           | 2    | 04F3h | Elan Microelectronics Corp.   |
| 10     | idProduct          | 2    | 0104h |                               |
| 12     | bcdDevice          | 2    | 0200h | 2.00                          |
| 14     | iManufacturer      | 1    | 01h   | "STMicroelectronics"          |
| 15     | iProduct           | 1    | 02h   | "NANcoin......."              |
| 16     | iSerialNumber      | 1    | 03h   |                               |
| 17     | bNumConfigurations | 1    | 01h   |                               |

**Configuration Descriptor 1** 

| Offset | Field                                           | Size | Value    | Description   |
| ------ | ----------------------------------------------- | ---- | -------- | ------------- |
| 0      | bLength                                         | 1    | 09h      |               |
| 1      | bDescriptorType                                 | 1    | 02h      | Configuration |
| 2      | wTotalLength                                    | 2    | 0022h    |               |
| 4      | bNumInterfaces                                  | 1    | 01h      |               |
| 5      | bConfigurationValue                             | 1    | 01h      |               |
| 6      | iConfiguration                                  | 1    | 00h      |               |
| 7      | bmAttributes                                    | 1    | C0h      | Self Powered  |
|        | 4..0: Reserved                                  |      | ...00000 |               |
|        | 5: Remote Wakeup                                |      | ..0..... | No            |
|        | 6: Self Powered                                 |      | .1...... | Yes           |
|        | 7: Reserved (set to one) (bus-powered for  1.0) |      | 1....... |               |
| 8      | bMaxPower                                       | 1    | 32h      | 100 mA        |

**Interface Descriptor 0/0** HID,  1 Endpoint

| Offset | Field              | Size | Value | Description    |
| ------ | ------------------ | ---- | ----- | -------------- |
| 0      | bLength            | 1    | 09h   |                |
| 1      | bDescriptorType    | 1    | 04h   | Interface      |
| 2      | bInterfaceNumber   | 1    | 00h   |                |
| 3      | bAlternateSetting  | 1    | 00h   |                |
| 4      | bNumEndpoints      | 1    | 01h   |                |
| 5      | bInterfaceClass    | 1    | 03h   | HID            |
| 6      | bInterfaceSubClass | 1    | 01h   | Boot Interface |
| 7      | bInterfaceProtocol | 1    | 00h   |                |
| 8      | iInterface         | 1    | 00h   |                |

**HID Descriptor** 

| Offset | Field             | Size | Value | Description |
| ------ | ----------------- | ---- | ----- | ----------- |
| 0      | bLength           | 1    | 09h   |             |
| 1      | bDescriptorType   | 1    | 21h   | HID         |
| 2      | bcdHID            | 2    | 0100h | 1.00        |
| 4      | bCountryCode      | 1    | 00h   |             |
| 5      | bNumDescriptors   | 1    | 01h   |             |
| 6      | bDescriptorType   | 1    | 22h   | Report      |
| 7      | wDescriptorLength | 2    | 004Ah | 74 bytes    |

**Endpoint Descriptor 81** 1 In,  Interrupt, 32 ms

| Offset | Field               | Size | Value    | Description |
| ------ | ------------------- | ---- | -------- | ----------- |
| 0      | bLength             | 1    | 07h      |             |
| 1      | bDescriptorType     | 1    | 05h      | Endpoint    |
| 2      | bEndpointAddress    | 1    | 81h      | 1 In        |
| 3      | bmAttributes        | 1    | 03h      | Interrupt   |
|        | 1..0: Transfer Type |      | ......11 | Interrupt   |
|        | 7..2: Reserved      |      | 000000.. |             |
| 4      | wMaxPacketSize      | 2    | 0005h    | 5 bytes     |
| 6      | bInterval           | 1    | 20h      | 32 ms       |

**Interface 0 HID Report  Descriptor** Game Pad

| Item Tag (Value)                                | Raw Data |
| ----------------------------------------------- | -------- |
| Usage Page (Generic Desktop)                    | 05 01    |
| Usage (Game Pad)                                | 09 05    |
| Collection (Application)                        | A1 01    |
| Usage (Pointer)                                 | 09 01    |
| Collection (Physical)                           | A1 00    |
| Usage (X)                                       | 09 30    |
| Usage (Y)                                       | 09 31    |
| Logical Minimum (0)                             | 15 00    |
| Logical Maximum (255)                           | 26 FF 00 |
| Physical Minimum (0)                            | 35 00    |
| Physical Maximum (32767)                        | 46 FF 7F |
| Report Size (8)                                 | 75 08    |
| Report Count (2)                                | 95 02    |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit) | 81 02    |
| End Collection                                  | C0       |
| Usage (Slider)                                  | 09 36    |
| Logical Minimum (0)                             | 15 00    |
| Logical Maximum (255)                           | 26 FF 00 |
| Physical Minimum (0)                            | 35 00    |
| Physical Maximum (32767)                        | 46 FF 7F |
| Report Size (8)                                 | 75 08    |
| Report Count (1)                                | 95 01    |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit) | 81 02    |
| Usage (Hat Switch)                              | 09 39    |
| Logical Minimum (1)                             | 15 01    |
| Logical Maximum (8)                             | 25 08    |
| Report Size (4)                                 | 75 04    |
| Report Count (1)                                | 95 01    |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,Null,Bit) | 81 42    |
| Usage Page (Button)                             | 05 09    |
| Usage Minimum (Button 1)                        | 19 01    |
| Usage Maximum (Button 12)                       | 29 0C    |
| Report Size (1)                                 | 75 01    |
| Report Count (12)                               | 95 0C    |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit) | 81 02    |
| End Collection                                  | C0       |
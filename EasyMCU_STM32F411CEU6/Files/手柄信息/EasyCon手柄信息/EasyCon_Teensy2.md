| **Connection Status**     | Device connected |
| ------------------------- | ---------------- |
| **Current Configuration** | 1                |
| **Speed**                 | Full (12 Mbit/s) |
| **Device Address**        | 4                |
| **Number Of Open Pipes**  | 2                |

**Device Descriptor** MCU  CONTROLLER

| Offset | Field              | Size | Value | Description                   |
| ------ | ------------------ | ---- | ----- | ----------------------------- |
| 0      | bLength            | 1    | 12h   |                               |
| 1      | bDescriptorType    | 1    | 01h   | Device                        |
| 2      | bcdUSB             | 2    | 0200h | USB Spec 2.0                  |
| 4      | bDeviceClass       | 1    | 00h   | Class info in Ifc Descriptors |
| 5      | bDeviceSubClass    | 1    | 00h   |                               |
| 6      | bDeviceProtocol    | 1    | 00h   |                               |
| 7      | bMaxPacketSize0    | 1    | 40h   | 64 bytes                      |
| 8      | idVendor           | 2    | 0F0Dh | Hori Co., Ltd                 |
| 10     | idProduct          | 2    | 0092h |                               |
| 12     | bcdDevice          | 2    | 0100h | 1.00                          |
| 14     | iManufacturer      | 1    | 01h   | "ELMAGNIFICO"                 |
| 15     | iProduct           | 1    | 02h   | "MCU CONTROLLER"              |
| 16     | iSerialNumber      | 1    | 00h   |                               |
| 17     | bNumConfigurations | 1    | 01h   |                               |

**Configuration Descriptor 1**  Bus Powered, 500 mA

| Offset | Field                                           | Size | Value    | Description                |
| ------ | ----------------------------------------------- | ---- | -------- | -------------------------- |
| 0      | bLength                                         | 1    | 09h      |                            |
| 1      | bDescriptorType                                 | 1    | 02h      | Configuration              |
| 2      | wTotalLength                                    | 2    | 0029h    |                            |
| 4      | bNumInterfaces                                  | 1    | 01h      |                            |
| 5      | bConfigurationValue                             | 1    | 01h      |                            |
| 6      | iConfiguration                                  | 1    | 00h      |                            |
| 7      | bmAttributes                                    | 1    | A0h      | Bus Powered, Remote Wakeup |
|        | 4..0: Reserved                                  |      | ...00000 |                            |
|        | 5: Remote Wakeup                                |      | ..1..... | Yes                        |
|        | 6: Self Powered                                 |      | .0...... | No, Bus Powered            |
|        | 7: Reserved (set to one) (bus-powered for  1.0) |      | 1....... |                            |
| 8      | bMaxPower                                       | 1    | FAh      | 500 mA                     |

**Interface Descriptor 0/0** HID,  2 Endpoints

| Offset | Field              | Size | Value | Description |
| ------ | ------------------ | ---- | ----- | ----------- |
| 0      | bLength            | 1    | 09h   |             |
| 1      | bDescriptorType    | 1    | 04h   | Interface   |
| 2      | bInterfaceNumber   | 1    | 00h   |             |
| 3      | bAlternateSetting  | 1    | 00h   |             |
| 4      | bNumEndpoints      | 1    | 02h   |             |
| 5      | bInterfaceClass    | 1    | 03h   | HID         |
| 6      | bInterfaceSubClass | 1    | 00h   |             |
| 7      | bInterfaceProtocol | 1    | 00h   |             |
| 8      | iInterface         | 1    | 00h   |             |

**HID Descriptor** 

| Offset | Field             | Size | Value | Description |
| ------ | ----------------- | ---- | ----- | ----------- |
| 0      | bLength           | 1    | 09h   |             |
| 1      | bDescriptorType   | 1    | 21h   | HID         |
| 2      | bcdHID            | 2    | 0111h | 1.11        |
| 4      | bCountryCode      | 1    | 00h   |             |
| 5      | bNumDescriptors   | 1    | 01h   |             |
| 6      | bDescriptorType   | 1    | 22h   | Report      |
| 7      | wDescriptorLength | 2    | 0056h | 86 bytes    |

**Endpoint Descriptor 02** 2 Out,  Interrupt, 1 ms

| Offset | Field               | Size | Value    | Description |
| ------ | ------------------- | ---- | -------- | ----------- |
| 0      | bLength             | 1    | 07h      |             |
| 1      | bDescriptorType     | 1    | 05h      | Endpoint    |
| 2      | bEndpointAddress    | 1    | 02h      | 2 Out       |
| 3      | bmAttributes        | 1    | 03h      | Interrupt   |
|        | 1..0: Transfer Type |      | ......11 | Interrupt   |
|        | 7..2: Reserved      |      | 000000.. |             |
| 4      | wMaxPacketSize      | 2    | 0040h    | 64 bytes    |
| 6      | bInterval           | 1    | 01h      | 1 ms        |

**Endpoint Descriptor 81** 1 In,  Interrupt, 1 ms

| Offset | Field               | Size | Value    | Description |
| ------ | ------------------- | ---- | -------- | ----------- |
| 0      | bLength             | 1    | 07h      |             |
| 1      | bDescriptorType     | 1    | 05h      | Endpoint    |
| 2      | bEndpointAddress    | 1    | 81h      | 1 In        |
| 3      | bmAttributes        | 1    | 03h      | Interrupt   |
|        | 1..0: Transfer Type |      | ......11 | Interrupt   |
|        | 7..2: Reserved      |      | 000000.. |             |
| 4      | wMaxPacketSize      | 2    | 0040h    | 64 bytes    |
| 6      | bInterval           | 1    | 01h      | 1 ms        |

**Interface 0 HID Report  Descriptor** Game Pad

| Item Tag (Value)                                      | Raw Data |
| ----------------------------------------------------- | -------- |
| Usage Page (Generic Desktop)                          | 05 01    |
| Usage (Game Pad)                                      | 09 05    |
| Collection (Application)                              | A1 01    |
| Logical Minimum (0)                                   | 15 00    |
| Logical Maximum (1)                                   | 25 01    |
| Physical Minimum (0)                                  | 35 00    |
| Physical Maximum (1)                                  | 45 01    |
| Report Size (1)                                       | 75 01    |
| Report Count (16)                                     | 95 10    |
| Usage Page (Button)                                   | 05 09    |
| Usage Minimum (Button 1)                              | 19 01    |
| Usage Maximum (Button 16)                             | 29 10    |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)       | 81 02    |
| Usage Page (Generic Desktop)                          | 05 01    |
| Logical Maximum (7)                                   | 25 07    |
| Physical Maximum (315)                                | 46 3B 01 |
| Report Size (4)                                       | 75 04    |
| Report Count (1)                                      | 95 01    |
| Unit (Eng Rot: Degree)                                | 65 14    |
| Usage (Hat Switch)                                    | 09 39    |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,Null,Bit)       | 81 42    |
| Unit (None)                                           | 65 00    |
| Report Count (1)                                      | 95 01    |
| **Input** (Cnst,Ary,Abs)                              | 81 01    |
| Logical Maximum (255)                                 | 26 FF 00 |
| Physical Maximum (255)                                | 46 FF 00 |
| Usage (X)                                             | 09 30    |
| Usage (Y)                                             | 09 31    |
| Usage (Z)                                             | 09 32    |
| Usage (Rz)                                            | 09 35    |
| Report Size (8)                                       | 75 08    |
| Report Count (4)                                      | 95 04    |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)       | 81 02    |
| Usage Page (Vendor-Defined 1)                         | 06 00 FF |
| Usage (Vendor-Defined 32)                             | 09 20    |
| Report Count (1)                                      | 95 01    |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)       | 81 02    |
| Usage (Vendor-Defined 9761)                           | 0A 21 26 |
| Report Count (8)                                      | 95 08    |
| **Output** (Data,Var,Abs,NWrp,Lin,Pref,NNul,NVol,Bit) | 91 02    |
| End Collection                                        | C0       |
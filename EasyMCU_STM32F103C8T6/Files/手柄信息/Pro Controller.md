| **Connection Status**     | Device connected |
| ------------------------- | ---------------- |
| **Current Configuration** | 1                |
| **Speed**                 | Full (12 Mbit/s) |
| **Device Address**        | 4                |
| **Number Of Open Pipes**  | 2                |

**Device Descriptor** Pro  Controller

| Offset | Field              | Size | Value | Description                   |
| ------ | ------------------ | ---- | ----- | ----------------------------- |
| 0      | bLength            | 1    | 12h   |                               |
| 1      | bDescriptorType    | 1    | 01h   | Device                        |
| 2      | bcdUSB             | 2    | 0200h | USB Spec 2.0                  |
| 4      | bDeviceClass       | 1    | 00h   | Class info in Ifc Descriptors |
| 5      | bDeviceSubClass    | 1    | 00h   |                               |
| 6      | bDeviceProtocol    | 1    | 00h   |                               |
| 7      | bMaxPacketSize0    | 1    | 40h   | 64 bytes                      |
| 8      | idVendor           | 2    | 057Eh | Nintendo Co., Ltd             |
| 10     | idProduct          | 2    | 2009h |                               |
| 12     | bcdDevice          | 2    | 0210h | 2.10                          |
| 14     | iManufacturer      | 1    | 01h   | "Nintendo Co., Ltd."          |
| 15     | iProduct           | 1    | 02h   | "Pro Controller"              |
| 16     | iSerialNumber      | 1    | 03h   | "000000000001"                |
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
| 7      | wDescriptorLength | 2    | 00CBh | 203 bytes   |

**Endpoint Descriptor 81** 1 In,  Interrupt, 8 ms

| Offset | Field               | Size | Value    | Description |
| ------ | ------------------- | ---- | -------- | ----------- |
| 0      | bLength             | 1    | 07h      |             |
| 1      | bDescriptorType     | 1    | 05h      | Endpoint    |
| 2      | bEndpointAddress    | 1    | 81h      | 1 In        |
| 3      | bmAttributes        | 1    | 03h      | Interrupt   |
|        | 1..0: Transfer Type |      | ......11 | Interrupt   |
|        | 7..2: Reserved      |      | 000000.. |             |
| 4      | wMaxPacketSize      | 2    | 0040h    | 64 bytes    |
| 6      | bInterval           | 1    | 08h      | 8 ms        |

**Endpoint Descriptor 01** 1 Out,  Interrupt, 8 ms

| Offset | Field               | Size | Value    | Description |
| ------ | ------------------- | ---- | -------- | ----------- |
| 0      | bLength             | 1    | 07h      |             |
| 1      | bDescriptorType     | 1    | 05h      | Endpoint    |
| 2      | bEndpointAddress    | 1    | 01h      | 1 Out       |
| 3      | bmAttributes        | 1    | 03h      | Interrupt   |
|        | 1..0: Transfer Type |      | ......11 | Interrupt   |
|        | 7..2: Reserved      |      | 000000.. |             |
| 4      | wMaxPacketSize      | 2    | 0040h    | 64 bytes    |
| 6      | bInterval           | 1    | 08h      | 8 ms        |

**Interface 0 HID Report  Descriptor** Joystick

| Item Tag (Value)                                     | Raw Data       |
| ---------------------------------------------------- | -------------- |
| Usage Page (Generic Desktop)                         | 05 01          |
| Logical Minimum (0)                                  | 15 00          |
| Usage (Joystick)                                     | 09 04          |
| Collection (Application)                             | A1 01          |
| Report ID (48)                                       | 85 30          |
| Usage Page (Generic Desktop)                         | 05 01          |
| Usage Page (Button)                                  | 05 09          |
| Usage Minimum (Button 1)                             | 19 01          |
| Usage Maximum (Button 10)                            | 29 0A          |
| Logical Minimum (0)                                  | 15 00          |
| Logical Maximum (1)                                  | 25 01          |
| Report Size (1)                                      | 75 01          |
| Report Count (10)                                    | 95 0A          |
| Unit Exponent (0)                                    | 55 00          |
| Unit (None)                                          | 65 00          |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)      | 81 02          |
| Usage Page (Button)                                  | 05 09          |
| Usage Minimum (Button 11)                            | 19 0B          |
| Usage Maximum (Button 14)                            | 29 0E          |
| Logical Minimum (0)                                  | 15 00          |
| Logical Maximum (1)                                  | 25 01          |
| Report Size (1)                                      | 75 01          |
| Report Count (4)                                     | 95 04          |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)      | 81 02          |
| Report Size (1)                                      | 75 01          |
| Report Count (2)                                     | 95 02          |
| **Input** (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Bit)      | 81 03          |
| Usage (Generic Desktop:Pointer)                      | 0B 01 00 01 00 |
| Collection (Physical)                                | A1 00          |
| Usage (Generic Desktop:X)                            | 0B 30 00 01 00 |
| Usage (Generic Desktop:Y)                            | 0B 31 00 01 00 |
| Usage (Generic Desktop:Z)                            | 0B 32 00 01 00 |
| Usage (Generic Desktop:Rz)                           | 0B 35 00 01 00 |
| Logical Minimum (0)                                  | 15 00          |
| Logical Maximum (65535)                              | 27 FF FF 00 00 |
| Report Size (16)                                     | 75 10          |
| Report Count (4)                                     | 95 04          |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)      | 81 02          |
| End Collection                                       | C0             |
| Usage (Generic Desktop:Hat Switch)                   | 0B 39 00 01 00 |
| Logical Minimum (0)                                  | 15 00          |
| Logical Maximum (7)                                  | 25 07          |
| Physical Minimum (0)                                 | 35 00          |
| Physical Maximum (315)                               | 46 3B 01       |
| Unit (Eng Rot: Degree)                               | 65 14          |
| Report Size (4)                                      | 75 04          |
| Report Count (1)                                     | 95 01          |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)      | 81 02          |
| Usage Page (Button)                                  | 05 09          |
| Usage Minimum (Button 15)                            | 19 0F          |
| Usage Maximum (Button 18)                            | 29 12          |
| Logical Minimum (0)                                  | 15 00          |
| Logical Maximum (1)                                  | 25 01          |
| Report Size (1)                                      | 75 01          |
| Report Count (4)                                     | 95 04          |
| **Input** (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)      | 81 02          |
| Report Size (8)                                      | 75 08          |
| Report Count (52)                                    | 95 34          |
| **Input** (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Bit)      | 81 03          |
| Usage Page (Vendor-Defined 1)                        | 06 00 FF       |
| Report ID (33)                                       | 85 21          |
| Usage (Vendor-Defined 1)                             | 09 01          |
| Report Size (8)                                      | 75 08          |
| Report Count (63)                                    | 95 3F          |
| **Input** (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Bit)      | 81 03          |
| Report ID (129)                                      | 85 81          |
| Usage (Vendor-Defined 2)                             | 09 02          |
| Report Size (8)                                      | 75 08          |
| Report Count (63)                                    | 95 3F          |
| **Input** (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Bit)      | 81 03          |
| Report ID (1)                                        | 85 01          |
| Usage (Vendor-Defined 3)                             | 09 03          |
| Report Size (8)                                      | 75 08          |
| Report Count (63)                                    | 95 3F          |
| **Output** (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Vol,Bit) | 91 83          |
| Report ID (16)                                       | 85 10          |
| Usage (Vendor-Defined 4)                             | 09 04          |
| Report Size (8)                                      | 75 08          |
| Report Count (63)                                    | 95 3F          |
| **Output** (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Vol,Bit) | 91 83          |
| Report ID (128)                                      | 85 80          |
| Usage (Vendor-Defined 5)                             | 09 05          |
| Report Size (8)                                      | 75 08          |
| Report Count (63)                                    | 95 3F          |
| **Output** (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Vol,Bit) | 91 83          |
| Report ID (130)                                      | 85 82          |
| Usage (Vendor-Defined 6)                             | 09 06          |
| Report Size (8)                                      | 75 08          |
| Report Count (63)                                    | 95 3F          |
| **Output** (Cnst,Var,Abs,NWrp,Lin,Pref,NNul,Vol,Bit) | 91 83          |
| End Collection                                       | C0             |
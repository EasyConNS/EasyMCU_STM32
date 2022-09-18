/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */
  0x05, 0x01,               //Usage Page (Generic Desktop)
  0x09, 0x05,               //Usage (Game Pad)
  0xA1, 0x01,               //Collection (Application)
  0x15, 0x00,               //    Logical Minimum (0)
  0x25, 0x01,               //    Logical Maximum (1)
  0x35, 0x00,               //    Physical Minimum (0)
  0x45, 0x01,               //    Physical Maximum (1)
  0x75, 0x01,               //    Report Size (1)
  0x95, 0x10,               //    Report Count (16)
  0x05, 0x09,               //    Usage Page (Button)
  0x19, 0x01,               //    Usage Minimum (Button 1)
  0x29, 0x10,               //    Usage Maximum (Button 16)
  0x81, 0x02,               //    Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
  0x05, 0x01,               //    Usage Page (Generic Desktop)
  0x25, 0x07,               //    Logical Maximum (7)
  0x46, 0x3B, 0x01,         //    Physical Maximum (315)
  0x75, 0x04,               //    Report Size (4)
  0x95, 0x01,               //    Report Count (1)
  0x65, 0x14,               //    Unit (Eng Rot: Degree)
  0x09, 0x39,               //    Usage (Hat Switch)
  0x81, 0x42,               //    Input (Data,Var,Abs,NWrp,Lin,Pref,Null,Bit)
  0x65, 0x00,               //    Unit (None)
  0x95, 0x01,               //    Report Count (1)
  0x81, 0x01,               //    Input (Cnst,Ary,Abs)
  0x26, 0xFF, 0x00,         //    Logical Maximum (255)
  0x46, 0xFF, 0x00,         //    Physical Maximum (255)
  0x09, 0x30,               //    Usage (X)
  0x09, 0x31,               //    Usage (Y)
  0x09, 0x32,               //    Usage (Z)
  0x09, 0x35,               //    Usage (Rz)
  0x75, 0x08,               //    Report Size (8)
  0x95, 0x04,               //    Report Count (4)
  0x81, 0x02,               //    Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
  0x06, 0x00, 0xFF,         //    Usage Page (Vendor-Defined 1)
  0x09, 0x20,               //    Usage (Vendor-Defined 32)
  0x95, 0x01,               //    Report Count (1)
  0x81, 0x02,               //    Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
  0x0A, 0x21, 0x26,         //    Usage (Vendor-Defined 9761)
  0x95, 0x08,               //    Report Count (8)
  0x91, 0x02,               //    Output (Data,Var,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  UNUSED(event_idx);
  UNUSED(state);

  /* Start next USB packet transfer once data processing is completed */
  USBD_CUSTOM_HID_ReceivePacket(&hUsbDeviceFS);

  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : usb_desc.h
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Descriptor Header for Mass Storage Device
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "HAL_device.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define MASS_SIZ_DEVICE_DESC              18
#define MASS_SIZ_CONFIG_DESC              32

#define MASS_SIZ_STRING_LANGID            4
#define MASS_SIZ_STRING_VENDOR            46
#define MASS_SIZ_STRING_PRODUCT           38
#define MASS_SIZ_STRING_SERIAL            26
#define MASS_SIZ_STRING_INTERFACE         16

/* Exported functions ------------------------------------------------------- */
extern const uint8_t MASS_DeviceDescriptor[MASS_SIZ_DEVICE_DESC];
extern const uint8_t MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC];

extern const uint8_t MASS_StringLangID[MASS_SIZ_STRING_LANGID];
extern const uint8_t MASS_StringVendor[MASS_SIZ_STRING_VENDOR];
extern const uint8_t MASS_StringProduct[MASS_SIZ_STRING_PRODUCT];
extern uint8_t MASS_StringSerial[MASS_SIZ_STRING_SERIAL];
extern const uint8_t MASS_StringInterface[MASS_SIZ_STRING_INTERFACE];

#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/



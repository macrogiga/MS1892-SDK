/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : usb_mem.h
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Utility prototypes functions for memory/PMA transfers
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_MEM_H
#define __USB_MEM_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UserToPMABufferCopy(uint8_t *pbUsrBuf, uint16_t EPNum, uint16_t wNBytes);
void PMAToUserBufferCopy(uint8_t *pbUsrBuf, uint16_t EPNum, uint16_t wNBytes);

/* External variables --------------------------------------------------------*/

#endif  /*__USB_MEM_H*/

/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/

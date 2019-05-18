/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : usb_regs.h
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Interface prototype functions to USB cell registers
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_REGS_H
#define __USB_REGS_H

/* endpoints enumeration */
#define ENDP0   ((uint8_t)0)
#define ENDP1   ((uint8_t)1)
#define ENDP2   ((uint8_t)2)
#define ENDP3   ((uint8_t)3)
#define ENDP4   ((uint8_t)4)

/* Exported macro ------------------------------------------------------------*/


#define _SetUSB_Connect  	            USB_OTG_FS->OTG_CTRL |= 0x80
#define _SetUSB_Disconnect  	        USB_OTG_FS->OTG_CTRL &= ~0x80
/* SetUSB_ISTR */
#define _ClrUSB_INT_STA(wRegValue)  	USB_OTG_FS->INT_STAT = wRegValue
/* GetUSB_ISTR */
#define _GetUSB_INT_STA()  	           USB_OTG_FS->INT_STAT

#define _ClrUSB_EPn_HALT(n)  	    USB_OTG_FS->EP_CTL[n] &= ~(1<<1);
#define _GetUSB_EPn_HALT(n)  	    ((USB_OTG_FS->EP_CTL[n]>>1) & 0x01)

/* GetEP_INT_STA */
#define _GetEP_INT_STA()  	            (USB_OTG_FS->STAT)//(USB->rEP_INT_STATE)

/* Set_EP_EN */
#define _SetUSB_ADDR(wRegValue)	        USB_OTG_FS->ADDR = wRegValue //(USB->rADDR = (uint16_t)wRegValue)

 
/* External variables --------------------------------------------------------*/
extern __IO uint16_t wIstr;  /* ISTR register last read value */

/* Exported functions ------------------------------------------------------- */
void EndPointInit( USB_OTG_BDT_TypeDef *pUSB_OTG_BDT,u32 dataBufAdd);

#endif /* __USB_REGS_H */

/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/

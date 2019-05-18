/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : usb_mem.c
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Utility functions for memory transfers to/from PMA
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"

extern USB_OTG_BDT_TypeDef *pUSB_OTG_BDT;
extern u8 rxUsbBufOdd[16] ;
extern u8 txUsbBufOdd[16] ;
extern u8 epInDataNum[16] ;
extern u8 epOutDataNum[16];
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : UserToPMABufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf: pointer to user memory area.
*                  - wPMABufAddr: address into PMA.
*                  - wNBytes: no. of bytes to be copied.
* Output         : None.
* Return         : None	.
*******************************************************************************/
void UserToPMABufferCopy(uint8_t *pbUsrBuf, uint16_t EPNum, uint16_t wNBytes)
{
    uint32_t i;
    u8 *pBuf;
    

    pBuf = (u8*)((pUSB_OTG_BDT+EPNum)->TX_BUF[txUsbBufOdd[EPNum]].ADRESS) ;
  
    (pUSB_OTG_BDT+EPNum)->TX_BUF[txUsbBufOdd[EPNum]].FORMAT &= 0x00ff00;

    for (i = 0;i < wNBytes;  i++)
    {
        pBuf[i] = *pbUsrBuf;
        pbUsrBuf++;
    }   
    
    (pUSB_OTG_BDT+EPNum)->TX_BUF[txUsbBufOdd[EPNum]].FORMAT |= (wNBytes<<16)|(epInDataNum[EPNum]<<6)/**/;
    epInDataNum[EPNum] = !epInDataNum[EPNum];
    (pUSB_OTG_BDT+EPNum)->TX_BUF[txUsbBufOdd[EPNum]].FORMAT |= 1<<7;
    txUsbBufOdd[EPNum] = !txUsbBufOdd[EPNum];    
}
/*******************************************************************************
* Function Name  : PMAToUserBufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf    = pointer to user memory area.
*                  - wPMABufAddr = address into PMA.
*                  - wNBytes     = no. of bytes to be copied.
* Output         : None.
* Return         : None.
*******************************************************************************/
void PMAToUserBufferCopy(uint8_t *pbUsrBuf, uint16_t EPNum, uint16_t wNBytes)
{
  uint32_t i;
  u8 *pBuf;

  pBuf = (u8*)((pUSB_OTG_BDT+EPNum)->RX_BUF[rxUsbBufOdd[EPNum]].ADRESS) ;

  for (i = 0;i < wNBytes;  i++)
  {
      *pbUsrBuf = pBuf[i];
      pbUsrBuf++;
  }   
  
  (pUSB_OTG_BDT+EPNum)->RX_BUF[rxUsbBufOdd[EPNum]].FORMAT &= 0x00ff00;//清除计数,清除stall
  (pUSB_OTG_BDT+EPNum)->RX_BUF[rxUsbBufOdd[EPNum]].FORMAT |= (64<<16);//设置最大接收字节
  (pUSB_OTG_BDT+EPNum)->RX_BUF[rxUsbBufOdd[EPNum]].FORMAT |= 1<<7;
}

/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/

/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : usb_regs.c
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Interface functions to USB cell registers
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void EndPointInit( USB_OTG_BDT_TypeDef *pUSB_OTG_BDT,u32 dataBufAdd)
{
  int i;
  pUSB_OTG_BDT->RX_BUF[0].FORMAT = 0;
  pUSB_OTG_BDT->RX_BUF[0].ADRESS = dataBufAdd;
  pUSB_OTG_BDT->RX_BUF[0].FORMAT |= 0x40<<16;//长度64byte
  pUSB_OTG_BDT->RX_BUF[0].FORMAT |= 1<<7;

  pUSB_OTG_BDT->RX_BUF[1].FORMAT = 0;
  pUSB_OTG_BDT->RX_BUF[1].FORMAT |= 0x40<<16;//长度64byte
  pUSB_OTG_BDT->RX_BUF[1].ADRESS = pUSB_OTG_BDT->RX_BUF[0].ADRESS+0x40;
  pUSB_OTG_BDT->RX_BUF[1].FORMAT |= 1<<6;
  pUSB_OTG_BDT->RX_BUF[1].FORMAT |= 1<<7;

  pUSB_OTG_BDT->TX_BUF[0].FORMAT = 0x00;
  pUSB_OTG_BDT->TX_BUF[0].ADRESS = pUSB_OTG_BDT->RX_BUF[0].ADRESS+0x80;    

  pUSB_OTG_BDT->TX_BUF[1].FORMAT = 0x00;
  pUSB_OTG_BDT->TX_BUF[1].ADRESS = pUSB_OTG_BDT->RX_BUF[0].ADRESS+0xC0;
  
  for(i = 0;i < 64;i ++)
  {
    *(u8*)(pUSB_OTG_BDT->TX_BUF[0].ADRESS + i) = 0;
    *(u8*)(pUSB_OTG_BDT->TX_BUF[1].ADRESS + i) = 0;
    *(u8*)(pUSB_OTG_BDT->RX_BUF[0].ADRESS + i) = 0;
    *(u8*)(pUSB_OTG_BDT->RX_BUF[1].ADRESS + i) = 0;
  }
}
/*******************************************************************************
* Function Name  : ToWord
* Description    : merge two byte in a word.
* Input          : bh: byte high, bl: bytes low.
* Output         : None.
* Return         : resulted word.
*******************************************************************************/
uint16_t ToWord(uint8_t bh, uint8_t bl)
{
  uint16_t wRet;
  wRet = (uint16_t)bl | ((uint16_t)bh << 8);
  return(wRet);
}
/*******************************************************************************
* Function Name  : ByteSwap
* Description    : Swap two byte in a word.
* Input          : wSwW: word to Swap.
* Output         : None.
* Return         : resulted word.
*******************************************************************************/
uint16_t ByteSwap(uint16_t wSwW)
{
  uint8_t bTemp;
  uint16_t wRet;
  bTemp = (uint8_t)(wSwW & 0xff);
  wRet =  (wSwW >> 8) | ((uint16_t)bTemp << 8);
  return(wRet);
}

/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/

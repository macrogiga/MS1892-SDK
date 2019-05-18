/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : usb_int.c
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Endpoint CTR (Low and High) interrupt's service routines
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
extern void (*pEpInt_IN[7])(void);    /*  Handles IN  interrupts   */
extern void (*pEpInt_OUT[7])(void);   /*  Handles OUT interrupts   */
extern USB_OTG_BDT_TypeDef *pUSB_OTG_BDT;
/* Private function prototypes -----------------------------------------------*/
USB_OTG_BDT_TypeDef *pUSB_OTG_BDT;
u8 rxUsbBufOdd[16] ;
u8 txUsbBufOdd[16] ;

u8 epInDataNum[16] ;

u8 setupPacket[8] ;
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CTR_LP.
* Description    : Low priority Endpoint Correct Transfer interrupt's service
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_LP(void)
{
  uint32_t wEpIntSta = 0,wEpxIntSta = 0;
  uint32_t wEpStat;
  u8 txUsbBufOddTemp;

  wEpStat = _GetEP_INT_STA();
  _ClrUSB_INT_STA(OTG_FS_INT_STAT_DNE) ;
  wEpIntSta = (wEpStat>>4)&0xf;           //获取中断端点号
  
  if(wEpIntSta == 0)       //端点0
  {
      EPindex = 0;

      if(((wEpStat>>3)&0x01) == 0)//RX
      {
        rxUsbBufOdd[0] = (wEpStat>>2)&0x1;
        wEpxIntSta = pUSB_OTG_BDT->RX_BUF[rxUsbBufOdd[0]].FORMAT;
      }
      else
      {
        txUsbBufOddTemp = (wEpStat>>2)&0x1;
        wEpxIntSta = pUSB_OTG_BDT->TX_BUF[txUsbBufOddTemp].FORMAT;
        pUSB_OTG_BDT->TX_BUF[txUsbBufOddTemp].FORMAT = 0;
      }
      wEpxIntSta = (wEpxIntSta >> 2)&0xf;

      if(wEpxIntSta == 0x0D)//setup
      {
        USB_OTG_FS->CTL &= ~(1<<5);//tip
        epInDataNum[0] = 1;
        Setup0_Process();      
        return;
      }
      
      if(wEpxIntSta == 0x01)//|EPn_INT_STATE_OUTNACK
      {

        Out0_Process();
        return;
      }

      if(wEpxIntSta == 0x9)//|EPn_INT_STATE_ACK
      {

        In0_Process();
          
        return;
      }      
  }/* if(EPindex == 0) */
  else if(wEpIntSta != 0)
  {
    EPindex = wEpIntSta;
    if(((wEpStat>>3)&0x01) == 0)//RX
    {
      rxUsbBufOdd[wEpIntSta] = (wEpStat>>2)&0x1;
      wEpxIntSta = (pUSB_OTG_BDT+EPindex)->RX_BUF[rxUsbBufOdd[EPindex]].FORMAT;
    }
    else
    {
      txUsbBufOddTemp = (wEpStat>>2)&0x1;
      wEpxIntSta = (pUSB_OTG_BDT + EPindex)->TX_BUF[txUsbBufOddTemp].FORMAT;
      (pUSB_OTG_BDT+EPindex)->TX_BUF[txUsbBufOddTemp].FORMAT = 0;
    }
    wEpxIntSta = (wEpxIntSta >> 2)&0xf;
    if(wEpxIntSta == 0x01)//|EPn_INT_STATE_OUTNACK
    {
      (*pEpInt_OUT[EPindex-1])(); 
    }
    if(wEpxIntSta == 0x9)//|EPn_INT_STATE_ACK
    {
      (*pEpInt_IN[EPindex-1])();
    }
     
  }/* if(EPindex != 0)*/
    
}

/*******************************************************************************
* Function Name  : CTR_HP.
* Description    : High Priority Endpoint Correct Transfer interrupt's service 
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_HP(void)
{

}

/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/

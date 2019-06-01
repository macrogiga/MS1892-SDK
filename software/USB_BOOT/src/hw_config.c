/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : hw_config.c
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "HAL_device.h"
#include "hw_config.h"
#include "usb_conf.h"
#include "mass_mal.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_lib.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
     initialize the PLL and update the SystemFrequency variable. */


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);


	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //key
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //led
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void CRS_USB_Config(void)
{
    RCC->APB1ENR |= 0x08000000;
	CRS->CFGR&=~0xffff;
	CRS->CFGR|=0xBB7F;

    CRS->CFGR &= 0xcfffffff; //
    /*Enables the automatic hardware adjustment of TRIM bits: AUTOTRIMEN:*/
    CRS->CFGR |= 0x2<<28; //GPIO
    CRS->CR |= 0x40; //autoen
    /*Enables the oscillator clock for frequency error counter CEN*/
    CRS->CR |= 0x20; //cen
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div3);

  /* Enable USB clock */

  RCC->AHB2ENR |= 0x1<<7;
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }

}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USB_OTG_FS_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/*******************************************************************************
* Function Name  : Led_Config
* Description    : configure the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;


  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_8|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : Led_RW_ON
* Description    : Turn ON the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_RW_ON(void)
{
}

/*******************************************************************************
* Function Name  : Led_RW_OFF
* Description    : Turn off the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_RW_OFF(void)
{
}
/*******************************************************************************
* Function Name  : USB_Configured_LED
* Description    : Turn ON the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Configured_LED(void)
{
}

/*******************************************************************************
* Function Name  : USB_NotConfigured_LED
* Description    : Turn off the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_NotConfigured_LED(void)
{
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
      _SetUSB_Connect;
  }
  else
  {
      _SetUSB_Disconnect;
  }
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

  if (Device_Serial0 != 0)
  {
    MASS_StringSerial[2] = (uint8_t)(Device_Serial0 & 0x000000FF);
    MASS_StringSerial[4] = (uint8_t)((Device_Serial0 & 0x0000FF00) >> 8);
    MASS_StringSerial[6] = (uint8_t)((Device_Serial0 & 0x00FF0000) >> 16);
    MASS_StringSerial[8] = (uint8_t)((Device_Serial0 & 0xFF000000) >> 24);

    MASS_StringSerial[10] = (uint8_t)(Device_Serial1 & 0x000000FF);
    MASS_StringSerial[12] = (uint8_t)((Device_Serial1 & 0x0000FF00) >> 8);
    MASS_StringSerial[14] = (uint8_t)((Device_Serial1 & 0x00FF0000) >> 16);
    MASS_StringSerial[16] = (uint8_t)((Device_Serial1 & 0xFF000000) >> 24);

    MASS_StringSerial[18] = (uint8_t)(Device_Serial2 & 0x000000FF);
    MASS_StringSerial[20] = (uint8_t)((Device_Serial2 & 0x0000FF00) >> 8);
    MASS_StringSerial[22] = (uint8_t)((Device_Serial2 & 0x00FF0000) >> 16);
    MASS_StringSerial[24] = (uint8_t)((Device_Serial2 & 0xFF000000) >> 24);
  }
}

/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/

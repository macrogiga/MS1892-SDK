/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : MM32x103_it.c
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
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
#include "usb_lib.h"
#include "usb_istr.h"
#include "sdcard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
#include "mm32flash.h"

extern unsigned char Boot_value;

void NMI_Handler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+8)); AppEntry();}
void HardFault_Handler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+12)); AppEntry();}
void MemManage_Handler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+16)); AppEntry();}
void BusFault_Handler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+20)); AppEntry();}
void UsageFault_Handler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+24)); AppEntry();}
void SVC_Handler(void)   
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+44)); AppEntry();}
void DebugMon_Handler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+48)); AppEntry();}
void PendSV_Handler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+56)); AppEntry();}
void SysTick_Handler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+60)); AppEntry();}
void WWDG_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+64)); AppEntry();}
void PVD_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+68)); AppEntry();}
void TAMPER_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+72)); AppEntry();}
void RTC_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+76)); AppEntry();}
void FLASH_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+80)); AppEntry();}
void RCC_CRS_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+84)); AppEntry();}
void EXTI0_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+88)); AppEntry();}
void EXTI1_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+92)); AppEntry();}
void EXTI2_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+96)); AppEntry();}
void EXTI3_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+100)); AppEntry();}
void EXTI4_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+104)); AppEntry();}
void DMA1_Channel1_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+108)); AppEntry();}
void DMA1_Channel2_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+112)); AppEntry();}
void DMA1_Channel3_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+116)); AppEntry();}
void DMA1_Channel4_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+120)); AppEntry();}
void DMA1_Channel5_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+124)); AppEntry();}
void DMA1_Channel6_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+128)); AppEntry();}
void DMA1_Channel7_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+132)); AppEntry();}
void ADC1_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+136)); AppEntry();}
void CAN1_RX_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+148)); AppEntry();}
void EXTI9_5_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+156)); AppEntry();}
void TIM1_BRK_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+160)); AppEntry();}
void TIM1_UP_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+164)); AppEntry();}
void TIM1_TRG_COM_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+168)); AppEntry();}
void TIM1_CC_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+172)); AppEntry();}
void TIM2_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+176)); AppEntry();}
void TIM3_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+180)); AppEntry();}
void TIM4_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+184)); AppEntry();}
void I2C1_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+188)); AppEntry();}
void I2C2_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+196)); AppEntry();}
void SPI1_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+204)); AppEntry();}
void SPI2_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+208)); AppEntry();}
void UART1_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+212)); AppEntry();}
void UART2_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+216)); AppEntry();}
void UART3_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+220)); AppEntry();}
void EXTI15_10_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+224)); AppEntry();}
void RTCAlarm_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+228)); AppEntry();}
void USB_WKUP_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+232)); AppEntry();}
void TIM8_BRK_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+236)); AppEntry();}
void TIM8_UP_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+240)); AppEntry();}
void TIM8_TRG_COM_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+244)); AppEntry();}
void TIM8_CC_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+248)); AppEntry();}
void SDIO_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+260)); AppEntry();}
void TIM5_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+264)); AppEntry();}
void SPI3_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+268)); AppEntry();}
void UART4_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+272)); AppEntry();}
void UART5_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+276)); AppEntry();}
void TIM6_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+280)); AppEntry();}
void TIM7_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+284)); AppEntry();}
void DMA2_Channel1_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+288)); AppEntry();}
void DMA2_Channel2_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+292)); AppEntry();}
void DMA2_Channel3_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+296)); AppEntry();}
void DMA2_Channel4_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+300)); AppEntry();}
void DMA2_Channel5_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+304)); AppEntry();}
void ETHERNET_MAC_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+308)); AppEntry();}
void COMP1_2_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+320)); AppEntry();}
void USB_OTG_FS_IRQHandler(void)
{
    volatile void (*AppEntry)(void);
    
    if (Boot_value)
    {
        AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+332));
        AppEntry();
    }
    else
    {
        USB_Istr();
    }
}
void UART6_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+348)); AppEntry();}
void AES_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+380)); AppEntry();}
void TRNG_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+384)); AppEntry();}
void UART7_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+392)); AppEntry();}
void UART8_IRQHandler(void)
{volatile void (*AppEntry)(void); AppEntry = (volatile void(*)())(*(uint32_t*)(APP_ENTRY_ADDRESS+396)); AppEntry();}
/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/

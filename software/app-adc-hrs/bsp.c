/*
    Copyright (c) 2019 Macrogiga Electronics Co., Ltd.

    Permission is hereby granted, free of charge, to any person 
    obtaining a copy of this software and associated documentation 
    files (the "Software"), to deal in the Software without 
    restriction, including without limitation the rights to use, copy, 
    modify, merge, publish, distribute, sublicense, and/or sell copies 
    of the Software, and to permit persons to whom the Software is 
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be 
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.
*/
#include "HAL_conf.h"
#include "spi.h"
#include "IRQ_RF.h"
#include "mg_api.h"
#include "iwdg.h"

__IO uint32_t SysTick_Count = 0;
/**
  * @brief  Get SysTick count
  * @param  None
  * @retval SysTick Count
  */
unsigned int GetSysTickCount(void) //porting api
{
    return SysTick_Count;
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    SysTick_Count++;
    
    ble_nMsRoutine();
}


/**
  * @brief  Initialize SysTick
  * @param  None
  * @retval None
  */
#define CONFIG_TICKS_PER_SECOND 1000
void SysTick_Configuration(void)
{
    /*选择外部时钟  HCLK/8*/
//    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    if (SysTick_Config(SystemCoreClock / CONFIG_TICKS_PER_SECOND))
    { 
        /* Capture error */ 
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);
}


/**
  * @brief  Delay function
  * @param  Delay time /ms
  * @retval None
  */
void delay(u16 systicktime)
{
    uint32_t tickstart = GetSysTickCount();
    uint32_t wait = systicktime;

    /* Add a period to guaranty minimum wait */
    if (wait < 0xFFFFFFFFU)
    {
        wait++;
    } 

    while((GetSysTickCount() - tickstart) < wait)
    {
    }
}

void BSP_Init(void)
{
    SPIM_Init(SPI3, 24); //SPI_BaudRatePrescaler_16,9Mhz
    IRQ_RF();
    
    SysTick_Configuration();
    
    /*复位时长约为6.4s*/
    Write_Iwdg_ON(IWDG_Prescaler_256, 4000); //1000=6.4s
}

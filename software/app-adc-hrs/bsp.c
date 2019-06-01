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

//实时时钟配置
//初始化RTC时钟,同时检测时钟是否工作正常
//BKP->DR1用于保存是否第一次配置的设置
//返回0:正常
//其他:错误代码
u8 RTC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //检查是不是第一次配置时钟
    u8 temp=0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
    PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
    
    BKP_DeInit();	//复位备份区域 
    RCC_LSICmd(ENABLE);//使能内部低速时钟
    
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)	//检查指定的RCC标志位设置与否,等待内部低速晶振就绪
    {
        temp++;
        delay(1);
    }
    if(temp>=250)
        return 1;//初始化时钟失败,晶振有问题
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//设置RTC时钟(RTCCLK),选择LSI作为RTC时钟 
    
    RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟
    
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
    NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    
    
    RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
    delay(1);
    RTC_WaitForSynchro();	//等待RTC寄存器同步 
    
    RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒钟中断
    RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
    delay(1);

    RTC_EnterConfigMode();// 允许配置
    RTC_SetPrescaler(39999); //设置RTC预分频的值，40k即1秒钟
    RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成

    return 0; //ok
}

void BSP_Init(void)
{
    SPIM_Init(SPI3, 24); //SPI_BaudRatePrescaler_16,9Mhz
    IRQ_RF();
    
    SysTick_Configuration();
    RTC_Init();
    /*复位时长约为6.4s*/
    Write_Iwdg_ON(IWDG_Prescaler_256, 4000); //1000=6.4s
}


//RTC时钟中断
//每秒触发一次  
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
    {
        RTC_ClearITPendingBit(RTC_IT_SEC);
        RTC_WaitForLastTask(); 
    }
}

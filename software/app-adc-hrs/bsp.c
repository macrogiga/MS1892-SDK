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
    /*ѡ���ⲿʱ��  HCLK/8*/
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

//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������
u8 RTC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //����ǲ��ǵ�һ������ʱ��
    u8 temp=0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
    PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
    
    BKP_DeInit();	//��λ�������� 
    RCC_LSICmd(ENABLE);//ʹ���ڲ�����ʱ��
    
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ��ڲ����پ������
    {
        temp++;
        delay(1);
    }
    if(temp>=250)
        return 1;//��ʼ��ʱ��ʧ��,����������
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//����RTCʱ��(RTCCLK),ѡ��LSI��ΪRTCʱ�� 
    
    RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��
    
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1λ,�����ȼ�3λ
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//��ռ���ȼ�0λ,�����ȼ�4λ
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
    NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    
    
    RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
    delay(1);
    RTC_WaitForSynchro();	//�ȴ�RTC�Ĵ���ͬ�� 
    
    RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC�����ж�
    RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
    delay(1);

    RTC_EnterConfigMode();// ��������
    RTC_SetPrescaler(39999); //����RTCԤ��Ƶ��ֵ��40k��1����
    RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������

    return 0; //ok
}

void BSP_Init(void)
{
    SPIM_Init(SPI3, 24); //SPI_BaudRatePrescaler_16,9Mhz
    IRQ_RF();
    
    SysTick_Configuration();
    RTC_Init();
    /*��λʱ��ԼΪ6.4s*/
    Write_Iwdg_ON(IWDG_Prescaler_256, 4000); //1000=6.4s
}


//RTCʱ���ж�
//ÿ�봥��һ��  
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//�����ж�
    {
        RTC_ClearITPendingBit(RTC_IT_SEC);
        RTC_WaitForLastTask(); 
    }
}

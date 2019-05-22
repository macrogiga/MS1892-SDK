#include "adc.h"

void ADC0_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    ADC1_SingleChannel(ADC_Channel_0);
}

/********************************************************************************************************
**������Ϣ ��void ADC1_SingleChannel(uint8_t ADC_Channel_x)      
**�������� ������ADC1����ת��ģʽ
**������� ��ADC_Channel_x , xΪ0~8
**������� ����
**    ��ע ��
********************************************************************************************************/
void ADC1_SingleChannel(uint8_t ADC_Channel_x)
{
    ADC_InitTypeDef  ADC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    /* Initialize the ADC_PRESCARE values */
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
    /* Initialize the ADC_Mode member */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
    /* Initialize the ADC_ContinuousConvMode member */
    //ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    /* Initialize the ADC_DataAlign member */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    /* Initialize the ADC_ExternalTrigConv member */
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//ADC1 ͨ��1����PA1
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /*��������ͨ��*/
    ADC_RegularChannelConfig(ADC1, DISABLE_ALL_CHANNEL , 0, 0); 
    /*ʹ��ѡ��ͨ��,�����������*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, ADC_SampleTime_13_5Cycles); 
    if(ADC_Channel_x == ADC_Channel_refvol)
    {
        /*1.2V�ο���ѹʹ��*/
        ADC_TempSensorVrefintCmd(ENABLE);
    }
    ADC_Cmd(ADC1, ENABLE); 
}

/********************************************************************************************************
**������Ϣ ��ADC1_SingleChannel_Get()       
**�������� ����ȡADC1ת������
**������� ��ADC_Channel_x , xΪ0~8
*puiADData ,ADC1ʵ��ת������
**������� ��ucStatus ,0 ��ʾ���ݻ�ȡʧ��,1 ��ʾ�ɹ�
********************************************************************************************************/
u16 ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)
{	
    u16 puiADData;
    
	/*ADCR�Ĵ�����ADSTλʹ�ܣ��������ת��*/
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==0);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	puiADData=ADC1->ADDATA&0xfff;
	return puiADData;
}

/********************************************************************************************************
**������Ϣ ��Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)     
**�������� ����ȡ����ADC1����ֵ��ƽ��ֵ
**������� ��ADC_Channel_x , xΪ0~8
**������� ��puiADDataΪADC������ֵ
********************************************************************************************************/
u16 Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)
{
	u32 temp_val=0;
	u8 t;
	u8 delay;
    
	for(t=0;t<times;t++)
	{
		temp_val+=ADC1_SingleChannel_Get(ADC_Channel_x);
		for(delay=0;delay<100;delay++);
	}
	return temp_val/times;
} 	

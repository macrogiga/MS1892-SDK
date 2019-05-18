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
#include "HAL_device.h"
#include "HAL_conf.h"
#include "spi.h"


unsigned char SPI_WriteRead(unsigned char SendData,unsigned char WriteFlag) //porting api
{
    SPI3->TXREG = SendData;
    while(!(SPI3->CSTAT & SPI_FLAG_TXEPT));
    while(!(SPI3->CSTAT & SPI_FLAG_RXAVL));
    return SPI3->RXREG;
}

void SPI_CS_Enable_(void) //porting api
{
    SPI3->SCSR &= SPI_CS_BIT0;
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
}

void SPI_CS_Disable_(void) //porting api
{
    SPI3->SCSR |= ~SPI_CS_BIT0;
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
}

unsigned char SPI_WriteBuf(unsigned char reg, unsigned char const *pBuf, unsigned char len)
{
    unsigned char result = 0;
    unsigned char i;
    
    SPI_CS_Enable_();
    
    SPI_WriteRead(reg, 1);
    
    for (i=0;i<len;i++)
    {
        SPI_WriteRead(*pBuf++, 1);
    }
    
    SPI_CS_Disable_();
    
    return result;
}

unsigned char SPI_ReadBuf(unsigned char reg, unsigned char *pBuf, unsigned char len)
{
    unsigned char result = 0;
    unsigned char i;
    
    SPI_CS_Enable_();
    
    result = SPI_WriteRead(reg, 1);
    
    for (i=0;i<len;i++)
    {
        *(pBuf+i) = SPI_WriteRead(0xff, 0);
    }
    
    SPI_CS_Disable_();
    
    return result;
}

/********************************************************************************************************
**函数信息 ：SPIM_TXEn(SPI_TypeDef* SPIx)
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXEn(SPI_TypeDef* SPIx)
{
    //Transmit Enable bit TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}

/********************************************************************************************************
**函数信息 ：SPIM_TXDisable(SPI_TypeDef* SPIx)
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXDisable(SPI_TypeDef* SPIx)
{
    //disable TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Tx);
}

/********************************************************************************************************
**函数信息 ：SPIM_RXEn(SPI_TypeDef* SPIx)
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXEn(SPI_TypeDef* SPIx)
{
    //enable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}

/********************************************************************************************************
**函数信息 ：SPIM_RXDisable(SPI_TypeDef* SPIx)
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXDisable(SPI_TypeDef* SPIx)
{
    //disable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Rx);
}

/********************************************************************************************************
**函数信息 ：SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)            
**功能描述 :可修改参数初始化SPI
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2  ;spi_baud_div
**输出参数 ：无
********************************************************************************************************/
void SPIM_Init(SPI_TypeDef* SPIx,unsigned short spi_baud_div)
{
    SPI_InitTypeDef     SPI_InitStructure;
    GPIO_InitTypeDef    GPIO_InitStructure;

    SPI_DeInit(SPIx);
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_13);    // CS
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_13);    // SCK
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_6);    // MOSI
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_14);    // MISO

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
    
    SPI_CS_Disable_();

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);

    SPI_Cmd(SPIx, ENABLE); //Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率
    SPIM_TXEn(SPIx);
    SPIM_RXEn(SPIx);
}

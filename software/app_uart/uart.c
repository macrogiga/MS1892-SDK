#include <stdio.h>
#include "HAL_conf.h"


u32 BaudRate = 115200;

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif 

#ifdef USE_IAR
PUTCHAR_PROTOTYPE
{
    while((UART1->CSR&UART_IT_TXIEN)==0);//循环发送,直到发送完毕   
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}

#else
#pragma import(__use_no_semihosting)
             
struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout;
#pragma weak fputc
 
void _sys_exit(int x)
{ 
    x = x; 
} 

int fputc(int ch, FILE *f)
{      
    while((UART1->CSR&UART_IT_TXIEN)==0);//循环发送,直到发送完毕   
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}

#endif 


void uart_init(void)
{
    /*GPIO端口设置*/
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /*使能UART1，GPIOA时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /*UART1 NVIC 配置*/     
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    /*抢占优先级3*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    /*子优先级3*/    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /*IRQ通道使能*/    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /*根据指定的参数初始化VIC寄存器*/
    NVIC_Init(&NVIC_InitStructure);
    
    /*UART 初始化设置*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

    /*串口波特率*/
    UART_InitStructure.UART_BaudRate = BaudRate;
    /*字长为8位数据格式*/    
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    /*一个停止位*/    
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    /*无奇偶校验位*/    
    UART_InitStructure.UART_Parity = UART_Parity_No;
    /*无硬件数据流控制*/    
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    /*收发模式*/
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    /*初始化串口1*/
    UART_Init(UART1, &UART_InitStructure);
    /*开启串口接受中断*/    
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    /*使能串口1*/    
    UART_Cmd(UART1, ENABLE);

    /*UART1_TX   GPIOA.9*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*复用推挽输出*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
    
    /*UART1_RX  GPIOA.10初始化*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    /*浮空输入*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
    
    NVIC_SetPriority(UART1_IRQn,1);
}

void ChangeBaudRate(void)
{
    UART_InitTypeDef UART_InitStructure;
    UART_Cmd(UART1, DISABLE);
    UART_InitStructure.UART_BaudRate = BaudRate;//串口波特率
    UART_Init(UART1, &UART_InitStructure); //初始化串口1
    UART_Cmd(UART1, ENABLE);
}

#define MAX_BUF_LEN 100
u8 rxBuf[MAX_BUF_LEN];
u8 BufRdPos = 0;
u8 BufWrPos = 0;

void reset_uart_buf(void)
{
    BufRdPos = 0;
    BufWrPos = 0;
}

void uart_2_ble(void)
{
    u8 num;
    u8 BufWrPos_bk = BufWrPos;
    
    if(BufRdPos == BufWrPos)return;//empty
    
    if(BufWrPos_bk > BufRdPos)
    {
        num = sconn_notifydata(rxBuf+BufRdPos,BufWrPos_bk-BufRdPos);
        BufRdPos += num;
    }
    else
    {
        num = sconn_notifydata(rxBuf+BufRdPos,MAX_BUF_LEN-BufRdPos);
        BufRdPos += num;
        
        BufRdPos %= MAX_BUF_LEN;
    }
}

void UART1_IRQHandler(void)                	//串口1中断服务程序
{
    if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET)  //接收中断
    {
        UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
        rxBuf[BufWrPos] = UART_ReceiveData(UART1); //fputc(rxBuf[BufWrPos],0);
        BufWrPos ++;//pls check over flow if any
        BufWrPos %= MAX_BUF_LEN;
    }
    if(UART_GetITStatus(UART1, UART_IT_TXIEN)  != RESET)
    {
        UART_ClearITPendingBit(UART1,UART_IT_TXIEN);
    }
}

void MouduleOutData(u8* data, u8 len)
{
    while(len)
    {
        fputc(*data,0);
        len--;
        data++;
    }
}

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
    while((UART1->CSR&UART_IT_TXIEN)==0);//ѭ������,ֱ���������   
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
    while((UART1->CSR&UART_IT_TXIEN)==0);//ѭ������,ֱ���������   
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}

#endif 


void uart_init(void)
{
    /*GPIO�˿�����*/
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /*ʹ��UART1��GPIOAʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /*UART1 NVIC ����*/     
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    /*��ռ���ȼ�3*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    /*�����ȼ�3*/    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /*IRQͨ��ʹ��*/    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /*����ָ���Ĳ�����ʼ��VIC�Ĵ���*/
    NVIC_Init(&NVIC_InitStructure);
    
    /*UART ��ʼ������*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

    /*���ڲ�����*/
    UART_InitStructure.UART_BaudRate = BaudRate;
    /*�ֳ�Ϊ8λ���ݸ�ʽ*/    
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    /*һ��ֹͣλ*/    
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    /*����żУ��λ*/    
    UART_InitStructure.UART_Parity = UART_Parity_No;
    /*��Ӳ������������*/    
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    /*�շ�ģʽ*/
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    /*��ʼ������1*/
    UART_Init(UART1, &UART_InitStructure);
    /*�������ڽ����ж�*/    
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    /*ʹ�ܴ���1*/    
    UART_Cmd(UART1, ENABLE);

    /*UART1_TX   GPIOA.9*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*�����������*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
    
    /*UART1_RX  GPIOA.10��ʼ��*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    /*��������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
    
    NVIC_SetPriority(UART1_IRQn,1);
}

void ChangeBaudRate(void)
{
    UART_InitTypeDef UART_InitStructure;
    UART_Cmd(UART1, DISABLE);
    UART_InitStructure.UART_BaudRate = BaudRate;//���ڲ�����
    UART_Init(UART1, &UART_InitStructure); //��ʼ������1
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

void UART1_IRQHandler(void)                	//����1�жϷ������
{
    if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET)  //�����ж�
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

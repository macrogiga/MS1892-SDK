#include "HAL_device.h"
#include "HAL_conf.h"



#define KEY_PIN_SET (GPIOA)
#define KEY_PIN_Src (GPIO_PinSource0)
#define KEY_PIN     (GPIO_Pin_0)

void key_init(void) //PA0
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_PinAFConfig(KEY_PIN_SET, KEY_PIN_Src, GPIO_AF_0);
    
    GPIO_InitStructure.GPIO_Pin  = KEY_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(KEY_PIN_SET, &GPIO_InitStructure);
}

u8 key_input(void)
{
    return (GPIO_ReadInputData(KEY_PIN_SET)>>KEY_PIN_Src) & 0x01;
}

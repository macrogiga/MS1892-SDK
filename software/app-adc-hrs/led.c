#include "HAL_device.h"
#include "HAL_conf.h"



#define LED_PIN_SET (GPIOB)
#define LED_PIN_Src (GPIO_PinSource8)
#define LED_PIN     (GPIO_Pin_8)

void led_init(void) //toggle test io init
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_PinAFConfig(LED_PIN_SET, LED_PIN_Src, GPIO_AF_0);
    
    GPIO_InitStructure.GPIO_Pin  = LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(LED_PIN_SET, &GPIO_InitStructure);
}

void led_toggle(void)//toggle test io
{
    static u8 _on_off_ = 0;
    
    _on_off_ = !_on_off_;
    if(_on_off_)
        GPIO_SetBits(LED_PIN_SET, LED_PIN);
    else
        GPIO_ResetBits(LED_PIN_SET, LED_PIN);
}

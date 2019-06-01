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
#include "mg_api.h"


extern void uart_init(void);
extern void BSP_Init(void);
extern void led_init(void);
extern void ADC0_Init(void);


const unsigned char AdvDat_HRS[]=
{
    0x02,0x01,0x06,
    0x03,0x19,0x41,0x03,
    0x07,0x03,0x0D,0x18,0x0A,0x18,0x0F,0x18
};
unsigned char  Boot_value __attribute__ ((at(0x20000000))) = 0x01;
int main (void)
{
    unsigned char *ble_mac_addr;
    
    BSP_Init();	
    ADC0_Init();

    uart_init();
    led_init();
    
    SetBleIntRunningMode();
    radio_initBle(TXPWR_0DBM, &ble_mac_addr);
    
    ble_set_adv_data((unsigned char *)AdvDat_HRS, sizeof(AdvDat_HRS));
    
    ble_run_interrupt_start(160*2); //ble adv interval 200ms
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    while(1)
    {
        //IrqMcuGotoSleepAndWakeup();
    }
    
    return 0;
}

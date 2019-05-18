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
extern void key_init(void);

int main (void)
{
    unsigned char *ble_mac_addr;
    
    BSP_Init();	

    uart_init();
    
    led_init();
    key_init();
    
    SetBleIntRunningMode();
   
    radio_initBle(TXPWR_0DBM, &ble_mac_addr);

#if 0 //SRRC test
    while(1)
    {
        test_SRRCCarrier(2, TXPWR_0DBM);
        while(0 == key_input()){Write_Iwdg_RL();}; //key release
        while(key_input()){};
        test_SRRCCarrier(26, TXPWR_0DBM);
        while(0 == key_input()){Write_Iwdg_RL();};
        while(key_input()){};
        test_SRRCCarrier(80, TXPWR_0DBM);
        while(0 == key_input()){Write_Iwdg_RL();};
        while(key_input()){};
        test_PRBS9(80, TXPWR_0DBM);
        while(0 == key_input()){Write_Iwdg_RL();};
        while(key_input()){};
        test_PRBS9(26, TXPWR_0DBM);
        while(0 == key_input()){Write_Iwdg_RL();};
        while(key_input()){};
        test_PRBS9(2, TXPWR_0DBM);
        while(0 == key_input()){Write_Iwdg_RL();};
        while(key_input()){};
    }
#endif

    ble_run_interrupt_start(160*2); //ble adv interval 200ms
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    while(1)
    {
        //IrqMcuGotoSleepAndWakeup();
    }
    
    return 0;
}

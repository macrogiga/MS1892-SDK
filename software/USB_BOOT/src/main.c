#include "HAL_device.h"
#include "HAL_pwr.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "mm32flash.h"


static void delay(uint32_t loop)
{
    __IO uint32_t i = 0;
    while(loop > 0){
        loop --;
        for(i = 14400; i != 0; i--)
        {
        }
    }
}

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
    register unsigned long AppStackPointer    __asm("msp");
    
    Set_System();
    
    if(PWR_GetFlagStatus(PWR_FLAG_WU) == 0){
        if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))  //press PA0 to enter boot
        {
            CRS_USB_Config();
            Set_USBClock();
            USB_Interrupts_Config();
            USB_Init();
            while (bDeviceState != CONFIGURED);
            
            while (1)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_8); //LEDon
                delay(500);
                GPIO_SetBits(GPIOB, GPIO_Pin_8);  //LEDoff
                delay(500);
            }
        }
    }
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);//led on
    
    AppStackPointer = *(unsigned long *)APP_ENTRY_ADDRESS;     //set sp
    ((void (*)(void))(*(unsigned long *)(APP_ENTRY_ADDRESS+4)))(); //set pc
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

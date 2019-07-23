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
    /*  HCLK/8*/
//    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    if (SysTick_Config(SystemCoreClock / CONFIG_TICKS_PER_SECOND))
    { 
        /* Capture error */ 
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);
}
void BSP_Init(void)
{
    SPIM_Init(SPI3, SPI_BaudRatePrescaler_16); //9Mhz
    IRQ_RF();
    
    SysTick_Configuration();
    
    /*6.4s*/
    Write_Iwdg_ON(IWDG_Prescaler_256, 4000); //1000=6.4s
}



/*
0~3KB BootCode
3~4KB UserData
4~258KB RunCode
258~512KB OTA
*/
#define FLASH_E2PROM_ADDR_BASE (0x08000000)
//#define FLASH_BOOT_ROM_SIZE    (3*1024)
//#define FLASH_E2PROM_ADDR_USR  (FLASH_E2PROM_ADDR_BASE + FLASH_BOOT_ROM_SIZE)
#define FLASH_E2PROM_ADDR_OTA  (FLASH_E2PROM_ADDR_BASE + 254*1024)

#define APPLICATION_ADDRESS     (uint32_t)0x08001010


void OtaSystemReboot(void)//porting api
{
    NVIC_SystemReset();
}

u32 GetOtaAddr(void)
{
    return FLASH_E2PROM_ADDR_OTA;
}


void WriteFlashE2PROM(u8* data, u16 len, u32 pos, u8 flag) //4 bytes aligned
{  
    u32 t;
    
    if(flag)FLASH_Unlock();
    
    while(len >= 4)
    {
        t = data[3]; t <<= 8;
        t |= data[2]; t <<= 8;
        t |= data[1]; t <<= 8;
        t |= data[0];
        FLASH_ProgramWord(pos, t);
        pos += 4;
        len -= 4;
        data += 4;
    }

    if(flag)FLASH_Lock(); 
}


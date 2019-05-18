/**
******************************************************************************
* @file    system_MM32F103xCxE_o.c
* @version V1.0.1
* @date    2018/09/17
* @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
* 
* 1.  This file provides two functions and one global variable to be called from 
*     user application:
*      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
*                      factors, AHB/APBx prescalers and Flash settings). 
*                      This function is called at startup just after reset and 
*                      before branch to main program. This call is made inside
*                      the "startup_MM32F103xCxE_o.s" file.
*
*      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
*                                  by the user application to setup the SysTick 
*                                  timer or configure other parameters.
*                                     
*      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
*                                 be called whenever the core clock is changed
*                                 during program execution.
*
* 2. After each device reset the HSI (8 MHz) is used as system clock source.
*    Then SystemInit() function is called, in "startup_MM32F103xCxE_o.s" file, to
*    configure the system clock before to branch to main program.
*
* 3. If the system clock source selected by user fails to startup, the SystemInit()
*    function will do nothing and HSI still used as system clock source. User can 
*    add some code to deal with this issue inside the SetSysClock() function.
*
* 4. The default value of HSE crystal is set to 8 MHz (or 25 MHz, depedning on
*    the product used), refer to "HSE_VALUE" define in "MM32F103xCxE_o.h" file. 
*    When HSE is used as system clock source, directly or through PLL, and you
*    are using different crystal you have to adapt the HSE value to your own
*    configuration.
*        
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MINDMOTION SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
******************************************************************************
*/

/** @addtogroup CMSIS
* @{
*/
#include "HAL_device.h"


/**
* @}
*/

/**
* @}
*/

/*!< Uncomment the line corresponding to the desired System clock (SYSCLK)
frequency (after reset the HSI is used as SYSCLK source)

IMPORTANT NOTE:
============== 
1. After each device reset the HSI is used as System clock source.

2. Please make sure that the selected System clock doesn't exceed your device's
maximum frequency.

3. If none of the define below is enabled, the HSI is used as System clock
source.

4. The System clock configuration functions provided within this file assume that:
- For Low, Medium and High density Value line devices an external 8MHz 
crystal is used to drive the System clock.
- For Low, Medium and High density devices an external 8MHz crystal is
used to drive the System clock.
- For Connectivity line devices an external 25MHz crystal is used to drive
the System clock.
If you are using different crystal you have to adapt those functions accordingly.
*/

//#define SYSCLK_FREQ_HSE     HSE_VALUE 

//#define SYSCLK_FREQ_XXMHz  96000000L

//#define SYSCLK_HSI_XXMHz   48000000L
#define SYSCLK_HSI_XXMHz  144000000L
/*!< Uncomment the following line if you need to relocate your vector Table in
Internal SRAM. */ 
//#define VECT_TAB_SRAM   0x20000000
#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field. 
This value must be a multiple of 0x200. */

/**
* @}
*/
#define PLL_SOURCE_HSI         0
#define PLL_SOURCE_HSE         1
#define PLL_SOURCE_HSE_DIV_2   2

#define SYS_CLOCK_HSI          0
#define SYS_CLOCK_HSE          1
#define SYS_CLOCK_PLL          2

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/
#ifdef SYSCLK_FREQ_HSE
#define SYSTEM_CLOCK            SYSCLK_FREQ_HSE;         /*!< System Clock Frequency (Core Clock) */
#define PLL_SOURCE              PLL_SOURCE_HSE
#define SYS_CLOCK_SRC           SYS_CLOCK_HSE

#elif defined SYSCLK_FREQ_XXMHz
#define SYSTEM_CLOCK            SYSCLK_FREQ_XXMHz;        /*!< System Clock Frequency (Core Clock) */
#define PLL_SOURCE              PLL_SOURCE_HSE
#define SYS_CLOCK_SRC           SYS_CLOCK_PLL


#elif defined SYSCLK_HSI_XXMHz
#define SYSTEM_CLOCK            SYSCLK_HSI_XXMHz;        /*!< System Clock Frequency (Core Clock) */
#define PLL_SOURCE              PLL_SOURCE_HSI
#define SYS_CLOCK_SRC           SYS_CLOCK_PLL

#else /*!< HSI Selected as System Clock source */
#define SYSTEM_CLOCK            HSI_VALUE_PLL_OFF;        /*!< System Clock Frequency (Core Clock) */
#define PLL_SOURCE              PLL_SOURCE_HSI
#define SYS_CLOCK_SRC           SYS_CLOCK_HSI
#endif

__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/**
* @}
*/

/**
* @brief  use to return the pllm&plln.
* @param  pllclkSourceFrq : PLL source clock frquency;
          pllclkFrq : Target PLL clock frquency;
		  plln : PLL factor PLLN
		  pllm : PLL factor PLLM
* @retval amount of error
*/
u32 AutoCalPllFactor(u32 pllclkSourceFrq,u32 pllclkFrq,u8 *plln,u8* pllm)
{
    u32 n,m;
    u32 tempFrq;
    u32 minDiff = pllclkFrq;
    u8  flag = 0;
    for(m = 0;m < 4 ; m++)
    {
        for(n = 0;n < 64 ;n++)
        {
            tempFrq =  pllclkSourceFrq*(n+1)/(m+1);
            tempFrq = (tempFrq >  pllclkFrq) ? (tempFrq - pllclkFrq) : (pllclkFrq - tempFrq) ; 
            
            if(minDiff > tempFrq)
            {
                minDiff =  tempFrq;
                *plln = n;
                *pllm = m;
            }
            if(minDiff == 0)
            {
                flag = 1;
                break;
            }
        }
        if(flag != 0)
        {
            break;
        }
    }
    return  minDiff;
}

uint32_t SystemCoreClock = SYSTEM_CLOCK;
/**
* @brief  Setup the microcontroller system
*         Initialize the Embedded Flash Interface, the PLL and update the 
*         SystemCoreClock variable.
* @note   This function should be used only after reset.
* @param  pllclkSourceFrq:PLL source clock frquency;
          pllclkFrq:Target PLL clock frquency;
* @retval None
*/
void SetSysClockToXX_Define(void)
{
  __IO uint32_t StartUpCounter = 0, ClkSrcStatus = 1;
  u32 temp;
  u8 plln,pllm; 
  int i;
 
  #if 1
  CACHE->CCR &= ~(0x3<<3);
  CACHE->CCR |= 1;
  while((CACHE->SR & 0x3) != 2);
//  RCC->SYSCFG |= 1<<2;
  #endif   
    
  #if ((SYS_CLOCK_SRC == SYS_CLOCK_PLL)&&(PLL_SOURCE == PLL_SOURCE_HSE))||(SYS_CLOCK_SRC == SYS_CLOCK_HSE)
  
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);//config HSE clock
  do
  {
    ClkSrcStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  }
  while((ClkSrcStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));
  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    ClkSrcStatus = (uint32_t)0x01;
    i =100;while(i--);
  }
  else
  {
    ClkSrcStatus = (uint32_t)0x00;
  }
  #endif
  SystemCoreClock = SYSTEM_CLOCK;
  if (ClkSrcStatus == (uint32_t)0x01)
  {

    /* Config FLASH latency */ 
    FLASH->ACR |= FLASH_ACR_PRFTBE; 
    FLASH->ACR &= ~0x07;
//    temp = (SystemCoreClock-1)/24000000;
      temp = (SystemCoreClock)/24000000;
    if(temp > 7)
    {
      temp = 7;
    }
    #if (0)
    if(SystemCoreClock == 168000000)
    {
         temp = 7;
    }
    #endif
    FLASH->ACR |= temp;
    
    #if (1)
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    /* PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    
    #else
    if (SystemCoreClock < 30000000)
    {
        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
        /* PCLK1 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    }
    else if (SystemCoreClock < 60000000)
    {
        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
        /* PCLK1 = HCLK/2 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
    }
    else if (SystemCoreClock < 90000000)
    {
        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        /* PCLK2 = HCLK/2 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV2;
        /* PCLK1 = HCLK/4 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV4;
    }
    else if (SystemCoreClock < 120000000)
    {
        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        /* PCLK2 = HCLK/2 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV2;
        /* PCLK1 = HCLK/4 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV4;
    }
    else if (SystemCoreClock < 150000000)
    {
        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        /* PCLK2 = HCLK/2 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV4;
        /* PCLK1 = HCLK/4 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV8;
    }
    else if (SystemCoreClock < 160000000)
    {
        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        /* PCLK2 = HCLK/2 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV4;
        /* PCLK1 = HCLK/4 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV8;
    }
    else
    {
        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        /* PCLK2 = HCLK/2 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV8;
        /* PCLK1 = HCLK/4 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV16;
    }
    #endif

	/* Config pll clock */
	#if(SYS_CLOCK_SRC == SYS_CLOCK_PLL) 
    
    RCC->CFGR &= (uint32_t)0xFFFCFFFF;
    #if(PLL_SOURCE == PLL_SOURCE_HSI)
    temp = 48000000/4;
    RCC->CFGR &= ~(uint32_t)RCC_CFGR_PLLSRC;
    #elif(PLL_SOURCE == PLL_SOURCE_HSE)
    temp = HSE_VALUE;  
    RCC->CFGR |= (uint32_t)RCC_CFGR_PLLSRC;
    #elif(PLL_SOURCE == PLL_SOURCE_HSE_DIV_2)  
    temp = HSE_VALUE/2; 
    #endif

    AutoCalPllFactor(temp,SystemCoreClock, &plln, &pllm); 
    
    RCC->CR &= (uint32_t)0x000FFFFF;  
    
    RCC->CR |= (plln<<26)|(pllm<<20);
    
    RCC->APB1ENR |= 0x10000000;
    
    #if 1
    if(SystemCoreClock > 96000000)
    {
      PWR->CR |= 1<<16;
      PWR->CR |= 3<<14;
      while(!(PWR->CSR&(1<<16)));
      RCC->CR |= RCC_CR_PLLON;
      while((RCC->CR & RCC_CR_PLLRDY) == 0);
    
      do
      {
        PWR->CR |= 1<<17;
      }
      while(!(PWR->CSR&(1<<17)));
      i=1000;while(i--);
    }
    else
    #endif
    {
      RCC->CR |= RCC_CR_PLLON;
    }
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    #endif
	
    /* select system clock */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW)); 

    RCC->CFGR |= (uint32_t)SYS_CLOCK_SRC;  
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)(SYS_CLOCK_SRC<<2))
    {

    }
  }
  else
  {
  } 
}
/**
* @brief  Setup the microcontroller system
*         Initialize the Embedded Flash Interface, the PLL and update the 
*         SystemCoreClock variable.
* @note   This function should be used only after reset.
* @param  None
* @retval None
*/
void SystemInit (void)
{
    /* Reset the RCC clock configuration to the default reset state(for debug purpose) */   
    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFGR &= (uint32_t)0xF8FF000C;    
    
    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;
    
    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;
    
    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    RCC->CFGR &= (uint32_t)0xFF80FFFF;
    RCC->CR &= (uint32_t)0x000FFFFF;
    
    /* Disable all interrupts and clear pending bits  */
    RCC->CIR = 0x009F0000;
    
    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    SetSysClockToXX_Define();	
#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif 
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/    
/*-------------------------(C) COPYRIGHT 2018 MindMotion ----------------------*/

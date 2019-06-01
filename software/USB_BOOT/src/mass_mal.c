/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : mass_mal.c
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Medium Access Layer interface
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "fsmc_nand.h"
#include "mass_mal.h"
#include "mm32flash.h"
#include "HAL_flash.h"

#define     FLASH_SIZE              0x3C000  	    // 240K 
#define     FLASH_PAGE_SIZE         0x200//0x400         // 1k Bytes per page,
#define     FLASH_WAIT_TIMEOUT      100000

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Mass_Memory_Size[2];
uint32_t Mass_Block_Size[2];
uint32_t Mass_Block_Count[2];


__IO uint32_t Status = 0;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int VD_WriteBlock(u32 Memory_Offset,u8* Writebuff,u32 Transfer_Length);
int VD_ReadBlock(u32 Memory_Offset,u8* Readbuff,u32 Transfer_Length);

/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the MM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{
  uint16_t status = MAL_OK;

  switch (lun)
  {
    case 0:
      FLASH_Unlock(); 
      break;
    default:
      return MAL_FAIL;
  }
  return status;
}
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint8_t *Writebuff, uint16_t Transfer_Length)
{

  switch (lun)
  {
    case 0:
      MM32FLASH_Write(FLASH_START_ADDR + Memory_Offset,(uint16_t*)Writebuff,Transfer_Length/2);				
      return MAL_OK;
    
    default:
      return MAL_FAIL;
  }
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/

uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint8_t *Readbuff, uint16_t Transfer_Length)
{

  switch (lun)
  {
    case 0:
       MM32FLASH_Read(FLASH_START_ADDR+Memory_Offset,(u16 *)Readbuff, Transfer_Length/2);

      return MAL_OK;	
    default:
      return MAL_FAIL;
  }
}


/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
  if (lun == 0)
  {
    Mass_Block_Count[0] = FLASH_SIZE/FLASH_PAGE_SIZE; 
    Mass_Block_Size[0] = FLASH_PAGE_SIZE;                    
    Mass_Memory_Size[0] = FLASH_SIZE;
    return MAL_OK;
	}
 
  return MAL_FAIL;
}

/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/

/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : memory.c
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : Memory management layer
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "memory.h"
#include "usb_scsi.h"
#include "usb_bot.h"
#include "usb_regs.h"
#include "usb_mem.h"
#include "usb_conf.h"
#include "hw_config.h"
#include "mass_mal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t Block_Read_count = 0;
__IO uint32_t Block_offset;
__IO uint32_t BulkWriteCounter = 0;
uint32_t  Idx;
uint8_t Data_Buffer[BULK_MAX_PACKET_SIZE *16]; /* 1024 bytes*/

uint8_t TransferState = TXFR_IDLE;
/* Extern variables ----------------------------------------------------------*/
extern uint8_t Bulk_Data_Buff[BULK_MAX_PACKET_SIZE];  /* data buffer*/
extern uint16_t Data_Len;
extern uint8_t Bot_State;
extern Bulk_Only_CBW CBW;
extern Bulk_Only_CSW CSW;
extern uint32_t Mass_Memory_Size[2];
extern uint32_t Mass_Block_Size[2];

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Read_Memory
* Description    : Handle the Read operation from the microSD card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Read_Memory(uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{
  static uint32_t Offset, Length;

  if (TransferState == TXFR_IDLE )
  {
    Offset = Memory_Offset * Mass_Block_Size[lun];
    Length = Transfer_Length * Mass_Block_Size[lun];
    TransferState = TXFR_ONGOING;
  }

  if (TransferState == TXFR_ONGOING )
  {
    if (!Block_Read_count)
    {
      MAL_Read(lun ,
               Offset ,
               (u8*)Data_Buffer,
               Mass_Block_Size[lun]);

      UserToPMABufferCopy((uint8_t *)Data_Buffer, ENDP1, BULK_MAX_PACKET_SIZE);
      
      Block_Read_count = Mass_Block_Size[lun] - BULK_MAX_PACKET_SIZE;
      Block_offset = BULK_MAX_PACKET_SIZE;
    }
    else
    {
      UserToPMABufferCopy((uint8_t *)Data_Buffer + Block_offset, ENDP1, BULK_MAX_PACKET_SIZE);
      Block_Read_count -= BULK_MAX_PACKET_SIZE;
      Block_offset += BULK_MAX_PACKET_SIZE;
    }

    Offset += BULK_MAX_PACKET_SIZE;
    Length -= BULK_MAX_PACKET_SIZE;

    CSW.dDataResidue -= BULK_MAX_PACKET_SIZE;
    Led_RW_ON();
  }
  if (Length == 0)
  {
    Block_Read_count = 0;
    Block_offset = 0;
    Offset = 0;
    Bot_State = BOT_DATA_IN_LAST;
    TransferState = TXFR_IDLE;
    Led_RW_OFF();
  }
}

/*******************************************************************************
* Function Name  : Write_Memory
* Description    : Handle the Write operation to the microSD card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Write_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{

  static uint32_t W_Offset, W_Length;

  if (TransferState == TXFR_IDLE )
  {
    W_Offset = Memory_Offset * Mass_Block_Size[lun];
    W_Length = Transfer_Length * Mass_Block_Size[lun];
    TransferState = TXFR_ONGOING;
  }
  
  if (TransferState == TXFR_ONGOING )
  {

    for (Idx = 0 ; Idx < Data_Len; Idx++)
    {
        Data_Buffer[BulkWriteCounter] = Bulk_Data_Buff[Idx];
        BulkWriteCounter++;
    }

    W_Offset += Data_Len;
    W_Length -= Data_Len;

    if (!(W_Length % Mass_Block_Size[lun]))
    {
      BulkWriteCounter = 0;

      MAL_Write(lun ,
                W_Offset - Mass_Block_Size[lun],
                Data_Buffer,
                Mass_Block_Size[lun]);
    }
        CSW.dDataResidue -= Data_Len;

    Led_RW_ON();
  }

  if ((W_Length == 0) || (Bot_State == BOT_CSW_Send))
  {
    BulkWriteCounter = 0;
    Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
    TransferState = TXFR_IDLE;
    Led_RW_OFF();
  }
}
/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/


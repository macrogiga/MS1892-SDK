#include "mm32flash.h"
#include "HAL_flash.h"
#include "usb_bot.h"
#include "FAT12.h"

extern Bulk_Only_CSW CSW;
/*******************************************************************************
* Function Name  : MM32FLASH_ReadHalfWord
* Description    : Read the halfword of the specified address.
* Input          : faddr(Read address)(This address must be a multiple of 2)
* Return         : faddr
*******************************************************************************/
u16 MM32FLASH_ReadHalfWord(u32 faddr)
{
    if (faddr < (u32)DiskImage+sizeof(DiskImage))
        return *(vu16*)faddr; 
    else
        return 0xffff;
}

#if MM32_FLASH_WREN	
/********************************************************************************************
* Function Name  : MM32FLASH_Write_NoCheck
* Description    : Write without checking.
* Input          : WriteAddr(initial address),pBuffer(Data pointer),NumToWrite(Halfword count)
* Output         : None
*********************************************************************************************/
void MM32FLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
    
	for(i=0;i<NumToWrite;i++)
	{ 
		  FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
      /*Address increase 2*/
	    WriteAddr+=2;
	}  
} 

#if MM32_FLASH_SIZE<256
#define MM32_SECTOR_SIZE 1024 
#else 
#define MM32_SECTOR_SIZE	2048
#endif		 
u16 MM32FLASH_BUF[MM32_SECTOR_SIZE/2];/*Up to 2K bytes*/
/*******************************************************************************
* Function Name  : MM32FLASH_Write
* Description    : Write data of the specified length from the specified address.
* Input          : WriteAddr(initial address)(This address must be a multiple of 2),
                   pBuffer(Data pointer),NumToWrite(Halfword count)
* Output         : None
*******************************************************************************/
void MM32FLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
  /*Remove the address after 0x08000000*/
	u32 offaddr;   
  offaddr = MM32_FLASH_BASE+1024*MM32_FLASH_SIZE;
  /*Illegal address*/
	if(WriteAddr < MM32_FLASH_BASE||( WriteAddr>= (offaddr)))return;
    
    if (WriteAddr > ((u32)DiskImage+sizeof(DiskImage)))
    {
        FLASH_Unlock();						

        //整page
        if (0 == (WriteAddr&0x000003ff))
            FLASH_ErasePage(WriteAddr);
        MM32FLASH_Write_NoCheck(WriteAddr,pBuffer,NumToWrite);

        FLASH_Lock();
    }
}
#endif

/********************************************************************************************
* Function Name  : MM32FLASH_Read
* Description    : Read data of the specified length from the specified address.
* Input          : ReadAddr(initial address),pBuffer(Data pointer),NumToWrite(Halfword count)
* Output         : dat
*********************************************************************************************/
void MM32FLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	  u16 i;
    u32 dat;
   
    dat = CSW.dDataResidue;
	  for(i=0;i<NumToRead;i++)
	  {
        /*Read two bytes*/
	    	pBuffer[i]=MM32FLASH_ReadHalfWord(ReadAddr);
        /*Offset two bytes*/
		    ReadAddr+=2;
         
  	}
    CSW.dDataResidue = dat;
}


#ifndef __MM32FLASH_H__
#define __MM32FLASH_H__

#include "HAL_device.h"

/*Users set according to their own needs*/
#define MM32_FLASH_SIZE 512 	 		  /*The selected FLASH capacity size of MM32(The unit is K)*/
#define MM32_FLASH_WREN 1           /*Enable FLASH write(0,Disable;1,Enable)*/  

/*FLASH starting address*/
#define MM32_FLASH_BASE     0x08000000

#define FLASH_START_ADDR    0x08004400    // Flash start address 
#define APP_ENTRY_ADDRESS   0x08005000

u16 MM32FLASH_ReadHalfWord(u32 faddr);		                            
void MM32FLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	
u32 MM32FLASH_ReadLenByte(u32 ReadAddr,u16 Len);						
void MM32FLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);	
void MM32FLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   	

								   
#endif


















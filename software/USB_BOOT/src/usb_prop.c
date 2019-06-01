/******************** (C) COPYRIGHT 2018 MindMotion ********************
* File Name          : usb_prop.c
* Version            : V1.0.0
* Date               : 2018/08/21
* Description        : All processing related to Mass Storage Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"

#include "usb_bot.h"
#include "mass_mal.h"

extern USB_OTG_BDT_TypeDef *pUSB_OTG_BDT;
extern u8 rxUsbBufOdd[16] ;
extern u8 txUsbBufOdd[16] ;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t Max_Lun = 0;

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    MASS_init,
    MASS_Reset,
    MASS_Status_In,
    MASS_Status_Out,
    MASS_Data_Setup,
    MASS_NoData_Setup,
    MASS_Get_Interface_Setting,
    MASS_GetDeviceDescriptor,
    MASS_GetConfigDescriptor,
    MASS_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Mass_Storage_GetConfiguration,
    Mass_Storage_SetConfiguration,
    Mass_Storage_GetInterface,
    Mass_Storage_SetInterface,
    Mass_Storage_GetStatus,
    Mass_Storage_ClearFeature,
    Mass_Storage_SetEndPointFeature,
    Mass_Storage_SetDeviceFeature,
    Mass_Storage_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)MASS_DeviceDescriptor,
    MASS_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)MASS_ConfigDescriptor,
    MASS_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR String_Descriptor[5] =
  {
    {(uint8_t*)MASS_StringLangID, MASS_SIZ_STRING_LANGID},
    {(uint8_t*)MASS_StringVendor, MASS_SIZ_STRING_VENDOR},
    {(uint8_t*)MASS_StringProduct, MASS_SIZ_STRING_PRODUCT},
    {(uint8_t*)MASS_StringSerial, MASS_SIZ_STRING_SERIAL},
    {(uint8_t*)MASS_StringInterface, MASS_SIZ_STRING_INTERFACE},
  };

/* Extern variables ----------------------------------------------------------*/
extern unsigned char Bot_State;
extern Bulk_Only_CBW CBW;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MASS_init
* Description    : Mass Storage init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void MASS_init()
{
  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();
  /* USB interrupts initialization */
	
  /*RCC->CFGR &= ~(0x3<<22);
  RCC->CFGR |= (0x0<<22);*/
  RCC->AHB2ENR |= 1<<7;           //Enable usb clock
  
  USB_OTG_FS->OTG_CTRL = 0;
  USB_OTG_FS->BDT_PAGE_03 = 0x20;
  USB_OTG_FS->BDT_PAGE_02 = 0x00;
  USB_OTG_FS->BDT_PAGE_01 = 0x70; //配置描述表地址
  USB_OTG_FS->ADDR = 0;
  USB_OTG_FS->CTL |= 1 << 1; 
  USB_OTG_FS->CTL &= ~(1 << 1);//复位BDT ping/pang bits

  pUSB_OTG_BDT = (USB_OTG_BDT_TypeDef*)((USB_OTG_FS->BDT_PAGE_03<<24)|(USB_OTG_FS->BDT_PAGE_02<<16)|(USB_OTG_FS->BDT_PAGE_01<<8));
  
  EndPointInit( pUSB_OTG_BDT + 0,(u32)pUSB_OTG_BDT+0x200+0x100*0);
  EndPointInit( pUSB_OTG_BDT + 1,(u32)pUSB_OTG_BDT+0x200+0x100*1);
  EndPointInit( pUSB_OTG_BDT + 2,(u32)pUSB_OTG_BDT+0x200+0x100*2);
  EndPointInit( pUSB_OTG_BDT + 3,(u32)pUSB_OTG_BDT+0x200+0x100*3);
  
  txUsbBufOdd[0] = 0;
  USB_OTG_FS->EP_CTL[0] = 0x0d;     //使能端点0
  USB_OTG_FS->EP_CTL[1] = 0x05;     //使能端点1
  USB_OTG_FS->EP_CTL[2] = 0x09;     //使能端点2

  USB_OTG_FS->CTL |= 1;           //使能USB
  USB_OTG_FS->OTG_CTRL |= 0x80;   //连接
  USB_OTG_FS->INT_ENB = /*USB_INT_STAT_SOF_TOK|*/USB_INT_STAT_RST|USB_INT_STAT_TOK_DNE;
  
  USB_OTG_FS->OTG_CTRL |= 0x80;   //连接   H
  /* configure the USART 1 to the default settings */

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : MASS_Reset
* Description    : Mass Storage reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void MASS_Reset()
{
    /* Set the device as not configured */
    Device_Info.Current_Configuration = 0;

    /* Current Feature initialization */
    pInformation->Current_Feature = MASS_ConfigDescriptor[7];

    pUSB_OTG_BDT = (USB_OTG_BDT_TypeDef*)((USB_OTG_FS->BDT_PAGE_03<<24)|(USB_OTG_FS->BDT_PAGE_02<<16)|(USB_OTG_FS->BDT_PAGE_01<<8));
    
    EndPointInit( pUSB_OTG_BDT + 0,(u32)pUSB_OTG_BDT+0x200+0x100*0);
    EndPointInit( pUSB_OTG_BDT + 1,(u32)pUSB_OTG_BDT+0x200+0x100*1);
    EndPointInit( pUSB_OTG_BDT + 2,(u32)pUSB_OTG_BDT+0x200+0x100*2);
    EndPointInit( pUSB_OTG_BDT + 3,(u32)pUSB_OTG_BDT+0x200+0x100*3);
    
    USB_OTG_FS->EP_CTL[0] = 0x0d;     //使能端点0
    USB_OTG_FS->EP_CTL[1] = 0x05;     //使能端点1
    USB_OTG_FS->EP_CTL[2] = 0x09;     //使能端点2

    USB_OTG_FS->CTL |= 1 << 1; 
    USB_OTG_FS->CTL &= ~(1 << 1);//复位BDT ping/pang bits
    txUsbBufOdd[0] = 0;
    SetDeviceAddress(0);
    
    _SetUSB_Connect;
    bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : Mass_Storage_SetConfiguration
* Description    : Handle the SetConfiguration request.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Mass_Storage_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : Mass_Storage_ClearFeature
* Description    : Handle the ClearFeature request.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Mass_Storage_ClearFeature(void)
{
  /* when the host send a CBW with invalid signature or invalid length the two
     Endpoints (IN & OUT) shall stall until receiving a Mass Storage Reset     */
  if (CBW.dSignature != BOT_CBW_SIGNATURE)
    Bot_Abort(BOTH_DIR);
}

/*******************************************************************************
* Function Name  : Mass_Storage_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Mass_Storage_SetDeviceAddress (void)
{
  SetDeviceAddress(pInformation->USBwValue0);
  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : MASS_Status_In
* Description    : Mass Storage Status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void MASS_Status_In(void)
{
  return;
}

/*******************************************************************************
* Function Name  : MASS_Status_Out
* Description    : Mass Storage Status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void MASS_Status_Out(void)
{
  u8 temp;
  PMAToUserBufferCopy(&temp, ENDP0, 0);
  return;
}

/*******************************************************************************
* Function Name  : MASS_Data_Setup.
* Description    : Handle the data class specific requests..
* Input          : RequestNo.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT MASS_Data_Setup(uint8_t RequestNo)
{
  uint8_t    *(*CopyRoutine)(uint16_t);

  CopyRoutine = NULL;
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == GET_MAX_LUN) && (pInformation->USBwValue == 0)
      && (pInformation->USBwIndex == 0) && (pInformation->USBwLength == 0x01))
  {
    CopyRoutine = Get_Max_Lun;
  }
  else
  {
    return USB_UNSUPPORT;
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);

  return USB_SUCCESS;

}

/*******************************************************************************
* Function Name  : MASS_NoData_Setup.
* Description    : Handle the no data class specific requests.
* Input          : RequestNo.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT MASS_NoData_Setup(uint8_t RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == MASS_STORAGE_RESET) && (pInformation->USBwValue == 0)
      && (pInformation->USBwIndex == 0) && (pInformation->USBwLength == 0x00))
  {
    /*intialise the CBW signature to enable the clear feature*/
    CBW.dSignature = BOT_CBW_SIGNATURE;
    Bot_State = BOT_IDLE;

    return USB_SUCCESS;
  }
  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : MASS_Get_Interface_Setting
* Description    : Test the interface and the alternate setting according to the
*                  supported one.
* Input          : uint8_t Interface, uint8_t AlternateSetting.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT MASS_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;/* in this application we don't have AlternateSetting*/
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;/*in this application we have only 1 interfaces*/
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : MASS_GetDeviceDescriptor
* Description    : Get the device descriptor.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *MASS_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor );
}

/*******************************************************************************
* Function Name  : MASS_GetConfigDescriptor
* Description    : Get the configuration descriptor.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *MASS_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor );
}

/*******************************************************************************
* Function Name  : MASS_GetStringDescriptor
* Description    : Get the string descriptors according to the needed index.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *MASS_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;

  if (wValue0 > 5)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : Get_Max_Lun
* Description    : Handle the Get Max Lun request.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *Get_Max_Lun(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = LUN_DATA_LENGTH;
    return 0;
  }
  else
  {
    return((uint8_t*)(&Max_Lun));
  }
}

/******************* (C) COPYRIGHT 2018 MindMotion *****END OF FILE****/

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
#include <string.h>
#include "HAL_conf.h"
#include "mg_api.h"



#define SOFTWARE_INFO                 "SV5.0.0"
#define MANU_INFO                     "MG-BLE"

#define TYPE_INC                      0x2802
#define TYPE_CHAR                     0x2803
#define TYPE_INFO                     0x2901
#define TYPE_CFG                      0x2902
#define TYPE_xRpRef                   0x2907
#define TYPE_RpRef                    0x2908

/// Characteristic Properties Bit
#define ATT_CHAR_PROP_RD                            0x02
#define ATT_CHAR_PROP_W_NORSP                       0x04
#define ATT_CHAR_PROP_W                             0x08
#define ATT_CHAR_PROP_NTF                           0x10
#define ATT_CHAR_PROP_IND                           0x20

#define UUID16_FORMAT                 0xFF
#define GATT_PRIMARY_SERVICE_UUID     0x2800

// Statement ---------------------------------------------------------------------
/* Note: make sure each notify handle by invoking function: set_notifyhandle(hd) */
u16 cur_notifyhandle = 0x12;  //Note: make sure each notify handle by invoking function: set_notifyhandle(hd);

// APP ---------------------------------------------------------------------------

char DeviceInfo[]   =   "MS1892-HRS";  /*max len is 24 bytes*/

typedef struct ble_character16{
    u16 type16;          //type2
    u16 handle_rec;      //handle
    u8  characterInfo[5];//property1 - handle2 - uuid2
    u8  uuid128_idx;     //0xff means uuid16,other is idx of uuid128
}BLE_CHAR;

typedef struct ble_UUID128{    
    u8  uuid128[16];    //uuid128 string: little endian
}BLE_UUID128;

/**********************************************************************************
                 *****DataBase****

    01 - 06  GAP (Primary service) 0x1800
      03:04  name
    07 - 0f  Device Info (Primary service) 0x180a
      0a:0b  firmware version
      0e:0f  software version
    10 - 19  UART service (Primary service) 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
      11:12  6E400003-B5A3-F393-E0A9-E50E24DCCA9E(0x04)  RxNotify
      13     cfg
      14:15  6E400002-B5A3-F393-E0A9-E50E24DCCA9E(0x0C)  Tx
      16     cfg
      17:18  6E400004-B5A3-F393-E0A9-E50E24DCCA9E(0x0A)  BaudRate
      19     0x2901  info

************************************************************************************/

//
///STEP0:Character declare
//
const BLE_CHAR AttCharList[] = {
// ======  gatt =====  Do NOT Change!!
    {TYPE_CHAR,0x0003, {ATT_CHAR_PROP_RD, 0x04,0, 0x00,0x2a}, UUID16_FORMAT},//name
    //05-06 reserved
// ======  device info =====    Do NOT Change if using the default!!!  
    {TYPE_CHAR,0x0008, {ATT_CHAR_PROP_RD, 0x09,0, 0x29,0x2a}, UUID16_FORMAT},//manufacture
    {TYPE_CHAR,0x000a, {ATT_CHAR_PROP_RD, 0x0b,0, 0x26,0x2a}, UUID16_FORMAT},//firmware version
    {TYPE_CHAR,0x000e, {ATT_CHAR_PROP_RD, 0x0f,0, 0x28,0x2a}, UUID16_FORMAT},//sw version
    
// ======  User service or other services added here =====  User defined  
    {TYPE_CHAR,0x11,ATT_CHAR_PROP_NTF, 0x12,0, 0x37,0x2A, UUID16_FORMAT},//HRM value
    {TYPE_CFG, 0x13,ATT_CHAR_PROP_RD|ATT_CHAR_PROP_W},//cfg
    {TYPE_CHAR,0x14,ATT_CHAR_PROP_RD, 0x15,0, 0x38,0x2A, UUID16_FORMAT},

    {TYPE_CHAR,0x17,ATT_CHAR_PROP_RD|ATT_CHAR_PROP_NTF,    0x18,0,0x19,0x2A,UUID16_FORMAT},//battery value
    {TYPE_CFG, 0x19,ATT_CHAR_PROP_RD|ATT_CHAR_PROP_W},//cfg
};

const BLE_UUID128 AttUuid128List[] = {
0
};

const BLE_UUID128 AttUuid16List[][2] = {
{0x0D,0x18},
{0x0f,0x18},
};

u8 GetCharListDim(void)
{
    return sizeof(AttCharList)/sizeof(AttCharList[0]);
}

//////////////////////////////////////////////////////////////////////////
///STEP1:Service declare
// read by type request handle, primary service declare implementation
void att_server_rdByGrType( u8 pdu_type, u8 attOpcode, u16 st_hd, u16 end_hd, u16 att_type )
{
  //!!!!!!!!  hard code for gap and gatt, make sure here is 100% matched with database:[AttCharList] !!!!!!!!!
                     
    if((att_type == GATT_PRIMARY_SERVICE_UUID) && (st_hd == 1))//hard code for device info service
    {
        //att_server_rdByGrTypeRspDeviceInfo(pdu_type);//using the default device info service
        //GAP Device Name
        u8 t[] = {0x00,0x18};
        att_server_rdByGrTypeRspPrimaryService(pdu_type,0x1,0x6,(u8*)(t),2);
        return;
    }
    else if((att_type == GATT_PRIMARY_SERVICE_UUID) && (st_hd <= 0x07))//hard code for device info service
    {
        //apply user defined (device info)service example
        u8 t[] = {0x0a,0x18};
        att_server_rdByGrTypeRspPrimaryService(pdu_type,0x7,0xf,(u8*)(t),2);
        return;
    }
    
    else if((att_type == GATT_PRIMARY_SERVICE_UUID) && (st_hd <= 0x10)) //usr's service
    {
        att_server_rdByGrTypeRspPrimaryService(pdu_type,0x10,0x15,(u8*)(AttUuid16List[0]),2);
        return;
    }
    else if((att_type == GATT_PRIMARY_SERVICE_UUID) && (st_hd <= 0x16)) //usr's service
    {
        att_server_rdByGrTypeRspPrimaryService(pdu_type,0x16,0x19,(u8*)(AttUuid16List[1]),2);
        return;
    }
    //other service added here if any
    //to do....

    ///error handle
    att_notFd( pdu_type, attOpcode, st_hd );
}

///STEP2:data coming
/// write response, data coming....
void ser_write_rsp(u8 pdu_type/*reserved*/, u8 attOpcode/*reserved*/, 
                   u16 att_hd, u8* attValue/*app data pointer*/, u8 valueLen_w/*app data size*/)
{
    switch(att_hd)
    {
        case 0x13://cfg 
        case 0x19://cfg 
            ser_write_rsp_pkt(pdu_type);  /*if the related character has the property of WRITE(with response) or TYPE_CFG, one MUST invoke this func*/      
            break;
        
        default:
            att_notFd( pdu_type, attOpcode, att_hd );	/*the default response, also for the purpose of error robust */
            break;
    }    		
 }
 unsigned char SimBatt=100;
 //// read response
void server_rd_rsp(u8 attOpcode, u16 attHandle, u8 pdu_type)
{
    u8 Type[1] = {0x03};//finger
    
    switch(attHandle) //hard code
    {
        case 0x04://GAP name
            att_server_rd( pdu_type, attOpcode, attHandle, (u8*)(DeviceInfo), strlen(DeviceInfo));
            break;
                
        case 0x09: //MANU_INFO
            //att_server_rd( pdu_type, attOpcode, attHandle, (u8*)(MANU_INFO), sizeof(MANU_INFO)-1);
            att_server_rd( pdu_type, attOpcode, attHandle, get_ble_version(), strlen((const char*)get_ble_version())); 
            break;
        
        case 0x0b: //FIRMWARE_INFO
            //do NOT modify this code!!!
            att_server_rd( pdu_type, attOpcode, attHandle, GetFirmwareInfo(),strlen((const char*)GetFirmwareInfo()));
            break;
        
        case 0x0f://SOFTWARE_INFO
            att_server_rd( pdu_type, attOpcode, attHandle, (u8*)(SOFTWARE_INFO), sizeof(SOFTWARE_INFO)-1);
            break;
        
        case 0x13://cfg
        case 0x19://cfg
        case 0x15:
            att_server_rd( pdu_type, attOpcode, attHandle, Type, 1);
            break;
        
        case 0x18:
            att_server_rd( pdu_type, attOpcode, attHandle, &SimBatt, 1);
            break;
        
        default:
            att_notFd( pdu_type, attOpcode, attHandle );/*the default response, also for the purpose of error robust */
            break;
    }
}

static unsigned char HRMData[3]={
    0x00,0x00,0x01
};
/* Callback --------------------------------------------------------------- */

void gatt_user_send_notify_data_callback(void)
{
    static u8 Cont=0;
    u16 Val=0;
    
    Cont++;
    if (Cont >= 40)
    {
        Cont = 0;

        Val = Get_Adc_Average(ADC_Channel_0,5); //0~4095
        ADC1_SingleChannel(ADC_Channel_refvol);
        
        Val = Val>>3; // 0~511 for HRM data
        cur_notifyhandle = 0x12;
        if (Val<0x100)
        {
            HRMData[0] = 0; //1Byte
            HRMData[1] = Val;
            sconn_notifydata(HRMData,2);
        }
        else
        {
            HRMData[0] = 1; //2Byte
            HRMData[1] = Val;
            HRMData[2] = Val>>8;
            sconn_notifydata(HRMData,3);
        }
    }
    else if (20 == Cont)
    {
        Val = Get_Adc_Average(ADC_Channel_refvol,5); //0~4095
        ADC1_SingleChannel(ADC_Channel_0);
        
        /*Val/4095 = 1.2/Vbat  =>  Vbat = 4095*1.2/Val
          % = (Vbat - 2.5)/(3.3-2.5) * 100
        %=4095*1200/8/Val - 2500/8
        */
        Val = 614250/Val;
        if(Val > (2500>>3))
            SimBatt = Val - (2500>>3);
        else
            SimBatt = 0;

        cur_notifyhandle = 0x18;
        sconn_notifydata(&SimBatt,1);

//        printf("%d%%\n",SimBatt);
    }
}

/* Callback --------------------------------------------------------------- */

u8* getDeviceInfoData(u8* len)
{    
    //*len = sizeof(DeviceInfo);
    *len = strlen(DeviceInfo);
    return (u8*)DeviceInfo;
}


/// Queued Writes data if any
void ser_prepare_write(u16 handle, u8* attValue, u16 attValueLen, u16 att_offset)//user's call back api 
{
    //queued data:offset + data(size)
    //when ser_execute_write() is invoked, means end of queue write.
    
    //to do    
}
 
void ser_execute_write(void)//user's call back api 
{
    //end of queued writes  
    
    //to do...    
}

// return 1 means found
int GetPrimaryServiceHandle(unsigned short hd_start, unsigned short hd_end,
                            unsigned short uuid16,   
                            unsigned short* hd_start_r,unsigned short* hd_end_r)
{
    // if(hd_start <= 0x10)
    // {
        // if(uuid16 == 0xFEB3)
        // {
            // *hd_start_r = 0x10;
            // *hd_end_r   = 0x1c;
            // return 1;
        // }        
    // }
    
    return 0;
}

u8* getsoftwareversion(void)
{
    return (u8*)SOFTWARE_INFO;
}


void ConnectStausUpdate(unsigned char IsConnectedFlag) //porting api
{
    //[IsConnectedFlag] indicates the connection status    
    void reset_uart_buf(void);
    
    reset_uart_buf();
}

void server_blob_rd_rsp(u8 attOpcode, u16 attHandle, u8 dataHdrP,u16 offset)
{
}

//---- MindMotion HardWare AES implemenation -----
//unsigned char aes_encrypt_HW(unsigned char *painText128bitBE,unsigned char *key128bitBE); //porting api, returns zero means not supported
unsigned char aes_encrypt_HW(unsigned char *_data, unsigned char *_key)
{
    unsigned int tmp = 0;

#ifndef AES_HW_SUPPORT
    return 0; //HW AES NOT supported

#else  //HW AES supported

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_AES, ENABLE); //AES CLK enable
    
    AES->CR = 0x00;	
    AES->CR|=0x03<<20;

    AES->KEYR3 = (u32)(_key[0] << 24)|(u32)(_key[1] << 16)|(u32)(_key[2] << 8)|(u32)(_key[3] << 0);
    AES->KEYR2 = (u32)(_key[4] << 24)|(u32)(_key[5] << 16)|(u32)(_key[6] << 8)|(u32)(_key[7] << 0);
    AES->KEYR1 = (u32)(_key[8] << 24)|(u32)(_key[9] << 16)|(u32)(_key[10] << 8)|(u32)(_key[11] << 0);
    AES->KEYR0 = (u32)(_key[12] << 24)|(u32)(_key[13] << 16)|(u32)(_key[14] << 8)|(u32)(_key[15] << 0);	
    
    AES->CR |= 0x01;  //start encryption
    
    AES->DINR = (u32)(_data[0] << 24)|(u32)(_data[1] << 16)|(u32)(_data[2] << 8)|(u32)(_data[3] << 0);
    AES->DINR = (u32)(_data[4] << 24)|(u32)(_data[5] << 16)|(u32)(_data[6] << 8)|(u32)(_data[7] << 0);
    AES->DINR = (u32)(_data[8] << 24)|(u32)(_data[9] << 16)|(u32)(_data[10] << 8)|(u32)(_data[11] << 0);
    AES->DINR = (u32)(_data[12] << 24)|(u32)(_data[13] << 16)|(u32)(_data[14] << 8)|(u32)(_data[15] << 0);	
    
    //��ѯģʽ
    while(1)
    {
        if((AES->SR & 0x01) )
        {
            AES->CR |=  0x1<<7; //clear ccf flag
            break;
        }
    }
    
    tmp = AES->DOUTR;  //encrypted output data0
    _data[0] = tmp >> 24;
    _data[1] = tmp >> 16;
    _data[2] = tmp >>  8;
    _data[3] = tmp;
    _data += 4;
    
    tmp = AES->DOUTR;  //encrypted output data1
    _data[0] = tmp >> 24;
    _data[1] = tmp >> 16;
    _data[2] = tmp >>  8;
    _data[3] = tmp;
    _data += 4;
    
    tmp = AES->DOUTR;  //encrypted output data2
    _data[0] = tmp >> 24;
    _data[1] = tmp >> 16;
    _data[2] = tmp >>  8;
    _data[3] = tmp;
    _data += 4;
    
    tmp = AES->DOUTR;  //encrypted output data3
    _data[0] = tmp >> 24;
    _data[1] = tmp >> 16;
    _data[2] = tmp >>  8;
    _data[3] = tmp;    

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_AES, DISABLE); //AES CLK disable

    return 1;// HW supported value
#endif
}

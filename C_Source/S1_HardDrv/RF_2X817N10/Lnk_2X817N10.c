/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: Lnk_Rn8302B.c
**Author: maji
**date: 
**description: c code for  link code of measure chip RN8302B
**note: memer type  SD03A
**--------------------Version History -------------------------------------
** Date        Rev    By     Description 
**============================================================================
**
** 1/21/2013  0.1  sosomj   1. frist .                             
**
**============================================================================ 
*/

#include <MyIncludes_H.h>


RF_BUFF_STRUCT gs_RxbufferFromRF;                  //接收无线数据缓存
INT16U RXCont=0;
const INT8U PN9_Tab2[]={  
    0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0
};  // 定义一个发送数据包



/****************************************************************
Function:		INT8U Lnk_2X817N10_Intilize(void)  	
Description: 
Input:
Parameters:     
Returns:                                
Others:         
****************************************************************/
INT8U Lnk_2X817N10_Intilize(void)
{
        if(LSD_RF_InitRF()==1) return FALSE;
	 CLRWDT();
        LSD_RF_RXmode();                     //进入RX,准备接受数据//
        return TRUE;

}

/****************************************************************
Function:		INT8U Lnk_2X817N10_Intilize(void)  	
Description: 
Input:
Parameters:     
Returns:                                
Others:         
****************************************************************/
/*void Proc_2X817N10_rx_data(void)
{
    if(LSD4RF_SPI_SET_DIO0_DIR_IN() ==0) return;
    Lib_Delay_Nop(20);
    if(LSD4RF_SPI_SET_DIO0_DIR_IN() ==0) return;   

    //DIO0 =high, mean RFmodule had receievd data  //   
    LSD_RF_RxPacket(gs_RxbufferFromRF.data_buffer,&gs_RxbufferFromRF.data_len);     //接收数据//
    if(Lib_Cmp_TwoArry(&gs_RxbufferFromRF.data_buffer,PN9_Tab2, gs_RxbufferFromRF.data_len) ==0)
    {
      //  LED_L1_LOSS_ON();
        RXCont++;
        if(RXCont>99)  //防止LCD屏幕显示溢出
        {
            RXCont=1; 
        }
        LSD_RF_SendPacket(gs_RxbufferFromRF.data_buffer,gs_RxbufferFromRF.data_len);      //发送数据
       // LED_L1_LOSS_OFF();                   //LED2点亮 
    }
    do
    {
        NOP();
    } while(LSD4RF_READ_DIO0() !=0);

    LSD_RF_RXmode();                     //进入RX,准备接受数据//
}

	 */

/***************************************************************
*    END
****************************************************************/

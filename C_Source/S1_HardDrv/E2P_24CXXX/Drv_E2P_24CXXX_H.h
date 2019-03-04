/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_E2P_24CXXX_H.h
**Author		: maji
**date			: 2016-4-19
**description	: c code for read&Write 24CXXX
**note			:  Driver code for GENERAL
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-19   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#ifndef 	_Drv_E2P_24CXXX_H
#define	_Drv_E2P_24CXXX_H

//******************************************************************
//              1     立即数预定义                                            //                      
//******************************************************************
// 操作命令字定义 //
#define     WRITE_EEPROM_CMD              0xA0
#define     READ_EEPROM_CMD               WRITE_EEPROM_CMD+1

//铁电类型定义  //
#define _E2P_TYPE_AT24               // 普通E2P时定义，铁电类型时则注释掉 //

//E2P 类型定义 //
#define EE24XX32        0
#define EE24XX64        1
#define EE24XX128       2
#define EE24XX256       3
#define EE24XX512       4
#define EE24XX08        5
#define EE24XX16        6


#define EE_Chip         EE24XX256           //EEPROM chip(EE24XX08/EE24XX16/EE24XX32/EE24XX64/EE24XX128/EE24XX256/EE24XX512)
#if (EE_Chip == EE24XX08)
    #define EE_Capacity     0x03ffU         //末尾地址
    #define EE_PageMask     0x10U           //16Bytes,页大小0~0f
#elif (EE_Chip == EE24XX16)
    #define EE_Capacity     0x07ffU         //末尾地址
    #define EE_PageMask     0x10U           //16Bytes,页大小0~0f
#elif (EE_Chip == EE24XX32)
    #define EE_Capacity     0x0fffU         //末尾地址
    #define EE_PageMask     0x20U           //32Bytes,页大小0~1f
#elif   (EE_Chip == EE24XX64)
    #define EE_Capacity     0x1fffU         //末尾地址
    #define EE_PageMask     0x20U           //32Bytes,页大小0~1f
#elif   (EE_Chip == EE24XX128)
    #define EE_Capacity     0x3fffU         //末尾地址
    #define EE_PageMask     0x40U           //64Bytes,页大小0~3f
#elif   (EE_Chip == EE24XX256)
    #define EE_Capacity     0x7fffU         //末尾地址
    #define EE_PageMask     0x40U           //64Bytes,页大小0~3f
#elif   (EE_Chip == EE24XX512)
    #define EE_Capacity     0xffffU         //末尾地址
    #define EE_PageMask     0x80U           //128Bytes,页大小0~7f
#else
    #error "Unrecognized EEPROM Chip!"
#endif

//******************************************************************
//               2     数据结构声明                                                 //
//******************************************************************


//******************************************************************
//              3    变量声明                                                             //
//******************************************************************


//******************************************************************
//             4     函数声明                                                              //
//******************************************************************
extern uint8 Read_EEPROM_Page_Sub(uint8 *Buf,uint16 Addr,uint8 Len);
extern uint8 Write_EEPROM_Page_Sub(uint16 Addr,uint8 *Buf,uint8 Len);
extern uint8 Read_EEPROM_Page(uint8* u8p_dst, uint16 u16_src, uint8 u8_len);
extern uint8 Write_EEPROM_Page(uint16 u16_dst, uint8 *u8p_src, uint8 u8_len);
extern uint8  Write_EEPROM(uint16 u16_dst, uint8 *u8p_src, uint16 u16_len);
extern uint8 Read_EEPROM(uint8* u8p_dst, uint16 u16_src, uint16 u16_len);

//******************************************************************
//             5     其他内容，例如伪指令等              //
//******************************************************************
#define     Disable_WP()                  NOP()
#define     Enable_WP()                   NOP()


/***************************************************************
*    END
****************************************************************/

#endif
/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: Lib_myMath_H.H
**Author: maji
**date: 
**description:  define for general library function
**note: memer type  SD03A
**--------------------Version History -------------------------------------
** NO.	Date        		Ver    		By     			Description 
**==============================================================
** 1		2014-05-22  		0.1  			sosomj   			1. frist version                             
**
**==============================================================
*/



#ifndef  _Lib_myMath_H
#define	    _Lib_myMath_H

//******************************************************************
//              1     立即数预定义                                            
//                      
//******************************************************************



//******************************************************************
//               2     数据结构声明                                                 //
//******************************************************************


//******************************************************************
//              3    变量声明                                                             //
//******************************************************************



//******************************************************************
//             4     函数声明                                                              //
//******************************************************************
extern INT8U Lib_byte_bcd(INT8U xx);
extern INT8U Lib_bcd_byte(INT8U xx);
extern void Lib_word_bcd2( INT8U *ptr, INT16U xx );
extern INT16U Lib_bcd2_word(INT8U* ptr);
extern void Lib_long_bcd4(INT8U* ptr, INT32U xx);
extern INT32U Lib_bcd4_long( INT8U* ptr );
extern void Lib_bcd_inc ( INT8U* ptr );
extern INT8U Lib_bcd_arry_add1(INT8U* ptr, INT8U len);
extern INT16U Lib_get_csck_int16u_num( INT8U *ptr, INT16U len, INT16U key);
extern INT8U Lib_get_csck_int8u_num( INT8U *ptr, INT16U len, INT8U key);
extern uint8 Lib_BCDCHK(uint8 *ptr,uint8 len);
extern INT16U Lib_GetGmDay( INT8U* Addr );
extern INT32U Lib_GetGmHour( INT8U* Addr );
extern INT32U Lib_GetGmMin( INT8U* Addr );
extern INT32U Lib_GetGmSec( INT8U* Addr ) ;
extern INT8U Lib_Check_RTC_data_FORM(INT8U* ClkData);
extern INT8U  Lib_ENOB_Get(INT8U* s1, INT8U len,INT8U ENOB_len);
extern INT8U Lib_AtoB( INT8U Asc ) ;
extern INT8U  Lib_BtoA( INT8U BCD ) ;
extern void Lib_Asc_BCD( INT8U *BCD, INT8U *Asc, INT8U Lenth ) ;
extern void Lib_Asc_BCDA( INT8U *BCD, INT8U *Asc, unsigned char Lenth ) ;
extern void Lib_BCD_Asc( INT8U *Asc, INT8U *BCD ,unsigned char Lenth ) ;
extern void Lib_BCD_AscA( INT8U *Asc, INT8U *BCD ,unsigned char Lenth ) ;
extern INT8U Lib_math_get_BCC_chk(INT8U* yourBuf, INT8U StartID, INT8U EndID);
extern unsigned char Lib_check_bitnum(unsigned char value);

//******************************************************************
//             5     其他内容，例如伪指令等              //
//******************************************************************




#endif
/***************************************************************
*    END
****************************************************************/


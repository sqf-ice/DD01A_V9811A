/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: SYS_MCU_MACRO_H.H
**Author: maji
**date: 
**description:  MCU相关操作伪指令
**note:  适用于V98XX系列芯片
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-18   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#ifndef 	_SYS_MCU_MACRO_H
#define	_SYS_MCU_MACRO_H



//******************************************************************
//			   		//
//******************************************************************
#define NOP()     _nop_()   //__ASM ("NOP")
#define nop()	  NOP()
#define  DI()    EA=0                       //关闭中断//
#define  EI()    EA=1   
#define DisableINT()   	DI()               //开启中断响应  //
#define EnableINT()    	EI()               //关闭中断响应  //
#define  CLRWDT()    {wdt0=0XA5;wdt1=0x5a;NOP() ;wdt0=0XA5;wdt1=0x5a;NOP() ;} //清除看门狗命令//



/***************************************************************
*    END
****************************************************************/

#endif
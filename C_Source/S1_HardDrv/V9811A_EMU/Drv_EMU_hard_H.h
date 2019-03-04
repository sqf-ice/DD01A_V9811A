/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_EMU_hard_H.H
**Author		: maji
**date			: 2016-04-26 
**description	: V9811A的EMU计量模块硬件底层驱动C代码
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#ifndef _Drv_EMU_hard_H
#define _Drv_EMU_hard_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/
/**************************ADC CHANNEL********************/
#define ADCAPDN   BIT0
#define ADCBPDN   BIT1
#define ADCUPDN   BIT2
#define ADCMPDN   BIT3
//open adc
#define ADCOPEN(Channel)  {CtrlADC6|=Channel;}
//close adc
#define ADCCLOSE(Channel) {CtrlADC6&=(~Channel);}
/***********************************************/
/***********************ADC PARA****************/
#define ADCA1X    	0
#define ADCA4X    	BIT0
#define ADCA8X    	BIT1
#define ADCA16X  	BIT0+BIT1
#define ADCA32X  	BIT0+BIT1+BIT2
#define ADCB1X    	0
#define ADCB4X    	BIT3
#define ADCB8X    	BIT4
#define ADCB16X  	BIT3+BIT4
#define ADCB32X  	BIT3+BIT4+BIT5
#define ADCU1X  	0
#define ADCU2X    	BIT6
#define ADCM1X    	0
#define ADCM2X    	BIT7

/***************************************************************
*    2     数据结构声明     
****************************************************************/




/***************************************************************
*    3    变量声明       
****************************************************************/





/***************************************************************
*    4     函数声明        
****************************************************************/
extern void Open_EmuPwr(uint8 mode);
extern void Close_EmuPwr(void);
extern void OpenCF(void);
extern void Close_Cf(void);
extern INT8U SetMeterCfgACKPM800K(unsigned long u32PMdatal ,unsigned int addr);
extern INT8U SetMeterCfgI(unsigned long u32PMdatal ,unsigned int addr);
extern INT8U SetMeterCfgACK(unsigned long u32PMdatal ,unsigned int addr);
extern uint32 ReadMeterParaACK(uint16 addr);


/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
//Open_EmuPwr(uint8 mode)    mode  define //
#define EMU_800k_MODE 		0x01
#define EMU_NORMALL_MODE	0x02
#define EMU_Sleep800k_MODE	0x04		//800k睡眠模式单独处理 


/***************************************************************
*    END
****************************************************************/
#endif

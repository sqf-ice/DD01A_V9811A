/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_RTC_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: RTC模块的硬件驱动C代码的声明文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#ifndef _Drv_RTC_H
#define _Drv_RTC_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/



/***************************************************************
*    2     数据结构声明     
****************************************************************/
typedef struct {
	uint8 Week;
	uint8 Year;
	uint8 Month;
	uint8 Day;
	uint8 Hour;
	uint8 Minute;
	uint8 Second;
} RTC_TYPE;




/***************************************************************
*    3    变量声明       
****************************************************************/



/***************************************************************
*    4     函数声明        
****************************************************************/
extern void Open_SecOut(void);
extern void Close_SecOut(void);
extern void Init_RTC(void);
extern void SetExtRTC(uint8 SEC,uint8 intRTC);
extern void Get_RTCTime(RTC_TYPE *pTime);
extern uint8 Set_RTCTime(RTC_TYPE *pTime);
extern void Clr_RTC_AdjParam(void);
extern void SetRTCNormal(int16 offset);
extern void Handl_RTC_Adj_per_minu(void);


/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/


/***************************************************************
*    END
****************************************************************/
#endif

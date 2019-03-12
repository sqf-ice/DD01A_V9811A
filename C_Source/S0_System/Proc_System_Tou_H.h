/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_System_Tou_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: 系统时标触发后的处理相关C代码的声明文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#ifndef _Proc_System_Tou_H
#define _Proc_System_Tou_H


/***************************************************************
*    1     立即数预定义   
****************************************************************/


/***************************************************************
*    2     数据结构声明     
****************************************************************/
typedef struct 
{
    RTC_TYPE   old_rtc;
    INT8U flag;  
} TOU_VAR;

//------------------------------------------------------------------------//
//                               数据结构声明
//------------------------------------------------------------------------//
typedef struct 
{
    INT8U u8_flg;          	           //清零标志  //
    INT8U u8_sec_cnt;		 //秒定时器 //
}AUTO_ClrMeter_VAR ;

/***************************************************************
*    3    变量声明       
****************************************************************/
extern RTC_TYPE gs_CurDateTime;
extern TOU_VAR  gs_tou_var;
extern  AUTO_ClrMeter_VAR  gs_auto_clrMeter_var;

/***************************************************************
*    4     函数声明        
****************************************************************/
extern void Proc_get_clock(void);
extern void Proc_init_tou_var(void);
extern void Proc_handl_tou_10ms(void);
extern void Proc_handl_tou_1s(void);

extern void  Initlize_Auto_ClrMeter_Var(void);
extern void  Proc_Auto_ClrMeter_Pre_sec(void);

extern uint8 button_flag;
extern uint8 MAGN_delay;
extern uint8 button_delay;
extern uint8 MD_delay;
extern uint8 MD_flag;
extern uint8 MAGN_flag;
extern uint8 MD_dis_delay;
extern INT8U Bill_Data;
extern uint8 KEY_READ_FLAG;
extern uint8 KEY_READ_DELAY;
extern uint8 IsModeTest ;
extern uint8 TestDelay ;

/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
//RTC 周日期定义  //
#define WEEK_Monday       1
#define WEEK_Tuesday       2
#define WEEK_Wednesday  3
#define WEEK_Thursday      4
#define WEEK_Friday           5
#define WEEK_Saturday     6
#define WEEK_Sunday     0


/***************************************************************
*    END
****************************************************************/

#endif

/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_System_Initialize_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: 系统复位初始化处理相关C代码的声明文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#ifndef _Proc_System_Initialize_H
#define _Proc_System_Initialize_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/
#define POWER_OFF  0
#define POWER_ON   1
#define POWER_RESET 2

/***************************************************************
*    2     数据结构声明     
****************************************************************/
// struct define of realy clock time //




/***************************************************************
*    3    变量声明       
****************************************************************/




/***************************************************************
*    4     函数声明        
****************************************************************/
extern void Proc_Initialize_INRam(void);
extern void Proc_sleep_when_reset(void);
extern void Proc_Initialize_systerm_normal_mode(void);
extern void Proc_Initialize_systerm_reset(void);
extern void Proc_judge_Power_down_process(void);
extern void sys_err(void);
extern void Proc_handle_when_systerm_reset(void);


/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/



/***************************************************************
*    END
****************************************************************/

#endif

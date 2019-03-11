/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name          : main.c
**Author		: maji
**date           : 2016-04-20
**description	: main 主程序C文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>

uint8  debug_tmp1;

/****************************************************************
Function:  		void main(void)   
Description: 		系统主函数
Input:
Parameters:     
Returns:                                
Others:         
****************************************************************/
void main(void)
{

    NOP();
    CLRWDT();     
    DisableINT();    				// disable interrupt function  //
    Proc_Initialize_systerm_reset();  
    EnableINT();
    NOP();    	
    while(1)
    {
	    CLRWDT();   // 清看门狗//
	    if(CheckPOWERUP()==TRUE)    
	    {
			CLRWDT();	 // 清看门狗//
	        if(gs_sys_run.back_fg&BIT0_FONT_FG_10MS)
	        {
	            gs_sys_run.back_fg &= ~BIT0_FONT_FG_10MS;
	            Proc_handl_tou_10ms();
	        }
			// 上电状态下的电源管理处理  //	
			Proc_judge_Power_down_process();	  
	        if(gs_sys_run.back_fg&BIT1_BACK_FG_RTC)
	        {
	            gs_sys_run.back_fg &= ~BIT1_BACK_FG_RTC;
	            Proc_handl_tou_1s();
	        }
			if(button_flag == 1||MAGN_flag == 1)
			{
				api_updated_LCDDisplayItem_per_second();
			}
			if(MD_flag == 1)
			{
				MD_flag = 0;
				api_clr_current_MD_data();
				CLRWDT();   // 清看门狗//
				api_deal_even_class_recode(ID_CLASS_I_resetMD,START);
				Full_SEG(0xFF);
				MD_dis_delay = 3;//延迟3秒
			}
			// 上电状态下的电源管理处理  //	
			Proc_judge_Power_down_process();	  
//			Drv_relay_run_check();   //继电器操作状态检查  //
			CLRWDT();	// 清看门狗//
	    }
		// 上电状态下的电源管理处理  //   
	    Proc_judge_Power_down_process();     
	}
}


/***************************************************************
*    END
****************************************************************/


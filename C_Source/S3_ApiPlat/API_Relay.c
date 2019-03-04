/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: API_Relay.c
**Author: maji
**date: 
**description: c code for 磁保持继电器操作
**note: memer type  DI03B
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2017-09-06   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>

/*********************************************************************************************************
**  全局变量区
********************************************************************************************************/
RELAY_MANAGE_VAR_TYPE gs_relay_manage_var;

/*********************************************************************************************************
**  常量数组表区
********************************************************************************************************/



/*********************************************************************************************************
**  代码区   start
********************************************************************************************************/




/*****************************************************************************
** Function name    :void Drv_relay_run_check(void)
**
** Description      :继电器命令检查处理
**
** Parameters       :NONE  
**
** Returned value	:NONE
**
** Note             :1 本函数在主循环中进行调用，可以实现对继电器状态的检查
**                  :2 
**----------------------------------------------------------------------------
** V01.01  MJ  2017-09-15
******************************************************************************/
void Drv_relay_run_check(void)
{
//	if((gs_relay_manage_var.u8_CMD==CMD_RELAY_OFF)&&( gs_relay_manage_var.u8_STA !=CMD_RELAY_OFF )) 
//	{
//		//合闸操作 //
//		if(gs_relay_manage_var.u8_tmr_opera == 0)
//		{
//			CTL_Relay_OFF(); 
//			gs_relay_manage_var.u8_tmr_opera = TMR_OPERA_RELAY;
//			//等待中断处理延时  //
//			while(gs_relay_manage_var.u8_tmr_opera!=0)
//			{
//				
//			}
//			CTL_Relay_FREE() ;
//			Lib_Delay_ms(20);  //此处的延时是为了确保合闸动作后的电源恢复，避免下次继电器动作间隔太小导致电源不足 //
//			//更改状态并保存   //
//			gs_relay_manage_var.u8_STA = CMD_RELAY_OFF;
//			mem_db_write(ADR_METER_PARAM1_RELAY_CMD, &gs_relay_manage_var.u8_CMD, 2, MEM_E2P1);
//			api_deal_even_class_recode(ID_CLASS_I_RELAY_OFF,START);  //生成合闸事件记录 //
//		}
//	}
//	else if((gs_relay_manage_var.u8_CMD==CMD_RELAY_ON)&&( gs_relay_manage_var.u8_STA !=CMD_RELAY_ON )) 
//	{
//		//拉闸操作 //
//		if(gs_relay_manage_var.u8_tmr_opera == 0)
//		{
//			CTL_Relay_ON(); 
//			gs_relay_manage_var.u8_tmr_opera = TMR_OPERA_RELAY;
//			//等待中断处理延时  //
//			while(gs_relay_manage_var.u8_tmr_opera!=0)
//			{
//				
//			}
//			CTL_Relay_FREE() ;
//			Lib_Delay_ms(20);  //此处的延时是为了确保合闸动作后的电源恢复，避免下次继电器动作间隔太小导致电源不足 //
//			//更改状态并保存   //			
//			gs_relay_manage_var.u8_STA = CMD_RELAY_ON;
//			mem_db_write(ADR_METER_PARAM1_RELAY_CMD, &gs_relay_manage_var.u8_CMD, 2, MEM_E2P1);		
//			api_deal_even_class_recode(ID_CLASS_I_RELAY_ON,START);	//生成拉闸事件记录 //
//		}			
//	}
//	else
//	{
//		return;
//	}

}


/*****************************************************************************
** Function name    :void Drv_relay_powerup_check(void)
**
** Description      :电能表上电初始化处理继电器程序
**
** Parameters       :NONE
**
** Returned value	:NONE
**
** Note             :1 初始化电能表先合闸，是为了可靠性考虑，在继电器故障时可以通过上下电恢复
**                  :2 注意，本函数只被上电初始化调用
**----------------------------------------------------------------------------
** V01.01  MJ  2017-09-15
******************************************************************************/
void Drv_relay_powerup_check(void)
{
	//1 硬件IO口初始化  //
	PIN_Relay_INITLIZE();
	CTL_Relay_FREE() ;
	Lib_Delay_ms(100);  //此处的延时是为了确保合闸动作前的电源稳定延时 //

	//上电预合闸  //
	CTL_Relay_OFF(); 
	Lib_Delay_ms(80);	
	CTL_Relay_FREE();

	//相关变量初始化  //
	mem_read( &gs_relay_manage_var.u8_CMD, ADR_METER_PARAM1_RELAY_CMD, 2, MEM_E2P1);
	gs_relay_manage_var.u8_STA =CMD_RELAY_OFF;//上电默认为合闸状态
	//读取继电器拉合闸发生的命令，若不是通讯引起的拉合闸，而是其他原因引起，///
	//例如超负荷等引起的拉闸，上电自动合闸处理 yang 2017-12-22////
	if((gs_relay_manage_var.u8_CMD != CMD_RELAY_OFF) &&(gs_relay_manage_var.u8_CMD != CMD_RELAY_ON))   
	{
		gs_relay_manage_var.u8_STA =CMD_RELAY_OFF;
		mem_db_write(ADR_METER_PARAM1_RELAY_CMD, &gs_relay_manage_var.u8_CMD, 2, MEM_E2P1);
	}
	gs_relay_manage_var.u8_tmr_opera =0;
	Lib_Delay_ms(100);  //此处的延时是为了确保合闸动作后的电源恢复，避免下次继电器动作间隔太小导致电源不足 //
    CLRWDT();   // 清看门狗//
}




/*****************************************************************************
** Function name    :INT8U Drv_relay_COMM_CMD_operating(INT8U *ptr_cmd)
**
** Description      :通讯处理层调用本函数处理继电器操作指令
**
** Parameters       :INT8U *ptr_cmd 通讯缓存区内的控制数据，2字节内容首地址   
**
** Returned value	:INT8U,命令控制数据检查不合法返回失败
**
** Note             :1 本函数只修改RAM区变量数据，不对继电器座实际操作
**                  :2 注意，本函数只被通讯处理程序调用
**----------------------------------------------------------------------------
** V01.01  MJ  2017-09-15
******************************************************************************/
INT8U Drv_relay_COMM_CMD_operating(INT8U *ptr_cmd)
{
	INT8U u8_cmd,u8_sta;
	
	u8_cmd = *ptr_cmd;
	u8_sta =  *(ptr_cmd+1);

			
	//输入命令数据判断  //
	if(u8_cmd != u8_sta )  return FALSE;    //数据格式不对  //

	if(u8_cmd ==CMD_RELAY_OFF )
		{
		//通讯合闸命令,强制系统合闸	//
		gs_relay_manage_var.u8_CMD = CMD_RELAY_OFF;
		gs_relay_manage_var.u8_STA = CMD_RELAY_ON;
		gs_relay_manage_var.u8_tmr_opera = 0;
		Drv_relay_run_check();   //继电器检查操作  //
		}
	else if(u8_cmd ==CMD_RELAY_ON)
		{
		//通讯合闸命令	,强制系统合闸//
		gs_relay_manage_var.u8_CMD = CMD_RELAY_ON;
		gs_relay_manage_var.u8_STA = CMD_RELAY_OFF;
		gs_relay_manage_var.u8_tmr_opera = 0;
		Drv_relay_run_check();  //继电器检查操作  //
		}
	else
		{
			return FALSE;	
		}
	
    CLRWDT();   // 清看门狗//
	return TRUE;
}



/*******************************************************************
*
*  end of file                                    
*
*******************************************************************/


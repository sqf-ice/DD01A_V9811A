/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_Data.c
**Author		: maji
**date			: 2016-04-20
**description	: 系统相关数据处理C文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#include <MyIncludes_H.h>



SYS_RUN_STATUS  gs_sys_run;
SYS_GLOBAL_VAR gs_sys_globaL_var;




// 0 版本号 常数 定义 //
/*********************************************************************************************************
**  版本号 常数 定义
********************************************************************************************************/

#if(meter_type == D_1P3W)
const SYS_PARM code sys_parm={
      0x00,METER_CONST_10WH_BCD,0x00,        //电表有功常数//
      'D','D','0','1','A','-','V','3','0','.','0','3',   //软件版本号  //
};
#endif
#if(meter_type == D_1P2W)
const SYS_PARM code sys_parm={
      0x00,METER_CONST_10WH_BCD,0x00,        //电表有功常数//
      'D','D','0','1','A','-','V','2','0','.','0','3',   //软件版本号  //
};
#endif
/*********************************************************************************************************
**  电能表参数1 初始化默认数据
********************************************************************************************************/
const  E2P_METER_PARAM1_MAP   code  default_meter_parm1_tab = 
{
    30,30,       //  MD_PARAM                                md_param;
#if (BILL_MAX_NUM>0)
    0x26,0x00,        //BILL_MONTH_SETTLED_PARAM     bill_ms_param; 
    0x30, //为月结算
#endif
    0,           //INT8U   EMU_fast_flg;    = 0X//EMU 加倍标识 //
    0X11,0X11,0X11,0X11,0X11,0X11,0XFF,0XFF,//INT8U   meter_id[16];
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0x11,0x11,0x11,0x11,0x11,0x11,				// 默认RF通讯地址12个1,共6字节 //	
    0,0,0,0,//INT8U   password[2][4];   //  2 级密码 //
    0,0,0,0,
    0,0,0,0,//INT8U   RF_password[2][4];   // RF 2 级密码 //
    0,0,0,0,
    METER_FACTORY_MARK,0,0,0,//INT8U   fac_cfg[4];          //  工厂信息配置字//
    0,28,   //INT8U   bat_level[2];          //  电池电压阀值//  29=2.90v
    0,0, // 有功组合方式字//
    0,0, // 无功组合方式字//
    0,	   //各通道负荷记录指针(1个通道)  	2       BIN  XXXX    
    0,	    //各通道负荷记录条数		   	2       BIN  XXXX    
    15,		   //各通道记录时间间隔（min）  	2       BCD  XXXX     
    1,1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,   // 各寄存器代码(1+9)   			10      BCD  XXXX         第一个位个数   
    0x123456,
    0,	//INT8U	relay_cmd;			 //继电器控制命令 =0x00 合闸 =0x3A 拉闸
    0,	//INT8U	relay_status;		 //继电器状态 =0x00合闸 =0x3A  拉闸
    0,	//INT8U	const_fast_flg; 	 //常数扩大标识 =0x3C 常数扩大10倍，其他常数保持
    0x65,0x34,0x12,// INT8U	EEPROM_Flag[3]; 	  // //  首次上电EEPROM检查状态字 !=0x653412标志当前为首次上电 
    0,          //INT16U  param1_csck;    //16位累加和校验值//
};



void Proc_sys_data_init(void)
{
    gs_sys_run.back_fg = 0;
    gs_sys_run.font_fg = 0;
    gs_sys_globaL_var.delay_emu =3;   // EMU 温度时间映射 //
    gs_sys_globaL_var.qf_time =0;     //无功脉宽控制//
    mem_read(&gs_sys_globaL_var.meter_factory_status, ADR_METER_PARAM1_FC_CFG, 1, MEM_E2P1);   //数据0=0X5A 代表工厂状态 // 
    mem_read(&gs_sys_globaL_var.fac_energy[0], ADR_METER_PARAM1_FAC_ENERGY, 2, MEM_E2P1);   
    mem_read(&gs_sys_globaL_var.Rac_energy[0], ADR_METER_PARAM1_REC_ENERGY, 2, MEM_E2P1);    
	//下电显示维持时间恢复
	mem_read(&gs_sys_globaL_var.sleep_sec.B08[0], ADR_DIS_PARAM_SLEEP_SEC, 4, MEM_E2P1);    

}



/***************************************************************
*    END
****************************************************************/


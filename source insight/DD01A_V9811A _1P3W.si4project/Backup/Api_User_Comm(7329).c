/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_User_Comm.c
**Author		: maji
**date			: 2016-04-21 
**description	: 通讯处理应用层相关函数程序C文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/



#include <MyIncludes_H.h>


/*******************************************************************************
* 全局变量定义区
*******************************************************************************/
uint8 *P_IEC_BUFF;    //通讯缓冲区指针 //
uint8 g_ch_comm;   //当前通讯通道号 //
UART_COMM_APP_VAR  gs_uart_iec_app_var[UART_CH_NUM];
COMM_DATA  comm_data;

/*******************************************************************************
* 常量表区code
*******************************************************************************/
const  INT8U Super_mima_iec[8] ={'4','5','6','1','2','3','0','0'};
const  INT8U meter_info[Meter_INFO_LENTH] = {'X','L','E','3','D','D','S','3','1','1','-','7','V','4','.','1',};	// 因为远红外，扩展为2400bps//
const  INT8U meter_info_IR[Meter_INFO_LENTH] = {'X','L','E','3','D','D','S','3','1','1','-','7','V','4','.','1',};	// 因为远红外，扩展为2400bps//
const  INT8U mima_info[MIMA_INFO_LENTH] = {'(','1','1','1','1','1','1','1','1',')',};

/*********************************************************************************************************
**  通讯ID表格
********************************************************************************************************/
 code IEC_COMDISTAB    IEC_ComDisTab[] =
{
	// 有功合相费率总//
	0x00010000, &gs_energy_user_data.us_val[EC_CH_AC_PT_TT][TARIFF_T].buf[0],	EN_R+P_RAM, 4,8,	//当前总电量
#if (BILL_MAX_NUM>0)
	0x00010001, ADR_BLOCK101_BILL1_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL ,4,8,	//上1月电量
	0x00010002, ADR_BLOCK102_BILL2_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//上2月电量
	0x00010003, ADR_BLOCK103_BILL3_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//上3月电量
	0x00010004, ADR_BLOCK104_BILL4_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//上4月电量
	0x00010005, ADR_BLOCK105_BILL5_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//上5月电量
	0x00010006, ADR_BLOCK106_BILL6_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//上6月电量
#endif
	//需量 总
	0x01010000, ADR_BLOCK01_MMD_L0_E2P+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1,9,18,//当前需量类数据 4+5 需量+时间
#if (BILL_MAX_NUM>0)
	0x01010001, ADR_BLOCK101_BILL1_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//上1月需量类
	0x01010002, ADR_BLOCK102_BILL2_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//上2月需量类
	0x01010003, ADR_BLOCK103_BILL3_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//上3月需量类
	0x01010004, ADR_BLOCK104_BILL4_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//上4月需量类
	0x01010005, ADR_BLOCK105_BILL5_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//上5月需量类
	0x01010006, ADR_BLOCK106_BILL6_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//上6月需量类
#endif
	//变量数据	//
	0x02010000,&gs_measure_var_data.gs_really[0].dw_u_val.B08[0],	EN_R+P_RAM, 2,4,	//合相电压 xxx.x		 YYY
	0x02010100,&gs_measure_var_data.gs_really[0].dw_u_val.B08[0],	EN_R+P_RAM, 2,4,	//A相电压 xxx.x		 YYY
	0x02020000,&gs_measure_var_data.gs_really[0].dw_i_val.B08[0],	EN_R+P_RAM, 3,6,	//合相电流	 XXX.XXX
	0x02020100,&gs_measure_var_data.gs_really[0].dw_i_val.B08[0],	EN_R+P_RAM, 3,6,	//A相电流   XXX.XXX
	0x02030000,&gs_measure_var_data.gs_really[0].dw_p_val.B08[0],	  EN_R+P_RAM,	3,6,	// TOTAL ACTIVE POWER	XX.XXXX 
	0x02030100,&gs_measure_var_data.gs_really[0].dw_p_val.B08[0],	  EN_R+P_RAM,	3,6,	// A相 ACTIVE POWER   XX.XXXX 
	0x02030200,&gs_measure_var_data.dw_p_n_val.B08[0],				 EN_R+P_RAM,	3,6,	// B相 ACTIVE POWER   XX.XXXX 
	0x02060000,&gs_measure_var_data.gs_really[0].dw_pf_val.B08[0],	 EN_R+P_RAM,2,4,	//合相功率因数  X.XXX
	0x02060100,&gs_measure_var_data.gs_really[0].dw_pf_val.B08[0],	 EN_R+P_RAM,2,4,	// A相功率因数  X.XXX
	0x02800001,&gs_measure_var_data.dw_i_n_val.B08[0],		EN_R+P_RAM, 3,6,	//N相电流   XXX.XXX
	0x02800002,&gs_measure_var_data.gs_really[0].w_freq_val.B08[0],    EN_R+P_RAM,2,4,	//频率  XX.XX
#if (TARIFF_MAX_NUM>1)
	0x02900001,&gs_current_triff_var.triff_No,	  EN_R+P_RAM,1,2,	//当前费率 1~4//
	0x02900002,&gs_current_triff_var.dayTab_No,    EN_R+P_RAM,1,2,	//当前日时段表 1~4//
	0x02900003,&gs_current_triff_var.weekTab_No,	EN_R+P_RAM,1,2, //当前周表 1~4//
	0x02900004,&gs_current_triff_var.seasonTab_No,	  EN_R+P_RAM,1,2,	//当前季节表 1~4//
#endif

	//表计信息///
	0x04000100,&gs_CurDateTime.Week,				EN_RW+P_RAM,7,14,	//注意日期，通讯读写顺序 YYMMDDWWhhmmss
	0x04000103,ADR_METER_PARAM1_MD, 			   EN_RW+P_E2P1,	2,	4,	//最大需量周期,滑差时间//
	0x04000401,ADR_METER_PARAM1_METER_ID,		EN_RW+P_E2P1,16,32, //通讯地址 表号
	0x04000402,ADR_METER_PARAM1_RF_COMM_ID,		EN_RW+P_E2P1,6,12, //RF通讯地址 yang 2017-12-21
	0x04000409,&sys_parm.meter_cons[0], 			EN_R+P_RAM,3,6, 	//常数	 // 	
#if (TARIFF_MAX_NUM>1)//2019-01-07 新增修改费率相关命令cxy
	0x04000801,ADR_BLOCK83_WEEK_TAB_E2P	,			EN_RW+P_E2P1,LEN_WEEK_TABLE_ONE,(2*LEN_WEEK_TABLE_ONE),	//周表起始地址(4个周表)	
	0x04000802,ADR_BLOCK83_WEEK_TAB_E2P+(1*LEN_WEEK_TABLE_ONE),		EN_RW+P_E2P1,LEN_WEEK_TABLE_ONE,(2*LEN_WEEK_TABLE_ONE),	//周表起始地址(4个周表)
	0x04000803,ADR_BLOCK83_WEEK_TAB_E2P+(2*LEN_WEEK_TABLE_ONE),		EN_RW+P_E2P1,LEN_WEEK_TABLE_ONE,(2*LEN_WEEK_TABLE_ONE),	//周表起始地址(4个周表)
	0x04000804,ADR_BLOCK83_WEEK_TAB_E2P+(3*LEN_WEEK_TABLE_ONE),		EN_RW+P_E2P1,LEN_WEEK_TABLE_ONE,(2*LEN_WEEK_TABLE_ONE),	//周表起始地址(4个周表) 

	0x04010000,ADR_BLOCK82_SEASON_TAB_E2P,	             EN_RW+P_E2P1,LEN_SEASON_TABLE, (2*LEN_SEASON_TABLE),    // 季节表 8*3  月日+周表号
	0x04010001,ADR_BLOCK84_DAY_TAB_E2P,				EN_RW+P_E2P1,LEN_DAY_TABLE_ONE, (2*LEN_DAY_TABLE_ONE),//时段表1  时段表起始地址(8个时段表，10个时段数)  288      BCD  hhnnNN          
	0x04010002,ADR_BLOCK84_DAY_TAB_E2P+(1*LEN_DAY_TABLE_ONE),		EN_RW+P_E2P1,LEN_DAY_TABLE_ONE, (2*LEN_DAY_TABLE_ONE),//时段表2  时段表起始地址(8个时段表，10个时段数)  288      BCD  hhnnNN          
	0x04010003,ADR_BLOCK84_DAY_TAB_E2P+(2*LEN_DAY_TABLE_ONE),		EN_RW+P_E2P1,LEN_DAY_TABLE_ONE, (2*LEN_DAY_TABLE_ONE),//时段表3  时段表起始地址(8个时段表，10个时段数)  288      BCD  hhnnNN          
	0x04010004,ADR_BLOCK84_DAY_TAB_E2P+(3*LEN_DAY_TABLE_ONE),		EN_RW+P_E2P1,LEN_DAY_TABLE_ONE, (2*LEN_DAY_TABLE_ONE),//时段表4  时段表起始地址(8个时段表，10个时段数)  288      BCD  hhnnNN
#endif

	0x04000B01,ADR_METER_PARAM1_SETL_DDHH,			EN_RW+P_E2P1,2,4,	//结算日
	0x04000C01,ADR_METER_PARAM1_PW0,					EN_W+P_E2P1,4,8,	//TTL 2级密码
	0x04000C02,ADR_METER_PARAM1_RF_PW0,					EN_W+P_E2P1,4,8,	//RF 2级密码
	0x04800003,ADR_METER_PARAM1_RELAY_CMD, 			   EN_RW+P_E2P1,	2,	4,//继电器命令及状态字//
    0x04000A01,	 ADR_METER_PARAM1_load_lptime_inv_var,  	EN_RW+P_E2P1,2,4,//记录间隔
    0x04000AA1,	 ADR_METER_PARAM1_load_egcode_var,            EN_RW+P_E2P1,10,20, //负荷表编码
    0x06010000,	 &LPRunPa.Num[0],				    EN_R+P_RAM,2,4, //已记录条数
			//显示类参数//
	0x04000302,ADR_DIS_PARAM_AUTO_SEC,			EN_RW+P_E2P1,1,2,	 //自动循显时间 
	0x04000303,ADR_DIS_PARAM_EC_FORM,		 EN_RW+P_E2P1,1,2,	  // 电量显示位数//
	0x04040100,ADR_DIS_PARAM_AUTO_TAB,		 EN_RW+P_E2P1,(DIS_MAX_ITEM+1),(2*(DIS_MAX_ITEM+1)), //自动循显表//
	0x04800001,&sys_parm.video[0],			EN_R+P_RAM,12,12,  //软件版本号(ASCII)


	// 工厂扩展类命令//
	0x80805001, 0x0000, 					EN_R+P_E2P1,12,24,  // 扩展命令 电量+继电器状态+日期时间 yang 2017-12-22// 
	0x08020100,0x0000,						EN_W+P_RAM,1,2, 		   //需量清零/手动复位//
	0x08020200,0x0000,						EN_W+P_RAM,1,2, 		   //=FF 事件记录清零  // 清开盖
	0x08020300,0x0000,						EN_W+P_RAM,1,2, 		   //=00 负荷记录清零  //
	0x08020400,0x0000,						EN_W+P_RAM,4,8, 		  //xxxxxxxx = 00000000时清除当前和历史电量需量数据//
	0x08020500,ADR_METER_PARAM1_FC_CFG, 	EN_RW+P_E2P1,4,8,		  //工厂出厂状态字，4字节  // 5A:厂内	//
	0x04000107,ADR_METER_PARAM1_BAT_LEVEL,	EN_RW+P_E2P1,	2,	4,	// 电池低压阀值  1 字节，BCD  X.X 
	//整机初始化 //
	0x0000FEFF,0x0000,							EN_W+P_RAM,2,4, 	// 初始化
	0xEE000109,ADR_BLOCK70_EMU_PARAM_E2P,			EN_RW+P_E2P1+W_FCT,32,64,//!校表参数区(U,I1,I2,P1,P2,Fre)
	0xEE000118,ADR_METER_PARAM1_EEPROM_Flag,	EN_RW+P_E2P1+W_FCT,3,6,//EEPROM首次上电标志位 首次上电 该标志位不等于0x653412，其他均为首次上电///
	//事件记录ID 拉闸//
	
	// 一类事件  //
	//需量复位   //
	0x03500000, ADR_BLOCK151_EVENI_resetMD_E2P,			    EN_R+P_E2P1, 2,4,	//总次数
	0x03500101, ADR_BLOCK151_EVENI_resetMD_E2P+2,		    EN_R+P_E2P1, 6,12, //最近一次档案记录  事件代码 开始时间 结束时间
	0x03500102, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 1*6),	EN_R+P_E2P1, 6,12,//最近二次档案记录  事件代码  开始时间 结束时间
	0x03500103, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 2*6),	EN_R+P_E2P1, 6,12,//最近三次档案记录  事件代码  开始时间 结束时间
	0x03500104, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 3*6),	EN_R+P_E2P1, 6,12,//最近二次档案记录  事件代码  开始时间 结束时间
	0x03500105, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 4*6),	EN_R+P_E2P1, 6,12,//最近三次档案记录  事件代码  开始时间 结束时间
	0x03500106, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 5*6),	EN_R+P_E2P1, 6,12,//最近二次档案记录  事件代码  开始时间 结束时间
	0x03500107, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 6*6),	EN_R+P_E2P1, 6,12,//最近三次档案记录  事件代码  开始时间 结束时间
	0x03500108, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 7*6),	EN_R+P_E2P1, 6,12,//最近二次档案记录  事件代码  开始时间 结束时间
	0x03500109, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 8*6),	EN_R+P_E2P1, 6,12,//最近三次档案记录  事件代码  开始时间 结束时间
	0x0350010A, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 9*6),	EN_R+P_E2P1, 6,12,//最近二次档案记录  事件代码  开始时间 结束时间

//	0x0350FFFF, ADR_BLOCK151_EVENI_resetMD_E2P+2,			EN_R+P_E2P1, 60,120,//最近二次档案记录  事件代码  开始时间 结束时间

	//密码认证错误
	0x03560000,  ADR_BLOCK153_EVENI_MMERR_E2P,		    EN_R+P_E2P1, 	2,4,	//总次数
	0x03560101,  ADR_BLOCK153_EVENI_MMERR_E2P+2,		EN_R+P_E2P1, 6,12, 		 //最近一次档案记录  事件代码 开始时间 结束时间
	0x03560102,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 1*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03560103,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 2*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03560104,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 3*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03560105,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 4*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03560106,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 5*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03560107,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 6*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03560108,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 7*6),	EN_R+P_E2P1 , 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03560109,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 8*6),	EN_R+P_E2P1 , 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x0356010A,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 9*6),	EN_R+P_E2P1 , 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间

//	0x0356FFFF,  ADR_BLOCK153_EVENI_MMERR_E2P+2,		EN_R+P_E2P1 , 60,120, ///最近二次档案记录  事件代码  开始时间 结束时间

	//时钟校准
	0x03570000,  ADR_BLOCK152_EVENI_setRtc_E2P,			    EN_R+P_E2P1, 	2,4,	//总次数
	0x03570101,  ADR_BLOCK152_EVENI_setRtc_E2P+2,	   	    EN_R+P_E2P1, 6,12, 		 //最近一次档案记录  事件代码 开始时间 结束时间
	0x03570102,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 1*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03570103,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 2*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03570104,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 3*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03570105,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 4*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03570106,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 5*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03570107,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 6*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03570108,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 7*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03570109,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 8*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x0357010A,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 9*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间

//	0x0357FFFF,  ADR_BLOCK152_EVENI_setRtc_E2P+2,		    	EN_R+P_E2P1, 60,120, 		 //最近一次档案记录  事件代码 开始时间 结束时间

	//E2P错误
	0x03580000,  ADR_BLOCK156_EVENII_EERR_E2P,		    EN_R+P_E2P1, 	2,4,	//总次数
	0x03580101,  ADR_BLOCK156_EVENII_EERR_E2P+2,	 	EN_R+P_E2P1, 6,12, 		 //最近一次档案记录  事件代码 开始时间 结束时间
	0x03580102,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 1*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03580103,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 2*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03580104,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 3*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03580105,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 4*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03580106,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 5*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03580107,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 6*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03580108,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 7*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03580109,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 8*6),	EN_R+P_E2P1, 6,12, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x0358010A,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 9*6),	EN_R+P_E2P1, 6,12, ///最近二次档案记录  事件代码  开始时间 结束时间

//	0x0358FFFF,   ADR_BLOCK156_EVENII_EERR_E2P+2,		    	EN_R+P_E2P1, 60,120, 		 //最近一次档案记录  事件代码 开始时间 结束时间

//  二类事件	//
	//电池欠压 特殊处理  只抄读发生时间//	
	0x03590000,   ADR_BLOCK162_EVENII_lowbattery_E2P,            EN_R+P_E2P1, 2,4,	//总次数
	0x03590001,   ADR_BLOCK162_EVENII_lowbattery_E2P+2,          EN_R+P_E2P1, 4,8,	//累计时间
	0x03590101,   ADR_BLOCK162_EVENII_lowbattery_E2P+6,		     EN_R+P_E2P1+CM_EBIL, 12,24,	 //最近一次档案记录  事件代码 开始时间 结束时间
	0x03590102,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 1*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近二次档案记录  事件代码  开始时间 结束时间
	0x03590103,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 2*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近三次档案记录  事件代码  开始时间 结束时间
	0x03590104,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 3*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近二次档案记录  事件代码  开始时间 结束时间
	0x03590105,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 4*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近三次档案记录  事件代码  开始时间 结束时间
	0x03590106,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 5*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近二次档案记录  事件代码  开始时间 结束时间
	0x03590107,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 6*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,//最近三次档案记录  事件代码  开始时间 结束时间
	0x03590108,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 7*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近二次档案记录  事件代码  开始时间 结束时间
	0x03590109,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 8*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近三次档案记录  事件代码  开始时间 结束时间
	0x0359010A,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 9*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,//最近二次档案记录  事件代码  开始时间 结束时间

//	0x0359FFFF,  ADR_BLOCK162_EVENII_lowbattery_E2P+6,	         EN_R+P_E2P1, 60,120,//开始记录  6*10次
	
	// 电源掉电  //	
	0x03380000,  ADR_BLOCK161_EVENII_PWN_E2P,                      EN_R+P_E2P1, 	2,4,	//总次数
	0x03380001,  ADR_BLOCK161_EVENII_PWN_E2P+2,                  EN_R+P_E2P1, 	4,8,	//累计时间
	0x03380101,  ADR_BLOCK161_EVENII_PWN_E2P+6,		        EN_R+P_E2P1+CM_EBIL, 12,24, 		 //最近一次档案记录  事件代码 开始时间 结束时间
	0x03380102,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 1*6),	 EN_R+P_E2P1+CM_EBIL, 12,24, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03380103,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 2*6),	 EN_R+P_E2P1+CM_EBIL, 12,24, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03380104,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 3*6),	 EN_R+P_E2P1+CM_EBIL, 12,24, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03380105,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 4*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x03380106,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 5*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03380107,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 6*6),	 EN_R+P_E2P1+CM_EBIL, 12,24, //最近三次档案记录  事件代码  开始时间 结束时间
	0x03380108,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 7*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///最近二次档案记录  事件代码  开始时间 结束时间
	0x03380109,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 8*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///最近三次档案记录  事件代码  开始时间 结束时间
	0x0338010A,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 9*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///最近二次档案记录  事件代码  开始时间 结束时间

//	0x0338FFFF,   ADR_BLOCK161_EVENII_PWN_E2P+6,		       EN_R+P_E2P1+CM_GETHER_EBIL, 120,240,		 //最近一次档案记录  事件代码 开始时间 结束时间

	//COVER_//	
	0x03450000,  ADR_BLOCK163_EVENII_COVER_E2P,           EN_R+P_E2P1, 	2,4,	//总次数
	0x03450001,  ADR_BLOCK163_EVENII_COVER_E2P+2,         EN_R+P_E2P1, 	4,8,	//总次数
	0x03450101,  ADR_BLOCK163_EVENII_COVER_E2P+6,		      EN_R+P_E2P1+CM_EBIL, 12,24,		 //最近一次档案记录  事件代码 开始时间 结束时间
	0x03450102,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 1*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近二次档案记录  事件代码  开始时间 结束时间
	0x03450103,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 2*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近三次档案记录  事件代码  开始时间 结束时间
	0x03450104,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 3*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近二次档案记录  事件代码  开始时间 结束时间
	0x03450105,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 4*6),  EN_R+P_E2P1+CM_EBIL, 12,24,///最近三次档案记录  事件代码  开始时间 结束时间
	0x03450106,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 5*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近二次档案记录  事件代码  开始时间 结束时间
	0x03450107,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 6*6),  EN_R+P_E2P1+CM_EBIL, 12,24,///最近三次档案记录  事件代码  开始时间 结束时间
	0x03450108,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 7*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近二次档案记录  事件代码  开始时间 结束时间
	0x03450109,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 8*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近三次档案记录  事件代码  开始时间 结束时间
	0x0345010A,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 9*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///最近二次档案记录  事件代码  开始时间 结束时间

//	0x0341FFFF,  ADR_BLOCK163_EVENII_NOL1_E2P+6,		       EN_R+P_E2P1+CM_GETHER_EBIL, 120,240,		 //最近一次档案记录  事件代码 开始时间 结束时间
	//L2 断相 //
//	0x03420000,  ADR_BLOCK164_EVENII_NOL2_E2P,                      EN_R+P_E2P1, 	2,4,	//总次数
//	0x03420001, ( ADR_BLOCK164_EVENII_NOL2_E2P+2),               EN_R+P_E2P1, 	4,8,	//总累计时间//
////	0x03420101,  ADR_BLOCK164_EVENII_NOL2_E2P+6,		         EN_R+P_E2P1+CM_EBIL, 	12,24,//最近一次档案记录  事件代码 开始时间 结束时间
//	0x03420102,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 1*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///最近二次档案记录  事件代码  开始时间 结束时间
//	0x03420103,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 2*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///最近三次档案记录  事件代码  开始时间 结束时间
//	0x03420104,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 3*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///最近二次档案记录  事件代码  开始时间 结束时间
//	0x03420105,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 4*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///最近三次档案记录  事件代码  开始时间 结束时间
//	0x03420106,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 5*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///最近二次档案记录  事件代码  开始时间 结束时间
//	0x03420107,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 6*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///最近三次档案记录  事件代码  开始时间 结束时间
//	0x03420108,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 7*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///最近二次档案记录  事件代码  开始时间 结束时间
//	0x03420109,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 8*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///最近三次档案记录  事件代码  开始时间 结束时间
//	0x0342010A,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 9*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///最近二次档案记录  事件代码  开始时间 结束时间

//	0x0342FFFF,  ADR_BLOCK164_EVENII_NOL2_E2P+6,		   EN_R+P_E2P1+CM_GETHER_EBIL, 	120,240,//最近一次档案记录  事件代码 开始时间 结束时间
	// 功率反向事件  //	
	0x03360000,  ADR_BLOCK164_EVENII_L1reverse_E2P,                       EN_R+P_E2P1, 	2,4,	//总次数
	0x03360001, ( ADR_BLOCK164_EVENII_L1reverse_E2P+2),                EN_R+P_E2P1, 	4,8,	//总累计时间//
	0x03360101,  (ADR_BLOCK164_EVENII_L1reverse_E2P+6),                EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//
	0x03360102, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+1*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//
	0x03360103,  (ADR_BLOCK164_EVENII_L1reverse_E2P+6+2*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//
	0x03360104, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+3*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//
	0x03360105, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+4*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//
	0x03360106, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+5*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//
	0x03360107,  (ADR_BLOCK164_EVENII_L1reverse_E2P+6+6*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//
	0x03360108, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+7*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//
	0x03360109, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+8*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//
	0x0336010A,  (ADR_BLOCK164_EVENII_L1reverse_E2P+6+9*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//开始时间+结束时间//

//	0x0336FFFF,  (ADR_BLOCK165_EVENII_L1reverse_E2P+6),                EN_R+P_E2P1+CM_GETHER_EBIL, 	120,240,	//开始时间+结束时间//

	// 上一次结算时间
	0x03700101 ,   ADR_BLOCK101_BILL1_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12,  // 年月日时分秒
	0x03700102 ,   ADR_BLOCK102_BILL2_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒
	0x03700103 ,   ADR_BLOCK103_BILL3_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒
	0x03700104 ,   ADR_BLOCK104_BILL4_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒
	0x03700105 ,   ADR_BLOCK105_BILL5_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒
	0x03700106 ,   ADR_BLOCK106_BILL6_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒 
//	0x03700107 ,   ADR_BLOCK107_BILL7_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒
//	0x03700108 ,   ADR_BLOCK108_BILL8_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒
//	0x03700109 ,   ADR_BLOCK109_BILL9_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒
//	0x0370010A ,   ADR_BLOCK110_BILL10_DATA_E2P ,     EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒
//	0x0370010B ,   ADR_BLOCK111_BILL11_DATA_E2P,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒 
//	0x0370010C ,   ADR_BLOCK112_BILL12_DATA_E2P,      EN_R+P_E2P1+CM_BILL, 6,12, // 年月日时分秒

//日冻结
	0x0506FF01,    0x0000,	  EN_R+P_E2P1, 18,36,
	0x0506FF02,    0x0000,	  EN_R+P_E2P1, 18,36,
	0x0506FF03,    0x0000,	  EN_R+P_E2P1, 18,36,
	0x0506FF04,    0x0000,	  EN_R+P_E2P1, 18,36,

	//通讯批量校表 6 //
	0x0000FEBB,0x0000,						EN_W+P_E2P1+W_FCT,2,4,//配置EMU 倍频 //
	0x80800803, 0x0000, 					EN_W+P_E2P1+W_FCT,2,4,  // EMU计量参数初始化 ，用默认参数赋值 // 
	0x0000FEB0, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,   //主回路 1.0L IB 误差和有效值校准// 
	0x0000FEB1, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,   //主回路 0.5L IB 误差校准// 
	0x0000FEB2, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,   //次回路1.0L IB 误差校准// 
	0x0000FEB3, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,   //次回路0.5L IB 误差校准// 
	0x0000FEB4, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,  //次回路0.5L IB 误差校准// 
	0x0000FEB5, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,  //次回路1.0L 5%IB 误差校准// 
};
//---------------------------------------------------------------------//
//功能:   计算由起始地址开始到F截止的长度//
//     0000F     return 4
//	*Addr 数据起始地址，num数据长度
//---------------------------------------------------------------------//
INT8U get_meter_id_asclen(INT8U *Addr,INT8U numbyte)
{
	INT8U i;
	INT8U asclen=0;
	for(i=0;i<numbyte;i++)
	{
		if((((*(Addr+i))>>4)&0x0F)==0x0F)
		{
			return asclen;
		}
		asclen++;
		if(((*(Addr+i))&0x0F)==0x0F)
		{
			return asclen;
		}
		asclen++;
	}
	return asclen;
}


/*****************************************************************
 name:          INT8U  get_comm_var( INT16U id )
 function:      get command information precess  
 input:         no            
 return:        GetComVar()
                =0    succeed  precess 
                =1    fail precess 
*****************************************************************/
INT8U  get_IEC_comm_var( INT32U id )
{
	INT16U i;  
    //不在表格内数据 负荷数据//
    if((id>=0x06010001)&&(id<=0x060105A0))//负荷数据最多记录900条   //SXL 2019-01-6
    {
		comm_data.addr =0;
		comm_data.Info.u16 = EN_R;		//不能或上
		comm_data.len = 0;
		comm_data.len_asc = 0;
		return TRUE;
    }
		 if((id>=0x060A0001)&&(id<=0x060A0120))//负荷数据块最多记录90条   //SXL 2019-01-6
    {
		comm_data.addr =0;
		comm_data.Info.u16 = EN_R;		//不能或上
		comm_data.len = 0;
		comm_data.len_asc = 0;
		return TRUE;
    }
   for( i = 0 ; i <dim(IEC_ComDisTab) ; i++ )
	{
	    if( id == IEC_ComDisTab[i].ComID )
	    {
	        comm_data.addr = IEC_ComDisTab[i].Addr;
	        comm_data.Info.u16 = IEC_ComDisTab[i].ComInfo;
	        comm_data.len = IEC_ComDisTab[i].Len;
	        comm_data.len_asc = IEC_ComDisTab[i].Len_Asc;
	        return TRUE;
	    }
	}
	return FALSE;	

	

/*   INT16U i;  

    //不在表格内数据//
    if((id>=0x06010001)&&(id<=0x060116A9))
    {
	comm_data.addr =0;
	comm_data.Info.u16 = EN_R;		//不能或上
	comm_data.len = 0;
	comm_data.len_asc = 0;
	return 1;
    }

    if((id>=0x06012001)&&(id<=0x06012245))
    {
	comm_data.addr =0;
	comm_data.Info.u16 = EN_R;		//不能或上
	comm_data.len = 0;
	comm_data.len_asc = 0;
	return 1;
    }

    //表格内数据分区域查表//
    
	if(id<0x01000000) //电量区域
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab) ; i++ )
	    {
	        if( id == IEC_ComDisTab[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab[i].ComInfo;
	            comm_data.len = IEC_ComDisTab[i].Len;
	            comm_data.len_asc = IEC_ComDisTab[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x02000000)//需量区
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab1) ; i++ )
	    {
	        if( id == IEC_ComDisTab1[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab1[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab1[i].ComInfo;
	            comm_data.len = IEC_ComDisTab1[i].Len;
	            comm_data.len_asc = IEC_ComDisTab1[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x03000000)//系统变量区
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab2) ; i++ )
	    {
	        if( id == IEC_ComDisTab2[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab2[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab2[i].ComInfo;
	            comm_data.len = IEC_ComDisTab2[i].Len;
	            comm_data.len_asc = IEC_ComDisTab2[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x04000000)//事件记录区
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab3) ; i++ )
	    {
	        if( id == IEC_ComDisTab3[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab3[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab3[i].ComInfo;
	            comm_data.len = IEC_ComDisTab3[i].Len;
	            comm_data.len_asc = IEC_ComDisTab3[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x05000000)//系统参数区
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab4) ; i++ )
	    {
	        if( id == IEC_ComDisTab4[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab4[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab4[i].ComInfo;
	            comm_data.len = IEC_ComDisTab4[i].Len;
	            comm_data.len_asc = IEC_ComDisTab4[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x09000000)//系统参数区
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab5) ; i++ )
	    {
	        if( id == IEC_ComDisTab5[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab5[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab5[i].ComInfo;
	            comm_data.len = IEC_ComDisTab5[i].Len;
	            comm_data.len_asc = IEC_ComDisTab5[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}

    return FALSE;*/
}

/*****************************************************************
 name:          uchar  GetComVar( uchar* Addr, uint* Info )
 function:      get command information precess  
 input:         no            
 return:        GetComVar()
                =0    succeed  precess 
                =1    fail precess 
*****************************************************************/
INT8U SecurityCK(void)
{
	INT8U secType;
	secType = comm_data.Info.B08[0]&0x07; //权限检测//
	/*
	if(secType ==(W_FCT>>8))
	{
		if(ADJA_KEY)	// 0 短接 1 未短接
		{
			return (mmover);
		}
	}
    */
	return (mmok);
}

//-------------------------------------------------------------------------------//
//  Function:      INT8U IEC_Cmd_Password_Deal(INT8U* sptr,INT8U len) 
//  Description:   iec 通讯密码指令处理程序
//  Parameters:    
//  Returns:        无                          
//  Others:         
//-------------------------------------------------------------------------------//
INT8U IEC_Cmd_Password_Deal(INT8U* sptr)
{
  INT8U tmp_8u_01[4];
  INT8U tmp_8u_02[8];

    if((gs_uart_iec_app_var[g_ch_comm].iec_status & (COMM_REQUIST_OK_IEC+COMM_INFO_OK_IEC)) != 0x03)   return(FALSE);

     mem_read(&tmp_8u_01[0], ADR_METER_PARAM1_PW0, 4, MEM_E2P1);
     Lib_BCD_AscA(&tmp_8u_02[0], &tmp_8u_01[0], 4);
	 

    if(Lib_Cmp_TwoArry((sptr+5), &tmp_8u_02[0], 8) == 0x00)  
    {
         gs_uart_iec_app_var[g_ch_comm].iec_status |= COMM_MIMA_OK_IEC;  // 给出请求命令成功标识 //
         Get_ACK_Answer(sptr);
         gs_uart_iec_app_var[g_ch_comm].len = 1;
         return TRUE;
    }
    else
    {
        if(Lib_Cmp_TwoArry((sptr+5), &Super_mima_iec[0], 8) == 0x00) 
        {
            gs_uart_iec_app_var[g_ch_comm].iec_status |= COMM_MIMA_OK_IEC;  // 给出请求命令成功标识 //
            Get_ACK_Answer(sptr);
            gs_uart_iec_app_var[g_ch_comm].len = 1;
            return TRUE;
        }
    }
    api_deal_even_class_recode(ID_CLASS_I_MMERR,START); //  
    return FALSE;
}


//-------------------------------------------------------------------------------//
//  Function:      INT8U IEC_Cmd_Read_Deal(INT8U* sptr,INT8U len)
//  Description:   iec 通讯读指令处理程序
//  Parameters:    
//  Returns:        无                          
//  Others:         
//-------------------------------------------------------------------------------//
INT8U IEC_Cmd_Read_Deal(INT8U* sptr)
{
 INT8U i;
 INT8U  mem_type;
 INT8U  data_buff[128];
 INT8U  *ptr_adr;
 INT16U ctmp1;
 ST_U32_U08  long_tmp;
 ST_U16_U08	 LPRecStart,LPRecEnd;

SUM_MMD_DATA_UNIT t;
NEW_MMD_DATA_UNIT te;
    if((gs_uart_iec_app_var[g_ch_comm].iec_status &(COMM_REQUIST_OK_IEC+COMM_INFO_OK_IEC+COMM_MIMA_OK_IEC)) != 0x07)   return(FALSE);	
    //get comm DI1 DI0
     Lib_Asc_BCDA(&comm_data.di1_di0.B08[0], sptr+4,8);

    //get other comm fram data 	
    if(get_IEC_comm_var( comm_data.di1_di0.u32) == 0 )   return(FALSE);
    if( ( comm_data.Info.u16& EN_R) == 0 )  return(FALSE);
    //  获取数据的存储类型 //
    mem_type = (comm_data.Info.B08[0]>>3)&0x07;   	
    //数据域清零并设置发送长度
    Lib_Set_String((sptr+2), '0', comm_data.len_asc);  

    ptr_adr = comm_data.addr;

      //    // 
	//不在表格内ID处理 单条负荷ID 
	if((comm_data.di1_di0.u32>=0x06010001)&&(comm_data.di1_di0.u32<=0x060105A0))
	{
		ctmp1 = (INT16U)(comm_data.di1_di0.u32 - 0x06010001);
		if(ctmp1 < LPRunPa.Num[0])	// 读取ID 小于于已记录数
		{
			comm_data.len = LPRec_Read_1(sptr+2, ctmp1);
			comm_data.len_asc = 2*comm_data.len;	
		}
		else
		{
//			comm_data.len = LPRec_Read_1(sptr+2, ctmp1);
//			comm_data.len_asc = 2*comm_data.len;	
//			Lib_Set_String((sptr+2), 'F', comm_data.len_asc); 
			comm_data.len_asc = 0;	
		}


		// 成功后数据组帧 //
		//api_deal_even_class_recode(ID_CLASS_I_COMM,START);
		gs_uart_iec_app_var[g_ch_comm].len= (comm_data.len_asc+5);
		*(sptr+0) = STX;
		*(sptr+1) = '(';
		CLRWDT(); *(sptr+2+comm_data.len_asc) = ')';
		*(sptr+gs_uart_iec_app_var[g_ch_comm].len-2) =ETX;
	CLRWDT(); 	*(sptr+gs_uart_iec_app_var[g_ch_comm].len-1) = Get_Bcc_Chk(sptr, 1, gs_uart_iec_app_var[g_ch_comm].len-2);

		return(TRUE); 
	}
	
	
	if((comm_data.di1_di0.u32>=0x060A0001)&&(comm_data.di1_di0.u32<=0x060A0120))//   //SXL 2019-01-6
  {
		LPRecStart.u16 =(INT16U)((comm_data.di1_di0.u32 - 0x060A0001)*BLOCK_MAX_LEN);
		LPRecEnd.u16 =LPRecStart.u16+BLOCK_MAX_LEN;
		
		if(LPReadPara.ReadLoraFlag == FALSE )
		{			
			LPReadPara.Original_FrameNum = LPRunPa.Num[0] ;
			LPReadPara.Original_Ptr = LPRunPa.Ptr[0] ;
		} 		
		
		comm_data.len_asc  = LPRec_Read_Block(sptr+2,LPRecStart.u16, LPRecEnd.u16);
	  	 
		
		if(LPReadPara.ReadLoraFlag == FALSE )	LPReadPara.ReadLoraFlag = TRUE; 
			
		gs_uart_iec_app_var[g_ch_comm].len= (comm_data.len_asc+5);
		CLRWDT(); 
		*(sptr+0) = STX;
		*(sptr+1) = '(';CLRWDT(); 
		*(sptr+2+comm_data.len_asc) = ')';
		CLRWDT(); *(sptr+gs_uart_iec_app_var[g_ch_comm].len-2) =ETX;
		*(sptr+gs_uart_iec_app_var[g_ch_comm].len-1) = Get_Bcc_Chk(sptr, 1, gs_uart_iec_app_var[g_ch_comm].len-2);
CLRWDT(); 
		return(TRUE); 
		
		}
	
    // 根据读取数据的ID项目做分支处理 //
    switch(comm_data.di1_di0.u32)
    {	
		case 0x02010000:    //电压 源数据为HEX32，0.1V量纲，需要转换为BCD码格式,XXX.XV //
		case 0x02010100:    //电压 源数据为HEX32，0.1V量纲，需要转换为BCD码格式,XXX.XV //
			Lib_long_bcd4(&long_tmp.B08[0],gs_measure_var_data.gs_really[0].dw_u_val.u32);
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[2], 2);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;
		
		case 0x02020000:    //L电流 源数据为HEX32，0.001A量纲，需要转换为BCD码格式,XXX.XXXA //
		case 0x02020100:    //L电流 源数据为HEX32，0.001A量纲，需要转换为BCD码格式,XXX.XXXA //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].dw_i_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;
		
		case 0x02800001:    //N电流 源数据为HEX32，0.001A量纲，需要转换为BCD码格式,XXX.XXXA //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.dw_i_n_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;
		
		case 0x02030000:    //L   功率源数据为HEX32，0.0001KW量纲，需要转换为BCD码格式,XX.XXXXkW //
		case 0x02030100:	//功率源数据为HEX32，0.0001KW量纲，需要转换为BCD码格式,XX.XXXXkW //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].dw_p_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;
		
		case 0x02030200:    //N功率源数据为HEX32，0.0001KW量纲，需要转换为BCD码格式,XX.XXXXkW //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.dw_p_n_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;

		case 0x02060000:	//功率因数数据为HEX16，0.001，需要转换为BCD码格式0.001 //
		case 0x02060100:    //功率因数数据为HEX16，0.001，需要转换为BCD码格式0.001 //
			Lib_word_bcd2(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].dw_pf_val.u16));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[0], 2);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;

		case 0x02800002:    //电网频率数据为HEX16，0.01Hz，需要转换为BCD码格式xx.xx //
			Lib_word_bcd2(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].w_freq_val.u16));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[0], 2);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;

		case 0x04000100://日期及星期(其中0代表星期天)YYMMDDWWhhmmss
			Lib_Copy_Str_TwoArry(&data_buff[0], ptr_adr, comm_data.len);
			data_buff[7] = data_buff[0];	//ww
			data_buff[0] = data_buff[1];	//YY
			data_buff[1] = data_buff[2];	//MM
			data_buff[2] = data_buff[3];	//DD
			data_buff[3] = data_buff[7];	//WW
			data_buff[4] = data_buff[4];	//hh
			data_buff[5] = data_buff[5];	//mm
			data_buff[6] = data_buff[6];	//ss
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;

		case 0x04000401://表地址 表号 NNNNNNNNNNNN
			mem_read(&data_buff[0], (INT16U)comm_data.addr,  comm_data.len,  mem_type );
			comm_data.len_asc = get_meter_id_asclen(&data_buff[0], 16);
			comm_data.len = comm_data.len_asc>>1;
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len);
		break;
		
		case 0x04800001: //软件版本号 已是ASC码
			Lib_Copy_Str_TwoArry(sptr+2, ptr_adr, comm_data.len);
		break;		
		
		case 0x80805001:	//当前月总电量+继电器状态+表计日期时间 //
			//当前月总电量
			mem_read(&data_buff[10], (INT16U)&gs_energy_user_data.us_val[0][0].buf[0], LEN_EC_UNIT, MEM_RAM);	  //获取当前数据 //
			api_get_energy_LCD(&data_buff[10], 0x62,&data_buff[0]);
			Lib_BCD_AscA(sptr+2, &data_buff[0], 4); 
			//继电器状态
			mem_read(&data_buff[0], (INT16U)ADR_METER_PARAM1_RELAY_CMD, 2, MEM_E2P1);	  //获取当前数据 //
			Lib_BCD_AscA(sptr+2+8, &data_buff[0], 2); 
			Lib_Copy_Str_TwoArry(&data_buff[0], &gs_CurDateTime.Year, 6);
			Lib_BCD_AscA(sptr+2+8+4, &data_buff[0], 6); 
		break;

		case 0x0506FF01: //冻结1
		case 0x0506FF02: //冻结2
		case 0x0506FF03: //冻结3
		case 0x0506FF04: //冻结4
			  i = comm_data.di1_di0.B08[3];
			  mem_read(&data_buff[10], &energy_data_array[i].buf[0], LEN_BLOCK_4Energy_E2P/4,MEM_RAM);//电量
			  api_get_energy_LCD(&data_buff[10], 0x62,&data_buff[0]);
			  Lib_BCD_AscA(sptr+2+10, &data_buff[0], 4);	
				
			  mem_read(&te, &st_mmd_unit_array[i].buf[0], LEN_NEW_MMD_UNIT, MEM_RAM);//需量
			  te.val.val_hex32 /=100;
			  data_buff[0] = te.val.val_hex32/1000000;
			  data_buff[1] = te.val.val_hex32%1000000/10000;
			  data_buff[2] = te.val.val_hex32%10000/100 ;
			  data_buff[3] = te.val.val_hex32%100  ;
			  Lib_BCD_AscA(sptr+2+10+8,&data_buff[0],4);
			  //Lib_BCD_AscA(sptr+2+10+8,&te.buf[0],4);
			 
			  mem_read(&t, &sum_st_mmd_unit_array[i].buf[0], LEN_SUM_MMD_UNIT, MEM_RAM);//累计需量
			  t.val.val_hex32 /=100;
			  data_buff[0] = t.val.val_hex32/1000000;
			  data_buff[1] = t.val.val_hex32%1000000/10000;
			  data_buff[2] = t.val.val_hex32%10000/100; 
			  data_buff[3] = t.val.val_hex32%100;
			  Lib_BCD_AscA(sptr+2+10+8,&data_buff[0],4);
			 // Lib_BCD_AscA(sptr+2+10+8+8,&t.buf[0],4);
			  data_buff[0] = te.val.date_rtc[0];	//YY
			  data_buff[1] = te.val.date_rtc[1];	//MM
			  data_buff[2] = te.val.date_rtc[2];	//DD
			  data_buff[3] = te.val.date_rtc[3];	//hh
			  data_buff[4] = te.val.date_rtc[4];	//mm
			  Lib_BCD_AscA(sptr+2, &data_buff[0], 5);
			  Lib_BCD_AscA(sptr+2+10+8+8+8,&t.buf[4],1);//次数
		break;
		
        default:
			////////////////电能量特殊处理/////////////////////////////
			if(comm_data.di1_di0.B08[0]==0x00)
			{
				comm_data.len = LEN_EC_UNIT;   // 修改获取长度//
			}
			///////////////////////////////////////////////////////////////////

			if((comm_data.Info.u16&CM_ALL) == CM_BILL)
			{
				mem_read(&data_buff[0],api_get_bill_record_addr((INT16U)comm_data.addr), comm_data.len,  mem_type );
			}
			else if((comm_data.Info.u16&CM_ALL) == CM_EBIL)//事件记录类数据
	  	{
        mem_read(&data_buff[0], comm_data.addr,     (comm_data.len>>1),  mem_type );
        mem_read(&data_buff[6], comm_data.addr+60,  (comm_data.len>>1),  mem_type );
	    }
			
			else 
			{
				if(mem_type==MEM_RAM)
				{
					Lib_Copy_Str_TwoArry(&data_buff[0], ptr_adr, comm_data.len);
				}
				else
				{
					mem_read(&data_buff[0],(INT16U)comm_data.addr,comm_data.len,mem_type );
				}
			}
			////////////////电能量特殊处理/////////////////////////////
			if(comm_data.di1_di0.B08[0]==0x00)
			{
				Lib_Copy_Str_TwoArry(&data_buff[10], &data_buff[0], LEN_EC_UNIT);
				api_get_energy_LCD(&data_buff[10], 0x62, &data_buff[0]) ;
				comm_data.len=4;
			}
			///////////////////////////////////////////////////////////////////

			////////////////最大需量特殊处理/////////////////////////////
			if(comm_data.di1_di0.B08[0]==0x01)
			{  
				Lib_Copy_Str_TwoArry(&long_tmp.B08[0], &data_buff[0], 4);
				Lib_long_bcd4(&data_buff[0],long_tmp.u32);
			}
			
			
			
			
			
			/////////////////////////////////////////////////////////////////// 		   
			if(comm_data.len_asc == 5)
			{
				*(sptr+2) = Lib_BtoA(data_buff[0]&0x0f);
				Lib_BCD_AscA(sptr+3, &data_buff[1], 2);   
			}
			else
			{
				Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
			}
            break;	
    }

    // 成功后数据组帧 //
    gs_uart_iec_app_var[g_ch_comm].len= (comm_data.len_asc+5);
    *(sptr+0) = STX;
    *(sptr+1) = '(';
    *(sptr+2+comm_data.len_asc) = ')';
    *(sptr+gs_uart_iec_app_var[g_ch_comm].len-2) =ETX;
    *(sptr+gs_uart_iec_app_var[g_ch_comm].len-1) = Get_Bcc_Chk(sptr, 1, gs_uart_iec_app_var[g_ch_comm].len-2);
	
    return(TRUE); 
}


//-------------------------------------------------------------------------------//
//  Function:      INT8U IEC_Cmd_Write_Deal(INT8U* sptr,INT8U len)
//  Description:   iec 通讯写指令处理程序
//  Parameters:   len为设置数据的实际长度
//  Returns:        无                          
//  Others:         
//-------------------------------------------------------------------------------//
INT8U IEC_Cmd_Write_Deal(INT8U* sptr,INT8U len)
{
// INT8U tmp_8u_01[16];
 INT8U  mem_type;
 INT8U  data_buff[70];
 INT8U	sectemp;
 INT8U i,MeterID_LenTmp; //表号  有效长度 asc
 INT8U  *ptr_adr;
 
    if((gs_uart_iec_app_var[g_ch_comm].iec_status &(COMM_REQUIST_OK_IEC+COMM_INFO_OK_IEC+COMM_MIMA_OK_IEC)) != 0x07)   return(FALSE);
    //get comm DI1 DI0
    Lib_Asc_BCDA(&comm_data.di1_di0.B08[0], sptr+4,8);
    //get other comm fram data 
    if(get_IEC_comm_var( comm_data.di1_di0.u32) == 0 )   return(FALSE);

    if( ( comm_data.Info.u16& EN_W) == 0 )  return(FALSE);
    //  获取数据的存储类型 //
    mem_type = (comm_data.Info.B08[0]>>3)&0x07;   	
    sectemp = SecurityCK();					//权限判断
    if(sectemp == mmover)   return(FALSE);

    //数据域清零并检查接收的数据长度合法性 //
    Lib_Set_String(&data_buff[0], 0x00, comm_data.len); 
    if(comm_data.di1_di0.u32== 0x04000401) //除设置表号外.都需要对比数据长度
    {
        if((len > comm_data.len_asc+16)||(len < 16))
        return(FALSE);
    }
    else
    {
        if(len != (comm_data.len_asc+16))	   
        return(FALSE);
    }

    ptr_adr = comm_data.addr;

    // 根据设置数据的ID项目做分支处理 //
    switch(comm_data.di1_di0.u32)
    {

        case 0x04000100://日期及星期(其中0代表星期天)YYMMDDWWhhmmss  
            Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
            gs_CurDateTime.Year= data_buff[0];
            gs_CurDateTime.Month= data_buff[1];
            gs_CurDateTime.Day= data_buff[2];
            gs_CurDateTime.Week= data_buff[3];
            gs_CurDateTime.Hour= data_buff[4];
            gs_CurDateTime.Minute= data_buff[5];
            gs_CurDateTime.Second= data_buff[6];
            EA=0;
            Set_RTCTime(&gs_CurDateTime);
            EA=1;
            mem_db_write(ADR_METER_VAR_RTC, &gs_CurDateTime.Week, 7, MEM_E2P1);
				    api_deal_even_class_recode(ID_CLASS_I_setRtc,START);
            api_init_md_data_ram();
		#if (TARIFF_MAX_NUM>0)
            api_update_triffNo_pre_minute();
		#endif
            break;
			

            //表号特殊处理
            case 0x04000401://表地址 表号 16个字节 自适应,如设奇数，则前补0
                MeterID_LenTmp = (len-16);	//有效表号  长度
                if(((len-16)%2)!=0)	//奇数则前面补一个0
                {
	                for(i=(len-16);i>0;i--)
	                {
		                *(sptr+13+i) = *(sptr+13+i-1) ;	//向后挪1位
	                }
	                *(sptr+13)= '0';					//高位补0
	                MeterID_LenTmp = (len-16) +1;
                }
				comm_data.len_asc = MeterID_LenTmp;
				Lib_Set_String(&data_buff[0], 0xFF, 16);
				Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)			
				mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);		//写入应该是16字节，而不是设置的长度
                break;

			
		case 0x04000B01://结算日DDHH 4
                Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
                if((data_buff[0]<0x01)||(data_buff[0]>0x28)||(data_buff[1]>0x23))		//错误判断
                {
                    return(FALSE);
                }
                mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
                break;
//		case 0x04800003://拉闸合闸操作//)
//				Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);	 //AtoB(INT8U Asc)
//				if(Drv_relay_COMM_CMD_operating(&data_buff[0])==FALSE)	
////				{
//					return(FALSE);
//				}
//				break;	

		case 0x04000103://需量参数 PERIOD : SLID   4  //修改后需量重新计算 //
                Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
                if(((data_buff[0]%data_buff[1]) !=0)||(data_buff[0]>30)||(data_buff[0]==0)||(data_buff[1]==0))		//错误判断//
                {
                    return(FALSE);
                }
                mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
                api_init_md_data_ram();
                break;


///////////误差校表//////////////////
		case 0x0000FEB0: 
		case 0x0000FEB1:
		case 0x0000FEB2:
		case 0x0000FEB3:
		case 0x0000FEB4:
		case 0x0000FEB5:
			Measure_Electric_Err_Gain(sptr);//必须先校准误差，延时1秒以上校准电参量才准//	
		break;
		//校表初始化(参数清零)
		case 0x80800803:		
			if((*(sptr+13)=='3')&&(*(sptr+14)=='3')&&(*(sptr+15)=='3'))  
			{
				if((*(sptr+16)=='3'))					// clear all
				{
					gs_adj_emu_param.w1gain=0;
					gs_adj_emu_param.p1cal=0;
					gs_adj_emu_param.watt1os=0;
					gs_adj_emu_param.irms1os=0;
					gs_adj_emu_param.w2gain=0;
					gs_adj_emu_param.p2cal=0;
					gs_adj_emu_param.watt2os=0;
					gs_adj_emu_param.irms2os=0;
					gs_adj_emu_param.urmsos=0;
					CLRWDT();
				}
				if((*(sptr+16)=='2'))					// 清第二路交表参数
				{
					gs_adj_emu_param.w1gain=0;
					gs_adj_emu_param.p1cal=0;
					gs_adj_emu_param.watt1os=0;
					gs_adj_emu_param.irms1os=0;
					gs_adj_emu_param.urmsos=0;
					CLRWDT();
				}
				Save_EMU_AdjParam();
				Check_EMU_AdjParam();//  恢复默认数据//
			}
			else
			{
				return FALSE;
			}
			break;	

		case 0x0000FEBB://计量倍频10倍或 //	
			if((*(sptr+13)=='0')&&(*(sptr+14)=='0')&&(*(sptr+15)=='1')&&(*(sptr+16)=='6')) 
			{
				//倍频10倍，常数为10000 //
				data_buff[0] = 0x3C;	 // 代表倍频10倍 //
				mem_db_write(ADR_METER_PARAM1_CONST_FAST_FLG, &data_buff[0], 1, MEM_E2P1);
				Check_EMU_AdjParam();//  恢复默认数据//
			}
			else if((*(sptr+13)=='0')&&(*(sptr+14)=='1')&&(*(sptr+15)=='B')&&(*(sptr+16)=='8')) 
			{
				//倍频恢复，常数为1000 //
				 data_buff[0] = 0;	   // 代表倍频10倍 //
				mem_db_write(ADR_METER_PARAM1_CONST_FAST_FLG, &data_buff[0], 1, MEM_E2P1);
				Check_EMU_AdjParam();//  恢复默认数据//
			}
			else
			{
				return FALSE;
			}
			break;
			
		case 0x08020200://NN=FF，清除所有通道；//
			if((*(sptr+13)=='F')&&(*(sptr+14)=='F'))	//YYY
			{
				api_clr_even_by_comm();	
				CLRWDT();
			}
			else
			{
				return(FALSE);
			}
			break;
		
			case 0x08020100://只清当前需量清零//
				Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
				if((data_buff[0]!=0))  return(FALSE);
				api_clr_current_MD_data();
				api_deal_even_class_recode(ID_CLASS_I_resetMD,START);
			CLRWDT();
			break;

	    case 0x08020300:
			// Asc_BCDA( &data_buff[0],(sptr+13), 1*2);
            // 一个通道负荷记录指针、记录个数 
            if((*(sptr+13)=='0')&&(*(sptr+14)=='0'))	//YYY
			{
				LoadProRst();
				CLRWDT();
			}
			else
			{
				return(FALSE);
			}
			break;
			
			
		case 0x08020400://电量清零(电表总清(电量+需量)) //
			Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
			if((data_buff[0]!=0)||(data_buff[1]!=0)||(data_buff[2]!=0)||(data_buff[3]!=0))  return(FALSE);
			CLRWDT();
			api_clr_current_energy_data();
			CLRWDT();
			api_clr_current_MD_data();
			CLRWDT();
			#if (BILL_MAX_NUM>0)
			api_clr_bill_data();
			#endif
			gs_uart_iec_app_var[g_ch_comm].delay_10ms = 0; // 
			LoadProRst();
			CLRWDT();
			api_clr_even_by_comm(); 
			CLRWDT();
			api_chg_LCDDisplay_adj_item(DIS_DATA_CLR);   // 液晶显示 //
			break;

		case 0x08020500:// 工厂模式//	
			Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);	 //AtoB(INT8U Asc)
			gs_sys_globaL_var.meter_factory_status = data_buff[0] ; //更新状态字
			mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
			break;
			
		case 0xEE000118://EEPROM标志//
			Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);	 //AtoB(INT8U Asc)
			CLRWDT();
			mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
			CLRWDT();
			while(1)
			{
				NOP();			// 复位，等待数据检查回复数据，10秒左右 //
			}	
			CLRWDT();
			break;
			
		 //校表参数外所有数据清零//
		case 0x0000FEFF://NN=FF，清除所有通道；//
			 if((*(sptr+13)=='5')&&(*(sptr+14)=='5')&&(*(sptr+15)=='5')&&(*(sptr+16)=='5'))  
			{
				mem_db_clr(0);   //数据区校验和全部清零 //
                while(1)
                {
                    NOP();          // 复位，等待数据检查回复数据，10秒左右 //
                }	
				CLRWDT();
			}
			else if((*(sptr+13)=='3')&&(*(sptr+14)=='3')&&(*(sptr+15)=='3')&&(*(sptr+16)=='3'))  
			 {
				 mem_db_clr(0xA5);   //包括校准参数全部清零 //
				 while(1)
				 {
					 NOP(); 		 // 复位，等待数据检查回复数据，10秒左右 //
				 }	 
				 CLRWDT();
			 }
			else
			{
				return(FALSE);
			}
			break;

          default:
                 Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
		    
                 if(mem_type == MEM_RAM)
                 {
                       mem_write((INT16U)comm_data.addr,&data_buff[0], comm_data.len, mem_type);
                 }
                 else
                 {
                       mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
                 }
				 
	          	break;	
		   
     	}
	 	
// 通讯过后 系统更新
    #if (TARIFF_MAX_NUM>1)
	if(((comm_data.di1_di0.u32 >= 0x04000801)&&(comm_data.di1_di0.u32 <= 0x04000807))
		||((comm_data.di1_di0.u32 >= 0x04010000)&&(comm_data.di1_di0.u32 <= 0x04010008)))
	{
	
		api_update_triffNo_pre_minute();
	}
    #endif

// 有功组合方式字更新处理//
	if((comm_data.di1_di0.u32 == 0x04000601))
	{
      mem_read(&gs_sys_globaL_var.fac_energy[0], ADR_METER_PARAM1_FAC_ENERGY, 2, MEM_E2P1);   
	}

	if((comm_data.di1_di0.u32 == 0x04000602))
	{
      mem_read(&gs_sys_globaL_var.Rac_energy[0], ADR_METER_PARAM1_REC_ENERGY, 2, MEM_E2P1);    
	}
/////////////////设置显示参数后需要更新///////
	if((comm_data.di1_di0.u32 == 0x04000302)||(comm_data.di1_di0.u32 == 0x04000303)||(comm_data.di1_di0.u32 == 0x04040100))
	{
		mem_read(&gs_dis_param.auto_sec, ADR_BLOCK21_DIS_PARAM_E2P, LEN_BLOCK21_DIS_PARAM_E2P, MEM_E2P1);
	}	
    // 数据设置成功后的组帧操作 //
     api_deal_even_class_recode(ID_CLASS_I_PROG,START);   //生成编程事件记录 //
    gs_uart_iec_app_var[g_ch_comm].len= 1;
    Get_ACK_Answer(sptr);	 
    return(TRUE); 
}


//-------------------------------------------------------------------------------//
//  Function:     INT8U IEC_Cmd_Break_Deal(INT8U* sptr,INT8U len)
//  Description:   iec 通讯终止指令处理程序
//  Parameters:   
//  Returns:        无                          
//  Others:         
//-------------------------------------------------------------------------------//
INT8U IEC_Cmd_Break_Deal(INT8U* sptr)
{

       gs_uart_iec_app_var[g_ch_comm].iec_status |= COMM_BREAK_OK_IEC;
	 gs_uart_iec_app_var[g_ch_comm].len= 1;
       Get_ACK_Answer(sptr);	 
       return(TRUE);  
}

/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :初始化当前需量计算RAM区数据，程序复位初始化调用        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void lnk_tx_comm_ready( uint8 ch,uint8 start_index)
{
    uint16 u16_badu;
    //comm_IEC_Tx_Ready();   
    switch(gs_uart_iec_app_var[ch].baud_rate)
    {
        case CBaud_3:
            u16_badu =300;
        break;
        case CBaud_6:
            u16_badu =600;
        break;
        case CBaud_12:
            u16_badu =1200;
        break;
        case CBaud_24:
            u16_badu =2400;
        break;
        case CBaud_48:
            u16_badu =4800;
        break;
        case CBaud_96:
            u16_badu =9600;
        break;

        default:
            u16_badu =300;
        break;
    }

    P_IEC_BUFF = &gs_uart_drv_var[ch].buff[0] ;
   // Tx_UART_frist(&P_IEC_BUFF[0], (gs_uart_iec_app_var[ch].len+start_index),u16_badu,ch); // 数据发送 //     
    Tx_UART_frist(&P_IEC_BUFF[start_index], (gs_uart_iec_app_var[ch].len),u16_badu,ch);        // 数据发送 //     
}


/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :初始化当前需量计算RAM区数据，程序复位初始化调用        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_handl_COMM_FRAME_ERROR( uint8 ch)
{
    if(ch==UART_CH0_IR)           
    {
        Lib_Clr_String(&gs_uart_iec_app_var[ch].len, sizeof(UART_COMM_APP_VAR));   
        Init_UART4_hard(BPS300);  //reset_uart(UART_RESET_ALL); 
        Init_UART_soft(ch); 
		    Lib_Set_String( &LPReadPara.Original_Ptr, 0, 5) ;		
    }
    
    if(ch==CH_RF)
    {
     Lib_Clr_String(&gs_uart_iec_app_var[ch].len, sizeof(UART_COMM_APP_VAR));   
     Init_UART_soft(ch); 
	 SX127x_init();
     Rx_mode(); 
    }
    
    return;
}

//-------------------------------------------------------------------------------//
//  Function:      void init_comm_link(INT8U mode)         
//  Description:   初始化串口链路层变量数据
//  Parameters:   mode: 通讯通道序号  
//  Returns:        无                          
//  Others:         
//-------------------------------------------------------------------------------//
void process_iec62056_21 (INT8U ch)
{
 INT8U buff_size;
 INT8U start_index;
 INT8U meterID_len;

 INT8U tmp_8u_01[16];
 INT8U tmp_8u_02[32];
 int i;
// INT16U  u16_badu;

    
    if(ch > UART_CH_INDEX_MAX)  return ;
    
    if(gs_uart_iec_app_var[ch].tx_delay_flg ==TRUE ) 
    {
        return ;    //发送等待状态时退出 //
    }

    buff_size = Get_UART_rx_buff(&P_IEC_BUFF,ch);
    if(buff_size==0)   return;    // 每个循环获取一次接收数据长度，=0，退出 //

/////////////////////////////////////////////////////////////////////////
    if((gs_uart_iec_app_var[ch].iec_status & COMM_INFO_OK_IEC) !=0)
    {
        CLRWDT();
        CLRWDT();
    }
////////////////////////////////////////////////////////////////////////

    // 前导字节处理 //
    for(start_index=0;start_index<5;start_index++)
    {
        if((P_IEC_BUFF[start_index]=='/')||(P_IEC_BUFF[start_index]==ACKCOM)||(P_IEC_BUFF[start_index]==SOH))  break;
    }

    if(start_index>3)
    {
        //接收的数据帧错误，补充串口复位操作      //
        api_handl_COMM_FRAME_ERROR(ch);
        return;
    }

    gs_uart_iec_app_var[ch].tx_delay_flg =FALSE;   // 有数据接收时清除发送延时标识 //
    gs_uart_iec_app_var[ch].delay_10ms = 0;   // 有数据处理时清除延时计数器 //
    gs_uart_iec_app_var[ch].err_flg = 0;
    g_ch_comm = ch;
    
    if(((buff_size >2) &&((P_IEC_BUFF[buff_size-2]) == CR ||(P_IEC_BUFF[buff_size-2]) == ETX )) )
    {
        buff_size =buff_size-start_index;
        // 数据帧处理 //
        switch(P_IEC_BUFF[start_index])
        {
            case IEC_COMM_REQUEST:	
                if((gs_uart_iec_app_var[ch].iec_status & COMM_INFO_OK_IEC) !=0)
                {
                    api_handl_COMM_FRAME_ERROR(ch);
                    return;
                }
                mem_read (&tmp_8u_01[0],ADR_METER_PARAM1_METER_ID, 16,MEM_E2P1);
                meterID_len = get_meter_id_asclen(&tmp_8u_01[0], 16);	//获取表号有效asc长度
                Lib_BCD_AscA(&tmp_8u_02[0], &tmp_8u_01[0], 16);
                if(IEC_Comm_Request_Cmd_Judge(&P_IEC_BUFF[start_index], buff_size, &tmp_8u_02[0],meterID_len)==TRUE)
                {
                    if(ch==UART_CH0_IR)   // TTL//
                   {
                         Get_IEC_Comm_Request_Cmd_Answer(&P_IEC_BUFF[start_index], Meter_INFO_LENTH,(uint8*)&meter_info_IR[0]);//start_index
                    }
                     if(ch==CH_RF)  // RF//
                   {
                         Get_IEC_Comm_Request_Cmd_Answer(&P_IEC_BUFF[start_index], Meter_INFO_LENTH,(uint8*)&meter_info[0]);
                    }           
                    gs_uart_iec_app_var[ch].len = Meter_INFO_LENTH+3;
                    gs_uart_iec_app_var[ch].iec_status |= COMM_INFO_OK_IEC;    // 握手命令成功标识 //
                }
                else
                {
                    //考虑到开放式通讯，失败时不返回数据 //
                    api_handl_COMM_FRAME_ERROR(ch);
                    return;
                }
            break;

            case IEC_COMM_ACKNOWLEDGEMENT:
                if((gs_uart_iec_app_var[ch].iec_status & COMM_INFO_OK_IEC) ==0)
                {
                    api_handl_COMM_FRAME_ERROR(ch);
                    return;
                }
                if(IEC_Comm_Acknowledgement_Cmd_Judge(&P_IEC_BUFF[start_index],buff_size)==TRUE)
                {
                    gs_uart_iec_app_var[ch].baud_rate = P_IEC_BUFF[start_index+2];      //                                     //波特率赋值  //
                    Get_IEC_Comm_Operand_Answer(&P_IEC_BUFF[start_index], MIMA_INFO_LENTH,(uint8*)&mima_info[0]); //   // 获取数据组帧 //
                    gs_uart_iec_app_var[ch].len = (MIMA_INFO_LENTH+6);
                    gs_uart_iec_app_var[ch].iec_status |= COMM_REQUIST_OK_IEC;  // 给出请求命令成功标识 //
                }
                else
                {
                    gs_uart_iec_app_var[ch].iec_status &= ~COMM_REQUIST_OK_IEC;  // clr 请求命令成功标识 //
                    Get_Error_Answer(&P_IEC_BUFF[start_index]);//
                    gs_uart_iec_app_var[ch].len = 1;
                }

            break;

            case IEC_COMM_COMMAND:
                if((gs_uart_iec_app_var[ch].iec_status & COMM_REQUIST_OK_IEC) ==0)
                {
                    api_handl_COMM_FRAME_ERROR(ch);
                    return;
                } 
	//			i = IEC_Comm_Command_Cmd_Judge(&P_IEC_BUFF[start_index],buff_size);             
                if(IEC_Comm_Command_Cmd_Judge(&P_IEC_BUFF[start_index],buff_size) !=TRUE)//
                {
                     // 命令出错
			// 数据解析失败 //
                    Get_Error_Answer(&P_IEC_BUFF[start_index]);
                    gs_uart_iec_app_var[ch].len = 1;   		           
                }
            break;

            default:
                gs_uart_iec_app_var[ch].err_flg |= FRAME_ERROR;   //帧错误，需要复位  //	
            break;
        }

    }
    else
    {
        gs_uart_iec_app_var[ch].err_flg |= FRAME_ERROR;   //帧错误，需要复位  //	
    }

    if((gs_uart_iec_app_var[ch].err_flg & FRAME_ERROR) )
    {
        //接收的数据帧错误，补充串口复位操作      //
        api_handl_COMM_FRAME_ERROR(ch);
        return;
    }
    else
    {
        gs_uart_iec_app_var[ch].tx_delay_flg =TRUE;
        gs_uart_iec_app_var[ch].tx_ready_10ms = 0;   // 有数据处理时清除延时计数器 //
        if(gs_uart_iec_app_var[ch].iec_status ==(COMM_INFO_OK_IEC+COMM_REQUIST_OK_IEC))
        {
            gs_uart_iec_app_var[ch].tx_ready_10ms  += 4;   // 切换波特率时延时增加10ms //
        }
        gs_uart_iec_app_var[ch].start_index = start_index;
    }
}


/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :初始化当前需量计算RAM区数据，程序复位初始化调用        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_handl_COMM_pre_10ms(uint8 ch)
{

        process_iec62056_21(ch);    

    if(gs_uart_iec_app_var[ch].delay_10ms >COMM_DELAY_3500MS)
    {   
        //超时退出握手，重新进入空闲等待状态 //
        api_handl_COMM_FRAME_ERROR(ch);    
    }

    if((gs_uart_iec_app_var[ch].tx_ready_10ms ==0)&&(gs_uart_iec_app_var[ch].tx_delay_flg ==TRUE))
    {   
        //数据准备发送   //
        gs_uart_iec_app_var[ch].tx_delay_flg =FALSE;
        lnk_tx_comm_ready(ch,gs_uart_iec_app_var[ch].start_index);     
    }



}
///////////////////////////////////////////////////////////////////
//  RF 通讯部分
///////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------//
//  Function:      void uart_comm_deal(void)    串口通讯处理函数        
//  Description:   实现串口功能的管理和操作
//  Parameters:   无  
//  Returns:        无                          
//  Others:         
//-------------------------------------------------------------------------------//
void RF_comm_deal(void)
{  
    // RF 通道是否接收到数据 //
    if(LSD4RF_READ_DIO0() ==0) return;
    Lib_Delay_Nop(20);
    if(LSD4RF_READ_DIO0() ==0) return;   
    
    // 接收到数据后的处理 //
	LSD_RF_RxPacket(&gs_uart_drv_var[CH_RF].buff[0]);
    CLRWDT();   // 清看门狗//
    gs_uart_iec_app_var[CH_RF].delay_10ms = 0;   // 有数据处理时清除延时计数器 //
    gs_uart_drv_var[CH_RF].rx_over_ticks =5;
	////////////////////////
	gs_uart_drv_var[CH_RF].buff_index = G_LoRaConfig.PayloadLength;
     ///////////////////////////////
}
/***************************************************************
*    END
****************************************************************/


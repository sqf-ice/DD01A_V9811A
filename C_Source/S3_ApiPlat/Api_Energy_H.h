/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Energy_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: 电能计量应用层相关函数程序声明文件
**note			: MCU- G80F92XD ，MERTER FOR DL06A
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-20   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/



#ifndef _Api_Energy_H
#define _Api_Energy_H

//******************************************************************
//              1     立即数预定义                                                                 
//******************************************************************


//******************************************************************
//               2     数据结构声明                               
//******************************************************************
//累计电能数据单元结构  //
typedef union
{
	uint8 buf[6];
	struct
	{
	  uint32 integ_hex32;   // 电能数据整数部分，32位16进制数据，量纲1kWh //
	  uint16 decim_hex16;   // 电能数据小数部分，16位16进制数据，量纲(1/电表常数)kWh //
	}val;
}US_EC_DATA_UNIT;

//当前电能数据单元结构  //
typedef struct 
{
	US_EC_DATA_UNIT   us_val[EC_CH_NUM][1+TARIFF_MAX_NUM]; //  0总 1正  2反
    INT16U  u16_csck;    //16位累加和校验值//
} ENERGY_USER_DATA;

//当前电能脉冲增量数据结构 //
typedef struct 
{
    uint8   pluse[EC_CH_NUM];
} ENERGY_VAR;
//日冻结四天的电量
typedef struct
{
	US_EC_DATA_UNIT   us_val[5];
//	INT16U  u16_csck;    //16位累加和校验值//
}E2P_ENERGY_DATA_ARRAY;


//当前电能数据E2P存储数据结构  //
typedef struct 
{
    US_EC_DATA_UNIT   us_val[EC_CH_NUM][1+TARIFF_MAX_NUM];
    INT16U  u16_csck;    //16位累加和校验值//
} E2P_CURRENT_ENERGY_DATA_MAP;

//******************************************************************
//              3    变量声明                                      
//******************************************************************
extern ENERGY_USER_DATA    gs_energy_user_data;
extern ENERGY_USER_DATA    gs_energy_user_data_back;
extern ENERGY_VAR               gs_energy_var;

//******************************************************************
//             4     函数声明                                      
//******************************************************************
extern void api_init_current_energy_data_ram(void);
extern void api_check_current_energy_data(void);
extern uint8 lnk_add_fac_energy_pluse(void);
extern uint8 lnk_add_rac_energy_pluse(void);
extern void api_accum_current_energy_data_pre_second(void);
extern void api_clr_current_energy_data(void);

//******************************************************************
//             5     其他内容，例如伪指令等             
//******************************************************************
//电能数据单元长度 //
#define LEN_EC_UNIT			sizeof(US_EC_DATA_UNIT)    
#define LEN_EC_CURRENT		( EC_CH_NUM*(1+TARIFF_MAX_NUM)*LEN_EC_UNIT)
#define LEN_EC_E2P         sizeof(ENERGY_USER_DATA)

// 单通道数据单元长度
#define LEN_EC_NUM	     ((1+TARIFF_MAX_NUM)*LEN_EC_UNIT)


/***************************************************************
*    END
****************************************************************/
#endif


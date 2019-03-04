/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Demand_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: 需量处理应用层相关函数程序声明文件
**note			: MERTER FOR DL06A
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-20   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/



#ifndef _Api_Demand_H
#define _Api_Demand_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/
//每个通道需量数据最大缓存区个数//
#define MAX_MD_BUFF_NUM 30   //需量最大周期不超过30分钟  //



/***************************************************************
*    2     数据结构声明     
****************************************************************/
//月最大需量数据单元结构  //
typedef union
{
	uint8 buf[9];
	struct
	{
	  uint32 val_hex32;   // 当前月最大需量数据，32位16进制数据，量纲0.0001kW//
	  uint8   date_rtc[5];   // 当前月最大需量发生时间，年月日时分//
	}val;
}MMD_DATA_UNIT;

typedef union
{
	uint8 buf[9];
	struct
	{
		uint32 val_hex32;   // 当前月最大需量数据，32位16进制数据，量纲0.0001kW//
		uint8   date_rtc[5];   // 当前月最大需量发生时间，年月日时分//
	}val;
}NEW_MMD_DATA_UNIT;


typedef union
{
	uint8 buf[10];
	struct
	{
		uint32 val_hex32;   // 当前月最大需量数据，32位16进制数据，量纲0.0001kW//
		uint8 val_count;//记录最大需量次数
		uint8   date_rtc[5];   // 当前月最大需量发生时间，年月日时分//
	}val;
}SUM_MMD_DATA_UNIT;
//日冻结四天的最大需量

typedef struct 
{
    SUM_MMD_DATA_UNIT   us_val[5];
//    INT16U  u16_csck;    //16位累加和校验值//

}E2P_MDmax_SUM_DATA_ARRAY;
typedef struct 
{
    NEW_MMD_DATA_UNIT   us_val[5];
//    INT16U  u16_csck;    //16位累加和校验值//

}E2P_MDmax_DATA_ARRAY;

//计算每个通道平均分钟时时需量数据的变量数据  //
typedef struct 
{
    uint8   cnt_sec;        //累加和使用的秒计数器，小于等于60秒，量纲1秒  //
    uint32  sum_min;   //当前分钟内的秒功率数据累加和，32位16进制数据，量纲0.0001kW   //
    uint32  val_min;   //每分钟平均功率数据，32位16进制数据，量纲0.0001kW  //
} MD_CH_MIN_VAR_UNIT;


//计算每个通道平均分钟时时需量数据的变量数据  //
typedef struct 
{
    uint32  val_slid;   //当前时时需量(每个滑差更新一次)，32位16进制数据，量纲0.0001kW  //
    uint32  date_buff[MAX_MD_BUFF_NUM];   //分钟平均功率数据数组，32位16进制数据，量纲0.0001kW  //
    uint8   cnt_min;        //累加和使用的分钟计数器，小于周期参数，量纲1分钟  //
    uint8   cal_flg;        //  需量计算标识，=1标识开始计算需量数据    //
} MD_CH_SLID_VAR_UNIT;


//需量管理参数  //
typedef struct 
{
    	uint8    u8_period;		//需量周期 //	
	uint8    u8_slid;			//需量滑差  //	
} MD_PARAM;



//需量变量数据  //
typedef struct 
{
    MD_CH_MIN_VAR_UNIT   min_var[MD_CH_NUM];   
    MD_CH_SLID_VAR_UNIT  slid_var[MD_CH_NUM];
    uint32   power_val[MD_CH_NUM];
    uint8  min_remaind[2];     // 下一次时时需量值生成剩余分钟数和秒数  //
} MD_VAR;


//需量管理参数  //
typedef struct 
{
    MMD_DATA_UNIT   us_val[MD_CH_NUM][1+TARIFF_MAX_NUM];
    INT16U  u16_csck;    //16位累加和校验值//
} MD_USER_DATA;


//当前最大需量数据E2P存储数据结构  //
typedef struct 
{
    MMD_DATA_UNIT   us_val[MD_CH_NUM][1+TARIFF_MAX_NUM];
    INT16U  u16_csck;    //16位累加和校验值//
} E2P_CURRENT_MD_DATA_MAP;

/***************************************************************
*    3    变量声明       
****************************************************************/
extern MD_VAR gs_md_var;           // 最大需量计算变量RAM区数据 //
extern MD_PARAM gs_md_param;  // 最大需量计算参数RAM区数据 //

/***************************************************************
*    4     函数声明        
****************************************************************/
extern uint8 api_check_Demand_para(MD_PARAM *s_para);
extern void api_init_md_data_ram(void);
extern void api_cmp_save_md_data(uint8 ch, uint32 val_hex32);
extern void lnk_updata_md_var_power_per_second(void);
extern void api_updata_md_var_min_per_second(void);
extern void api_updata_md_var_slid_per_minute(void);
extern void api_clr_current_MD_data(void);



/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
//月最大需量数据单元长度 //
#define LEN_MMD_UNIT			sizeof(MMD_DATA_UNIT)
#define LEN_NEW_MMD_UNIT		sizeof(NEW_MMD_DATA_UNIT)
#define LEN_SUM_MMD_UNIT		sizeof(SUM_MMD_DATA_UNIT)    
#define LEN_MMD_CURRENT		(sizeof( E2P_CURRENT_MD_DATA_MAP)-2)


//单通道最大需量数据长度
#define LEN_MMD_NUM	     ((1+TARIFF_MAX_NUM)*LEN_MMD_UNIT)
/***************************************************************
*    END
****************************************************************/

#endif

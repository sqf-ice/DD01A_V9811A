/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: Api_Measure_H.H
**Author: maji
**date: 
**description:  测量程序应用层相关函数程序
**note: memer type  SD03A
**--------------------Version History -------------------------------------
** NO.	Date        		Ver    		By     			Description 
**==============================================================
** 1    2015-10-12  		0.1  		sosomj   		1. frist version                             
**
**==============================================================
*/


#ifndef _Api_Bill_Record_H
#define _Api_Bill_Record_H

#if (BILL_MAX_NUM>0)
/***************************************************************
*    1     立即数预定义   
****************************************************************/
//bill info index define 
#define  BILL_ID0_MOTTH_SETTL         0            //月结算数据项ID //

#define  MAX_Fill_SETTLED_MONTHS      6     //最大上电补结算的月数 //

/***************************************************************
*    2     数据结构声明     
****************************************************************/

typedef struct 
{
    INT16U     head_adr;
    INT16U     ptr_adr;
    INT16U     data_size;
    INT8U       ptr_num;
    INT8U       mem_type;
} BILL_INFO;
 

typedef struct 
{
    INT8U                                   u8_RTC_record[6];           //年月日时分秒  //
    US_EC_DATA_UNIT                 ec_val[EC_CH_NUM][1+TARIFF_MAX_NUM];
    MMD_DATA_UNIT                   mmd_val[MD_CH_NUM][1+TARIFF_MAX_NUM];
} BILL_DATA_UNIT;

typedef struct 
{
    INT8U   ptrNo;                      //当前结算数据指针  //
    INT8U   dateTime[6];   //年月日时分秒  //
} BILL_EXRAM_VAR;

typedef struct 
{
    INT8U   u8_ddhh[2];             // [0]=day,[1]=hour //
} BILL_MONTH_SETTLED_PARAM;


//所有费率相关数据E2P存储数据结构  //
typedef struct 
{
    BILL_DATA_UNIT  bill_data;
    INT16U  bill_data_csck;    //16位累加和校验值//
} E2P_BILL_DATA_MAP_ONE;


typedef struct 
{
    BILL_EXRAM_VAR  bill_var;
    INT16U  bill_var_csck;    //16位累加和校验值//
} E2P_BILL_VAR_MAP;

/***************************************************************
*    3    变量声明       
****************************************************************/



/***************************************************************
*    4     函数声明        
****************************************************************/
extern INT16U api_get_bill_record_addr( INT16U addr );
extern void api_handl_settle_per_minute(void);
extern void api_handl_settle_when_powerUp(void);
extern void api_clr_bill_data(void);
extern void api_freeze_energy_Ram_pre_day(void);
extern void api_freeze_energy_pre_day(void);

extern uint8 d_flag;
extern US_EC_DATA_UNIT 	energy_data_array[5];
extern NEW_MMD_DATA_UNIT st_mmd_unit_array[5];
extern SUM_MMD_DATA_UNIT sum_st_mmd_unit_array[5];
extern uint8 csck,Excsck;

extern uint8 Judage_Year(uint8 year);
extern uint8 Judage_Month(uint8 month,uint8 year);

extern void api_write_bill_data(INT8U id) ;

/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
// 外部存储存储数据长度定义 //
#define LEN_BILL_DATA_UNIT	    (sizeof(BILL_DATA_UNIT) )
#define LEN_BILL_EXRAM_VAR	    (sizeof(BILL_EXRAM_VAR) )

#define BILL_BUFF_MAX_LEN       LEN_BILL_DATA_UNIT   // 最长的账单数据长度 //

// 外部存储存储数据偏移量和长度定义 //
     // E2P_BILL_VAR_MAP  //
#define OFFSET_bill_var                 ST_MB_OFFSET(E2P_BILL_VAR_MAP,bill_var)
#define OFFSET_bill_var_csck         ST_MB_OFFSET(E2P_BILL_VAR_MAP,bill_var_csck)
#define LEN_bill_var	                   (OFFSET_bill_var_csck+2)

#define OFFSET_bill_var_ptrNo           OFFSET_bill_var+ ST_MB_OFFSET(BILL_EXRAM_VAR,ptrNo)   
#define LEN_bill_var_ptrNo                  ST_MB_LEN(BILL_EXRAM_VAR,ptrNo)     
#define OFFSET_bill_var_dateTime       OFFSET_bill_var+ ST_MB_OFFSET(BILL_EXRAM_VAR,dateTime)   
#define LEN_bill_var_dateTime             6  //最近1次结算时间点数据 YYMMDD hhmmss  //  

    // E2P_BILL_DATA_MAP  //
#define OFFSET_bill1_data              ST_MB_OFFSET(E2P_BILL_DATA_MAP_ONE,bill_data)
#define OFFSET_bill1_data_csck         ST_MB_OFFSET(E2P_BILL_DATA_MAP_ONE,bill_data_csck)
#define LEN_bill1_data	                (OFFSET_bill1_data_csck+2)

#define OFFSET_bill_data_dateTime       ST_MB_OFFSET(BILL_DATA_UNIT,u8_RTC_record)       
#define OFFSET_bill_data_ec                 ST_MB_OFFSET(BILL_DATA_UNIT,ec_val) 
#define OFFSET_bill_data_mmd             ST_MB_OFFSET(BILL_DATA_UNIT,mmd_val)         



#endif

/***************************************************************
*    END
****************************************************************/

#endif

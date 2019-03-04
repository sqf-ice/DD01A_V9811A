/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Triff_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: 费率处理应用层相关函数程序声明文件
**note			: MERTER FOR DL06A
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-20   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#ifndef _Api_Triff_H
#define _Api_Triff_H


#if (TARIFF_MAX_NUM>1)
/***************************************************************
*    1     立即数预定义   
****************************************************************/
//Fee
#define MAX_DAY_TRIFF_CHG   8              //日时段表每日最大切换数  //
#define MAX_INDEX_DAY_TAB     4             //日时段表最大索引号  //
#define MAX_INDEX_WEEKDAY_TAB     4     //周表最大索引号  //
#define MAX_YEAR_TAB_CHG   8               //季节表表最大切换数  //
#define MAX_NUM_HOLIDAY_TAB     0      //节假日最大个数  //

// 默认费率号 //
#define DEF_TRIFF_no   1        // 默认费率为1 //
/***************************************************************
*    2     数据结构声明     
****************************************************************/
typedef struct
{
    uint8 hour_BCD8;        //起始点-小时  //
    uint8 minute_BCD8;     //起始点-分钟  //
    uint8 triff_No;       //费率序号1~4代表费率1鍉4 //
}DAY_TABLE_UNIT;

typedef struct
{
    uint8 month_BCD8;        //起始点-月  //
    uint8 day_BCD8;     //起始点-日  //
    uint8 weekTable_No;       //日时段表号，1起始 //
}SEASON_TABLE_UNIT;

typedef struct
{
    uint8 Sun_Tab;     //周日时段表号  //
    uint8 Mon_Tab;     //周1时段表号  //
    uint8 Tues_Tab;     //周2时段表号  //
    uint8 Wed_Tab;     //周3时段表号  //
    uint8 Thurs_Tab;     //周4时段表号  //
    uint8 Fri_Tab;     //周5时段表号  //
    uint8 Sat_Tab;     //周6时段表号  //
}WEEK_TABLE_UNIT;


typedef struct
{
    uint8 month_BCD8;        //起始点-月  //
    uint8 day_BCD8;     //起始点-日  //
    uint8 dayTable_No;       //日时段表号，1起始 //
}HOLIDAY_TABLE_UNIT;


typedef struct
{
    DAY_TABLE_UNIT  tab[MAX_DAY_TRIFF_CHG];
}DAY_TABLE;

typedef struct
{
    WEEK_TABLE_UNIT  tab[MAX_INDEX_WEEKDAY_TAB];
}WEEK_TABLE;

typedef struct
{
    SEASON_TABLE_UNIT  tab[MAX_YEAR_TAB_CHG];
}SEASON_TABLE;

#if (MAX_NUM_HOLIDAY_TAB >0)
typedef struct
{
    HOLIDAY_TABLE_UNIT tab[MAX_NUM_HOLIDAY_TAB];
}HOLIDAY_TABLE;
#endif     // #if (MAX_NUM_HOLIDAY_TAB >0)  //

typedef struct
{
    uint8 triff_No;             //当前费率号1~4代表费率1鍉4  //
    uint8 dayTab_No;        //当前日时段表号1~MAX_INDEX_DAY_TAB  //
    uint8 weekTab_No;     //当前周表号1~MAX_INDEX_WEEKDAY_TAB //
    uint8 seasonTab_No;     //当前季节表号MAX_YEAR_TAB_CHG //
}CURRENT_TRIFF_VAR;


//所有费率相关数据E2P存储数据结构  //
#if (MAX_NUM_HOLIDAY_TAB >0)
typedef struct 
{
    HOLIDAY_TABLE   holiday_tab;
    INT16U  holiday_tab_csck;    //16位累加和校验值//
} E2P_TRIFF_HOLIDAY_TABLE_MAP;
#endif     // #if (MAX_NUM_HOLIDAY_TAB >0)  //

typedef struct 
{
    SEASON_TABLE  season_tab;
    INT16U  season_tab_csck;    //16位累加和校验值//
} E2P_TRIFF_SEASON_TABLE_MAP;

typedef struct 
{
    WEEK_TABLE  week_tab;
    INT16U  week_tab_csck;    //16位累加和校验值//
} E2P_TRIFF_WEEK_TABLE_MAP;

typedef struct 
{
    DAY_TABLE day_tab[MAX_INDEX_DAY_TAB];
    INT16U  day_tab_csck;    //16位累加和校验值//
} E2P_TRIFF_DAY_TABLE_MAP;

typedef struct 
{
#if (MAX_NUM_HOLIDAY_TAB >0)
    E2P_TRIFF_HOLIDAY_TABLE_MAP   holiday_tab;
#endif     // #if (MAX_NUM_HOLIDAY_TAB >0)  //
    E2P_TRIFF_SEASON_TABLE_MAP  season_tab;
    E2P_TRIFF_WEEK_TABLE_MAP  week_tab;
    E2P_TRIFF_DAY_TABLE_MAP day_tab;
} E2P_TRIFF_PARAM_MAP;



/***************************************************************
*    3    变量声明       
****************************************************************/
extern CURRENT_TRIFF_VAR gs_current_triff_var;

#if (MAX_NUM_HOLIDAY_TAB >0)
extern const  E2P_TRIFF_HOLIDAY_TABLE_MAP  code   default_triff_holiday_tab;
#endif     // #if (MAX_NUM_HOLIDAY_TAB >0)  //
extern const  E2P_TRIFF_SEASON_TABLE_MAP  code   default_triff_season_tab;
extern const  E2P_TRIFF_WEEK_TABLE_MAP  code   default_triff_week_tab;
extern const  E2P_TRIFF_DAY_TABLE_MAP  code   default_triff_day_tab;
/***************************************************************
*    4     函数声明        
****************************************************************/
extern void api_update_triffNo_pre_minute(void);
extern void api_init_triff_var_ram(void);

/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
//当前费率号定义 //
#define EN_TRIFF_no     gs_current_triff_var.triff_No

//程序处理中的数据长度定义  //
#define LEN_HOLIDAY_TABLE	(sizeof(HOLIDAY_TABLE) )

#define LEN_SEASON_TABLE	(sizeof(SEASON_TABLE) )

#define LEN_WEEK_TABLE_ONE	(sizeof(WEEK_TABLE_UNIT) )
#define LEN_WEEK_TABLE          	(sizeof(WEEK_TABLE) ) 

#define LEN_DAY_TABLE_ONE		(sizeof(DAY_TABLE) )  
#define LEN_DAY_TABLE			(MAX_INDEX_DAY_TAB*LEN_DAY_TABLE_ONE) 

// 外部存储存储数据偏移量和长度定义 //
#if (MAX_NUM_HOLIDAY_TAB >0)
#define OFFSET_HoliTab              ST_MB_OFFSET(E2P_TRIFF_PARAM_MAP,holiday_tab)
#define LEN_HoliTab	                (sizeof(E2P_TRIFF_HOLIDAY_TABLE_MAP))
#endif   //#if (MAX_NUM_HOLIDAY_TAB >0)     //

#define OFFSET_SeasonTab              ST_MB_OFFSET(E2P_TRIFF_PARAM_MAP,season_tab)
#define LEN_SeasonTab	                (sizeof(E2P_TRIFF_SEASON_TABLE_MAP))

#define OFFSET_WeekTab              ST_MB_OFFSET(E2P_TRIFF_PARAM_MAP,week_tab)
#define LEN_WeekTab	                (sizeof(E2P_TRIFF_WEEK_TABLE_MAP))

#define OFFSET_DayTab              ST_MB_OFFSET(E2P_TRIFF_PARAM_MAP,day_tab)
#define LEN_DayTab	                (sizeof(E2P_TRIFF_DAY_TABLE_MAP))


#endif  // #if (TARIFF_MAX_NUM>1) //

/***************************************************************
*    END
****************************************************************/

#endif

/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_EXmem_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: 外部存储器C代码的声明文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#ifndef _Proc_EXmem_H
#define _Proc_EXmem_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/
#define DM_FLASH_EN 0		//	 1:才用芯片内部FLASH 	0:不使用
#define DM_E2P_EN   1		//	 1:才用芯片外部E2P 	0:不使用

//存储类型定义  //
//mem_table.type define
#define MEM_RAM				0X01			
#define MEM_EXRAM			(MEM_RAM+1)

#if (DM_E2P_EN==1)
#define MEM_E2P1			MEM_EXRAM   //MEM_DMFLASH 
#endif

#if (DM_FLASH_EN==1)	
#define MEM_DMFLASH         MEM_EXRAM//0X04  
#endif

#define E2P_PAGE   EE_PageMask


#define EE_AddrStart             0x0000 // 0x0000                                  //EEPROM起始地址 //
#define EE_DOUBLE_DATA_ADDR_START  EE_AddrStart      //双备份数据区的起始地址  //
#define E2PBAK_SIZE   (1024)   //  E2P备份数据为后半部分的开始//
#define EE_SINGLE_DATA_ADDR_START  (EE_DOUBLE_DATA_ADDR_START+2*(E2PBAK_SIZE))      //单份数据区的起始地址  //


/***************************************************************
*    2     数据结构声明     
****************************************************************/
//E2P数据检查表类型定义  //
typedef struct 
{
    uint8  u8_No;
    uint8  mem_type;
    uint16 u16_adr;
    uint16 u16_len; 
} E2P_CHECK_TAB;


//双备份带校验E2P存储数据格式声明  //
typedef struct 
{
     E2P_CURRENT_ENERGY_DATA_MAP    L0_energy_data;   // 60  字节//
     E2P_CURRENT_MD_DATA_MAP     L0_MD_data;
     E2P_METER_PARAM1_MAP    meter_param1;
     DIS_PARAM_DATA   dis_param;
     E2P_METER_VAR_MAP       meter_var;
     ST_EMU_ADJ_PARAM    emu_adj_param;       // 计量芯片的误差校准数据 //
    #if (BILL_MAX_NUM>0)
     E2P_BILL_VAR_MAP    Bill_var;                    //数据结算时的相关变量 //
     EVEN_CLASSII_VAR    even_var;         // 7组2类事件参数//
     #if (TARIFF_MAX_NUM>1)
     E2P_TRIFF_PARAM_MAP  Triff_param;  // 
     #endif
	 E2P_ENERGY_DATA_ARRAY Array_energy_data;
	 E2P_MDmax_DATA_ARRAY Array_mdmax_data;
	 E2P_MDmax_SUM_DATA_ARRAY Array_mdmax_data_sum;
    #endif
} DBE2P_MAP;


//单份带校验E2P存储数据格式声明  //
typedef struct 
{
#if (BILL_MAX_NUM>0)
    E2P_BILL_DATA_MAP_ONE  Bill_data[BILL_MAX_NUM];   // 12 个月结算数据 //
#endif
    E2P_EVEN_I_DATA_MAP     even_I;  // 2 类事件记录 //
    E2P_EVEN_II_DATA_MAP    even_II;  // 2 类事件记录 //
    INT8U       load_start_e2p;
} SGE2P_MAP;


/***************************************************************
*    3    变量声明       
****************************************************************/
extern const  E2P_CHECK_TAB  code mem_table[] ;


/***************************************************************
*    4     函数声明        
****************************************************************/
extern INT16U get_mem_ck_data( INT16U src, INT16U  lenth, INT8U memType );
extern INT8U get_mem_table_Index( INT16U adr, INT8U memType ) ;
extern void mem_to_mem( INT16U dst, INT16U src, 	INT16U lenth, INT8U mode );
extern void mem_fill( INT16U dst, INT16U lenth, INT8U xx, INT8U memType );
extern void mem_read( INT8U* dst,  INT16U src, INT16U lenth, INT8U memType );
extern void mem_write( INT16U dst,  INT8U *src, INT16U lenth, INT8U memType );
extern void mem_db_write( INT16U dst,  INT8U *src, INT16U len, INT8U memType );
extern void mem_db_check( void );
extern void mem_db_clr( uint8 type );

/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
// E2P MAP //


//BLOCK00  当前电量区 //
#define  No_BLOCK00_EC_L0_E2P    0    // 数据项目索引号 //
#define ADR_BLOCK00_EC_L0_E2P   EE_DOUBLE_DATA_ADDR_START +ST_MB_OFFSET(DBE2P_MAP,L0_energy_data)   //0. 当前有功电量数据区首地址  //
#define LEN_BLOCK00_EC_L0_E2P   sizeof(E2P_CURRENT_ENERGY_DATA_MAP)

//BLOCK01  当前月最大需量区 //
#define  No_BLOCK01_MMD_L0_E2P    1    // 数据项目索引号 //
#define ADR_BLOCK01_MMD_L0_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,L0_MD_data)   //  1. 当前月最大需量数据区首地址  //
#define LEN_BLOCK01_MMD_L0_E2P   sizeof(E2P_CURRENT_MD_DATA_MAP)

//记录四天电量区    	//cxy 2018-12-13新添
#define ADR_BLOCK_4Energy_L0_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,Array_energy_data)   //  电量数据区首地址  //
#define LEN_BLOCK_4Energy_E2P   sizeof(E2P_ENERGY_DATA_ARRAY)
//记录四天最大需量区    	//cxy 2018-12-13新添
#define ADR_BLOCK_4MMD_L0_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,Array_mdmax_data)   //  电量数据区首地址  //
#define LEN_BLOCK_4MMD_E2P   sizeof(E2P_MDmax_DATA_ARRAY)
//记录四天最大累计需量    	//cxy 2018-12-17 Array_mdmax_data_sum
#define ADR_BLOCK_4MMD_sum_L0_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,Array_mdmax_data_sum)   //  电量数据区首地址  //
#define LEN_BLOCK_4MMD_SUM_E2P   sizeof(E2P_MDmax_SUM_DATA_ARRAY)

//BLOCK20  电表参数区1 //
#define  No_BLOCK20_METER_PARAM1_E2P   20    // 数据项目索引号 //
#define ADR_BLOCK20_METER_PARAM1_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,meter_param1)   //  1. 当前月最大需量数据区首地址  //
#define LEN_BLOCK20_METER_PARAM1_E2P   sizeof(E2P_METER_PARAM1_MAP)

//BLOCK21  电表参数区-显示参数区 //
#define  No_BLOCK21_DIS_PARAM_E2P   21    // 数据项目索引号 //
#define ADR_BLOCK21_DIS_PARAM_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,dis_param)   //  1. 当前月最大需量数据区首地址  //
#define LEN_BLOCK21_DIS_PARAM_E2P   sizeof(DIS_PARAM_DATA)

//BLOCK25  电表变量数据区 //
#define  No_BLOCK25_METER_VAR_E2P   25    // 数据项目索引号 //
#define ADR_BLOCK25_METER_VAR_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,meter_var)   //  1. 当前月最大需量数据区首地址  //
#define LEN_BLOCK25_METER_VAR_E2P   sizeof(E2P_METER_VAR_MAP)


//BLOCK100  月结算变量数据区  //
#define  No_BLOCK26_BILL_VAR_E2P    26    // 数据项目索引号 //
#define ADR_BLOCK26_BILL_VAR_E2P           EE_DOUBLE_DATA_ADDR_START +ST_MB_OFFSET(DBE2P_MAP,Bill_var)   //0. 当前有功电量数据区首地址  //
#define LEN_BLOCK26_BILL_VAR_E2P           sizeof(E2P_BILL_VAR_MAP)

// 事件记录参数区// 单份带校验//
#define  No_BLOCK27_EVENII_data   27    // 数据项目索引号 //
#define  ADR_BLOCK27_EVENII_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,even_var)   //    //
//BLOCK70  EMU 校表参数区//
#define  No_BLOCK70_EMU_PARAM_E2P   70    // 数据项目索引号 // 
#define ADR_BLOCK70_EMU_PARAM_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,emu_adj_param)   //  1. 当前月最大需量数据区首地址  //
#define LEN_BLOCK70_EMU_PARAM_E2P   sizeof(ST_EMU_ADJ_PARAM)

//BLOCK81~84 费率表相关数据区  //
#if (TARIFF_MAX_NUM>1)
#define  No_BLOCK80_TRIFF_PARAM_E2P   80    // 数据项目索引号 //
#define ADR_BLOCK80_TRIFF_PARAM_E2P   EE_DOUBLE_DATA_ADDR_START+ ST_MB_OFFSET(DBE2P_MAP,Triff_param)   //  3. 当前月最大需量数据区首地址  //
#define LEN_BLOCK80_TRIFF_PARAM_E2P   sizeof(E2P_TRIFF_PARAM_MAP)
#if(MAX_NUM_HOLIDAY_TAB>0)  
#define  No_BLOCK81_HILIDAY_TAB_E2P   81    // 数据项目索引号 //
#define ADR_BLOCK81_HILIDAY_TAB_E2P   (ADR_BLOCK80_TRIFF_PARAM_E2P+OFFSET_HoliTab)
#define LEN_BLOCK81_HILIDAY_TAB_E2P    LEN_HoliTab
#endif

#define  No_BLOCK82_SEASON_TAB_E2P     82    // 数据项目索引号 //
#define ADR_BLOCK82_SEASON_TAB_E2P   (ADR_BLOCK80_TRIFF_PARAM_E2P+OFFSET_SeasonTab)
#define LEN_BLOCK82_SEASON_TAB_E2P     LEN_SeasonTab

#define  No_BLOCK83_WEEK_TAB_E2P    83    // 数据项目索引号 //
#define ADR_BLOCK83_WEEK_TAB_E2P   (ADR_BLOCK80_TRIFF_PARAM_E2P+OFFSET_WeekTab)
#define LEN_BLOCK83_WEEK_TAB_E2P      LEN_WeekTab

#define  No_BLOCK84_DAY_TAB_E2P    84    // 数据项目索引号 //
#define ADR_BLOCK84_DAY_TAB_E2P   (ADR_BLOCK80_TRIFF_PARAM_E2P+OFFSET_DayTab)
#define LEN_BLOCK84_DAY_TAB_E2P       LEN_DayTab
#endif


////////////////////             BLOCK 20        METER_PARAM1  /////////////
#define ADR_METER_PARAM1_MD                 (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,md_param))
#define ADR_METER_PARAM1_SETL_DDHH    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,bill_ms_param))
#define ADR_METER_PARAM1_EMU_FAST    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,EMU_fast_flg))
#define ADR_METER_PARAM1_METER_ID    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,meter_id[0]))
#define ADR_METER_PARAM1_RF_COMM_ID    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,RF_comm_id[0]))
#define ADR_METER_PARAM1_PW0    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,password[0][0]))
#define ADR_METER_PARAM1_PW1    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,password[1][0]))
#define ADR_METER_PARAM1_RF_PW0    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,RF_password[0][0]))
#define ADR_METER_PARAM1_RF_PW1    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,RF_password[1][0]))
#define ADR_METER_PARAM1_FC_CFG    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,fac_cfg[0]))
#define ADR_METER_PARAM1_BAT_LEVEL    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,bat_level[0]))
// 增加组合有功方式字//
#define ADR_METER_PARAM1_FAC_ENERGY   (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,fac_energy[0]))
#define ADR_METER_PARAM1_REC_ENERGY   (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,Rac_energy[0]))
// 负荷曲线运行参数//
#define ADR_METER_PARAM1_LOAD_PTR   (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,load_Ptr_var))
#define ADR_METER_PARAM1_LOAD_NUM   (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,load_Num_var))
// 负荷记录参数//
#define ADR_METER_PARAM1_load_lptime_inv_var   (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,load_lptime_inv_var))
#define ADR_METER_PARAM1_load_egcode_var   (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,load_egcode_var[0]))
#define ADR_METER_PARAM1_EE_ERR   (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,sys_e2_err))
//继电器参数
#define ADR_METER_PARAM1_RELAY_CMD    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,relay_cmd))
#define ADR_METER_PARAM1_RELAY_STATUS    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,relay_status))
//常数倍频标识
#define ADR_METER_PARAM1_CONST_FAST_FLG    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,const_fast_flg))
//首次上电检查标志 =0x654321为首次上电，需将单份区清零yang 2017-12-29//
#define ADR_METER_PARAM1_EEPROM_Flag    (ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,EEPROM_Flag[0]))
////////////////////            BLOCK21  电表参数区-显示参数? /////////////
#define ADR_DIS_PARAM_AUTO_SEC                 (ADR_BLOCK21_DIS_PARAM_E2P+ST_MB_OFFSET(DIS_PARAM_DATA,auto_sec))  // 自动循环显示持续时间  //
#define ADR_DIS_PARAM_KEY_SEC                 (ADR_BLOCK21_DIS_PARAM_E2P+ST_MB_OFFSET(DIS_PARAM_DATA,key_sec))  // 按键显示持续时间  //
#define ADR_DIS_PARAM_SLEEP_SEC                 (ADR_BLOCK21_DIS_PARAM_E2P+ST_MB_OFFSET(DIS_PARAM_DATA,sleep_sec))   // 背光点亮持续时间  //
#define ADR_DIS_PARAM_EC_FORM                 (ADR_BLOCK21_DIS_PARAM_E2P+ST_MB_OFFSET(DIS_PARAM_DATA,ec_form)) // 电量格式  //
#define ADR_DIS_PARAM_AUTO_TAB                 (ADR_BLOCK21_DIS_PARAM_E2P+ST_MB_OFFSET(DIS_PARAM_DATA,auto_item))  //0为显示个数，1~31为显示项目，最大可以显示31个项目  //


////////////////////////BLOCK 25     METER_VAR/////////////
#define ADR_METER_VAR_RTC                 (ADR_BLOCK25_METER_VAR_E2P+ST_MB_OFFSET(E2P_METER_VAR_MAP,save_rtc))

////////////////////////BLOCK 26     METER_VAR/////////////
#define ADR_BLOCK26_BILL_VAR_PTR_E2P   ADR_BLOCK26_BILL_VAR_E2P+OFFSET_bill_var_ptrNo
#define ADR_BLOCK26_BILL_VAR_DATETTIME_E2P   ADR_BLOCK26_BILL_VAR_E2P+OFFSET_bill_var_dateTime
//////////////////////BLOCK 27     METER_VAR/////////////
#define  ADR_CLASS2CNT_START_E2P         ADR_BLOCK27_EVENII_E2P+CLASSII_cnt_start
#define  ADR_CLASS2FLG_START_E2P         ADR_BLOCK27_EVENII_E2P+CLASSII_flg_start
#define  ADR_CLASS2DATA_START_E2P        ADR_BLOCK27_EVENII_E2P+CLASSII_data_start

///////////////////////////////////////////////////////////////////////////////////////////////////
//带校验单份数据区////
//////////////////////////////////////////////////////////////


//BLOCK101~136 月结算数据区  //
#if (BILL_MAX_NUM>0)
//BLOCK101  上1月结算数据区  //
#define No_BLOCK101_BILL1_DATA_E2P    101    // 数据项目索引号 //
#define ADR_BLOCK101_BILL1_DATA_E2P   EE_SINGLE_DATA_ADDR_START   //0. 当前有功电量数据区首地址  //
#define LEN_BLOCK10X_BILLX_DATA_E2P   sizeof(E2P_BILL_DATA_MAP_ONE)

//BLOCK102 上2月结算数据区  //
#define  No_BLOCK102_BILL2_DATA_E2P    102    // 数据项目索引号 //
#define ADR_BLOCK102_BILL2_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[1])   //0. 当前有功电量数据区首地址  //    

//BLOCK103  上3月结算数据区  //
#define  No_BLOCK103_BILL3_DATA_E2P    103   // 数据项目索引号 //
#define ADR_BLOCK103_BILL3_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[2])   //0. 当前有功电量数据区首地址  //
      

//BLOCK104  上4月结算数据区  //
#define  No_BLOCK104_BILL4_DATA_E2P    104    // 数据项目索引号 //
#define ADR_BLOCK104_BILL4_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[3])   //0. 当前有功电量数据区首地址  //

//BLOCK105  上5月结算数据区  //
#define  No_BLOCK105_BILL5_DATA_E2P    105    // 数据项目索引号 //
#define ADR_BLOCK105_BILL5_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[4])   //0. 当前有功电量数据区首地址  //

//BLOCK106  上6月结算数据区  //
#define  No_BLOCK106_BILL6_DATA_E2P    106    // 数据项目索引号 //
#define ADR_BLOCK106_BILL6_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[5])   //0. 当前有功电量数据区首地址  //

////BLOCK107  上7月结算数据区  //
//#define  No_BLOCK107_BILL7_DATA_E2P    107    // 数据项目索引号 //
//#define ADR_BLOCK107_BILL7_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[6])   //0. 当前有功电量数据区首地址  //

////BLOCK108  上8月结算数据区  //
//#define  No_BLOCK108_BILL8_DATA_E2P    108    // 数据项目索引号 //
//#define ADR_BLOCK108_BILL8_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[7])   //0. 当前有功电量数据区首地址  //

////BLOCK109  上9月结算数据区  //
//#define  No_BLOCK109_BILL9_DATA_E2P    109    // 数据项目索引号 //
//#define ADR_BLOCK109_BILL9_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[8])   //0. 当前有功电量数据区首地址  //

////BLOCK110  上10月结算数据区  //
//#define  No_BLOCK110_BILL10_DATA_E2P    110    // 数据项目索引号 //
//#define ADR_BLOCK110_BILL10_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[9])   //0. 当前有功电量数据区首地址  //

////BLOCK111  上11月结算数据区  //
//#define  No_BLOCK111_BILL11_DATA_E2P    111   // 数据项目索引号 //
//#define ADR_BLOCK111_BILL11_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[10])   //0. 当前有功电量数据区首地址  //

////BLOCK112  上12月结算数据区  //
//#define  No_BLOCK112_BILL12_DATA_E2P    112    // 数据项目索引号 //
//#define ADR_BLOCK112_BILL12_DATA_E2P   EE_SINGLE_DATA_ADDR_START +ST_MB_OFFSET(SGE2P_MAP,Bill_data[11])   //0. 当前有功电量数据区首地址  //
#endif



//BLOCK151  电表CLASSI 类事件区//
#define ADR_BLOCK150_EVENI_START_E2P   EE_SINGLE_DATA_ADDR_START+ ST_MB_OFFSET(SGE2P_MAP,even_I)   //   	1 类事件记录数据区首地址 //
#define  No_BLOCK150_EVENI_RELAY_ON_E2P  150    // 数据项目索引号 //
#define ADR_BLOCK150_EVENI_RELAY_ON_E2P   ADR_BLOCK150_EVENI_START_E2P+ ST_MB_OFFSET(E2P_EVEN_I_DATA_MAP,RELAY_ON)   //    //
#define  No_BLOCK151_EVENI_RELAY_OFF_E2P  151    // 数据项目索引号 //
#define ADR_BLOCK151_EVENI_RELAY_OFF_E2P   ADR_BLOCK150_EVENI_START_E2P+ ST_MB_OFFSET(E2P_EVEN_I_DATA_MAP,RELAY_OFF)   //    //

#define  No_BLOCK150_EVENI_PROG_E2P  150    // 数据项目索引号 //
#define ADR_BLOCK150_EVENI_PROG_E2P   ADR_BLOCK150_EVENI_START_E2P+ ST_MB_OFFSET(E2P_EVEN_I_DATA_MAP,PROG)   //    //
#define  No_BLOCK151_EVENI_resetMD_E2P  151    // 数据项目索引号 //
#define ADR_BLOCK151_EVENI_resetMD_E2P   ADR_BLOCK150_EVENI_START_E2P+ ST_MB_OFFSET(E2P_EVEN_I_DATA_MAP,resetMD)   //    //
#define  No_BLOCK152_EVENI_setRtc_E2P  152    // 数据项目索引号 //
#define ADR_BLOCK152_EVENI_setRtc_E2P   ADR_BLOCK150_EVENI_START_E2P+ ST_MB_OFFSET(E2P_EVEN_I_DATA_MAP,setRtc)   //    //
#define  No_BLOCK153_EVENI_MMERR_E2P  153    // 数据项目索引号 //
#define ADR_BLOCK153_EVENI_MMERR_E2P   ADR_BLOCK150_EVENI_START_E2P+ ST_MB_OFFSET(E2P_EVEN_I_DATA_MAP,MMERR)   //    //
#define  No_BLOCK154_EVENI_COMM_E2P  154    // 数据项目索引号 //
#define ADR_BLOCK154_EVENI_COMM_E2P   ADR_BLOCK150_EVENI_START_E2P+ ST_MB_OFFSET(E2P_EVEN_I_DATA_MAP,COMM)   //    //
#define  No_BLOCK155_EVENI_setTriff_E2P  155    // 数据项目索引号 //
#define ADR_BLOCK155_EVENI_setTriff_E2P   ADR_BLOCK150_EVENI_START_E2P+ ST_MB_OFFSET(E2P_EVEN_I_DATA_MAP,setTriff)   //    //
#define  No_BLOCK156_EVENII_EERR_E2P  156    // 数据项目索引号 //
#define ADR_BLOCK156_EVENII_EERR_E2P   ADR_BLOCK150_EVENI_START_E2P+ ST_MB_OFFSET(E2P_EVEN_I_DATA_MAP,EERR)   //    //

#define LEN_BLOCK15X_EVENI_E2P               LEN_CLASS_I_UNIT   // 
#define ADR_BLOCK_min_EVENI_E2P     ADR_BLOCK150_EVENI_PROG_E2P //     //
#define ADR_BLOCK_max_EVENI_E2P    ADR_BLOCK156_EVENII_EERR_E2P  //    //


//BLOCK151  电表CLASS II 类事件区//  160 起始
#define ADR_BLOCK160_EVENII_START_E2P   EE_SINGLE_DATA_ADDR_START+ ST_MB_OFFSET(SGE2P_MAP,even_II)   //   2 类事件记录数据区首地址 //
#define  No_BLOCK161_EVENII_PWN_E2P  161    // 数据项目索引号 //
#define ADR_BLOCK161_EVENII_PWN_E2P   ADR_BLOCK160_EVENII_START_E2P+ ST_MB_OFFSET(E2P_EVEN_II_DATA_MAP,PWN)   //    //
#define  No_BLOCK162_EVENII_lowbattery_E2P  162    // 数据项目索引号 //
#define ADR_BLOCK162_EVENII_lowbattery_E2P   ADR_BLOCK160_EVENII_START_E2P+ ST_MB_OFFSET(E2P_EVEN_II_DATA_MAP,lowbattery)   //    //
#define  No_BLOCK163_EVENII_COVER_E2P  163    // 数据项目索引号 //
#define ADR_BLOCK163_EVENII_COVER_E2P   ADR_BLOCK160_EVENII_START_E2P+ ST_MB_OFFSET(E2P_EVEN_II_DATA_MAP,Cover)   //    //

//#define  No_BLOCK163_EVENII_NOL1_E2P  163    // 数据项目索引号 //
//#define ADR_BLOCK163_EVENII_NOL1_E2P   ADR_BLOCK160_EVENII_START_E2P+ ST_MB_OFFSET(E2P_EVEN_II_DATA_MAP,NOL1)   //    //

//#define  No_BLOCK164_EVENII_NOL2_E2P  164    // 数据项目索引号 //
//#define ADR_BLOCK164_EVENII_NOL2_E2P   ADR_BLOCK160_EVENII_START_E2P+ ST_MB_OFFSET(E2P_EVEN_II_DATA_MAP,NOL2)   //    //
#define  No_BLOCK164_EVENII_L1reverse_E2P  164   // 数据项目索引号 //
#define ADR_BLOCK164_EVENII_L1reverse_E2P   ADR_BLOCK160_EVENII_START_E2P+ ST_MB_OFFSET(E2P_EVEN_II_DATA_MAP,L1reverse)   //    //
//#define  No_BLOCK166_EVENII_L2reverse_E2P  166  // 数据项目索引号 //
//#define ADR_BLOCK166_EVENII_L2reverse_E2P   ADR_BLOCK160_EVENII_START_E2P+ ST_MB_OFFSET(E2P_EVEN_II_DATA_MAP,L2reverse)   //    //

#define LEN_BLOCK16X_EVENI_E2P            LEN_CLASS_II_UNIT   // 
#define ADR_BLOCK_min_EVENII_E2P     ADR_BLOCK161_EVENII_PWN_E2P //     //
#define ADR_BLOCK_max_EVENII_E2P    ADR_BLOCK164_EVENII_L1reverse_E2P  //    //




//负荷曲线180  单份  不校验//
#define  No_BLOCK180_LOAD_START_DATA   180    // 数据项目索引号 //
#define  ADR_BLOCK180_LOAD_START_E2P   EE_SINGLE_DATA_ADDR_START+ ST_MB_OFFSET(SGE2P_MAP,load_start_e2p)   //0x472  1138//

/***************************************************************
*    END
****************************************************************/

#endif


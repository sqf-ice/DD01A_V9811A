/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Prco_Data_H.H
**Author		: maji
**date			: 2016-04-26
**description	: 系统相关数据处理C文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/



#ifndef _PROC_DATA_H
#define _PROC_DATA_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/


/***************************************************************
*    2     数据结构声明     
****************************************************************/
typedef struct 
{	
	INT8U	back_fg;                     // 中断形成标识 //
	INT8U	font_fg;                     // 程序处理过程中产生标识 //
} SYS_RUN_STATUS;


typedef struct 
{	
    INT8U  reset_status;                     // 系统复位标识字 //
    INT8U  work_mode;             // 系统电源标识 //
    INT8U  delay_emu;                    // EMU 稳定时间，初始化值=2， //
    INT8U  gs_check_err_status;   //校验状态字  //
    INT8U  gs_hard_err_status;   //硬件状态字//
    INT8U  gs_net_status;             //电网状态字//
    INT8U  open_cover_status;     //开表上盖状态字=0x5A代表发生开上盖//
    INT8U  meter_factory_status;     //电表工厂状态字=0x5A代表处在工厂状态//
    INT8U  fac_energy[2]; 			//有功计量模式字
    INT8U  Rac_energy[2]; 			//无功计量模式字
    ST_U32_U08  sys_e2_err; 	    //用于E2错误//
    INT8U   sys_e2_err_flg; 		//用于E2错误发生标识//
    ST_U32_U08 sleep_sec;   			//下电显示维持时间//
    INT8U  qf_time;     			//无功脉冲灯//
} SYS_GLOBAL_VAR;

typedef struct  
{
     INT8U      meter_cons[3];           //电表常数//
     INT8U      video[12];               //软件版本号12个字节ASCII码//
}SYS_PARM;





typedef struct 
{
    MD_PARAM                                md_param; // 2
   #if (BILL_MAX_NUM>0)
    BILL_MONTH_SETTLED_PARAM     bill_ms_param;  // 2
   #endif
    INT8U   EMU_fast_flg;  //EMU 加倍标识=0x3c 为加倍，其他为正常 // 1
    INT8U   meter_id[16];                                //16
    INT8U   RF_comm_id[6];        //RF通讯地址
    INT8U   password[2][4];   //  2 级密码 //   8
    INT8U   RF_password[2][4];   // RF 2 级密码 //   8
    INT8U   fac_cfg[4];          //  工厂信息配置字//4
    INT8U   bat_level[2];          //  电池电压阀值//2
    INT8U    fac_energy[2];  // 有功组合模式字
    INT8U    Rac_energy[2];  // 无功组合模式字
   //负荷曲线运行参数//
   INT16U   load_Ptr_var;
   INT16U   load_Num_var;
   INT16U   load_lptime_inv_var;     //各通道记录时间间隔（Min）  	2       BCD  XXXX     
   INT8U    load_egcode_var[10];  // 各寄存器代码(1+9)   			10      BCD  XXXX         第一个位个数 
   ST_U32_U08    sys_e2_err; 			 //用于计度器驱动的脉冲数 //
   INT8U   relay_cmd;			//继电器控制命令 =0x00 合闸 =0xA3 拉闸
   INT8U   relay_status;		//继电器状态 =0x00合闸 =0xA3  拉闸
   INT8U   const_fast_flg;		//常数扩大标识 =0x3C 常数扩大10倍，其他常数保持
   INT8U   EEPROM_Flag[3];       // //  首次上电EEPROM检查状态字 !=0x653412标志当前为首次上电 
   INT16U   param1_csck;    //16位累加和校验值//
} E2P_METER_PARAM1_MAP;

typedef struct 
{
    RTC_TYPE  save_rtc;  //下电保存的时钟 //
    INT16U  var_csck;    //16位累加和校验值//
} E2P_METER_VAR_MAP;


/***************************************************************
*    3    变量声明       
****************************************************************/
extern SYS_RUN_STATUS  gs_sys_run;
extern SYS_GLOBAL_VAR gs_sys_globaL_var;

//常量表 //
extern const  E2P_METER_PARAM1_MAP   code  default_meter_parm1_tab;
extern const SYS_PARM code sys_parm;

/***************************************************************
*    4     函数声明        
****************************************************************/
extern void Proc_sys_data_init(void);

/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
//  gs_sys_run.font_fg define 

//  gs_sys_globaL_var.open_cover_status define 
#define  OPENED_COVER_MARK      0x5A     // =0X5A 代表开表上盖事件发生//

//  gs_sys_globaL_var.meter_factory_status define 
#define  METER_FACTORY_MARK      0x5A     // =0X5A 代表处在工厂状态//

//  gs_sys_run.back_fg define 
#define  BIT0_FONT_FG_10MS   BIT0   // 10ms 中断产生标识 //
#define  BIT1_BACK_FG_RTC   BIT1   // RTC产生的1S中断标识 //
#define  BIT2_BACK_DIS_KEY   BIT2   // 显示按键产生的中断标识 //

#define BIT3_FONT_FG_readpower    BIT3// 读取后检验完成标识//
#define BIT4_FONT_FG_EnyBottom    BIT4// // 参数刷新
/***************************************************************
*    END
****************************************************************/

#endif

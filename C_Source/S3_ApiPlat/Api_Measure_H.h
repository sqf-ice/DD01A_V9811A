/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Measure_H.H
**Author		: maji
**date			: 2016-04-21 
**description	: c code for 测量程序声明文件
**note			: MCU- G80F92XD ，MERTER FOR DL06A
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-21   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#ifndef _Api_Measure_H
#define _Api_Measure_H


/***************************************************************
*    1     立即数预定义   
****************************************************************/



/***************************************************************
*    2     数据结构声明     
****************************************************************/
typedef struct 
{
    ST_U32_U08 dw_u_val;			//电压寄存器，len=4  //
    ST_U32_U08 dw_i_val;			//电流寄存器，len=4  //
    ST_U32_U08 dw_p_val;			//有功功率寄存器，len=4  //
    ST_U32_U08 dw_q_val;			//有功功率寄存器，len=4  //
    ST_U16_U08 dw_pf_val;			//功率因数寄存器，len=2  //
    ST_U16_U08 w_freq_val;		    //电网频率，len=2  //
}ELECTRIC_VAR_UNIT;

typedef struct  
{
    ELECTRIC_VAR_UNIT  gs_really[PHASE_MAX_UNM];   
    ST_U32_U08 		   dw_i_n_val;	        //零线电流，len=4  //
    ST_U32_U08 		   dw_p_n_val;        //零线功率，len=4  //
    ST_U16_U08		   w_temp;             //电表温度，len=2  //
    ST_U16_U08		   w_bat_v;            //电池电压，len=2 //
}MEASURE_VAR_DATA;




typedef struct  
{
    uint8  cnt_sec[3];            //状态判断秒计数器,   0-功率反向 
    uint8  cnt_dimmer;         //   判断计数器 //
    uint8  u8_status;          //判断状态 //
    uint8  emu_ch;             //当前计量通道  //
}MEASURE_STATU_JUDGE_VAR;

/***************************************************************
*    3    变量声明       
****************************************************************/
extern MEASURE_VAR_DATA gs_measure_var_data;
extern MEASURE_STATU_JUDGE_VAR gs_measure_status_judge_var;



/***************************************************************
*    4     函数声明        
****************************************************************/
extern void api_Measure_ram_INIT(void);
extern void api_measure_deal_pre_second(void);
extern void api_measure_VBAT_pre_min(void);
extern void api_measure_temperature_pre_min(void);


/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/


/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
//gs_measure_status_judge_var.u8_status define
#define REV_ACPOWER_L      BIT0       //合相反向标识 //
#define VBAT_LOW           BIT1       //电池电压低标识  //



/***************************************************************
*    END
****************************************************************/

#endif

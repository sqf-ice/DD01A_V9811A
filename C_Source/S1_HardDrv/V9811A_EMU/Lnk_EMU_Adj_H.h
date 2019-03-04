/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Lnk_EMU_Adj_H.H
**Author		: maji
**date			: 2016-05-09 
**description	: G80F92XD的EMU计量模块链路层校表处理C代码的声明文件
**note			: MCU- G80F92XD ，MERTER FOR DL06A
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-05-09   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#ifndef _Lnk_EMU_Adj_H
#define _Lnk_EMU_Adj_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/
// 自动校表步骤立即数定义//
#define HARD_EADJ_STEP0_L_INITLIZE    0  // 空载电流校准  //
#define HARD_EADJ_STEP1_L_100IB_100L    1  // 空载电流校准  //
#define HARD_EADJ_STEP2_L_100IB_50L    2  // 空载电流校准  //
#define HARD_EADJ_STEP3_L_5IB_100L    3  // 空载电流校准  //
#define HARD_EADJ_STEP4_N_INITLIZE    4  // 空载电流校准  //
#define HARD_EADJ_STEP5_N_100IB_100L    5  // 空载电流校准  //
#define HARD_EADJ_STEP6_N_100IB_50L    6  // 空载电流校准  //
#define HARD_EADJ_STEP7_N_5IB_100L    7  // 空载电流校准  //
#define HARD_EADJ_STEP8_METER_CLR    8  // 电表清零处理//

// 计量校准相关参数，参考技术文档中计算表格内的数据进行赋值 //
#define INITGATECP 		0x927C00      // 
#define INITGATECQ 		INITGATECP      //  无功潜动门限 0.1%//

#define INITGATEP  		73508912//E1门限 按照数据手册的计算理论数据*0.9 60hz门限值扩大1.2倍 // 
#define INITGATEQ  		(INITGATEP*1.568)// E2无功门限//
#define INITGATEP_10M  	(INITGATEP/10L)//常数扩大10倍，对应的门限值应缩小10倍  yang 2017-12-28
#define INITGATEQ_10M  	(INITGATEQ/10L)//常数扩大10倍，对应的门限值应缩小10倍  yang 2017-12-28

// soc  的功率理论值
#define REF_RMSPn          6534125L   //额定电压和基本电流点的有功功率理论值 //  
#define REF_RMSPn_9260     6534125L   //额定电压和基本电流点的有功功率理论值 //  


#define K_RMSP              0.003673042f  //9811有功功率系数 
#define K_RMSP2             0.003673042f  //无功功率系数

#define REF_START_Pn          ((uint32) (REF_RMSPn*0.001f))       //启动功率0.001 的基本点功率标准值 //  
#define REF_START_REV_Pn      ((~((uint32)(REF_RMSPn*0.001f)))+1) //启动功率0.001 的基本点功率标准值 //  



/***************************************************************
*    2     数据结构声明     
****************************************************************/
typedef struct
{
    uint32 w1gain;          //通道1功率增益寄存器(有功无功使用同一组)  //
    uint32 p1cal;           //通道1电压电流相位补偿寄存器  //
    uint32 watt1os;        //通道1有功功率偏置  //
    uint32 irms1os;        //通道1电流有效值偏置  //
    uint32 w2gain;          //通道2功率增益寄存器(有功无功使用同一组)  //
    uint32 p2cal;           //通道2电压电流相位补偿寄存器  //
    uint32 watt2os;        //通道2有功功率偏置  //
    uint32 irms2os;        //通道2电流有效值偏置  //
    uint32 urmsos;        //电压有效值偏置  //
    float vconst_val_ch1;   //通道1掉零线电压固定值 //    
    float vconst_val_ch2;   //通道2掉零线电压固定值 //
    float vrms_xs;          //人为计算的电压有效值系数  //
    float i1rms_xs;         //人为计算的通道1电流有效值系数  //
    float i2rms_xs;         //人为计算的通道2电流有效值系数  //
    uint16     csck;  		//校验和 //
}ST_EMU_ADJ_PARAM;


typedef struct
{
    uint32 U_ref;       //参比电压值，量纲0.1V  // 
    uint32 I_ref;       //参比电流值，量纲0.001A  //     
    uint32 C_GATEP;    // 计量门限值 //
    uint32 C_GATECP;   // 潜动门限值 //
    uint32 C_PCONST;   // 额定功率的校准后标准值//
    float   Kim_Prms;      //有功功率有效值系数//
    float   Kim_Prms2;     //无功功率有效值系数//
}ST_Adjust_CONST;

typedef struct 
{
    uint8   sec_flg;  //校准过程中的秒变化标识//
    uint8   adj_flg;  //校表过程状态字=0X55代表校表过程中 //
} ST_EMU_ADJ_VAR;


typedef struct 
{
    uint32   *ramAdr;                            //变量地址 //
    uint16    emuAdr;                            //寄存器地址 //
}EMU_CK_TAB;   

typedef struct 
{
    uint32   val;                               //常量数据//
    uint16    emuAdr;                      //寄存器地址 //
}EMU_CONST_CK_TAB;   



typedef struct 
{
    uint32   val_9260F;                               //常量数据//
    uint16    emuAdr_9260F;                      //寄存器地址 //
}EMU_CONST_CK_TAB_9260F;   

/***************************************************************
*    3    变量声明       
****************************************************************/
extern const  ST_EMU_ADJ_PARAM  code   default_adj_emu_param_tab;
extern const  ST_Adjust_CONST  code   cst_adj_const;
extern const  EMU_CK_TAB  code   cst_emu_selfchck_tab[];
extern const  EMU_CONST_CK_TAB  code   cst_emu_constchck_tab[];

extern ST_EMU_ADJ_PARAM  gs_adj_emu_param;
extern ST_EMU_ADJ_VAR      gs_emu_adj_var;


extern const  EMU_CONST_CK_TAB_9260F  code   cst_emu_constchck_tab_9260f[];
/***************************************************************
*    4     函数声明        
****************************************************************/
extern void Get_EMU_AdjParam(void);
extern void Check_EMU_AdjParam(void);
extern void Hardware_AdjustDiv_Sub(void);
extern void Save_EMU_AdjParam(void);

extern void Measure_Electric_Err_Gain(INT8U *sptr);
extern void Calibration_Power(float f_err_data);
extern void Calibration_Current(INT32U Current,INT32U Voltage);
extern void Calibration_Angular(float f_err_data);
extern void Calibration_small_signal(float f_err_data);

extern uint32 CalculateSChz(int16 error,int32 LastSC);
/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/



/***************************************************************
*    END
****************************************************************/
#endif


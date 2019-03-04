/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: SYS_Config_H.H
**Author		: maji
**date			: 2016-04-26
**description	: define for main config 
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/



#ifndef 	_SYS_Config_H
#define	_SYS_Config_H

// 系统主频 //
#define	  FSYSCLK_800k		 819200			  //系统基础主频819.2KHZ // 
#define	  FSYSCLK_1M6		 (FSYSCLK_800k*2	)		 //系统基础主频819.2KHZ // 
#define	  FSYSCLK_3M2		 (FSYSCLK_800k*4	)		 //系统基础主频819.2KHZ // 
#define	  FSYSCLK_6M5		 (FSYSCLK_800k*8	)		 //系统基础主频819.2KHZ // 
#define	  FSYSCLK_13M		 (FSYSCLK_800k*16)		 //系统基础主频819.2KHZ // 
#define	  FSYSCLK_26M		 (FSYSCLK_800k*32)		 //系统基础主频819.2KHZ // 


#define	  FSYSCLK			 FSYSCLK_3M2//FSYSCLK_6M5			 //系统频率6M5  //


//#define _DEBUGE_FUNCTION    // 不需要调试时屏蔽本条 mj 2016-8-1 //

//******************************************************************
//	数据大小端定义，
//   C51为大端，32位数据高8位字节存储在地址[0]	//
//   MDK中的M0为小端，32位数据低8位字节存储在地址[0]	
//******************************************************************
//#define LITTLE_END      // 此条有效为小端模式,此条注释掉为大端模式//

// 电能表参数//
#define METER_CONST  1000UL
#define METER_CONST_1KWH  METER_CONST
#define METER_CONST_10WH  ((uint8)(METER_CONST/100UL))
#define METER_CONST_10WH_BCD  0X10

#define       C_Un             240U                //额定电压  //
#define       C_Ib             10U                  //额定电流 //
#define       C_Fq             60U                  //额定频率 //

//  电能表线制//
#define PHASE	1  // 单相 //
#define WIRE    2      // 两线 //
#if (PHASE>1)
#define PHASE_MAX_UNM	PHASE+1  // 多相表专用 //
#else
#define PHASE_MAX_UNM	PHASE     // 单相表专用 //
#endif
//#endif

#define PHASE_TT  0   //合相通道  //
#define WIRE_L1  1	  //多相表 L1线//
#define WIRE_L2  2	  //多相表 L2线//
#define WIRE_L3  3	  //多相表 L3线//
#define WIRE_N   4	  // N线//


//电量整数最大范围//
#define MAX_ENERGY_INtEGER   99999999    // 电量数据最大为99999999 //


//  电能计量通道相关定义  //
#define EC_CH_NUM   		 2      //  电能计量总通道数 //
#define EC_CH_AC_PT_TT  	 0     //索引0：  合相总有功电能//
#define EC_CH_RE_PT_TT       1     //索引1：  合相总无功电能
//#define EC_FOR_AC_NUM   	 1     //有功正向
//#define EC_REV_AC_NUM        2     // 有功反向
//#define EC_CH_RE_PT_TT       3     //索引3：  合相总无功电能
//#define EC_FOR_REC_NUM   	 4     //无功正向
//#define EC_REV_REC_NUM       5     //无功功反向

//  最大需量计量通道相关定义  //
#define MD_CH_NUM   1   //  最大需量计量通道 //
#define MD_CH_AC_PT_TT   0   //索引0：  合相总有功电能//
#define MD_CH_RE_PT_TT   1   //索引0：  无功//

// 结算相关配置字 //
#define BILL_MAX_NUM  6 // =0为无结算功能，= 12为上12个月结算 //



#define OTHER_EMU   1 //  外挂EMU 控制  1 有外挂 0单soc

// 费率相关定义 //
#define TARIFF_MAX_NUM  4  // 单费率 //
#define TARIFF_T 0  // 总费率//
#define TARIFF1  1  //费率1 //
#define TARIFF2  2  //费率2 //
#define TARIFF3  3  //费率3 //
#define TARIFF4  4  //费率4 //


// 负荷记录开关



//16位累加校验和初始值  //
#define  CHECKWORD          0xa55a         // 双字节累计和校验基数   //


/***************************************************************
*    END
****************************************************************/

#endif


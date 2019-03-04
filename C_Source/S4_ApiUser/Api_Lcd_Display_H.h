/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Lcd_Display_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: 液晶显示处理应用层相关函数程序H文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#ifndef _Api_Lcd_Display_H
#define _Api_Lcd_Display_H


/***************************************************************
*    1     立即数预定义   
****************************************************************/
//DIS_PIXEL_VAR 部分BUFF下标定义//
#define DS_UNIT			12 	//共10 个显示单元//
#define DS_DATANum		6  //大数字个数//
#define DS_OBISNum		3  //OBIS个数//
#define DS_CHARNum		3  //其他字符个数//

//  //
#define DIS_RESET 	  0   //全屏自检显示  //
#define DIS_ITEM_V    1   //电压屏 //

#define DIS_MAX_ITEM   6    // 最大显示序号6//

#define DIS_DEFAULT_ITEM 	  3   //默认显示当前总有功电量项目  //
#define RESEAT_DIS_KEEP_3S   2	//刚上的复位全屏显示保持时间 //
//#define DIS_ERR_START  7      // 错误类显示起始屏数，目前常显开盖错误


// 硬件校表显示项目 //
#define DIS_ADJ_CH1_0 	  0XA0   //L 回路校表参数初始化 //
#define DIS_ADJ_CH1_1 	  0XA1  //L 回路1.0Ib 比差校准 //
#define DIS_ADJ_CH1_2 	  0XA2   //L 回路1.0Ib 0.5L 角差校准 //
#define DIS_ADJ_CH1_3 	  0XA3   //L 回路0.05Ib 小信号补偿校准 //
#define DIS_ADJ_CH2_0 	  0XA4   //N 回路校表参数初始化 //
#define DIS_ADJ_CH2_1 	  0XA5  //N 回路1.0Ib 比差校准 //
#define DIS_ADJ_CH2_2 	  0XA6   //N回路1.0Ib 0.5L 角差校准 //
#define DIS_ADJ_CH2_3 	  0XA7   //N回路0.05Ib 小信号补偿校准 //
#define DIS_DATA_CLR 	  0XA8   //电表清零 //
#define DIS_DATA_BROADCAST 	  0XA9 //广播命令显示 //
//DIS_CTL_VAR.mode define //
#define DIS_MODE_RESET	  0x01   // 复位
#define DIS_MODE_NOMAL  0x02  	//轮显

//LCDDISTAB 定义//
//LCDDISTAB.chartype define //
#define CHAR_AEC		0x01		// 电量类
#define CHAR_MD			0x02		// 需量类
#define CHAR_MD_TIME	0x03		// 需量发生时间
#define CHAR_V			0x04		// 电压类
#define CHAR_A			0x05		// 电流类
#define CHAR_KW			0x06		// 功率类
#define CHAR_HZ			0x07		// 频率类
#define CHAR_TIME		0x08		// 时间类
#define CHAR_DATE		0x09		// 日期类
#define CHAR_PF			0x0A		// 功率因数类
#define CHAR_SETTL		0x0B		// 结算参数类
#define CHAR_METERID     0x0C                  //表号类
//LCDDISTAB.obis_digit define //
#define  OBIS_DIGIT_TYPE0  0  // xxxx
#define  OBIS_DIGIT_TYPE1  1  // xxx.x
#define  OBIS_DIGIT_TYPE2  2  //xx.xx
#define  OBIS_DIGIT_TYPE3  3  // x.xxx
#define  OBIS_DIGIT_TYPE4  4  // x.xx.x

/***************************************************************
*    2     数据结构声明     
****************************************************************/
//显示参数表区 //
typedef struct 
{
    uint8  auto_sec;    // 自动循环显示持续时间  //
    uint8  key_sec;     // 按键显示持续时间  //
    uint32  sleep_sec;     //下电显示维持时间//
    uint8  ec_form;     // 电量格式  //
    uint8  auto_item[DIS_MAX_ITEM+1];    //0为显示个数，1~6为显示项目，最大可以显示6个项目  //
    INT16U  u16_csck;    //16位累加和校验值//
} DIS_PARAM_DATA;

//显示控制变量区 //
typedef struct 
{
    uint8  mode;    // 显示模式 //
    uint8  item;      // 当前显示项目  //
    uint8  ptr[4];    //各种显示模式的项目指针，0序号常规显示模式 //
    uint8  keep_sec[4];  // 显示类持续时间, 0序号为自动循环显示,1序号为按键显示保持时间2为背光维持时间3为上电全屏保持时间//
} DIS_CTL_VAR;

//显示元素变量区 //
typedef struct 
{
    uint8  dis_buff[DS_UNIT];    // 显示缓存 //
    uint8  dis_data_buff[DS_DATANum];				// data区8 位数据显示转换数据 //
    uint8  dis_obis_buff[DS_OBISNum];				// OBIS 区 6 位数据显示转换数据 //
    uint8  dis_char_buff[DS_CHARNum];				// 组合CHAR区14个符号显示转换数据 //
} DIS_PIXEL_VAR;


//显示像素表结构定义
typedef struct  
{
    INT8U      DisID;               //显示ID//
    INT16U     ObisNum;         // OBIS数据,简化为4个数字//
    INT16U     Addr;                //ram或 E2 地址//
    INT16U     DisInfo ;            // 存储类型(H)+ 显示位数(L) // 
    INT8U      chartype;            // 数据类型 //        
    INT8U      obis_digit;           //OBIS小数格式类型//
}LCDDISTAB;

/***************************************************************
*    3    变量声明       
****************************************************************/
extern DIS_PARAM_DATA   gs_dis_param;   // 电量显示相关参数  //
extern DIS_CTL_VAR  gs_dis_ctl_var;
extern DIS_PIXEL_VAR  gs_dis_pixel_var;
extern LCDDISTAB   gs_LCDDISTAB_var;

extern const DIS_PARAM_DATA  code  default_dis_param_tab;

extern INT8U dis_num_char;

/***************************************************************
*    4     函数声明        
****************************************************************/
extern void api_init_display(void);
extern void api_handl_dis_sleep(void);  
extern void api_updated_LCDDisplayPixel_per_second(void);
extern void api_updated_LED_per_second(void);
extern void api_updated_LCDDisplayItem_per_second(void);
extern INT8U api_get_next_dis_item(void);
extern INT8U Link_Get_DisTab_Var(INT8U u8_dis_itm);
extern void Link_Get_Dis_OBIS_Num_buff(void);
extern void Link_Get_Dis_Data_Num_buff(void);
extern void Link_Get_Dis_Char_buff(void);
extern void Link_Get_Dis_RealChar_buff(void);
extern void Link_Get_Dis_Drv_buff(void);
extern void api_get_energy_LCD(uint8*  temp, uint8 type, uint8*  result);
extern void api_chg_LCDDisplay_adj_item(uint8 u8_item);
extern void api_LCDDisplay_adj_item(uint8 u8_item);
extern void LED_display(void);
/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
//********************************************************************
// LCD character bitmap
//          3                a
//       -----            -----
//       |     |          |     |
//      2|     |7        f|     |b
//       |  6  |          |  g  |
//        -----            -----
//       |     |          |     |
//      1|     |5        e|     |c
//       |  0  |          |  d  |
//        -----  *4        -----  *dpc
//******************************************************************* 
//********************************************************************
// 常规数据区
//*******************************************************************                   								
//******************************************************************* 
//********************************************************************
// 常规数据区
//*******************************************************************                   								
#define DS_Chr_0     0x5F   // 0: 
#define DS_Chr_1     0x06   // 1:
#define DS_Chr_2     0x6B   // 2: 
#define DS_Chr_3     0x2F   // 3: 
#define DS_Chr_4     0x36   // 4: 
#define DS_Chr_5     0x3D   // 5: 
#define DS_Chr_6     0x7D   // 6: 
#define DS_Chr_7     0x07   // 7: 
#define DS_Chr_8     0x7F   // 8: 
#define DS_Chr_9     0x3F   // 9: 
#define DS_Chr_A     0x77   // A: 
#define DS_Chr_b     0x7C   // B: 
#define DS_Chr_C     0x59   // C: 
#define DS_Chr_d     0x6E   // D: 
#define DS_Chr_E     0x79   // E: 
#define DS_Chr_F     0x76   // F: 

//********************************************************************
// 下排OBIS   前3 位数字 
//*******************************************************************                 								
#define DS_OBIS_Chr_0     0xAF   // 0: 
#define DS_OBIS_Chr_1     0x06   // 1:
#define DS_OBIS_Chr_2     0x6D   // 2: 
#define DS_OBIS_Chr_3     0x4F   // 3: 
#define DS_OBIS_Chr_4     0xC6   // 4: 
#define DS_OBIS_Chr_5     0xCB   // 5: 
#define DS_OBIS_Chr_6     0xEB   // 6: 
#define DS_OBIS_Chr_7     0x0E   // 7: 
#define DS_OBIS_Chr_8     0xEF   // 8: 
#define DS_OBIS_Chr_9     0xCF   // 9: 
#define DS_OBIS_Chr_A     0xEE   // A: 
#define DS_OBIS_Chr_b     0xE3   // B: 
#define DS_OBIS_Chr_C     0xA9   // C: 
#define DS_OBIS_Chr_d     0x67   // D: 
#define DS_OBIS_Chr_E     0xE9   // E: 
#define DS_OBIS_Chr_F     0xE8   // F: 



#define DS_Chr_All_NUM 16  
//字符定义//

//dis_var.dis_char_buff[0]  bit6:bit0      //
#define CHAR_DATA_P1P2 	BIT0   
#define CHAR_DATA_P3 	BIT1    
#define CHAR_DATA_P4P5 	BIT2    
#define CHAR_DATA_P6	BIT3   
#define CHAR_DATA_P7 	BIT4   
#define CHAR_DATA_P8 	BIT5  
#define CHAR_DATA_P9 	BIT6   


//dis_var.dis_char_buff[1]  bit7:bit0      //
#define CHAR_DATA_kWh 	BIT0   
#define CHAR_DATA_kW 	BIT1   
#define CHAR_DATA_S1 	BIT2  
#define CHAR_DATA_S2 	BIT3  
#define CHAR_DATA_S3 	BIT4  
#define CHAR_DATA_S4 	BIT5  
#define CHAR_DATA_Received 	BIT6
#define CHAR_DATA_Delivered 	BIT7  



//dis_var.dis_char_buff[2]  bit7:bit0      //
#define CHAR_DATA_k 	BIT0   
#define CHAR_DATA_W 	BIT1   
#define CHAR_DATA_V 	BIT2  
#define CHAR_DATA_A 	BIT3  
#define CHAR_DATA_Hz 	BIT4  
#define CHAR_DATA_PF 	BIT5  
#define CHAR_DATA_MD 	BIT6  
#define CHAR_DATA_S9 	BIT7 



/***************************************************************
*    END
****************************************************************/

#endif

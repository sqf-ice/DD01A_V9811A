/*******************************************************************
*
*  FILE             :                                         
*  DATE            :                                    
*  DESCRIPTION :            
*  CPU GROUP    :                                                
*                                                                    
*  
*
*******************************************************************/

#ifndef _Api_AdjErr_H
#define  _Api_AdjErr_H
//------------------------------------------------------------------------//
//                  立即数预定义
//------------------------------------------------------------------------//
//#define URMS_MAX         1651          // XXXX.X V  数据格式，1270 *1.3 = 1651   127.0v *1.3 //
//#define IRMS_MAX         26000          // xxx.xxx A  数据格式，20000 *1.3 = 26000   20.000A *1.3 //
//#define PRMS_MAX        16510          //XX.XXXX kW  数据格式，1270*20*0.5*1.3 = 16510    //

//#define URMS_REF         1270          // XXXX.X V  数据格式， 127.0v //
//#define IRMS_REF         20000          // xxx.xxx A  数据格式， 20.000A //
//#define PRMS_REF        12700          //XX.XXXX kW  数据格式，1270*20*0.5 = 12700    //

//#define URMS_MIN         800            // XXXX.X V  数据格式，1270 *0.7 = 889   127.0v *0.7 //
//#define IRMS_MIN         13000          // xxx.xxx A  数据格式，20000 *0.7 = 14000   20.000A *0.7 //
//#define PRMS_MIN        8890          //XX.XXXX kW  数据格式，1270*20*0.5*0.7 = 16510    //

//#define PRMS_104PRE        13208          //XX.XXXX kW  数据格式，1270*20*0.5*1.04 = 13208   //
//#define PRMS_96PRE        12192          //XX.XXXX kW  数据格式，1270*20*0.5*0.96= 12192  //


//#define SEC_INIT_ADJ_ERR     5          // 校表参数初始化结束时间点//
//#define SEC_UI_ADJ_ERR	 11          // 电流电压增益校表时间点//         
//#define  SEC_PH_ADJ_ERR  	 12        //PH 相位校表时间点//  		  		
//     

//#define FLG_HAD_ERR_ADJ            1        // 本次短接已经完成校表标志//  



// 校表类型//

//#define ADJ_err_100IB_10L    BIT0 
//#define ADJ_err_100IB_05L    BIT1 
//#define ADJ_err_5IB_10L        BIT2 



//------------------------------------------------------------------------//
//                               数据结构声明
//------------------------------------------------------------------------//
//struct AUTO_ERR_ADJ_VAR
//{
//    INT8U u8_flg;          	           //校表标志  //
//    INT8U u8_sec_cnt;		 //秒定时器 //
//    INT8U u8_tpye;          	           //校表类型 //
//    INT32U u32_urms_tmp[PHASE_MAX_UNM];           // 两个回路的连续3次的表计电压有效值数据寄存器 //
//    INT32U  u32_irms_tmp[PHASE_MAX_UNM];             // 两个回路的连续3次的表计电流有效值数据寄存器 //
//    INT32U u32_prms_tmp[PHASE_MAX_UNM];            // 两个回路的连续3次的表计有功功率有效值数据寄存器 //
//     INT8U u8_vgain[PHASE_MAX_UNM];		//		//
//     INT8U u8_igain[PHASE_MAX_UNM];			//		//
//     INT8U u8_phsi[PHASE_MAX_UNM];
//} ;


//typedef struct 
//{
//    uint8   sec_flg;  //校准过程中的秒变化标识//
//    uint8   adj_flg;  //校表过程状态字=0X55代表校表过程中 //
//} ST_EMU_ADJ_VAR;



//typedef struct
//{
//    uint32 U_ref;       //参比电压值，量纲0.1V  // 
//    uint32 I_ref;       //参比电流值，量纲0.001A  //     
//    uint32 C_GATEP;    // 计量门限值 //
//    uint32 C_GATECP;   // 潜动门限值 //
//    uint32 C_PCONST;   // 额定功率的校准后标准值//
//    float   Kim_Prms;          //有功功率有效值系数//   
//}ST_Adjust_CONST;


//---------------------------------------------------------------//
//                  外部  变量声明
//---------------------------------------------------------------//
//extern struct AUTO_ERR_ADJ_VAR  gs_auto_err_adj_var;
//extern ST_EMU_ADJ_VAR      gs_emu_adj_var;

//extern const  ST_Adjust_CONST  code   cst_adj_const;

//---------------------------------------------------------------//
//                  外部  函数声明
//---------------------------------------------------------------//
//extern void  Proc_Auto_Err_Adj_Pre_sec(void);
//extern void  Calcu_Auto_Err_Channel_Para_PH(INT8U u8_channel);
//extern  void  Calcu_Auto_Err_Channel_Para_UI(INT8U u8_channel);
//extern void Initlize_Auto_Err_Channel_Para(INT8U u8_channel);
//extern INT8U  Judge_Auto_Err_Adj_Input_Data(INT8U u8_channel);
//extern void  Initlize_Auto_Err_Adj_Var(void);




#endif
/*******************************************************************
*
*  end of file                                    
*
*******************************************************************/





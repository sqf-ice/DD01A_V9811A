/*******************************************************************
* Copyright (c) 2010
* All rights reserved.
* 
* 文件名称：data.c
* 摘    要：           
*
* 应用系统：
*
* 版本号：     v01.00            
* 作者：          sosomj  
* 完成日期：2010.08.12
****************************************************************/
#include <MyIncludes_H.h>

//**********************************************************************
//*系统全局变量定义
//**********************************************************************
//struct AUTO_ERR_ADJ_VAR  gs_auto_err_adj_var;

// ST_EMU_ADJ_VAR      gs_emu_adj_var;

//增加
// ST_EMU_ADJ_PARAM  gs_adj_emu_param;


//const  ST_Adjust_CONST  code   cst_adj_const = 
//{
//    C_Un*10,        //uint32 U_ref;       //参比电压值，量纲0.1V  // 
//    C_Ib*1000,     //uint32 I_ref;       //参比电流值，量纲0.001A  //     
//    INITGATEP,     //uint32 GATEP;    // 计量门限值 //
//    INITGATECP,   //uint32 C_GATECP;   // 潜动门限值 //
//    REF_RMSPn,    // uint32 C_PCONST;   // 额定功率的校准后标准值//
//    K_RMSP,         //float   Kim_Prms;          //有功功率有效值系数//   
//};

//-------------------------------------------------------------------------------//
//  Function:      void get_default_mem_data(INT8U index)     
//  Description:  根据数据区域获取默认数据
//  Parameters:   无  
//  Returns:        无                          
//  Others:         
//-------------------------------------------------------------------------------//
//void  Proc_Auto_Err_Adj_Pre_sec(void)
//{
    INT8U  u8_channel_fg;

//    if(Judge_ERR_ADJ_key() == FALSE)
//    {
//        Initlize_Auto_Err_Adj_Var();
//        return;
//    }

//    if(gs_auto_err_adj_var.u8_flg == FLG_HAD_ERR_ADJ)   return;

//    switch(gs_auto_err_adj_var.u8_sec_cnt)
//    {
//        case 0:
//        case 1:
//        case 2:
//	 case 3:         
//            gs_auto_err_adj_var.u8_sec_cnt++;
//        break;
//        case 4:            // 添加功率读取判断校正点， 1.0IB 则先清比差//
//		ADJ_Err_prepare();
//	       gs_auto_err_adj_var.u8_sec_cnt++;
//	break;
//        case SEC_INIT_ADJ_ERR:   //短接稳定时间到位后进行通道校表参数 初始化//
//		ADJ_Err_Adj();
//		gs_dis_ctl_var.mode = DIS_MODE_ADJ;
//		gs_dis_ctl_var.keep_sec[4] = 5;
//		gs_auto_err_adj_var.u8_flg = FLG_HAD_ERR_ADJ;
//        break;
//        default:
//            Initlize_Auto_Err_Adj_Var();
//        break;

//    }

//}

//-------------------------------------------------------------------------------//
//  Function:      void get_default_mem_data(INT8U index)     
//  Description:  根据数据区域获取默认数据
//  Parameters:   无  
//  Returns:        无                          
//  Others:         
//-------------------------------------------------------------------------------//
//INT8U  Judge_Auto_Err_Adj_Input_Data(INT8U u8_channel)
//{
//	if(u8_channel>2)  return(0) ;

//	if(gs_auto_err_adj_var.u32_urms_tmp[u8_channel] >URMS_MAX)   return(0);
//    	if(gs_auto_err_adj_var.u32_urms_tmp[u8_channel] <URMS_MIN)   return(0);

//    
//	if(gs_auto_err_adj_var.u32_irms_tmp[u8_channel] >IRMS_MAX)   return(0);
//    	if(gs_auto_err_adj_var.u32_irms_tmp[u8_channel] <IRMS_MIN)   return(0);

//	//if(gs_auto_err_adj_var.u32_prms_tmp[u8_channel] >PRMS_MAX)   return(0);
//    	//if(gs_auto_err_adj_var.u32_prms_tmp[u8_channel] <PRMS_MIN)   return(0);  
//    	
//        return (1);
//}


//-------------------------------------------------------------------------------//
//  Function:      void get_default_mem_data(INT8U index)     
//  Description:  根据数据区域获取默认数据
//  Parameters:   无  
//  Returns:        无                          
//  Others:         
//-------------------------------------------------------------------------------//
//void  Initlize_Auto_Err_Adj_Var(void)
//{
//	gs_auto_err_adj_var.u8_flg = 0;
//	gs_auto_err_adj_var.u8_sec_cnt = 0;


//	gs_auto_err_adj_var.u32_urms_tmp[0] = 0;
//	gs_auto_err_adj_var.u32_irms_tmp[0]  =  0;

//}






/*********************************************************************************************************
** 函数名称: Hardware_AdjustDiv_Sub
** 功能描述: 硬件短接 校准步骤判断
** 入口参数: 无
** 出口参数: 无
********************************************************************************************************/
//void Hardware_AdjustDiv_Sub(void)
//{
//    uint8 i;
//    uint32 val;
//   uint32 tmpval;
//   uint32 tmpval1=0;	 
//   uint32 jzwcval;
//   uint8 ajust_type;
//   uint32 cmp_val[4];
//   uint32 adjust_wcval;

//    if(EA==0)  return;

//    //  准备阶段  ，获取功率值，判断进行哪一步的校表//
//   gs_emu_adj_var.adj_flg = 0x55;     // =0X55 标识目前进入校表状态 //

//    jzwcval=cst_adj_const.C_PCONST;     // 额定功率点的功率校准定点数据//
//    cmp_val[0]=jzwcval*25/10;         //250% IB 基本校表点参数采用默认值初始化 //
//    cmp_val[1]=jzwcval*70/100;       // 基本点校准 //
//    cmp_val[2]=jzwcval*35/100;       // 角差校准 //
//    cmp_val[3]=jzwcval*3/100;         // 小信号校准 //

//    CLRWDT(); 	 	 
// // 两路功率读取//
//    adjust_wcval=Get_Original_Power(4,EMU_CH_L);	 //读取实际功率
//   // A 路校表状态获取// 
//    if(adjust_wcval>cmp_val[0])
//    {
//        ajust_type=HARD_EADJ_STEP0_L_INITLIZE;//初始化 ch0;
//    }
//    else if(adjust_wcval>cmp_val[1])
//    {
//        ajust_type=HARD_EADJ_STEP1_L_100IB_100L;//IB 1.0 ch0;
//    }
//    else if(adjust_wcval>cmp_val[2])
//    {
//        ajust_type=HARD_EADJ_STEP2_L_100IB_50L;//IB 0.5 ch0;   
//    }   
//    else if(adjust_wcval>cmp_val[3])
//    {
//        ajust_type=HARD_EADJ_STEP3_L_5IB_100L;//5%IB 1.0 ch0;   
//    }

//    //B 路校表状态获取//
//    ReadRaccoonInt(DATAP,5,BPhy);     // 最大 200ms读取等待    //  理论时间76ms
//    if( gs_sys_run.font_fg & BIT3_FONT_FG_readpower)
//    	{
//    	    gs_sys_run.font_fg &= (~BIT3_FONT_FG_readpower) ;
//	    ReadbackABCPhy();   //功率、电压、电流、功率因素、频率获取//
//    	}
//	else // 读取失败， 重新读取一次// 
//	{
//           ReadRaccoonInt(DATAP,5,BPhy);     
//           gs_sys_run.font_fg &= (~BIT3_FONT_FG_readpower) ;
//           ReadbackABCPhy();   //功率、电压、电流、功率因素、频率获取//
//	}
//		
//        adjust_wcval = gs_measure_var_data.gs_really[1].dw_p_val.u32;	 //读取实际功率
//        // 状态获取//
//        if(adjust_wcval>cmp_val[0])
//        {
//            ajust_type=HARD_EADJ_STEP4_N_INITLIZE;//初始化 1.0 ch0;
//        }
//        else if(adjust_wcval>cmp_val[1])
//        {
//            ajust_type=HARD_EADJ_STEP5_N_100IB_100L;//IB 1.0 ch1;
//        }
//        else if(adjust_wcval>cmp_val[2])
//        {
//            ajust_type=HARD_EADJ_STEP6_N_100IB_50L;//IB 0.5 ch1;     
//        }   
//        else if(adjust_wcval>cmp_val[3])
//        {
//            ajust_type=HARD_EADJ_STEP7_N_5IB_100L;//5%IB 1.0 ch1;   
//        }
//        else
//        {
//            ajust_type=HARD_EADJ_STEP8_METER_CLR;// 电表清零  //   
//        }




//    //   分步骤校准处理   //
//    //  HARD_EADJ_STEP0_L_INITLIZE   L回路比差寄存器初始化//
//    if(ajust_type==HARD_EADJ_STEP0_L_INITLIZE)
//    {                      
//        gs_adj_emu_param.w1gain = 0;   //初始化L1通道比差寄存器  //
//        Save_EMU_AdjParam();
//        Check_EMU_AdjParam();
//        CLRWDT();
//        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH1_0);   // 液晶显示 //
//    }

//    //  HARD_EADJ_STEP1_L_100IB_100L   L回路100%Ib, 1.0L 校准//
//    if(ajust_type==HARD_EADJ_STEP1_L_100IB_100L)
//    {                      
//        //Dis_Adjust(1);  
//        // L回路有功增益校准 // 
//        SoftAdjust_IB_Process(jzwcval,EMU_CH_L); 

//        // L回路电流有效值增益校准 //  	
//        tmpval=0;
//        for(i=0;i<8;i++)
//        {
//            Lib_Delay_ms(30);
//            val = ReadMeterParaACK(RMSII1);
//            if(val>=0x80000000)
//            {
//                val=~val+1;  
//            } 		  		
//            tmpval=tmpval+val;
//            CLRWDT();          
//        }
//        tmpval=tmpval/8;
//        //电流有效值的位数在此处决定，取决于cst_adj_const.I_ref和基本电流整数的倍数//
//        gs_adj_emu_param.i1rms_xs=(float)cst_adj_const.I_ref/(float)tmpval;  
//        //电压有效值增益校准 //             		
//        tmpval=0;
//        for(i=0;i<8;i++)
//        {
//            Lib_Delay_ms(30);
//            val = ReadMeterParaACK(RMSIU);
//            if(val>=0x80000000)
//            {
//                val=~val+1;  
//            } 		  		
//            tmpval=tmpval+val;
//            CLRWDT();        
//        }
//        tmpval=tmpval/8;      
//        //电压有效值的位数在此处决定，取决于cst_adj_const.U_ref和基本电压整数的倍数//
//        gs_adj_emu_param.vrms_xs=(float)cst_adj_const.U_ref/(float)tmpval;  

//        // L回路掉零线电压常量校准//  
//        CLRWDT();            
//        //Dis_Adjust(99);   //准备阶段    
//        SoftAdjust_dlxProcess(jzwcval,EMU_CH_L);
//        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH1_1);   // 液晶显示 //
//    }

//    //  HARD_EADJ_STEP2_L_100IB_50L   L回路100%Ib, 0.5L 校准//
//    if(ajust_type==HARD_EADJ_STEP2_L_100IB_50L)
//    {                            
//        //Dis_Adjust(2);
//        jzwcval=jzwcval/2;
//        SoftAdjust_Phase_Process(jzwcval,EMU_CH_L);  
//        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH1_2);   // 液晶显示 //
//    }
//    //  HARD_EADJ_STEP3_L_5IB_100L   L回路5%Ib, 1.0L 校准//
//    if(ajust_type==HARD_EADJ_STEP3_L_5IB_100L)
//    {                            
//        //Dis_Adjust(3);
//        jzwcval=jzwcval/20;
//        SoftAdjust_IB5_Process(jzwcval,EMU_CH_L);   
//        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH1_3);   // 液晶显示 //   
//    }

//    //  HARD_EADJ_STEP4_N_INITLIZE   N回路比差寄存器初始化//
//    if(ajust_type==HARD_EADJ_STEP4_N_INITLIZE)
//    {                      
//        gs_adj_emu_param.w2gain = 0;   //初始化L1通道比差寄存器  //
//        Save_EMU_AdjParam();
//        Check_EMU_AdjParam();
//        CLRWDT();
//        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH2_0);   // 液晶显示 //
//    }

//    //  HARD_EADJ_STEP4_N_100IB_100L   N回路100%Ib, 1.0L 校准//
//    if(ajust_type==HARD_EADJ_STEP5_N_100IB_100L)
//    {              
//        //Dis_Adjust(1);  
//        // L回路有功增益校准 // 
//        SoftAdjust_IB_Process(jzwcval,EMU_CH_N); 

//        // L回路电流有效值增益校准 //  	
//        tmpval=0;
//        for(i=0;i<8;i++)
//        {
//            Lib_Delay_ms(30);
//            val = ReadMeterParaACK(RMSII2);
//            if(val>=0x80000000)
//            {
//                val=~val+1;  
//            } 		  		
//            tmpval=tmpval+val;
//            CLRWDT();          
//        }
//        tmpval=tmpval/8;
//        //电流有效值的位数在此处决定，取决于cst_adj_const.I_ref和基本电流整数的倍数//
//        gs_adj_emu_param.i2rms_xs=(float)cst_adj_const.I_ref/(float)tmpval;  

//        // N回路掉零线电压常量校准//  
//        CLRWDT();            
//        //Dis_Adjust(99);   //准备阶段    
//        SoftAdjust_dlxProcess(jzwcval,EMU_CH_N);
//        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH2_1);   // 液晶显示 //
//    }

//    //  HARD_EADJ_STEP6_N_100IB_50L   N回路100%Ib, 0.5L 校准//
//    if(ajust_type==HARD_EADJ_STEP6_N_100IB_50L)
//    {                      
//        //Dis_Adjust(5);
//        jzwcval=jzwcval/2;
//        SoftAdjust_Phase_Process(jzwcval,EMU_CH_N);  
//        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH2_2);   // 液晶显示 //
//    }    
//    //  HARD_EADJ_STEP7_N_5IB_100L   N回路5%Ib, 1.0L 校准//
//    if(ajust_type==HARD_EADJ_STEP7_N_5IB_100L)
//    {                         
//        //Dis_Adjust(6);
//        jzwcval=jzwcval/20;
//        SoftAdjust_IB5_Process(jzwcval,EMU_CH_N);   
//        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH2_3);   // 液晶显示 //   
//    }

//    if(ajust_type==HARD_EADJ_STEP8_METER_CLR)
//    {        
//        Proc_clr_meter();
//        api_chg_LCDDisplay_adj_item(DIS_DATA_CLR);   // 液晶显示 //
//    }

//    gs_emu_adj_var.adj_flg =0;   // =0X0 标识目前退出校表状态 //	

//}


/*--------------------------------------------------------------*/
//end of file
/*--------------------------------------------------------------*/

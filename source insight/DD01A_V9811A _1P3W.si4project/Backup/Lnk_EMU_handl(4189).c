/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Lnk_EMU_handl.c
**Author		: maji
**date			: 2016-04-20 
**description	: V9811A的EMU计量模块链路层处理C代码
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>


EMU_RUN_VAR   gs_emu_run_var;

/*********************************************************************************************************
** 函数名称: Init_EMU
** 功能描述: 初始化EMU
** 入口参数: 无
** 出口参数: 无
********************************************************************************************************/


void Init_EMU_RAM(void)
{
    Get_EMU_AdjParam();
}


/*********************************************************************************************************
** 函数名称: Init_EMU
** 功能描述: 初始化EMU
** 入口参数: 无
** 出口参数: 无
********************************************************************************************************/
void Init_EMU(uint8 u8_mode)
{


    if(u8_mode==SLEEP_MODE)
    {
        CtrlADC5=0X80;     //关闭M通道 //
        CtrlADC6=0;         //关闭所有AD采样 //  
        PMG=1;              //关闭计量时钟  //
        return;
    }

    PMG=0;              //打开计量时钟  //
    //step1  //
    SetMeterCfgACK(0x00,PMCtrl4);   //关闭能量累加、CF 输出以及起动/潜动判断电路 //
    SetMeterCfgACK(0x00,PMCtrl1);   // 关闭计量通道，并停止功率/有效值计算  //
    // step2  //
    CtrlADC5=0X80;   
    CtrlADC6=0;   
    if(u8_mode==NORMAL_MODE)
    {
        Set_EmuClock(EMUCLK_3M2);          // 3.2M 计量主频 //
        ADCOPEN(ADCAPDN+ADCBPDN+ADCUPDN);//OPEN ADC U  A  B
        SetMeterCfgACK(0X889374BC,PARABPF);    // 手册要求 //
    }
    else if((u8_mode ==DLX_MODE)||(u8_mode ==CONST_MODE))
    {
        Set_EmuClock(EMUCLK_800k);       // 800K计量主频 //
        ADCOPEN(ADCAPDN+ADCBPDN);//OPEN ADC  A B
        SetMeterCfgACK(0X911D3C9C,PARABPF);    // 手册要求 // 
    }
    else
    {
        sys_err();          // 输入参数异常，死循环退出 //
    }
    CtrlADC0=(ADCA4X+ADCB4X+ADCU2X); //ADC A 4X GAIN  ADC B 4x GAIN ADC U 2sX GAIN	

    // step3  //   
    SetMeterCfgACK(INITGATEP,GATEP);//设置计量初始值
    SetMeterCfgACK(INITGATEQ,GATEQ);

    // step4 // 
    SetMeterCfgACK(0,0x10f0);    //0x10f0  PPCNT  正向 E1 能量累加寄存器  //
    SetMeterCfgACK(0,0x10f1);    //0x10f1  NPCNT  反向 E1 能量累加寄存器 //
    SetMeterCfgACK(0,0x10f2);    //0x10f2  PPCFCNT  正向 E1 能量脉冲计数器   //
    SetMeterCfgACK(0,0x10f3);   //0x10f3  NPCFCNT  反向 E1 能量脉冲计数器   //
    SetMeterCfgACK(0,0x10f6);   //0x10f6  PQCNT  正向 E2 能量累加寄存器//
    SetMeterCfgACK(0,0x10f7);   //0x10f7  NQCNT  反向 E2 能量累加寄存器   //
    SetMeterCfgACK(0,0x10f8);   //0x10f8  PQCFCNT   正向 E2 能量脉冲计数器  //
    SetMeterCfgACK(0,0x10f9);   //0x10f9 NQCFCNT   反向 E2 能量脉冲计数器  //
    // step5// 
    SetMeterCfgACK(0xC8,PMCtrl4);		//  ?????  //
    SetMeterCfgACK(0x08,PMCtrl4);
    SetMeterCfgACK(0x00,PMCtrl4);
    // step6 // 
    SetMeterCfgACK(0,0x10f0);    //0x10f0  PPCNT  正向 E1 能量累加寄存器  //
    SetMeterCfgACK(0,0x10f1);    //0x10f1  NPCNT  反向 E1 能量累加寄存器 //
    SetMeterCfgACK(0,0x10f2);    //0x10f2  PPCFCNT  正向 E1 能量脉冲计数器   //
    SetMeterCfgACK(0,0x10f3);   //0x10f3  NPCFCNT  反向 E1 能量脉冲计数器   //
    SetMeterCfgACK(0,0x10f6);   //0x10f6  PQCNT  正向 E2 能量累加寄存器//
    SetMeterCfgACK(0,0x10f7);   //0x10f7  NQCNT  反向 E2 能量累加寄存器   //
    SetMeterCfgACK(0,0x10f8);   //0x10f8  PQCFCNT   正向 E2 能量脉冲计数器  //
    SetMeterCfgACK(0,0x10f9);   //0x10f9 NQCFCNT   反向 E2 能量脉冲计数器  //
 
    // step7// 
    SetMeterCfgACK(0x01,GATECP);//潜动1   ,=1会禁止脉冲发出    //
    SetMeterCfgACK(0x01,GATECQ);//潜动2   ,=1会禁止脉冲发出  //
//    SetMeterCfgACK(0x00,DATACP);//清常数功率寄存器 //

    // step8// 
    SetMeterCfgACK(0x57,PMCtrl1);//BIT7= 0  测试位确保为0  ; BIT6=1 使能角差校正  //
                                  //BIT5= 0  IA--I1,IB--I2 ; BIT4=1 使能PM数字信号处理  //
                                  //BIT3= 0  屏蔽M通道数字信号输入; BIT2=1 使能I2数字信号输入使能  //
                                  //BIT1= 1   使能I1数字信号输入使能; BIT0=1 使能U数字信号输入使能  //
    SetMeterCfgACK(0x00,PMCtrl2);// 数字增益类的参数寄存器，要求为0  //

    SetMeterCfgACK(0x70,PMCtrl3);//开启两路有功,带通和低通滤波器有效 //
 //   SetMeterCfgACK(0xD2,PMCtrl4);//BIT7= 1 E2 计量通道起动/潜动判断使能  ; BIT6=1  E2 计量通道起动/潜动判断使能  //
                                 //BIT5= 1 E2 能量 CF 脉冲输出开启; BIT4=1 使能E1 能量 CF 脉冲输出 //
                                 //BIT3= 0 禁止能量累加和能量脉冲计数; BIT2=0  E1 能量 CF 脉冲从引脚 CF1 输出，E2 能量 CF 脉冲从引脚 CF2 输出  //
                                 //BIT1:0= 10 常数功率 //
    SetMeterCfgACK(0,0x10f0);    //0x10f0  PPCNT  正向 E1 能量累加寄存器  //
    SetMeterCfgACK(0,0x10f1);    //0x10f1  NPCNT  反向 E1 能量累加寄存器 //
    SetMeterCfgACK(0,0x10f2);    //0x10f2  PPCFCNT  正向 E1 能量脉冲计数器   //
    SetMeterCfgACK(0,0x10f3);   //0x10f3  NPCFCNT  反向 E1 能量脉冲计数器   //
    SetMeterCfgACK(0,0x10f6);   //0x10f6  PQCNT  正向 E2 能量累加寄存器//
    SetMeterCfgACK(0,0x10f7);   //0x10f7  NQCNT  反向 E2 能量累加寄存器   //
    SetMeterCfgACK(0,0x10f8);   //0x10f8  PQCFCNT   正向 E2 能量脉冲计数器  //
    SetMeterCfgACK(0,0x10f9);   //0x10f9 NQCFCNT   反向 E2 能量脉冲计数器  //
    // step9 // 
    SetMeterCfgACK(0xDE,PMCtrl4);//BIT7= 1  E2 计量通道起动/潜动判断使能  ; BIT6=1  E2 计量通道起动/潜动判断使能  //
                                                  //BIT5= 0 E2 能量 CF 脉冲输出关闭; BIT4=1 使能E1 能量 CF 脉冲输出 //
                                                  //BIT3= 1 使能能量累加和能量脉冲计数; BIT2=0  E1 能量 CF 脉冲从引脚 CF1 输出，E2 能量 CF 脉冲从引脚 CF2 输出  //
                                                  //BIT1:0= 10 常数功率 //
     // step10 // 
    SetMeterCfgACK(0,PHCCtrl1);//清0
    SetMeterCfgACK(0,PHCCtrl2);//清0

    SetMeterCfgACK(0x00,CFCtrl);           //以E2正反绝对值累加能量为依据产生CF2  以E1正相能量为依据产生CF	//
 
     // step11// 
    SetMeterCfgACK(0x00,DATACP);//清常数功率寄存器 //
    P9OE&=(~(BIT5));//打开CF2 IO输出//
    P9FC|=(BIT5);
//    P1OE &=(~(BIT3));//打开CF 1 IO输出//
//    P13FS |=(BIT2);
    EIE|=0x02;
    ExInt3IE|=BIT7;     // 开启CF2中断 //  有功中断

    gs_emu_run_var.delay_emu = 3;   // 初始化后的稳定时间为2秒 //

}
/*******************************************************************************************
** 函数名称: Lnk_get_EMU_voltage
** 函数描述: 获取指定通道电压 
** 输入参数: ch: 通道序号0代表合相
** 输出参数: uint32 指定通道电压值，HEX码,量纲为0.01V
** 说明    : 校表系数为0.01V，/10转为0.1V
*******************************************************************************************/
uint32 Lnk_get_EMU_voltage(uint8 ch)
{
    uint32 val,tmpval;
    fp32   f32_tmp;
    ST_U32_U08 TempValue;
    if(ch==WIRE_L1) 
    {    
        val= ReadMeterParaACK(RMSIU);   
        if(val>=0x80000000)
        {
            val=~val+1;
        }       
        f32_tmp = (gs_adj_emu_param.vrms_xs*(fp32)val);
        tmpval = (uint32)f32_tmp;
    }
    else  if(ch==WIRE_L2)  
    {
	if(ReadRaccoon(RegAARTU,1,BPhy)==TRUE)     // 最大 200ms读取等待    //  理论时间15ms  RegAARTU   
	{
           memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
           Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
           val = TempValue.u32;
	}
	else // 读取失败，刷新串口// 
	{
	    gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	}

        if(val>=0x80000000)
        {
            val=~val+1;  
        }
        f32_tmp = gs_adj_emu_param.vrms_xs*(fp32)val;
        tmpval = (uint32)f32_tmp;
    }
    else
    {
        tmpval = 0;
    }


    if( tmpval <50)   tmpval =0;

    return(tmpval);


}
/*******************************************************************************************
** 函数名称: Lnk_get_EMU_current
** 函数描述: 获取指定通道电流
** 输入参数: ch: 通道序号0代表合相
** 输出参数: uint32 指定通道电流值，HEX码,量纲为0.001A
** 说明    : 
*******************************************************************************************/
uint32 Lnk_get_EMU_current(uint8 ch)
{
   uint32 val,tmpval;
   fp32   f32_tmp;
   ST_U32_U08 TempValue;

    if(ch==WIRE_L1) 
    {
        val = ReadMeterParaACK(RMSII2);  
        if(val>=0x80000000)
        {
            val=~val+1;  
        } 
        f32_tmp = gs_adj_emu_param.i2rms_xs*val;
        tmpval = (uint32)f32_tmp;
    }
    else  if(ch==WIRE_L2) 
    {
		val = ReadMeterParaACK(RMSII1);
		if(val>=0x80000000)
		{
			val=~val+1;  
		} 
		f32_tmp = gs_adj_emu_param.i2rms_xs*val;
		tmpval = (uint32)f32_tmp;

    }
    else
    {
        tmpval = 0;
    }

    if( tmpval <20)   tmpval =0;

    return (tmpval);

}
/*******************************************************************************************
** 函数名称: Lnk_get_EMU_activePower
** 函数描述: 获取指定通道电流
** 输入参数: ch: 通道序号
** 输出参数: uint32 指定通道功率值，HEX码,量纲为0.1W
** 说明    : 
*******************************************************************************************/
uint32 Lnk_get_EMU_activePower(uint8 ch)
{
   uint32 val,val2,tmpval;
   fp32   f32_tmp;
   ST_U32_U08 TempValue;

    if(ch==PHASE_TT) 
    {
      val=ReadMeterParaACK(DATAIP);	 
	  if(ReadRaccoon(RegAAP,1,BPhy)==TRUE)     // 最大 200ms读取等待    //  理论时间15ms
	  {
       memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
       Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
       val2 = TempValue.u32;
	  }
	   else // 读取失败，刷新串口// 
	  {
	   gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	  }	
	   
	  tmpval = val+val2;
    }
    if(ch==WIRE_L1) 
    {
       gs_emu_run_var.net_flg &= ~REV_L_ACPOWER;
       val=ReadMeterParaACK(DATAIP);	 
       if(val>=0x80000000)
       {
		val=~val+1;  
		gs_emu_run_var.net_flg |= REV_L_ACPOWER;
       }
				
    }
   else  if(ch==WIRE_L2) 
    {
    gs_emu_run_var.net_flg &= ~REV_N_ACPOWER;
	if(ReadRaccoon(RegAAP,1,BPhy)==TRUE)     // 最大 200ms读取等待    //  理论时间15ms
	{
       memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
       Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
       val = TempValue.u32;
	}
	else // 读取失败，刷新串口// 
	{
	    gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	}

     if(val>=0x80000000)
     {
	  val=~val+1; 
	  gs_emu_run_var.net_flg |= REV_N_ACPOWER;  
      }	
    }
    else
    {
        val = 0;
    }

    if(val<REF_START_Pn)
    {
        tmpval =0;    
        if(ch==WIRE_L1) gs_emu_run_var.net_flg &= ~REV_L_ACPOWER;
        if(ch==WIRE_L2) gs_emu_run_var.net_flg &= ~REV_N_ACPOWER;
    }
    else
    {
        if(ch==WIRE_L1) f32_tmp = cst_adj_const.Kim_Prms*val;
        if(ch==WIRE_L2) f32_tmp = cst_adj_const.Kim_Prms*val;
        tmpval = (uint32)f32_tmp;  	        
    }


   if((gs_emu_run_var.net_flg&REV_L_ACPOWER)||(gs_emu_run_var.net_flg&REV_N_ACPOWER))
    {
      gs_measure_status_judge_var.u8_status |= REV_ACPOWER_L ;
     }
   else 
    {
      gs_measure_status_judge_var.u8_status &=~ REV_ACPOWER_L ;
    }
   

    return (tmpval);
}
/*******************************************************************************************
** 函数名称: Lnk_get_EMU_freq
** 函数描述: 获取指定通道电网频率
** 输入参数: ch: 通道序号0代表合相
** 输出参数: uint32 指定通道频率值，HEX码,量纲为0.01Hz
** 说明    : 
*******************************************************************************************/
uint32 Lnk_get_EMU_freq(uint8 ch)
{
	uint32 val,tmpval;
	ST_U32_U08 TempValue;
    if(ch==WIRE_L1) 
    {    
        val= ReadMeterParaACK(DATAFREQ);          
        tmpval= (819200 *100)/val;			// 819200为ADC 频率  //
    }
   else  if(ch==WIRE_L2) 
	{
	if(ReadRaccoon(RegFREQ,1,BPhy)==TRUE)     // 频率瞬时值
	{
		memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
		Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
		val = TempValue.u32;
		tmpval= (819200 *100*4)/val;			// 819200为ADC 频率  //
	}
	else // 读取失败，刷新串口// 
	{
	    gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	}
    }
    else
    {
        tmpval = 0;
    }
#if (C_Fq == 60)
		tmpval = tmpval*1.2;
#else
		tmpval = tmpval; 
#endif           
    return(tmpval);
}

/*******************************************************************************************
** 函数名称: Lnk_CAL_EMU_constSum
** 函数描述: 常量计量每秒更新数据
** 输入参数: 
** 输出参数: 
** 说明    : 
*******************************************************************************************/
void lnk_CAL_EMU_constSum_per_second(void)
{
	ST_U32_U08 sumval0;
	ST_U32_U08 val1;
	ST_U32_U08 val2;
	ST_U32_U08 TempValue;

	//gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);

	//读取有无功计量模式字
	//  mem_read(&gs_sys_globaL_var.fac_energy[0], ADR_METER_PARAM1_FAC_ENERGY, 2, MEM_E2P1);   
	//  mem_read(&gs_sys_globaL_var.Rac_energy[0], ADR_METER_PARAM1_REC_ENERGY, 2, MEM_E2P1);    
	//9260F无功功率写入//
	//Lnk_get_EMU_ractive_const();

	//有功功率写入//
    val1.u32 = 0;

	//功率获取 //
	val1.u32+=ReadMeterParaACK(DATAIP);
	val1.u32+=ReadMeterParaACK(DATAIQ);	 
	if(val1.u32>=0x80000000)
	{
		gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);
		val1.u32=~val1.u32+1;  
	//	gs_emu_run_var.net_flg |= REV_TT_ACPOWER;// 合相功率反向
	}
	else
		gs_emu_run_var.net_flg |= REV_TT_ACPOWER;// 合相功率反向
//	sumval0.u32=REF_START_Pn;
	sumval0.u32=val1.u32;
	if(val1.u32<REF_START_Pn) 
	{
		sumval0.u32=0;
		gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);//屏蔽反向指示
	}
	SetMeterCfgACK(sumval0.u32,DATACP); 
	gs_measure_var_data.gs_really[PHASE_TT].dw_p_val.u32 = cst_adj_const.Kim_Prms*val1.u32;
   // val1.u32 =ReadMeterParaACK(DATAQ);	 

   /* //B路功率获取9260F  //
    if(ReadRaccoon(RegAAP,1,BPhy)==TRUE)     // 最大 200ms读取等待    //  理论时间76ms  RegIAP
	{
      memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
      Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//'
      val2.u32= TempValue.u32;
	}
 	else 
	{
      gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
      return;//数据读取失败  不更新常数寄存器
	}
*/
//#ifdef _DEBUGE_FUNCTION
//     val1.u32 = 0x06668888;
//     val2.u32 = 0xFD997778;
//#endif
	
    // 有功大于启动功率
	//if((val1.u32 <= REF_START_Pn)||(val1.u32 >= REF_START_REV_Pn))
	//{
	//  val1.u32 =0; 
	//}
	//SetMeterCfgACK( val1.u32,DATACP);
	/*
	if((val2.u32 <= REF_START_Pn)||(val2.u32 >= REF_START_REV_Pn))
	{
	  val2.u32 =0; 
	}
    if(gs_sys_globaL_var.fac_energy[0]==0x55) //0X55 代数和累加
    {

	  
      if(val1.u32>=0x80000000)
      {
		gs_emu_run_var.net_flg |= REV_L_ACPOWER;//A路反向
      }
	  else
	  {
		gs_emu_run_var.net_flg &= (~REV_L_ACPOWER);//A路正向
	  }	 
      if(val2.u32>=0x80000000)
      {
		gs_emu_run_var.net_flg |= REV_N_ACPOWER;  //B路反向
      }
	  else
	  {
		gs_emu_run_var.net_flg &= (~REV_N_ACPOWER);//B路正向
	  }

	  // A  B反向
	  if((gs_emu_run_var.net_flg&REV_N_ACPOWER)&&(gs_emu_run_var.net_flg&REV_L_ACPOWER))
	  {
	     val1.u32=~val1.u32+1;
	     val2.u32=~val2.u32+1; 
		 sumval0.u32 = val1.u32 + val2.u32 ; // 带方向累加
	 	 sumval0.u32=~sumval0.u32+1;   //反向
	  }
	   //B反  A正
	  else if((gs_emu_run_var.net_flg&REV_N_ACPOWER)&&(!(gs_emu_run_var.net_flg&REV_L_ACPOWER)))
	  {
	     val2.u32=~val2.u32+1; 
		 if(val2.u32>=val1.u32) //合相反向
		 {
             sumval0.u32 = val2.u32 - val1.u32 ;
	  		 sumval0.u32=~sumval0.u32+1;  
		 }
		 else //合相正向
		 {
             sumval0.u32 = val1.u32 - val2.u32 ;
		 }
	  }
	   //B正  A反
	  else if((gs_emu_run_var.net_flg&REV_L_ACPOWER)&&(!(gs_emu_run_var.net_flg&REV_N_ACPOWER)))
	  {
	     val1.u32=~val1.u32+1; 
		 if(val2.u32>=val1.u32) //合相正向
		 {
             sumval0.u32 = val2.u32 - val1.u32 ;
		 }
		 else //合相反向
		 {
             sumval0.u32 = val1.u32 - val2.u32 ;
			 sumval0.u32=~sumval0.u32+1;  

		 }
	  }
	   //A正 B正 
	  else if(!(gs_emu_run_var.net_flg&REV_L_ACPOWER)&&(!(gs_emu_run_var.net_flg&REV_N_ACPOWER)))
	  {
        sumval0.u32 = val1.u32 + val2.u32 ; // 带方向累加 
	  }

	  //合相功率方向判断	
	  if(sumval0.u32>0x80000000) 
	  {
		gs_emu_run_var.net_flg |= REV_TT_ACPOWER;// 合相功率反向
		sumval0.u32=~sumval0.u32+1;  
	  }
	  else
	  {
		gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);
	  }

      //瞬时有功功率放入常量配置寄存器 //
      SetMeterCfgACK( sumval0.u32,DATACP);

	 	// 有功有效值
//    gs_measure_var_data.gs_really[PHASE_TT].dw_p_val.u32 = cst_adj_const.Kim_Prms*sumval0.u32;
//    gs_measure_var_data.gs_really[WIRE_L1].dw_p_val.u32 = cst_adj_const.Kim_Prms*val1.u32;
//    gs_measure_var_data.gs_really[WIRE_L2].dw_p_val.u32 = cst_adj_const.Kim_Prms*val2.u32;
//    gs_measure_var_data.gs_really[WIRE_L3].dw_p_val.u32 = 0;
	}
	else  // 绝对值累加
	{

      if(val1.u32>=0x80000000)
      {
		gs_emu_run_var.net_flg |= REV_L_ACPOWER;//A路反向
      }
	  else
	  {
		gs_emu_run_var.net_flg &= (~REV_L_ACPOWER);//A路正向
	  }
      
      if(val2.u32>=0x80000000)
      {
		gs_emu_run_var.net_flg |= REV_N_ACPOWER;  //B路反向
      }
	  else
	  {
		gs_emu_run_var.net_flg &= (~REV_N_ACPOWER);//B路正向
	  }

	  // A  B反向
	  if((gs_emu_run_var.net_flg&REV_N_ACPOWER)&&(gs_emu_run_var.net_flg&REV_L_ACPOWER))
	  {
	     val1.u32=~val1.u32+1;
	     val2.u32=~val2.u32+1; 
		 sumval0.u32 = val1.u32 + val2.u32 ; // 带方向累加
	 	 sumval0.u32=~sumval0.u32+1;   //合相反向
	  }
	   //B反  A正 
	  else if((gs_emu_run_var.net_flg&REV_N_ACPOWER)&&(!(gs_emu_run_var.net_flg&REV_L_ACPOWER)))
	  {
	     val2.u32=~val2.u32+1; 
		 sumval0.u32 = val1.u32 + val2.u32 ; // 带方向累加
		 if(val2.u32>val1.u32) //合相反向
		 {
	 	   sumval0.u32=~sumval0.u32+1;   //合相反向
		 }
	  }
	   //B正  A反
	  else if((gs_emu_run_var.net_flg&REV_L_ACPOWER)&&(!(gs_emu_run_var.net_flg&REV_N_ACPOWER)))
	  {
	     val1.u32=~val1.u32+1; 
		 sumval0.u32 = val1.u32 + val2.u32 ; // 带方向累加
		 if(val2.u32>=val1.u32) //合相正向
		 {
		  ;  //合相正向
		 }
		 else //合相反向
		 {
			 sumval0.u32=~sumval0.u32+1;  
		 }
	  }
	   //A正 B正 
	  else if(!(gs_emu_run_var.net_flg&REV_L_ACPOWER)&&(!(gs_emu_run_var.net_flg&REV_N_ACPOWER)))
	  {
        sumval0.u32 = val1.u32 + val2.u32 ; // 带方向累加 
	  }
	  
	  //合相功率方向判断	
	  if(sumval0.u32>0x80000000) 
	  {
		gs_emu_run_var.net_flg |= REV_TT_ACPOWER;// 合相功率反向
		sumval0.u32=~sumval0.u32+1;  

	  }
	  else
	  {
		gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);
	  }	 
      //瞬时有功功率放入常量配置寄存器 //
      SetMeterCfgACK( sumval0.u32,DATACP);

	}
	*/
    //gs_measure_var_data.gs_really[PHASE_TT].dw_p_val.u32 = cst_adj_const.Kim_Prms*val1.u32;
   // gs_measure_var_data.gs_really[WIRE_L1].dw_p_val.u32 = cst_adj_const.Kim_Prms*val1.u32;
  //  gs_measure_var_data.gs_really[WIRE_L2].dw_p_val.u32 = cst_adj_const.Kim_Prms*val2.u32;
    //gs_measure_var_data.gs_really[WIRE_L3].dw_p_val.u32 = 0;
	

#ifdef _DEBUGE_FUNCTION
    sumval0.u32 = 0x6668888;
    SetMeterCfgACK( sumval0.u32,DATACP);
    SetMeterCfgACK( sumval0.u32,PARAQC);
#endif

}


/*******************************************************************************************
** 函数名称: 
** 函数描述: 获取无功功率
** 输入参数: 
** 输出参数: 
** 说明    : 
*******************************************************************************************/
void Lnk_get_EMU_ractive_const(void)
{
   ST_U32_U08 Temp_paraqc;
   ST_U32_U08 val1,sumval1;
   ST_U32_U08 val2,sumval2;
   ST_U32_U08 TempValue;
   float err_ec;

   val1.u32 = 0;
   Temp_paraqc.u32 = 0;
   val2.u32 = 0;
      //读取A路无功
	  val1.u32=ReadMeterParaACK(DATAIQ);	    
	  // 读取二次补偿寄存器
	  Temp_paraqc.u32=ReadMeterParaACK(PARAQC);
	  
      //B路功率以及电压电流获取获取9260F  //
      if(ReadRaccoon(RegAAQ,1,BPhy)==TRUE)     // 最大 200ms读取等待    //  理论时间76ms  RegIAP
      {
        memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
        Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//'
        val2.u32= TempValue.u32*1.669f; //0.01%精度
      }
 	  else 
 	  {
	    gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
		return;
	  }

      if((sumval1.u32 <= REF_START_Pn)||(sumval1.u32 >= REF_START_REV_Pn))// 无功启动
      {
       sumval1.u32 =0; 
      }

      if((sumval2.u32 <= REF_START_Pn)||(sumval2.u32 >= REF_START_REV_Pn))// 无功启动
      {
       sumval2.u32 =0; 
      }


      //提取合相以及分相方向//
      if(val1.u32>=0x80000000)
      {
       gs_emu_run_var.net_flg |= REV_TT_RACPOWER; // 合相反向
	  }
	  else
	  {
       gs_emu_run_var.net_flg &= (~REV_TT_RACPOWER); // 合相反向
	  }

      if(val2.u32>=0x80000000)
      {
		gs_emu_run_var.net_flg |= REV_N_RACPOWER; // B路反向 //
	  }
	  else
	  {
		gs_emu_run_var.net_flg &= (~REV_N_RACPOWER); // B路正向//
	  }

	  
	  //根据合相以及B路大小，判断A路方向//
//	  if(Temp_paraqc.u32==0) // 二次补偿寄存器
//	  {
//        if((gs_emu_run_var.net_flg&REV_TT_RACPOWER)!=REV_TT_RACPOWER) // A路正向
//        {
//	       if(gs_emu_run_var.net_flg&REV_N_RACPOWER) //无功 B路反向直接丢入 
//	       {
//	         if(gs_sys_globaL_var.Rac_energy[0]==0x55) //代数和
//	         {
// 			  val2.u32=~val2.u32+1;  // 取反丢入
//	         }
//			 else
//			 {
//			  val2.u32 = val2.u32; //直接丢入
//			 }
//		   }
//		   else 
//		   {
//	         if(gs_sys_globaL_var.Rac_energy[0]==0x55) //代数和
//	         {
// 			  val2.u32=~val2.u32+1;  // 取反丢入
//	         }
//			 else
//			 {
// 			  val2.u32=~val2.u32+1;  // 取反丢入
//			 }
//		   }
//        }
//		else  //A路反
//		{
//	       if(gs_emu_run_var.net_flg&REV_N_RACPOWER) //无功 B路反向直接丢入 
//	       {
//	         if(gs_sys_globaL_var.Rac_energy[0]==0x55) //代数和
//	         {
// 			  val2.u32=~val2.u32+1;  // 取反丢入
//	         }
//			 else
//			 {
// 			  val2.u32=~val2.u32+1;  // 取反丢入
//			 }
//		   }
//		   else 
//		   {
//	         if(gs_sys_globaL_var.Rac_energy[0]==0x55) //代数和
//	         {
// 			  val2.u32=~val2.u32+1;  // 取反丢入
//	         }
//			 else
//			 {
// 			  val2.u32=val2.u32;  // 直接丢入
//			 }
//		   }
//        }
//	     // 9260F 无功写入二次补偿寄存器 2^31/(2^31*gs_adj_emu_param.w1gain*sumval2.u32*(2.175/1.2/1.086))//
//	     err_ec = (float)((float)(0x80000000/(float)(0x80000000+gs_adj_emu_param.w1gain)));
//		 val2.u32 =  val2.u32*err_ec ;
//		 SetMeterCfgACK( val2.u32,PARAQC);
//	     gs_measure_var_data.gs_really[PHASE_TT].dw_q_val.u32 = cst_adj_const.Kim_Prms2*sumval0.u32;
//	     gs_measure_var_data.gs_really[WIRE_L1].dw_q_val.u32 = cst_adj_const.Kim_Prms2*val1.u32;
//	     gs_measure_var_data.gs_really[WIRE_L2].dw_q_val.u32 = cst_adj_const.Kim_Prms2*val2.u32;
//	  }
//	  else
//	  {
        if((gs_emu_run_var.net_flg&REV_TT_RACPOWER)!=REV_TT_RACPOWER) // A路正向
        {
	       if(gs_emu_run_var.net_flg&REV_N_RACPOWER) //无功 B路反向直接丢入 
	       {
 			  val2.u32=~val2.u32+1;  //
 			 //{
			   //if(val1.u32 >= val2.u32)
//			   gs_emu_run_var.net_flg|REV_L_RACPOWER ;  //A符号
			   TempValue.u32 = val2.u32+ val1.u32;     //A路功率值
			   //全部取反丢入
			 //}
		   
		   }
		   else //无功B路正
		   {
			   if(val1.u32 >= val2.u32)
			   {
			     TempValue.u32 = val1.u32 - val2.u32 ;        //A路功率值
			     gs_emu_run_var.net_flg&=(~REV_L_RACPOWER);   //A符号
	 			 val2.u32=~val2.u32+1;  // 取反丢入
			   }
			   else
			   {
//			     gs_emu_run_var.net_flg|REV_L_RACPOWER;   //A符号
			     TempValue.u32 = val2.u32 - val1.u32 ;   //A路功率值
				 if(gs_sys_globaL_var.Rac_energy[0]==0x55) //代数和
				 {
				   val2.u32=~val2.u32+1;  // 取反丢入
				 }
			   }
		   }
        }
		else  //A路反
		{
	       if(gs_emu_run_var.net_flg&REV_N_RACPOWER) //无功 B路反向直接丢入 
	       {
			   if(val1.u32 >= val2.u32)
			   {
			     TempValue.u32 = val1.u32 - val2.u32 ;        //A路功率值
			     gs_emu_run_var.net_flg|=REV_L_RACPOWER;   //A符号
				 if(gs_sys_globaL_var.Rac_energy[0]!=0x55)   //代数和
				 {
  				   val2.u32= val2.u32;    //直接丢入
				 }
				 else
				 {
				   val2.u32=~val2.u32+1;  // 取反丢入
				 }
				}
			   else
			   {
			     gs_emu_run_var.net_flg&=(~REV_L_RACPOWER);   //A符号
			     TempValue.u32 = val2.u32 - val1.u32 ;       //A路功率值
				 val2.u32=~val2.u32+1;  // 取反丢入
			   }
		   }
		   else //无功B路正
		   {
			     TempValue.u32 = val1.u32 + val2.u32 ;        //A路功率值
			     gs_emu_run_var.net_flg|=REV_L_RACPOWER;   //A符号
				 if(gs_sys_globaL_var.Rac_energy[0]!=0x55)   //代数和
				 {
  				   val2.u32= val2.u32;    //直接丢入
				 }
				 else
				 {
				   val2.u32=~val2.u32+1;  // 取反丢入
				 }
		   }
        }
		
	     // 9260F 无功写入二次补偿寄存器 2^31/(2^31*gs_adj_emu_param.w1gain*sumval2.u32*(2.175/1.2/1.086))//
	     err_ec = (float)((float)(0x80000000/(float)(0x80000000+gs_adj_emu_param.w1gain)));

		 if(val2.u32>0x80000000)
		 {
	      val2.u32=~val2.u32+1;  // 取反丢入
		  val2.u32 =  val2.u32*err_ec ;
	      val2.u32=~val2.u32+1;  // 取反丢入
		 }
		 else  val2.u32 =  val2.u32*err_ec ;

		 SetMeterCfgACK(val2.u32,PARAQC);
//	  }

	    //有效值获取//
		if(val2.u32>0x80000000)
		{
	      val2.u32=~val2.u32+1;  // 取反丢入
		}
		
	    if(val1.u32>0x80000000)
		{
	      val1.u32=~val1.u32+1;  // 取反丢入
		}	
	    if(TempValue.u32>0x80000000)
		{
	      TempValue.u32=~TempValue.u32+1;  // 取反丢入
		}		
	     gs_measure_var_data.gs_really[PHASE_TT].dw_q_val.u32 = cst_adj_const.Kim_Prms2*val1.u32;
	     gs_measure_var_data.gs_really[WIRE_L1].dw_q_val.u32 = cst_adj_const.Kim_Prms2*TempValue.u32;
	     gs_measure_var_data.gs_really[WIRE_L2].dw_q_val.u32 = cst_adj_const.Kim_Prms2*val2.u32;
	     gs_measure_var_data.gs_really[WIRE_L3].dw_q_val.u32 = 0;

}


/*******************************************************************************************
** 函数名称: Lnk_get_EMU_activePower
** 函数描述: 获取无功功率
** 输入参数: ch: 通道序号
** 输出参数: uint32 指定通道功率值，HEX码,量纲为0.1W
** 说明    : 
*******************************************************************************************/
uint32 Lnk_get_EMU_ractivePower(uint8 ch)
{
   uint32 val,tmpval;
   fp32   f32_tmp;
   ST_U32_U08 TempValue;

    if(ch==WIRE_L1) 
    {
       gs_emu_run_var.net_flg &= ~REV_L_RACPOWER;
	   val=ReadMeterParaACK(DATAIQ);	 
       if(val>=0x80000000)
       {
		val=~val+1; 
		gs_emu_run_var.net_flg |= REV_L_RACPOWER;
       }
    }
	
    else  if(ch==WIRE_L2) 
    {
	if(ReadRaccoon(RegAAQ,1,BPhy)==TRUE)     // 最大 200ms读取等待    //  理论时间15ms
	{
       memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
       Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
       val = TempValue.u32;
	}
	else // 读取失败，刷新串口// 
	{
	    gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	}
      gs_emu_run_var.net_flg &= ~REV_N_RACPOWER;

       if(val>=0x80000000)
       {
		val=~val+1; 
		gs_emu_run_var.net_flg |= REV_N_RACPOWER;
       }

	   val = val*1.669;//2.175f/1.2f*1.086f;
    }
    else
    {
        val = 0;
    }

    if(val<REF_START_Pn)
    {
        tmpval =0;    
        if(ch==WIRE_L1) gs_emu_run_var.net_flg &= ~REV_L_RACPOWER;
        if(ch==WIRE_L2) gs_emu_run_var.net_flg &= ~REV_N_RACPOWER;
    }
    else
    {
        
        if(ch==WIRE_L1) f32_tmp = cst_adj_const.Kim_Prms2*val;
        if(ch==WIRE_L2) f32_tmp = cst_adj_const.Kim_Prms2*val;
        tmpval = (uint32)f32_tmp;  	        
    }
	
    return (tmpval);
}

/*******************************************************************************************
**    END
*******************************************************************************************/


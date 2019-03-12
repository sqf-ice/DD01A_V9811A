/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Lnk_EMU_Adj.c
**Author		: maji
**date			: 2016-04-20 
**description	: G80F92XD的EMU计量模块链路层校表处理C代码
**note			: MCU- G80F92XD ，MERTER FOR DL06A
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-20   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>



ST_EMU_ADJ_PARAM  gs_adj_emu_param;
ST_EMU_ADJ_VAR      gs_emu_adj_var;


/*********************************************************************************************************
**  校表相关参数初始化默认数据
********************************************************************************************************/
const  ST_EMU_ADJ_PARAM   code   default_adj_emu_param_tab = 
{
    0,  //uint32 w1gain;         //通道1功率增益寄存器(有功无功使用同一组)  //0xFF,0x3B,0x66,0x28,
    0,  //uint32 p1cal;           //通道1电压电流相位补偿寄存器  //
    0,  //uint32 watt1os;      //通道1有功功率偏置  //
    0,  //uint32 irms1os;      //通道1电流有效值偏置  //
    0,  //uint32 w2gain;         //通道2功率增益寄存器(有功无功使用同一组)  //0xFF3B6628
    0,  //uint32 p2cal;           //通道2电压电流相位补偿寄存器  //
    0,  //uint32 watt2os;      //通道2有功功率偏置  //
    0,  //uint32 irms2os;      //通道2电流有效值偏置  //
	0,  //uint32 urmsos       //电压有效值偏置  //
    0.075138056,  //uint32 vconst_val_ch1;   //通道1掉零线电压固定值 //    
    0.112707084,  //uint32 vconst_val_ch2;   //通道2掉零线电压固定值 //
    1.07355e-005,  //float vrms_xs;          //人为计算的电压有效值系数  //9.19946e-006
    //2019-01-17修改电流有效值系数
	0.000163094,//0.000153309,  //float i1rms_xs;         //人为计算的通道1电流有效值系数  //
    0.000163094,//0.000153309,  //float i2rms_xs;         //人为计算的通道2电流有效值系数  //8.279879e-005   1.14993e-007
    0,  //uint16     csck;  //校验和 //
};

const  ST_Adjust_CONST  code   cst_adj_const = 
{
    C_Un*10,        //uint32 U_ref;       //参比电压值，量纲0.1V  // 
    C_Ib*1000,     //uint32 I_ref;       //参比电流值，量纲0.001A  //     
    INITGATEP,     //uint32 GATEP;       // 计量门限值 //
    INITGATECP,   //uint32 C_GATECP;     // 潜动门限值 //
    REF_RMSPn,    // uint32 C_PCONST;    // 额定功率的校准后标准值//
    K_RMSP,         //float   Kim_Prms;  //有功功率有效值系数//  
    K_RMSP2,   		// 无功功率有效值系数
};


const  EMU_CK_TAB  code   cst_emu_selfchck_tab[] = 
{
    &gs_adj_emu_param.w1gain,            SCP,    //通道1功率增益寄存器(有功无功使用同一组)  //
    &gs_adj_emu_param.w2gain,            SCQ,    //通道2功率增益寄存器(有功无功使用同一组)  //
    &gs_adj_emu_param.p1cal,            PHCCtrl1,    //通道1电压电流相位补偿寄存器  //
    &gs_adj_emu_param.p2cal,            PHCCtrl2,    //通道2电压电流相位补偿寄存器  //
    &gs_adj_emu_param.watt1os,            PARAPC,    //通道1有功功率偏置  // 
    &gs_adj_emu_param.watt2os,            PARAQC,    //通道2有功功率偏置  //
	&gs_adj_emu_param.irms1os,			  SCI1,	 //通道1电流有效值偏置  // 
	&gs_adj_emu_param.irms2os,			  SCI2,	 //通道1电流有效值偏置    // 
	&gs_adj_emu_param.urmsos,			  SCU,	 //电压有效值偏置  //
};


const  EMU_CONST_CK_TAB  code   cst_emu_constchck_tab[] = 
{
    INITGATECP,           GATECP,    //uint32 C_GATECP;   // 潜动门限值 //
    INITGATECQ,           GATECQ,    //uint32 C_GATECP;   // 潜动门限值 //
};


//9260F 固定表参数//
const  EMU_CONST_CK_TAB_9260F code   cst_emu_constchck_tab_9260f[] = 
{
    0x1D0004C3,          RegANCtrl0,    //0x185  BIT0-BIT1 I增益 00 x32 01 x16 10 x8  11 x1
	0x30000000,          RegANCtrl1,    //0x185 增益I:32，U:1，M:4 温度系数，全局电流偏置
    0x00000000,          RegANCtrl2,     //0x187  
    0x811D2BA7,          RegBPFPARA,    //0x0125 带通滤波器系数 0x811D2BA7
    0x00000000,          RegMTPARA0,    //0x183 CF工作时钟3.2MHz 使能CF输出，能量累加，起动潜动，能量来源：平均全波有功
    0,                   RegWARTU,     // U32  电压有效值比差校正值 //
    0,                   RegWARTI,     // U32  I1 电流有效值比差校正值//
};

/*********************************************************************************************************
** 函数名称: Save_EMU_AdjParam
** 功能描述: EMU相关参数修改后的保存更新处理
** 入口参数: 无
** 出口参数: 无
********************************************************************************************************/
void Save_EMU_AdjParam(void)
{
    gs_adj_emu_param.csck =Lib_get_csck_int16u_num((uint8 *)&gs_adj_emu_param.w1gain,(LEN_BLOCK70_EMU_PARAM_E2P-2),CHECKWORD);
    mem_db_write(ADR_BLOCK70_EMU_PARAM_E2P, (uint8 *)&gs_adj_emu_param.w1gain,(LEN_BLOCK70_EMU_PARAM_E2P-2),MEM_E2P1); 
}



/*********************************************************************************************************
** 函数名称: Save_EMU_AdjParam
** 功能描述: EMU相关参数修改后的保存更新处理
** 入口参数: 无
** 出口参数: 无
********************************************************************************************************/
void Get_EMU_AdjParam(void)
{
   uint16  csck_u16;

    //RAM 数据检查  //
    csck_u16 =Lib_get_csck_int16u_num((uint8 *)&gs_adj_emu_param.w1gain,(LEN_BLOCK70_EMU_PARAM_E2P-2),CHECKWORD);
    if(csck_u16 != gs_adj_emu_param.csck)
    {
        mem_read((uint8 *)&gs_adj_emu_param.w1gain, ADR_BLOCK70_EMU_PARAM_E2P, LEN_BLOCK70_EMU_PARAM_E2P, MEM_E2P1);
    }
}


/*********************************************************************************************************
** 函数名称: Check_EMU_AdjParam
** 功能描述: EMU寄存器校验
** 入口参数: 无
** 出口参数: 无
********************************************************************************************************/
void Check_EMU_AdjParam(void)
{
   ST_U32_U08 regval;
   uint8 i;
   EMU_CK_TAB tmp_tab;
   EMU_CONST_CK_TAB  tmp_const_tab;
   uint32 ucSum=0;
   uint8  u8_tmp;

    // 检查RAM内的校表参数数据，如果校验不正确则从EEPROM内获取 //
    Get_EMU_AdjParam();   

	///读出常数倍频标志，以便给出正确的门限值 yang 2017-12-28///
	mem_read(&u8_tmp, ADR_METER_PARAM1_CONST_FAST_FLG, 1, MEM_E2P1); 	
	if(u8_tmp==0x3C)
	{
			  //倍频10倍  //
		SetMeterCfgACK(INITGATEP_10M,GATEP);		//设置I1通道功率门限制
		SetMeterCfgACK(INITGATEP_10M,GATEQ);		//设置I2通道功率门限制 				
	}
	else
	{
		SetMeterCfgACK(INITGATEP,GATEP);		//设置I1通道功率门限制
		SetMeterCfgACK(INITGATEP,GATEQ);		//设置I2通道功率门限制 
	}


    // 固定值配置的校表相关数据检查 //    
    for(i=0;i<(dim(cst_emu_constchck_tab));i++)
    {
        tmp_const_tab.emuAdr = cst_emu_constchck_tab[i].emuAdr;
        tmp_const_tab.val = cst_emu_constchck_tab[i].val;
        regval.u32 = 0;
        regval.u32 = ReadMeterParaACK(tmp_const_tab.emuAdr);     //计量门限值检查  //
        if(regval.u32 != tmp_const_tab.val)
        {
            regval.u32 = tmp_const_tab.val;
            SetMeterCfgACK(regval.u32,tmp_const_tab.emuAdr);
        }  
    }
    // 读取存储芯片内的校表数据进行检查 //
    for(i=0;i<(dim(cst_emu_selfchck_tab));i++)
    {
        tmp_tab.emuAdr = cst_emu_selfchck_tab[i].emuAdr;
        tmp_tab.ramAdr = cst_emu_selfchck_tab[i].ramAdr;
        regval.u32 = 0;
        regval.u32= ReadMeterParaACK(tmp_tab.emuAdr);
        if((tmp_tab.emuAdr==PHCCtrl1)||(tmp_tab.emuAdr==PHCCtrl2))
        {
            if(regval.B08[3] != (uint8)(*tmp_tab.ramAdr))
            {
                regval.u32 = (uint8)(*tmp_tab.ramAdr);
                SetMeterCfgACK(regval.u32,tmp_tab.emuAdr);
            }      
        }
        else
        {
            if(regval.u32 != *tmp_tab.ramAdr)
            {
                regval.u32 = *tmp_tab.ramAdr;
                SetMeterCfgACK(regval.u32,tmp_tab.emuAdr);
            }
        }    
     }

}

/*********************************************************************************************************
** 函数名称: Get_Original_Power
** 功能描述: 获取原始功率值
** 入口参数: avr_times-平均次数 
** 出口参数: 无		
********************************************************************************************************/
uint32 Get_Original_Power(uint8 avr_times,uint8 ch)
{
    uint32 val;
    uint8  i;
    uint32 aery_old;
    uint32 aery_val;
    uint32 avr_aery;
    ST_U32_U08 TempValue;
    CLRWDT();

    avr_aery=0;
    gs_emu_adj_var.sec_flg=FALSE; 
    for (i=0;i<avr_times;i++)
    {
        CLRWDT();
        while(gs_emu_adj_var.sec_flg==FALSE);
        gs_emu_adj_var.sec_flg=FALSE; 
        CLRWDT(); 
        if(ch==EMU_CH_L)
        {
            val=ReadMeterParaACK(DATAIQ);	  //DATAP 2017-1-12 ZWK
        }
        else
        {
           // val=ReadMeterParaACK(DATAIQ);  // RegIAP 全波功率瞬时值
	   if(ReadRaccoon(RegIAP,1,BPhy)==TRUE)   //RegAAP 2017-1-12 ZWK   // 最大 200ms读取等待    //  理论时间76ms
	   {
		memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
		Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
		val = TempValue.u32;
	   }
	   else // 读取失败， 
	   {
		gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	   }
        }

       if(val>=0x80000000)
       {
          val=~val+1;  
       }

       aery_old=val;
       avr_aery=avr_aery+aery_old;
    }   

    aery_val=avr_aery/avr_times;
    CLRWDT();
    NOP();
    return(aery_val);
}	 	



/*********************************************************************************************************
** 函数名称: Get_Original_Irms
** 功能描述: 获取原始电流有效值数据
** 入口参数: avr_times-平均次数 
** 出口参数: 无		
********************************************************************************************************/
uint32 Get_Original_Irms(uint8 avr_times,uint8 ch)
{
    uint32 val;
    uint8  i;
    uint32 aery_old;
    uint32 aery_val;
    uint32 avr_aery;
    ST_U32_U08 TempValue;
    CLRWDT();

    avr_aery=0;
    gs_emu_adj_var.sec_flg=FALSE; 
    for (i=0;i<avr_times;i++)
    {
        CLRWDT();
        while(gs_emu_adj_var.sec_flg==FALSE);
        gs_emu_adj_var.sec_flg=FALSE; 
        CLRWDT(); 
        if(ch==EMU_CH_L)
        {
            val=ReadMeterParaACK(RMSII1);	 
        }
        else
        {
           // val=ReadMeterParaACK(DATAIQ); //RegIAP 全波电流瞬时值
	   if(ReadRaccoon(RegARTI,1,BPhy)==TRUE)     // 最大 200ms读取等待    //  理论时间15ms
	   {
		memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
		Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
	       val = TempValue.u32;
	   }
	   else // 读取失败， 重新读取一次// 
	   {
		gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	   }
        }

       if(val>=0x80000000)
       {
          val=~val+1;  
       }

       aery_old=val;
       avr_aery=avr_aery+aery_old;
    }   

    aery_val=avr_aery/avr_times;
    CLRWDT();
    NOP();
    return(aery_val);
}	 


/*=========================================================================================\n
* @function_name: CalculateSC
* @function_file: EnergyBottom.c
* @描述: 比差计算公式
* 
* 
* @参数: 
* @param:CurValue  :电表有功功率值
* @param:TarValue  :标准表有功功率值
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
INT32U CalculateSC(INT32U CurValue,INT32U TarValue)
{
    //  2016-5-20  mj //                
    INT32S s32_tmp0;
    double f_CurValue,f_TarValue,f_Error;
    double f_s32_tmp0,f_s32_tmp1,f_s32_tmp2,f_s32_tmp3;

    // 获取当前误差  ((CurValue-TarValue)/TarValue )*100% //    
    f_CurValue = (double)CurValue;
    f_TarValue = (double)TarValue;
    f_Error = (f_CurValue-TarValue)*1.0/f_TarValue;

	f_s32_tmp0 = 1.0/(1+f_Error);

	f_s32_tmp1 = f_s32_tmp0 - 1.0;
	
	f_s32_tmp2 = f_s32_tmp1* 0x80000000;
	
	f_s32_tmp3 = floor(f_s32_tmp2);     // 向下取整 //
	s32_tmp0 = (INT32S)f_s32_tmp3;	
	return (s32_tmp0);
}

/*=========================================================================================\n
* @function_name: CalculateAC
* @function_file: EnergyBottom.c
* @描述: 角差计算公式
* 
* 
* @参数: 
* @param:CurValue   ：电表功率 
* @param:TarValue   ：标准表功率
* 
* @返回: 
* @return: uint8    
* @作者:   lwb (2012-06-18)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
INT8U CalculateAC(INT32U CurValue,INT32U TarValue)
{
    //计算公式更换,雷文斌，2011年7月14日//
    INT16U Error;                    
    INT8U Result;
	INT8U JCFH;
	
	JCFH = 0;
	if(TarValue < 50)   return 0;
	
	if(TarValue>CurValue)  
	{
		// 负误差 //
		Error=(TarValue-CurValue)*1000/TarValue;            //计算误差值。。扩大1000
		JCFH = 1;
	}
	else
	{
		// 正误差 //
		Error=(CurValue-TarValue)*1000/TarValue;            //计算误差值。。扩大1000
	}
    Result = (INT32U)3011*Error/2000;
	if(JCFH==1)
	{
		Result|=BIT7;
	}
    return Result; 
}

/*********************************************************************************************************
** 函数名称: SoftAdjust_IB_Process
** 功能描述: 校准大功率
** 入口参数: jzval：理论功率值：脉冲常数/3600*表台输出功率/1000*ICONT*8388608 如：1200/3600/220*10/1000*96*8388608=576716
**           channel: 电流通道 0-通道1 1-通道2       
** 出口参数: 无	
********************************************************************************************************/
void SoftAdjust_IB_Process(uint32 jzval,uint8 channel)
{
   uint32 val;
   uint32 adjust_wcval;

    CLRWDT();
    if(channel==EMU_CH_L)
    {
        val=0;
        SetMeterCfgACK(val,SCQ);  //校正功率增益
        SetMeterCfgACK(val,PARAQC);   
    }
    else
    {
        val=0;
        WriteRaccoon(val,RegWAPT, BPhy);  //校正功率增益
        //SetMeterCfgACK(val,PARAQC);   
       WriteRaccoon(val,RegWWAPT, BPhy);  // 角差  
    }
   adjust_wcval=Get_Original_Power(2,channel); 
   val = CalculateSC(adjust_wcval,jzval);
   
    CLRWDT(); 
    if(channel==EMU_CH_L)
    {
        gs_adj_emu_param.w2gain=val;      
    }
    else
    {
        gs_adj_emu_param.w1gain=val;      
    }
}	

/*********************************************************************************************************
** 函数名称: SoftAdjust_IB5_Process
** 功能描述: 校准小功率
** 入口参数: jzval：理论功率值 脉冲常数/3600*表台输出功率/1000*ICONT*8388608 如：1200/3600/220*0.5/1000*96*8388608=28835
**           channle:电流通道号 0-通道1  1-通道2
** 出口参数: 无	
********************************************************************************************************/
void SoftAdjust_IB5_Process(uint32 jzval,uint8 channel)
{
   bit Flag_fu=0;   //负号标志	

   uint32 val;		 
   uint32 adjust_wcval;
              

    CLRWDT();
    val=0;
    if(channel==EMU_CH_L)
    {
        SetMeterCfgACK(val,PARAQC);   
    }
    else
    {
        //SetMeterCfgACK(val,PARAQC);   
       WriteRaccoon(val,RegWWAPT, BPhy);  //二次补偿
    }
   adjust_wcval=Get_Original_Power(3,channel); 
   val = CalculateAC(adjust_wcval,jzval);

    CLRWDT(); 
    if(channel==EMU_CH_L)
    {
        gs_adj_emu_param.watt2os=val;      
    }
    else
    {
        gs_adj_emu_param.watt1os=val;      
    }

}
			
/*********************************************************************************************************
** 函数名称: SoftAdjust_Phase_Process
** 功能描述: 功率校准相位
** 入口参数: jzval：理论功率值 脉冲常数/3600*表台输出功率/1000*ICONT*8388608 如：1200/3600/220*5/1000*96*8388608=288358
**           channel：通道号
** 出口参数: 无	
********************************************************************************************************/
void SoftAdjust_Phase_Process(uint32 jzval,uint8 channel)
{   
   uint32 val;		 
   uint32 adjust_wcval;
  
           

    CLRWDT();
    val=0;
    if(channel==EMU_CH_L)
    {
        SetMeterCfgACK(val,PHCCtrl2);   
    }
    else
    {
         WriteRaccoon(0x000B00A9 ,RegMTPARA1, BPhy);  // 角差  
    }
    CLRWDT();

    gs_emu_adj_var.sec_flg=FALSE;
    while(gs_emu_adj_var.sec_flg==FALSE);              //延时1秒 等待功率稳定
    gs_emu_adj_var.sec_flg=FALSE; 

   adjust_wcval=Get_Original_Power(2,channel); 
   val = CalculateAC(adjust_wcval,jzval);
   
    CLRWDT(); 
    if(channel==EMU_CH_L)
    {
        gs_adj_emu_param.p2cal=val;      
    }
    else  //   
    {
	 if(val&BIT7)  //负值
	 {
	  val&=~0x00000080;
	  val = (val*1.1f);
	  val =(val<<8);
	  val|=(0x00040000);  //bit18位 写入1 延时电压
	 }
	 else
	 {
	  val = (val*1.1f);
	  val =(val<<8);
	  val&=(~0x00040000);//  取消符号位
	 }
	 gs_adj_emu_param.p2cal = 0x000B00A9;
	 gs_adj_emu_param.p2cal |=val;
    }

}
      
/*********************************************************************************************************
** 函数名称: SoftAdjust_dlxProcess	
** 功能描述: 校准调零线校准
** 入口参数: jzval：理论功率值 脉冲常数/3600*表台输出功率/1000*ICONT*8388608 如：1200/3600/220*5/1000*96*8388608=288358
**           channel：通道号
** 出口参数: 无	
********************************************************************************************************/
//void SoftAdjust_dlxProcess(uint32 jzval,uint8 channel)
//{
//    uint32 xdata adjust_wcval;

//    CLRWDT();
//    Init_EMU(DLX_MODE);
//    gs_emu_adj_var.sec_flg=FALSE;
//    while(gs_emu_adj_var.sec_flg==FALSE);              //延时1秒 等待功率稳定
//    gs_emu_adj_var.sec_flg=FALSE; 
//    CLRWDT();

//    gs_emu_adj_var.sec_flg=FALSE;
//    while(gs_emu_adj_var.sec_flg==FALSE);              //延时1秒 等待功率稳定
//    gs_emu_adj_var.sec_flg=FALSE;    
//    CLRWDT();

//    if(channel==EMU_CH_L)
//    {
//        //Dis_Adjust(7);		
//        adjust_wcval=Get_Original_Irms(3,EMU_CH_L);		//读取A通道实际 电流有效值数据 //
//       gs_adj_emu_param.vconst_val_ch1 =((float) jzval)/((float) adjust_wcval);   //获取A通道的常数计量系数 //
//    }
//    else
//    {
//        //Dis_Adjust(8);
//        adjust_wcval=Get_Original_Irms(3,EMU_CH_N);   //读取B通道实际 电流有效值数据 //
//        gs_adj_emu_param.vconst_val_ch2 =((float) jzval)/((float) adjust_wcval);   //获取B通道的常数计量系数 //
//    }

//    CLRWDT();
//    Init_EMU(NORMAL_MODE);
//    Save_EMU_AdjParam();
//    Check_EMU_AdjParam();
//    NOP();
//    NOP();
//}


//无功补偿
uint32 CalculateSChz(int16 error,int32 LastSC)
{
    //计算公式更换,雷文斌，2011年7月14日
    int16 Error;                    
    uint16 Stemp0remd,Stemp2remd;  
    int32 Stemp0,Stemp1,Stemp2,Stemp3;//
    uint32 Result;
    //公式说明 计算误差公式 S=2^31(1/(1+e)-1)+S1(1/(1+e)),如果e扩大1000倍后，
    //化解公式得:  S=  2^31(-e/(1000+e))+S1(1000/(1000+e)=S1(1000/(1000+e)-2^31(e/(1000+e))
    //下面计算  2^31(e/(1000+e))
    Error=error;                                                               //计算误差值。。扩大1000
    Stemp0=0x80000000/(1000+Error);                     //先保留整数部分 2^31/(1000+e)=0x80000000/(1000+e)
    Stemp0remd=0x80000000%(1000+Error);                 //保留余数
    Stemp0remd=Stemp0remd*1000/(1000+Error);            //计算小数部分，小数部分扩大1000
    Stemp1=Stemp0*Error+(Stemp0remd*Error)/1000;        //把计算的整数和小数部分都乘以ERROR，得2^31(e/(1000+e))
    //下面计算S1*(1000/(1000+e)
    Stemp2=LastSC/(1000+Error);                         //计算整数部分
    Stemp2remd=LastSC%(1000+Error);                     //计算余数部分
    Stemp2remd=Stemp2remd*1000/(1000+Error);            //小数部分
    Stemp3=Stemp2*1000+Stemp2remd;                      //整数部分加小数部分  

    Result=Stemp3-Stemp1;                               //S=S1(1000/(1000+e)-2^31(e/(1000+e))
    return Result;
 
}
void TimeEventProc(void)
{
    static uint32 temp=0;
    uint16 HZ;
	
	HZ = 8192000/ReadMeterParaACK(0x10fd);
	if(temp != HZ) 
	{
	    temp = HZ;
	    if(HZ > 500)
	    {
	        gs_adj_emu_param.w2gain = CalculateSChz((HZ -500)*2,gs_adj_emu_param.w1gain);  //每隔0.1hz补0.2%
	        SetMeterCfgACK(gs_adj_emu_param.w2gain,SCQ);     
	    }
	    else if(HZ < 500)
	    {
	        gs_adj_emu_param.w2gain = CalculateSChz(-(500 -HZ)*2,gs_adj_emu_param.w1gain);
	        SetMeterCfgACK(gs_adj_emu_param.w2gain,SCQ); 
	    }
	    else
	    {
	        SetMeterCfgACK(gs_adj_emu_param.w1gain,SCQ); 
	        gs_adj_emu_param.w2gain = gs_adj_emu_param.w1gain;
	    }
	}

}
/*********************************************************************************************************
** 函数名称: Hardware_AdjustDiv_Sub
** 功能描述: 硬件短接 校准步骤判断
** 入口参数: 无
** 出口参数: 无
********************************************************************************************************/
void Hardware_AdjustDiv_Sub(void)
{
   uint8 i;
    uint32 val;
   uint32 tmpval;
   uint32 tmpval1=0;	 
   uint32 jzwcval;
   uint8 ajust1_type; // A路校表步骤
    uint8 ajust2_type;// B路校表步骤
    uint32 cmp_val[4];//  校表点功率阈值
   uint32 adjust1_wcval;  // A 路校表功率
//   uint32 adjust2_wcval;  // B路校表功率
   ST_U32_U08 TempValue;

    if(EA==0)  return;

    //  准备阶段  ，获取功率值，判断进行哪一步的校表//
    gs_emu_adj_var.adj_flg =0x55;   // =0X55 标识目前进入校表状态 //

    jzwcval=cst_adj_const.C_PCONST;     // 额定功率点的功率校准定点数据//
    cmp_val[0]=jzwcval*25/10;         //250% IB 基本校表点参数采用默认值初始化 //
    cmp_val[1]=jzwcval*70/100;       // 基本点校准 //
    cmp_val[2]=jzwcval*35/100;       // 角差校准 //
    cmp_val[3]=jzwcval*3/100;         // 小信号校准 //

    CLRWDT(); 	

// A路准备 	 
    adjust1_wcval=Get_Original_Power(4,EMU_CH_L);	 //读取实际功率
    if(adjust1_wcval>cmp_val[0])
    {
        ajust1_type=HARD_EADJ_STEP0_L_INITLIZE;//IB 1.0 ch0;
    }
    else if(adjust1_wcval>cmp_val[1])
    {
        ajust1_type=HARD_EADJ_STEP1_L_100IB_100L;//IB 1.0 ch0;
    }
    else if(adjust1_wcval>cmp_val[2])
    {
        ajust1_type=HARD_EADJ_STEP2_L_100IB_50L;//IB 0.5 ch0;   
    }   
    else if(adjust1_wcval>cmp_val[3])
    {
        ajust1_type=HARD_EADJ_STEP3_L_5IB_100L;//5%IB 1.0 ch0;   
    }
    else 
    {
        ajust1_type=HARD_EADJ_STEP8_METER_CLR;//5%IB 1.0 ch0;   
	}

	
 // B路状态//
   /*    adjust2_wcval = Get_Original_Power(4,EMU_CH_N);

        if(adjust2_wcval>cmp_val[0])
        {
            ajust2_type=HARD_EADJ_STEP4_N_INITLIZE;//IB 1.0 ch0;
        }
        else if(adjust2_wcval>cmp_val[1])
        {
            ajust2_type=HARD_EADJ_STEP5_N_100IB_100L;//IB 1.0 ch1;
        }
        else if(adjust2_wcval>cmp_val[2])
        {
            ajust2_type=HARD_EADJ_STEP6_N_100IB_50L;//IB 0.5 ch1;     
        }   
        else if(adjust2_wcval>cmp_val[3])
        {
            ajust2_type=HARD_EADJ_STEP7_N_5IB_100L;//5%IB 1.0 ch1;   
        }
        else
        {
            ajust2_type=HARD_EADJ_STEP8_METER_CLR;// 电表清零  //   
        }

*/
    //   分步骤校准处理   //
  // A路//
    //  HARD_EADJ_STEP0_L_INITLIZE   L回路比差寄存器初始化//
    if(ajust1_type==HARD_EADJ_STEP0_L_INITLIZE)
    {                      
        gs_adj_emu_param.w2gain = 0;   //初始化L1通道比差寄存器  //
        gs_adj_emu_param.p2cal = 0;   //初始化L1通道比差寄存器  //
		api_chg_LCDDisplay_adj_item(DIS_ADJ_CH1_0);   // 液晶显示 //
    }

    //  HARD_EADJ_STEP1_L_100IB_100L   L回路100%Ib, 1.0L 校准//
    if(ajust1_type==HARD_EADJ_STEP1_L_100IB_100L)
    {                      
        // L回路有功增益校准 // 
        SoftAdjust_IB_Process(jzwcval,EMU_CH_L); 

        // L回路电流有效值增益校准 //  	
        tmpval=0;
        for(i=0;i<8;i++)
        {
            Lib_Delay_ms(30);
            val = ReadMeterParaACK(RMSII2); // 2017-1-12 ZWK
            if(val>=0x80000000)
            {
                val=~val+1;  
            } 		  		
            tmpval=tmpval+val;
            CLRWDT();          
        }
        tmpval=tmpval/8;
        //电流有效值的位数在此处决定，取决于cst_adj_const.I_ref和基本电流整数的倍数//
        gs_adj_emu_param.i2rms_xs=(float)cst_adj_const.I_ref/(float)tmpval;  
        //电压有效值增益校准 //             		
        tmpval=0;
        for(i=0;i<8;i++)
        {
            Lib_Delay_ms(30);
            val = ReadMeterParaACK(RMSIU); // 修改为读秒平均值
            if(val>=0x80000000)
            {
                val=~val+1;  
            } 		  		
            tmpval=tmpval+val;
            CLRWDT();        
        }
          tmpval=tmpval/8;      
        //电压有效值的位数在此处决定，取决于cst_adj_const.U_ref和基本电压整数的倍数//
        gs_adj_emu_param.vrms_xs=(float)cst_adj_const.U_ref/(float)tmpval;  
	    api_chg_LCDDisplay_adj_item(DIS_ADJ_CH1_1);   // 液晶显示 //

	CLRWDT();
    }

    //  HARD_EADJ_STEP2_L_100IB_50L   L回路100%Ib, 0.5L 校准//
    if(ajust1_type==HARD_EADJ_STEP2_L_100IB_50L)
    {                            
        jzwcval=jzwcval/2;
        SoftAdjust_Phase_Process(jzwcval,EMU_CH_L);  
	api_chg_LCDDisplay_adj_item(DIS_ADJ_CH1_2);   // 液晶显示 //

    }
    //  HARD_EADJ_STEP3_L_5IB_100L   L回路5%Ib, 1.0L 校准//
    if(ajust1_type==HARD_EADJ_STEP3_L_5IB_100L)
    {                            
        jzwcval=jzwcval/20;
        SoftAdjust_IB5_Process(jzwcval,EMU_CH_L);   
	api_chg_LCDDisplay_adj_item(DIS_ADJ_CH1_3);   // 液晶显示 //

    }


// 9260F校表相关//

    //  HARD_EADJ_STEP4_N_INITLIZE   N回路比差寄存器初始化//
    if(ajust2_type==HARD_EADJ_STEP4_N_INITLIZE)
    {                      
        gs_adj_emu_param.w2gain = 0;   //初始化L1通道比差寄存器  //
        gs_adj_emu_param.p2cal= 0x000B00A9;   //初始化L1通道比差寄存器  //
	    api_chg_LCDDisplay_adj_item(DIS_ADJ_CH2_0);   // 液晶显示 //
    }

    //  HARD_EADJ_STEP4_N_100IB_100L   N回路100%Ib, 1.0L 校准//
    if(ajust2_type==HARD_EADJ_STEP5_N_100IB_100L)
    {              
        // L回路有功增益校准 // 
        SoftAdjust_IB_Process(REF_RMSPn_9260,EMU_CH_N);
        tmpval=0;
        for(i=0;i<8;i++)
        {
	   if(ReadRaccoon(RegARTI,1,BPhy)==TRUE)    // 修改为读秒平均值  // 最大 200ms读取等待    //  理论时间76msRegWARTI
	   {
		memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
		Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
		val = TempValue.u32;
	   }
	   else // 读取失败， 重新读取一次// 
	   {
		gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	   }
            if(val>=0x80000000)
            {
                val=~val+1;  
            } 		  		
            tmpval=tmpval+val;
            CLRWDT();          
        }
        tmpval=tmpval/8;
        //电流有效值的位数在此处决定，取决于cst_adj_const.I_ref和基本电流整数的倍数//
        gs_adj_emu_param.i2rms_xs=(float)cst_adj_const.I_ref/(float)tmpval;  

        /// 2路电压有效值增益校准 //             		
        tmpval=0;
        for(i=0;i<8;i++)
        {
	   if(ReadRaccoon(RegARTU,1,BPhy)==TRUE)    // 修改为读秒平均值  // 最大 200ms读取等待    //  理论时间76ms   RegWARTU
	   {
		memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
		Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
		val = TempValue.u32;
	   }
	   else // 读取失败， 重新读取一次// 
	   {
		gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	   }
            if(val>=0x80000000)
            {
                val=~val+1;  
            } 		  		
            tmpval=tmpval+val;
            CLRWDT();        
        }
        tmpval=tmpval/8;      
        //电压有效值的位数在此处决定，取决于cst_adj_const.U_ref和基本电压整数的倍数//
        gs_adj_emu_param.vrms_xs=(float)cst_adj_const.U_ref/(float)tmpval;  
	    api_chg_LCDDisplay_adj_item(DIS_ADJ_CH2_1);   // 液晶显示 //
    }

    if(ajust2_type==HARD_EADJ_STEP6_N_100IB_50L)
    {                      
        jzwcval=jzwcval/2;
        SoftAdjust_Phase_Process(jzwcval,EMU_CH_N);  
        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH2_2);   // 液晶显示 //		
    }    
    //  HARD_EADJ_STEP7_N_5IB_100L   N回路5%Ib, 1.0L 校准//
    if(ajust2_type==HARD_EADJ_STEP7_N_5IB_100L)
    {                         
        jzwcval=jzwcval/20;
        SoftAdjust_IB5_Process(jzwcval,EMU_CH_N);   
        api_chg_LCDDisplay_adj_item(DIS_ADJ_CH2_3);   // 液晶显示 //   
    }

    if((ajust2_type==HARD_EADJ_STEP8_METER_CLR)&&(ajust1_type==HARD_EADJ_STEP8_METER_CLR))
    {        
        api_clr_current_energy_data(); // 电量清零//
        CLRWDT();
	 //   api_clr_current_MD_data(); //当前需量清零
        CLRWDT();

#if (BILL_MAX_NUM>0)
	    api_clr_bill_data();
#endif
	    gs_uart_iec_app_var[g_ch_comm].delay_10ms = 0; // 
        CLRWDT();
	    //api_deal_even_class_recode(ID_CLASS_I_resetMD,START);
	    api_chg_LCDDisplay_adj_item(DIS_DATA_CLR);   // 液晶显示 //
    }

    Save_EMU_AdjParam();
    Check_EMU_AdjParam();
    CLRWDT();
    gs_emu_adj_var.adj_flg =0;   // =0X0 标识目前退出校表状态 //	

}
//////////////以下为新增软件校准步骤 yang 2017-12-26/////////////////
//-------------------------------------------------------------------------------//
//  Function:      Measure_Electric_Parameter_Per_Sec(void)      
//  Description: 较表增益计算Gain//
//  Parameters:   无  
//  Returns:        无                          
//  Others:   xxx.xxV   xx.xxx A  xxxx.x W     
//-------------------------------------------------------------------------------//
void Measure_Electric_Err_Gain(INT8U *sptr)
{   
	union Union_DWordDef u32_tmp_U,u32_tmp_A,u32_tmp_P,u32_tmp;

	 char errbuf[7];
	 float Err_Data;	
	///Voltage///
	u32_tmp.BYTE[0]=0;	
	u32_tmp.BYTE[1]=Lib_AtoB(*(sptr+13));
	u32_tmp.BYTE[2]=((Lib_AtoB(*(sptr+14)))<<4) + Lib_AtoB(*(sptr+15));
	u32_tmp.BYTE[3]=((Lib_AtoB(*(sptr+17)))<<4) + Lib_AtoB(*(sptr+18));
	u32_tmp_U.UDWORD = Lib_bcd4_long(&u32_tmp.BYTE[0]);
	///Current///
	u32_tmp.BYTE[0]=0;	
	u32_tmp.BYTE[1]=Lib_AtoB(*(sptr+19));
	u32_tmp.BYTE[2]=((Lib_AtoB(*(sptr+20)))<<4) + Lib_AtoB(*(sptr+22));
	u32_tmp.BYTE[3]=((Lib_AtoB(*(sptr+23)))<<4) + Lib_AtoB(*(sptr+24));
	u32_tmp_A.UDWORD = Lib_bcd4_long(&u32_tmp.BYTE[0]);
	///Active Power///
	u32_tmp.BYTE[0]=0;	
	u32_tmp.BYTE[1]=Lib_AtoB(*(sptr+25));
	u32_tmp.BYTE[2]=((Lib_AtoB(*(sptr+26)))<<4) + Lib_AtoB(*(sptr+27));
	u32_tmp.BYTE[3]=((Lib_AtoB(*(sptr+28)))<<4) + Lib_AtoB(*(sptr+30));
	u32_tmp_P.UDWORD = Lib_bcd4_long(&u32_tmp.BYTE[0]);

	//误差转换//
	Lib_Copy_Str_TwoArry( &errbuf[0], sptr+13+6+6+6,6);
	errbuf[6] = '\0';
	Err_Data = atof(&errbuf[0]);   // asc 转 float  //

	switch(comm_data.di1_di0.u32)//(comm_data.di1_di0.UDWORD)
	{
		case  0x0000FEB0:
		case  0x0000FEB3:
			Calibration_Power(Err_Data);	
			CLRWDT();
			Calibration_Current(u32_tmp_A.UDWORD,u32_tmp_U.UDWORD);
			break;
		case  0x0000FEB1:
		case  0x0000FEB4:
			Calibration_Angular(Err_Data);
			break;
		case  0x0000FEB2:
		case  0x0000FEB5:
			Calibration_small_signal(Err_Data);
			break;
		default :
			return;
	}
    Save_EMU_AdjParam();
    Check_EMU_AdjParam();
    CLRWDT();
}
//***************************************************************************
// 功能:   通讯中调用的校准功率能量比差的函数（1路和2路自动判断）
// 输入： POWER，格式0.1W,16进制
// 中间量：u32PMdatal，为比差校准的参数结果
//***************************************************************************
void Calibration_Power(float f_err_data)
{
//	 INT8U  offset;
	 INT32U u32_tmp;
	 INT32U u32_PMdataltemp;

//	 static uint32 temp=0;
//	 uint16 HZ;
   	CLRWDT();

    if(f_err_data>0)
    {
        u32_tmp = 100000*(100+f_err_data) ;   
    }
    else
    {
        u32_tmp = 100000*(100+f_err_data) ;  //-
    }
#if meter_type == D_1P3W

	if(comm_data.di1_di0.u32 == 0x0000FEB3)    // DI03B  IA作为N回路
	{

		u32_PMdataltemp=ReadMeterParaACK(SCP);  
		u32_PMdataltemp=CalculateSC(u32_tmp,10000000);  
		SetMeterCfgACK(u32_PMdataltemp,SCP);
        gs_adj_emu_param.w1gain=u32_PMdataltemp;      
	}
	else if(comm_data.di1_di0.u32 == 0x0000FEB0)  // DI03B  IB作为主回路  //
	{
		u32_PMdataltemp=ReadMeterParaACK(SCQ);
		u32_PMdataltemp=CalculateSC(u32_tmp,10000000);  
		SetMeterCfgACK(u32_PMdataltemp,SCQ);
        gs_adj_emu_param.w2gain=u32_PMdataltemp;      
	}
#endif

#if meter_type == D_1P2W
	if(comm_data.di1_di0.u32 == 0x0000FEB0)    // DI03B  IA作为N回路
	{

		u32_PMdataltemp=ReadMeterParaACK(SCP);  
		u32_PMdataltemp=CalculateSC(u32_tmp,10000000);  
		SetMeterCfgACK(u32_PMdataltemp,SCP);
        gs_adj_emu_param.w1gain=u32_PMdataltemp;
		     
	}

#endif
}


//***************************************************************************
// 功能:   通讯中调用的校准电压电流的函数（1路和2路自动判断）
// 输入： Current，格式0.001A,16进制
//        Voltage, 格式0.01V,16进制
// 中间量：u32PMdatal，为比差校准的参数结果
//***************************************************************************
void  Calibration_Current(INT32U Current,INT32U Voltage)
{
	 INT32U u32_tmp,u32_PMdataltemp,u32_PMdataltempSII1;
  
	if(comm_data.di1_di0.u32 == 0x0000FEB0)    // DI03B  IA作为N回路  //0x0000FEB3 <-> 0x0000FEB0
	{
		// 电流校准 //
	    u32_tmp = ReadMeterParaACK(RMSI1);
	   	u32_tmp *= gs_adj_emu_param.i1rms_xs;//获取电流显示值 yang 2017-12-28
		u32_PMdataltemp = ReadMeterParaACK(SCI1);
		if(u32_PMdataltemp == 0)
		{
			//未校准，需要进行校准 //
		  	u32_PMdataltemp =CalculateSC(u32_tmp,Current);	
			gs_adj_emu_param.irms1os=u32_PMdataltemp;
		}

		// 电压校准 //
		u32_tmp = ReadMeterParaACK(RMSU);
	   	u32_tmp *= (gs_adj_emu_param.vrms_xs*10);//获取电压显示值 yang 2017-12-28
		u32_PMdataltemp =ReadMeterParaACK(SCU);
		if(u32_PMdataltemp == 0) 
		{
			//未校准，需要进行校准 //
			u32_PMdataltemp=CalculateSC(u32_tmp,Voltage);
			gs_adj_emu_param.urmsos=u32_PMdataltemp;
			SetMeterCfgACK(u32_PMdataltemp,SCU);
		}
	}
	else if(comm_data.di1_di0.u32 == 0x0000FEB3)  // DI03B  IB作为主回路  //
	{
		// 电流校准 //
		u32_tmp = ReadMeterParaACK(RMSI2);
		u32_tmp*= gs_adj_emu_param.i2rms_xs;//获取电流的显示值 yang 2017-12-28
		u32_PMdataltemp = ReadMeterParaACK(SCI2);
		if(u32_PMdataltemp == 0)
		{
			//未校准，需要进行校准 //
			u32_PMdataltemp =CalculateSC(u32_tmp,Current);	
			gs_adj_emu_param.irms2os=u32_PMdataltemp;
		}

		// 电压校准 //
		u32_tmp = ReadMeterParaACK(RMSU);
		u32_tmp *= (gs_adj_emu_param.vrms_xs*10);//获取电压显示值 yang 2017-12-28
		u32_PMdataltemp =ReadMeterParaACK(SCU);
		if(u32_PMdataltemp == 0) 
		{
			//未校准，需要进行校准 //
			u32_PMdataltemp=CalculateSC(u32_tmp,Voltage);
			gs_adj_emu_param.urmsos=u32_PMdataltemp;
			SetMeterCfgACK(u32_PMdataltemp,SCU);
		}
	}


	
}

//***************************************************************************
// 功能:   功率能量角差校准（1路和2路自动判断）
// 输入： POWER，格式0.1W,16进制
// 中间量：u32PMdatal，为角差校准的参数结果
//***************************************************************************
void Calibration_Angular(float f_err_data)
{
	INT32U u32_tmp,u32_PMdataltemp;
//	INT8U  offset;
	union Union_DWordDef    PHCCTRL1_adj_val;	// I1 角差调节寄存器 实际为8位  //
	union Union_DWordDef	PHCCTRL2_adj_val;	// I2 角差调节寄存器 实际为8位  //
    if(f_err_data>0)
    {
        u32_tmp = 100000*(100+f_err_data) ;   
    }
    else
    {
        u32_tmp = 100000*(100+f_err_data) ;  //-
    }

    u32_PMdataltemp=(INT32U)CalculateAC(u32_tmp,10000000);	
#if meter_type == D_1P3W
	if(comm_data.di1_di0.u32 == 0x0000FEB4)    // DI03B  IA作为N回路  
	{
		/*u32_PMdataltemp =ReadMeterParaACK(DATAP);
		if (u32_PMdataltemp&0x80000000)
		{    
		u32_PMdataltemp = ~u32_PMdataltemp+1;
		} 
		u32_tmp= u32_PMdataltemp*10;
		u32_tmp /=K_RMSP;
		//offset=113;	// 2回路的人工干预补偿值  //  0.6
		offset=0;
		*/
		gs_adj_emu_param.p1cal=u32_PMdataltemp;
		Lib_long_bcd4(&PHCCTRL1_adj_val.BYTE[0],u32_PMdataltemp);
		SetMeterCfgACK(PHCCTRL1_adj_val.BYTE[3],PHCCtrl1);  //设置I1角差值
	}
	else if(comm_data.di1_di0.u32 == 0x0000FEB1)   // DI03B  IB作为主回路  //	
	{
		/*
		u32_PMdataltemp =ReadMeterParaACK(DATAQ);
		if (u32_PMdataltemp&0x80000000)
		{    
			u32_PMdataltemp = ~u32_PMdataltemp+1;
		} 
		u32_tmp= u32_PMdataltemp*10;
		u32_tmp /=K_RMSQ;
		u32_PMdataltemp=(INT32U)CalculateAC(u32_tmp,POWER);	
		*/
		gs_adj_emu_param.p2cal=u32_PMdataltemp;
		Lib_long_bcd4(&PHCCTRL2_adj_val.BYTE[0],u32_PMdataltemp);
		SetMeterCfgACK(PHCCTRL2_adj_val.BYTE[3],PHCCtrl2);  //设置I2角差值

	}
#endif
#if meter_type == D_1P2W
	if(comm_data.di1_di0.u32 == 0x0000FEB1)    // DI03B  IA作为N回路  
	{
		gs_adj_emu_param.p1cal=u32_PMdataltemp;
		Lib_long_bcd4(&PHCCTRL1_adj_val.BYTE[0],u32_PMdataltemp);
		SetMeterCfgACK(PHCCTRL1_adj_val.BYTE[3],PHCCtrl1);  //设置I1角差值
	}
	else if(comm_data.di1_di0.u32 == 0x0000FEB4)   // DI03B  IB作为主回路  //	
	{
		gs_adj_emu_param.p2cal=u32_PMdataltemp;
		Lib_long_bcd4(&PHCCTRL2_adj_val.BYTE[0],u32_PMdataltemp);
		SetMeterCfgACK(PHCCTRL2_adj_val.BYTE[3],PHCCtrl2);  //设置I2角差值

	}
#endif

}

//***************************************************************************
// 功能:   小信号补偿（）
// 输入： POWER，格式0.1W,16进制
// 中间量：
//***************************************************************************
void Calibration_small_signal(float f_err_data)
{
	 INT32U u32_PMdataltemp;
	 INT8U  offset;
	 INT32S s32_tmp,s32_PMdataltemp;

#if meter_type == D_1P3W
	if(comm_data.di1_di0.u32 == 0x0000FEB5)          // DI03B  IA作为N回路  
	{
		s32_PMdataltemp=ReadMeterParaACK(DATAP);
	
		s32_tmp = (INT32S)((float)f_err_data *s32_PMdataltemp/100.0);

		offset=0;    //2回路的人工干预补偿值  //
		if(s32_tmp>0) u32_PMdataltemp = s32_tmp;
		else	u32_PMdataltemp = 	0xFFFFFFFF +s32_tmp;
		

		SetMeterCfgACK(u32_PMdataltemp,PARAPC);
        gs_adj_emu_param.watt1os=u32_PMdataltemp;      
	}

	else if(comm_data.di1_di0.u32 == 0x0000FEB2) 	// DI03B  IB作为主回路  //
	{
		s32_PMdataltemp=ReadMeterParaACK(DATAQ);
	
		s32_tmp = (INT32S)((float)f_err_data *s32_PMdataltemp/100.0);

		offset=0;    //2回路的人工干预补偿值  //
		if(s32_tmp>0) u32_PMdataltemp = s32_tmp;
		else	u32_PMdataltemp = 	0xFFFFFFFF +s32_tmp;
		

		SetMeterCfgACK(u32_PMdataltemp,PARAQC);
        gs_adj_emu_param.watt2os=u32_PMdataltemp;      
	}
#endif
#if meter_type == D_1P2W
	if(comm_data.di1_di0.u32 == 0x0000FEB2)          // DI03B  IA作为N回路  
	{
		s32_PMdataltemp=ReadMeterParaACK(DATAP);
	
		s32_tmp = (INT32S)((float)f_err_data *s32_PMdataltemp/100.0);

		offset=0;    //2回路的人工干预补偿值  //
		if(s32_tmp>0) u32_PMdataltemp = s32_tmp;
		else	u32_PMdataltemp = 	0xFFFFFFFF +s32_tmp;
		

		SetMeterCfgACK(u32_PMdataltemp,PARAPC);
        gs_adj_emu_param.watt1os=u32_PMdataltemp;      
	}

	else if(comm_data.di1_di0.u32 == 0x0000FEB5) 	// DI03B  IB作为主回路  //
	{
		s32_PMdataltemp=ReadMeterParaACK(DATAQ);
	
		s32_tmp = (INT32S)((float)f_err_data *s32_PMdataltemp/100.0);

		offset=0;    //2回路的人工干预补偿值  //
		if(s32_tmp>0) u32_PMdataltemp = s32_tmp;
		else	u32_PMdataltemp = 	0xFFFFFFFF +s32_tmp;
		

		SetMeterCfgACK(u32_PMdataltemp,PARAQC);
        gs_adj_emu_param.watt2os=u32_PMdataltemp;      
	}
#endif

}




/////////////////////////////////////////////////////////////////////////

/*******************************************************************************************
**    END
*******************************************************************************************/



/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Measure.H
**Author		: maji
**date			: 2016-04-21 
**description	: c code for 测量程序应用层
**note			: MCU- G80F92XD ，MERTER FOR DL06A
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-21   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#include <MyIncludes_H.h>

MEASURE_VAR_DATA gs_measure_var_data;
MEASURE_STATU_JUDGE_VAR gs_measure_status_judge_var;


/*****************************************************************************
** Function name    :api_Measure_ram_INIT
**
** Description         :电网测量变量初始化
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_Measure_ram_INIT(void)
{ 
    Lib_Set_String(&gs_measure_var_data.gs_really[0].dw_u_val.B08[0],0,sizeof(MEASURE_VAR_DATA));
    Lib_Set_String((uint8 *)&gs_measure_status_judge_var,0,sizeof(MEASURE_STATU_JUDGE_VAR));
}

/*****************************************************************************
** Function name    :api_measure_deal_pre_second
**
** Description         :每秒调用的测量函数
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_measure_deal_pre_second(void)
{
 uint32 u32_UI, u32_P, u32_PF;
     CLRWDT();   // 清看门狗//

    //获取计量通道、能量数据赋值、//
     lnk_CAL_EMU_constSum_per_second();

    //电压获取  //
    gs_measure_var_data.gs_really[PHASE_TT].dw_u_val.u32= Lnk_get_EMU_voltage(WIRE_L1);
   // gs_measure_var_data.gs_really[WIRE_L2].dw_u_val.u32= Lnk_get_EMU_voltage(WIRE_L2);
    //电流获取  //
    //2018-12-18 cxy  修改为能两路计量
    gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32 = 0;
   	gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32 += Lnk_get_EMU_current(WIRE_L1);
	gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32 += Lnk_get_EMU_current(WIRE_L2);
	//电网频率测量//
    gs_measure_var_data.gs_really[PHASE_TT].w_freq_val.u16 = (uint16)Lnk_get_EMU_freq(WIRE_L1);
   // gs_measure_var_data.gs_really[WIRE_L2].w_freq_val.u16 = (uint16)Lnk_get_EMU_freq(WIRE_L2);

   
    //功率因数计算  //
    //计算功率因数，量纲0.001 格式X.XXX //
    u32_P = 1000*gs_measure_var_data.gs_really[PHASE_TT].dw_p_val.u32;
    u32_UI = ((gs_measure_var_data.gs_really[PHASE_TT].dw_u_val.u32 * gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32 )/1000);
    if(u32_UI ==0)
    {
         gs_measure_var_data.gs_really[PHASE_TT].dw_pf_val.u16= 1000;             
    }
    else
    {
        u32_PF = u32_P/u32_UI;
        if(u32_PF>995)  u32_PF = 1000;
        gs_measure_var_data.gs_really[PHASE_TT].dw_pf_val.u16 = (uint16)(u32_PF);        
    }    
}	  
/*****************************************************************************
** Function name    :api_measure_VBAT_pre_min
**
** Description         :每分钟测试一次电池电压
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_measure_VBAT_pre_min(void)
{
    gs_measure_var_data.w_bat_v.u16= (Samlpe_BatAdc());
  
      if( gs_measure_var_data.w_bat_v.u16<= 280 ) 
     	{
          gs_measure_var_data.w_bat_v.u16= (Samlpe_BatAdc());
          if( gs_measure_var_data.w_bat_v.u16<= 280 )  
		 {
		 gs_measure_status_judge_var.u8_status |=VBAT_LOW;
		
          	}
     	}
      else
      	{
         gs_measure_status_judge_var.u8_status &= (~VBAT_LOW);
       }	
}


/*****************************************************************************
** Function name    :api_measure_VBAT_pre_min
**
** Description         :每分钟测试一次电池电压
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_measure_temperature_pre_min(void)
{
    Enable_M_ADC(M_ADC_CH1_TEMPERATURE);
    Lib_Delay_ms(10);

    gs_measure_var_data.w_temp.u16 = Samlpe_temperatureAdc();

    Disable_M_ADC();
}


/***************************************************************
*    END
****************************************************************/


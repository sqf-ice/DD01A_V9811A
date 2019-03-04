/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Demand.C
**Author		: maji
**date			: 2016-04-20 
**description	: 需量处理应用层相关函数程序C文件
**note			: MERTER FOR DL06A
**                         当前最大需量用户数据均存储在外部E2P内
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-20   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>


MD_VAR gs_md_var;           // 最大需量计算变量RAM区数据 //
MD_PARAM gs_md_param;  // 最大需量计算参数RAM区数据 //

/*****************************************************************************
** Function name    :api_check_Demand_para
**
** Description         :检查需量参数合法性     
**
** Parameters         :*u8_data 需量参数数组首地址      
**
** Returned value   :flg=TRUE 合法；flg=FALSE 非法
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
uint8 api_check_Demand_para(MD_PARAM *s_para)
{
 uint8 flg=FALSE;
 uint8 u8_period,u8_slid;
    
    u8_period = s_para->u8_period;
    u8_slid = s_para->u8_slid;

    if((u8_period>0)&&(u8_slid>0))  
    {
        if((u8_period<=30)&&(u8_slid<=30))  
        {
            if(u8_slid <= u8_period)
            {
                if((u8_period%u8_slid)==0)
                {
                    flg=TRUE;
                }
            }            
        }
    }

    return(flg);
    
}


/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :初始化当前需量计算RAM区数据，程序复位初始化调用        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_init_md_data_ram(void)
{

    //获取最大需量计算参数 //
    mem_read(&gs_md_param.u8_period, ADR_METER_PARAM1_MD, 2, MEM_E2P1);

    if(api_check_Demand_para(&gs_md_param) ==FALSE)
    {
        // 出错后取默认数据 //
        gs_md_param.u8_period =30;
        gs_md_param.u8_slid =30;
    }

    // 清除需量计算相关时时变量 //
    Lib_Set_String(&gs_md_var.min_var[0].cnt_sec,0,sizeof(MD_VAR));
}

/*****************************************************************************
** Function name    :api_updata_md_var_min_per_second
**
** Description         :需量功率数据通道每秒累加处理，秒变化时调用    
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_cmp_save_md_data(uint8 ch, uint32 val_hex32)
{
 MMD_DATA_UNIT st_mmd_unit;
 uint16 adr_offset;
 //SUM_MMD_DATA_UNIT sum_st_mmd_unit;//累计最大需量

    if(ch<MD_CH_NUM)
    {
        adr_offset = ( ch*(1+TARIFF_MAX_NUM)*LEN_MMD_UNIT) ;  
        mem_read(&st_mmd_unit.buf[0], ADR_BLOCK01_MMD_L0_E2P+adr_offset, LEN_MMD_UNIT, MEM_E2P1);  
        if(val_hex32>st_mmd_unit.val.val_hex32) 
        {
            st_mmd_unit.val.val_hex32 =  val_hex32;
            st_mmd_unit.val.date_rtc[0] = gs_CurDateTime.Year;
            st_mmd_unit.val.date_rtc[1] = gs_CurDateTime.Month;
            st_mmd_unit.val.date_rtc[2] = gs_CurDateTime.Day;
            st_mmd_unit.val.date_rtc[3] = gs_CurDateTime.Hour;
            st_mmd_unit.val.date_rtc[4] = gs_CurDateTime.Minute;
            mem_db_write(ADR_BLOCK01_MMD_L0_E2P+adr_offset,&st_mmd_unit.buf[0],LEN_MMD_UNIT,MEM_E2P1);
			mem_write(&st_mmd_unit_array[0].buf[0],&st_mmd_unit.buf[0],LEN_MMD_UNIT,MEM_RAM);
			mem_write(ADR_BLOCK_4MMD_L0_E2P,&st_mmd_unit.buf[0],LEN_MMD_UNIT,MEM_E2P1);
        }
        #if (TARIFF_MAX_NUM>1)
        // 费率 最大需量处理    //
        adr_offset = ( ch*(1+TARIFF_MAX_NUM)*LEN_MMD_UNIT) +(EN_TRIFF_no*LEN_MMD_UNIT);  
        mem_read(&st_mmd_unit.buf[0], ADR_BLOCK01_MMD_L0_E2P+adr_offset, LEN_MMD_UNIT, MEM_E2P1);  
        if(val_hex32>st_mmd_unit.val.val_hex32) 
        {
            st_mmd_unit.val.val_hex32 =  val_hex32;
            st_mmd_unit.val.date_rtc[0] = gs_CurDateTime.Year;
            st_mmd_unit.val.date_rtc[1] = gs_CurDateTime.Month;
            st_mmd_unit.val.date_rtc[2] = gs_CurDateTime.Day;
            st_mmd_unit.val.date_rtc[3] = gs_CurDateTime.Hour;
            st_mmd_unit.val.date_rtc[4] = gs_CurDateTime.Minute;
            mem_db_write(ADR_BLOCK01_MMD_L0_E2P+adr_offset,&st_mmd_unit.buf[0],LEN_MMD_UNIT,MEM_E2P1);
        }
        
        #endif   
		
//				//日冻结需量
//		mem_read(&st_mmd_unit.buf[0], ADR_BLOCK_4MMD_L0_E2P, LEN_MMD_UNIT, MEM_E2P1);//读出当前最大需量（当日中的）
//		mem_read(&sum_st_mmd_unit.buf[0], ADR_BLOCK_4MMD_sum_L0_E2P, LEN_SUM_MMD_UNIT, MEM_E2P1);
//		if(val_hex32>st_mmd_unit.val.val_hex32)
//		{
//			st_mmd_unit.val.val_hex32 =  val_hex32;
//			sum_st_mmd_unit.val.val_hex32 = val_hex32;
//			sum_st_mmd_unit.val.val_count ++;
//			sum_st_mmd_unit.val.date_rtc[0] = st_mmd_unit.val.date_rtc[0] = gs_CurDateTime.Year;
//			sum_st_mmd_unit.val.date_rtc[1] = st_mmd_unit.val.date_rtc[1] = gs_CurDateTime.Month;
//			sum_st_mmd_unit.val.date_rtc[2] = st_mmd_unit.val.date_rtc[2] = gs_CurDateTime.Day;
//			sum_st_mmd_unit.val.date_rtc[3] = st_mmd_unit.val.date_rtc[3] = gs_CurDateTime.Hour;
//			sum_st_mmd_unit.val.date_rtc[4] = st_mmd_unit.val.date_rtc[4] = gs_CurDateTime.Minute;
//			mem_db_write(ADR_BLOCK_4MMD_L0_E2P,&st_mmd_unit.buf[0],LEN_MMD_UNIT,MEM_E2P1);
//			mem_db_write(ADR_BLOCK_4MMD_sum_L0_E2P,&sum_st_mmd_unit.buf[0],LEN_SUM_MMD_UNIT,MEM_E2P1);
//		}    
    }
  

}


/*****************************************************************************
** Function name    :lnk_updata_md_var_power_per_second
**
** Description         :每秒更新每个最大需量通道的时时功率数据    
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void lnk_updata_md_var_power_per_second(void)
{
 uint8 i;
    
    for(i=0;i<MD_CH_NUM;i++)
    {
        switch(i)
        {
            case MD_CH_AC_PT_TT:   // 当前总有功最大需量通道  //
                gs_md_var.power_val[i] = gs_measure_var_data.gs_really[PHASE_TT].dw_p_val.u32 ;    // 需要修改为最终的功率数据变量，不应该是0 //
                break;

			case MD_CH_RE_PT_TT:   // 当前总有功最大需量通道  //
				gs_md_var.power_val[i] = gs_measure_var_data.gs_really[PHASE_TT].dw_q_val.u32 ;    // 需要修改为最终的功率数据变量，不应该是0 //
			break;
            default:
                sys_err();
        }
    }

}


/*****************************************************************************
** Function name    :api_updata_md_var_min_per_second
**
** Description         :需量功率数据通道每秒累加处理，秒变化时调用    
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_updata_md_var_min_per_second(void)
{
 uint8 i;

    //获取每个通道的秒功率数据 //
    lnk_updata_md_var_power_per_second();



    for(i=0;i<MD_CH_NUM;i++)
    {
        gs_md_var.min_var[i].cnt_sec++;
        gs_md_var.min_var[i].sum_min += gs_md_var.power_val[i] ;
    }
 
}



/*****************************************************************************
** Function name    :api_updata_md_var_slid_per_minute
**
** Description         :初始化当前需量计算RAM区数据，程序复位初始化调用        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_updata_md_var_slid_per_minute(void)
{
 uint8 i,j;
 uint32 sum_hex32;

    // 需量参数检查 //
  if(api_check_Demand_para(&gs_md_param) ==FALSE)
    {
        // 出错后取默认数据 //
        gs_md_param.u8_period =30;
        gs_md_param.u8_slid =30;
    }

    for(i=0;i<MD_CH_NUM;i++)
    {
        // 计算每分钟时时平均功率(时时需量) //
        if(gs_md_var.min_var[i].cnt_sec !=0)
        {
            gs_md_var.min_var[i].val_min = (gs_md_var.min_var[i].sum_min/gs_md_var.min_var[i].cnt_sec);
            gs_md_var.min_var[i].cnt_sec  = 0; 
            gs_md_var.min_var[i].sum_min = 0;
        }
        
        // 更新滑差计算方法相关数据 //
        if(gs_md_var.slid_var[i].cnt_min<gs_md_param.u8_period)
        {
            gs_md_var.slid_var[i].date_buff[gs_md_var.slid_var[i].cnt_min] = gs_md_var.min_var[i].val_min;
            gs_md_var.slid_var[i].cnt_min++;
            if(gs_md_var.slid_var[i].cnt_min==gs_md_param.u8_period)
            {
                gs_md_var.slid_var[i].cal_flg = TRUE;
                gs_md_var.slid_var[i].cnt_min = 0;
            }

            if(gs_md_var.slid_var[i].cal_flg == TRUE)
            {
                sum_hex32 =0;
                if((gs_md_var.slid_var[i].cnt_min %gs_md_param.u8_slid)==0 )   
                {
                    for(j=0;j<gs_md_param.u8_period;j++)
                    {
                        sum_hex32 += gs_md_var.slid_var[i].date_buff[j];    
                    }
                    gs_md_var.slid_var[i].val_slid =sum_hex32/gs_md_param.u8_period;
                    api_cmp_save_md_data(i, gs_md_var.slid_var[i].val_slid );
                }
            }
        }

    }

}


/*****************************************************************************
** Function name    :api_clr_current_energy_data
**
** Description         :当前电能数据的清零处理
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_clr_current_MD_data(void)
{
    MD_USER_DATA tmp_ms_user_data;
	MMD_DATA_UNIT st_mmd_unit;

	mem_read(&st_mmd_unit.buf[0], ADR_BLOCK01_MMD_L0_E2P, LEN_MMD_UNIT, MEM_E2P1);
	sum_st_mmd_unit_array[0].val.val_hex32 += st_mmd_unit.val.val_hex32;
	sum_st_mmd_unit_array[0].val.val_count++;
    mem_write(ADR_BLOCK_4MMD_sum_L0_E2P,&sum_st_mmd_unit_array[0].buf[0],LEN_SUM_MMD_UNIT,MEM_E2P1);   
    //当前需量清零  //
    Lib_Clr_String(&tmp_ms_user_data.us_val[0][0].buf[0],LEN_MMD_CURRENT);
	Lib_Clr_String(&st_mmd_unit_array[0].buf[0],LEN_MMD_CURRENT);
    mem_db_write(ADR_BLOCK01_MMD_L0_E2P,&tmp_ms_user_data.us_val[0][0].buf[0], LEN_MMD_CURRENT, MEM_E2P1);   
	mem_write(ADR_BLOCK_4MMD_L0_E2P,&st_mmd_unit_array[0].buf[0],LEN_MMD_UNIT,MEM_E2P1);
	CLRWDT();
    // 清除需量计算相关时时变量 //
    Lib_Set_String(&gs_md_var.min_var[0].cnt_sec,0,sizeof(MD_VAR));
}




/***************************************************************
*    END
****************************************************************/


/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Energy.C
**Author		: maji
**date			: 2016-04-20 
**description	: 电能计量应用层相关函数C文件
**note			: MERTER FOR DL06A
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-20   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#include <MyIncludes_H.h>




ENERGY_USER_DATA    gs_energy_user_data;            //当前电量RAM主数据  //
ENERGY_USER_DATA    gs_energy_user_data_back;   //当前电量RAM备份数据 //
ENERGY_VAR		      gs_energy_var;                 // 电能增量脉冲变量数据，上电初始化为0  //


/*****************************************************************************
** Function name    :api_init_current_energy_data_ram
**
** Description         :初始化当前电量RAM区数据，程序复位初始化调用        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_init_current_energy_data_ram(void)
{
//定义变量
	uint8 Bill_Data;
	uint8 i;
	ST_U16_U08 u16_month_A,u16_month_B;
	ST_U16_U08 u16_day_A,u16_day_B;
	uint8 last_rec_dateTime[6];//最近一次冻结时间
	uint8 max_dateTime;
////////
    Lib_Set_String(&gs_energy_user_data.us_val[0][0].buf[0],0,sizeof(ENERGY_USER_DATA));
    Lib_Set_String(&gs_energy_user_data_back.us_val[0][0].buf[0],0,sizeof(ENERGY_USER_DATA));
    api_check_current_energy_data(); 


	//校验和不对就从EEP读取数据
	mem_read(&energy_data_array[0].buf[0],ADR_BLOCK_4Energy_L0_E2P,  5*LEN_EC_UNIT,MEM_E2P1);
	mem_read(&st_mmd_unit_array[0].buf[0],ADR_BLOCK_4MMD_L0_E2P,5*LEN_NEW_MMD_UNIT,MEM_E2P1);
	mem_read(&sum_st_mmd_unit_array[0].buf[0],ADR_BLOCK_4MMD_sum_L0_E2P,5*LEN_SUM_MMD_UNIT,MEM_E2P1);
	d_flag = 0;

	//校验日冻结RAM数据 2019-01-25
//	Excsck = 0;
//	Excsck += Lib_get_csck_int16u_num(&energy_data_array[0].buf[0],5*LEN_EC_UNIT,CHECKWORD); 
//	Excsck += Lib_get_csck_int16u_num(&st_mmd_unit_array[0].buf[0],5*LEN_NEW_MMD_UNIT,CHECKWORD);
//	Excsck += Lib_get_csck_int16u_num(&sum_st_mmd_unit_array[0].buf[0],5*LEN_SUM_MMD_UNIT,CHECKWORD);
//	if(csck == Excsck)
//	{
//		mem_read(&last_rec_dateTime[0],ADR_BLOCK_4MMD_L0_E2P+4,5,MEM_E2P1);//年月日时分
//		//2019-01-25 cxy  将当前电量读出并写入RAM区
//		mem_read(&energy_data_array[0].buf[0],&gs_energy_user_data_back.us_val[0][0].buf[0], LEN_EC_CURRENT, MEM_RAM);
//		//2019-01-25 cxy  上电将RAM区日冻结数据写入EEP	
//		mem_write(ADR_BLOCK_4Energy_L0_E2P,&energy_data_array[0].buf[0],5*LEN_EC_UNIT,MEM_E2P1);
//		mem_write(ADR_BLOCK_4MMD_L0_E2P,&st_mmd_unit_array[0].buf[0],5*LEN_NEW_MMD_UNIT,MEM_E2P1);
//		mem_write(ADR_BLOCK_4MMD_sum_L0_E2P,&sum_st_mmd_unit_array[0].buf[0],5*LEN_SUM_MMD_UNIT,MEM_E2P1);
//		
//		//计算掉电过了几个零点
		mem_read(&last_rec_dateTime[0],ADR_BLOCK161_EVENII_PWN_E2P+6,5,MEM_E2P1);//年月日时分
		u16_month_A.u16 = Lib_bcd_byte(last_rec_dateTime[0])*12+ Lib_bcd_byte(last_rec_dateTime[1]);//几个月
		u16_month_B.u16 = Lib_bcd_byte(gs_CurDateTime.Year)*12 + Lib_bcd_byte(gs_CurDateTime.Month);
		CLRWDT();
		if(u16_month_B.u16 >= u16_month_A.u16)
		{
			if(u16_month_B.u16 - u16_month_A.u16 > 1)
			{
				 max_dateTime = 6;
			}
			else
			{
				if(u16_month_B.u16 - u16_month_A.u16 == 1)
					max_dateTime = Judage_Month(last_rec_dateTime[1],last_rec_dateTime[0])-last_rec_dateTime[2]+gs_CurDateTime.Day;
				else
					max_dateTime = gs_CurDateTime.Day -last_rec_dateTime[2];
			}
			//读取结算方式  2019-01-30
			mem_read(&Bill_Data, ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,BILL_FLAG), 1, MEM_E2P1);
	
			if(Bill_Data == 0x01)
			{
				if(max_dateTime > 0 && max_dateTime <=12)
				{
					CLRWDT();
					for(i=0;i<max_dateTime ;i++)
					{
						api_write_bill_data(BILL_ID0_MOTTH_SETTL);
						CLRWDT();
					}
					CLRWDT();
					api_clr_current_MD_data();
	//				sum_st_mmd_unit_array[0].val.val_count--;  //多加一次
					CLRWDT();
				}
			}
//			if(Bill_Data == 0x30)
//			{
//				//补月结算
//				#if (BILL_MAX_NUM>1)
//					api_handl_settle_when_powerUp();   
//				#endif
//			}
		}
//	}
//	else
//	{	

//  }

	// 两三特殊//
    gs_energy_var.pluse[0] = 0;
}

/*****************************************************************************
** Function name    :api_check_current_energy_data
**
** Description         :当前电能数据的检查处理，电能累计函数调用
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_check_current_energy_data(void)
{   
    INT16U csck_tmp;
 
    //电能数据获取  //   
    csck_tmp = Lib_get_csck_int16u_num(&gs_energy_user_data.us_val[0][0].buf[0],LEN_EC_CURRENT,CHECKWORD);
    if(csck_tmp != gs_energy_user_data.u16_csck) 
    {
        csck_tmp = Lib_get_csck_int16u_num(&gs_energy_user_data_back.us_val[0][0].buf[0],LEN_EC_CURRENT,CHECKWORD);        
		if(csck_tmp != gs_energy_user_data_back.u16_csck) 
        {
            mem_read(&gs_energy_user_data_back.us_val[0][0].buf[0], ADR_BLOCK00_EC_L0_E2P, LEN_EC_CURRENT, MEM_E2P1);
            gs_energy_user_data_back.u16_csck = Lib_get_csck_int16u_num(&gs_energy_user_data_back.us_val[0][0].buf[0],LEN_EC_CURRENT,CHECKWORD);
			/*
			mem_read(&gs_energy_user_data_back.us_val[0][0].buf[0], ADR_BLOCK00_EC_L0_E2P, (LEN_EC_CURRENT+2), MEM_E2P1);
			csck_tmp= Lib_get_csck_int16u_num(&gs_energy_user_data_back.us_val[0][0].buf[0],LEN_EC_CURRENT,CHECKWORD);
			if(csck_tmp != gs_energy_user_data_back.u16_csck)
			{
				Lib_Set_String(&gs_energy_user_data.us_val[0][0].buf[0],0,sizeof(ENERGY_USER_DATA));
				Lib_Set_String(&gs_energy_user_data_back.us_val[0][0].buf[0],0,sizeof(ENERGY_USER_DATA));
			}
			*/
        }
        Lib_Copy_Str_TwoArry(&gs_energy_user_data.us_val[0][0].buf[0],&gs_energy_user_data_back.us_val[0][0].buf[0],LEN_EC_CURRENT);
        gs_energy_user_data.u16_csck = gs_energy_user_data_back.u16_csck;       
    }
}

/*****************************************************************************
** Function name    :lnk_inc_energy_pluse
**
** Description         :当前有功电能脉冲增量处理，安装定义的通道进行累计    
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
uint8 lnk_add_fac_energy_pluse(void)
{

  uint8 chg_fg=FALSE; 
  uint8 u8_tmp,i;
	
  for (i=0; i<EC_CH_NUM; i++)
  {        
	  if(gs_energy_var.pluse[i]>0)
	  {
		  DisableINT();    				// disable interrupt function  //
		  u8_tmp = gs_energy_var.pluse[i];
		  gs_energy_var.pluse[i] =0;  //有功脉冲清零处理 //
		  EnableINT();
		
		  gs_energy_user_data.us_val[i][0].val.decim_hex16 += u8_tmp;  // 总累加	组合不累加//
		  #if (TARIFF_MAX_NUM>1)
		  gs_energy_user_data.us_val[i][EN_TRIFF_no].val.decim_hex16 += u8_tmp; //  组合不累加//
		  #endif
		  chg_fg = TRUE;
	  }
	}
	
	 return (chg_fg);
}



/*****************************************************************************
** Function name    :lnk_inc_energy_pluse
**
** Description         :当前无功功电能脉冲增量处理，安装定义的通道进行累计    
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
uint8 lnk_add_rac_energy_pluse(void)
{

uint8 chg_fg=FALSE;
	/*
	uint8 u8_tmp;

	//象限模式字获取，两相不判断象限只判断正反相  0x0F为1234象限 正反相
	mem_read(&gs_sys_globaL_var.Rac_energy[0], ADR_METER_PARAM1_REC_ENERGY, 2, MEM_E2P1);   
	if(gs_energy_var.pluse[1]>0)
	{
		DisableINT();    				// disable interrupt function  //
		u8_tmp = gs_energy_var.pluse[1];
		gs_energy_var.pluse[1] =0;  //有功脉冲清零处理 //
		EnableINT();

        if((gs_sys_globaL_var.Rac_energy[1]&0x0F)==0x06) //2、3不累加 1、4 象限累加 正向累加
		{
		  if((gs_emu_run_var.net_flg&REV_TT_RACPOWER)==0)
		  {
		   gs_energy_user_data.us_val[EC_CH_RE_PT_TT][0].val.decim_hex16 += u8_tmp;  // 总累加  组合不累加//
		   #if (TARIFF_MAX_NUM>1)
	 	   gs_energy_user_data.us_val[EC_CH_RE_PT_TT][EN_TRIFF_no].val.decim_hex16 += u8_tmp; //  组合不累加//
		   #endif
		  }
		 chg_fg = TRUE;
		}
		else if((gs_sys_globaL_var.Rac_energy[1]&0x0F)==0x09)  //1、4象限不累加。2、3象限累加  反向累加//
		{
		  if(gs_emu_run_var.net_flg&REV_TT_RACPOWER)
		  {
		   gs_energy_user_data.us_val[EC_CH_RE_PT_TT][0].val.decim_hex16 += u8_tmp;  // 总累加  组合不累加//
		   #if (TARIFF_MAX_NUM>1)
	 	   gs_energy_user_data.us_val[EC_CH_RE_PT_TT][EN_TRIFF_no].val.decim_hex16 += u8_tmp; //  组合不累加//
		   #endif
		  }
		 chg_fg = TRUE;
		}
		else //默认总  合相正反累加
		{
		   gs_energy_user_data.us_val[EC_CH_RE_PT_TT][0].val.decim_hex16 += u8_tmp;  // 总累加  组合不累加//
		   #if (TARIFF_MAX_NUM>1)
	 	   gs_energy_user_data.us_val[EC_CH_RE_PT_TT][EN_TRIFF_no].val.decim_hex16 += u8_tmp; //  组合不累加//
		   #endif
		   chg_fg = TRUE;
		}
		
		if(gs_emu_run_var.net_flg&REV_TT_RACPOWER)
		{
		 	  gs_energy_user_data.us_val[EC_REV_REC_NUM][0].val.decim_hex16 += u8_tmp; // 反向累加
		   #if (TARIFF_MAX_NUM>1)
		      gs_energy_user_data.us_val[EC_REV_REC_NUM][EN_TRIFF_no].val.decim_hex16 += u8_tmp; // 反向累加
		   #endif
		}
		else 
		{
		  	  gs_energy_user_data.us_val[EC_FOR_REC_NUM][0].val.decim_hex16 += u8_tmp;//正向累加
		   #if (TARIFF_MAX_NUM>1)
		      gs_energy_user_data.us_val[EC_FOR_REC_NUM][EN_TRIFF_no].val.decim_hex16 += u8_tmp;//正向累加
		   #endif
		}
	}
*/
	 return (chg_fg);
}

/*****************************************************************************
** Function name    :api_accum_current_energy_data_pre_second
**
** Description         :当前电能数据累计处理，每秒调用
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_accum_current_energy_data_pre_second(void)
{
	uint8 save_fg=FALSE;
	uint8 i,chg_fg1;

    //if(CheckPOWERUP() !=TRUE)   return;  
	// 电能数据合法性检查 //
	api_check_current_energy_data();
	//有功电能脉冲增量处理 //
	chg_fg1 =lnk_add_fac_energy_pluse(); 

	if(chg_fg1==FALSE) return; 
	
	save_fg=FALSE;
	for(i=0;i<EC_CH_NUM;i++)
	{
		if(gs_energy_user_data.us_val[i][0].val.decim_hex16>=METER_CONST_1KWH)
		{
 		 	if(i==0) 	save_fg = TRUE;   //暂时不支持无功整度的存取   电能通道1的电量发生整数进位时配置存储标识 //
			gs_energy_user_data.us_val[i][0].val.decim_hex16 -= METER_CONST_1KWH;
			gs_energy_user_data.us_val[i][0].val.integ_hex32++;
			if(gs_energy_user_data.us_val[i][0].val.integ_hex32 > MAX_ENERGY_INtEGER)
			{
				gs_energy_user_data.us_val[i][0].val.integ_hex32 = 0;  // 超出最大范围后清理 //
			}
		}
		#if (TARIFF_MAX_NUM>0)
		if(gs_energy_user_data.us_val[i][EN_TRIFF_no].val.decim_hex16>=METER_CONST_1KWH)
		{
			gs_energy_user_data.us_val[i][EN_TRIFF_no].val.decim_hex16 -= METER_CONST_1KWH;
			gs_energy_user_data.us_val[i][EN_TRIFF_no].val.integ_hex32++;
			if(gs_energy_user_data.us_val[i][EN_TRIFF_no].val.integ_hex32 > MAX_ENERGY_INtEGER)
			{
				gs_energy_user_data.us_val[i][EN_TRIFF_no].val.integ_hex32 = 0;  // 超出最大范围后清理 //
			}
		}		
		#endif
		
	}
	
	// 更新RAM区电能校验和和备份区数据//
	gs_energy_user_data.u16_csck = Lib_get_csck_int16u_num(&gs_energy_user_data.us_val[0][0].buf[0],LEN_EC_CURRENT,CHECKWORD);  
	Lib_Copy_Str_TwoArry(&gs_energy_user_data_back.us_val[0][0].buf[0],&gs_energy_user_data.us_val[0][0].buf[0],LEN_EC_E2P);

	// 电能数据保存 //
	if(save_fg == TRUE)
	{
		mem_db_write(ADR_BLOCK00_EC_L0_E2P,&gs_energy_user_data.us_val[0][0].buf[0],LEN_EC_CURRENT,MEM_E2P1);
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
void api_clr_current_energy_data(void)
{
	// 清除当前电量数据的RAM主区数据 //
	Lib_Set_String(&gs_energy_user_data.us_val[0][0].buf[0],0,LEN_EC_CURRENT);
	gs_energy_user_data.u16_csck = Lib_get_csck_int16u_num(&gs_energy_user_data.us_val[0][0].buf[0],LEN_EC_CURRENT,CHECKWORD);  
	// 清除当前电量数据的RAM备份区数据 //
	Lib_Copy_Str_TwoArry(&gs_energy_user_data_back.us_val[0][0].buf[0],&gs_energy_user_data.us_val[0][0].buf[0],LEN_EC_E2P);
	// 清除当前电量数据的E2P区数据 //
	mem_db_write(ADR_BLOCK00_EC_L0_E2P,&gs_energy_user_data.us_val[0][0].buf[0],LEN_EC_E2P-2,MEM_E2P1);
}


/***************************************************************
*    END
****************************************************************/


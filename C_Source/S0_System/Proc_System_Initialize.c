/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_System_Initialize.c
**Author		: maji
**date			: 2016-04-20
**description	: 系统复位初始化处理相关C代码
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>



/****************************************************************
Function:           Proc_Initialize_systerm_normal_mode 
Description:            系统上电/复位初始化函数 
Input:			no
Parameters:         	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_handle_when_systerm_reset(void)
{
   #if(MCU_TYPE == MCU_G80F92X)   // 中颖芯片 //
	IEN0=0x00;
	IEN1=0x00;
	gs_sys_globaL_var.reset_status = RSTSTAT;
	Switch_Clk_4P096M();
   #endif

   #if(MCU_TYPE == MCU_V98XX)   //万工芯片 //
   gs_sys_globaL_var.reset_status = Systate;
         #ifndef _RTC_CAL    //不需要温补操作时执行下面内容 //
            if((RTCCH != 0) | (RTCCL != 0))
            {
                RTCPEN = 0x96;
                RTCPWD = 0x57;
                RTCCH = 0x00;
                RTCCL = 0x00;
                RTCPEN = 0x96;
                RTCPWD = 0x56;
            }
            NOP();
          #endif

    SPCFNC=1;  		//调整FLASH功耗
    XBYTE[0x0402]=0x86;
    SPCFNC=0;
    CBANK=0X01;
    Set_McuClock(FSYSCLK);   //SetMCUPLL();  6Mhz
   #endif


    Init_GPIO_sleep();
    E2p_IIC_Initail();
    E2p_Stop_i2c();

}


/****************************************************************
Function:   		Proc_Initialize_INRam
Description: 		上电初始化RAM相关
Input:			no
Parameters:     	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_Initialize_INRam(void)
{
//	uint8 Bill_Data;
//	uint8 i;
//	ST_U16_U08 u16_month_A,u16_month_B;
//	ST_U16_U08 u16_day_A,u16_day_B;
//	uint8 last_rec_dateTime[6];//最近一次冻结时间
//	uint8 max_dateTime;


	INT8U eeprom_flg[3];
	//////////dubge   将E2P初始化////////////
	//mem_db_clr(0XA5);
	////////////////////////
	////读取EEPROM是否首次上电标志位，若flg=0x653412为已经上过电的表计，///////
	////反之，该表计从未上过电，将单备份区清零 双校验区校验位清零 yang 2017-12-29////
	mem_read(&eeprom_flg[0], ADR_METER_PARAM1_EEPROM_Flag, 3, MEM_E2P1);
	if((eeprom_flg[0]!=0x65)||(eeprom_flg[1]!=0x34)||(eeprom_flg[2]!=0x12))
	{
		d_flag = 1;
		api_freeze_energy_pre_day(); //上电时将日冻结EEP区的数据清零
		mem_fill(ADR_BLOCK101_BILL1_DATA_E2P, 0x1000, 0x00, MEM_E2P1);//
		mem_db_clr(0XA5);//将双备份去校验清零
		CLRWDT();
	}

//	d_flag = 1;
//	api_freeze_energy_pre_day(); //上电时将日冻结EEP区的数据清零

	if(gs_sys_globaL_var.work_mode == NORMAL_MODE)
	{
		mem_db_check();  //数据校验检查 //
		CLRWDT();
	}
	////////////////////////////////////////////////
	//  S0_System 区相关RAM初始化
	////////////////////////////////////////////////
	Proc_sys_data_init();
	Proc_init_Check_var();
	CLRWDT();
	////////////////////////////////////////////////
	//  S1_HardDrv 区相关RAM初始化
	////////////////////////////////////////////////
	mem_read((uint8 *)&gs_adj_emu_param, ADR_BLOCK70_EMU_PARAM_E2P, LEN_BLOCK70_EMU_PARAM_E2P, MEM_E2P1);
	Init_EMU_RAM();    
	Proc_init_tou_var();

	////////////////////////////////////////////////
	//  S3_ApiPlat 区相关RAM初始化
	////////////////////////////////////////////////
	Init_LoadPro_param();
	CLRWDT();
	api_init_md_data_ram();
	api_init_current_energy_data_ram();
	api_Measure_ram_INIT();
	api_measure_VBAT_pre_min();
	api_measure_temperature_pre_min();   
	CLRWDT();
#ifdef _RTC_CAL    //不需要温补操作时执行下面d内容 //
	Handl_RTC_Adj_per_minu();
#endif

#if (TARIFF_MAX_NUM>1)
	api_update_triffNo_pre_minute();  
	CLRWDT();
#endif
//mem_read(&Bill_Data, ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,BILL_FLAG), 1, MEM_E2P1);
//if(Bill_Data == 0x30)
//{
//#if (BILL_MAX_NUM>1)
//	CLRWDT();
//	mem_read(&last_rec_dateTime[0],ADR_BLOCK161_EVENII_PWN_E2P+6,5,MEM_E2P1);//年月日时分
//	u16_month_A.u16 = Lib_bcd_byte(last_rec_dateTime[0])*12 + Lib_bcd_byte(last_rec_dateTime[1]);	    //最近1次结算的月单位数据 //
//    u16_month_B.u16 = Lib_bcd_byte(gs_CurDateTime.Year)*12 + Lib_bcd_byte(gs_CurDateTime.Month);
//	if(u16_month_B.u16 - u16_month_A.u16 > 6)
//	{
//		max_dateTime = 6;
//	}
//	else
//	{
//		max_dateTime = u16_month_B.u16 - u16_month_A.u16;
//	}
//	for(i=0;i<max_dateTime ;i++)
//	{
//		CLRWDT();
//		api_handl_settle_per_minute();
//		CLRWDT();
//	}
//	//api_handl_settle_when_powerUp();   
//#endif
//}
	////////////////////////////////////////////////
	//  S4_ApiUser 区相关RAM初始化
	////////////////////////////////////////////////
	api_init_display();
}


/****************************************************************
Function:   		void SysInitialize(void)  
Description: 		系统上电/复位初始化函数 
Input:			no
Parameters:     	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_sleep_when_reset(void)
{
   uint8 volatile  check_cnt;
   uint8 volatile  check_cnt2;
  
    check_cnt=0;
    check_cnt2 = 0;

   CLRWDT(); 
#if(MCU_TYPE == MCU_G80F92X)   // 中颖芯片 //
 
   EMUIE=0x00;  
   EMUSR=0x00;  
   LDOCON=0x00;
   CLRWDT();     
   Init_GPIO_sleep();
  // Config_CpuReg_Sleep();  
   IEN0=0x00;
   IEN1=0x00;                       
   TPCON=0x00;
   Close_SecOut(); 
/////////////////////////////////////
    gs_sys_globaL_var.work_mode = SLEEP_MODE;
    api_Measure_ram_INIT();          //清除瞬时量 //
    api_check_current_energy_data();		//  初始化电量//
    //初始化开盖状态和工厂状态的全局变量代码//
    mem_read(&gs_sys_globaL_var.open_cover_status, ADR_METER_VAR_OPENED, 1, MEM_E2P1);
    mem_read(&gs_sys_globaL_var.meter_factory_status, ADR_METER_PARAM1_FC_CFG, 1, MEM_E2P1);   //数据0=0x5A 代表工厂状态 // 
    Init_LCD();
    Init_RTC();
    gs_dis_ctl_var.item=0x03;
    Link_Get_DisTab_Var(gs_dis_ctl_var.item) ;    		// 获取显示项目信息  //
    api_updated_LCDDisplayPixel_per_second();
///////////////////////////////////////////////// 
   CLRWDT();

   CLKCON=Bin(00000000);       //降速到32K  
   IT21=0;
   IT20=1;                    //外部中断2下降沿触发
   IEN1|=Bin(10000000);       //允许LPD中断  
   IEN1|=Bin(00000010);       //允许外部中断2
   IEN1|=Bin(00001000);       //允许RTC中断
   EA=1;


    while( 1 )
    {		
       CLRWDT();                
        Enter_PwrDwnMode();         // 进入休眠状态 //
        CLRWDT();                          //唤醒后的处理 //

        if(gs_sys_run.back_fg&BIT1_BACK_FG_RTC)
        {
            gs_sys_run.back_fg &= ~BIT1_BACK_FG_RTC;    
            //下电状态下的开表上盖检测 //         
            api_handl_dis_sleep();
        }   

        if(CheckPOWERUP()== TRUE)		//市电判断  //
        {
            Feed_WDT(0x02);        //利用看门狗复位，保证可靠运行 //
            while(1);   
        }

        if(Check_LX_mode()== TRUE)		//PT电源判断  //
        {
            Feed_WDT(0x02);        //利用看门狗复位，保证可靠运行 //
            while(1);   
        }

   } 

#endif


#if(MCU_TYPE == MCU_V98XX)   //万工芯片 //
    CLRWDT();     
    api_Measure_ram_INIT();          //清除瞬时量 //
	api_init_current_energy_data_ram();//  初始化电量//
    //api_check_current_energy_data();		//  初始化电量//
    Init_LCD();
	api_handl_dis_sleep();
	Init_GPIO_sleep();
	SetExtRTC(0x01,0x02);// 小时唤醒
    //SetExtRTC(0x01,0x01);// 分钟唤醒//
	IOWK|=BIT0;
  Set_McuSleep();
	if((Systate&BIT0)==0x01)  //休眠以后人工上电全屏自检
  {   
     sys_err();	  //如果上电则休眠处理 //
  }
#endif
}
	





/****************************************************************
Function:           Proc_Initialize_systerm_normal_mode 
Description:            系统上电/复位初始化函数 
Input:			no
Parameters:         	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_Initialize_systerm_normal_mode(void)
{
	Set_McuClock(FSYSCLK_3M2);   //SetMCUPLL();  6Mhz
	Init_GPIO_run();
	Init_LCD();
	Full_SEG(0xFF);
	CLRWDT();
	Proc_Initialize_INRam();  //初始化RAM区 //
//	Drv_relay_powerup_check();	 //继电器初始化检查处理  //
//	Lnk_SX2176_Intilize();	//LORA//
	Init_Timer();	//定时器
	api_handl_COMM_FRAME_ERROR(UART_CH0_IR);	//TTL 通讯硬件初始化
	Init_RTC();
	Init_EMU(NORMAL_MODE);
	Check_EMU_AdjParam();
	Lib_Set_String( &LPReadPara.Original_Ptr, 0, 5) ;	
	gs_even_fun_var.into_cnt[ID_CLASS_II_PWN-ID_CLASS_II_min]=4;  // 事件保持时间 二类事件序号
	gs_even_fun_var.esc_cnt[ID_CLASS_II_PWN-ID_CLASS_II_min]=4;  // 事件保持时间
	api_deal_even_class_recode(ID_CLASS_II_PWN,END);	
	Lib_Set_String(&gs_even_fun_var.esc_cnt,0,sizeof(EVEN_FUN_VAR));
	KEY_ERR_COVER_INITLIZE() ;
}



/****************************************************************
Function:          Proc_Initialize_systerm_reset
Description:        系统上电/复位初始化函数 
Input:			no
Parameters:         	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_Initialize_systerm_reset(void)
{

	Proc_handle_when_systerm_reset();   // 系统复位的处理函数，注意MCU不同时需要进行修改函数内容 //
	if(CheckPOWERUP()== FALSE)		//
	{
		Lib_Delay_ms(300);
	}
	if(CheckPOWERUP()== FALSE)		//
	{
		gs_sys_globaL_var.work_mode = SLEEP_MODE;  
	}
	else
	{
		gs_sys_globaL_var.work_mode = NORMAL_MODE;
	}

	if(gs_sys_globaL_var.work_mode == SLEEP_MODE)
	{
		Proc_sleep_when_reset();                                   // 休眠初始化//
	}
	else
	{
		Proc_Initialize_systerm_normal_mode();    //  市电供应情况下的初始化//
	}
}

void Proc_display_Power_down_process(void)
{
	 //下电显示

		//检测按键


		if(Judge_PG_DN_key() == TRUE )//&& button_delay == 0)
		{
			button_flag = 1;
//			button_delay = 1;
		//	api_updated_LCDDisplayItem_per_second();
		}
//		if(Judge_PG_DN_key() == FALSE)
//		{
//			button_delay = 0;
//		}
///////////////////////////

		if(button_flag  == 1)
		{
			button_flag = 0;
			if(dis_num_char == 5)
			{
				dis_num_char = 0;
			}
			else
			{
				dis_num_char ++;
			}
			api_updated_LCDDisplayPixel_per_second();
			gs_dis_ctl_var.keep_sec[0] = 6; //轮显持续时间//
		}
		else
		{
			if(gs_dis_ctl_var.keep_sec[0] > 0)
			 	gs_dis_ctl_var.keep_sec[0]--;
		}
		if(gs_dis_ctl_var.keep_sec[0] == 0)
		{
			dis_num_char = 0;
			api_updated_LCDDisplayPixel_per_second();
		}

}

/****************************************************************
Function:           Proc_judge_Power_down_process
Description:       运行过程中的掉电检查函数
Input:			no
Parameters:         	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_judge_Power_down_process(void)
{
   	RTC_TYPE rtc_data_tmp;

	if(CheckPOWERUP()==FALSE)
	{
		PMG=1;                  //关闭计量时钟
		DisableINT();    				// disable interrupt function  //
		api_accum_current_energy_data_pre_second();
		mem_db_write(ADR_BLOCK00_EC_L0_E2P,&gs_energy_user_data.us_val[0][0].buf[0],LEN_EC_CURRENT,MEM_E2P1);
		
//		d_flag = 1;
//		api_freeze_energy_pre_day(); //上电时将日冻结EEP区的数据写入EEP
		gs_sys_globaL_var.work_mode = SLEEP_MODE;
//		mem_db_write( ADR_METER_PARAM1_LOAD_PTR, (INT8U*)&LPRunPa.Ptr[0], LP_RunPara_Len,MEM_E2P1); //负荷记录运行参数 
		mem_db_write(ADR_METER_VAR_RTC, &gs_CurDateTime.Week,7,MEM_E2P1);	
		
		gs_even_fun_var.into_cnt[ID_CLASS_II_PWN-ID_CLASS_II_min]=4;  // 事件保持时间传人ID - 0-6
		api_deal_even_class_recode(ID_CLASS_II_PWN,START);	
	//电网类事件结束
		//	gs_even_fun_var.into_cnt[ID_CLASS_II_NOL1-ID_CLASS_II_min]=4;  // 事件保持时间 二类事件序号
		//	gs_even_fun_var.esc_cnt[ID_CLASS_II_NOL1-ID_CLASS_II_min]=4;  // 事件保持时间
		//	api_deal_even_class_recode(ID_CLASS_II_NOL1,END);
		//	gs_even_fun_var.into_cnt[ID_CLASS_II_NOL2-ID_CLASS_II_min]=4;  // 事件保持时间 二类事件序号
		//	gs_even_fun_var.esc_cnt[ID_CLASS_II_NOL2-ID_CLASS_II_min]=4;  // 事件保持时间
		//	api_deal_even_class_recode(ID_CLASS_II_NOL2,END);
		gs_even_fun_var.into_cnt[ID_CLASS_II_L1reverse-ID_CLASS_II_min]=4;  // 事件保持时间 二类事件序号
		gs_even_fun_var.esc_cnt[ID_CLASS_II_L1reverse-ID_CLASS_II_min]=4;  // 事件保持时间
		api_deal_even_class_recode(ID_CLASS_II_L1reverse,END);
		//	gs_even_fun_var.into_cnt[ID_CLASS_II_L2reverse-ID_CLASS_II_min]=4;  // 事件保持时间 二类事件序号
		//	gs_even_fun_var.esc_cnt[ID_CLASS_II_L2reverse-ID_CLASS_II_min]=4;  // 事件保持时间
		//	api_deal_even_class_recode(ID_CLASS_II_L2reverse,END);

		mem_read(&gs_even_fun_var.even_fun_doingflg[ID_CLASS_II_COVER-ID_CLASS_II_min],ADR_CLASS2FLG_START_E2P+ (ID_CLASS_II_COVER-ID_CLASS_II_min)* LEN_CLASSII_flg,LEN_CLASSII_flg,MEM_E2P1);	
//EEP操作在关闭IIC之前
		api_check_current_energy_data();		//  初始化电量//
		api_handl_dis_sleep();
			
		Init_GPIO_sleep();

		//下翻按键	2019-01-18
		P14FS = 0x00;
		P1IE |= BIT4;//输入使能
	  
		E2p_IIC_Initail();
		E2p_Stop_i2c();
		api_Measure_ram_INIT();          //清除瞬时量 //
		
		KEY_ERR_COVER_INITLIZE() ;		
		gs_even_fun_var.esc_cnt[ID_CLASS_II_COVER-ID_CLASS_II_min] =0; 
		gs_even_fun_var.into_cnt[ID_CLASS_II_COVER-ID_CLASS_II_min] =0;

		gs_dis_ctl_var.keep_sec[0] = 0;//2019-01-18		cxy
		d_flag = 0;
		while(1)
		{
			CLRWDT();
			SetExtRTC(0x01,0x00);//秒唤醒
			IOWK|=BIT0;
			Init_RTC();  
			Set_McuSleep();
			
			//		
			CLRWDT();
			Set_McuClock(FSYSCLK_3M2); 
//			Proc_get_clock();
			
			//获取当前时间数据数据   //
    		Get_RTCTime(&rtc_data_tmp);
			Lib_Copy_Str_TwoArry(&gs_CurDateTime.Week, &rtc_data_tmp.Week, 7);
			if(Judge_ERR_COVER_key()==TRUE)
				api_pwl_cover_even_class_recode(ID_CLASS_II_COVER-ID_CLASS_II_min,START);
			else 
				api_pwl_cover_even_class_recode(ID_CLASS_II_COVER-ID_CLASS_II_min,END);
			
			if((Systate&BIT0)==0x01)  
			{
				Init_GPIO_run();
    				E2p_IIC_Initail();
    				E2p_Start_i2c();
				////
				mem_write(ADR_BLOCK_4Energy_L0_E2P, &energy_data_array[0].buf[0], 5*LEN_EC_UNIT,MEM_E2P1);
				mem_write(ADR_BLOCK_4MMD_L0_E2P,&st_mmd_unit_array[0].buf[0],5*LEN_NEW_MMD_UNIT,MEM_E2P1);
				mem_write(ADR_BLOCK_4MMD_sum_L0_E2P,&sum_st_mmd_unit_array[0].buf[0],5*LEN_SUM_MMD_UNIT,MEM_E2P1);		
			
//				mem_read(&Bill_Data, ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,BILL_FLAG), 1, MEM_E2P1);
				/////
			//	api_freeze_energy_pre_day();  
				sys_err();	  //如果上电 //  
			}
			api_freeze_energy_Ram_pre_day();
			
			Proc_display_Power_down_process();		   
    	}
	}
	else
	{
		return;
	}
}


/****************************************************************
Function:   		void sys_err(void)
Description: 		系统错误后的看门狗复位处理
Input:			no
Parameters:     	no
Returns:			no                        
Others:         
****************************************************************/
void sys_err(void)
{  
    CLRWDT();

    while(1)
    {
		NOP();
		NOP();
		NOP();
    }  
}



/***************************************************************
*    END
****************************************************************/


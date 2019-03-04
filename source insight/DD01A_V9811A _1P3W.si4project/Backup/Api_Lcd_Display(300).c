/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Lcd_Display.c
**Author		: maji
**date			: 2016-04-21 
**description	: 液晶显示应用层相关函数程序C文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#include <MyIncludes_H.h>


/*******************************************************************************
* 全局变量定义区
*******************************************************************************/
DIS_PARAM_DATA   gs_dis_param;   // 电量显示相关参数  //
DIS_CTL_VAR  gs_dis_ctl_var;
DIS_PIXEL_VAR  gs_dis_pixel_var;
LCDDISTAB   gs_LCDDISTAB_var;
INT8U dis_data_type = 0;//电表上排显示电量还是时间还是日期的标志
INT8U dis_data_sec = 0;//除功率外显示持续时间
INT8U dis_num_char = 0;	//0:当前电量，		1:当前需量，		2:当前累计最大需量，
						//3:前一天电量，4:前一天需量，5:前一天累计最大需量




/*******************************************************************************
* 常量表区
*******************************************************************************/
const DIS_PARAM_DATA   code default_dis_param_tab= {
    5,  //uint8  auto_sec;    // 自动循环显示持续时间  //
    15, //uint8  key_sec;     // 按键显示持续时间  //
    360,//uint8  bg_sec;     // 下电显示维持时间 默认6小时 单位min//
    0x51,//uint8  ec_form;     // 电量格式  //
    4,//uint8  auto_item[21];    //0为显示个数，1~19为显示项目，最大可以显示31个项目  //
    1,2,3,4,0xFF,0xFF,            //uint8  auto_item[1~10];     //电压屏，电流屏，功率屏，频率屏
    0x00,//INT16U  u16_csck;    //16位累加和校验值//
};


//  常量字符表定义 x  // D C B A - E G F 
const INT8U code  NumSegD1_D6[DS_Chr_All_NUM] = 
{	DS_Chr_0,DS_Chr_1,DS_Chr_2,
	DS_Chr_3,DS_Chr_4,DS_Chr_5,
	DS_Chr_6,DS_Chr_7,DS_Chr_8,
	DS_Chr_9,
	DS_Chr_A,DS_Chr_b,DS_Chr_C,
	DS_Chr_d,DS_Chr_E,DS_Chr_F
};

const INT8U code  NumSegD7_D9[DS_Chr_All_NUM] = 
{	DS_OBIS_Chr_0,DS_OBIS_Chr_1,DS_OBIS_Chr_2,
	DS_OBIS_Chr_3,DS_OBIS_Chr_4,DS_OBIS_Chr_5,
	DS_OBIS_Chr_6,DS_OBIS_Chr_7,DS_OBIS_Chr_8,
	DS_OBIS_Chr_9,
	DS_OBIS_Chr_A,DS_OBIS_Chr_b,DS_OBIS_Chr_C,
	DS_OBIS_Chr_d,DS_OBIS_Chr_E,DS_OBIS_Chr_F
};




//-------------------------------------------------------------------------------//
//  Function:     液晶显示项目的元素表       --在ROM区
//-------------------------------------------------------------------------------//

const LCDDISTAB code LcdDisTab[] =
 {  
	//DisID	,ObisNum ,	Addr ,	DisInfo, 	chartype,	obis_digit		
	////////////////////////////ALL  DISPLAY////////////////////////////////////////////////////////
	0,  0x8888,	0X0000,										0 ,	 				0,	OBIS_DIGIT_TYPE0,	// ALL DISPLAY  //
	1,  0x327F,	(INT16U)&gs_measure_var_data.gs_really[PHASE_TT].dw_u_val.B08[2],(MEM_RAM<<8)+2, CHAR_V,OBIS_DIGIT_TYPE1,	//电压
	2,	0x317F,	(INT16U)&gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.B08[0],(MEM_RAM<<8)+2, CHAR_A,OBIS_DIGIT_TYPE2,	//电流
	3,  0x170F,	(INT16U)&gs_measure_var_data.gs_really[PHASE_TT].dw_p_val.B08[0],(MEM_RAM<<8)+2,	CHAR_KW,OBIS_DIGIT_TYPE2,	//有功功率	
	4,  0x1470,	(INT16U)&gs_measure_var_data.gs_really[PHASE_TT].w_freq_val[0],(MEM_RAM<<8)+2,	CHAR_HZ,OBIS_DIGIT_TYPE2,	//频率	
	5,  0x1370,	(INT16U)&gs_measure_var_data.gs_really[PHASE_TT].dw_pf_val[0],(MEM_RAM<<8)+2,	CHAR_PF,OBIS_DIGIT_TYPE3,	// 功率因数	

	////////////////////////////ACTIVE ENERGY  DISPLAY////////////////////////////////////////////////////////
	// 0x01,  0x180F,	 (INT16U)&gs_energy_user_data.us_val[EC_CH_AC_PT_TT][0].buf[0],               (MEM_RAM<<8)+6,	CHAR_AEC,OBIS_DIGIT_TYPE1,	// L0 ACTIVE ENERGY TOTAL 
} ;
                                         

/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :每秒更新液晶显示项目       
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_init_display(void)
{
    mem_read(&gs_dis_param.auto_sec, ADR_BLOCK21_DIS_PARAM_E2P, LEN_BLOCK21_DIS_PARAM_E2P, MEM_E2P1);	 //     
    gs_dis_ctl_var.mode = DIS_MODE_RESET;
    gs_dis_ctl_var.item = DIS_RESET;
    gs_dis_ctl_var.ptr[0] =1;  //错过条数数组 //
    gs_dis_ctl_var.keep_sec[0] = 0;
    gs_dis_ctl_var.keep_sec[1] = 0;
    gs_dis_ctl_var.keep_sec[2] = 0;             //  背光上电点亮15S//
    gs_dis_ctl_var.keep_sec[3] = RESEAT_DIS_KEEP_3S; //  上电全屏显示时间
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
void api_handl_dis_key(void)
{

    if((gs_dis_ctl_var.mode == DIS_MODE_NOMAL)||(gs_dis_ctl_var.mode == DIS_MODE_RESET))
    {
        gs_dis_ctl_var.mode = DIS_MODE_NOMAL;
        mem_read(&gs_dis_param.auto_item[0], ADR_DIS_PARAM_AUTO_TAB, 32, MEM_E2P1);	 // 
        //gs_dis_ctl_var.ptr[0] =0xff;
    }

    gs_dis_ctl_var.keep_sec[1] = gs_dis_param.key_sec; // 按键持续时间
    gs_dis_ctl_var.item = api_get_next_dis_item();

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
void api_handl_dis_sleep(void)
{
    gs_dis_ctl_var.item = DIS_DEFAULT_ITEM;    
    gs_dis_ctl_var.mode = DIS_MODE_NOMAL ;
    mem_read(&gs_dis_param.auto_sec, ADR_BLOCK21_DIS_PARAM_E2P, LEN_BLOCK21_DIS_PARAM_E2P, MEM_E2P1);	 //   
    api_updated_LCDDisplayPixel_per_second();
}



/*****************************************************************************
** Function name    :api_updata_LCDDisplay_per_second
**
** Description         :每秒更新液晶显示内容      
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_updated_LCDDisplayPixel_per_second(void)
{ 		
    Lib_Set_String(&gs_dis_pixel_var.dis_buff[0],0,DS_UNIT);
	Lib_Set_String(&gs_dis_pixel_var.dis_obis_buff[0],0,DS_OBISNum);
 	Lib_Set_String(&gs_dis_pixel_var.dis_data_buff[0] ,0,DS_DATANum);
 	Lib_Set_String(&gs_dis_pixel_var.dis_char_buff[0] ,0,DS_CHARNum);
	
    // 增加硬件校表显示功能 //
    if((gs_dis_ctl_var.item>=DIS_ADJ_CH1_0) &&(gs_dis_ctl_var.item<=DIS_DATA_CLR))
    {
        api_LCDDisplay_adj_item(gs_dis_ctl_var.item);
		Link_Get_Dis_Data_Num_buff();
 		Link_Get_Dis_Char_buff();            
        if(gs_sys_globaL_var.work_mode != SLEEP_MODE)Link_Get_Dis_RealChar_buff();
        Link_Get_Dis_Drv_buff();		
        Write_LCD(&gs_dis_pixel_var.dis_buff[0]);         //将数据写入液晶驱动//
        return;
    }
    if(gs_dis_ctl_var.item==DIS_DATA_BROADCAST) 
    {
        api_LCDDisplay_adj_item(gs_dis_ctl_var.item);
		//Link_Get_Dis_Data_Num_buff();
 		//Link_Get_Dis_Char_buff();            
        if(gs_sys_globaL_var.work_mode != SLEEP_MODE)Link_Get_Dis_RealChar_buff();
        Link_Get_Dis_Drv_buff();		
        Write_LCD(&gs_dis_pixel_var.dis_buff[0]);         //将数据写入液晶驱动//
        return;
    }
    if(gs_dis_ctl_var.item>DIS_MAX_ITEM)
    {
        gs_dis_ctl_var.item = DIS_DEFAULT_ITEM;    // 系统显示项目异常后的默认为显示系统错误屏  //
    }

    if(gs_dis_ctl_var.item == DIS_RESET)
    {
        Lib_Set_String(&gs_dis_pixel_var.dis_buff[0],0xff,DS_UNIT);
    }
    else
    {
		Link_Get_DisTab_Var(gs_dis_ctl_var.item) ;    		// 获取显示项目信息  //
		Link_Get_Dis_Data_Num_buff();
//		Link_Get_Dis_OBIS_Num_buff();                   //下排用来显示其他数据 //
 		Link_Get_Dis_Char_buff();            
 		if(gs_sys_globaL_var.work_mode != SLEEP_MODE)  Link_Get_Dis_RealChar_buff();
 		Link_Get_Dis_Drv_buff();		 
    }

    Write_LCD(&gs_dis_pixel_var.dis_buff[0]);         //将数据写入液晶驱动
}


/*****************************************************************************
** Function name    :api_updata_LCDDisplay_per_second
**
** Description         :每秒更新液晶显示内容      
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_updated_LED_per_second(void)
{
   // 反向指示  任意一相反向则灯亮//      
   if(gs_emu_run_var.net_flg&REV_TT_ACPOWER)// 有功合相功率反向
   {
	//LED_L2_REV_ON();
   }
   else 
   {
	//LED_L2_REV_OFF();
   }
}


/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :每秒更新液晶显示项目       
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_updated_LCDDisplayItem_per_second(void)
{
    switch (gs_dis_ctl_var.mode )
    {
        case DIS_MODE_RESET:			 
            gs_dis_ctl_var.keep_sec[3]--;
            if(gs_dis_ctl_var.keep_sec[3]==0)  
            {
                gs_dis_ctl_var.mode = DIS_MODE_NOMAL;
                gs_dis_ctl_var.keep_sec[0] = gs_dis_param.auto_sec;
                gs_dis_ctl_var.ptr[0] = 0;
                gs_dis_ctl_var.item = gs_dis_param.auto_item[gs_dis_ctl_var.ptr[0]+1];  //  0位置为显示项目个数，1位置开始为项目 //                
				dis_num_char = 0;//显示当前电量
			}
        break;

        case DIS_MODE_NOMAL:
			if(Judge_PG_DN_key() == TRUE)
			{
				if(dis_num_char == 5)
				{
					dis_num_char = 0;
				}
				else
				{
					dis_num_char ++;
				}

				gs_dis_ctl_var.keep_sec[0] = gs_dis_param.auto_sec; //轮显持续时间//
				gs_dis_ctl_var.item = api_get_next_dis_item();
			}
			if( gs_dis_ctl_var.keep_sec[0] > gs_dis_param.auto_sec)
			{
				gs_dis_ctl_var.keep_sec[0] = gs_dis_param.auto_sec; //轮显持续时间//
			}
			else
			{
				gs_dis_ctl_var.keep_sec[0]--;
				if( gs_dis_ctl_var.keep_sec[0] == 0)
				{
					gs_dis_ctl_var.keep_sec[0] = gs_dis_param.auto_sec; //轮显持续时间//
					gs_dis_ctl_var.item = api_get_next_dis_item();

					if(dis_num_char >= 3)
					{
						dis_num_char = 0;
					}
					else
					{
						dis_num_char++;//轮显
					}
				}
			}
            
        break;

        default:
            gs_dis_ctl_var.mode = DIS_MODE_NOMAL;
        break;

    }   
}
	   
/*****************************************************************************
** Function name    :api_get_next_dis_item
**
** Description         :返回下一个显示项目     
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
INT8U api_get_next_dis_item(void)
{
 //INT8U vaild_len;
 INT8U item;

    item = DIS_DEFAULT_ITEM;
    if((gs_dis_ctl_var.mode  ==DIS_MODE_NOMAL))
    {
        gs_dis_ctl_var.ptr[0]++;
        if((gs_dis_ctl_var.ptr[0]>=(gs_dis_param.auto_item[0]))||(gs_dis_ctl_var.ptr[0]>DIS_MAX_ITEM))  //项目为0~6,共7项 //
        {
            gs_dis_ctl_var.ptr[0]=0;
        }

        item = gs_dis_param.auto_item[gs_dis_ctl_var.ptr[0]+1];  //  0位置为显示项目个数，1位置开始为项目 //
        if(item>DIS_MAX_ITEM)   
        {
            item =     DIS_DEFAULT_ITEM;
        }
    }
    return (item);
}

//*****************************************************************
//  Function:       INT8U Link_Get_DisIndex(u8_dis_itm)        
//  Description:    获取显示项目信息                                        
//  Input:                                                                            
//  Output:         no                              
//  Return:         no                              
//  Others:         
//***************************************************************/
INT8U Link_Get_DisTab_Var(INT8U u8_dis_itm)
{
 INT8U i;
 INT8U u8_result=FALSE;

	if(u8_dis_itm>DIS_MAX_ITEM)
	{
		u8_dis_itm = DIS_DEFAULT_ITEM;    // 系统显示项目异常后的默认为显示系统错误屏  //
	}


	for( i = 0 ; i <=DIS_MAX_ITEM ; i++ )
	{
		if( u8_dis_itm== LcdDisTab[i].DisID)
		{
			gs_LCDDISTAB_var.DisID = u8_dis_itm;
			gs_LCDDISTAB_var.ObisNum= LcdDisTab[i].ObisNum;
			gs_LCDDISTAB_var.Addr =LcdDisTab[i].Addr;
			gs_LCDDISTAB_var.DisInfo = LcdDisTab[i].DisInfo;
			gs_LCDDISTAB_var.chartype = LcdDisTab[i].chartype;
			gs_LCDDISTAB_var.obis_digit= LcdDisTab[i].obis_digit;
			u8_result = TRUE;
			return (u8_result);
		}
	}

	if(u8_result == FALSE )
	{
		gs_LCDDISTAB_var.DisID = DIS_DEFAULT_ITEM;  // 默认显示当前有功总 
		gs_LCDDISTAB_var.ObisNum = LcdDisTab[0x01].ObisNum;
		gs_LCDDISTAB_var.Addr =LcdDisTab[0x01].Addr;
		gs_LCDDISTAB_var.DisInfo = LcdDisTab[0x01].DisInfo;
		gs_LCDDISTAB_var.chartype = LcdDisTab[0x01].chartype;		
	}
	return (u8_result);
			
}


//*****************************************************************
//  Function:      void Link_Get_Dis_OBIS_Num_buff(void)      
//  Description:    获取OBIS区的显示内容， 2个8字区                                       
//  Input:                                                                            
//  Output:         no                              
//  Return:         no                              
//  Others:         dis_obis_buff[0] 对应OBIS区显示的最左边1位(高位)
//***************************************************************/
void Link_Get_Dis_OBIS_Num_buff(void)
{ 
	INT8U	u8_len,u8_mem_type,lib_len;
	ST_U32_U08	u32_tmp,buffer;
	
	u8_len = (INT8U)gs_LCDDISTAB_var.DisInfo;
	
	if(u8_len>2) u8_len= 2;
	u8_mem_type =  (INT8U)(gs_LCDDISTAB_var.DisInfo>>8);
	//检测到拉闸后 ，OBIS区固定显示oFF yang 2017-12-27// 
	if(gs_relay_manage_var.u8_STA== CMD_RELAY_ON)
	{
		gs_dis_pixel_var.dis_obis_buff[0] = 0x36 ;	//o
		gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[0x0F]; //F
		gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[0x0F];//F
		return;
	}
	// 数据获取处理  //
	switch (gs_LCDDISTAB_var.chartype)
	{
		case CHAR_V://数据前移1位
					mem_read(&u32_tmp.B08[2],gs_LCDDISTAB_var.Addr, 2, u8_mem_type);		//获取当前数据 //
					Lib_word_bcd2(&buffer.B08[0],u32_tmp.W16[1]);		   //  16进制转10进制// 
			break;
			
		case CHAR_HZ:
				mem_read(&u32_tmp.B08[2],gs_LCDDISTAB_var.Addr, 2, u8_mem_type);		//获取当前数据 //
				Lib_word_bcd2(&buffer.B08[0],u32_tmp.W16[1]);		   //  16进制转10进制// 
				gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P9; 
			break;
			
		case CHAR_PF:
					mem_read(&u32_tmp.B08[2],gs_LCDDISTAB_var.Addr, 2, u8_mem_type);		//获取当前数据 //
					Lib_word_bcd2(&buffer.B08[0],u32_tmp.W16[1]);		   //  16进制转10进制// 
					gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P8; 
			break;

		case CHAR_A: //xxx.xxx A
					mem_read(&u32_tmp.B08[0],gs_LCDDISTAB_var.Addr, 4, u8_mem_type);		//获取当前数据 //
					Lib_long_bcd4(&buffer.B08[0],u32_tmp.u32);			//	16进制转10进制// 
					lib_len=Lib_ENOB_Get(&buffer.B08[0],4,1);
					if(lib_len<=4)
					{
						buffer.u32 =((buffer.u32&0x0000FFFF)<<16);
						gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P8; 
					}
					else if(lib_len==5)
					{
						buffer.u32 =((buffer.u32&0x000FFFFF)<<12);
						gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P9; 
					}
					else if(lib_len==6)
					{
						buffer.u32 =((buffer.u32&0x00FFFFFF)<<8);
					}
					else if(lib_len==7)
					{
						buffer.u32 =((buffer.u32&0x0FFFFFFF)<<4);
					}
					else 
					{
						buffer.u32 =(buffer.u32&0xFFFFFFFF);
					}
			break;			
		case CHAR_KW:// xx.xxxx kW
					mem_read(&u32_tmp.B08[0],gs_LCDDISTAB_var.Addr, 4, u8_mem_type);		//获取当前数据 //
					Lib_long_bcd4(&buffer.B08[0],u32_tmp.u32);			//	16进制转10进制// 
					lib_len=Lib_ENOB_Get(&buffer.B08[0],4,1);
					if(lib_len<=5)
					{
						buffer.u32 =((buffer.u32&0x000FFFFF)<<12);
						gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P8; 
					}
					else if(lib_len==6)
					{
						buffer.u32 =((buffer.u32&0x00FFFFFF)<<8);
						gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P9; 
					}
					else if(lib_len==7)
					{
						buffer.u32 =((buffer.u32&0x0FFFFFFF)<<4);
					}
					else 
					{
						buffer.u32 =(buffer.u32&0xFFFFFFFF);
					}
			break;			

					
		default:
			mem_read(&buffer.B08[0],gs_LCDDISTAB_var.Addr, u8_len, u8_mem_type);		//获取当前数据 //
			break;
	}
	// 测试/// 0XE6
	//	buffer.B08[3] = 0XE6;
	//	buffer.B08[2] = 0x07;
	//	buffer.B08[1] = 0x42;
	//	buffer.B08[0] = 0x48;
	//////////////
	
		switch(u8_len)
		{
			case 2:
				gs_dis_pixel_var.dis_obis_buff[0] = NumSegD7_D9[(buffer.B08[0]&0xF0)>>4] ;  
				gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[(buffer.B08[0]&0x0F)]; 
				gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[(buffer.B08[1]&0xF0)>>4] ;  
				break;
			case 1:
				gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[(buffer.B08[1]&0xF0)>>4] ;  
				gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[(buffer.B08[1]&0x0F)]; 
				break;
			default:	  // 长度为0 显示空
				   break;
		}
		

}		
		


//*****************************************************************
//  Function:      void Link_Get_Dis_OBIS_Num_buff(void)      
//  Description:    获取DATA区的显示内容， 8个8字区                                       
//  Input:                                                                            
//  Output:         no                              
//  Return:         no                              
//  Others:         dis_data_buff[0] 对应NUM区显示的最左边1位(高位)
//***************************************************************/
void Link_Get_Dis_Data_Num_buff(void)
{
	ST_U32_U08   buffer;
	INT8U  temp[6];
	NEW_MMD_DATA_UNIT t;
	RTC_TYPE rtc_data_tmp;
	////上排固定显示当前总电量 yana 2017-12-15////	2018-11-20改为上排显示可变cxy

	if(dis_data_type == CHAR_TIME && dis_data_sec != 0)
	{	//显示时间
		Get_RTCTime(&rtc_data_tmp);

		gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[rtc_data_tmp.Hour>>4];
		gs_dis_pixel_var.dis_data_buff[1] = NumSegD1_D6[rtc_data_tmp.Hour&0x0F];
		gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[rtc_data_tmp.Minute>>4];
		gs_dis_pixel_var.dis_data_buff[3] = NumSegD1_D6[rtc_data_tmp.Minute&0x0F];
		gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[rtc_data_tmp.Second>>4];
		gs_dis_pixel_var.dis_data_buff[5] = NumSegD1_D6[rtc_data_tmp.Second&0x0F];
		dis_data_sec --;
		if(dis_data_sec == 0){dis_data_type = 0;}
	}
	else
	{	
		if(dis_num_char == 0)
		{
			//显示当前电量 kwh
			mem_read(&temp[0], (INT16U)&gs_energy_user_data.us_val[EC_CH_AC_PT_TT][0].buf[0], LEN_EC_UNIT, MEM_RAM);	  //获取当前数据 //
			//mem_read(&temp[0], ADR_BLOCK00_EC_L0_E2P, 6, MEM_E2P1);
			api_get_energy_LCD(&temp[0], gs_dis_param.ec_form,&buffer.B08[0]);
			//debug////
			//buffer.B08[1]=0x12;
			//buffer.B08[2]=0x34;
			//buffer.B08[3]=0x56;
			gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[(buffer.B08[1]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_data_buff[1] = NumSegD1_D6[(buffer.B08[1]&0x0F)];
			gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[(buffer.B08[2]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_data_buff[3] = NumSegD1_D6[(buffer.B08[2]&0x0F)]; 
			gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[(buffer.B08[3]&0xF0)>>4] ;  
			if(gs_dis_param.ec_form!=0x50)
			{
				gs_dis_pixel_var.dis_data_buff[5] = NumSegD1_D6[(buffer.B08[3]&0x0F)]; 
			}
			//下排显示  2019-01-11
			//gs_dis_pixel_var.dis_obis_buff[0] = NumSegD7_D9[(buffer.B08[0]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[0]; 
			gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[1] ;
		}
		if(dis_num_char == 1)
		{
			//显示当前需量kw
			//mem_read(&temp[0], (INT16U)&gs_energy_user_data.us_val[EC_CH_AC_PT_TT][0].buf[0], LEN_EC_UNIT, MEM_RAM);	  //获取当前数据 //
			mem_read(&t, ADR_BLOCK_4MMD_L0_E2P, 4, MEM_E2P1);				    
			Lib_long_bcd4(&buffer.B08[0],t.val.val_hex32);
//			mem_read(&buffer.B08[0], ADR_BLOCK_4MMD_L0_E2P, 4, MEM_E2P1);
//			api_get_energy_LCD(&temp[0], gs_dis_param.ec_form,&buffer.B08[0]);
			//debug////
			//buffer.B08[1]=0x12;
			//buffer.B08[2]=0x34;
			//buffer.B08[3]=0x56;
			gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[0] ;  
			gs_dis_pixel_var.dis_data_buff[1] = NumSegD1_D6[(buffer.B08[0]&0xF0)>>4];
			gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[(buffer.B08[0]&0x0F)] ;  
			gs_dis_pixel_var.dis_data_buff[3] = NumSegD1_D6[(buffer.B08[1]&0xF0)>>4]; 
			gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[(buffer.B08[1]&0x0F)] ;  
			if(gs_dis_param.ec_form!=0x50)
			{
				gs_dis_pixel_var.dis_data_buff[5] = NumSegD1_D6[(buffer.B08[2]&0xF0)>>4]; 
			}

			//下排显示  2019-01-11
			//gs_dis_pixel_var.dis_obis_buff[0] = NumSegD7_D9[(buffer.B08[0]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[0]; 
			gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[2] ;
		}
		if(dis_num_char == 2)
		{
			//显示当前累计需量kw
//			mem_read(&temp[0], (INT16U)&gs_energy_user_data.us_val[EC_CH_AC_PT_TT][0].buf[0], LEN_EC_UNIT, MEM_RAM);	  //获取当前数据 //
			mem_read(&t, ADR_BLOCK_4MMD_sum_L0_E2P, 4, MEM_E2P1);
			Lib_long_bcd4(&buffer.B08[0],t.val.val_hex32);
//			api_get_energy_LCD(&temp[0], gs_dis_param.ec_form,&buffer.B08[0]);
			//debug////
			//buffer.B08[1]=0x12;
			//buffer.B08[2]=0x34;
			//buffer.B08[3]=0x56;
			gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[0] ;  
			gs_dis_pixel_var.dis_data_buff[1] = NumSegD1_D6[(buffer.B08[0]&0xF0)>>4];
			gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[(buffer.B08[0]&0x0F)] ;  
			gs_dis_pixel_var.dis_data_buff[3] = NumSegD1_D6[(buffer.B08[1]&0xF0)>>4]; 
			gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[(buffer.B08[1]&0x0F)] ;  
			if(gs_dis_param.ec_form!=0x50)
			{
				gs_dis_pixel_var.dis_data_buff[5] = NumSegD1_D6[(buffer.B08[2]&0xF0)>>4]; 
			}

			//下排显示  2019-01-11
			//gs_dis_pixel_var.dis_obis_buff[0] = NumSegD7_D9[(buffer.B08[0]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[0]; 
			gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[4] ;
		}
		if(dis_num_char == 3)
		{
			//显示前一天电量kwh
//			mem_read(&temp[0], ADR_BLOCK_4Energy_L0_E2P+LEN_BLOCK_4Energy_E2P/4, LEN_BLOCK_4Energy_E2P/4, MEM_E2P1);	  //获取当前数据 //
			mem_read(&temp[0], &energy_data_array[1].buf[0], LEN_BLOCK_4Energy_E2P/4,MEM_RAM);
			api_get_energy_LCD(&temp[0], gs_dis_param.ec_form,&buffer.B08[0]);
			//debug////
			//buffer.B08[1]=0x12;
			//buffer.B08[2]=0x34;
			//buffer.B08[3]=0x56;
			gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[(buffer.B08[1]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_data_buff[1] = NumSegD1_D6[(buffer.B08[1]&0x0F)];
			gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[(buffer.B08[2]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_data_buff[3] = NumSegD1_D6[(buffer.B08[2]&0x0F)]; 
			gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[(buffer.B08[3]&0xF0)>>4] ;  
			if(gs_dis_param.ec_form!=0x50)
			{
				gs_dis_pixel_var.dis_data_buff[5] = NumSegD1_D6[(buffer.B08[3]&0x0F)]; 
			}

			//下排显示  2019-01-11
			//gs_dis_pixel_var.dis_obis_buff[0] = NumSegD7_D9[(buffer.B08[0]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[2]; 
			gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[1] ;
		}
		if(dis_num_char == 4)
		{
			//显示前一天需量kw
//			mem_read(&temp[0], (INT16U)&gs_energy_user_data.us_val[EC_CH_AC_PT_TT][0].buf[0], LEN_EC_UNIT, MEM_RAM);	  //获取当前数据 //
			mem_read(&t, ADR_BLOCK_4MMD_L0_E2P+LEN_NEW_MMD_UNIT, 4, MEM_E2P1);
			Lib_long_bcd4(&buffer.B08[0],t.val.val_hex32);
//			api_get_energy_LCD(&temp[0], gs_dis_param.ec_form,&buffer.B08[0]);
			//debug////
			//buffer.B08[1]=0x12;
			//buffer.B08[2]=0x34;
			//buffer.B08[3]=0x56;
			gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[0] ;  
			gs_dis_pixel_var.dis_data_buff[1] = NumSegD1_D6[(buffer.B08[0]&0xF0)>>4];
			gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[(buffer.B08[0]&0x0F)] ;  
			gs_dis_pixel_var.dis_data_buff[3] = NumSegD1_D6[(buffer.B08[1]&0xF0)>>4]; 
			gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[(buffer.B08[1]&0x0F)] ;  
			if(gs_dis_param.ec_form!=0x50)
			{
				gs_dis_pixel_var.dis_data_buff[5] = NumSegD1_D6[(buffer.B08[2]&0xF0)>>4]; 
			}

			//下排显示  2019-01-11
			//gs_dis_pixel_var.dis_obis_buff[0] = NumSegD7_D9[(buffer.B08[0]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[2]; 
			gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[2] ;
		}
		if(dis_num_char == 5)
		{
			//显示前一天累计需量kw
//			mem_read(&temp[0], (INT16U)&gs_energy_user_data.us_val[EC_CH_AC_PT_TT][0].buf[0], LEN_EC_UNIT, MEM_RAM);	  //获取当前数据 //
			mem_read(&t, ADR_BLOCK_4MMD_sum_L0_E2P+LEN_SUM_MMD_UNIT, 4, MEM_E2P1);
			Lib_long_bcd4(&buffer.B08[0],t.val.val_hex32);
//			api_get_energy_LCD(&temp[0], gs_dis_param.ec_form,&buffer.B08[0]);
			//debug////
			//buffer.B08[1]=0x12;
			//buffer.B08[2]=0x34;
			//buffer.B08[3]=0x56;
			gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[0] ;  
			gs_dis_pixel_var.dis_data_buff[1] = NumSegD1_D6[(buffer.B08[0]&0xF0)>>4];
			gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[(buffer.B08[0]&0x0F)] ;  
			gs_dis_pixel_var.dis_data_buff[3] = NumSegD1_D6[(buffer.B08[1]&0xF0)>>4]; 
			gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[(buffer.B08[1]&0x0F)] ;  
			if(gs_dis_param.ec_form!=0x50)
			{
				gs_dis_pixel_var.dis_data_buff[5] = NumSegD1_D6[(buffer.B08[2]&0xF0)>>4]; 
			}

			//下排显示  2019-01-11
			//gs_dis_pixel_var.dis_obis_buff[0] = NumSegD7_D9[(buffer.B08[0]&0xF0)>>4] ;  
			gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[2]; 
			gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[4] ;
		}
	}
}
//*****************************************************************
//  Function:       void Link_Get_Dis_Char_buff(void) 
//  Description:    获取显示的配合字符信息                                      
//  Input:                                                                            
//  Output:         no                              
//  Return:         no                              
//  Others:         
//***************************************************************/
void Link_Get_Dis_Char_buff(void)
{
//	RTC_TYPE rtc_data_tmp;
//	Get_RTCTime(&rtc_data_tmp);//获取时间显示费率
//	if(rtc_data_tmp.Hour)
	switch (gs_current_triff_var.triff_No)//显示费率
	{
		case 1:
			gs_dis_pixel_var.dis_char_buff[1] |= CHAR_DATA_S1;
			break;
		case 2:
			gs_dis_pixel_var.dis_char_buff[1] |= CHAR_DATA_S2;
			break;
		case 3:
			gs_dis_pixel_var.dis_char_buff[1] |= CHAR_DATA_S3;
			break;
		case 4:
			gs_dis_pixel_var.dis_char_buff[1] |= CHAR_DATA_S4;
			break;
	}
	if(dis_data_type == CHAR_TIME)
	{
		gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P1P2;
		gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P4P5;
	}
	else
	{	
		if(dis_num_char == 0 || dis_num_char == 3)
		{
			if(gs_dis_param.ec_form!=0x50)
			{
				gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P6;  //
			}
			gs_dis_pixel_var.dis_char_buff[1] |= CHAR_DATA_kWh;  //kWh
		}
		else
		{
			if(gs_dis_param.ec_form!=0x50)
			{
				gs_dis_pixel_var.dis_char_buff[0] |= CHAR_DATA_P6;  //
			}
			gs_dis_pixel_var.dis_char_buff[1] |= CHAR_DATA_kW;  //kW
			gs_dis_pixel_var.dis_char_buff[2] |= CHAR_DATA_MD;
		}
	}
	if(gs_relay_manage_var.u8_STA== CMD_RELAY_ON)
	{
		return;
	}
	if(gs_dis_ctl_var.item== DIS_DATA_CLR)
	{
		return;
	}

	// DATA 区的小数点和单位字符获取 //
//	switch(gs_LCDDISTAB_var.chartype)
	switch(gs_LCDDISTAB_var.chartype = 10)
	{
	 	case CHAR_V:	// 电压显示 xxx V //
			gs_dis_pixel_var.dis_char_buff[2] |= CHAR_DATA_V; 
			break;
		case CHAR_A:	// 电流显示 xx.x A//
			gs_dis_pixel_var.dis_char_buff[2] |= CHAR_DATA_A; 
			break;
		case CHAR_KW:	 // 功率显示 xx.x kw//
		    gs_dis_pixel_var.dis_char_buff[2] |= (CHAR_DATA_k+CHAR_DATA_W);  //kW
			break;
		case CHAR_HZ:	// 频率显示 xx.x HZ//
		    gs_dis_pixel_var.dis_char_buff[2] |= CHAR_DATA_Hz;  //Hz
			break;
		default:
			break;
	}	

			
}

//*****************************************************************
//  Function:       INT8U Link_Get_DisIndex(u8_dis_itm)        
//  Description:    获取显示项目信息                                        
//  Input:                                                                            
//  Output:         no                              
//  Return:         no                              
//  Others:         
//***************************************************************/
void Link_Get_Dis_RealChar_buff(void)
{

	 if(gs_measure_status_judge_var.u8_status & VBAT_LOW)           gs_dis_pixel_var.dis_char_buff[2] |=CHAR_DATA_S9;	//电池欠压
	 if(gs_emu_run_var.net_flg&REV_TT_ACPOWER)
	{
		gs_dis_pixel_var.dis_char_buff[1] |=CHAR_DATA_Received;//反向提示
	}
	 else
	{
		 gs_dis_pixel_var.dis_char_buff[1] |=CHAR_DATA_Delivered;// 默认正向显示
 	}
}


//*****************************************************************
//  Function:      void Link_Get_Dis_Drv_buff(void)   
//  Description:    获取显示缓冲buff的数据                                    
//  Input:                                                                            
//  Output:         no                              
//  Return:         no                              
//  Others:         
//***************************************************************/
void Link_Get_Dis_Drv_buff(void)
{
	gs_dis_pixel_var.dis_buff[0] |= (gs_dis_pixel_var.dis_data_buff[0] &0x7F);
	gs_dis_pixel_var.dis_buff[1] |= (gs_dis_pixel_var.dis_data_buff[1] &0x7F);
	gs_dis_pixel_var.dis_buff[2] |= (gs_dis_pixel_var.dis_data_buff[2] &0x7F);
	gs_dis_pixel_var.dis_buff[3] |= (gs_dis_pixel_var.dis_data_buff[3] &0x7F);
	gs_dis_pixel_var.dis_buff[4] |= (gs_dis_pixel_var.dis_data_buff[4] &0x7F);
	gs_dis_pixel_var.dis_buff[5] |= (gs_dis_pixel_var.dis_data_buff[5] &0x7F);

	gs_dis_pixel_var.dis_buff[6] |= (gs_dis_pixel_var.dis_obis_buff[0] &0xEF);
	gs_dis_pixel_var.dis_buff[7] |= (gs_dis_pixel_var.dis_obis_buff[1] &0xEF);
	gs_dis_pixel_var.dis_buff[8] |= (gs_dis_pixel_var.dis_obis_buff[2] &0xEF);

	
	 //  dis_var.dis_char_buff[0:2] 的转换//
	if(gs_dis_pixel_var.dis_char_buff[0] &CHAR_DATA_P1P2)	{gs_dis_pixel_var.dis_buff[1] |= BIT7;gs_dis_pixel_var.dis_buff[2] |= BIT7;}
	if(gs_dis_pixel_var.dis_char_buff[0] &CHAR_DATA_P3)   	gs_dis_pixel_var.dis_buff[3] |= BIT7;
	if(gs_dis_pixel_var.dis_char_buff[0] &CHAR_DATA_P4P5)   {gs_dis_pixel_var.dis_buff[4] |= BIT7;gs_dis_pixel_var.dis_buff[9] |= BIT7;}
	if(gs_dis_pixel_var.dis_char_buff[0] &CHAR_DATA_P6)     gs_dis_pixel_var.dis_buff[5] |= BIT7;
	if(gs_dis_pixel_var.dis_char_buff[0] &CHAR_DATA_P7)     gs_dis_pixel_var.dis_buff[6] |= BIT4;
	if(gs_dis_pixel_var.dis_char_buff[0] &CHAR_DATA_P8)     gs_dis_pixel_var.dis_buff[7] |= BIT4;
	if(gs_dis_pixel_var.dis_char_buff[0] &CHAR_DATA_P9)   	gs_dis_pixel_var.dis_buff[8] |= BIT4;
	
	if(gs_dis_pixel_var.dis_char_buff[1] &CHAR_DATA_kWh)   	{gs_dis_pixel_var.dis_buff[9] |= BIT6;gs_dis_pixel_var.dis_buff[9] |= BIT2;}
	if(gs_dis_pixel_var.dis_char_buff[1] &CHAR_DATA_kW)   	gs_dis_pixel_var.dis_buff[9] |= BIT6;
	if(gs_dis_pixel_var.dis_char_buff[1] &CHAR_DATA_S1)   	gs_dis_pixel_var.dis_buff[9] |= BIT4;
	if(gs_dis_pixel_var.dis_char_buff[1] &CHAR_DATA_S2)   	gs_dis_pixel_var.dis_buff[9] |= BIT0;
	if(gs_dis_pixel_var.dis_char_buff[1] &CHAR_DATA_S3)   	gs_dis_pixel_var.dis_buff[9] |= BIT5;
	if(gs_dis_pixel_var.dis_char_buff[1] &CHAR_DATA_S4)   	gs_dis_pixel_var.dis_buff[9] |= BIT1;

	if(gs_dis_pixel_var.dis_char_buff[1] &CHAR_DATA_Received)   	gs_dis_pixel_var.dis_buff[10] |= BIT7;
	if(gs_dis_pixel_var.dis_char_buff[1] &CHAR_DATA_Delivered)   	gs_dis_pixel_var.dis_buff[10] |= BIT6;

	if(gs_dis_pixel_var.dis_char_buff[2] &CHAR_DATA_k)		gs_dis_pixel_var.dis_buff[10] |= BIT0;
	if(gs_dis_pixel_var.dis_char_buff[2] &CHAR_DATA_W)		
	{
		gs_dis_pixel_var.dis_buff[10] |= BIT1;
		gs_dis_pixel_var.dis_buff[10] |= BIT2;
		gs_dis_pixel_var.dis_buff[11] |= BIT6;
		gs_dis_pixel_var.dis_buff[11] |= BIT7;
	}
	if(gs_dis_pixel_var.dis_char_buff[2] &CHAR_DATA_V)		{gs_dis_pixel_var.dis_buff[10] |= BIT1;gs_dis_pixel_var.dis_buff[10] |= BIT2;}
	if(gs_dis_pixel_var.dis_char_buff[2] &CHAR_DATA_A)		
	{
		gs_dis_pixel_var.dis_buff[11] |= BIT7;
		gs_dis_pixel_var.dis_buff[10] |= BIT2;
		gs_dis_pixel_var.dis_buff[10] |= BIT3;
	}
	if(gs_dis_pixel_var.dis_char_buff[2] &CHAR_DATA_Hz)		gs_dis_pixel_var.dis_buff[11] |= BIT5;
	if(gs_dis_pixel_var.dis_char_buff[2] &CHAR_DATA_PF)		gs_dis_pixel_var.dis_buff[11] |= BIT4;
	if(gs_dis_pixel_var.dis_char_buff[2] &CHAR_DATA_MD)		gs_dis_pixel_var.dis_buff[10] |= BIT4;
	if(gs_dis_pixel_var.dis_char_buff[2] &CHAR_DATA_S9)		gs_dis_pixel_var.dis_buff[10] |= BIT5;
	
}


/*****************************************************************************
** Function name    :api_clr_current_energy_data
**
** Description         : 需量电量数据转换
**
** Parameters         :  type 数据类型     0默认全整数 
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_get_energy_LCD(uint8*  temp, uint8 type, uint8*  result)
{
 ST_U32_U08   u32_result, BCD_temp;
 US_EC_DATA_UNIT ec_tmp;
   
    // if(type!=0x50)     type= 0x51;
    //电量格式数据赋值 //
    Lib_Copy_Str_TwoArry(&ec_tmp.buf[0], temp, LEN_EC_UNIT);
	
    u32_result.u32 =0;

	if(type==0x80)
    {
        if(ec_tmp.val.integ_hex32>=100000000)   ec_tmp.val.integ_hex32 -=100000000;
        Lib_long_bcd4(&u32_result.B08[0],ec_tmp.val.integ_hex32);       
    }
    
    if(type==0x62)
    {
        // 整数部分处理 //
        if(ec_tmp.val.integ_hex32>=1000000)   ec_tmp.val.integ_hex32 -=1000000;
        Lib_long_bcd4(&BCD_temp.B08[0],ec_tmp.val.integ_hex32);
        u32_result.B08[0]= BCD_temp.B08[1];
        u32_result.B08[1]= BCD_temp.B08[2];
        u32_result.B08[2]= BCD_temp.B08[3];

         // 小数部分处理 //
         ec_tmp.val.decim_hex16  /= METER_CONST_10WH;
         u32_result.B08[3] = Lib_byte_bcd((INT8U)ec_tmp.val.decim_hex16);

    }
    if(type==0x60)
    {
        if(ec_tmp.val.integ_hex32>=1000000)   ec_tmp.val.integ_hex32 -=1000000;
        Lib_long_bcd4(&u32_result.B08[0],ec_tmp.val.integ_hex32);       
    }
    
    if(type==0x61)
    {
        // 整数部分处理 //
        if(ec_tmp.val.integ_hex32>=1000000)   ec_tmp.val.integ_hex32 -=1000000;
        Lib_long_bcd4(&BCD_temp.B08[0],ec_tmp.val.integ_hex32);
        u32_result.B08[0]= BCD_temp.B08[1];
        u32_result.B08[1]= BCD_temp.B08[2];
        u32_result.B08[2]= BCD_temp.B08[3];

         // 小数部分处理 //
         ec_tmp.val.decim_hex16  /= METER_CONST_10WH;
         u32_result.B08[3] = Lib_byte_bcd((INT8U)ec_tmp.val.decim_hex16);

    }
    if(type==0x51)
    {
		// 整数部分处理 //
		if(ec_tmp.val.integ_hex32>=100000)   ec_tmp.val.integ_hex32 -=100000;
		Lib_long_bcd4(&BCD_temp.B08[0],ec_tmp.val.integ_hex32);
		u32_result.B08[0]= 0x00;
		u32_result.B08[1]= (((BCD_temp.B08[1]&0x0F)<<4)+((BCD_temp.B08[2]&0xF0)>>4));
		u32_result.B08[2]= (((BCD_temp.B08[2]&0x0F)<<4)+((BCD_temp.B08[3]&0xF0)>>4));
		u32_result.B08[3]|= ((BCD_temp.B08[3]&0x0F)<<4);

		// 小数部分处理 //
		ec_tmp.val.decim_hex16  /= METER_CONST_10WH;
		u32_result.B08[3]|=(((Lib_byte_bcd((INT8U)ec_tmp.val.decim_hex16))&0xF0)>>4);
    }
    if(type==0x50)
    {
		// 整数部分处理 //
		if(ec_tmp.val.integ_hex32>=100000)   ec_tmp.val.integ_hex32 -=100000;
		Lib_long_bcd4(&BCD_temp.B08[0],ec_tmp.val.integ_hex32);
		u32_result.B08[0]= 0x00;
		u32_result.B08[1]= (((BCD_temp.B08[1]&0x0F)<<4)+((BCD_temp.B08[2]&0xF0)>>4));
		u32_result.B08[2]= (((BCD_temp.B08[2]&0x0F)<<4)+((BCD_temp.B08[3]&0xF0)>>4));
		u32_result.B08[3]|= ((BCD_temp.B08[3]&0x0F)<<4);
    }
    //数据输出 //
    Lib_Copy_Str_TwoArry(result, &u32_result.B08[0], 4);

}


/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :每秒更新液晶显示项目       
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_chg_LCDDisplay_adj_item(uint8 u8_item)
{
    if((u8_item>=DIS_ADJ_CH1_0) &&(u8_item<=DIS_DATA_BROADCAST))
    {
        //更改校准显示项目//
        gs_dis_ctl_var.mode = DIS_MODE_NOMAL;
		if(u8_item==DIS_DATA_BROADCAST)
		{
	        gs_dis_ctl_var.keep_sec[0] = 10;    //强制显示10秒 //
		}
		else
		{
	        gs_dis_ctl_var.keep_sec[0] = 5;    //强制显示5秒 //
		}
		//gs_dis_ctl_var.keep_sec[0] = 2;
        gs_dis_ctl_var.ptr[0] = 0;
        gs_dis_ctl_var.item = u8_item;    
    }
    else
    {
        //异常情况下显示自动循显项目 //
        gs_dis_ctl_var.mode = DIS_MODE_NOMAL;
        gs_dis_ctl_var.keep_sec[0] = gs_dis_param.auto_sec;
        gs_dis_ctl_var.ptr[0] = 0;
        gs_dis_ctl_var.item = gs_dis_param.auto_item[gs_dis_ctl_var.ptr[0]+1];  //  0位置为显示项目个数，1位置开始为项目 //  
    }

    api_updated_LCDDisplayPixel_per_second();
}

/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :每秒更新液晶显示项目       
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_LCDDisplay_adj_item(uint8 u8_item)
{
	INT8U adr[6];
    if((u8_item<DIS_ADJ_CH1_0) &&(u8_item>DIS_DATA_BROADCAST))
    {
        return;
    }

    Lib_Set_String(&gs_dis_pixel_var.dis_data_buff[0],0,DS_DATANum);	
    Lib_Set_String(&gs_dis_pixel_var.dis_obis_buff[0],0,DS_OBISNum);
    Lib_Set_String(&gs_dis_pixel_var.dis_char_buff[0] ,0,DS_CHARNum);

    // 数据获取处理  //A F E D B G C MD  //
    switch (u8_item)
    {
        case DIS_ADJ_CH1_0:	//L 回路校表参数初始化 //
        case DIS_ADJ_CH1_1:	//L 回路校表参数初始化 //
        case DIS_ADJ_CH1_2:	//L 回路校表参数初始化 //
        case DIS_ADJ_CH1_3:	//L 回路校表参数初始化 //
            gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[0X0C]	;      //LCD-D1  C  //
            gs_dis_pixel_var.dis_data_buff[1] = 0x67	;               //LCD-D2  H  //
            gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[0X01]	;     //LCD-D3  1  //
            gs_dis_pixel_var.dis_data_buff[3] = 0X02	; 	          //LCD-D4  -//
            gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[(u8_item&0X0F)]	;  //LCD-D5  0~3  //
        break;

        case DIS_ADJ_CH2_0:	//L 回路校表参数初始化 //
        case DIS_ADJ_CH2_1:	//L 回路校表参数初始化 //
        case DIS_ADJ_CH2_2:	//L 回路校表参数初始化 //
        case DIS_ADJ_CH2_3:	//L 回路校表参数初始化 //
            gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[0X0C]	;      //LCD-D1  C  //
            gs_dis_pixel_var.dis_data_buff[1] = 0x67	;                                //LCD-D2  H  //
            gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[0X02]	;     //LCD-D3  2  //
            gs_dis_pixel_var.dis_data_buff[3] = 0X02	; 	                          //LCD-D4  -//
            gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[(u8_item&0X0F)-4]	;     //LCD-D5  0~3  //
        break;

        case DIS_DATA_CLR:	//清零显示 //
            gs_dis_pixel_var.dis_obis_buff[0] = NumSegD7_D9[0X0C]	;      //LCD-D1  C  //
            gs_dis_pixel_var.dis_obis_buff[1] = 0x1A	;              //LCD-D2  L  //
            gs_dis_pixel_var.dis_obis_buff[2] = 0x06	;              //LCD-D3  r  //
        break;
		
        case DIS_DATA_BROADCAST:	//广播命令显示通讯地址 //
			mem_read(&adr[0], ADR_METER_PARAM1_RF_COMM_ID, 6, MEM_E2P1);//读取RF通讯地址
            gs_dis_pixel_var.dis_data_buff[0] = NumSegD1_D6[(adr[1]&0x0F)];  
            gs_dis_pixel_var.dis_data_buff[1] = NumSegD1_D6[(adr[2]&0xF0)>>4];
            gs_dis_pixel_var.dis_data_buff[2] = NumSegD1_D6[(adr[2]&0x0F)];
            gs_dis_pixel_var.dis_data_buff[3] = NumSegD1_D6[(adr[3]&0xF0)>>4];
            gs_dis_pixel_var.dis_data_buff[4] = NumSegD1_D6[(adr[3]&0x0F)];
            gs_dis_pixel_var.dis_data_buff[5] = NumSegD1_D6[(adr[4]&0xF0)>>4];
            gs_dis_pixel_var.dis_obis_buff[0] = NumSegD7_D9[(adr[4]&0x0F)];
            gs_dis_pixel_var.dis_obis_buff[1] = NumSegD7_D9[(adr[5]&0xF0)>>4];
            gs_dis_pixel_var.dis_obis_buff[2] = NumSegD7_D9[(adr[5]&0x0F)];
        break;
        default:	
            sys_err();
        break;
    }
 
}


/***************************************************************
*    END
****************************************************************/









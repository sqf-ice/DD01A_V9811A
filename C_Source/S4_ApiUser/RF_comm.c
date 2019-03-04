/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: RF_comm.c
**Author: maji
**date: 
**description: c code for lora RF communicate
**note: memer type  DI03B
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2017-09-06   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/
#include <MyIncludes_H.h>
/*#include "..\myInclude\myInclude.h"
#include "..\hardDriver\hardDriver.h"
#include "..\myLib\myLib.h"
#include "data.h"*/


RF_DRV_VAR gs_rf_drv_var;
uint8 *P_DL645_07_BUFF;    //通讯缓冲区指针 //
DLT645_07_VAR gs_dlt645_07_var;



code INT8U Super_mima_DLT[4] ={0X00,0X23,0X61,0X45};  //超级密码，00级，456123 //



/************************************************************************************************************
* DL/T 645-2007  部分代码
*************************************************************************************************************/
/*******************************************************************
* uchar MeteIDCheck():
********************************************************************/
INT8U MeteIDCheck(INT8U *ptr)
{
	 register INT8U i;
	 INT8U adr[6];

	 //  //
	 mem_read(&adr[0], ADR_METER_PARAM1_RF_COMM_ID, 6, MEM_E2P1);

	 //
	 for(i=6; i>0; i--)
	 {
		 if( *(ptr-i) !=0x99)  break;
	 }
	 if(i==0)		return(TRUE); 
 
	 //
	 for(i=6; i>0; i--)
	 {
		if( ( *(ptr-i) != 0xaa ) && ( *(ptr-i) != adr[6-i] ) ) 
		{
			return(FALSE);
		} 
	 }	 
	 return(TRUE); 
}

/*****************************************************************
密码检查
*****************************************************************/
INT8U DLT_SecurityCK(INT8U *ptr)
{
    INT8U ary[4];
	
    mem_read(&ary[0], ADR_METER_PARAM1_PW1, 4, MEM_E2P1);  //get level 1 password   //
	if ( Lib_Cmp_TwoArry( ptr+DLT_MIMAPAPtr, &ary[0], 4) ==0)  return (mmok);
	if ( Lib_Cmp_TwoArry( ptr+DLT_MIMAPAPtr, &Super_mima_DLT[0], 4) ==0)  return (mmok);
	return(mmerr);
}

/*****************************************************************
DLT645 C=0X11 读命令，返回成功结果是数据域已经填写好返回数据
*****************************************************************/
INT8U DLT645_07_CTL0X11_read(INT8U *ptr)
{
	INT8U u8_returnLen,mem_type;
	INT8U i;
	INT8U  *ptr_adr;
	INT8U  data_buff[70],profile_adr[9];
	ST_U32_U08	long_tmp;
	INT16U ctmp1;
	//数据控制ID,传输为地位在前，C51          U32类型为大端模式  //
	gs_dlt645_07_var.u32_DI.BYTE[0] = ptr[DLT_CmdDI3Ptr];
	gs_dlt645_07_var.u32_DI.BYTE[1] = ptr[DLT_CmdDI2Ptr];
	gs_dlt645_07_var.u32_DI.BYTE[2] = ptr[DLT_CmdDI1Ptr];
	gs_dlt645_07_var.u32_DI.BYTE[3] = ptr[DLT_CmdDI0Ptr];
	
   if(get_IEC_comm_var(gs_dlt645_07_var.u32_DI.UDWORD) == 0 )  return(RS_State_IVID);
   if( ( comm_data.Info.u16& EN_R) == 0 )  return(RS_State_IVData);
   //  获取数据的存储类型 //
   mem_type = (comm_data.Info.B08[0]>>3)&0x07;	   
    //数据域清零并设置发送长度
   // Lib_Set_String((sptr+2), '0', comm_data.len_asc);  
    ptr_adr = comm_data.addr;

	//不在表格内ID处理 单条负荷ID 
	if((gs_dlt645_07_var.u32_DI.UDWORD>=0x06010001)&&(gs_dlt645_07_var.u32_DI.UDWORD<=0x06010168))
	{
		ctmp1 = (INT16U)(gs_dlt645_07_var.u32_DI.UDWORD - 0x06010001);
		if(ctmp1 < LPRunPa.Num[0])	// 读取ID 小于于已记录数
		{
			comm_data.len = LPRec_Read_1(&profile_adr[0], ctmp1);
			comm_data.len_asc = 2*comm_data.len;	
			Lib_Asc_BCDA( &data_buff[0], &profile_adr[0], comm_data.len_asc);  
			for(i=0;i<comm_data.len;i++)
			{
				ptr[DLT_ReadDataPtr+i]=data_buff[comm_data.len-1-i];
			}
			u8_returnLen =comm_data.len;		
		}
		else
		{
			comm_data.len = LPRec_Read_1(profile_adr, ctmp1);
			comm_data.len_asc = 2*comm_data.len;	
			for(i=0;i<comm_data.len;i++)
			{
				ptr[DLT_ReadDataPtr+i]=0xFF;
			}
			u8_returnLen =comm_data.len;		
		}

		//操作成功后数据处理  //	
		gs_dlt645_07_var.u8_lenOfData =u8_returnLen;		//返回数据长度定义赋值  //
		ptr[DLT_CtlPtr] += 0X80;
		ptr[DLT_LenPtr] =gs_dlt645_07_var.u8_lenOfData+4;	 //4为固定ID数据 //		
		return(RS_State_OK);	
	}
	
	switch (gs_dlt645_07_var.u32_DI.UDWORD)
	{	
		case 0x02010000:	//电压 源数据为HEX32，0.1V量纲，需要转换为BCD码格式,XXX.XV //
		case 0x02010100:	//电压 源数据为HEX32，0.1V量纲，需要转换为BCD码格式,XXX.XV //
			Lib_long_bcd4(&long_tmp.B08[0],gs_measure_var_data.gs_really[0].dw_u_val.u32);
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[2], 2);
			ptr[DLT_ReadDataPtr] = data_buff[1];
			ptr[DLT_ReadDataPtr+1] = data_buff[0];
			u8_returnLen =2;		
			break;
			
		case 0x02020000:	//电流 源数据为HEX32，0.0001A量纲，需要转换为BCD码格式,XXX.XXXA //
		case 0x02020100:	//电流 源数据为HEX32，0.0001A量纲，需要转换为BCD码格式,XXX.XXXA //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].dw_i_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			ptr[DLT_ReadDataPtr] = data_buff[2];
			ptr[DLT_ReadDataPtr+1] = data_buff[1];
			ptr[DLT_ReadDataPtr+2] = data_buff[0];
			u8_returnLen =3;			
			break;

		case 0x02800001:	//零线电流 源数据为HEX32，0.0001A量纲，需要转换为BCD码格式,XXX.XXXA //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.dw_i_n_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			ptr[DLT_ReadDataPtr] = data_buff[2];
			ptr[DLT_ReadDataPtr+1] = data_buff[1];
			ptr[DLT_ReadDataPtr+2] = data_buff[0];
			u8_returnLen =3;			
			break;

		case 0x02030000:	//功率源数据为HEX32，0.0001KW量纲，需要转换为BCD码格式,XX.XXXXkW //
		case 0x02030100:	//功率源数据为HEX32，0.0001KW量纲，需要转换为BCD码格式,XX.XXXXkW //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].dw_p_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			ptr[DLT_ReadDataPtr] = data_buff[2];
			ptr[DLT_ReadDataPtr+1] = data_buff[1];
			ptr[DLT_ReadDataPtr+2] = data_buff[0];
			u8_returnLen =3;
			break;
			
		case 0x02030200:	//N功率源数据为HEX32，0.0001KW量纲，需要转换为BCD码格式,XX.XXXXkW //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.dw_p_n_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			ptr[DLT_ReadDataPtr] = data_buff[2];
			ptr[DLT_ReadDataPtr+1] = data_buff[1];
			ptr[DLT_ReadDataPtr+2] = data_buff[0];
			break;
			
		case 0x02060000:	//功率因数数据为HEX16，0.001，需要转换为BCD码格式0.001 //
		case 0x02060100:	//功率因数数据为HEX16，0.001，需要转换为BCD码格式0.001 //
			Lib_word_bcd2(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].dw_pf_val.u16));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[0], 2);
			ptr[DLT_ReadDataPtr] = data_buff[1];
			ptr[DLT_ReadDataPtr+1] = data_buff[0];
			u8_returnLen =2;			
			break;

		case 0x02800002:	//电网频率数据为HEX16，0.01Hz，需要转换为BCD码格式xx.xx //
			Lib_word_bcd2(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].w_freq_val.u16));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[0], 2);
			ptr[DLT_ReadDataPtr] = data_buff[1];
			ptr[DLT_ReadDataPtr+1] = data_buff[0];
			u8_returnLen =2;			
			break;

		case 0x04000100://日期及星期(其中0代表星期天)YYMMDDWWhhmmss
			Lib_Copy_Str_TwoArry(&data_buff[0], ptr_adr, comm_data.len);
			data_buff[7] = data_buff[0];	//ww
			data_buff[0] = data_buff[1];	//YY
			data_buff[1] = data_buff[2];	//MM
			data_buff[2] = data_buff[3];	//DD
			data_buff[3] = data_buff[7];	//WW
			data_buff[4] = data_buff[4];	//hh
			data_buff[5] = data_buff[5];	//mm
			data_buff[6] = data_buff[6];	//ss
			ptr[DLT_ReadDataPtr] = data_buff[6];
			ptr[DLT_ReadDataPtr+1] = data_buff[5];
			ptr[DLT_ReadDataPtr+2] = data_buff[4];
			ptr[DLT_ReadDataPtr+3] = data_buff[3];
			ptr[DLT_ReadDataPtr+4] = data_buff[2];
			ptr[DLT_ReadDataPtr+5] = data_buff[1];
			ptr[DLT_ReadDataPtr+6] = data_buff[0];
			u8_returnLen =7;			
			break;

		case 0x04000401://表地址 表号 NNNNNNNNNNNN
			mem_read(&data_buff[0], (INT16U)comm_data.addr,  comm_data.len,  mem_type );
			comm_data.len_asc = get_meter_id_asclen(&data_buff[0], 16);
			comm_data.len = comm_data.len_asc/2;
			for(i=0;i<comm_data.len;i++)
			{
				ptr[DLT_ReadDataPtr+i]=data_buff[comm_data.len-1-i];
			}
			u8_returnLen =comm_data.len;			
			break;

		case 0x04800001: //软件版本号 已是ASC码
			Lib_Copy_Str_TwoArry(&data_buff[0], ptr_adr, comm_data.len);
			for(i=0;i<comm_data.len;i++)
			{
				ptr[DLT_ReadDataPtr+i]=data_buff[comm_data.len-1-i];
			}
			u8_returnLen =comm_data.len;			
			break;		
		case 0x80805001:	//当前月总电量+继电器状态+表计日期时间 //
			//当前月总电量
			mem_read(&data_buff[10], (INT16U)&gs_energy_user_data.us_val[0][0].buf[0], LEN_EC_UNIT, MEM_RAM);	  //获取当前数据 //
			api_get_energy_LCD(&data_buff[10], 0x62,&data_buff[0]);
			for(i=0;i<4;i++)
			{
				ptr[DLT_ReadDataPtr+i]=data_buff[4-1-i];
			}
			//继电器状态
			mem_read(&data_buff[0], (INT16U)ADR_METER_PARAM1_RELAY_CMD, 2, MEM_E2P1);	  //获取当前数据 //
			for(i=0;i<2;i++)
			{
				ptr[DLT_ReadDataPtr+i+4]=data_buff[2-1-i];
			}
			Lib_Copy_Str_TwoArry(&data_buff[0], &gs_CurDateTime.Year, 6);
			for(i=0;i<6;i++)
			{
				ptr[DLT_ReadDataPtr+i+4+2]=data_buff[6-1-i];
			}
			u8_returnLen =comm_data.len;			
		break;
		default:
////////////////电能量特殊处理/////////////////////////////
			if(gs_dlt645_07_var.u32_DI.BYTE[0]==0x00)
			{
				comm_data.len = LEN_EC_UNIT;   // 修改获取长度//
			}
///////////////////////////////////////////////////////////////////

			if((comm_data.Info.u16&CM_ALL) == CM_BILL)
			{
				mem_read(&data_buff[0], api_get_bill_record_addr((INT16U)comm_data.addr), comm_data.len,  mem_type );
			}
			else 
			{
				if(mem_type==MEM_RAM)
				{
					Lib_Copy_Str_TwoArry(&data_buff[0], ptr_adr, comm_data.len);
				}
				else
				{
					mem_read(&data_buff[0], (INT16U)comm_data.addr, comm_data.len, mem_type );
				}
			}

////////////////电能量特殊处理/////////////////////////////
			if(gs_dlt645_07_var.u32_DI.BYTE[0]==0x00)
			{
				Lib_Copy_Str_TwoArry(&data_buff[10], &data_buff[0], LEN_EC_UNIT);
				api_get_energy_LCD(&data_buff[10], 0x62, &data_buff[0]) ;
				comm_data.len=4;
			}
///////////////////////////////////////////////////////////////////

////////////////最大需量特殊处理/////////////////////////////
			if(gs_dlt645_07_var.u32_DI.BYTE[0]==0x01)
			{  
				Lib_Copy_Str_TwoArry(&long_tmp.B08[0], &data_buff[0], 4);
				Lib_long_bcd4(&data_buff[0],long_tmp.u32);
			}
/////////////////////////////////////////////////////////////////// 		   
			for(i=0;i<comm_data.len;i++)
			{
				ptr[DLT_ReadDataPtr+i]=data_buff[comm_data.len-1-i];
			}
			u8_returnLen =comm_data.len;			
			break;	
	}


	//操作成功后数据处理  //	
	gs_dlt645_07_var.u8_lenOfData =u8_returnLen;        //返回数据长度定义赋值  //
	ptr[DLT_CtlPtr] += 0X80;
	ptr[DLT_LenPtr] =gs_dlt645_07_var.u8_lenOfData+4;    //4为固定ID数据 //		
	return(RS_State_OK); 	
}

/*****************************************************************
DLT645 C=0X14 读命令，返回成功结果是数据域已经填写好返回数据
*****************************************************************/
INT8U DLT645_07_CTL0X14_write(INT8U *ptr)
{
	//INT8U u8_returnLen;
	INT8U i,MeterID_LenTmp;
	INT8U mem_type;
	INT8U u8_buff[100];

	// 0  密码检查判断处理          //
	if(DLT_SecurityCK(ptr) != mmok )  
	{
		//密码错误  //
		gs_dlt645_07_var.u8_lenOfData =1;		//返回数据长度定义赋值  //
		return(RS_State_PswE); 		
	}

	// 1 数据控制ID,传输为地位在前，C51		  U32类型为大段模式	//
	gs_dlt645_07_var.u32_DI.BYTE[0] = ptr[DLT_CmdDI3Ptr];
	gs_dlt645_07_var.u32_DI.BYTE[1] = ptr[DLT_CmdDI2Ptr];
	gs_dlt645_07_var.u32_DI.BYTE[2] = ptr[DLT_CmdDI1Ptr];
	gs_dlt645_07_var.u32_DI.BYTE[3] = ptr[DLT_CmdDI0Ptr];
	
	if(get_IEC_comm_var(gs_dlt645_07_var.u32_DI.UDWORD) == 0 )	return(RS_State_IVID);
	if( ( comm_data.Info.u16& EN_W) == 0 )	return(RS_State_IVData);
	//	获取数据的存储类型 //
	mem_type = (comm_data.Info.B08[0]>>3)&0x07; 	
    //数据域清零并检查接收的数据长度合法性 //
    Lib_Set_String(&u8_buff[0], 0x00, comm_data.len); 
    if(gs_dlt645_07_var.u32_DI.UDWORD== 0x04000401) //除设置表号外.都需要对比数据长度
    {
        if((ptr[DLT_LenPtr] > (12+comm_data.len))||(ptr[DLT_LenPtr] < 12))	   
		{
			//数据合法性判断  //
			gs_dlt645_07_var.u8_lenOfData =1;		//返回数据长度定义赋值  //
			return(RS_State_IVData); 
		}
    }
    else
    {
        if(ptr[DLT_LenPtr] != (12+comm_data.len))	   
		{
			//数据合法性判断  //
			gs_dlt645_07_var.u8_lenOfData =1;		//返回数据长度定义赋值  //
			return(RS_State_IVData); 
		}
    }
	// 2 写入数据判断处理          //	
	switch (gs_dlt645_07_var.u32_DI.UDWORD)
		{
			case 0x04800003:
				//修改继电器状态，操作继电器拉合闸 //
				for(i=0;i<comm_data.len;i++)
				{
					u8_buff[i]=ptr[DLT_WriteDataPtr+i];
				}
				if(Drv_relay_COMM_CMD_operating(&u8_buff[0])==FALSE)  
				{
					//数据合法性判断  //
					gs_dlt645_07_var.u8_lenOfData =1;		//返回数据长度定义赋值  //
					return(RS_State_IVData); 
				}
				break;

			case 0x04000100://日期及星期(其中0代表星期天)YYMMDDWWhhmmss  
				gs_CurDateTime.Year= ptr[DLT_WriteDataPtr];
				gs_CurDateTime.Month= ptr[DLT_WriteDataPtr+1];
				gs_CurDateTime.Day= ptr[DLT_WriteDataPtr+2];
				gs_CurDateTime.Week= ptr[DLT_WriteDataPtr+3];
				gs_CurDateTime.Hour= ptr[DLT_WriteDataPtr+4];
				gs_CurDateTime.Minute= ptr[DLT_WriteDataPtr+5];
				gs_CurDateTime.Second= ptr[DLT_WriteDataPtr+6];
				EA=0;
				Set_RTCTime(&gs_CurDateTime);
				EA=1;
				mem_db_write((INT16U)ADR_METER_VAR_RTC, &gs_CurDateTime.Week, 7, MEM_E2P1);
				api_init_md_data_ram();
				#if (TARIFF_MAX_NUM>0)
				api_update_triffNo_pre_minute();
				#endif
				break;

			//表号特殊处理
			case 0x04000401://表地址 表号 16个字节 自适应,如设奇数，则前补0
				MeterID_LenTmp = (ptr[DLT_LenPtr]-12);	//有效表号	长度
				Lib_Set_String(&u8_buff[0], 0xFF, 16);
				for(i=0;i<MeterID_LenTmp;i++)
				{
					u8_buff[i]=ptr[DLT_WriteDataPtr+i];
				}
				mem_db_write((INT16U)comm_data.addr, &u8_buff[0], comm_data.len, mem_type);		//写入应该是16字节，而不是设置的长度
				break;

			case 0x04000B01://结算日DDHH 4
					for(i=0;i<comm_data.len;i++)
					{
						u8_buff[i]=ptr[DLT_WriteDataPtr+i];
					}
					if((u8_buff[0]<0x01)||(u8_buff[0]>0x28)||(u8_buff[1]>0x23))		//错误判断
					{
						return(RS_State_IVData);
					}
					mem_db_write((INT16U)comm_data.addr, &u8_buff[0], comm_data.len, mem_type);
					break;
					
			case 0x04000103://需量参数 PERIOD : SLID   4  //修改后需量重新计算 //
					for(i=0;i<comm_data.len;i++)
					{
						u8_buff[i]=ptr[DLT_WriteDataPtr+i];
					}
					if(((u8_buff[0]%u8_buff[1]) !=0)||(u8_buff[0]>30)||(u8_buff[0]==0)||(u8_buff[1]==0))		//错误判断//
					{
						return(RS_State_IVData);
					}
					mem_db_write((INT16U)comm_data.addr, &u8_buff[0], comm_data.len, mem_type);
					api_init_md_data_ram();
					break;

			case 0x08020200://NN=00，清除所有通道；//
				for(i=0;i<comm_data.len;i++)
				{
					u8_buff[i]=ptr[DLT_WriteDataPtr+i];
				}
				if(u8_buff[0]==0x00)
				{
					api_clr_even_by_comm(); 
					CLRWDT();
				}
				else
				{
					return(RS_State_IVData);
				}
				break;
				
			case 0x08020100://只清当前需量清零//
				for(i=0;i<comm_data.len;i++)
				{
					u8_buff[i]=ptr[DLT_WriteDataPtr+i];
				}
				if(u8_buff[0]!=0)
				return(RS_State_IVData);
				api_clr_current_MD_data();
				CLRWDT();
			break;
			
		case 0x08020300:
			for(i=0;i<comm_data.len;i++)
			{
				u8_buff[i]=ptr[DLT_WriteDataPtr+i];
			}
			if(u8_buff[0]==0x00)
			{
				LoadProRst();
				CLRWDT();
			}
			else
			{
				return(RS_State_IVData);
			}
			break;
		case 0x08020400://电量清零(电表总清(电量+需量)) //
			for(i=0;i<comm_data.len;i++)
			{
				u8_buff[i]=ptr[DLT_WriteDataPtr+i];
			}
			if((u8_buff[0]!=0)||(u8_buff[1]!=0)||(u8_buff[2]!=0)||(u8_buff[3]!=0))
				return(RS_State_IVData);
			api_clr_current_energy_data();
			CLRWDT();
			api_clr_current_MD_data();
			CLRWDT();
#if (BILL_MAX_NUM>0)
			api_clr_bill_data();
#endif
			CLRWDT();
			LoadProRst();
			CLRWDT();
			api_clr_even_by_comm(); 
			CLRWDT();
			api_chg_LCDDisplay_adj_item(DIS_DATA_CLR);	 // 液晶显示 //
			break;
		default:
			for(i=0;i<comm_data.len;i++)
			{
				u8_buff[i]=ptr[DLT_WriteDataPtr+i];
			}
			
			 if(mem_type == MEM_RAM)
			 {
				   mem_write((INT16U)comm_data.addr,&u8_buff[0], comm_data.len, mem_type);
			 }
			 else
			 {
				   mem_db_write((INT16U)comm_data.addr, &u8_buff[0], comm_data.len, mem_type);
			 }
			 
			break;	
		}
	/////////////////设置显示参数后需要更新///////
		if((gs_dlt645_07_var.u32_DI.UDWORD == 0x04000302)||(gs_dlt645_07_var.u32_DI.UDWORD == 0x04000303)||(gs_dlt645_07_var.u32_DI.UDWORD == 0x04040100))
		{
			mem_read(&gs_dis_param.auto_sec, ADR_BLOCK21_DIS_PARAM_E2P, LEN_BLOCK21_DIS_PARAM_E2P, MEM_E2P1);
		}	
	//操作成功后数据处理  //	
	gs_dlt645_07_var.u8_lenOfData =0;		//操作成功返回数据长度定义赋值  //
	ptr[DLT_CtlPtr] += 0X80;
	ptr[DLT_LenPtr] =gs_dlt645_07_var.u8_lenOfData;		
	return(RS_State_OK); 
}

/*****************************************************************
DLT645 C=0X18 密码修改命令,返回成功结果是数据域已经填写好返回数据
*****************************************************************/
INT8U DLT645_07_CTL0X18_chgMIMA(INT8U *ptr)
{
	INT8U i;
	
	// 0  密码检查判断处理          //
	if(DLT_SecurityCK(ptr) != mmok )  
	{
		//密码错误  //
		gs_dlt645_07_var.u8_lenOfData =1;		//返回数据长度定义赋值  //
		return(RS_State_PswE); 		
	}

	// 1 数据控制ID,传输为地位在前，C51		  U32类型为大端模式	//
	gs_dlt645_07_var.u32_DI.BYTE[0] = ptr[DLT_CmdDI3Ptr];
	gs_dlt645_07_var.u32_DI.BYTE[1] = ptr[DLT_CmdDI2Ptr];
	gs_dlt645_07_var.u32_DI.BYTE[2] = ptr[DLT_CmdDI1Ptr];
	gs_dlt645_07_var.u32_DI.BYTE[3] = ptr[DLT_CmdDI0Ptr];

	// 2  ID判断处理  //
	if((gs_dlt645_07_var.u32_DI.UDWORD<0X04000C01) || (gs_dlt645_07_var.u32_DI.UDWORD >0X04000C02) )
	{
		return(RS_State_IVID);
	}

	// 3  数据长度合法性判断处理  //
	if(ptr[DLT_LenPtr] != 12 )           //12为固定长度，4ID，4老密码，4新密码  //
	{
		//数据合法性判断  //
		gs_dlt645_07_var.u8_lenOfData =1;		//返回数据长度定义赋值  //
		return(RS_State_IVData); 
	}	

	//ID正确后的密码修改处理  //	
	mem_db_write(ADR_METER_PARAM1_PW1, &ptr[DLT_WriterC0Ptr], 4, MEM_E2P1);

	//操作成功后数据处理  //	
	gs_dlt645_07_var.u8_lenOfData =4;		//操作成功返回数据长度定义赋值  //
	ptr[DLT_CtlPtr] += 0X80;
	ptr[DLT_LenPtr] =gs_dlt645_07_var.u8_lenOfData;
	for(i=0;i<gs_dlt645_07_var.u8_lenOfData;i++)
	{
		ptr[DLT_Cmd+i] = ptr[DLT_WriterC0Ptr+i];
	}	
	return(RS_State_OK); 
}


/*****************************************************************************
** Function name    :INT8U Lnk_SX127X_rx_data(INT8U *rx_ptr)
**
** Description          :判断并获取RF模式接收数据并存放在RAM区
**
** Parameters           :NONE
**
** Returned value       :INT8U  接收返回长度，0代表未接收到数据
**
** Note                 :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-12-07
******************************************************************************/
void api_DLT645_07_protocol_analysis(void)
{
	INT8U start_index;
	INT8U i;
	INT8U result_analysis;
	INT8U offset_cs;

    //前导字节和 帧头判断处理//
    for(start_index=0;start_index<5;start_index++)
    {
        if((P_DL645_07_BUFF[start_index]==0X68)&&(P_DL645_07_BUFF[start_index+DLT_Head0Ptr]==0X68))  break;
    } 
    if(start_index>4)
    {
        //接收的数据帧错误，复位处理      //
        api_RF_commRAM_RESET();
        return;
    }

    //CS校验和处理//
    offset_cs = DLT_Cmd+P_DL645_07_BUFF[start_index+DLT_LenPtr];
    if(P_DL645_07_BUFF[start_index+offset_cs] !=(INT8U)( Lib_get_csck_int16u_num( &(P_DL645_07_BUFF[start_index]), (INT8U)(offset_cs),0 )))
    { 
        //接收的数据帧错误，复位处理      //
        api_RF_commRAM_RESET();
        return;
    } 

    //结束字符处理//
    if(P_DL645_07_BUFF[start_index+offset_cs+1] !=0x16)
    { 
        //接收的数据帧错误，复位处理      //
        api_RF_commRAM_RESET();
        return;
    } 

    //表号检查判断//
    if(MeteIDCheck(&P_DL645_07_BUFF[start_index+DLT_Head0Ptr])!=TRUE )
    { 
        //接收的数据帧错误，复位处理      //
        api_RF_commRAM_RESET();
        return;
    } 

	//data  subtract 0x33 process   //
    for(i=0; i<P_DL645_07_BUFF[start_index+DLT_LenPtr]; i++ )
    {
        P_DL645_07_BUFF[start_index+DLT_Cmd+i] -= 0x33;
    }

	//控制码C分支处理  //
	switch (P_DL645_07_BUFF[start_index+DLT_CtlPtr]& 0x1F)
	{
		case DLT_CTL_Read:
			result_analysis = DLT645_07_CTL0X11_read(&P_DL645_07_BUFF[start_index] );
			break;

		case DLT_CTL_Write:
			result_analysis = DLT645_07_CTL0X14_write(&P_DL645_07_BUFF[start_index] );
			break;

		case DLT_CTL_ChgPswd:
			result_analysis = DLT645_07_CTL0X18_chgMIMA(&P_DL645_07_BUFF[start_index] );
			break;

		case DLT_CTL_Broadcast:
			result_analysis = RS_State_Broadcast;
			break;
			
		default:
			//接收控制码错误，不应答//
			api_RF_commRAM_RESET();
			return;
	}
	//针对返回结果的分支处理 //
	if(result_analysis==RS_State_OK)
		{		
		}
	else if((result_analysis==RS_State_IVData)||(result_analysis==RS_State_IVID))
		{
			P_DL645_07_BUFF[start_index+DLT_CtlPtr] +=0XC0;
			P_DL645_07_BUFF[start_index+DLT_LenPtr] =1;
			P_DL645_07_BUFF[start_index+DLT_Cmd] =BIT0;
		}	
	else if(result_analysis==RS_State_PswE)
		{
			P_DL645_07_BUFF[start_index+DLT_CtlPtr] +=0XC0;
			P_DL645_07_BUFF[start_index+DLT_LenPtr] =1;
			P_DL645_07_BUFF[start_index+DLT_Cmd] =BIT2;
		}
	else if(result_analysis==RS_State_Broadcast)
		{
			api_chg_LCDDisplay_adj_item(DIS_DATA_BROADCAST);   // 液晶显示 //
			//直接复位RF通讯接口，无返回数据  //
			api_RF_commRAM_RESET();
			return;
		}
	else
		{
			//直接复位RF通讯接口，无返回数据	//
			api_RF_commRAM_RESET();
			return;
		}


	/////////////////////////////////////////////////////////
	//发送数据组帧
	////////////////////////////////////////////////////////

	// 1 获取表号数据  //
	mem_read(&P_DL645_07_BUFF[start_index+DLT_AddrPtr], ADR_METER_PARAM1_RF_COMM_ID, 6, MEM_E2P1);

	// 2 data  add 0x33 process   //
    for(i=0; i<P_DL645_07_BUFF[start_index+DLT_LenPtr]; i++ )
    {
        P_DL645_07_BUFF[start_index+DLT_Cmd+i] += 0x33;
    }

	// 3 CS 校验和获取 //
    offset_cs = DLT_Cmd+P_DL645_07_BUFF[start_index+DLT_LenPtr];
    P_DL645_07_BUFF[start_index+offset_cs] =(INT8U)( Lib_get_csck_int16u_num( &(P_DL645_07_BUFF[start_index]), (INT8U)(offset_cs),0 ));	

	// 4 结束符获取 //
	P_DL645_07_BUFF[start_index+offset_cs+1] =0x16;


	/////////////////////////////////////////////////////////
	//配置发送前准备
	////////////////////////////////////////////////////////
	gs_dlt645_07_var.start_index = start_index;
	gs_dlt645_07_var.tx_frame_len= start_index+12+P_DL645_07_BUFF[start_index+DLT_LenPtr];   // 获取数据发送帧总长度 //
	gs_dlt645_07_var.tx_delay_flg = TRUE;
	gs_dlt645_07_var.tx_ready_10ms =8;         //延时80ms //

}


/*****************************************************************************
** Function name    :INT8U Lnk_SX127X_rx_data(INT8U *rx_ptr)
**
** Description          :判断并获取RF模式接收数据并存放在RAM区
**
** Parameters           :NONE
**
** Returned value       :INT8U  接收返回长度，0代表未接收到数据
**
** Note                 :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-12-07
******************************************************************************/
void api_RF_tx_data(INT8U *txbuf,INT8U tx_len)
{
    if(tx_len==0)   return;
    G_LoRaConfig.PayloadLength= tx_len;   
    SX1276_TxPacket(txbuf);
    gs_rf_drv_var.status = RF_STATUS_TX;   
    gs_rf_drv_var.tmr_tx_max_wait = 0;
}


/*****************************************************************************
** Function name    :INT8U Lnk_SX127X_rx_data(INT8U *rx_ptr)
**
** Description          :判断并获取RF模式接收数据并存放在RAM区
**
** Parameters           :NONE
**
** Returned value       :INT8U  接收返回长度，0代表未接收到数据
**
** Note                 :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-12-07
******************************************************************************/
void api_RF_commRAM_RESET(void)
{
	gs_rf_drv_var.status = RF_STATUS_RX;
	gs_rf_drv_var.data_len = 0;
	Lib_Clr_String(&gs_rf_drv_var.buff[0],RF_BUFF_MAX_SIZE);
	gs_rf_drv_var.tmr_tx_max_wait = 0;
	Lib_Clr_String(&gs_dlt645_07_var.u8_result,sizeof(DLT645_07_VAR));
	
	Lnk_SX2176_Intilize();	
}

/*****************************************************************************
** Function name    :INT8U Lnk_SX127X_rx_data(INT8U *rx_ptr)
**
** Description          :判断并获取RF模式接收数据并存放在RAM区
**
** Parameters           :NONE
**
** Returned value       :INT8U  接收返回长度，0代表未接收到数据
**
** Note                 :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-12-07
******************************************************************************/
INT8U api_RF_judge_rx(INT8U **p_buf)
{
    if(gs_rf_drv_var.status == RF_STATUS_TX)   return(0);       //发送状态不处理接收 //
    
    // 返回缓冲区地址 //
    *p_buf = &gs_rf_drv_var.buff[0] ;
 	// RF 通道是否接收到数据 //
	gs_rf_drv_var.data_len = Lnk_sx1276_rx_data(&gs_rf_drv_var.buff[0]);

	return (gs_rf_drv_var.data_len);
}


/*****************************************************************************
** Function name    :INT8U Lnk_SX127X_rx_data(INT8U *rx_ptr)
**
** Description          :判断并获取RF模式接收数据并存放在RAM区
**
** Parameters           :NONE
**
** Returned value       :INT8U  接收返回长度，0代表未接收到数据
**
** Note                 :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-12-07
******************************************************************************/
void api_RF_judge_tx(void)
{
	if((gs_dlt645_07_var.tx_ready_10ms ==0)&&(gs_dlt645_07_var.tx_delay_flg ==TRUE))
		{
        P_DL645_07_BUFF = &gs_rf_drv_var.buff[0];
        api_RF_tx_data(&P_DL645_07_BUFF[0], gs_dlt645_07_var.tx_frame_len);   // 数据发送，带前导 //   
        gs_dlt645_07_var.tx_delay_flg =FALSE;
		}
}

/*****************************************************************************
** Function name    :INT8U Lnk_SX127X_rx_data(INT8U *rx_ptr)
**
** Description          :判断并获取RF模式接收数据并存放在RAM区
**
** Parameters           :NONE
**
** Returned value       :INT8U  接收返回长度，0代表未接收到数据
**
** Note                 :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-12-07
******************************************************************************/
void api_RF_judge_tx_over_delay(void)
{
    if(gs_rf_drv_var.status != RF_STATUS_TX)    return ;           //不是发送状态，退出 //

	//RF模块发送超时，返回TRUE,用来复位整个通讯通道 //
    if((gs_rf_drv_var.tmr_tx_max_wait>=RF_TX_WAIT_MAX_TRM))
    {      
		api_RF_commRAM_RESET();    		
		return; 
    }

	// RF 通道是否发送完数据 //
   if(LSD_RF_READ_DIO0()==0) return;
   Lib_Delay_2us(5);
   if(LSD_RF_READ_DIO0() ==0) return ;   

   api_RF_commRAM_RESET();		   
   return; 
}


/*****************************************************************************
** Function name    :INT8U Lnk_SX127X_rx_data(INT8U *rx_ptr)
**
** Description          :判断并获取RF模式接收数据并存放在RAM区
**
** Parameters           :NONE
**
** Returned value       :INT8U  接收返回长度，0代表未接收到数据
**
** Note                 :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-12-07
******************************************************************************/
void api_RF_comm_deal(void)
{
	INT8U buff_size;

    if(gs_rf_drv_var.status == RF_STATUS_TX)   return;       //发送状态不处理接收 //
	CLRWDT();	// 清看门狗//
	buff_size = api_RF_judge_rx(&P_DL645_07_BUFF); 

    if(buff_size==0) return;    // 每个循环获取一次接收数据长度，=0，退出 //	

	gs_rf_drv_var.data_len = buff_size;
	CLRWDT();	// 清看门狗//
	api_DLT645_07_protocol_analysis();

}



/*******************************************************************
*
*  end of file                                    
*
*******************************************************************/


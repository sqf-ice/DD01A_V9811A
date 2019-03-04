/*******************************************************************************
*模块名   : LoadProfile.c
*相关文件 : 
*文件功能 : 负荷记录处理模块
*作者     : 研发三部
*编写日期 :
-------------------------------------------------------------------------------
*修改记录 : 
*日 期        版本     修改人              修改内容
*YYYY/MM/DD   X.Y      <作者或修改者名>    <修改内容>
*******************************************************************************/
#include <MyIncludes_H.h>


#if  FUNC_LoadPro   //负荷记录使能开关

//----------------------------------------------------------------全局变量定义区
LPREAD	    LPRd;            //负荷记录存储信息
LPRUNPARA   LPRunPa;         //负荷记录参数
LPReadPRO  LPReadPara;
//-------------------------------------------------------------全局变量定义区end

/*********************************************************************
原型：void LoadProRst(void)
功能：复位负荷表
备注：无。
*********************************************************************/
void LoadProRst(void)
{
    Lib_Set_String( (uint8*)&LPRunPa, 0x00, (sizeof(LPRUNPARA)) );
    mem_db_write( ADR_METER_PARAM1_LOAD_PTR, (uint8*)&LPRunPa, LP_RunPara_Len, MEM_E2P1);// 负荷记录指针、记录个数  !!
    
    Lib_Set_String( &LPReadPara.Original_Ptr, 0, 5) ;		




}

/*********************************************************************
原型：uchar ReadLPRunPa(void)
功能：读取负荷记录参数
备注：无。
*********************************************************************/
uchar ReadLPRunPa(void)
{
    return TRUE;
}

/*******************************************************************************
		相关通讯
*********************************************************************************/
/*********************************************************************
原型：uchar LPRec_Read(void)
功能：读负荷表数据  -- 分ID
备注：无。
*********************************************************************/
uchar LPRec_Read_1(INT8U *dst,INT16U idNo)
{
	uchar  RecLen;
	INT8U Buffer[20];
	INT32U  LPAddr;
	INT16U ptr;


	if(LPRunPa.Ptr[0] >= idNo)
	{
		ptr = LPRunPa.Ptr[0] - idNo;	// 前第几条
	}
	else
	{
	    ptr = LPRd.MaxRecNum - (idNo - LPRunPa.Ptr[0] );
	}
	
	RecLen = 18;	//一个通道数据长度
	LPAddr = (ADR_BLOCK180_LOAD_START_E2P+ (INT32U)ptr*RecLen);
	mem_read( &Buffer[0],LPAddr, RecLen, MEM_E2P1);
	Lib_BCD_AscA(dst,(char*)&Buffer[0],RecLen); 
//	Lib_BCD_AscA(dst,(char*)&Buffer[0+5],4); 
//	Lib_BCD_AscA(dst,(char*)&Buffer[0+14],2); 
//	Lib_BCD_AscA(dst,(char*)&Buffer[0+21],3);
//  Lib_BCD_AscA(dst,(char*)&Buffer[0+29],RecLen);

  
  return   (RecLen);
}


/*********************************************************************
原型：uchar LPRec_Read_Blank(void)
功能：读负荷表数据  -- 分ID
备注：无。
*********************************************************************/
INT16U LPRec_Read_Block(INT8U *dst,INT16U StartARESS,INT16U EndARESS)
{
	uchar Ch,RecLen;
	INT8U Buffer[200];
	INT32U  LPAddr,LPAddr2;
	INT16U  HGAddr,LWAddr;
	INT16U ComLenth,ptr;
	INT16U ComNo;
	ST_U16_U08 TempLen;

	ptr =0; ComLenth =0;
	CLRWDT(); 
  if((LPReadPara.Original_FrameNum==0)||( LPReadPara.Original_FrameNum < StartARESS ))return(FALSE); 
	else LWAddr = StartARESS;
	
	if(EndARESS >= READLP_MAX_LEN) HGAddr = READLP_MAX_LEN-1;
	else	HGAddr = EndARESS;
	
	TempLen.u16 = HGAddr - LWAddr + 1;	
	
	ptr = (LPReadPara.Original_Ptr+ READLP_MAX_LEN - LWAddr)%READLP_MAX_LEN ;	
	
	RecLen = 18;	//一个通道数据长度
	
	LPAddr = (ADR_BLOCK180_LOAD_START_E2P);
  CLRWDT(); 
	for( Ch=0; Ch<BLOCK_MAX_LEN; Ch++)
	{
    LPAddr2=LPAddr+((ptr+ READLP_MAX_LEN-Ch)%READLP_MAX_LEN)*RecLen;
		mem_read( &Buffer[0+Ch*RecLen],LPAddr2 , RecLen, MEM_E2P1);	  

	}

	if(( TempLen.u16 >= ( LPReadPara.Original_FrameNum-StartARESS ))&&( LPReadPara.Original_FrameNum <READLP_MAX_LEN))
		TempLen.u16 = LPReadPara.Original_FrameNum-StartARESS;
	
	if( TempLen.u16 <= BLOCK_MAX_LEN )
		ComNo = TempLen.u16;
	else 
		ComNo = BLOCK_MAX_LEN;
	ptr =0; 
	
	
	CLRWDT(); 
	for( Ch=0; Ch < ComNo; Ch++)
	{
		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[Ch*RecLen],RecLen); 
//	   ptr += 10;			
//		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[5+Ch*RecLen],4); 
//	   ptr += 8;
//		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[14+Ch*RecLen],2); 
//	   ptr += 4;
//		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[21+Ch*RecLen],3); 
//	   ptr += 6;
//		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[29+Ch*RecLen],4); 
	   ptr += 36;
     ComLenth += 36;			
	}
	 
  return  (ComLenth);
}

/*********************************************************************
原型：uchar Get_LoadRegNo(uchar Code)
功能：得到LoadReg表格下标
备注：通道号从0开始
*********************************************************************/
//uchar Get_LoadRegNo(uchar Code)
//{
//    uchar i;
//    
//    for(i=0; i<CLPRegNum; i++)
//    {
//	if(Code == LPReg[i].Code)
//	    return i;
//    }
//    return FALSE_ERR;
//}

/*********************************************************************
原型：char Load_Rec(char Ch)
功能：手动记录负荷数据
备注：通道号从0开始
*********************************************************************/

//char Load_Rec(char Ch)//一个通道
//{
//    INT32U  LPAddr;
//    INT8U   Len;  //,CTmp ,RegNo i,
//    INT8U Buffer[20];
 

//    if(Ch >= MaxLPCh) return FALSE;
//    if( FALSE == ReadLPRunPa() ) return FALSE;
    /////////////////////////////////////////////////////////////////////////////////////////////	
    ///////////////////取一个通道一次记录数据
    /****通道数据和通讯顺序相反
    通讯顺序:年月日时分 ，REG1（高~低），REG2（高~低）……
    存储顺序：年月日时分 ，REG1（高~低），REG2（高~低）……
    *************************************/
//    Lib_Set_String( (uchar*)&Buffer[0], 0x00, 20 );
//    Len = 0;
//	 Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], &gs_CurDateTime.Year, 5 );// 取当前时间
//    Len += 5;
   //////负荷记录有功总电量 获取电量 4字节放入   BUFF 5   ////////  4字节XXXXXX.XX  kwh  BCD 
//     api_get_energy_LCD(&gs_energy_user_data.us_val[0][0].buf[0], 0x62,&Buffer[Len]);
//     Len += 4;
   /*     负荷记录需量 3字节放入   BUFF 5   //////// 存入格式为XX.XXXX kw   BCD
      Lib_long_bcd4(&Buffer[6],gs_md_var.min_var[0].val_min);          //  16进制转10进制// 		
	  Buffer[Len] =  0x00 ;
	  Buffer[Len+1]= Buffer[7];
      Buffer[Len+2]= Buffer[8]; 
      Buffer[Len+3]= Buffer[9]; 
      Len += 4;
      */
    //////存一个通道一次记录数据//////////////
    //各通道存储指针和条数+1
//    LPRunPa.Num[Ch]++;
//    if( LPRunPa.Num[Ch] > LPRd.MaxRecNum ) LPRunPa.Num[Ch] = LPRd.MaxRecNum;
//    LPRunPa.Ptr[Ch]++;
//    if( LPRunPa.Ptr[Ch] > LPRd.MaxRecNum ) LPRunPa.Ptr[Ch] = 0;


//    LPAddr = ADR_BLOCK180_LOAD_START_E2P;
//    LPAddr += (INT32U)LPRunPa.Ptr[Ch]*Len;

//	if((LPAddr +Len) > (ADR_BLOCK180_LOAD_START_E2P+LP_MAX_LEN*9))		// 超过最大区域字节数	54000   
//	{
//		LPRunPa.Ptr[Ch] = 0;
//		LPAddr = ADR_BLOCK180_LOAD_START_E2P;
//		LPAddr += (INT32U)LPRunPa.Ptr[Ch]*Len;
//	}
	    
//    mem_write( (LPAddr),(char*)&Buffer[0], Len, MEM_E2P1);

//    
//    return TRUE;
//}

char Load_Rec(char Ch)//一个通道
{
	INT32U  LPAddr;
	INT8U   Len;  //,CTmp ,RegNo i,
	INT8U Buffer[20];


	if(Ch >= MaxLPCh) return FALSE;
	//    if( FALSE == ReadLPRunPa() ) return FALSE;
	/////////////////////////////////////////////////////////////////////////////////////////////	
	///////////////////取一个通道一次记录数据
	/****通道数据和通讯顺序相反
	通讯顺序:年月日时分 ，REG1（高~低），REG2（高~低）……
	存储顺序：年月日时分 ，REG1（高~低），REG2（高~低）……
	*************************************/
	Lib_Set_String( (uchar*)&Buffer[0], 0x00, 20 );
	Len = 0;
	Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], &gs_CurDateTime.Year, 5 );// 取当前时间
	Len += 5;
//	switch (Ch)
//	{
//		case 0:
//			//////负荷记录有功总电量 获取电量 4字节放入   BUFF 5   ////////  4字节XXXXXX.XX  kwh  BCD 
			api_get_energy_LCD(&gs_energy_user_data.us_val[0][0].buf[0], 0x62,&Buffer[Len]);
			Len += 4;
//			break;
//		case 1:
			//////负荷记录当前电压 获取电压		2字节   xxx.x V BCD	
			//api_get_energy_LCD(&gs_energy_user_data.us_val[0][0].buf[0], 0x62,&Buffer[Len]);
			Lib_long_bcd4((char*)&Buffer[Len],gs_measure_var_data.gs_really[PHASE_TT].dw_u_val.u32);
			Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], (char*)&Buffer[Len+2], 2 );
			Len += 2;
//			break;
//		case 2:
			//////负荷记录当前电流 获取电流  	3字节XXX.XXX A BCD
//			Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], &(gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32), 3 );
			Lib_long_bcd4((char*)&Buffer[Len],gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32);
			Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], (char*)&Buffer[Len+1], 3 );
			Len += 3;
//			break;
//		case 3:
			//////负荷记录当前需量 获取需量 	4字节XXXXXX.XX  kwh  ASCII
			mem_read((char*)&Buffer[Len], ADR_BLOCK01_MMD_L0_E2P, 4, MEM_E2P1);
			Len += 4;
//			break;
//		default:
//			break;
		
		
	

	/*     负荷记录需量 3字节放入   BUFF 5   //////// 存入格式为XX.XXXX kw   BCD
	Lib_long_bcd4(&Buffer[6],gs_md_var.min_var[0].val_min);          //  16进制转10进制// 		
	Buffer[Len] =  0x00 ;
	Buffer[Len+1]= Buffer[7];
	Buffer[Len+2]= Buffer[8]; 
	Buffer[Len+3]= Buffer[9]; 
	Len += 4;
	*/
	//////存一个通道一次记录数据//////////////
	//各通道存储指针和条数+1
	LPRunPa.Num[Ch]++;
	if( LPRunPa.Num[Ch] > LPRd.MaxRecNum ) LPRunPa.Num[Ch] = LPRd.MaxRecNum;
	LPRunPa.Ptr[Ch]++;
	if( LPRunPa.Ptr[Ch] >= LPRd.MaxRecNum ) LPRunPa.Ptr[Ch] = 0;


	LPAddr = ADR_BLOCK180_LOAD_START_E2P;
	LPAddr += (INT32U)LPRunPa.Ptr[Ch]*Len;
  
	mem_db_write( ADR_METER_PARAM1_LOAD_PTR, (uint8*)&LPRunPa, LP_RunPara_Len, MEM_E2P1);// 负荷记录指针、记录个数  !!
//	if((LPAddr +Len) > (ADR_BLOCK180_LOAD_START_E2P+LP_MAX_LEN*9))		// 超过最大区域字节数	54000   
//	{
//		LPRunPa.Ptr[Ch] = 0;
//		LPAddr = ADR_BLOCK180_LOAD_START_E2P;
//		LPAddr += (INT32U)LPRunPa.Ptr[Ch]*Len;
//	}
	    
//	mem_write( (LPAddr),(char*)&Buffer[0], Len, MEM_DMFLASH);
	mem_write( (LPAddr),(char*)&Buffer[0], Len, MEM_E2P1);

	return TRUE;
}


/*********************************************************************
原型：void LoadPro_Rec(void)
功能：负荷数据记录
备注：
*********************************************************************/
void LoadPro_Rec(void)
{
	INT8U Ch;
	INT16U Addr;
	INT16U TimeInv;
	INT8U flag;
	INT8U Buffer[2];
	
	for( Ch=0; Ch<MaxLPCh; Ch++ ) 
	{
		Addr = ADR_METER_PARAM1_load_lptime_inv_var + 2*Ch;
		//读取个通道记录间隔时间
		mem_read( (char*)&Buffer[0], (int)Addr, 2, MEM_E2P1 );
		TimeInv = Lib_bcd2_word((uchar *)&Buffer[0]);
		if( TimeInv != 0 ) 
		{
			flag = 0;
			if ( TimeInv <= 60 )      //小时计算，限定4小时		 
			{
#ifdef _DEBUGE_FUNCTION
				Buffer[0] = Lib_bcd_byte(gs_CurDateTime.Second); 
				if(!(Buffer[0]%TimeInv)) flag = 1;
				// Buffer[0] = Lib_bcd_byte(gs_CurDateTime.Minute); 
				// if(!(Buffer[0]%TimeInv)) flag = 1;
#endif		
				Buffer[0] = Lib_bcd_byte(gs_CurDateTime.Minute); 
				if(!(Buffer[0]%TimeInv)) flag = 1;
			}	
			if(flag)
			{
				Load_Rec(Ch);
			}
		}
	}
}



/*********************************************************************
原型：void LoadPro_Rec(void)
功能：负荷数据上电初始化
备注：通讯中若要调用，注意先保存负荷记录参数
*********************************************************************/
void Init_LoadPro_param(void)
{

	// 记得上一次负荷记录参数的保存
	mem_read((char*)&LPRunPa, ADR_METER_PARAM1_LOAD_PTR, LP_RunPara_Len, MEM_E2P1);              
	//读取寄存器个数
	mem_read((char*)&LPRd.RegNum, ADR_METER_PARAM1_load_egcode_var, 1, MEM_E2P1);
	if( ( LPRd.RegNum < 1 ) || ( LPRd.RegNum > MaxLPReg ) ) 
	{
		LPRd.RegNum = 1;
	}
	
	//取最大记录条数  
	LPRd.MaxRecNum =  LP_MAX_LEN ;   // 最大60天，每4小时一个记录的容量 时间+电量   YYMMDDhhmm+[XXXXXX.XX]+[XX.XXXX]

	mem_read(&LPRd.loadp_reg_code[0],ADR_METER_PARAM1_load_egcode_var, 10, MEM_E2P1);	// 读寄存器代码

#if DEBUG_TEST

#endif

}


#endif  

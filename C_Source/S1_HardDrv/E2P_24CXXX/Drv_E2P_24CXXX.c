/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_E2P_FM24c16.c
**Author		: maji
**date			: 2016-04-20
**description	: c code for read&Write FM24C16(铁电)
**note			:  Driver code for GENERAL
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-20   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>


/*******************************************************************************************
** 函数名称: Read_EEPROM_Page_Sub
** 函数描述: 从EEPROM读取若干字节 （在1个PAGE的范围内操作）
** 输入参数: *Buf:目标据RAM数据首地址指针
**           Addr:源数据EEPROM存储地址
**           Len: 读取的字节数
** 输出参数: TRUE-读取正确   FALSE-读取失败
** 说明    : 在1个PAGE的范围内操作使用，无双读比对冗错机制
*******************************************************************************************/
uint8 Read_EEPROM_Page_Sub(uint8 *Buf,uint16 Addr,uint8 Len)
{
	uint8 NewWrCmd,NewRdCmd;
	#if (EE_Chip>=EE24XX08 )
	uint8 Page;
	#endif
	uint8 err_cnt=0;
	   

	#if (EE_Chip>=EE24XX08 )

	Page=HIBYTE(Addr);
	Page=Page&Bin(00000111);
	Page=Page<<1;
	NewWrCmd=WRITE_EEPROM_CMD|Page;
	NewRdCmd=READ_EEPROM_CMD|Page;
	#else
	NewWrCmd=WRITE_EEPROM_CMD;
	NewRdCmd=READ_EEPROM_CMD;
	#endif

	do
	{
		E2p_Stop_i2c();
		err_cnt++;
		if(err_cnt>10)	return FALSE;   // 重试10次不能成功则返回失败标识//
		E2p_Start_i2c();
	}while (E2p_TxByte_i2c(NewWrCmd)!=TRUE);

	#if (EE_Chip>=EE24XX08 )
	if(E2p_TxByte_i2c(LOBYTE(Addr))!=TRUE) return FALSE;
	#else
	if(E2p_TxByte_i2c(HIBYTE(Addr))!=TRUE) return FALSE;
	if(E2p_TxByte_i2c(LOBYTE(Addr))!=TRUE) return FALSE;
	#endif

	E2p_Start_i2c();                                           //重发启始条件  //

	if(E2p_TxByte_i2c(NewRdCmd)!=TRUE) return FALSE;

	while(Len--)
	{
		*Buf = E2p_RxByte_i2c((Len==0)? FALSE: TRUE);		//读出数据内容  //
		Buf++;
	}
	E2p_Stop_i2c();
	return TRUE;
}

/*******************************************************************************************
** 函数名称: Write_EEPROM_Page_Sub
** 函数描述: 从EEPROM写入若干字节 （在1个PAGE的范围内操作）
** 输入参数: addr:目标数据EEPROM存储地址
**           *buf:源数据RAM数据首地址指针
**           len: 写入的字节数
** 输出参数: TRUE-写入正确   FALSE-写入失败
** 说明    : 在1个PAGE的范围内操作使用，无写入读出比对冗错机制
*******************************************************************************************/
uint8 Write_EEPROM_Page_Sub(uint16 Addr,uint8 *Buf,uint8 Len)
{
	uint8 NewWrCmd,NewRdCmd;

	#if (EE_Chip>=EE24XX08 )
	uint8 Page;
	#endif
	uint8 err_cnt=0;

	#if (EE_Chip>=EE24XX08 )
	Page=HIBYTE(Addr);
	Page=Page&Bin(00000111);
	Page=Page<<1;
	NewWrCmd=WRITE_EEPROM_CMD|Page;
	NewRdCmd=READ_EEPROM_CMD|Page;
	#else
	NewWrCmd=WRITE_EEPROM_CMD;
	NewRdCmd=READ_EEPROM_CMD;
	#endif

	Disable_WP();
	do
	{
		E2p_Stop_i2c();
		err_cnt++;
		if(err_cnt>10)	return FALSE;   // 重试10次不能成功则返回失败标识//
		E2p_Start_i2c();
	}while (E2p_TxByte_i2c(NewWrCmd)!=TRUE);

	#if (EE_Chip>=EE24XX08 )
	if(E2p_TxByte_i2c(LOBYTE(Addr))!=TRUE) return FALSE;
	#else
	if(E2p_TxByte_i2c(HIBYTE(Addr))!=TRUE) return FALSE;
	if(E2p_TxByte_i2c(LOBYTE(Addr))!=TRUE) return FALSE;
	#endif

	do
	{
		if (E2p_TxByte_i2c(*Buf)!=TRUE )
		{
			Enable_WP();
			return FALSE;
		}
		Buf++;
		Addr++;
		Len--;
	}while (Len != 0) ;

	E2p_Stop_i2c();
	#ifdef _E2P_TYPE_AT24
	Lib_Delay_ms(5);   //  普通E2P写入后需要延时5ms//			
	#endif
	Enable_WP();

	return TRUE;
}



/*******************************************************************************************
** 函数名称: Read_EEPROM_Page
** 函数描述: 从EEPROM读取若干字节 （在1个PAGE的范围内操作并带双读比对）
** 输入参数: * u8p_dst:目标数据指针
**           u16_src:  源数据EEPROM存储地址
**           u8_len: 读取的字节数
** 输出参数: TRUE-读取正确   FALSE-读取失败
** 说明    : 在1个PAGE的范围内操作使用，具备双读比对冗错机制
**           失败情况下最大可以重试3次
*******************************************************************************************/
uint8 Read_EEPROM_Page(uint8* u8p_dst, uint16 u16_src, uint8 u8_len)
{
 uint8 data00[EE_PageMask];
 uint8 i;
  
	if((u8_len<= 0)||(u8_len>EE_PageMask))     //if len is illegal,program run with dead circle
	{
		return(FALSE); 
	}
	for(i=0;i<3;i++)                                                 //read and compare, if error,repeat 6 times.
	{
		Read_EEPROM_Page_Sub(&data00[0],u16_src , u8_len); 
		Read_EEPROM_Page_Sub(u8p_dst, u16_src, u8_len);
		if(Lib_Cmp_TwoArry(u8p_dst,&data00[0],u8_len)==0)  return (TRUE);  
	}
	return(FALSE);
}


/*******************************************************************************************
** 函数名称: Write_EEPROM_Page
** 函数描述: 从EEPROM读取若干字节 （在1个PAGE的范围内操作并带写入读出比对）
** 输入参数: u16_dst:   目标数据EEPROM存储地址
**           *u8p_src:  源数据RAM数据首地址指针
**           u8_len: 写入的字节数
** 输出参数: TRUE-写入正确   FALSE-写入失败
** 说明    : 在1个PAGE的范围内操作使用，具备写入读出比对冗错机制
**           失败情况下最大可以重试3次
*******************************************************************************************/
uint8 Write_EEPROM_Page(uint16 u16_dst, uint8 *u8p_src, uint8 u8_len)
{
 uint8 data00[EE_PageMask];
 uint8 i;
  
   if((u8_len<= 0)||(u8_len>EE_PageMask))                                    //if len is illegal,program run with dead circle
   {
       sys_err();
   }
   for(i=0;i<3;i++)                                                           //write and read compare, if error,repeat 3 times.
   {
       Write_EEPROM_Page_Sub(u16_dst, u8p_src, u8_len);
       Read_EEPROM_Page_Sub(&data00[0], u16_dst , u8_len); 
       if(Lib_Cmp_TwoArry(u8p_src,&data00[0],u8_len)== 0 )  return (TRUE);  
   }
   return(FALSE);
}


/*******************************************************************************************
** 函数名称: Read_EEPROM
** 函数描述: 从EEPROM读取若干字节 （实现任意位置起始的16位字节长度数据读取）
** 输入参数: * u8p_dst:目标数据指针
**           u16_src:  源数据EEPROM存储地址
**           u16_len: 读取的字节数
** 输出参数: TRUE-读取正确   FALSE-读取失败
** 说明    : 无起始位置的限制，
**           读取长度为16位字节
**           自动实现跨页读出处理，并带有两次读出比对功能
*******************************************************************************************/
uint8 Read_EEPROM(uint8* u8p_dst, uint16 u16_src, uint16 u16_len)
{
 uint8 tmp;
 uint8 u8_lenth;

	while( u16_len > 0 )
	{
		tmp = (INT8U)(u16_src & (EE_PageMask -1));	// 源数据 在页当中的地址。 //
		if( tmp != 0 ) 
		{
			u8_lenth = EE_PageMask - tmp;			   	// 源数据首地址 到页尾的  长度 //
		}
		else 
		{
			u8_lenth = EE_PageMask;
		}
		if( u16_len < u8_lenth ) u8_lenth = u16_len;       				 // 

		if(Read_EEPROM_Page(u8p_dst, u16_src, u8_lenth) == FALSE)  return(FALSE);
		u8p_dst += u8_lenth;
		u16_src += u8_lenth;
		u16_len -= u8_lenth;
	} 
	return (TRUE);  
}

/*******************************************************************************************
** 函数名称: Write_EEPROM_Page
** 函数描述: 从EEPROM读取若干字节 （实现任意位置起始的16位字节长度数据写入）
** 输入参数: u16_dst:   目标数据EEPROM存储地址
**           *u8p_src:  源数据RAM数据首地址指针
**           u16_len: 写入的字节数
** 输出参数: TRUE-写入正确   FALSE-写入失败
** 说明    : 无起始位置的限制，
**           读取长度为16位字节
**           自动实现跨页写入处理，并带有写入读出比对功能
*******************************************************************************************/
uint8  Write_EEPROM(uint16 u16_dst, uint8 *u8p_src, uint16 u16_len)
{
 uint8 tmp;
 uint8 u8_lenth;
  
   while( u16_len > 0 )
   {
		tmp = (INT8U)(u16_dst & (EE_PageMask -1));	// 源数据 在页当中的地址。 //
		if( tmp != 0 ) 
		{
			u8_lenth = EE_PageMask - tmp;			   	// 源数据首地址 到页尾的  长度 //
		}
		else 
		{
			u8_lenth = EE_PageMask;
		}
		if( u16_len < u8_lenth ) u8_lenth = u16_len;       				 // 


       if(Write_EEPROM_Page(u16_dst, u8p_src, u8_lenth)==FALSE)  return(FALSE);
       u16_dst += u8_lenth;
       u8p_src += u8_lenth;
       u16_len -= u8_lenth;
   } 
   return (TRUE); 
}

/*******************************************************************************************
**    END
*******************************************************************************************/

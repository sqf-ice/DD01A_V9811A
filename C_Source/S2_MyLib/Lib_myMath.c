/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: Lib_myMath.c
**Author: maji
**date: 
**description: c code for general library function 
**note: memer type  DJ04A
**--------------------Version History -------------------------------------
** NO.	Date        		Ver    		By     			Description 
**==============================================================
** 1		2014-07-08  		0.1  			sosomj   			1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>


const INT8U MonTab[13] = 
{
	0,31,28,31,30,31,30,31,31,30,31,30,31
};

const INT16U DayTab[13] = 
{
	0,0,31,59,90,120,151,181,212,243,273,304,334 
};
//周日，一~六 对应 0~6
const char DefaultData[]={0,0,0,1,1,9,4}; //秒 分 时 日 月 年 周


/**********************************************************************
 turn byte data to BCD data
**********************************************************************/
INT8U Lib_byte_bcd(INT8U xx)
{
   if( xx > 99 ) return 0x00;
   return( xx/10*0x10 + xx%10 );
}

/**********************************************************************
 turn BCD data to byte data
**********************************************************************/
INT8U Lib_bcd_byte(INT8U xx)
{
   return(((xx>>4)&0x0F)*10+(xx&0x0F));
}

//**********************************************************************
// turn word data to BCD2 data
//**********************************************************************
void Lib_word_bcd2( INT8U *ptr, INT16U xx )
{
   if( xx > 9999 ) 
   {
      *(ptr+1) = 0x00;
      *ptr = 0x00;
      return;
   }

#ifdef LITTLE_END
   *(ptr+1) = Lib_byte_bcd( (INT8U)(xx/100) );
   *(ptr) =Lib_byte_bcd( (INT8U)(xx%100) );
#else
   *(ptr) = Lib_byte_bcd( (INT8U)(xx/100) );
   *(ptr+1) =Lib_byte_bcd( (INT8U)(xx%100) );
#endif

}

//**********************************************************************
// turn  BCD2 data to  word data 
//**********************************************************************
INT16U Lib_bcd2_word(INT8U* ptr)
{
#ifdef LITTLE_END
   return(  (INT16U)Lib_bcd_byte(*(ptr))+
            (INT16U)Lib_bcd_byte(*(ptr+1))*100 );
#else
   return(  (INT16U)Lib_bcd_byte(*(ptr+1))+
            (INT16U)Lib_bcd_byte(*(ptr))*100 );
#endif

}

//**********************************************************************
// turn long data to BCD4 data
//**********************************************************************
void Lib_long_bcd4(INT8U* ptr, INT32U xx)
{
	if( xx > 99999999 ) 
	{
      	*(ptr+3) = 0x00;
      	*(ptr+2) = 0x00;
      	*(ptr+1) = 0x00;
      	*ptr = 0x00;
      	return;
   	}

#ifdef LITTLE_END
	Lib_word_bcd2(ptr+2, xx/10000);
	Lib_word_bcd2(ptr, xx%10000);
#else
	Lib_word_bcd2(ptr, xx/10000);
	Lib_word_bcd2(ptr+2, xx%10000);
#endif


} 

//**********************************************************************
// turn BCD4 data to long data
//**********************************************************************
INT32U Lib_bcd4_long( INT8U* ptr )
{
#ifdef LITTLE_END
	return ( Lib_bcd_byte(*(ptr+0))+ (INT32U)(Lib_bcd_byte(*(ptr+1)))*100+ (INT32U)(Lib_bcd_byte(*(ptr+2)))*10000+(INT32U)(Lib_bcd_byte(*(ptr+3)))*1000000 );
#else
	return ( Lib_bcd_byte(*(ptr+3))+ (INT32U)(Lib_bcd_byte(*(ptr+2)))*100+ (INT32U)(Lib_bcd_byte(*(ptr+1)))*10000+(INT32U)(Lib_bcd_byte(*(ptr+0)))*1000000 );
#endif


}


/******************************************************************
*
*
*******************************************************************/
void Lib_bcd_inc ( INT8U* ptr )
{
	
	(*ptr)++;
	if( (*ptr & 0x0f) >= 0x0A )
	{
		(*ptr) &=~ 0x0f;
		*ptr+=0x10;
		if( ((*ptr>>4)&0x0f) >=0x0A )   *ptr =0;
	}
}


	
/******************************************************************
* KEIL C , ptr -->  HIGH ADDR
*
*******************************************************************/
INT8U Lib_bcd_arry_add1(INT8U* ptr, INT8U len)
{
  INT8U result;
    
    while(len > 0)
    {
    	result = 0;
    	Lib_bcd_inc (ptr);
    	if(*ptr != 0)  break;
        len--;
        ptr--;
        result=1;
    }
    return (result);
}


/****************************************************************
Function:  	INT16U Lib_get_csck_int16u_num( INT8U *ptr, INT8U len, INT16U key) 		
Description: 	获取16U格式的数据串的+key的累计校验和
Input:		INT8U *ptr: 校验数据串的首地址
			INT8U len :  校验数据串的长度
			INT16U key :  校验数据累计和的KEY			
Parameters:     
Returns:		INT16U数据，= 数据累加和+KEY
Others:	
****************************************************************/
INT16U Lib_get_csck_int16u_num( INT8U *ptr, INT16U len, INT16U key)
{      
        while( len > 0 )
        {
            key += *ptr;
            ptr++;
            len--;
        }
        return key;
}

/****************************************************************
Function:  	INT8U Lib_get_csck_int8u_num( INT8U *ptr, INT8U len, INT8U key)	
Description: 	获取8U格式的数据串的+key的累计校验和
Input:		INT8U *ptr: 校验数据串的首地址
			INT8U len :  校验数据串的长度
			INT8U key :  校验数据累计和的KEY			
Parameters:     
Returns:		INT8U数据，= 数据累加和+KEY
Others:	
****************************************************************/
INT8U Lib_get_csck_int8u_num( INT8U *ptr, INT16U len, INT8U key)
{      
        while( len > 0 )
        {
            key += *ptr;
            ptr++;
            len--;
        }
        return key;
}


/*******************************************************************************************
** 函数名称: BCDCHK
** 函数描述: 检查ptr指向的数据区中len个数据是否为BCD码元
** 输入参数: *ptr :要处理的数据区指针
**           len  :处理的长度
** 输出参数: TRUE/FALSE 
*******************************************************************************************/
uint8 Lib_BCDCHK(uint8 *ptr,uint8 len)
{	
	uint8 i;
	uint8 Temp;		

	i=0x00;
	do
	{		
		Temp = *ptr;		
		if( (Temp&0x0F)>0x09 || (Temp&0xF0)>0x90 ) return FALSE;		
		ptr++;
		i++;		
	}while(i<len);

	return TRUE;
}

/*********************************************************************
原型：uint GetGmDay( INT8U* Addr )
功能：日期转化为天数，
备注：时间基准从00年1月1日算起
*********************************************************************/
INT16U Lib_GetGmDay( INT8U* Addr ) //Addr：日
{
    INT16U Day,Mon,Year;
    
    Year= Lib_bcd_byte(*(Addr+2));
    Mon = Lib_bcd_byte(*(Addr+1));
    Day = Lib_bcd_byte(*(Addr)) + DayTab[Mon] + Year*365 + (Year>>2) + 1;//*Year 如果是4，111，移2位还剩1
    if( ((Year & 0x03) == 0) && (Mon <= 2) ) Day--;//**(Year & 0x03) == 0  闰年。 只算普通年，不包括世纪年。
    return Day;
}

/*********************************************************************
原型：ulong GetGmHour( INT8U* Addr )
功能：时间转化为小时数，
备注：时间基准从00年1月1日算起
*********************************************************************/
INT32U Lib_GetGmHour( INT8U* Addr )//Addr：时  
{
    return (INT32U)Lib_bcd_byte(*(Addr))//时
        + (INT32U)Lib_GetGmDay(Addr+1)*24;//日
}

/*********************************************************************
原型：ulong GetGmMin( INT8U* Addr )
功能：时间转化为分钟，
备注：时间基准从00年1月1日算起
*********************************************************************/
INT32U Lib_GetGmMin( INT8U* Addr ) //Addr：分 
{
    return (INT32U)Lib_bcd_byte(*(Addr))//分
        + (INT32U)Lib_GetGmHour(Addr+1)*60;//时
}

/*********************************************************************
原型：ulong GetGmSec( INT8U* Addr )
功能：时间转化为秒，
备注：时间基准从00年1月1日算起
*********************************************************************/
INT32U Lib_GetGmSec( INT8U* Addr ) //Addr：秒
{
    return (INT32U)Lib_bcd_byte(*(Addr))//秒
        + (INT32U)Lib_GetGmMin(Addr+1)*60;//分
}

/*****************************************************************************
** Function name    :Check_RTC_data_FORM
**
** Description         :检查RTC数据      
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
INT8U Lib_Check_RTC_data_FORM(INT8U* ClkData)
{
    //WEEKDAY//[0]
    if(ClkData[0] >6) return FALSE;
	
    //Year//[1]
    if(ClkData[1] > 0x99) return FALSE;			//   || ClkData[1] < 0x07
    if((ClkData[1] & 0x0F) > 0x09) return FALSE;

    //Mon//[2]
    if(ClkData[2] > 0x12 || ClkData[2] < 1) return FALSE;
    if((ClkData[2] & 0x0F) > 0x09) return FALSE;
    //Day//[3]
    if( ClkData[3] < 1 || ClkData[3] > ( ( ( ClkData[2] == 2 ) && ( Lib_bcd_byte(ClkData[1]) & 0x03 ) == 0 )? 0x29:Lib_byte_bcd( MonTab[Lib_bcd_byte(ClkData[2])] ) ) ) 	
    return FALSE;
    if((ClkData[3] & 0x0F) > 0x09) return FALSE;

    //Hour//[4]
    if(ClkData[4] > 0x23) return FALSE;
    if((ClkData[4] & 0x0F) > 0x09) return FALSE;
    //Min//[5]
    if(ClkData[5] > 0x59) return FALSE;
    if((ClkData[5] & 0x0F) > 0x09) return FALSE;
    //Sec//[6]
    if(ClkData[6] > 0x59) return FALSE;
    if((ClkData[6] & 0x0F) > 0x09) return FALSE;

	 return TRUE;
}

/*********************************************************************
原型：char GetWeek( void )
功能：获取当前星期
备注：时间基准从00年1月1日星期六 算起
*********************************************************************/
/*
char GetWeek(void)
{
	INT8U buftmp[7];
	Copy_Str_TwoArry(&buftmp[0], &rtc.year, 6);
	//
	Rev_Copy_Arry(&buftmp[0], 6);		//先颠倒顺序 （年月日时分秒->秒分时日月年）
    return (char)(( GetGmDay( (uchar*)&buftmp[3]) + REF_WEEK ) % 7);
}
*/


//**********************************************************************
//   数组的有效数据个数获取函数
//输入:	s1 有效数据数组高地址，len有效数据的数组总长度 (字节)
//                ENOB_len:有效数据的最小长度  (asc)
//输出:    输入数组的有效数据位个数
//**********************************************************************
INT8U  Lib_ENOB_Get(INT8U* s1, INT8U len,INT8U ENOB_len)
{
INT8U i;
      for(i=2*len; i>0; i--)
      {
          if(i<=ENOB_len) return (i);
          if(i&0x01)
          {
             if(((*s1)&0x0f) != 0)  return(i);  
	       s1++;
          }
	    else
	    {
	       if(((*s1)&0xf0) != 0)  return(i);  
	    }
      }
	  return 1;
}


//**********************************************************************
//单字节ASC码数据转BCD码，输出数据范围0:F 
//**********************************************************************
INT8U Lib_AtoB( INT8U Asc ) 
{
   INT8U Tmp=0x00;

    if ( Asc >= 0x30 && Asc <= 0x39 )
        Tmp = Asc - 0x30;
    else if ( Asc >= 0x41 && Asc <= 0x46 )
        Tmp = Asc - 0x37;
    return Tmp;
}

//**********************************************************************
//单字节BCD码数据转ASC码数据，输入数据范围要求0:F	 
//**********************************************************************
INT8U  Lib_BtoA( INT8U BCD ) 
{
   INT8U Tmp=0x00;

    if ( BCD < 10 )
        Tmp = BCD + 0x30;
    else if ( BCD >= 10 )
        Tmp = BCD + 0x37;
    return Tmp;
}

//**********************************************************************
//   Turn ASCII to BCD data Lenth是Asc数据的长度 固定为BCD的两倍
//  注:BCD数据和ASC的数据方向相反
//**********************************************************************
void Lib_Asc_BCD( INT8U *BCD, INT8U *Asc, INT8U Lenth )  
{
    unsigned char i;
    INT8U *Ptr;

    for ( i=(Lenth>>1); i>0; i-- )
    {
        Ptr = BCD + i - 1;
        *Ptr = Lib_AtoB( *Asc++ ) << 4;
        *Ptr += Lib_AtoB( *Asc++ );
    }
}
//**********************************************************************
//	Turn  BCD to ASCII data Lenth是Asc数据的长度 固定为BCD的两倍 
//  注:BCD数据和ASC的数据方向相同
//**********************************************************************
void Lib_Asc_BCDA( INT8U *BCD, INT8U *Asc, unsigned char Lenth ) 
{
    unsigned char i;
    INT8U *Ptr;

    Lenth >>= 1;
    for ( i=0; i<Lenth; i++ )
    {
        Ptr = BCD + i ;
        *Ptr = Lib_AtoB( *Asc++ ) << 4;
        *Ptr += Lib_AtoB( *Asc++ );
    }
}
//**********************************************************************
// Turn BCD to ASCII data,Lenth是BCD数据的长度
//  注:BCD数据和ASC的数据方向相反
//**********************************************************************
void Lib_BCD_Asc( INT8U *Asc, INT8U *BCD ,unsigned char Lenth ) 
{
   unsigned char i;
   INT8U *Ptr;

    for( i=Lenth; i>0; i-- )
    {
        Ptr = BCD + i - 1;
        *Asc++ = Lib_BtoA( (*Ptr>>4) & 0x0F );
        *Asc++ = Lib_BtoA( *Ptr & 0x0F );
    }
}
//**********************************************************************
// Turn BCD to ASCII data,Lenth是BCD数据的长度
//  注:BCD数据和ASC的数据方向一致	 
//**********************************************************************
void Lib_BCD_AscA( INT8U *Asc, INT8U *BCD ,unsigned char Lenth )  
{
   unsigned char i;
   INT8U *Ptr;

    for( i=0; i<Lenth; i++ )
    {
        Ptr = BCD + i;
        *Asc++ = Lib_BtoA( (*Ptr>>4) & 0x0F );
        *Asc++ = Lib_BtoA( *Ptr & 0x0F );
    }
}

//-------------------------------------------------------------------------------//
//  Function:             INT8U Get_Bcc_Chk(INT8U* yourBuf, INT8U StartID, INT8U EndID)
//  Description:          IEC 规约的BCC校验
//  Parameters:          INT8U* yourBuf  通讯缓存数组首地址
//                            
//  Returns:               无
//  Others:                1适用于DL/T645-1997/2007
//                            2 
//  author:                 sosomj         2009.02.16      V01.00
//-------------------------------------------------------------------------------//
INT8U Lib_math_get_BCC_chk(INT8U* yourBuf, INT8U StartID, INT8U EndID)
{
    INT8U i;
    INT8U tmp;

    tmp=0x00;
    for(i=StartID;i<=EndID;i++)
    {
        tmp ^= *(yourBuf+i);
    }
    return tmp;
}
//---------------------------------------------------------------------------------------------------------------------//
//     * @brief  检测数据中1的个数，返回0表示含有有偶数个数的1，返回1表示有奇数个数的1
//     * @param  value 待检测的数据
//     * @return  0---带检测数据中有偶数个1
//     *              1---带检测的数据中奇数个1
//---------------------------------------------------------------------------------------------------------------------//
unsigned char Lib_check_bitnum(unsigned char value)
{
      uint8 i,temp_data,re_value;
      temp_data=value;
      re_value=0;
      for(i=0;i<8;i++)
      { 
          if((temp_data&0x01)==1)
          {
              re_value++;  
          }
          temp_data=temp_data>>1;
      }
      re_value=re_value%2;
      return re_value;
}

/***************************************************************
*    END
****************************************************************/


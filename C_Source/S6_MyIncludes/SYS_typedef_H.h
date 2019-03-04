/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: Config.H
**Author: maji
**date: 
**description:  重定义数据类型文件 (C51项目内使用)
**note: 和MCU无关，通用文件
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-18   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#ifndef 	_SYS_typedef_H
#define	_SYS_typedef_H


/*******************************************************************************/
/*                        CPU无关性定义                                        */
/*******************************************************************************/
typedef	bit				   Bit;
typedef	bit				   Bool;

typedef unsigned char      uint8;                   //无符号8位整型变量//
typedef	unsigned char	   Byte;
typedef	unsigned char	   BCD8;
typedef signed   char      int8;                    //有符号8位整型变量//

typedef unsigned short     uint16;                  //无符号16位整型变量//
typedef	unsigned int	   Word;
typedef signed   short     int16;                   //有符号16位整型变量//

typedef unsigned long      uint32;                  //无符号32位整型变量
typedef	unsigned long	   Dword;
typedef signed   long      int32;                   //有符号32位整型变量

typedef float              fp32;                    //单精度浮点数（32位长度）
typedef double             fp64;                    //双精度浮点数（64位长度）


typedef union
{
	uint16  u16;
	uint8 B08[2];
	struct
	{
      uint8 buf1:8;
      uint8 buf0:8;
	}TBuf;
}ST_U16_U08;

typedef union
{
	uint32  u32;
	uint16  W16[2];
	uint8 B08[4];
	struct
	{
      uint8 buf3:8;
      uint8 buf2:8;
      uint8 buf1:8;
      uint8 buf0:8;
	}TBuf;
}ST_U32_U08;

typedef union
{
  uint8 byte;
  struct{
      uint8 b0:1;
      uint8 b1:1;
      uint8 b2:1;
      uint8 b3:1;
      uint8 b4:1;
      uint8 b5:1;
      uint8 b6:1;
      uint8 b7:1;
  }Bits;
}ST_BIT_Group;


//////////////////////////////////////////////////////////////
typedef 	unsigned char  BOOL;

typedef 	unsigned char  BOOLEAN;
typedef 	signed char	INT8S;
typedef unsigned 	char	INT8U;
typedef 	signed	int	INT16S;
typedef unsigned 	int	INT16U;
typedef signed	long	INT32S;
typedef unsigned 	long	INT32U;
typedef unsigned 	char	uchar;

/**data */
typedef signed long  data sdata32;
typedef signed short data sdata16;
typedef signed char  data sdata8;


typedef volatile signed long data vsdata32;
typedef volatile signed short data vsdata16;
typedef volatile signed char  data vsdata8;


typedef unsigned long  data udata32;
typedef unsigned short data udata16;
typedef unsigned char  data udata8;


typedef volatile unsigned long  data vudata32;
typedef volatile unsigned short data vudata16;
typedef volatile unsigned char  data vudata8;
/**idata*/
typedef signed long idata sidata32;
typedef signed short idata sidata16;
typedef signed char  idata sidata8;


typedef volatile signed long  idata vsidata32;
typedef volatile signed short idata vsidata16;
typedef volatile signed char  idata vsidata8;


typedef unsigned long  idata uidata32;
typedef unsigned short idata uidata16;
typedef unsigned char  idata uidata8;


typedef volatile unsigned long  idata vuidata32;
typedef volatile unsigned short idata vuidata16;
typedef volatile unsigned char  idata vuidata8;

/**xdata*/
typedef signed long  xdata sxdata32;
typedef signed short xdata sxdata16;
typedef signed char  xdata sxdata8;


typedef volatile signed long  xdata vsxdata32;
typedef volatile signed short xdata vsxdata16;
typedef volatile signed char  xdata vsxdata8;


typedef unsigned long  xdata uxdata32;
typedef unsigned short xdata uxdata16;
typedef unsigned char  xdata uxdata8;


typedef volatile unsigned long  xdata vuxdata32;
typedef volatile unsigned short xdata vuxdata16;
typedef volatile unsigned char  xdata vuxdata8;


union Union_WordDef
{
	unsigned int WORD;
	unsigned char BYTE[2];
};
union Union_DWordDef
{
	signed long   DWORD;
	unsigned long  UDWORD;
	unsigned int WORD[2];
	unsigned char BYTE[4];
};

/***************************************************************
*    END
****************************************************************/

#endif

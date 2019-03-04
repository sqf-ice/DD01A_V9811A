/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_EMU_hard.c
**Author		: maji
**date			: 2016-04-20 
**description	: V9811A的EMU计量模块硬件底层驱动C代码
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>



/*******************************************************************************************
** 函数名称: Open_EmuPwr
** 函数描述: 打开EMU基准电源
** 输入参数: 无
** 输出参数: 无
** 说明    : 
*******************************************************************************************/
/*void Open_EmuPwr(uint8 mode)
{
    INT8U temp;

    temp=0;
    while(!temp)
        temp=SetPLL(SETPLL_3DOT2M);

   if(mode ==EMU_NORMALL_MODE)
   {
        //ADC SET  //
        CtrlADC5=0; //m channel measure temperature new sensor
	CtrlADC6=0;
	ADCOPEN(ADCAPDN+ADCUPDN);//OPEN ADC U A 
       CtrlADC0=(ADCA16X+ADCU2X); //ADC A 16X GAIN  ADC B 4x GAIN ADC U 2X GAIN	      
   }

        //     SP 初始化  // 
    PMG=0;
    SetMeterCfgACK(0x00,PMCtrl4);
    SetMeterCfgACK(0x00,PMCtrl1);
    SetMeterCfgACK(INITGATEP,GATEP);//设置计量初始值
    SetMeterCfgACK(INITGATEQ,GATEQ);
    SetMeterCfgACK(0,0x10f0);
    SetMeterCfgACK(0,0x10f1);
    SetMeterCfgACK(0,0x10f2);
    SetMeterCfgACK(0,0x10f3);
    SetMeterCfgACK(0,0x10f6);
    SetMeterCfgACK(0,0x10f7);
    SetMeterCfgACK(0,0x10f8);
    SetMeterCfgACK(0,0x10f9);
    SetMeterCfgACK(0xC8,PMCtrl4);		// 待定
    SetMeterCfgACK(0x08,PMCtrl4);
    SetMeterCfgACK(0x00,PMCtrl4);
    SetMeterCfgACK(0,0x10f0);
    SetMeterCfgACK(0,0x10f1);
    SetMeterCfgACK(0,0x10f2);
    SetMeterCfgACK(0,0x10f3);
    SetMeterCfgACK(0,0x10f6);
    SetMeterCfgACK(0,0x10f7);
    SetMeterCfgACK(0,0x10f8);
    SetMeterCfgACK(0,0x10f9);
    SetMeterCfgACK(0x01,GATECP);//潜动1  //
    SetMeterCfgACK(0x01,GATECQ);//潜动2   //
    SetMeterCfgACK(0x00,DATACP);//常数功率寄存器 //

    SetMeterCfgACK(0x57,PMCtrl1);//开启所有通道
    SetMeterCfgACK(0x00,PMCtrl2);//
    SetMeterCfgACK(0x30,PMCtrl3);//开启两路有功，次谐波抑制
    SetMeterCfgACK(0xc0,PMCtrl4);//等待滤波器稳定后开启
    SetMeterCfgACK(0,PHCCtrl1);//清0
       SetMeterCfgACK(0,PHCCtrl2);//清0   

} */

/*******************************************************************************************
** 函数名称: Close_EmuPwr
** 函数描述: 关闭EMU基准电源
** 输入参数: 无
** 输出参数: 无
** 说明    : 
*******************************************************************************************/
void Close_EmuPwr(void)
{

}

/*******************************************************************************************
** 函数名称: OpenCF
** 函数描述:  打开CF计量功能
** 输入参数: 无
** 输出参数: 
** 说明    : 
*******************************************************************************************/
void OpenCF(void)
{
    if((EIE&BIT1)!=BIT1)       EIE|=BIT1;
    if((ExInt3IE&BIT7)!=BIT7)  ExInt3IE|=BIT7;     // 开启CF2中断 //

}


/*******************************************************************************************
** 函数名称: Close_Cf
** 函数描述:  关闭CF计量功能
** 输入参数: 无
** 输出参数: 
** 说明    : 
*******************************************************************************************/
void Close_Cf(void)
{   
	INT32U PmPara;

	PmPara = ReadMeterParaACK(PMCtrl4);
	PmPara &= ~(BIT4+BIT3+BIT5);     
	SetMeterCfgACK(PmPara,PMCtrl4);  //禁止E1 、E2  、累加和脉冲输出和计数 //

	ExInt2IE &= ~BIT7;           // 关闭CF中断 //
   // 输出GPIO 禁止//
	P1OD|= BIT3;
}


/*******************************************************************************************
** 函数名称: SetMeterCfgACKPM800K
** 函数描述: 双口ram设置，使用ACK信号
** 输入参数: u32PMdatal 数据
**                      : addr  地址
** 输出参数: FALSE 设置失败；TRUE 设置成功;
** 说明    : 
*******************************************************************************************/
INT8U SetMeterCfgACKPM800K(unsigned long u32PMdatal ,unsigned int addr)
{
    unsigned char  index;
    index=0;
	
    XBYTE[INVD]=0XCC;
    XBYTE[INVD]=0x00;
    while(XBYTE[ACK])
     {
       index++;
	   if(index>CFGDELAY)
	   	{
		  return FALSE	;
	   	}
     }
   XBYTE[BUFF0]=(INT8U)(u32PMdatal&0xff);
   XBYTE[BUFF1]=(INT8U)((u32PMdatal>>8)&(0xff));
   XBYTE[BUFF2]=(INT8U)((u32PMdatal>>16)&(0xff));
   XBYTE[BUFF3]=(INT8U)((u32PMdatal>>24)&(0xff));
   XBYTE[addr]=0;//向真正的地址写入0，启动缓冲输入到真正的地址
   for(index=0;index<CFGDELAY;index++)
  	_nop_();
   index=0;
   while(XBYTE[ACK])
      {
	  index++;
	  if(index>CFGDELAY)
		 return FALSE	;
      }
   return TRUE	;
    
}




/*******************************************************************************************
** 函数名称: SetMeterCfgI
** 函数描述: 双口ram设置，使用ACK信号
** 输入参数: u32PMdatal 数据
**                      : addr  地址
** 输出参数: FALSE 设置失败；TRUE 设置成功;
** 说明    : 
*******************************************************************************************/
INT8U SetMeterCfgI(unsigned long u32PMdatal ,unsigned int addr)
{
    unsigned char  index,i;
    index=0;
	
    XBYTE[INVD]=0XCC;
    XBYTE[INVD]=0x00;
    while(XBYTE[ACK])
     {
       index++;
	   if(index>CFGDELAY)
	   	{
		  return FALSE	;
	   	}
     }
   XBYTE[BUFF0]=(INT8U)(u32PMdatal&0xff);
   XBYTE[BUFF1]=(INT8U)((u32PMdatal>>8)&(0xff));
   XBYTE[BUFF2]=(INT8U)((u32PMdatal>>16)&(0xff));
   XBYTE[BUFF3]=(INT8U)((u32PMdatal>>24)&(0xff));
   
   MEAFRQ=1;
	 while(!(MEAFRQ))
		{
		   i++;
		   if(i>30)
		   {
		      break ; //在一定时间内没有锁定
		   }
		}  
   XBYTE[addr]=0;//向真正的地址写入0，启动缓冲输入到真正的地址
  for(index=0;index<CFGDELAY;index++)
  	_nop_();
   index=0;
   while(XBYTE[ACK])
      {
	  index++;
	  if(index>CFGDELAY)
		 return FALSE	;
      }
    
   return TRUE	;
    
}


/*******************************************************************************************
** 函数名称: SetMeterCfgACK
** 函数描述: 双口ram设置，使用ACK信号
** 输入参数: u32PMdatal 数据
**                      : addr  地址
** 输出参数: FALSE 设置失败；TRUE 设置成功;
** 说明    : 
*******************************************************************************************/
INT8U SetMeterCfgACK(unsigned long u32PMdatal ,unsigned int addr)
{
    unsigned char  index;
    index=0;
	
    XBYTE[INVD]=0XCC;
    XBYTE[INVD]=0x00;
    while(XBYTE[ACK])
     {
       index++;
	   if(index>CFGDELAY)
	   	{
		  return FALSE	;
	   	}
     }
   XBYTE[BUFF0]=(INT8U)(u32PMdatal&0xff);
   XBYTE[BUFF1]=(INT8U)((u32PMdatal>>8)&(0xff));
   XBYTE[BUFF2]=(INT8U)((u32PMdatal>>16)&(0xff));
   XBYTE[BUFF3]=(INT8U)((u32PMdatal>>24)&(0xff));
   XBYTE[addr]=0;//向真正的地址写入0，启动缓冲输入到真正的地址
  for(index=0;index<CFGDELAY;index++)
  	_nop_();
   index=0;
   while(XBYTE[ACK])
      {
	  index++;
	  if(index>CFGDELAY)
		 return FALSE	;
      }
    
  return TRUE;  
}


/*******************************************************************************************
** 函数名称: ReadMeterParaACK
** 函数描述: 双口ram设置，使用ACK信号
** 输入参数: u32PMdatal 数据
**                      : addr  地址
** 输出参数: FALSE 设置失败；TRUE 设置成功;
** 说明    : 
*******************************************************************************************/
uint32 ReadMeterParaACK(uint16 addr)
{
    uint8 data index;
    uint32  u32PMdatal,temp;

    XBYTE[INVD]=0XCC;
    XBYTE[INVD]=0x00;
    index=0;
    while(XBYTE[ACK])
    {
        index++;
        if(index>CFGDELAY)
        {
        return FALSE;
        }
    }

    index=XBYTE[addr];//读取真实地址，加载数据到缓冲//
    for(index=0;index<CFGDELAY;index++)
    _nop_();
    index=0;
    while(XBYTE[ACK])
    {
        index++;
        if(index>CFGDELAY)
        {
        return FALSE;
        }
    }
    u32PMdatal=(INT32U)XBYTE[BUFF0];
    temp=(INT32U)XBYTE[BUFF1]<<8;
    u32PMdatal+= temp;
    temp=(INT32U)XBYTE[BUFF2];
    temp=temp<<16;
    u32PMdatal+=temp;
    temp=(INT32U)XBYTE[BUFF3];
    temp=temp<<24;
    u32PMdatal+=temp;
    return u32PMdatal;
	 
}


/*******************************************************************************************
**    END
*******************************************************************************************/

/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_RTC.c
**Author		: maji
**date			: 2016-04-20 
**description	: RTC模块的硬件驱动C代码
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
** 函数名称: Open_SecOut
** 功能描述: 打开秒输出功能
** 入口参数: 无
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void Open_SecOut(void)
{
    P13FS = 0x06; // PLL秒脉冲输出
		
    P1OE&=~BIT3;	// output 
    P1IE&=~BIT3;	//
}

/*******************************************************************************************
** 函数名称: Close_SecOut
** 功能描述: 关闭秒输出功能
** 入口参数: 无
** 出口参数: 无
** 说明    : 
*******************************************************************************************/


/*******************************************************************************************
** 函数名称: Init_RTC
** 功能描述: 初始化RTC
** 入口参数: 无
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void Init_RTC(void)
{
	EIE|=BIT1;
	ExInt3IE|=BIT6;
}

/*******************************************************************************************
** 函数名称: SetExtRTC
** 功能描述: 设置RTC中断唤醒间隔
** 入口参数: uint8 SEC: intRTC=7的时候配置SEC秒唤醒
**                        uint8 intRTC :intRTC 配置说明: 0, 1 秒；1, 1 分；2, 1 小时；3, 1 天；4, 500 毫秒；
**                                           5, 250 毫秒；6, 125 毫秒；7, 配合SECINT设置任意秒中断
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void SetExtRTC(uint8 SEC,uint8 intRTC)
{
    //RTC允许写 
    RTCPEN = 0x96;      //password 1
    RTCPWD = 0x57;      //password 2
    Lib_Delay_2us(50);
	
    INTRTC = intRTC;
/////////////////////////////////////////
//设置任意秒中断  2016-2-18 mj//
    if(intRTC ==7)
    {
        SECINT = BIT6 ;
        SECINT += SEC&0x3f ;    
    }
/////////////////////////////////////////

    Lib_Delay_2us(50);
    //RTC禁止写  
    RTCPEN = 0x96;
    RTCPWD = 0x56;
    Lib_Delay_2us(50);

}

/*******************************************************************************************
** 函数名称: Get_RTCTime
** 函数描述: 获取RTC时间
** 输入参数: 无
** 输出参数: 无 
** 说明    :  //  0-秒 1-分 2-时 3-周 4-日 5-月 6-年 //  
*******************************************************************************************/
void Get_RTCTime(RTC_TYPE *pTime)
{
volatile INT8U RTCFLAG ;

    RTCFLAG=RTCLATCH;     //很重要，本操作用于锁定RTC数据，保证在读取的过程中不变化  //
    Lib_Delay_2us(100);
	
    pTime->Week = RTCWC;             // weekday  // 
    pTime->Year = RTCYC;              // year  // 
    pTime->Month = RTCMoC;               // month  // 
    pTime->Day = RTCDC;               // day  // 
    pTime->Hour = RTCHC;              // hour  // 
    pTime->Minute = RTCMiC;              // minu  // 
    pTime->Second = RTCSC;              // sec  // 
}
	
/*******************************************************************************************
** 函数名称: Get_RTCTime
** 函数描述: 获取RTC时间
** 输入参数: 无
** 输出参数: 无 
** 说明    : //  0-秒 1-分 2-时 3-周 4-日 5-月 6-年 //  
*******************************************************************************************/
uint8 Set_RTCTime(RTC_TYPE *pTime)
{
	//RTC允许写 
    RTCPEN = 0x96;      //password 1
    RTCPWD = 0x57;      //password 2
    Lib_Delay_2us(50);
    //写RTC时间
    RTCWC = pTime->Week;		//weekday
    RTCYC = pTime->Year;		//year
    RTCMoC = pTime->Month;	//month
    RTCDC = pTime->Day;		//day
    RTCHC = pTime->Hour;		//hour
    RTCMiC = pTime->Minute;	//minu
    RTCSC = pTime->Second;		//sec
    Lib_Delay_2us(50);
    //RTC禁止写  

    RTCPEN = 0x96;
    RTCPWD = 0x56;
   Lib_Delay_2us(50);

    return (TRUE);
}



/*******************************************************************************************
** 函数名称: Clr_RTC_AdjParam
** 函数描述: 清除RTC的校正寄存器
** 输入参数: 无
** 输出参数: 无 
** 说明    :  无温补处理的表要求在上电初始的时候调用本函数
*******************************************************************************************/
void Clr_RTC_AdjParam(void)
{
	//RTC允许写 
    RTCPEN = 0x96;      //password 1
    RTCPWD = 0x57;      //password 2
    Lib_Delay_2us(50);
	RTCCH = 0x00;
	RTCCL = 0x00;
    //RTC禁止写  

    RTCPEN = 0x96;
    RTCPWD = 0x56;
	Lib_Delay_2us(50);

}

/*******************************************************************************************
** 函数名称: SetRTCNormal
** 函数描述: 校正RTC频率
** 输入参数: int16 offset
** 输出参数: 无 
** 说明    : 温补函数中调用
*******************************************************************************************/
void SetRTCNormal(int16 offset)
{
    ST_U32_U08 temp1,temp2;
    int16 temp3;

    temp2.u32=0;
    temp1.u32 =(uint16)offset;
    if((temp1.B08[2]>0x80))
    { 
        temp1.W16[1]=(~temp1.W16[1]);
        temp1.W16[1]&=0x1fff;
        temp1.u32=temp1.u32*20/30; 
        temp2.u32=(6553600/2) -1;
        temp2.u32=temp2.u32+(temp1.u32/2);
    }
    else
    {
        temp1.u32=(temp1.u32)*20/30;
        temp2.u32=(6553600/2);
        temp2.u32=temp2.u32-(temp1.u32/2);
    }

    temp3=offset/10;
    temp1.u32=(uint16)temp3;
    RTCPEN=0x96;              //          // 1S唤醒//
    RTCPWD=0x57;
    Lib_Delay_2us(50);
    RTCCH=temp1.B08[2];
    RTCCL=temp1.B08[3];
    DIVTHH=temp2.B08[1];
    DIVTHM=temp2.B08[2];
    DIVTHL=temp2.B08[3];

    Lib_Delay_2us(50);
    RTCPEN=0x96;
    RTCPWD=0x56;
}


/*******************************************************************************************
** 函数名称: Handl_RTC_Adj_per_minu
** 函数描述: RTC温补控制程序
** 输入参数: 无
** 输出参数: 无 
** 说明    :  无温补处理的表要求在上电初始的时候调用本函数
*******************************************************************************************/
void Handl_RTC_Adj_per_minu(void)
{
 uint8 i;  
 uint8 code *p;
 ST_U32_U08 temp1;
 float temperature; //当前温度//

 int16 delta;
 int16  OSC;
 int32 Bpara;
 uint8 Ti;


    return;

    Bpara = 0x061A80;
    Ti= 24;
    p=(uint8 code*)0x48C;		// RTC常温偏移//

    for(i=0;i<3;i++)
    {
        Lib_FCpyTMem((uint8*)&temp1.B08[0],p+(i*4),4);	
        temp1.B08[2] = Lib_get_csck_int8u_num((uint8*)&temp1.B08[0],2,0x33);
        if(temp1.B08[2] == temp1.B08[3])
        {
            break;
        }
    }

    if(temp1.B08[2] != temp1.B08[3])
    {
        temp1.W16[0]= 0;		// 校验出错 默认
    }
    delta=temp1.W16[0];

    //获取温度  //
    Enable_M_ADC(M_ADC_CH1_TEMPERATURE);
    Lib_Delay_ms(10);
    temperature = Cal_temperature_data();
    //计算数据  //
    OSC=(int16)((((float)Bpara)*(temperature-((float)Ti))*(temperature-((float)Ti)))/1000000+delta);		// 精工提供//
    //校准数据  //
    SetRTCNormal(OSC);

}



/*******************************************************************************************
**    END
*******************************************************************************************/

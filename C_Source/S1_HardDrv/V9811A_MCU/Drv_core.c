/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_core.c
**Author		: maji
**date			: 2016-04-20 
**description	: MCU core内核模块的硬件驱动C代码
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
** 函数名称: Set_McuClock
** 函数描述:  MCU主频配置
** 输入参数: osc :   FSYSCLK_800k    FSYSCLK_1M6 
**                         PHZ :  50 0r  60 HZ
** 说明    : 
*******************************************************************************************/
uint8 Set_McuClock(uint32 u32_freq )
{
    uint8 i=0;

    FWC=0;
    FSC=0;
    CtrlCLK|=(BIT6+BIT7); // 开启BGP 以及PLL电路//
#if (C_Fq == 60)
    CtrlPLL = BIT5;
#else
    CtrlPLL = 0; 
#endif           

    switch(u32_freq)
    {
        case FSYSCLK_800k:
            CtrlCLK&=~(BIT0+BIT1);   // 00为 1x FSYSCLK_800k
            CtrlPLL =0;     //BIT5 =0 为50HZ
            break;

        case FSYSCLK_1M6:
            CtrlCLK |= BIT0;
            CtrlCLK&=~(BIT1);   // 10为 2x FSYSCLK_800k
            CtrlPLL =0;     //BIT5 =0 为50HZ
            break;

        case FSYSCLK_3M2:
            CtrlCLK&=~(BIT0);   // 10 为 4x FSYSCLK_800k
            CtrlCLK |= BIT1;
            break;

        case FSYSCLK_6M5:
            CtrlCLK |= BIT0;   // 11 为  8x FSYSCLK_800k
            CtrlCLK |= BIT1;
            break;

        case FSYSCLK_13M:
            CtrlCLK |= BIT0;     // 11 为  8x FSYSCLK_800k
            CtrlCLK |= BIT1;
            CtrlPLL |=BIT6;              // mcuclock x 2
            break;

        case FSYSCLK_26M:
            CtrlCLK |= BIT0;     // 11 为  8x FSYSCLK_800k
            CtrlCLK |= BIT1;
            CtrlPLL |=(BIT7+BIT6);       //  mcuclock x 4 
            break;

        default:
            sys_err();	
        break;
    }

    while(!(PLLLCK&BIT0))
    {
        i++;
        if(i>50)
        {
            return FALSE;       //在一定时间内没有锁定
        }
    }
    MCUFRQ=1; //MCU时钟源: PLL时钟
    i=0;

    while(!(MCUFRQ))
    {
        i++;
        if(i>50)
        {
            return FALSE;       //在一定时间内没有锁定
        }
    }

    CtrlCry0=0;                 //调整启振波形
    CtrlCry1=3;                // 使用RTC   
    CtrlCry2|=BIT5;        // 停振复位功能//

    return TRUE;
}

/*******************************************************************************************
** 函数名称: Set_EmuClock
** 函数描述: 计量时钟设置
** 输入参数: Tpye:   EMUCLK_OFF 关计量
                              EMUCLK_800k  时钟选择
**                         
** 说明    : 注意EMU的主频必须是ADC主频的4倍或8倍
*******************************************************************************************/
uint8 Set_EmuClock(uint32 u32_freq)
{
    uint8 i=0;

    PMG = 0;
    CtrlCLK|=(BIT6+BIT7); // 开启BGP 以及PLL电路//
 
    switch(u32_freq)
    {
        case EMUCLK_OFF:
            PMG = 1;
            return TRUE;

        case EMUCLK_800k:
            CtrlCLK&=~(BIT3+BIT2);   // 00为  1*EMUCLK_800k
            CtrlCLK&=~(BIT5+BIT4);   // 00为  1*ADCCLK_204k
            break;

        case EMUCLK_1M6:
            CtrlCLK |= BIT2;
            CtrlCLK&=~(BIT3);   // 01 为 2x EMUCLK_800k

        	CtrlCLK |= BIT4;
        	CtrlCLK&=~(BIT5);   // 01 为 2x ADCCLK_204k
            break;
	
        case  EMUCLK_3M2:
            CtrlCLK&=~(BIT2);   // 10 为 4x EMUCLK_800k
            CtrlCLK |= BIT3;

            CtrlCLK&=~(BIT4);   // 10 为 4x ADCCLK_204k
            CtrlCLK |= BIT5;
            break;

        default:
            sys_err();	
        break;
   }

    while(!(PLLLCK&BIT0))
    {
        i++;
        if(i>50)
        {
            return FALSE;       //在一定时间内没有锁定
        }
    }

    MEAFRQ=1;   //MEA时钟源: PLL时钟
    i=0;
    while(!(MEAFRQ))
    {
        i++;
        if(i>50)
        {
            return FALSE;       //在一定时间内没有锁定
        }
    }
	
    return TRUE;
}

/*******************************************************************************************
** 函数名称: Set_McuSleep
** 函数描述: 系统休眠处理
** 输入参数: NONE
** 输出参数: 无
** 说明    : 
*******************************************************************************************/
void Set_McuSleep(void)
{
    uint8 i;
    FWC=0;
    FSC=0;

    CtrlADC6=0;             //关闭所有的ADC
    CtrlLDO = 0x00;

   for(i=0;i<20;i++);      //等待
    PMG=1;                  //关闭计量时钟

//   	Full_SEG(0x00);
    if((Systate&BIT0)==0x01)
    {   
        sys_err();	  //如果上电则休眠处理 //
    }

    MCUFRQ=0;
    while(MCUFRQ);

    if((Systate&BIT0)==0x01)
    {   
        sys_err();	  //如果上电则休眠处理 //
    }

    for(i=0;i<3;i++);       //等待//
    CtrlCLK=0x00;
    while(CtrlCLK);

    SLEEP0 = 1;
    Lib_Delay_2us(50);    // 5 OSC //
    NOP();
//	if((Systate&BIT0)==0x01)  //休眠以后人工上电全屏自检
//    {   
//        sys_err();	  //如果上电则休眠处理 //
//    }
}


/*******************************************************************************************
** 函数名称: Set_McuSleep
** 函数描述: 系统休眠处理
** 输入参数: NONE
** 输出参数: 无
** 说明    : 
*******************************************************************************************/
void Set_Sleep_out(void)
{
 uint8 i;



   I2C_DIO_OUT;
   I2C_PORT_OUT |=PIN_E2SDA;    
   I2C_CLK_OUT;
   I2C_CLK_HIGH;
	
//关闭UART
    TCON2=0;
    TCON3=0;
    TCON4=0;
    CFGA=0;
// 关AD//
    CtrlADC6=0;                         //关AD通道
    CtrlLDO|=BIT5;                      //关LDO

    SetMeterCfgACK(0x00,PMCtrl4);   //关数字的AD通道
    SetMeterCfgACK(0x00,PMCtrl1);   //关CF脉冲

   for(i=0;i<20;i++);
   
   PMG=1;                              //关闭计量时钟


}


/*******************************************************************************************
**    END
*******************************************************************************************/

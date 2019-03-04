/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_ADC.c
**Author		: maji
**date			: 2016-04-20 
**description	: ADC模块的硬件驱动C代码
**note			: V9811A ，
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>

//RTC温度曲线K系数
code INT8U InitParaRTC[64]=
{
    0x03,0xE5,	//0.997 
    0x03,0xE8, // 1.000 
    0x03,0xEC, // 1.004 
    0x03,0xEF,// 1.007 
    0x03,0xD9,	//0.985 
    0x03,0xDC,	//0.988 
    0x03,0xE0,	//0.992 
    0x03,0xE3,	//0.995 
    0x03,0xD3,	//0.979 
    0x03,0xD6,	//0.982 
    0x03,0xD9,	//0.985 
    0x03,0xDC,	//0.988 
    0x03,0xC6,	//0.966 
    0x03,0xC9,	//0.969 
    0x03,0xCC,	//0.972 
    0x03,0xCF,	//0.975 
    0x04,0x10,	// 1.040 
    0x04,0x13,	// 1.043 
    0x04,0x16,	// 1.046 
    0x04,0x19,	// 1.049 
    0x04,0x03,	// 1.027 
    0x04,0x07,  //	//1.031 
    0x04,0x0A,  //	//1.034 
    0x04,0x0D,  //	//1.037 
    0x03,0xFA,  //	//1.018 
    0x03,0xFD,  //	//1.021 
    0x04,0x00,  //	//1.024 
    0x04,0x03,  //	//1.027 
    0x03,0xED, //	//1.005 
    0x03,0xF1, //	//1.009 
    0x03,0xF4, //	//1.012 
    0x03,0xF7, //	//1.015 
};


/*******************************************************************************************
** 函数名称: Enable_M_ADC
** 函数描述: 开通M通道指定的 测量功能
** 输入参数: uint8  m_ch  [M_ADC_CH1_TEMPERATURE ,  M_ADC_CH2_VBAT]
** 输出参数: 无 
** 说明    : 开通后10ms延时后才可以获取数据
*******************************************************************************************/
void Enable_M_ADC(uint8  m_ch)
{
INT32U ctmp;
 

    if(m_ch == M_ADC_CH1_TEMPERATURE)   
    {
        CtrlADC5 = (BIT7+BIT0);  //CH1- 测温度通道,此处为M通道专用控制器，采用直接赋值方式  //           
    } 
    else  if(m_ch == M_ADC_CH2_VBAT)   
    {
     // M2通道//
        CtrlADC5 = (BIT7+BIT5+BIT1);  // CH2- 测VBAT通道, ，采用直接赋值方式  //            
    }
    else
    {
        return;
    }  

    CtrlADC6 |= BIT3;             //BIT3 =1  开启M通道ADC   ，此处为多处复用控制器，采用或方式赋值 //
    CtrlM |= BIT0;

    ctmp = ReadMeterParaACK(PMCtrl1);
    ctmp |= (BIT3+BIT4);                //BIT3 =1使能M通道数字信号输入； BIT4 =1使能M通道数字信号处理 //
    SetMeterCfgACK( ctmp,PMCtrl1);    

}

/*******************************************************************************************
** 函数名称: Disable_M_ADC
** 函数描述: 关闭M通道ADC功能
** 输入参数: 无
** 输出参数: 无 
** 说明    : 为了降低电池在检测时的功耗，所以需要关闭
*******************************************************************************************/
void Disable_M_ADC(void)
{
INT32U ctmp;

   	CtrlADC6 &=~BIT3;   //BIT3 =0 关闭M通道ADC   ，此处为多处复用控制器，采用与方式赋值 //
 	CtrlADC5 = 0x80;      //测量内部低  // 默认测试温度
//	CtrlADC5 = 0x81;      //测量内部低  // 默认测试温度
	CtrlM |= BIT0;
	ctmp = ReadMeterParaACK(PMCtrl1);
	ctmp &=~ BIT3;      //BIT3 =0关闭M通道数字信号输入 //
	SetMeterCfgACK( ctmp,PMCtrl1);     
}


/*******************************************************************************************
** 函数名称: Samlpe_BatAdc
** 函数描述: 获取VBAT管脚的M通道的测量直流电压
** 输入参数: 无
** 输出参数: uint16 ，电池电压值，量纲:0.01V
** 说明    : 
*******************************************************************************************/
uint16  Samlpe_BatAdc(void)
{
 uint16 nVBAT;
 uint32 u32_PMdataltemp;

    Enable_M_ADC(M_ADC_CH2_VBAT);
    Lib_Delay_ms(10);

    u32_PMdataltemp =ReadMeterParaACK(DATAOM);
    nVBAT =(uint16)(u32_PMdataltemp>>16);
    if (nVBAT& 0x8000) 
    {
       // nVBAT= 0;
       nVBAT=(~nVBAT)+1;
    }
    else
    {
        nVBAT = nVBAT/71-5;		//电容方式 分压//
    }

    NOP();
  // Disable_M_ADC();
    return nVBAT;
}


/*******************************************************************************************
** 函数名称: Samlpe_BatAdc
** 函数描述: 获取VBAT管脚的M通道的测量直流电压
** 输入参数: 无
** 输出参数: uint16 ，电池电压值，量纲:0.01V
** 说明    : 
*******************************************************************************************/
int16  Samlpe_temperatureAdc(void)
{
 float temperature;
   
    Enable_M_ADC(M_ADC_CH1_TEMPERATURE);
    Lib_Delay_ms(10);
    temperature = Cal_temperature_data();
    return((int16)(temperature*10));
}


/*******************************************************************************************
** 函数名称: Samlpe_temperatureAdc
** 函数描述: 获取当前电表温度
** 输入参数: 无
** 输出参数: uint16 ，电池电压值，量纲:0.01V
** 说明    : 
*******************************************************************************************/
float  Cal_temperature_data(void)
{
 uint8 i;  
 uint8 code *p;

 S_TEMPPARA s_TempPara;
 uint32  MAdcValue;
 ST_U16_U08 Kpara;
 ST_U32_U08 temp1;

 float temperature; //当前温度//
 uint8 TempParaSum;
 uint32  temp;


    p=(uint8 code*)0x420;
    for(i=0;i<3;i++)
    {
        Lib_FCpyTMem((uint8*)&s_TempPara.ul_TempParaA,p+i*24,sizeof(S_TEMPPARA));	// 注意小端//
        TempParaSum=Lib_get_csck_int8u_num((uint8*)&s_TempPara.ul_TempParaA,sizeof(S_TEMPPARA)-2,0x33);
        if(TempParaSum==s_TempPara.uc_Add33)
        {
            break;
        }
    }
    Lib_Rev_Copy_Arry((INT8U *)&s_TempPara.ul_TempParaA, 4);
    Lib_Rev_Copy_Arry((INT8U *)&s_TempPara.ul_TempParaB, 4);
    Lib_Rev_Copy_Arry((INT8U *)&s_TempPara.ul_TempParaC, 4);
    Lib_Rev_Copy_Arry((INT8U *)&s_TempPara.ul_TempParaD, 4);
    Lib_Rev_Copy_Arry((INT8U *)&s_TempPara.ul_TempParaE, 4);

    MAdcValue=ReadMeterParaACK(DATAOM);//读M通道原始值

    temp=MAdcValue>>16;
    Lib_FCpyTMem(&Kpara.B08[0], (uint8 code*)&InitParaRTC[(CtrlBGP&0x3e)], 0x02);
    temp=(temp*Kpara.u16)/1000;

    if(TempParaSum!=s_TempPara.uc_Add33)
    {        
        s_TempPara.ul_TempParaA = 18018;	// 精工
        s_TempPara.ul_TempParaB = 1;
        s_TempPara.ul_TempParaC = 224019254;
        s_TempPara.ul_TempParaD = 7092;
        s_TempPara.ul_TempParaE = 10;
    }
 temperature=(s_TempPara.ul_TempParaB*sqrt(s_TempPara.ul_TempParaC+s_TempPara.ul_TempParaD*temp)-s_TempPara.ul_TempParaA)/s_TempPara.ul_TempParaE;


    p=(uint8 code*)0x480;
    for(i=0;i<3;i++)
    {
        Lib_FCpyTMem((uint8*)&temp1.B08[0],p+(i*4),4);	
        temp1.B08[2]	= Lib_get_csck_int8u_num((uint8*)&temp1.B08[0],2,0x33);
        if(temp1.B08[2] == temp1.B08[3])
        {
            break;
        }
    }

    if(temp1.B08[2] != temp1.B08[3])
    {
        temp1.W16[0] = 0;		// 校验出错 默认0//
    }

    if(temp1.W16[0]&0x8000)
    {
        temp1.W16[0] = ~temp1.W16[0]+1;
        temperature -= (temp1.W16[0]/10);
    }
    else
    {
        temperature += (temp1.W16[0]/10);
    }

    if(temperature>90)
    {   
        temperature=90;
    }else if(temperature<(-43))
    {   
        temperature=-43;
    }

    return (temperature);

}






/*******************************************************************************************
**    END
*******************************************************************************************/

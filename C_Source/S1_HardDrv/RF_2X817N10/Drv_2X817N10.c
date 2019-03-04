/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: App_Metering.c
**Author: maji
**date: 
**description: c code for   app function of energy sum
**note: memer type  SD03A
**--------------------Version History -------------------------------------
** Date        Rev    By     Description 
**============================================================================
**
** 1/21/2013  0.1  sosomj   1. frist .                             
**
**============================================================================ 
*/


#include <MyIncludes_H.h>


uint8 RF_Version;


#if (VERSION==LSD4RF_2X717N10)
#define  HIGH_POWER    0   // =1  HIGH_POWER 设置//
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 寄存器组
//// 说明     : AGC=0，AFC=0，9600，200ohm ，20dbm  
//////////////////////////////////////////////////////////////////////////////////
code _SX12XX_REG LSD_RFregConfig[] = {         // 
     
    {REG_OPMODE,        0x04},           
    {REG_DATAMODUL,     0x00},           
    {REG_BITRATEMSB,    0X0D},//9600bps
    {REG_BITRATELSB,    0X05}, 
    {REG_FDEVMSB,       0x01},// 20KDEV
    {REG_FDEVLSB,       0x48},    
    {REG_FRFMSB,        0XE1}, //902MHz
    {REG_FRFMID,        0X80},           
    {REG_FRFLSB,        0X00},           
    {REG_OSC1,          0X41},           
    {REG_AFCCTRL,       0x00},
    {REG_LOWBAT,        0x02},           
    {REG_LISTEN1,       0X92},           
    {REG_LISTEN2,       0XF5},           
    {REG_LISTEN3,       0X20},                      
    {REG_PALEVEL,       0x7F},    // PA1 + pA2 , 17dBm    high power off
    {REG_PARAMP ,       0X09},           
    {REG_OCP,           0X1A},           
    {REG_SERVED14,      0x40},
    {REG_SERVED15,      0xB0},
    {REG_SERVED16,      0x7B},
    {REG_SERVED17,      0x9B},      
    {REG_LNA ,          0X89},
    {REG_RXBW,          0xC4}, //   31K ,0.25
    {REG_AFCBW,         0X8B},           
    {REG_OOKPEAK,       0X40},           
    {REG_OOKAVG,        0X80},           
    {REG_OOKFIX,        0X06},           
    {REG_AFCFEI,        0X10},        
    {REG_DIOMAPPING1,   0X00},     
    {REG_DIOMAPPING2,   0X0F},     //修改时间----20150108，07->0F                 
    {REG_RSSITHRESH ,   0XFF},     //修改时间----20150108    
    {REG_RXTIMEOUT1 ,   0X00},           
    {REG_RXTIMEOUT2 ,   0X00},           
    {REG_PREAMBLEMSB,   0X00},           
    {REG_PREAMBLELSB,   0X04},            
    {REG_SYNCCONFIG,    0X9A},           
    {REG_SYNCVALUE1,    0XD3},
    {REG_SYNCVALUE2,    0X91},
    {REG_SYNCVALUE3,    0XD3},     
    {REG_SYNCVALUE4,    0X91},     
    {REG_SYNCVALUE5,    0X00},           
    {REG_SYNCVALUE6,    0X00},           
    {REG_SYNCVALUE7,    0X00},           
    {REG_SYNCVALUE8,    0X00},           
    {REG_PACKETCONFIG1, 0X90},
    {REG_PAYLOADLENGTH, 66},            // max to 66 bytes
    {REG_NODEADRS,      0X00},           
    {REG_BROADCASTADRS, 0X00},           
    {REG_AUTOMODES,     0X00},           
    {REG_FIFOTHRESH,    0XC2},           
    {REG_PACKETCONFIG2, 0X02},        
    {REG_AESKEY1,       0X00},                       
    {REG_AESKEY2,       0X00},                       
    {REG_AESKEY3,       0X00},                       
    {REG_AESKEY4,       0X00},                       
    {REG_AESKEY5,       0X00},                       
    {REG_AESKEY6,       0X00},                       
    {REG_AESKEY7,       0X00},                       
    {REG_AESKEY8,       0X00},                       
    {REG_AESKEY9,       0X00},                       
    {REG_AESKEY10,      0X00},                       
    {REG_AESKEY11,      0X00},                       
    {REG_AESKEY12,      0X00},                       
    {REG_AESKEY13,      0X00},                       
    {REG_AESKEY14,      0X00},                       
    {REG_AESKEY15,      0X00},                       
    {REG_AESKEY16,      0X00}, 
    {REG_PREAMBLE,      0XCA},          //修改时间----20150108，开启Premble
    {REG_TESTLNA,       0x2D},
    {REG_TESTPA1,       0x55},
    {REG_TESTPA2,       0x70},
    {REG_TESTDAGC,      0x30},
    {REG_TESTAFC,       0x00},  
};
//470-500Mhz频率表//
code  unsigned char Freq_Cal_Tab[]=
{
      0x75,0x80,0x00,		//	470.0	MHZ
      0x75,0x8C,0xE0,		//	470.2	MHZ
      0x75,0x99,0xAD,		//	470.4	MHZ
      0x75,0xA6,0x7A,		//	470.6	MHZ
      0x75,0xB3,0x46,		//	470.8	MHZ
      0x75,0xC0,0x13,		//	471.0	MHZ
      0x75,0xCC,0xE0,		//	471.2	MHZ
      0x75,0xD9,0xAD,		//	471.4	MHZ
      0x75,0xE6,0x7A,		//	471.6	MHZ
      0x75,0xF3,0x46,		//	471.8	MHZ
      0x76,0x00,0x13,		//	472.0	MHZ
      0x76,0x0C,0xE0,		//	472.2	MHZ
      0x76,0x19,0xAD,		//	472.4	MHZ
      0x76,0x26,0x7A,		//	472.6	MHZ
      0x76,0x33,0x47,		//	472.8	MHZ
      0x76,0x40,0x13,		//	473.0	MHZ
      0x76,0x4C,0xE0,		//	473.2	MHZ
      0x76,0x59,0xAD,		//	473.4	MHZ
      0x76,0x66,0x7A,		//	473.6	MHZ
      0x76,0x73,0x47,		//	473.8	MHZ
      0x76,0x80,0x13,		//	474.0	MHZ
      0x76,0x8C,0xE0,		//	474.2	MHZ
      0x76,0x99,0xAD,		//	474.4	MHZ
      0x76,0xA6,0x7A,		//	474.6	MHZ
      0x76,0xB3,0x47,		//	474.8	MHZ
      0x76,0xC0,0x13,		//	475.0	MHZ
      0x76,0xCC,0xE0,		//	475.2	MHZ
      0x76,0xD9,0xAD,		//	475.4	MHZ
      0x76,0xE6,0x7A,		//	475.6	MHZ
      0x76,0xF3,0x47,		//	475.8	MHZ
      0x77,0x00,0x13,		//	476.0	MHZ
      0x77,0x0C,0xE0,		//	476.2	MHZ
      0x77,0x19,0xAD,		//	476.4	MHZ
      0x77,0x26,0x7A,		//	476.6	MHZ
      0x77,0x33,0x47,		//	476.8	MHZ
      0x77,0x40,0x14,		//	477.0	MHZ
      0x77,0x4C,0xE0,		//	477.2	MHZ
      0x77,0x59,0xAD,		//	477.4	MHZ
      0x77,0x66,0x7A,		//	477.6	MHZ
      0x77,0x73,0x47,		//	477.8	MHZ
      0x77,0x80,0x14,		//	478.0	MHZ
      0x77,0x8C,0xE0,		//	478.2	MHZ
      0x77,0x99,0xAD,		//	478.4	MHZ
      0x77,0xA6,0x7A,		//	478.6	MHZ
      0x77,0xB3,0x47,		//	478.8	MHZ
      0x77,0xC0,0x14,		//	479.0	MHZ
      0x77,0xCC,0xE0,		//	479.2	MHZ
      0x77,0xD9,0xAD,		//	479.4	MHZ
      0x77,0xE6,0x7A,		//	479.6	MHZ
      0x77,0xF3,0x47,		//	479.8	MHZ
      0x78,0x00,0x14,		//	480.0	MHZ
      0x78,0x0C,0xE0,		//	480.2	MHZ
      0x78,0x19,0xAD,		//	480.4	MHZ
      0x78,0x26,0x7A,		//	480.6	MHZ
      0x78,0x33,0x47,		//	480.8	MHZ
      0x78,0x40,0x14,		//	481.0	MHZ
      0x78,0x4C,0xE0,		//	481.2	MHZ
      0x78,0x59,0xAD,		//	481.4	MHZ
      0x78,0x66,0x7A,		//	481.6	MHZ
      0x78,0x73,0x47,		//	481.8	MHZ
      0x78,0x80,0x14,		//	482.0	MHZ
      0x78,0x8C,0xE1,		//	482.2	MHZ
      0x78,0x99,0xAD,		//	482.4	MHZ
      0x78,0xA6,0x7A,		//	482.6	MHZ
      0x78,0xB3,0x47,		//	482.8	MHZ
      0x78,0xC0,0x14,		//	483.0	MHZ
      0x78,0xCC,0xE1,		//	483.2	MHZ
      0x78,0xD9,0xAD,		//	483.4	MHZ
      0x78,0xE6,0x7A,		//	483.6	MHZ
      0x78,0xF3,0x47,		//	483.8	MHZ
      0x79,0x00,0x14,		//	484.0	MHZ
      0x79,0x0C,0xE1,		//	484.2	MHZ
      0x79,0x19,0xAD,		//	484.4	MHZ
      0x79,0x26,0x7A,		//	484.6	MHZ
      0x79,0x33,0x47,		//	484.8	MHZ
      0x79,0x40,0x14,		//	485.0	MHZ
      0x79,0x4C,0xE1,		//	485.2	MHZ
      0x79,0x59,0xAD,		//	485.4	MHZ
      0x79,0x66,0x7A,		//	485.6	MHZ
      0x79,0x73,0x47,		//	485.8	MHZ
      0x79,0x80,0x14,		//	486.0	MHZ
      0x79,0x8C,0xE1,		//	486.2	MHZ
      0x79,0x99,0xAE,		//	486.4	MHZ
      0x79,0xA6,0x7A,		//	486.6	MHZ
      0x79,0xB3,0x47,		//	486.8	MHZ
      0x79,0xC0,0x14,		//	487.0	MHZ
      0x79,0xCC,0xE1,		//	487.2	MHZ
      0x79,0xD9,0xAE,		//	487.4	MHZ
      0x79,0xE6,0x7A,		//	487.6	MHZ
      0x79,0xF3,0x47,		//	487.8	MHZ
      0x7A,0x00,0x14,		//	488.0	MHZ
      0x7A,0x0C,0xE1,		//	488.2	MHZ
      0x7A,0x19,0xAE,		//	488.4	MHZ
      0x7A,0x26,0x7A,		//	488.6	MHZ
      0x7A,0x33,0x47,		//	488.8	MHZ
      0x7A,0x40,0x14,		//	489.0	MHZ
      0x7A,0x4C,0xE1,		//	489.2	MHZ
      0x7A,0x59,0xAE,		//	489.4	MHZ
      0x7A,0x66,0x7A,		//	489.6	MHZ
      0x7A,0x73,0x47,		//	489.8	MHZ
      0x7A,0x80,0x14,		//	490.0	MHZ
      0x7A,0x8C,0xE1,		//	490.2	MHZ
      0x7A,0x99,0xAE,		//	490.4	MHZ
      0x7A,0xA6,0x7A,		//	490.6	MHZ
      0x7A,0xB3,0x47,		//	490.8	MHZ
      0x7A,0xC0,0x14,		//	491.0	MHZ
      0x7A,0xCC,0xE1,		//	491.2	MHZ
      0x7A,0xD9,0xAE,		//	491.4	MHZ
      0x7A,0xE6,0x7B,		//	491.6	MHZ
      0x7A,0xF3,0x47,		//	491.8	MHZ
      0x7B,0x00,0x14,		//	492.0	MHZ
      0x7B,0x0C,0xE1,		//	492.2	MHZ
      0x7B,0x19,0xAE,		//	492.4	MHZ
      0x7B,0x26,0x7B,		//	492.6	MHZ
      0x7B,0x33,0x47,		//	492.8	MHZ
      0x7B,0x40,0x14,		//	493.0	MHZ
      0x7B,0x4C,0xE1,		//	493.2	MHZ
      0x7B,0x59,0xAE,		//	493.4	MHZ
      0x7B,0x66,0x7B,		//	493.6	MHZ
      0x7B,0x73,0x47,		//	493.8	MHZ
      0x7B,0x80,0x14,		//	494.0	MHZ
      0x7B,0x8C,0xE1,		//	494.2	MHZ
      0x7B,0x99,0xAE,		//	494.4	MHZ
      0x7B,0xA6,0x7B,		//	494.6	MHZ
      0x7B,0xB3,0x47,		//	494.8	MHZ
      0x7B,0xC0,0x14,		//	495.0	MHZ
      0x7B,0xCC,0xE1,		//	495.2	MHZ
      0x7B,0xD9,0xAE,		//	495.4	MHZ
      0x7B,0xE6,0x7B,		//	495.6	MHZ
      0x7B,0xF3,0x47,		//	495.8	MHZ
      0x7C,0x00,0x14,		//	496.0	MHZ
      0x7C,0x0C,0xE1,		//	496.2	MHZ
      0x7C,0x19,0xAE,		//	496.4	MHZ
      0x7C,0x26,0x7B,		//	496.6	MHZ
      0x7C,0x33,0x48,		//	496.8	MHZ
      0x7C,0x40,0x14,		//	497.0	MHZ
      0x7C,0x4C,0xE1,		//	497.2	MHZ
      0x7C,0x59,0xAE,		//	497.4	MHZ
      0x7C,0x66,0x7B,		//	497.6	MHZ
      0x7C,0x73,0x48,		//	497.8	MHZ
      0x7C,0x80,0x14,		//	498.0	MHZ
      0x7C,0x8C,0xE1,		//	498.2	MHZ
      0x7C,0x99,0xAE,		//	498.4	MHZ
      0x7C,0xA6,0x7B,		//	498.6	MHZ
      0x7C,0xB3,0x48,		//	498.8	MHZ
      0x7C,0xC0,0x14,		//	499.0	MHZ
      0x7C,0xCC,0xE1,		//	499.2	MHZ
      0x7C,0xD9,0xAE,		//	499.4	MHZ
      0x7C,0xE6,0x7B,		//	499.6	MHZ
      0x7C,0xF3,0x48,		//	499.8	MHZ
      0x7D,0x00,0x14		//	500.0	MHZ
};

code  _SX12XX_REG DateRate[]={
  
    //9.6Kbps
    {REG_BITRATEMSB,    0X0D},//9600bps
    {REG_BITRATELSB,    0X05},
    {REG_FDEVMSB,       0x01},// 20KDEV
    {REG_FDEVLSB,       0x48},
    {REG_RXBW,          0xC4}, //   31K ,0.25  
    //  4.8kbps
    {REG_BITRATEMSB,    0X1A},//4800bps
    {REG_BITRATELSB,    0X0B},
    {REG_FDEVMSB,       0x01},// 16KDEV
    {REG_FDEVLSB,       0x06},
    {REG_RXBW,          0x44}, //   31.3K ,4   
    //20kbps
    {REG_BITRATEMSB,    0X06},//20kbps
    {REG_BITRATELSB,    0X40},
    {REG_FDEVMSB,       0x01},// 30KDEV
    {REG_FDEVLSB,       0xEC},
    {REG_RXBW,          0x4B}, // 50K ,4
    //40kbps
    {REG_BITRATEMSB,    0X03},//40kbps
    {REG_BITRATELSB,    0X20},
    {REG_FDEVMSB,       0x02},// 40KDEV
    {REG_FDEVLSB,       0x8F},
    {REG_RXBW,          0x43}, //62.5K ,4
    
};
#endif






//**********************************************************************************
// 功能描述 : spi发送or接收1字节
// 输入参数 : unsigned char mosi
// 返回参数 : unsigned char
// 说明     : 
//**********************************************************************************
static unsigned char LSD_RF_SpiInOut (unsigned char mosi) 
{ 
    unsigned char i;
    unsigned char onebyte;
    onebyte = mosi;
    for(i = 8; i > 0; i--) 
    {
        LSD_RF_DELAY();
        if(onebyte & 0x80) 
        {
            LSD4RF_SPI_SET_SDO_HIGH();//LSD_SPI_SIMO_OUT(1) ;
        }
        else 
        {
            LSD4RF_SPI_SET_SDO_LOW() ;
        }

        LSD_RF_DELAY();
        LSD4RF_SPI_SET_CLK_HIGH();
        onebyte <<= 1;      // next bit
        if(LSD_SPI_SOMI_IN()) 
        {
            onebyte++;      // 1 found on MISO
        }
        LSD_RF_DELAY();
        LSD4RF_SPI_SET_CLK_LOW();
    }
    return onebyte;         // mosi now becomes the value of miso
}
//**********************************************************************************
// 功能描述 : spi读写寄存器
// 输入参数 : unsigned char addr地址 unsigned char val数据
// 返回参数 : 
// 说明     : Note: don't call it directely use macro SpiWriteReg and SpiWriteReg  to call this routine
//**********************************************************************************
unsigned char LSD_RF_SpiRW_Reg (unsigned char addr, unsigned char val) 
{
    unsigned char rc;
    LSD_RF_SPIInit();
    LSD4RF_SPI_SET_NSS_LOW();
    LSD_RF_SpiInOut(addr);
    rc = LSD_RF_SpiInOut(val);
    LSD4RF_SPI_SET_NSS_HIGH();
    return rc;
}
//**********************************************************************************
// 功能描述 : 写寄存器
// 输入参数 : addr地址 val数据
// 返回参数 : 无
// 说明     : 
//**********************************************************************************
#define LSD_RF_WriteReg(addr, val)         LSD_RF_SpiRW_Reg(addr|0x80,val)
//**********************************************************************************
// 功能描述 : 读寄存器
// 输入参数 : addr地址
// 返回参数 : 寄存器值
// 说明     : 
//**********************************************************************************
#define LSD_RF_ReadReg(addr)               LSD_RF_SpiRW_Reg(addr&0x7F, 0)
//**********************************************************************************
// 功能描述 : 清除FIFO以及溢出标志位
// 输入参数 : 
// 返回参数 : 
// 说明     : 
//**********************************************************************************
#define LSD_RF_ClearFIFO()                    LSD_RF_WriteReg(REG_IRQFLAGS2, 0x10)
//**********************************************************************************
// 功能描述 : 确保进入standby
// 输入参数 : 
// 返回参数 : 
// 说明     : 注意：此函数将listen功能关闭,TS_OSC>500us
//**********************************************************************************
void LSD_RF_StandbyMode(void)
{
    unsigned long  i=600000;
    LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY));
    while (((LSD_RF_ReadReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00)&&i)i--; // Wait for ModeReady
}
//**********************************************************************************
// 功能描述 : 确保进入sleep
// 输入参数 : 
// 返回参数 : 
// 说明     : 注意：此函数将listen功能关闭
//**********************************************************************************
void LSD_RF_SleepMode(void)
{
    unsigned long  i=600000;
    LSD_RF_StandbyMode();       //先切换到standby模式再切到sleep模式//
    LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_SLEEP));
    while (((LSD_RF_ReadReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00)&&i)i--; // Wait for ModeReady
//    LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00+RF_DIOMAPPING1_DIO1_11+RF_DIOMAPPING1_DIO2_01+RF_DIOMAPPING1_DIO3_01); // DIO0 = packet sent
//    LSD_RF_WriteReg(REG_DIOMAPPING2, RF_DIOMAPPING2_DIO4_00+RF_DIOMAPPING2_DIO5_00); // DIO0 = packet sent
    LSD4RF_SPI_SET_NSS_LOW();  //进入低功耗，要将NSS拉低，防止漏电流，这个很重要//
}
//**********************************************************************************
// 功能描述 : 确保进入RX模式
// 输入参数 : 
// 返回参数 : 
// 说明     : 注意：此函数将listen功能关闭  ，DIO3=Premble   DIO0=PLD
//**********************************************************************************
void LSD_RF_RXmode(void)
{
    unsigned char nTimes,cData;
    LSD_RF_StandbyMode();       //先切换到standby模式//
    if(RF_Version==RF_SX1208)
       LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAP1_DIO0_PLD_RDY|RF_DIOMAPPING1_DIO3_01); // DIO0 = PLD_RDY
    else 
       LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAP1_DIO0_PLD_RDY|RF_DIOMAPPING1_DIO3_10); // DIO0 = PLD_RDY
    LSD_RF_ClearFIFO();         //清FIFO  
    while(1)    //防止外部因素导致SPI切换到RX模式失败，此代码为做保护用。//
    {
         nTimes=100;
         LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_RECEIVER));
         //等待进入接收状态//
         do
         {
             cData=LSD_RF_ReadReg(REG_OPMODE) & 0x1c;
             nTimes--;
         }
         while((cData!=RF_OPMODE_RECEIVER)&&nTimes);
         if(!nTimes)
              continue;

         return;
     } 
}
////**********************************************************************************
//// 功能描述 : 写配置表
//// 输入参数 : 无
//// 返回参数 : 无
//// 说明     : 
////**********************************************************************************
void LSD_RF_Config(void)
{
    _SX12XX_REG code *p;
    unsigned char i;
    p = LSD_RFregConfig;
    for(i=sizeof(LSD_RFregConfig)/2; i>0; i--) 
    {
        LSD_RF_WriteReg(p->addr, p->val);
        p++;
    }
}
////**********************************************************************************
//// 功能描述 : 复位无线模块
//// 输入参数 : 无
//// 返回参数 : 无
//// 说明     : 
////**********************************************************************************
void LSD_RF_RestRF(void)
{
    LSD4RF_SPI_SET_RESET_DIR_OUT() ;  //复位引脚做输出//
    LSD4RF_SET_RESET_HIGH(); //复位引脚输出高电平复位//
    Lib_Delay_ms(20);
    LSD4RF_SET_RESET_LOW();   //复位引脚输出低电平//
    Lib_Delay_ms(20);

}
////**********************************************************************************
//// 功能描述 : RF初始化
//// 输入参数 : 无
//// 返回参数 : 无
//// 说明     : 
////**********************************************************************************
unsigned char LSD_RF_InitRF(void) 
{
    LSD_RF_RestRF();                    //复位RF
    LSD_RF_SPIInit();                   //RF初始化 //
    LSD_RF_StandbyMode();               //进入standby模式//
    LSD_RF_Config();                    //写RF配置表，最好在standby模式或sleep模式下修改//
    RF_Version = LSD_RF_ReadReg(REG_VERSION);
    // to test SPI
    LSD_RF_WriteReg(REG_SYNCVALUE8, 0x55);
    if(LSD_RF_ReadReg(REG_SYNCVALUE8) != 0x55) 
    {
        return 1;// something wrong with SPI
    }
    LSD_RF_WriteReg(REG_SYNCVALUE8, 0xD3);
    if(LSD_RF_ReadReg(REG_SYNCVALUE8) != 0xD3) 
    {
        return 1;// something wrong with SPI
    }
   return 0;           
}

////////////////////////////////////////////////////////////////////////////
// 功能描述 : 发送数据
// 输入参数 : Uint8 *txBuffer:发送数据存储数组首地址,
//            Uint8 size：发送数据长度
// 返回参数 : 无
// 说明     : 
//////////////////////////////////////////////////////////////////////////////
void LSD_RF_SendPacket(uint8 *txBuffer, uint8 size)
{
    uint8 i;
    unsigned long  j=600000;   //发送超时退出，根据实际应用请做调整//
    LSD_RF_StandbyMode();               //进入standby模式//
    LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAP1_DIO0_PKT_SENT); // DIO0 = packet sent
    if(HIGH_POWER==1)   //如果High Power 设置enable（+20dbm capability on PA_BOOST）
    {
        LSD_RF_WriteReg(REG_OCP,0x0F);   
        LSD_RF_WriteReg(REG_TESTPA1,0x5D);   
        LSD_RF_WriteReg(REG_TESTPA2,0x7C);
    }
    LSD_RF_ClearFIFO();    //清除FIFO
    LSD_RF_WriteReg(REG_FIFO, size);
    for(i=0;i<size;i++)
    {
        LSD_RF_WriteReg(REG_FIFO, txBuffer[i]);
    }
    LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_TRANSMITTER));  //切换到发送//
    while((!LSD4RF_READ_DIO0())&&j)j--;         // packet sent
    LSD_RF_StandbyMode();               //进入standby模式//
    if(HIGH_POWER==1)                 //退出发送后，注意一定要将Hing Power Disable。//
    {
        LSD_RF_WriteReg(REG_OCP,0x1B);   
        LSD_RF_WriteReg(REG_TESTPA1,0x55);   
        LSD_RF_WriteReg(REG_TESTPA2,0x70); 
    }
    
}

//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 接收数据包
//// 输入参数 : Uint8 *cRxBuf,返回数据存储首地址。 Uint8 cLength 读的数据长度
//// 返回参数 : 无
//// 说明     : 
//////////////////////////////////////////////////////////////////////////////////
void LSD_RF_RxPacket(uint8 *cRxBuf, uint8 *cLength)
{
    uint8 i;
    LSD_RF_StandbyMode();               //进入standby模式//
    *cLength = LSD_RF_ReadReg(REG_FIFO);
    for(i=0;i<*cLength;i++)
      cRxBuf[i]=LSD_RF_ReadReg(REG_FIFO);
    LSD_RF_ClearFIFO();    //清除FIFO
}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : FreqCalSet
//// 输入参数 : ch   建议ch=0~75
//// 返回参数 : 无
//// 说明     : 
//////////////////////////////////////////////////////////////////////////////////
void LSD_RF_FreqSet(uint8 ch)
{
    LSD_RF_StandbyMode();               //进入standby模式//
    LSD_RF_WriteReg(REG_FRFMSB,Freq_Cal_Tab[ch*3]);
    LSD_RF_WriteReg(REG_FRFMID,Freq_Cal_Tab[ch*3+1]);
    LSD_RF_WriteReg(REG_FRFLSB,Freq_Cal_Tab[ch*3+2]);
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : WOR初始化
// 输入参数 : 无
//            
// 返回参数 : 无
// 说明     : 前导（2）+同步(2)         
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WORInit(void)
{
    LSD_RF_StandbyMode();                //进入standby模式//
    //配置参数
    LSD_RF_WriteReg(REG_PREAMBLELSB,0x02);   //设置2个前导码//
    LSD_RF_WriteReg(REG_SYNCCONFIG,0x88);   //设置2个同步字//
    LSD_RF_WriteReg(REG_SYNCVALUE1,0x55);   //设置同步字内容 //
    LSD_RF_WriteReg(REG_SYNCVALUE2,0x55);   //设置同步字内容 //
//    LSD_RF_WriteReg(REG_PACKETCONFIG1,0x80);   //关闭CRC  
    LSD_RF_WriteReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO3_10); // 映射DIO3 = SYNC_ADDR（也可以用DIO0，这里用DIO3为例子）    //
   
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 执行WOR操作
// 输入参数 : Uint8 cLength  0 进入睡眠模式。  1 进入接收模式。
//            
// 返回参数 : 无
// 说明     :  DIO3做WOR唤醒中断
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WORexecute(uint8 cLength)
{
    unsigned long  i=600000; 
    switch(cLength)
    {
        case 0:  //进入睡眠模式//
            // LSD_RF_DIO3_IE =0;      //关闭中断//
            //                Sleep_RX=0;             //指示睡眠//
            LSD_RF_SleepMode();     //进入睡眠//
            //LSD_Sleep_Timerout();   //开启睡眠定时器//
            break;
        case 1://进入接收模式//
            //LSD_RF_DIO3_IE =0;          //关闭中断//
            //               Sleep_RX=1;                 //指示接收//
            LSD_RF_SleepMode();         //进入睡眠 //
            LSD_RF_ClearFIFO();         //清FIFO //
            LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_RECEIVER));//进入接收//
            while (((LSD_RF_ReadReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00)&&i)i--; // Wait for ModeReady
            //LSD_RF_DIO3_IFG=0;          //清除中断标志位//
            // LSD_RF_DIO3_IE =1;          //开启中断//
            //LSD_RX_Timerout();          //开启接收定时器//
            break;
        default:break;
    }

}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 退出WOR,滞留在sdandby模拟式
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 退出WOR,将改变过的寄存器恢复到寄存器数组中的值
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WOR_Exit(void)
{
   //关闭中断
    //LSD_RF_DIO3_IFG=0;   //清除中断标志位//
   // LSD_RF_DIO3_IE =0;  //关闭中断//
    //TimerA_Init();      //重新初始化定时器，关闭定时器//
    //恢复寄存器//
    LSD_RF_StandbyMode();                //进入standby模式//
    LSD_RF_WriteReg(REG_DIOMAPPING1, 0x00); // 恢复原值//
    LSD_RF_WriteReg(REG_PREAMBLELSB,0x04);   //恢复原值//
    LSD_RF_WriteReg(REG_SYNCCONFIG,0x9A);   //恢复原值//
    LSD_RF_WriteReg(REG_SYNCVALUE1,0xD3);    //恢复原值//
    LSD_RF_WriteReg(REG_SYNCVALUE2,0x91);    //恢复原值//
//    LSD_RF_WriteReg(REG_PACKETCONFIG1,0x90);   //恢复原值 //  
}

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 唤醒函数
// 输入参数 : 无
//            
// 返回参数 : 无
// 说明     : 前导唤醒方式
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_AwakePkt(void)
{
    //LSD_RF_DIO0_IFG=0;                   //清除MCU标志位//
   // LSD_RF_DIO0_IE=0;                    //关闭中断//
    LSD_RF_StandbyMode();               //进入standby模式//
    if(HIGH_POWER==1)   //如果High Power 设置enable（+20dbm capability on PA_BOOST）
    {
        LSD_RF_WriteReg(REG_OCP,0x0F);   
        LSD_RF_WriteReg(REG_TESTPA1,0x5D);   
        LSD_RF_WriteReg(REG_TESTPA2,0x7C);
    }
    LSD_RF_ClearFIFO();    //清除FIFO
//     P6OUT |= BIT5;     //测试探针
    LSD_RF_WriteReg(REG_OPMODE, (RF_OPMODE_SEQUENCER_ON|RF_OPMODE_LISTEN_OFF | RF_OPMODE_TRANSMITTER));  //切换到发送//
//    Lib_Delay_s(5);//实际示波器测试发送时间为4.2s


//     P6OUT &=~ BIT5;    //测试探针//
    LSD_RF_StandbyMode();               //进入standby模式
    if(HIGH_POWER==1)                 //退出发送后，注意一定要将Hing Power Disable。
    {
        LSD_RF_WriteReg(REG_OCP,0x1B);   
        LSD_RF_WriteReg(REG_TESTPA1,0x55);   
        LSD_RF_WriteReg(REG_TESTPA2,0x70); 
    }
}



//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : Trigger a RSSI Measurement 触发一次RSSI测量
//// 输入参数 : 无         
//// 返回参数 : 无
//// 说明     : 
//////////////////////////////////////////////////////////////////////////////////
void LSD_RF_TriggerRssi(void)
{
    LSD_RF_WriteReg(REG_RSSICONFIG,0x01);   
}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 计算RSSI值
//// 输入参数 : 无
////            
//// 返回参数 : 无
//// 说明     : 
//////////////////////////////////////////////////////////////////////////////////
unsigned char LSD_RF_GetRSSI(void)
{
    unsigned char cRssi;
    cRssi = LSD_RF_ReadReg(REG_RSSIVALUE);
    cRssi = cRssi/2;
    return cRssi;
}


/****************************************************************
Function:		INT8U Lnk_2X817N10_Intilize(void)  	
Description: 
Input:
Parameters:     
Returns:                                
Others:         
****************************************************************/

INT8U Lnk_sx1276_rx_data(INT8U *rx_ptr)
{
	// RF 通道是否接收到数据 //
    if(LSD4RF_READ_DIO0()==0) return 0;
    Lib_Delay_Nop(20);
    if(LSD4RF_READ_DIO0() ==0) return 0;   
    
    // 接收到数据后的处理 //
    //DIO0 =high, mean RFmodule had receievd data  //   
    LSD_RF_RxPacket(rx_ptr,64);     //接收数据//

    return (TRUE);
}


/***************************************************************
*    END
****************************************************************/


////////////////////////////////////////////////////////////////////////////////
// 版权:     利尔达科技集团股份有限公司
// 文件名:   
// 版本：   
// 工作环境:
// 作者:     
// 生成日期: 
// 功能:      
// 相关文件: 
// 修改日志：
//      见更新说明
////////////////////////////////////////////////////////////////////////////////

#include <MyIncludes_H.h>





//===================================定义变量===================================================
float G_BandWidthKHz;//本地计算Symbol周期使用
float G_TsXms;
S_LoRaConfig G_LoRaConfig;//在main中初始化





INT8U Lnk_SX2176_Intilize(void)
{
     if(SX127x_init() != NORMAL) return FALSE;
     Rx_mode();                        //进入RX,准备接受数据//
     return TRUE;
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
    SX1276SPISetup();
    SPI_CS_LOW()  ;  
    LSD_RF_SpiInOut(addr);
    rc = LSD_RF_SpiInOut(val);
    SPI_CS_HIGH() ;
    return rc;
}

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向寄存器地址写1字节数据
// 输入参数 : uint8_t addr,寄存器地址 uint8_t data数据
// 返回参数 : 
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void SX1276Write( uint8 addr, uint8 temp )    
{
  LSD_RF_SpiRW_Reg(addr|0x80,temp);
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向寄存器地址读1字节数据
// 输入参数 : uint8_t addr,寄存器地址 uint8_t *data读数据存储地址
// 返回参数 : 
// 说明     :
////////////////////////////////////////////////////////////////////////////////
#define SX1276Read(addr)               LSD_RF_SpiRW_Reg(addr&0x7F, 0);
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向FIFO读数据
// 输入参数 : uint8_t *buffer,数组指针 uint8_t size长度
// 返回参数 : uint8_t *buffer 存储读取内容
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void  SX1276ReadFifo( uint8 *buffer, uint8 size ) 
{
   SX1276ReadBuffer( 0, buffer, size );
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向寄存器地址连续发送数据
// 输入参数 : uint8_t addr,寄存器地址 uint8_t *buffer,发送数组指针 uint8_t size指针长度
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
  void SX1276WriteBuffer( uint8 addr, uint8 *buffer, uint8 size )
{
    uint8 i;  
  // LSD_RF_SpiInOut((addr | 0x80));
    for( i = 0; i < size; i++ )
    { 
        SX1276Write(addr ,buffer[i]);                // Send data
    }

}


////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向寄存器地址连续读数据
// 输入参数 : uint8_t addr,寄存器地址 uint8_t *buffer,存储数组指针 uint8_t size要读的长度
// 返回参数 : 数据返回到*buffer中
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void SX1276ReadBuffer( uint8 addr, uint8 *buffer, uint8 size )
{   
    uint8 i;  
   // SX1276Read((addr));
    for( i = 0; i < size; i++ )
    { 
        buffer[i] = SX1276Read(addr);                // Send data
    }
}


BOOL LoRaConfig_Check()
{
    if((G_LoRaConfig.LoRa_Freq<410000000)||(G_LoRaConfig.LoRa_Freq>1020000000))
        return FALSE;
    if((G_LoRaConfig.LoRa_Freq>525000000)&&(G_LoRaConfig.LoRa_Freq<862000000))
        return FALSE;
    G_LoRaConfig.BandWidth = (t_BandWidth)(G_LoRaConfig.BandWidth&0xF0);
    if(G_LoRaConfig.BandWidth>BW500KHZ)
        return FALSE;
    //计算BandWidth
    switch(G_LoRaConfig.BandWidth){
    case BW500KHZ:G_BandWidthKHz = 500.0;break;
    case BW250KHZ:G_BandWidthKHz = 250.0;break;
    case BW125KHZ:G_BandWidthKHz = 125.0;break;
    case BW62_50KHZ:G_BandWidthKHz = 62.5;break;
    case BW41_66KHZ:G_BandWidthKHz = 41.66;break;
    case BW31_25KHZ:G_BandWidthKHz = 31.25;break;
    case BW20_83KHZ:G_BandWidthKHz = 20.83;break;
    case BW15_62KHZ:G_BandWidthKHz = 15.62;break;
    case BW10_41KHZ:G_BandWidthKHz = 10.41;break;
    case BW7_81KHZ:G_BandWidthKHz = 7.81;break;
    }
    G_LoRaConfig.SpreadingFactor = (t_SpreadingFactor)(G_LoRaConfig.SpreadingFactor&0xF0);
    if((G_LoRaConfig.SpreadingFactor>SF12)||(G_LoRaConfig.SpreadingFactor<SF06))
        return FALSE;
    //计算LoRa码元周期，单位ms
    G_TsXms = (2<<((G_LoRaConfig.SpreadingFactor>>4)-1))/G_BandWidthKHz;
    
    G_LoRaConfig.CodingRate = (t_CodingRate)(G_LoRaConfig.CodingRate&0x0E);
    if((G_LoRaConfig.CodingRate>CR_4_8)||(G_LoRaConfig.CodingRate<CR_4_5))
        return FALSE;
    if(G_LoRaConfig.PowerCfig>15)
        return FALSE;
    if(G_LoRaConfig.PayloadLength>127)
        return FALSE;
    return TRUE;
}
//===================================子函数===================================================
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF初始化
// 输入参数 : tSX127xInitPara initPara  输入速率Init_LoRa_0_8K, Init_LoRa_4_8K , Init_LoRa_10k,
// 返回参数 : tSX127xError              错误枚举内容
// 说明     :  初始化时，信道初始化默认为0信道
////////////////////////////////////////////////////////////////////////////////

tSX127xError SX127x_init()
{     
     uint8 test = 0;
      //RF 模块//
    //配置各个参数
    G_LoRaConfig.LoRa_Freq = 915000000;   //中心频点470MHz
    G_LoRaConfig.BandWidth = BW500KHZ;    //BW = 125KHz
    G_LoRaConfig.SpreadingFactor = SF07;  //SF = 9
    G_LoRaConfig.CodingRate = CR_4_8;     //CR = 4/6
    G_LoRaConfig.PowerCfig = 15;          //19±dBm
    G_LoRaConfig.MaxPowerOn = TRUE;       
    G_LoRaConfig.CRCON = TRUE;            //CRC开启
    G_LoRaConfig.ExplicitHeaderOn = TRUE; //Header开启
    G_LoRaConfig.PayloadLength = 64;      //数据包长度


    if(FALSE==LoRaConfig_Check())   //如果输入参数错误
    {
        return PARAMETER_INVALID;  //报错输入
    }
    SX1276InitIo();                // PAIO口初始化            
    SX1276Reset();                 //复位RF
    SX1276SPISetup();              //SPI初始化

    //切换到LoRamode，standby状态
     SX1276Write( REG_LR_OPMODE, RFLR_OPMODE_SLEEP);            
     SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_SLEEP );       
     SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );
    /*------------------------------------------------
    SPI 验证                   */  
    test = 0;
    SX1276Write( REG_LR_HOPPERIOD,0x91 );//选一个用不到的寄存器来做验证
     test = SX1276Read( REG_LR_HOPPERIOD);
    if( test != 0x91)
        return SPI_READCHECK_WRONG;           

    //Frequency Configuration 
    LSD_RF_FreqSet();    //设置频率
    
    //PA Configuration  
    LSD_RF_PoutSet();
    SX1276Write( REG_LR_PARAMP,RFLR_PARAMP_0250_US);  
    //  ↑PA Ramp的时间，如果用户LDO不能快速输出大电流（泵能力），适当增加PA Ramp时间
    //  ↑如果Ramp时间过短超过了LDO的能力时，会出现进入TX后，系统电流为发射电流，但是RF信号不出现的现象
    SX1276Write( REG_LR_OCP,0x20|RFLR_OCP_TRIM_240_MA);//电流过载保护 Over Current Protection
    
    //PayloadLength 初始化             
    SX1276Write( REG_LR_PAYLOADLENGTH,G_LoRaConfig.PayloadLength); 
    //注意，无头模式（Implicit Header）时，必须提前规定好收发双方的PL
	
    //BW、CR、Header有无，初始化   0x90 0x08 0x01     
    SX1276Write( REG_LR_MODEMCONFIG1,\
        (((uint8)G_LoRaConfig.BandWidth)|((uint8)G_LoRaConfig.CodingRate))|(\
            (TRUE==G_LoRaConfig.ExplicitHeaderOn)?0x00:0x01));
    
    //SF、CRC初始化   0x74               
 /*   SX1276Write( REG_LR_MODEMCONFIG2,\
        ((uint8)G_LoRaConfig.SpreadingFactor)|(\
            (TRUE==G_LoRaConfig.CRCON)?0x04:0x00));
*/
	SX1276Write( REG_LR_MODEMCONFIG2,\
    ((uint8)G_LoRaConfig.SpreadingFactor)|(\
        (TRUE==G_LoRaConfig.CRCON)?0x04:0x00)|0x03);


    if(SF06==G_LoRaConfig.SpreadingFactor){   //慎用SF = 6，需要的配置很特殊
        uint8 temp = 0;
        temp = SX1276Read( 0x31);
        SX1276Write(  0x31,(temp& 0xF8)|0x05);
        SX1276Write(  0x37,0x0C); 
   }
//	SX1276Write(REG_LR_MODEMCONFIG1, ((9<<4)+(0x04<<1)+0x01));//Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
//	SX1276Write(REG_LR_MODEMCONFIG2, ((7<<4)+(0x01<<2)+0x03));

//   低速率优化功能是否开启、AutoAGC默认开启  
     SX1276Write( REG_LR_MODEMCONFIG3,0x0c);


/*    SX1276Write( REG_LR_MODEMCONFIG3,((G_TsXms>16.0)?\
      RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON:RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_OFF\
          )|RFLR_MODEMCONFIG3_AGCAUTO_ON);
 */ 
    return NORMAL;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF发送数据包
// 输入参数 : uint8 *data：发送数据指针
// 返回参数 : 无
// 说明     : 设置为发送是， preamble改回默认值
////////////////////////////////////////////////////////////////////////////////
void SX1276_TxPacket(uint8 *temp)
{
   unsigned long int j=16777215;// 超时时间。调试..
//   ExInt4IFG&=(~BIT2);
//   EIE&=~BIT2;              //int2 发送前关闭中断   // 不使用中断
//   ExInt4IE&=~BIT2;
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );      
    SX1276Write( REG_LR_PREAMBLEMSB,0);
    SX1276Write( REG_LR_PREAMBLELSB,8);
    SX1276Write( REG_LR_PAYLOADLENGTH,G_LoRaConfig.PayloadLength);                    
    SX1276WriteRxTx(TRUE);                                      
    SX1276Write( REG_LR_FIFOADDRPTR,0x80);                      
    SX1276WriteBuffer(REG_LR_FIFO,temp,G_LoRaConfig.PayloadLength);  
    SX1276Write(REG_LR_IRQFLAGS,0xff);                          
    SX1276Write( REG_LR_IRQFLAGSMASK, ~(RFLR_IRQFLAGS_TXDONE)); 
    SX1276Write( REG_LR_DIOMAPPING1, RFLR_DIOMAPPING1_DIO0_01 );
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_TRANSMITTER );   
    // while((!LSD4RF_READ_DIO0())&&j)j--;         // packet sent	 
    while((!LSD_RF_READ_DIO0())&&j)j--;         // packet sent	 
	 
//     while ((RF_send())&&j) j--;   //表端//
  //  while( ExInt4IFG&BIT2);
 //   P1OE|=(BIT1);        //INT 禁止输出
 //   P1IE|=(BIT1);        //INT 允许输入
  //  P11FS=0x04;         //   INT
   // ExInt4IFG&=(~BIT2);
 //   EIE|=BIT2;              //int2中断
//    ExInt4IE|=BIT2;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF进入接收状态
// 输入参数 : 
// 返回参数 : 无
// 说明     : 进入接收后preamble设置回默认值为
////////////////////////////////////////////////////////////////////////////////
void Rx_mode()
{ 
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );      
    SX1276Write( REG_LR_PREAMBLEMSB,0);
    SX1276Write( REG_LR_PREAMBLELSB,8);
    SX1276Write( REG_LR_PAYLOADLENGTH,G_LoRaConfig.PayloadLength);
    SX1276Write( REG_LR_IRQFLAGSMASK, ~(RFLR_IRQFLAGS_RXDONE)); 
    SX1276Write( REG_LR_DIOMAPPING1, RFLR_DIOMAPPING1_DIO0_00 );
    SX1276WriteRxTx(FALSE);                                     
    SX1276Write( REG_LR_FIFOADDRPTR,0x00);                      
    SX1276Write(REG_LR_IRQFLAGS,0xff);                          
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_RECEIVER ); 
//    P1OE|=(BIT1);        //INT 禁止输出
//    P1IE|=(BIT1);        //INT 允许输入
//    P11FS=0x04;         //   INT
//    ExInt4IFG&=(~BIT2);
//    EIE|=BIT2;              //int2中断
//    ExInt4IE|=BIT2;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF接收数据包
// 输入参数 : uint8*cbuf接收数组指针
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_RxPacket(uint8 *cbuf)
{
   // if(true==G_LoRaConfig.ExplicitHeaderOn){
        //有头那么从寄存器中读，否则按照G_LoRaConfig设置长度读取FIFO
      
         G_LoRaConfig.PayloadLength = SX1276Read(REG_LR_NBRXBYTES);
          SX1276Write( REG_LR_FIFOADDRPTR,0x00);
  //  }
    SX1276ReadFifo(cbuf,G_LoRaConfig.PayloadLength);   
    SX1276Write(REG_LR_IRQFLAGS,0xff);        
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF进入SLEEP状态
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_SleepMode(void)
{
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );      
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_SLEEP );     
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF进入standby状态
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_StandbyMode(void)
{
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );       
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF配置频率
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
tSX127xError LSD_RF_FreqSet()
{
    uint32 freq_reg;
	uint8  test_FRFMSB,test_FRFMID,test_FRFLSB;
    if((G_LoRaConfig.LoRa_Freq<410000000)||(G_LoRaConfig.LoRa_Freq>1020000000))
        return FALSE;
    if((G_LoRaConfig.LoRa_Freq>525000000)&&(G_LoRaConfig.LoRa_Freq<862000000))
        return FALSE;
     freq_reg = (uint32)(G_LoRaConfig.LoRa_Freq/FREQ_STEP);
     test_FRFMSB = 0;
	 test_FRFMID=0;
	 test_FRFLSB=0;
    LSD_RF_StandbyMode();
    // FREQ = 474.6MHz
    SX1276Write( REG_LR_FRFMSB, (uint8 )(freq_reg>>16));//Carrier Freq 470M
    SX1276Write( REG_LR_FRFMID, (uint8 )(freq_reg>>8) );
    SX1276Write( REG_LR_FRFLSB, (uint8 )(freq_reg) );
    
    test_FRFMSB = SX1276Read(REG_LR_FRFMSB);
    test_FRFMID = SX1276Read(REG_LR_FRFMID);
    test_FRFLSB = SX1276Read(REG_LR_FRFLSB);
    
    if(test_FRFMSB != (uint8 )(freq_reg>>16))
        return SPI_READCHECK_WRONG;
    if(test_FRFMID != (uint8 )(freq_reg>>8))
        return SPI_READCHECK_WRONG;
    if(test_FRFLSB != (uint8 )(freq_reg))
        return SPI_READCHECK_WRONG;
    return NORMAL;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF配置功率
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
tSX127xError LSD_RF_PoutSet()
{
uint8 test;
    if(G_LoRaConfig.PowerCfig>15)
        return PARAMETER_INVALID;
    LSD_RF_StandbyMode();
    SX1276Write( REG_LR_PACONFIG, 0xf0|G_LoRaConfig.PowerCfig);
    test = 0;
    test = SX1276Read(REG_LR_PACONFIG);
    if((0xf0|G_LoRaConfig.PowerCfig)!=test)
        return SPI_READCHECK_WRONG;
    if(TRUE==G_LoRaConfig.MaxPowerOn)
        SX1276Write( REG_LR_PADAC, 0x80|RFLR_PADAC_20DBM_ON );  
    if (TRUE !=G_LoRaConfig.MaxPowerOn)
        SX1276Write( REG_LR_PADAC, 0x80|RFLR_PADAC_20DBM_OFF );
   
    return NORMAL;
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
    if(LSD_RF_READ_DIO0()==0) return 0;
    Lib_Delay_Nop(20);
    if(LSD_RF_READ_DIO0() ==0) return 0;   
	CLRWDT();	// 清看门狗//
    // 接收到数据后的处理 //
    //DIO0 =high, mean RFmodule had receievd data  //   
    LSD_RF_RxPacket(rx_ptr);     //接收数据//

    return (G_LoRaConfig.PayloadLength);
}

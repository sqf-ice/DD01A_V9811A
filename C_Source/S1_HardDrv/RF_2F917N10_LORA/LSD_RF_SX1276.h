
#ifndef LSD_RF_SX1276_H
#define LSD_RF_SX1276_H
//===================================定义===================================================

typedef enum
{
    NORMAL,                //正常      
    PARAMETER_INVALID,    //参数不可用
    SPI_READCHECK_WRONG,  //SPI出错
}tSX127xError;           //定义出错枚举

typedef enum
{
  // LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
        // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
    BW500KHZ = 0x90,      
    BW250KHZ = 0x80,
    BW125KHZ = 0x70,
    BW62_50KHZ = 0x60,
    BW41_66KHZ = 0x50,
    BW31_25KHZ = 0x40,
    BW20_83KHZ = 0x30,
    BW15_62KHZ = 0x20,
    BW10_41KHZ = 0x10,
    BW7_81KHZ = 0x00,
}t_BandWidth;      //定义带宽枚举
typedef enum{
    // LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    SF12 = 0xC0,
    SF11 = 0xB0,
    SF10 = 0xA0,
    SF09 = 0x90,
    SF08 = 0x80,
    SF07 = 0x70,
    SF06 = 0x60,
}t_SpreadingFactor;
typedef enum{
    // LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] 
    CR_4_8 = 0x08,
    CR_4_7 = 0x06,
    CR_4_6 = 0x04,
    CR_4_5 = 0x02,
}t_CodingRate;
typedef struct S_LoRaConfig
{
    uint32 LoRa_Freq;
    t_BandWidth BandWidth;                     
    t_SpreadingFactor SpreadingFactor;            
    t_CodingRate CodingRate;
    uint8 PowerCfig; //0~15,输出功率 = 2+PowerReg,具体参考数据手册
                     //如果开启MaxPower，PowerReg = 15时，输出功率 = 19±1dBm
    BOOL MaxPowerOn;                    // [false: OFF, true: ON]
    BOOL CRCON;                         // [false: OFF, true: ON]
    BOOL ExplicitHeaderOn;              // [false: OFF, true: ON]
    uint8 PayloadLength;              //1~127
}S_LoRaConfig;

extern S_LoRaConfig G_LoRaConfig;
extern float G_BandWidthKHz;//计算Symbol周期使用
extern float G_TsXms;//Symbol周期，单位ms

//===================================函数声明===================================================
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF初始化
// 输入参数 : 
// 返回参数 : tSX127xError              错误枚举内容
// 说明     : 初始化时，调用全局变量G_LoRaConfig
////////////////////////////////////////////////////////////////////////////////
tSX127xError SX127x_init();
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF发送数据包
// 输入参数 : uint8_t*data：发送数据指针，uint8_t size发送数据长度
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void SX1276_TxPacket(uint8 *temp);
void SX1276_Process(void);
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF进入接收状态
// 输入参数 : 
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void Rx_mode(void);
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF接收可变数据包
// 输入参数 : uint8_t*cbuf接收数组指针
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_RxPacket(uint8 *cbuf);
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF进入SLEEP状态
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_SleepMode(void);
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF进入standby状态
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_StandbyMode(void);
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF设置频率
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
tSX127xError LSD_RF_FreqSet();
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF设置功率
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
tSX127xError LSD_RF_PoutSet();
//*****************************************************************************************

void SX1276WriteFifo( uint8 *buffer, uint8 size );
void SX1276Write( uint8 addr, uint8 temp );
void SX1276InitIo( void );
void SX1276SPISetup(void);
void SX1276WriteBuffer( uint8 addr, uint8 *buffer, uint8 size );
void SX1276ReadBuffer( uint8 addr, uint8 *buffer, uint8 size );
void SX1276WriteFifo( uint8 *buffer, uint8 size );
void SX1276ReadFifo( uint8 *buffer, uint8 size );
void SX1276Reset();
void SX1276WriteRxTx( BOOL txEnable );
extern INT8U Lnk_SX2176_Intilize(void);
INT8U Lnk_sx1276_rx_data(INT8U *rx_ptr);

#endif

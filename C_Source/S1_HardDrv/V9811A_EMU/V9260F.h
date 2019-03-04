#ifndef __RACCOON_H__
#define __RACCOON_H__


#ifndef  RACCOONEXT
    #define RACCOONEXT extern
#else
    #define RACCOONSELF
#endif


//=============================================================
//RACCOON 模拟控制寄存器  ANCtrl1、ANCtrl2必须为0
//=============================================================
#define   RegANCtrl0   0x0185
#define   RegANCtrl1   0X0186
#define   RegANCtrl2   0X0187

//=============================================================
//RACCOON 计量配置寄存器  
//=============================================================
#define   RegMTPARA0   0x0183
#define   RegMTPARA1   0x0184   // 角差数据为8-15位   0x00F0  

//=============================================================
//RACCOON 系统配置寄存器  
//=============================================================
#define   RegSysCtrl   0x0180

//=============================================================
//RACCOON 直流分量寄存器  
//=============================================================
#define   RegDCU       0x0114
#define   RegDCI       0x0115
#define   RegDCM       0x0116
#define   RegDCIM      0x00F9
#define   RegDCVTM     0x00FB
#define   RegDCVDCM1   0x00FD
#define   RegDCVDCM2   0x00FF
#define   RegDCTM      0x0101

//=============================================================
//RACCOON 频率寄存器  
//=============================================================
#define   RegFREQ      0x019A
#define   RegSAFREQ    0x011D
#define   RegAFREQ     0x011E

//=============================================================
//RACCOON 电压/电流/M通道有效值寄存器  
//=============================================================
#define   RegARRTI     0x0105
#define   RegARRTU     0x0104
#define   RegARRTM     0x0106
#define   RegBRRTU     0x0109
#define   RegBRRTI     0x010A
#define   RegARTI      0x010E
#define   RegARTU      0x010D
#define   RegARTM      0x010F
#define   RegARTIM     0x00F8
#define   RegARVTM     0x00FA
#define   RegARVDCM1   0x00FC
#define   RegARVDCM2   0x00FE
#define   RegARTMM     0x0100
#define   RegBRTU      0x0112
#define   RegBRTI      0x0113
#define   RegAARTI     0x011C
#define   RegAARTU     0x011B
#define   RegAARTM     0x0117
#define   RegABRTU     0x0121
#define   RegABRTI     0x0122

//=============================================================
//RACCOON 有功/无功功率寄存器  
//=============================================================
#define   RegRAP       0x0102
#define   RegRAQ       0x0103
#define   RegRBP       0x0107
#define   RegRBQ       0x0108
#define   RegIAP       0x010B
#define   RegIAQ       0x010C
#define   RegIBP       0x0110
#define   RegIBQ       0x0111
#define   RegAAP       0x0119
#define   RegAAQ       0x011A
#define   RegABP       0x011F
#define   RegABQ       0x0120


//=============================================================
//RACCOON 有功/无功能量累加、CF脉冲阀寄存器  
//=============================================================
#define   RegPEGY      0x01A1
#define   RegNEGY      0x01A2
#define   RegPCFCNT    0x01A3
#define   RegNCFCNT    0x01A4


//=============================================================
//RACCOON 预设直流偏置值寄存器  
//=============================================================
#define   RegZZDCI     0x0123
#define   RegZZDCU     0x0124

//=============================================================
//RACCOON 电压/电流/测量值有效值校正寄存器  
//=============================================================
#define   RegWARTI     0x012C
#define   RegWARTU     0x0132
#define   RegWARTM     0x012D
#define   RegWBRTI     0x0126
#define   RegWBRTU     0x012B

//=============================================================
//RACCOON 全波有功/无功功率校正寄存器  
//=============================================================
#define   RegWAPT      0x012E
#define   RegWAQT      0x0130
#define   RegWWAPT     0x012F
#define   RegWWAQT     0x0131
#define   RegWBPT      0x0127
#define   RegWBQT      0x0129
#define   RegWWBPT     0x0128
#define   RegWWBQT     0x012A

//=============================================================
//RACCOON 门限值寄存器  
//=============================================================
#define   RegEGYTH     0x0181
#define   RegCTH       0x0182
#define   RegIDETTH    0x0134

//=============================================================
//RACCOON 校验和寄存器  
//=============================================================
#define   RegCKSUM     0x0133


//=============================================================
//RACCOON 带通滤波器系数寄存器  
//=============================================================
#define   RegBPFPARA   0x0125

//=============================================================
//RACCOON 功率值寄存器  
//=============================================================
#define   RegDATACP    0x0189


//=============================================================
//函数功能:RACCOON的从机地址
//
//=============================================================


//下面是为了编译通过

////功率/电流数据寄存器
/*
#define DATAP       0x0119
#define DATAQ       0x10d7
#define RMSU        0x011B
#define RMSI1       0x011C
#define RMSI2       0x0117

#define RAP         0x0102  //全波有功功率原始值
#define RAQ         0x0103  //全波无功功率原始值
#define ARRTU       0x0104  //全波电流有效值的原始值
#define ARRTI       0x0105  //全波电压有效值的原始值
 
//
#define DATAFREQ    0x011D
//
*/
//上面是为了编译通过


#define Slave0  0
#define Slave1  1
#define Slave2  2
#define Slave3  3

#define SlaveNum    Slave0
//#define HZCONST   3200*256*10*1.0254
#define HZCONST   8192000

#define VARHZ   0x4c000

extern uint8 I_Dirct;
extern uint8 Aphy_Dly;
extern uint32 power,npower;



#define divf     3475      //8号表
#define divf1    3630      //8号表
#define divf2    3570      //8号表


#ifdef RACCOONSELF
uint16 const  RegAddrSoc[]=
{
    SCP,         //有功功率比差寄存器
    SCQ,         //无功功率比差寄存器
    SCU,         //电压有效值比差寄存器
    SCI1,        //电流有效值比差寄存器
    PARAPC,      //有功功率二次补偿寄存器 
    PARAQC,      //无功功率二次补偿寄存器 
    PHCCtrl1,    //Ia通道角差寄存器
};

uint16 const  RegAddrRac[]=
{
    RegANCtrl0,     //模拟控制寄存器0
    RegANCtrl1,     //模拟控制寄存器1
    RegANCtrl2,     //模拟控制寄存器2
    RegBPFPARA,     //带通滤波器系数寄存器
    RegMTPARA0,     //计量控制寄存器0 
    
    RegMTPARA1,     //计量控制寄存器1
    RegWARTU,       //全波电压有效值比差寄存器
    RegWARTI,       //全波电流有效值比差寄存器 
    RegWAPT,        //全波有功功率比差寄存器
    RegWWAPT,       //全波有功功率二次补偿值寄存器
    RegWWAQT,
};
#else
extern uint16 const  RegAddrSoc[];
extern uint16 const  RegAddrRac[];
#endif


//=============================================================
//结构体：校表参数
//
//=============================================================
typedef struct
{
    uint32  Soc_SCP;             //有功功率比差寄存器
    uint32  Soc_SCQ;             //无功功率比差寄存器
    uint32  Soc_SCU;             //电压有效值比差寄存器
    uint32  Soc_SCI1;            //电流有效值比差寄存器
    uint32  Soc_PARAPC;          //有功功率二次补偿寄存器                
    uint32  Soc_PARAQC;          //无功功率二次补偿寄存器 
    uint8   Soc_PHCCtrl1;         //Ia通道 角差寄存器
}S_JBRESoc;


typedef struct
{
    uint32  Rac_MTPARA1;         //计量控制寄存器1 包含角差
    uint32  Rac_WARTU;           //全波电压有效值比差寄存器
    uint32  Rac_WARTI;           //全波电流有效值比差寄存器
    uint32  Rac_WAPT;            //全波有功功率比差寄存器
    uint32  Rac_WWAPT;           //全波有功功率二次补偿寄存器                
    uint32  Rac_WWAQT;           //全波无功功率二次补偿寄存器                
}S_JBRERac;


typedef struct
{
    uint8  uc_80msCnt;            // 小信号有功80MS  
    uint8  uc_80msCntRE;      //小信号无功80ms
    uint8  uc_160msCnt;         //大信号有功160ms
    uint8  uc_160msCntRE;   //大信号无功160ms
    uint16  uc_1280msCnt;     //秒平均计数
}S_IMPTIM;


RACCOONEXT S_IMPTIM gs_ipmtim;

typedef struct 
{
    uint16 ui_MeterC;           // 表常数
    uint16 ui_Un;               // 标称电压
    uint16 ui_Ib;               // 标称电流
    
    uint32  Com_EGYTH;          //有功门限值寄存器
    uint32  Com_REEGYTH;        //无功门限值寄存器

    uint32  Rac_ANCtrl0;        //模拟控制寄存器0
    uint32  Rac_ANCtrl1;        //模拟控制寄存器1
    uint32  Rac_ANCtrl2;        //模拟控制寄存器2
    uint32  Rac_BPFPARA;        //带通滤波器系数寄存器
    uint32  Rac_MTPARA0;        //计量控制寄存器0
    
    S_JBRESoc  gs_JBA;          //A相比差组
    S_JBRERac  gs_JBB;          //B相比差组

    uint32 ul_PG;               //A相有功功率比例系数
    uint32 ul_URmG;             //A相电压通道比例系数
    uint32 ul_I1RmG;            //A相电流通道1比例系数
    
    uint8  Soc_AnaG;             //A相模拟增益(包含M bit[7],U bit[6],IB bit[5:3],IA bit[2:0])

    uint16 u16_csck;            //  CS累加和
//    uint16 ui_JbCRC;            // 校表参数的CRC结果
}S_JBPM;

RACCOONEXT S_JBPM gs_JbPm;            //校表参数




//=============================================================
//函数功能:RACCOON的读写控制
//
//=============================================================
#define APhy  0x08
#define BPhy  0x00
#define ABCPhy 0x03
#define RacRead  0x01
#define RacWrite 0x02 
#define RacBroadcast 0



#define   guc_Dy9260FOver           0x25        //模拟串口超时保护 370ms 通讯保护? //


enum          //gs_RacCtrl.ucStatus参数
{
  Rac_Idle=0,
  Rac_Send,
  Rac_Rev,
  Rac_WaitPro,
   
};

typedef struct 
{
  uint8 ucStatus;               
  uint8 ucSendLen;
  uint8 ucSendPoint;
  uint8 ucRevLen;
  uint8 ucRevPoint;
  uint8 ucBuf[100];
}S_RacCtrl;
RACCOONEXT S_RacCtrl gs_RacCtrl;


RACCOONEXT volatile uint8 guc_CommDelayTime;             //通讯等待延时
RACCOONEXT volatile uint8 guc_CommDelayTime1;




typedef struct
{
    uint8 uc_Phystat;
    uint8 uc_Physendtim;
    uint8 uc_Sendstat;
    uint8 uc_Powstat;
    uint8 uc_flapara;
    uint8 uc_Uartstat;
}S_RacPhy;
RACCOONEXT S_RacPhy gs_RacPhy;
RACCOONEXT uint8 guc_cntA; //反向计数
RACCOONEXT uint8 guc_cntB; 

RACCOONEXT uint8 guc_powerdir;     //反相标记
RACCOONEXT uint8 guc_start;   //读计量开始
RACCOONEXT uint8 guc_powerdirA;     //反相标记
RACCOONEXT uint8 guc_powerdirB;     //反相标记
RACCOONEXT uint8 guc_powerdirAmid;     //反相标记中间变量
RACCOONEXT uint8 guc_powerdirBmid;     //反相标记中间变量



enum
{
     power_a = 0x01,
     power_b = 0x02,
     power_c = 0x04,
};


#define V9260Start_Send()       { SCON4|=BIT1; ExInt2IFG|=(BIT2); }


void Init_RacCtrl(void);
uint8 WriteRaccoon(uint32 Data,uint16 Addr,uint8 Phyadd);
uint8 ReadRaccoon(uint16 Addr,uint8 num,uint8 Phyadd);
void ReadRaccoonInt(uint16 Addr,uint8 num,uint8 Phyadd); 
void RxReset_V9260(void);
void InitRaccoon(void);
void V9260_InitDefaultPara(void);
void CheckRegSum(void);
void CheckSysErr(void);
uint8 UpdateRaccoonPara(void);
uint32 Read_Power(uint16 reg,uint8 phyadd);
void BroadcastWriteRaccoon(uint32 Data,uint16 Addr);
void UpdateRaccoonParABC(uint8 phyadd,uint8 pow);
void Accumulate_abc(void);
void  Init_9260F_EMU(void);

void ReadbackABCPhy(void);  

#endif

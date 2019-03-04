/*******************************************************************
*
*  FILE             :                                        
*  DATE            :                                  
*  DESCRIPTION :                    
*  CPU GROUP    :R8C,002                                                   
*                                                                  
*
*******************************************************************/
//#ifndef _Api_LoadProfile_H
//#define _Api_LoadProfile_H


//******************************************************************
//              1     立即数预定义                                            //                      
//******************************************************************


#define  FUNC_LoadPro 1
#define  ReaEC	      0     // 无功电量开关

#define	DEBUG_TEST    0 

//------------------------------------------------------------------------------
#define MaxLPReg	9//寄存器最多个数
#define MaxLPCh		1//通道最多数
#define LP_RunPara_Len	4	//运行参数
#define LP_E2P		1//=1：E2P存储。=0：默认是Flash




#define LP_MAX_LEN	1455 	  //记录负荷曲线的最大条数 防止抄收过程尾部刷新


#define READLP_MAX_LEN	1455 	  //读取负荷曲线的最大条数  负荷记录15分钟间隔 15天

#define FALSE_ERR 0xFF

#define BLOCK_MAX_LEN  5         //负荷记录块包含条数    不分帧情况下建议不要超过5条

#define LPDataType 0xFF00
//相关电量
#define LPECType   0xF00//1111'0000'0000
#define LPECTypeBs 0x100
#define LPEC	   LPECTypeBs*(0+1)	
#define LPRevEC	   LPECTypeBs*(No_RevEC+1)
#define LPReaEC	   LPECTypeBs*(No_ReaEC+1)

#define CLPRegNum  (sizeof(LPReg)/sizeof(LPREG))

typedef struct
{
    INT8U	Code;	
    INT8U	*Reg;    // 寄存器的数据源的指针
    INT8U	DataLen; // 数据长度
    INT16U	Info; // 信息
} LPREG;

typedef struct
{
    INT8U 	Ch;//通道号//从0开始
    INT16U	Ptr;//读指针(当前通道的)
    INT8U	RegNum;	// 寄存器代码个数
    INT16U	MaxRecNum;	//固定的数据区域根据设置的记录代码，得出的最大条数
    INT8U	loadp_reg_code[1+9]; // 第一字节寄存器代码长度  
}LPREAD;

typedef struct
{
    INT16U	Ptr[MaxLPCh];
    INT16U	Num[MaxLPCh];
}LPRUNPARA;
typedef struct
{
    INT16U	Original_Ptr;
    INT16U	Original_FrameNum;
//	  INT8U FrameACK;
	  INT8U ReadLoraFlag;
}LPReadPRO;

////////////////////////////////////////////////////////////////////接口声明
extern LPREAD	   LPRd;            //负荷记录存储信息
extern LPRUNPARA   LPRunPa;         //负荷记录参数
extern LPReadPRO  LPReadPara;

extern uchar ReadLPRunPa(void);  //读取负荷记录参数
extern void LoadProRst(void);    //复位负荷表
extern uchar LPRec_Read(void);   //读负荷表数据
extern char Load_Rec(char Ch);   //手动记录负荷数据
extern void LoadPro_Rec(void);
extern uchar LPRec_Read_1(INT8U *dst,INT16U idNo);
extern INT16U LPRec_Read_Block(INT8U *dst,INT16U StartARESS,INT16U EndARESS);
extern void Init_LoadPro_param(void);




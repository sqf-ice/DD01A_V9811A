/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_UART_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: UART模块的硬件驱动C代码的声明文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#ifndef _Drv_UART_H
#define _Drv_UART_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/
//uart  bps define   //
#define BPS300     300UL
#define BPS600     600UL
#define BPS1200    1200UL
#define BPS2400    2400UL
#define BPS4800    4800UL
#define BPS9600    9600UL

#define		UART_BAUDRATE_300			(32768-FSYSCLK/16UL/BPS300)	
#define		UART_BAUDRATE_600			(32768-FSYSCLK/16UL/BPS600)	
#define		UART_BAUDRATE_1200			(32768-FSYSCLK/16UL/BPS1200)	
#define		UART_BAUDRATE_2400			(32768-FSYSCLK/16UL/BPS2400)	
#define		UART_BAUDRATE_4800			(32768-FSYSCLK/16UL/BPS4800)	
#define		UART_BAUDRATE_9600			(32768-FSYSCLK/16UL/BPS9600)	


//uart  data bits lenth  define 
#define UART_8BITS    8
#define UART_7BITS    7

// UART 通道 //
#define UART_CH_NUM 1
#define UART_CH_INDEX_MAX  (UART_CH_NUM-1)


#define UART_CH0_IR 0    // UART4  TTL//
//#define UART_CH1_RS 1    // RF //
#define CH_RF   1               // RF使用的 UART的缓存  禁止操作串口4// 
//#define UART_CH1_RS 1   // UART4 //
#define UART_BUFF_SIZE    250    //缓冲区最大长度为200个  //
//RF  通道BUFF 最大长度  //
#define RF_BUFF_MAX_LENTH 250  //数据缓冲区字节长度最大64  //


/***************************************************************
*    2     数据结构声明     
****************************************************************/
typedef struct 
{	
    uint8  tx_len;					// 串口发送数据长度//
    uint8	buff_index;				// 当前串口数据BUFF 指针下标 //
    uint8  buff_index_back;			// 上一次串口接收数据BUFF 指针下标 //
    uint8  rx_over_ticks;           // 串口接收数据超时结束定时器，10mS基准  //
    uint8  buff[UART_BUFF_SIZE];	//  串口缓冲区 //
    uint8  status;					// 串口工作标识//
    uint16 badu;					// 串口波特率 //
    uint8  bits_len;				// 串口位数 8或7 //
} UART_DRV_VAR;



//通讯过程中的应用层变量数据 //
struct RF_COMM_VAR
{
    INT8U   len;                                      //接收发送帧数据长度//
    INT8U   buff[RF_BUFF_MAX_LENTH];                             //通讯波特率暂存//
    INT8U   err_flg;                                //链路层操作状态暂存//
    INT8U   iec_status;                   //通讯模式//
    INT8U	  comm_reflash_flag;				//通讯过后刷新标志//
    INT16U   tmr_10ms_delay;                   //通讯链路状体的延时等待，3.5秒  //
} ;
/***************************************************************
*    3    变量声明       
****************************************************************/
extern UART_DRV_VAR gs_uart_drv_var[UART_CH_NUM];

/***************************************************************
*    4     函数声明        
****************************************************************/
extern void Set_Uart2_badu(uint16 rate);
extern void Set_Uart4_badu(uint16 rate);
extern void En_UART2_rx(void);
extern void En_UART4_rx(void);
extern void Dis_UART2_rx(void);
extern void Dis_UART4_rx(void);
extern void En_UART2_Tx(void);
extern void En_UART4_Tx(void);
extern void Init_UART2_hard(uint16 badu);
extern void Init_UART4_hard(uint16 badu);
extern void Init_UART_soft(uint8 ch);
extern void Hdl_UART_10ms_run(uint8 ch);
extern void Tx_UART_frist (uint8 *p_buf, uint8 len,uint16 badu,uint8 ch);
extern uint8 Get_UART_rx_buff (uint8 **p_buf,uint8 ch);

extern void Uart4_Transmit(void);

/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/

//UART_DRV_VAR. status define  //
#define UART_STATUS_IDLE                    0                                                                    // 空闲状态 //
#define UART_STATUS_BUSY_RX       UART_STATUS_IDLE+1                   // 空闲状态 //
#define UART_STATUS_BUSY_RX_OVER       UART_STATUS_BUSY_RX+1    // 空闲状态 //
#define UART_STATUS_BUSY_TX       UART_STATUS_BUSY_RX_OVER+1    // 空闲状态 //
#define UART_STATUS_BUSY_TX_OVER       UART_STATUS_BUSY_TX+1    // 空闲状态 //

/***************************************************************
*    END
****************************************************************/
#endif

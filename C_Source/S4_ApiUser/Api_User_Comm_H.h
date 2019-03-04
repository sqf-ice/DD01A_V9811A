/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_User_Comm_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: 通讯处理应用层相关函数程序H文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/



#ifndef _Api_User_Comm_H
#define _Api_User_Comm_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/
/////////////////////////////////
//security
////////////////////////////////
#define  mmok    0x00           //   权限判断正确
#define  mmover  0x01           // 密码超权限
#define  mmerr   0x02           //  密码错误
#define  keyoff  0x03            //   编程开关无效
#define  uferr   0x04             //


//通讯链路层退出的最长时间定义//
#define  COMM_DELAY_3500MS     350//350
#define  COMM_DELAY_100MS      5  //发送延时50ms
#define  COMM_DELAY_500MS      40

//#define   _COMM_FUNCTION
//IEC_COMM_LINK.mode define 
#define   IEC_COMM_END                                   0x00
#define   IEC_COMM_REQUEST                            '/'
#define   IEC_COMM_ACKNOWLEDGEMENT           0x06   // ACK //
#define   IEC_COMM_COMMAND                          0X01   // SOH  //

#define UART_BUFF_MAX_LENTH 170  //数据缓冲区字节长度最大100  //
#define UART_BUFF_MIN_SIZE 12       //最小数据长度为12字节  //


//  起始请求数据的固定字长定义  //
#define IEC_START_LENTH05   5      // no meter id
#define IEC_START_LENTH13	13    // with  meter id 12位

#define IEC_START_LENTH17	17    // with meter id	8位


#define Meter_INFO_LENTH  16     //电表信息数组长度   //
#define MIMA_INFO_LENTH   10    // 电表密码标识数组长度 //

/***************************************************************
*    2     数据结构声明     
****************************************************************/
//通讯表
typedef struct 
{
	INT32U 	ComID;
	INT8U 	*Addr;
  	INT16U 	ComInfo;
	INT8U 	Len;
	INT8U     Len_Asc;
}IEC_COMDISTAB;


 typedef struct 
{
    ST_U16_U08	    Info;
    ST_U32_U08	    di1_di0;
   INT8U                             *addr;//INT16U                             addr;
    INT8U                               len;
    INT8U                               len_asc;
    INT8U                               id_mode;
} COMM_DATA;


//通讯过程中的应用层变量数据 //
typedef struct 
{
    INT8U                        len;                                      //接收发送帧数据长度//
    INT8U                        baud_rate;                             //通讯波特率暂存//
    INT8U                        err_flg;                                //链路层操作状态暂存//
    INT8U                        iec_status;                            //通讯模式//
    INT16U                      delay_10ms;                         //  握手成功后的3S空闲等待复位延时寄存器    //
    INT16U                      tx_ready_10ms;                   //接收数据成功后发送数据前的延时   //
    INT8U                        tx_delay_flg;
    INT8U                        start_index;
} UART_COMM_APP_VAR;



/***************************************************************
*    3    变量声明       
****************************************************************/
extern uint8 *P_IEC_BUFF;    //通讯缓冲区指针 //
extern uint8 g_ch_comm;   //当前通讯通道号 //
extern UART_COMM_APP_VAR  gs_uart_iec_app_var[UART_CH_NUM];
extern COMM_DATA  comm_data;
extern uint8 clr_flag;
/***************************************************************
*    4     函数声明        
****************************************************************/
extern INT8U get_meter_id_asclen(INT8U *Addr,INT8U numbyte);
extern INT8U  get_IEC_comm_var( INT32U id );
extern INT8U SecurityCK(void);
extern INT8U IEC_Cmd_Password_Deal(INT8U* sptr);
extern INT8U IEC_Cmd_Read_Deal(INT8U* sptr);
extern INT8U IEC_Cmd_Write_Deal(INT8U* sptr,INT8U len);
extern INT8U IEC_Cmd_Break_Deal(INT8U* sptr);

extern void lnk_tx_comm_ready( uint8 ch,uint8 start_index);
extern void api_handl_COMM_FRAME_ERROR( uint8 ch);
extern void process_iec62056_21 (INT8U ch);
extern void api_handl_COMM_pre_10ms(uint8 ch);

extern void RF_comm_deal(void);
/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/
// ComInfo  define 
// 11000000 00000000    read or program ability  
#define EN_R   0X8000            //enable read
#define EN_W   0x4000            //enable program
#define EN_RW  0xc000
#define EN_E   0x0000		// Execute

//00111000 00000000    operate data type        
#define P_RAM    	0x0800*1
#define P_E2P1   	(0x0800*2)		//P_DMFLASH//
#define P_DMFLASH   (0x0800*MEM_DMFLASH)//
#define P_RTC    	0x0800*8

// 00000111 00000000  operate security level
#define W_MM  0x0100*0           // 密码权限
#define W_HKEY  0x0100*1         // 编程按键
#define W_SKEY  0x0100*2         // also by sofy-key control
#define W_FCT  0x0100*3          // 工厂按键
#define W_CLR  0x0100*4         //电量清零  
#define W_SETM	0x0100*5     //密码设置c=0F


// 00000000 11110000  special command operate
#define CM_ALL      	 0X0010*15        //用于取值使用
#define CM_NOMAL    	 0x0010*0         //常规项目
#define CM_BILL     	 0x0010*1         //读账单数据
#define CM_SET_TIME 	 0x0010*2         //设置时钟数据
#define CM_SET_LIMIT     0x0010*3         //限制项目
#define CM_BILL_48HOUR   0x0010*4         //读48小时结算账单数据//
#define CM_EBIL			 0x0010*5		  //读事件记录 暂未和账单数据合并//
#define CM_GETHER_NOMAL  0x0010*6         //读取普通集合数据
#define CM_GETHER_BILL   0x0010*7         //读取账单集合数据
#define CM_GETHER_EBIL   0x0010*8         //读取事件记录集合


// uart_vomm_app_avr.iec_status   bit define
#define COMM_INFO_OK_IEC           0X01     // bit0 
#define COMM_REQUIST_OK_IEC        0x02     //  请求命令有效 //
#define COMM_MIMA_OK_IEC           0X04     //  密码认证有效 //
#define COMM_CMD_OK_IEC            0X08     //  密码认证有效 //
#define COMM_BREAK_OK_IEC          0X10     //  终止指令有效 //

//uart_vomm_app_avr.err_flg define
#define  FRAME_ERROR     0X01     //帧错误，需要复位  //


/***************************************************************
*    END
****************************************************************/

#endif

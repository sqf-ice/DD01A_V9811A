/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_UART.c
**Author		: maji
**date			: 2016-04-20 
**description	: UART模块的硬件驱动C代码
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>



UART_DRV_VAR gs_uart_drv_var[UART_CH_NUM];


/*******************************************************************************************
** 函数名称: Set_Uart2_badu
** 功能描述: UART2波特率配置
** 入口参数: u16 badu：串口波特率，双字节16进制数据
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void Set_Uart2_badu(uint16 rate)
{
    TMOD2 = 0x20;                               // 8-bit counter with auto-reload
    switch(rate)
    {
        case BPS600:
#if(FSYSCLK==FSYSCLK_3M2)
        TCON2 = 0x82;
#if(C_Fq == 60)
        TL21=TH21=0xDE;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xE4;	 
#endif
#endif
#if(FSYSCLK==FSYSCLK_6M5)
        TCON2 = 0x82;		
#if(C_Fq == 60)
        TL21=TH21=0xBC;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xC7;	 
#endif
#endif
        break;

        case BPS1200:
#if(FSYSCLK==FSYSCLK_3M2)
        TCON2 = 0x82;
#if(C_Fq == 60)
        TL21=TH21=0xEF;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xF2;	 
#endif
#endif
#if(FSYSCLK==FSYSCLK_6M5)
        TCON2 = 0x82;		
#if(C_Fq == 60)
        TL21=TH21=0xDE;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xE4;	 
#endif
#endif
        break;

        case BPS2400:
#if(FSYSCLK==FSYSCLK_3M2)
        TCON2 = 0xA2;		//波特率加倍 //波特率发生器时钟源clk 作为时钟源//波特率发生器的启动
#if(C_Fq == 60)
        TL21=TH21=0x9A;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xAB;	 
#endif
#endif
#if(FSYSCLK==FSYSCLK_6M5)
        TCON2 = 0xA2;		
#if(C_Fq == 60)
        TL21=TH21=0x34;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0x56;	 
#endif
#endif
        break;

        case BPS4800:

#if(FSYSCLK==FSYSCLK_3M2)
        TCON2 = 0xA2;
#if(C_Fq == 60)
        TL21=TH21=0xCD;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xD5;	 
#endif
#endif
#if(FSYSCLK==FSYSCLK_6M5)
        TCON2 = 0xA2;		
#if (C_Fq == 60)
        TL21=TH21=0x9A;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xAB;	 
#endif
#endif
        break;

        case BPS9600:
#if(FSYSCLK==FSYSCLK_3M2)
       TCON2 = 0xA2;
#if(C_Fq == 60)
        TL21=TH21=0xe7;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xEB;	 
#endif
#endif
#if(FSYSCLK==FSYSCLK_6M5)
        TCON2 = 0xA2;		
#if(C_Fq == 60)
        TL21=TH21=0xCD;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xD5;	 
#endif
#endif
        break;
        case BPS300:
        default:	// 300
#if(FSYSCLK==FSYSCLK_3M2)
        TCON2 = 0x82;				//波特率加倍 //波特率发生器时钟源clk/12 作为时钟源/
#if(C_Fq == 60)
        TL21=TH21=0xBE;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0xC7;	 
#endif
#endif
#if(FSYSCLK==FSYSCLK_6M5)
        TCON2 = 0x82;		
#if(C_Fq == 60)
        TL21=TH21=0x78;	 
#endif
#if(C_Fq == 50)
        TL21=TH21=0X8E;	 
#endif
#endif
        break;

    }

}


/*******************************************************************************************
** 函数名称: Set_Uart4_badu
** 功能描述: UART4波特率配置
** 入口参数: u16 badu：串口波特率，双字节16进制数据
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void Set_Uart4_badu(uint16 rate)
{
    TMOD4 = 0x20;                               // 8-bit counter with auto-reload
    switch(rate)
    {
        case BPS600:
		#if(FSYSCLK==FSYSCLK_3M2)
			TCON4 = 0x22;				//波特率不加倍 //波特率发生器时钟源clk 作为时钟源/
			#if (C_Fq == 60)
				TL41=TH41=0x33;  
			#else
				TL41=TH41=0x55;  
			#endif
		#endif
		#if(FSYSCLK==FSYSCLK_6M5)
			TCON4 = 0x82;				//波特率加倍 //波特率发生器时钟源clk/12 作为时钟源/
			#if (C_Fq == 60)
				TL41=TH41=0xBC;  
			#else
				TL41=TH41=0xC7;  
			#endif
		#endif
        break;

        case BPS1200:
		#if(FSYSCLK==FSYSCLK_3M2)
        	TCON4 = 0x82;//波特率加倍 //波特率发生器时钟源clk/12 作为时钟源/
			#if (C_Fq == 60)
				TL41=TH41=0xEF;  
			#else
				TL41=TH41=0xF2;  
			#endif
		#endif
		#if(FSYSCLK==FSYSCLK_6M5)
			TCON4 = 0x82;//波特率加倍 //波特率发生器时钟源clk/12 作为时钟源/
			#if (C_Fq == 60)
				TL41=TH41=0xDE;  
			#else
				TL41=TH41=0xE4;  
			#endif
		#endif
        break;

        case BPS2400:
		#if(FSYSCLK==FSYSCLK_3M2)
        	TCON4 = 0xA2;		//波特率加倍 //波特率发生器时钟源clk 作为时钟源//波特率发生器的启动
			#if (C_Fq == 60)
				TL41=TH41=0x9A;  
			#else
				TL41=TH41=0xAB;  
			#endif
		#endif
		#if(FSYSCLK==FSYSCLK_6M5)
	        TCON4 = 0xA2;		//波特率加倍 //波特率发生器时钟源clk 作为时钟源//波特率发生器的启动
			#if (C_Fq == 60)
		        TL41=TH41=0x33;	 
			#else
		        TL41=TH41=0x55;	 
			#endif
		#endif
        break;

        case BPS4800:
		#if(FSYSCLK==FSYSCLK_3M2)
			TCON4 = 0xA2;		//波特率加倍 //波特率发生器时钟源clk 作为时钟源//波特率发生器的启动
			#if (C_Fq == 60)
				TL41=TH41=0xCD;  
			#else
				TL41=TH41=0xD5;  
			#endif
		#endif
		#if(FSYSCLK==FSYSCLK_6M5)
			TCON4 = 0xA2;		//波特率加倍 //波特率发生器时钟源clk 作为时钟源//波特率发生器的启动
			#if (C_Fq == 60)
				TL41=TH41=0x9A;  
			#else
				TL41=TH41=0xAB;  
			#endif
		#endif			
        break;

        case BPS9600:
		#if(FSYSCLK==FSYSCLK_3M2)
			TCON4 = 0xA2;		//波特率加倍 //波特率发生器时钟源clk 作为时钟源//波特率发生器的启动
			#if (C_Fq == 60)
				TL41=TH41=0xE6;  
			#else
				TL41=TH41=0xEB;  
			#endif
		#endif	
		#if(FSYSCLK==FSYSCLK_6M5)
			TCON4 = 0xA2;		//波特率加倍 //波特率发生器时钟源clk 作为时钟源//波特率发生器的启动
			#if (C_Fq == 60)
				TL41=TH41=0xCD;  
			#else
				TL41=TH41=0xD5;  
			#endif
		#endif			
        break;
        case BPS300:
        default:	// 300
		#if(FSYSCLK==FSYSCLK_3M2)
			TCON4 = 0x82;				//波特率加倍 //波特率发生器时钟源clk/12 作为时钟源/
			#if (C_Fq == 60)
				TL41=TH41=0xBC;  
			#else
				TL41=TH41=0xC7;  
			#endif
		#endif
		#if(FSYSCLK==FSYSCLK_6M5)
			TCON4 = 0x82;				//波特率加倍 //波特率发生器时钟源clk/12 作为时钟源/
			#if (C_Fq == 60)
				TL41=TH41=0x77;  
			#else
				TL41=TH41=0x8E;  
			#endif
		#endif
        break;

    }

}



/*******************************************************************************************
** 函数名称: Dis_UART0_rx
** 功能描述: UART2串口接收禁止
** 入口参数: NO
** 出口参数: NO
** 说明    : 
*******************************************************************************************/
void En_UART2_rx(void)
{
    UART2_INITLIZE_ON(); 
    SCON2 = 0x50;        //8 位 UART，波特率由波特率发生器决定。  接收使能//
    ExInt2IFG &= ~(BIT0+BIT1);			
    ExInt2IE&=~BIT0;		//取消发送中断 //
    ExInt2IE|=BIT1;		//接收中断 en
    EIE|=BIT0;
}

/*******************************************************************************************
** 函数名称: Dis_UART0_rx
** 功能描述: UART0串口接收禁止
** 入口参数: NO
** 出口参数: NO
** 说明    : 
*******************************************************************************************/
/*void En_UART4_rx(void)
{
    UART4_INITLIZE_ON();      
//   SCON4 = 0x50;        //8 位 UART，波特率由波特率发生器决定。  接收使能  62056//
    SCON4=0xD0;                                 //数据位9位,8位数据位+1校验位// 9260+07
    ExInt2IFG &= ~(BIT2+BIT3);			
    ExInt2IE&=~BIT2;		//发送中断dis
    ExInt2IE|=BIT3;		//接收中断 en
    EIE|=BIT0;
}
*/
void En_UART4_rx(void)
{
 /*   SCON4=0xD0;                                 //数据位9位,8位数据位+1校验位// 9260+07
//    P2OE &= ~BIT1;                      //Uart4发送口输出允许
//    P2IE &= ~BIT1;                      //Uart4发送口输入禁止
//    P2OE |= BIT0;                       //Uart4接收口输出禁止
//    P2IE |= BIT0;                       //Uart4接收口输入允许
    P20FS=2;                            //P2.0配置成RXD
    P21FS=2;                            //P2.1配置成TXD
    EIP|=BIT0;
	
    ExInt2IE|=BIT2;
    ExInt2IE|=BIT3;
    EIE|=BIT0;
    ExInt2IFG&=(~BIT2);       //清中断标志
    ExInt2IFG&=(~BIT3);
    */
    
	 UART4_INITLIZE_ON(); 
	 SCON4 = 0x50;		  //8 位 UART，波特率由波特率发生器决定。  接收使能//
	 ExInt2IFG &= ~(BIT2+BIT3); 		 
	 ExInt2IE&=~BIT2;		 //取消发送中断 //
	 ExInt2IE|=BIT3;	 //接收中断 en
	 EIE|=BIT0;
}

/*******************************************************************************************
** 函数名称: Dis_UART0_rx
** 功能描述: UART0串口接收禁止
** 入口参数: NO
** 出口参数: NO
** 说明    : 
*******************************************************************************************/
void Dis_UART2_rx(void)
{
    ExInt2IE &= ~BIT1;	 //关闭串口接收中断//
}

/*******************************************************************************************
** 函数名称: Dis_UART0_rx
** 功能描述: UART0串口接收禁止
** 入口参数: NO
** 出口参数: NO
** 说明    : 
*******************************************************************************************/
void Dis_UART4_rx(void)
{
    ExInt2IE&=~BIT3;		//发送中断dis 
}

/*******************************************************************************************
** 函数名称: Dis_UART0_rx
** 功能描述: UART0串口接收禁止
** 入口参数: NO
** 出口参数: NO
** 说明    : 
*******************************************************************************************/
void En_UART2_Tx(void)
{
    ExInt2IFG &= ~(BIT0+BIT1);			
    ExInt2IE |=BIT0;		//开放发送中断 //
    ExInt2IE &= ~BIT1;	 //关闭串口接收中断//
    EIE|=BIT0;          //开放中断向量8  //
  //  UART2_PWM38K_EN();
}


/*******************************************************************************************
** 函数名称: Dis_UART0_rx
** 功能描述: UART0串口接收禁止
** 入口参数: NO
** 出口参数: NO
** 说明    : 
*******************************************************************************************/
void En_UART4_Tx(void)
{
    ExInt2IFG &= ~(BIT2+BIT3);			
    ExInt2IE |=BIT2;		//开放发送中断 //
    ExInt2IE &= ~BIT3;	 //关闭串口接收中断//
    EIE|=BIT0;          //开放中断向量8  //
}



/*******************************************************************************************
** 函数名称: Init_UART0_hard
** 功能描述: UART0串口硬件初始化
** 入口参数: u16 badu：串口波特率，双字节16进制数据
**           u16 bits_len: 通讯字节位数格式，8或7
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void Init_UART2_hard(uint16 badu)
{
     
    Set_Uart2_badu(badu);
    En_UART2_rx();

}

/*******************************************************************************************
** 函数名称: Init_UART0_hard
** 功能描述: UART0串口硬件初始化
** 入口参数: u16 badu：串口波特率，双字节16进制数据
**           u16 bits_len: 通讯字节位数格式，8或7
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void Init_UART4_hard(uint16 badu)
{
     
    Set_Uart4_badu(badu);
    En_UART4_rx();

}


/*******************************************************************************************
** 函数名称: Init_UART_soft
** 功能描述: UART[X]通道软件初始化
** 入口参数: uint8 ch: 通道序号X
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void Init_UART_soft(uint8 ch)
{
uint8 i;
    //init mem
    if(ch > UART_CH_INDEX_MAX )  return ;

    
    gs_uart_drv_var[ch].tx_len = 0;				//COMM_485_TX_LEN = 0 ;
    gs_uart_drv_var[ch].buff_index = 0;			//COMM_485_BUFF_INDEX = 0 ;
    gs_uart_drv_var[ch].buff_index_back = 0;	//COMM_485_BUFF_INDEX_BAK = 0 ;
    gs_uart_drv_var[ch].rx_over_ticks = 0;		//COMM_485_RX_OVERTIME_TICKS = 0 ;
    gs_uart_drv_var[ch].status = UART_STATUS_IDLE;		//COMM_485_RX_OVERTIME_TICKS = 0 ;
    gs_uart_drv_var[ch].bits_len = 7;		//COMM_485_RX_OVERTIME_TICKS = 0 ;
    for (i=0; i<UART_BUFF_SIZE; i++)
    {
        gs_uart_drv_var[ch].buff[i]=0;    
    }
}

/*******************************************************************************************
** 函数名称: Hdl_UART_10ms_run
** 功能描述: UART[X]通道每10ms处理
** 入口参数: uint8 ch: 通道序号X
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void Hdl_UART_10ms_run(uint8 ch)
{
    uint8 rx_len ;

	if(ch > UART_CH_INDEX_MAX )  return ;

    // 串口发送状态时，溢出定时寄存器不累加      //
    if (gs_uart_drv_var[ch].tx_len != 0)
    {
        gs_uart_drv_var[ch].rx_over_ticks = 0 ;
        return ;
    }

    //发送和发送结束状态有效时不接收数据 //
    if (gs_uart_drv_var[ch].status ==UART_STATUS_BUSY_TX)
    {
        gs_uart_drv_var[ch].rx_over_ticks = 0 ;
        return ;
    }
    
    if (gs_uart_drv_var[ch].status ==UART_STATUS_BUSY_TX_OVER)
    {
        if(ch==UART_CH0_IR)
        {
            En_UART4_rx();
        }
        if(ch==CH_RF)
        {
          //  En_UART4_rx();  通道兼容9260F 禁止操作串口
          nop();
        }
        Init_UART_soft(ch);
        return ;
    }


    // 未接收到数据时，溢出定时寄存器不累加     //
    rx_len = gs_uart_drv_var[ch].buff_index	;
    if (rx_len == 0)
    {
        gs_uart_drv_var[ch].rx_over_ticks = 0 ;
        return ;
    }

    gs_uart_drv_var[ch].status = UART_STATUS_BUSY_RX;
    // 接收到数据时，如果接收数据溢出     定时寄存器不累加     //
    if (rx_len ==gs_uart_drv_var[ch].buff_index_back)
    {
        gs_uart_drv_var[ch].rx_over_ticks++;
    }
    else
    {
        gs_uart_drv_var[ch].rx_over_ticks = 0 ;     	
    }
    gs_uart_drv_var[ch].buff_index_back = rx_len;


}


/*******************************************************************************************
** 函数名称: Tx_UART_frist
** 功能描述: UART[X]通道发送首字节处理函数
** 入口参数: uint8 *p_buf: 发送数据通道的缓存区首地址
**           uint8 len : 发送数据字节长度   
**           uint16 badu: 串口发送波特率
**           uint8 ch   : 串口通道号
** 出口参数: 无
** 说明    : 
*******************************************************************************************/
void Tx_UART_frist (uint8 *p_buf, uint8 len,uint16 badu,uint8 ch)
{    
    uint8 i ;
    uint8 bits_len;


    bits_len = 7;  //代表7位数据位 //
    //
    if (len > UART_BUFF_SIZE)
    {
    len = UART_BUFF_SIZE ;
    }

    if(ch > UART_CH_INDEX_MAX )  return ;
    
    for (i=0; i<len; i++)
    {
        gs_uart_drv_var[ch].buff[i] = p_buf[i] ;
    }  
    gs_uart_drv_var[ch].tx_len = len-1;
    gs_uart_drv_var[ch].buff_index = 1;
    gs_uart_drv_var[ch].buff_index_back = 0;
    gs_uart_drv_var[ch].status = UART_STATUS_BUSY_TX;

    if(ch==UART_CH0_IR)
    {
		DisableINT(); 
    	if(gs_uart_drv_var[ch].bits_len==7)
    	{
    	// 7位数据处理方式 //
    	    ACC=(gs_uart_drv_var[ch].buff[0] &0x7F);
    	    if(P)
    	    {
    	        gs_uart_drv_var[ch].buff[0] |= BIT7;
    	    }
    	    else
    	    {
    	        gs_uart_drv_var[ch].buff[0] &= ~BIT7;
    	    }
    	}
    	EnableINT();
        Set_Uart4_badu(badu);// 波特率切换
        Dis_UART4_rx();
        En_UART4_Tx();
        SBUF4=gs_uart_drv_var[ch].buff[0];   
    }

    if(ch==CH_RF)
    {
       G_LoRaConfig.PayloadLength = len;
   	   SX1276_TxPacket(&gs_uart_drv_var[ch].buff[0]);                    //发送数据
	   Rx_mode(); 
       gs_uart_drv_var[CH_RF].buff_index = 0;
       gs_uart_drv_var[CH_RF].status = UART_STATUS_BUSY_TX_OVER;   
       gs_uart_drv_var[CH_RF].tx_len =0 ;
    }

       

}


/*******************************************************************************************
** 函数名称: Get_UART_rx_buff
** 功能描述: 获取UART[X]通道接收的数据首地址和长度
** 入口参数: uint8 **p_buf: 用来返回接收数据的首地址数据
**           uint8 ch   : 串口通道号
** 出口参数: uint8 len : 接收数据长度
** 说明    : 
*******************************************************************************************/
uint8 Get_UART_rx_buff (uint8 **p_buf,uint8 ch)
{
    uint8 len ;
    
	if(ch > UART_CH_INDEX_MAX )  return 0;
    //

	*p_buf = &gs_uart_drv_var[ch].buff[0] ;
    
    //recv...
    if(gs_uart_drv_var[ch].rx_over_ticks <10)  //此处延时参数为判断数据接收结束的时间，需要配合时可以调整150ms  //
    {
        return 0;
    }

     // recv ok    
    gs_uart_drv_var[ch].rx_over_ticks = 0;
    gs_uart_drv_var[ch].status = UART_STATUS_BUSY_RX_OVER;

    if(ch==UART_CH0_IR)
    {
        //close recv
        Dis_UART4_rx () ;
    }

    if(ch==CH_RF)
    {
        //close recv
     // Dis_UART4_rx () ;通道兼容9260F 禁止操作串口
     nop();
    }
    //
    len = gs_uart_drv_var[ch].buff_index;
    gs_uart_drv_var[ch].buff_index = 0;
    gs_uart_drv_var[ch].buff_index_back = 0;

    if(len >UART_BUFF_SIZE)
    {
        len = UART_BUFF_SIZE;
    }
    
    return len ;
}



void Uart4_Transmit(void)
{
    uint8 i;
    if(gs_RacCtrl.ucStatus==Rac_Send)
    {
      if(gs_RacCtrl.ucSendPoint<gs_RacCtrl.ucSendLen)   
      {
        i=Lib_check_bitnum(gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint]);            //发送奇校验位
        if(i==0)
        {    
          SCON4 |=0x08;                        //偶校验的值  0x1906的bit3 1
        }
        else
        {    
          SCON4 &=0xf7;                  //奇校验的值  0x1906的bit3 0
        }
        SBUF4 = gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint++];
      }
      else
      {
        gs_RacCtrl.ucStatus=Rac_Rev;  
      }
    }
}

/*******************************************************************************************
**    END
*******************************************************************************************/

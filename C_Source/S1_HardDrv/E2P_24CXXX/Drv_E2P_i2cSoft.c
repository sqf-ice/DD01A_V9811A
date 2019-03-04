/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_E2P_i2cSoft.c
**Author		: maji
**date			: 2016-04-19
**description	: c code for e2p's I2C bus operation 
**note			: 适用于中颖G80F92XD芯片
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-19   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>



/*******************************************************************************************
** 函数名称: E2p_IIC_Initail
** 函数描述: I2C总线硬件初始化
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void E2p_IIC_Initail(void)
{
	I2C_DIO_IN;      // DIO =1  //
	I2C_CLK_HIGH;    // CLK =1  //
	I2C_CLK_OUT;
}


/*******************************************************************************************
** 函数名称: E2p_Start_i2c
** 函数描述: 模拟发送I2C总线起始信号 , SCL为高电平时，SDA发出下降沿信号  
** 输入参数: 无
** 输出参数: 无
** 说明    : SCL=0  SDA=0 为起始状态
*******************************************************************************************/
void E2p_Start_i2c(void)
{   
    I2C_DIO_HIGH;                                  // SDA  = 1
    I2C_CLK_HIGH;                                  // SCL  = 1
    HOLD_I2C_CLK();    
    I2C_DIO_LOW;                                  // SDA = 0
    HOLD_I2C_CLK();
    I2C_CLK_LOW ;                                 // SCL= 0
}


/*******************************************************************************************
** 函数名称: E2p_Stop_i2c
** 函数描述: 模拟发送I2C总线停止信号 , SCL为高电平时，SDA发出上升沿信号    
** 输入参数: 无
** 输出参数: 无
** 说明    : SCL=1  SDA=1 为停止状态/空闲状态
*******************************************************************************************/
void E2p_Stop_i2c(void)
{	
	//防冲突处理 //
/*	I2C_DIO_HIGH;
	while(I2C_DIO_JUDGE==0)
	{
		if(I2C_CLK_JUDGE==0)
		{
			I2C_CLK_HIGH;	
		}
		else
		{
			I2C_CLK_LOW;
		}
		HOLD_I2C_CLK();
	}

	I2C_CLK_LOW;
*/
	// 正式停止命令处理 //

    I2C_DIO_LOW ;                        // SDA = 0
	HOLD_I2C_CLK();
    I2C_CLK_HIGH;                        // SCL = 1
	HOLD_I2C_CLK();
    I2C_DIO_HIGH;                        //SDA = 1
    HOLD_I2C_CLK();   
}


/*******************************************************************************************
** 函数名称: E2p_TxByte_i2c
** 函数描述: 模拟I2C总线发送一个字节数据 
** 输入参数: uint8 txData 需要发送的8位数据
** 输出参数: uint8 发送结果，=TRUE代表成功，=FALSE代表失败
** 说明    :1. SCL上升沿发出数据//
**          2. E2P回的应答信号在最后一个CLK变低时就会发出
*******************************************************************************************/
uint8 E2p_TxByte_i2c(uint8 txData)
{
	uint8 data i=0;

	// 8BITS 数据发送  //
	for (i=0;i<8;i++)
	{
		I2C_CLK_LOW ;               // SCL= 0  ,very importent
		if( txData & 0x80 )
		{
			I2C_DIO_HIGH;             //SDA = 1
		}    
		else
		{
			I2C_DIO_LOW ;            // SDA = 0 
		}
		txData <<= 1;
		I2C_CLK_HIGH;                // SCL = 1
		HOLD_I2C_CLK();      
	}

	// 接收判断总线应答信号，低电平有效//
	I2C_CLK_LOW ;               // SCL= 0  ,very importent
	I2C_DIO_IN;
	i=32;
	for(i=0;i<32;i++)
	{
		if(I2C_DIO_JUDGE == 0)  // 有应答信号 //
		{
			I2C_CLK_HIGH;
			HOLD_I2C_CLK();
			I2C_CLK_LOW;
			return TRUE;
		}
	}

	//总线无应答信号后发送总线停止命令  //
	I2C_CLK_HIGH;
	HOLD_I2C_CLK();
	I2C_CLK_LOW;
	E2p_Stop_i2c();

	return FALSE;
}


/*******************************************************************************************
** 函数名称: E2p_RxByte_i2c
** 函数描述: 模拟I2C总线接收一个字节数据 
** 输入参数: uint8 ack： =FALSE不发送应答信号,= TRUE 发送低电平应答信号
** 输出参数: uint8 接收到的字节数据
** 说明    :1. SCL上升沿接收数据//
**          2. 连续接收数据时的最后一个字节首不用发送ACK命令
*******************************************************************************************/
uint8 E2p_RxByte_i2c(uint8 u8_ack)
{
	uint8 data  i=0;
	uint8 data rxData=0;

	I2C_DIO_IN;					//SDA_DIR = IN  //
	I2C_CLK_LOW ;                         // SCL= 0  

	// 接收一个字节数据代码段 //
	for (i=0;i<8;i++)
	{
		rxData <<= 1;
		I2C_CLK_HIGH;                    // SCL = 1
		HOLD_I2C_CLK();
		if(I2C_DIO_JUDGE !=0)
		{
			rxData |= 0x01; 	
		}
		I2C_CLK_LOW ;            // SCL= 0 
		HOLD_I2C_CLK(); 
	}

	//根据输入参数判断是否需要输出应答低电平信号  //
	if(u8_ack != FALSE)
	{
		I2C_DIO_LOW;	// 发送应答信号 //
	}
	else
	{
		I2C_DIO_HIGH;  // 不发送应答信号 //
	}

	I2C_CLK_HIGH;                    // SCL = 1
	HOLD_I2C_CLK();
	I2C_CLK_LOW ;            // SCL= 0 
	I2C_DIO_HIGH;			//SDA = 1 //

	return(rxData);         // //
}


/***************************************************************
*    END
****************************************************************/

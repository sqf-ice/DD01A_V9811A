/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name        : SYS_IOdefine_H.H
**Author      : 
**date        : 2016-05-06
**description : define for  系统IO口相关定义
**note        : memer type  
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26  v01.00   sosomj     1. frist version                             
** 2  2016- 5-6  v01.00  zwk    
**==============================================================
*/


#ifndef 	_SYS_IOdefine_H
#define	_SYS_IOdefine_H

//******************************************************************
//  E2P 24CXXX  i2c  PORT DEFINE    //
//******************************************************************
#define PIN_E2SDA		   BIT1       //DIO   p1.5
#define PIN_E2SCL		   BIT2       //CLK   p1.2
#define I2C_PORT_OUT	   P1OD
#define I2C_PORT_IN	       P1ID
#define I2C_DIR_IN		   P1IE
#define I2C_DIR_OUT		   P1OE

#define I2C_DIO_IN			do{I2C_DIR_OUT |= PIN_E2SDA; I2C_DIR_IN |= PIN_E2SDA;  } while(0)
#define I2C_DIO_OUT			do{I2C_DIR_IN &= (~PIN_E2SDA);I2C_DIR_OUT &= (~PIN_E2SDA); }  while(0)
#define I2C_CLK_IN			do{I2C_DIR_IN |= PIN_E2SCL;I2C_DIR_OUT |= PIN_E2SCL; }  while(0)
#define I2C_CLK_OUT			do{I2C_DIR_OUT &= (~PIN_E2SCL); I2C_DIR_IN &= (~PIN_E2SCL);}  while(0)

#define I2C_DIO_HIGH    I2C_DIO_IN    //{I2C_PORT_OUT |=PIN_E2SDA; }	采用输入态的上拉电阻输出高，避免总线冲突 //	
#define I2C_DIO_LOW     do{I2C_PORT_OUT &=(~PIN_E2SDA); I2C_DIO_OUT;}  while(0) 	//采用输出0方式 //	
#define I2C_CLK_HIGH    do{I2C_PORT_OUT |=PIN_E2SCL; } while(0)
#define I2C_CLK_LOW     do{I2C_PORT_OUT &=(~PIN_E2SCL); } while(0)
#define I2C_DIO_JUDGE  	(I2C_PORT_IN&PIN_E2SDA)
#define I2C_CLK_JUDGE  	(I2C_PORT_IN&PIN_E2SCL)

#define I2C_PORT_INITLIZE()  do{I2C_DIO_IN;I2C_CLK_HIGH;I2C_CLK_OUT;}  while(0)  
//******************************************************************
// RS485_UART PORT DEFINE    //  UART4 
//******************************************************************
#define PIN_UART4_RX    BIT0
#define PIN_UART4_TX    BIT1
#define UART4_FUNC_EN()      do{P2OE &= ~PIN_UART4_TX;P2IE |=PIN_UART4_RX; P20FS = 0X02;P21FS = 0X02;} while(0)
#define UART4_FUNC_DIS()     do {P2OE |= PIN_UART4_TX;P2IE &=~PIN_UART4_RX; P20FS = 0;P21FS = 0;} while(0)
#define UART4_TX_DIR_OUT()   do{P2OE &= ~PIN_UART4_TX;} while(0)
#define UART4_RX_DIR_IN()    do{P2IE |= PIN_UART4_RX;} while(0)
#define UART4_TX_HIGH()      do{P2OD |= PIN_UART4_TX;} while(0)
#define UART4_TX_LOW()       do{P2OD &= ~PIN_UART4_TX;} while(0)

#define UART4_INITLIZE_OFF()  do{UART4_FUNC_DIS();UART4_TX_HIGH();UART4_TX_DIR_OUT() ;UART4_RX_DIR_IN();} while(0)
#define UART4_INITLIZE_ON()   do{UART4_TX_HIGH();UART4_TX_DIR_OUT() ;UART4_RX_DIR_IN();UART4_FUNC_EN();} while(0)

//******************************************************************
// FAR IR_UART PORT DEFINE    // TTL
//******************************************************************
#define PIN_UART2_RX    BIT4  
#define PIN_UART2_TX    BIT5
#define UART2_FUNC_EN()   do{P2OE &= ~PIN_UART2_TX;P2IE |=PIN_UART2_RX; P24FS = 0X02;P25FS = 0X02;} while(0)
#define UART2_FUNC_DIS()   do{P2OE |= PIN_UART2_TX;P2IE &=~PIN_UART2_RX; P24FS = 0;P25FS = 0;} while(0)
#define UART2_TX_DIR_OUT()   do{P2OE &= ~PIN_UART2_TX;} while(0)
#define UART2_RX_DIR_IN()   do{P2IE |= PIN_UART2_RX;} while(0)
#define UART2_TX_HIGH()   do{P2OD |= PIN_UART2_TX;} while(0)
#define UART2_TX_LOW()   do{P2OD &= ~PIN_UART2_TX;} while(0)
#define UART2_PWM38K_EN()   do{P25FS = 0X02;Txd2FS &=~ BIT0;CARRHH= 0;CARRHL=61;CARRLH= 0;CARRLL=111;} while(0) // 带有载波信号38K//
#define UART2_PWM38K_DIS()   do{P25FS = 0X00;Txd2FS |=BIT0;UART2_TX_LOW();UART2_TX_DIR_OUT();} while(0) //关闭串口发送功能 //

#define UART2_INITLIZE_OFF()  do{UART2_FUNC_DIS();UART4_TX_HIGH();UART2_TX_DIR_OUT() ;UART2_RX_DIR_IN();} while(0)
#define UART2_INITLIZE_ON() do {UART2_TX_HIGH();UART4_TX_DIR_OUT() ;UART2_RX_DIR_IN();UART2_FUNC_EN();P25FS = 0X02;Txd2FS|=BIT0;} while(0)

//******************************************************************
//LED port define  //
//******************************************************************
/*
#define PIN_LED_L1_FAIL   BIT6    //  A相断相指示  //  P7.6
#define LED_L1_FAIL_ON()      do{P7OE &= ~PIN_LED_L1_FAIL; P7OD |= PIN_LED_L1_FAIL;} while(0)
#define LED_L1_FAIL_OFF()     do{P7OE &= ~PIN_LED_L1_FAIL; P7OD &=(~PIN_LED_L1_FAIL);} while(0)
*/
//******************************************************************
//******************************************************************
#define PIN_KEY_ERR_ADJ   BIT4    //P2.4  
#define KEY_ERR_ADJ_INITLIZE()  do{P2IE |= PIN_KEY_ERR_ADJ;P2OE |= PIN_KEY_ERR_ADJ;P24FS=0x00; } while(0)
#define KEY_READ_ERR_ADJ  (P2ID&PIN_KEY_ERR_ADJ)

#define PIN_KEY_ERR_COVER   BIT0    //P2.4  
#define KEY_ERR_COVER_INITLIZE()  do{P9IE |= PIN_KEY_ERR_COVER;P9OE |= PIN_KEY_ERR_COVER; } while(0)
#define KEY_READ_ERR_COVER  (P9DI&PIN_KEY_ERR_COVER)

//******************************************************************
#define P_RELAY_CTL_OFF   BIT1    //P1.1  
#define P_RELAY_CTL_ON   BIT2    //P1.2  
#define CTL_Relay_OFF() 	{ (P1OD |= P_RELAY_CTL_OFF) ; (P1OD &= (~P_RELAY_CTL_ON)) ;}//合闸
#define CTL_Relay_ON() 		{ (P1OD |= P_RELAY_CTL_ON) ; (P1OD &= (~P_RELAY_CTL_OFF)) ;}//拉闸
#define CTL_Relay_FREE() 	{(P1OD &= (~P_RELAY_CTL_ON)) ;  (P1OD &= (~P_RELAY_CTL_OFF)) ;}

#define PIN_Relay_INITLIZE()  {P1OE &= (~(P_RELAY_CTL_OFF+P_RELAY_CTL_ON));P1IE &= (~(P_RELAY_CTL_OFF+P_RELAY_CTL_ON));}while(0)

// other  ports define  //
//******************************************************************
#define PIN_RTC_OPEN()    do{P13FS = 0x05;P1OE&=~BIT3;P1IE&=~BIT3;} while(0)  // P1.3    CF //
//#define PIN_CF_OPEN()    do{P13FS = 0x04;P1OE&=~BIT3;P1OD|=BIT3;} while(0)  // P1.3    CF //
//#define PIN_CF_CLOSE()   do{P13FS = 0x00;P1OE&=~BIT3;P1IE&=~BIT3; P1OD =0x00;}  while(0) // P1.3    RTC 关闭 //
#define PIN_CFLED	             BIT5		// enarge pluse led port,  DIR = O  //
#define PIN_CF_OPEN()  {P9FC |= PIN_CFLED; P9OE &= ~PIN_CFLED;P9DO&= ~PIN_CFLED;}
#define PIN_CF_CLOSE()     {P9FC &= ~PIN_CFLED; P9OE &= ~PIN_CFLED;P9DO&= ~PIN_CFLED;}

// DI0 p8.1
#define LSD4RF_SPI_SET_DIO0_DIR_IN() do{P8IE|=BIT1;P8OE|=BIT1;}while(0) 
#define LSD4RF_READ_DIO0()                 ((P8ID & BIT1))

/***************************************************************
*    END
****************************************************************/

#endif

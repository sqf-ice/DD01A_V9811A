/*******************************************************************
* Copyright (c) 2009,     HXGROUP
* All rights reserved.
* 
* 文件名称：spiSoft.h
* 摘    要：      soft spi 函数模块声明
*            
* 应用系统：
*
* 版本号：    1.0   作者：zwk  
*完成日期：2009.02.16
****************************************************************/



// RFSI 9.2
#define SPI_D0_HIGH()   do{P9IE&= ~BIT2;P9OE&= ~BIT2;P9DO|=BIT2;P9FC &=~BIT2; }while(0)
#define SPI_D0_LOW()    do{P9IE&= ~BIT2;P9OE&= ~BIT2;P9DO&= ~BIT2; P9FC &=~BIT2;}while(0)

//RFS0 9.1
#define SPI_DI_HIGH()    do{P9IE&= ~BIT1;P9OE&= ~BIT1;P9DO|=BIT1;  P9FC &=~BIT1;}while(0)
#define SPI_DI_LOW()     do{P9IE&= ~BIT1;P9OE&= ~BIT1;P9DO&= ~BIT1; P9FC &=~BIT1 }while(0)

//RFSCK  P9.0
#define SPI_CLK_HIGH()    do{P9IE&= ~BIT0;P9OE&= ~BIT0;P9DO|=BIT0; P9FC &=~BIT0;}while(0)
#define SPI_CLK_LOW()     do{P9IE&= ~BIT0;P9OE&= ~BIT0;P9DO&= ~BIT0;P9FC &=~BIT0; }while(0)

//RFNSS   p2.5
#define SPI_CS_HIGH()    do{P2IE&= ~BIT5;P2OE&= ~BIT5;P2OD|=BIT5; P25FS =0;}while(0)
#define SPI_CS_LOW()     do{P2IE&= ~BIT5;P2OE&= ~BIT5;P2OD&= ~BIT5;P25FS =0; }while(0)

//RFSO  p9.1
#define   SPI_DI_Enable()   do{P9IE|=BIT1;P9OE|=BIT1; P9FC &=~BIT1;}while(0) 
#define   SPI_DI_JUDGE      ((P9DI& BIT1))            

//RFSI   p9.2
#define   SPI_DO_Enable()  do{P9IE|=BIT2;P9OE|=BIT2;P9FC &=~BIT2;}while(0)  
#define   SPI_DO_JUDGE      ((P9DI& BIT2))            


//RFRST   P8.2
#define LSD_RF_SXRESET_HIGH()   do{P8IE&= ~BIT2;P8OE&= ~BIT2;P8OD|=BIT2; }while(0)
#define LSD_RF_SXRESET_LOW()    do{P8IE&= ~BIT2;P8OE&= ~BIT2;P8OD&= ~BIT2;}while(0)


// DI0 p8.1
#define LSD_RF_SET_DIO0_DIR_IN() do{P8IE|=BIT1;P8OE|=BIT1;}while(0) 
#define LSD_RF_READ_DIO0()                 ((P8ID & BIT1))

//#define LSD_RF_SPI_SET_DIO1_DIR_IN() do{P4IE|=BIT0;P4OE|=BIT0;}while(0) 
//#define LSD_RF_READ_DIO1()   ((P4ID & BIT0))
//#define LSD_RF_SPI_SET_DIO3_DIR_IN() do{P3IE|=BIT3;P3OE|=BIT3;}while(0) 
//#define LSD_RF_READ_DIO3()  ((P3ID & BIT3))

#define LORA_RF_SPI_INITLIZE()  {P2OE&= (~BIT5); P9OE&= (~BIT0);P9OE&= (~BIT2);P9OE|= BIT1; P9IE|= BIT1;P8OD&= (~BIT2);P8OE|= BIT1; P8IE|= BIT1;}

//#define RF_send()       (P1ID&BIT1)                           //表盖检测按键

 unsigned char LSD_RF_SpiInOut (unsigned char mosi) ;



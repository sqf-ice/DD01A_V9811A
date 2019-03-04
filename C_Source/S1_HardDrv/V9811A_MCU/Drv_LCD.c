/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_LCD.c
**Author		: maji
**date			: 2016-04-20 
**description	: LCD模块的硬件驱动C代码
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>



/*******************************************************************************************
** 函数名称: LCD_ON
** 函数描述: LCD打开显示
** 输入参数: 无
** 输出参数: 无 
** 说明    : 
*******************************************************************************************/
void LCD_ON(void)
{
    LCDG=0;		    //开LCD时钟
    CtrlLCDV=0;	     // 3.3V
    LCDCtrl= 0XC2;       //6C0TYPE=0, 6COM,300K,256HZ
}

/*******************************************************************************************
** 函数名称: LCD_OFF
** 函数描述: LCD关闭显示
** 输入参数: 无
** 输出参数: 无 
** 说明    : 
*******************************************************************************************/
void LCD_OFF(void)
{
   LCDG = 1;   // 关闭 //
}


//*****************************************************************
//  Function:      void Lcd_initial(void)       
//  Description:                                           
//  Others:           
//***************************************************************
void Clr_all_lcd(void)
{
	LCDM8 =  0x00;    
	LCDM9 =  0x00;  
	LCDM10 = 0x00; 
	LCDM11 =  0x00; 
	LCDM12 =  0x00; 
	LCDM13 =  0x00; 
	LCDM14 =  0x00; 
	LCDM15 =  0x00; 
	LCDM18 =  0x00; 
	LCDM19 =  0x00; 
	LCDM20 =  0x00; 
}

/*******************************************************************************************
** 函数名称: Full_SEG
** 函数描述: 将SEG填入指定的值
** 输入参数: val:要填入的值
** 输出参数: 无 
** 说明    :   V9821   4com
*******************************************************************************************/
void Full_SEG(uint8 val)
{
	LCDM0 =  val&0XFF;     
	LCDM4 =  val&0XFF;     
	LCDM5 =  val&0XFF;     
	LCDM6 =  val&0XFF;    
	LCDM7 =  val&0XFF;    
	LCDM9 =  val&0XFF;    
	LCDM10 =  val&0XFF;    
	LCDM11 =  val&0XFF;    
	LCDM15 =  val&0XFF; 
	LCDM16 =  val&0XFF;				//2018-11-15 cxy
	LCDM17 =  val&0X0F;   		    //2018-11-15 cxy
	LCDM19 =  val&0XFF;    
}

/*******************************************************************************************
** 函数名称: LCD_Init
** 函数描述: LCD初始化
** 输入参数: 无
** 输出参数: 无 
** 说明    : 
*******************************************************************************************/
void Init_LCD(void)
{
//LCD显示，必须配置为禁止输入输出

   P3OE |=(BIT0+BIT1+BIT2+BIT3);           // 3.0 - 3.3 com口
    P4OE |= (BIT0+BIT1);           // P4.0~P4.1
    P5OE |= 0xFF;           // P5.0~P5.7   
    P6OE |= (BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);           // P6.2~P6.7   
    P7OE |= (BIT6+BIT7);           // P7.6~P7.7   
	P8OE |= (BIT0+BIT1+BIT2);     //P8.0~P8.2   cxy 2018-11-13新增管脚
	
    P3IE &=~  (BIT0+BIT1+BIT2+BIT3); 
    P4IE &=~  (BIT0+BIT1);         
    P5IE &=~  0xFF;          // P5.0~P5.1
    P6IE &=~ (BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);          // P6.0~P6.3
    P7IE &=~ (BIT6+BIT7);           // P7.6~P7.7   
    P8IE &=~ (BIT0+BIT1+BIT2);      //P8.0~P8.2   cxy 2018-11-13新增管脚
    
    LCD_ON();
    //SegCtrl0: seg0~7,  seg0~1 有效  //
    SegCtrl0=Bin(00000011);      // seg0~seg1  ON //
    //SegCtrl1: seg8~15,  seg8~15 有效  //
    SegCtrl1=Bin(11111111);	     // seg8~seg15  ON//

	//SegCtrl2: seg16~23,  seg18~20 有效  //
    SegCtrl2=Bin(11111100);     // seg18~seg23  ON//
    //SegCtrl3: seg24~31,  seg30~31 有效  //
    SegCtrl3=Bin(11000000);     // seg30~seg31  ON//
    //SegCtrl4: seg32~39,  seg32~34,  seg38~39有效  //
    SegCtrl4=Bin(11000111);     // seg32~seg34    seg38 seg39  ON//	cxy 2018-11-13

}


//*****************************************************************
//  Function:       void InitLCD(void)           
//  Description:                                           
//  Others:        len 为   
//***************************************************************
void Write_LCD(INT8U* bf_i2cData)
{ 
	
	LCDM0 =  (*(bf_i2cData+6)>>4)&0X0F|((*(bf_i2cData+0))&0XF0);     
	LCDM4 =  ((*(bf_i2cData+0))&0X0F)|((*(bf_i2cData+1))&0XF0);    
	LCDM5 =  (*(bf_i2cData+1))&0X0F|((*(bf_i2cData+2))&0XF0);     
	LCDM6 =  (*(bf_i2cData+2))&0X0F|((*(bf_i2cData+3))&0XF0);     
	LCDM7 =  (*(bf_i2cData+3))&0X0F|((*(bf_i2cData+4))&0XF0);     
	LCDM9 =  (*(bf_i2cData+4))&0X0F|((*(bf_i2cData+5))&0XF0);     
	LCDM10 = (*(bf_i2cData+5))&0X0F|((*(bf_i2cData+9))&0xF0);     	  //P5 S5 S3 S1
	LCDM11 = (*(bf_i2cData+9))&0X0F|((*(bf_i2cData+10))&0xF0);   	  //S7 S8 S9 S10  / S6 S4 S2 
	LCDM15 = (*(bf_i2cData+6))&0X0F|((*(bf_i2cData+7))&0xF0);		 
	LCDM16 = (*(bf_i2cData+7))&0X0F|((*(bf_i2cData+8))&0xF0);
	LCDM17 = (*(bf_i2cData+8))&0X0F;     
	LCDM19 = (*(bf_i2cData+10))&0X0F|((*(bf_i2cData+11))&0xF0);  //T5 T6 S12 S11  T4 T3 T2 T1 

}


/*******************************************************************************************
**    END
*******************************************************************************************/

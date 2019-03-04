/****************************************Copyright (c)**************************************************
**                                    南京立超电子有限公司                              
**                                        技  术  部 
**                                    http://www.dycmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: G80F92XD.h
**创   建   人: 
**最后修改日期: 2013-10-15
**描        述: G80F93X芯片特殊功能寄存器定义
********************************************************************************************************/
#ifndef __G80F92XD_H__
#define __G80F92XD_H__

//#include <absacc.h>

#define XBYTE 		((unsigned char volatile xdata *) 0)

/* CPU */
          
sfr ACC = 0xE0;
sfr B = 0xF0;
sfr AUXC = 0xF1;
sfr PSW = 0xD0;  
sfr SP = 0x81;
sfr DPL = 0x82;
sfr DPH = 0x83;
sfr DPL1 = 0x84;
sfr DPH1 = 0x85;
sfr INSCON = 0x86;
sfr XPAGE = 0xF7;

/* POWER  */
sfr PCON = 0x87;
sfr SUSLO = 0x8E;
sfr PASLO = 0xE7;

/* LPD */
sfr LPDCON = 0xB3;
sfr LPDCON1 = 0xBF;
sfr LDOCON = 0xDF;

/* FLASH */
sfr IB_OFFSET = 0xFB;
sfr IB_DATA = 0xFC;
sfr IB_CON1 = 0xF2;
sfr IB_CON2 = 0xF3;
sfr IB_CON3 = 0xF4;
sfr IB_CON4 = 0xF5;
sfr IB_CON5 = 0xF6;

/* ISP */
sfr ISPLO = 0xA5;
sfr ISPCON = 0xA6;
sfr FLASHCON = 0xA7;

/* WDT */
sfr RSTSTAT = 0xB1;

/* CLOCK */
sfr CLKCON = 0xB2;
sfr OSCLO = 0xB6;

/* INTERRUPT */
sfr IEN0 = 0xA8;
sfr IEN1 = 0xA9;
sfr IPL0 = 0xB8;
sfr IPL1 = 0xB9;
sfr IPH0 = 0xB4;
sfr IPH1 = 0xB5;

/* PORT */
sfr P0 = 0x80;
sfr P1 = 0x90;
sfr P2 = 0xA0;
sfr P3 = 0xB0;
sfr P4 = 0xC0;
sfr P5 = 0xCF;
sfr P0CR = 0xE1;
sfr P1CR = 0xE2;
sfr P2CR = 0xE3;
sfr P3CR = 0xE4;
sfr P4CR = 0xE5;
sfr P5CR = 0xE6;
sfr P0PCR = 0xE9;
sfr P1PCR = 0xEA;
sfr P2PCR = 0xEB;
sfr P3PCR = 0xEC;
sfr P4PCR = 0xED;
sfr P5PCR = 0xEE;
sfr PXMOD = 0xEF;
sfr P2DRV = 0xF9;
sfr P3DRV = 0xFA;

/* TIMER */
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0 = 0x8A;
sfr TH0 = 0x8C;
sfr TL1 = 0x8B;
sfr TH1 = 0x8D;
sfr T2CON = 0xC8;
sfr T2MOD = 0xC9;
sfr RCAP2L = 0xCA;
sfr RCAP2H = 0xCB;
sfr TL2 = 0xCC;
sfr TH2 = 0xCD;
sfr TCON1 = 0xCE;

/* EUART0 */
sfr SCON = 0x98;
sfr SBUF = 0x99;
sfr SADDR = 0x9A;
sfr SADEN = 0x9B;
sfr SBRTH = 0x9C;
sfr SBRTL = 0x9D;
sfr SFINE = 0x9E;

/* EUART1 */
sfr SCON1 = 0xD8;
sfr SBUF1 = 0xD9;
sfr SADDR1 = 0xDA;
sfr SADEN1 = 0xDB;
sfr SBRT1H = 0xDC;
sfr SBRT1L = 0xDD;

/* EUART2 */
sfr SCON2 = 0xF8;
sfr SBUF2 = 0xBA;
sfr SADDR2 = 0xBB;
sfr SADEN2 = 0xBC;
sfr SBRT2H = 0xBD;
sfr SBRT2L = 0xBE;	
sfr SFINE2 = 0xDE;

/* IR */
sfr IRCON = 0xA1;

/* ADC */
sfr ADCON = 0xC1;
sfr ADT = 0xC2;
sfr ADCH = 0xAF;
sfr ADDL = 0x91;
sfr ADDH = 0x92;

/* LCD */
sfr LCDCON1 = 0xA2;
sfr LCDCON = 0xA3;
sfr P0SS = 0xAA;
sfr P1SS = 0xAB;
sfr P3SS = 0xAC;
sfr P4SS = 0xAD;
sfr P5SS = 0xAE;

/* EMU */
sfr EADR =  0xD1;
sfr EDTAH = 0xD2;
sfr EDTAM = 0xD3;
sfr EDTAL = 0xD4;
sfr EMUSR = 0xD5;
sfr EMUIE = 0xD6;
sfr EMUIF = 0xD7;

/* EXF */
sfr EXF0 =  0xE8;

/*PWM*/
sfr PWM0CON = 0xC3;
sfr PWM1CON = 0x93;
sfr PWM0PH = 0xC7;
sfr PWM0PL = 0xC6;
sfr PWM0DH = 0xC5;
sfr PWM0DL = 0xC4;
sfr PWM1PH = 0x97;
sfr PWM1PL = 0x96;
sfr PWM1DH = 0x95;
sfr PWM1DL = 0x94;

/*--------------------------  BIT Register -------------------- */
/*  PSW   */
sbit CY = 0xD7;
sbit AC = 0xD6;
sbit F0 = 0xD5;
sbit RS1 = 0xD4;
sbit RS0 = 0xD3;
sbit OV = 0xD2;
sbit F1 = 0xD1;
sbit P = 0xD0;

/*  TCON  */
sbit TF1 = 0x8F;
sbit TR1 = 0x8E;
sbit TF0 = 0x8D;
sbit TR0 = 0x8C;
sbit IE1 = 0x8B;
sbit IT1 = 0x8A;
sbit IE0 = 0x89;
sbit IT0 = 0x88;

/*  T2CON  */
sbit TF2 = 0xCF;
sbit EXF2 = 0xCE;
sbit EXEN2 = 0xCB;
sbit TR2 = 0xCA;
sbit C_T2 = 0xC9;
sbit CP_RL2 = 0xC8;

/*  IPL0   */ 
sbit PADTPL = 0xBE;
sbit PT2L = 0xBD;
sbit PS0L = 0xBC;
sbit PT1L = 0xBB;
sbit PX1L = 0xBA;
sbit PT0L = 0xB9;
sbit PX0L = 0xB8;

/*  IEN0   */
sbit EA = 0xAF;
sbit EADTP = 0xAE;
sbit ET2 = 0xAD;
sbit ES0  = 0xAC;
sbit ET1 = 0xAB;
sbit EX1 = 0xAA;
sbit ET0 = 0xA9;
sbit ET= 0xA8;

/*  EXF0  */
sbit EMUF = 0xEE;
sbit IT31 = 0xED;
sbit IT30 = 0xEC;
sbit IT21 = 0xEB;
sbit IT20 = 0xEA;
sbit IE3 = 0xE9;
sbit IE2 = 0xE8;

/*  SCON  */
sbit SM0_FE = 0x9F;
sbit SM1_RXOV = 0x9E;
sbit SM2_TXCOL = 0x9D;
sbit REN = 0x9C;
sbit TB8 = 0x9B;
sbit RB8 = 0x9A;
sbit TI = 0x99;
sbit RI = 0x98;

/*  SCON1  */
sbit SM10_FE1 = 0xDF;
sbit SM11_RXOV1 = 0xDE;
sbit SM21_TXCOL1 = 0xDD;
sbit REN1 = 0xDC;
sbit TB81 = 0xDB;
sbit RB81 = 0xDA;
sbit TI1 = 0xD9;
sbit RI1 = 0xD8;

/*  SCON2  */
sbit SM20_FE2 = 0xFF;
sbit SM21_RXOV2 = 0xFE;
sbit SM22_TXCOL2 = 0xFD;
sbit REN2 = 0xFC;
sbit TB82 = 0xFB;
sbit RB82 = 0xFA;
sbit TI2 = 0xF9;
sbit RI2 = 0xF8;

/* P0 */
sbit P0_0 = P0^0;
sbit P0_1 = P0^1;
sbit P0_2 = P0^2;
sbit P0_3 = P0^3;
sbit P0_4 = P0^4;
sbit P0_5 = P0^5;
sbit P0_6 = P0^6;
sbit P0_7 = P0^7;

/* P1 */
sbit P1_0 = P1^0;
sbit P1_1 = P1^1;
sbit P1_2 = P1^2;
sbit P1_3 = P1^3;
sbit P1_4 = P1^4;
sbit P1_5 = P1^5;
sbit P1_6 = P1^6;
sbit P1_7 = P1^7;

/* P2 */
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;
sbit P2_4 = P2^4;
sbit P2_5 = P2^5;
sbit P2_6 = P2^6;
sbit P2_7 = P2^7;

/* P3 */
sbit P3_0 = P3^0;
sbit P3_2 = P3^2;
sbit P3_3 = P3^3;
sbit P3_4 = P3^4;
sbit P3_5 = P3^5;
sbit P3_6 = P3^6;

/* P4 */
sbit P4_0 = P4^0;
sbit P4_1 = P4^1;
sbit P4_2 = P4^2;
sbit P4_3 = P4^3;
sbit P4_4 = P4^4;
sbit P4_5 = P4^5;
sbit P4_6 = P4^6;
sbit P4_7 = P4^7;


/* 扩展寄存定义 */
/* RTC */ 
#define  SBSC      XBYTE[0xFFA0]
#define  SEC       XBYTE[0xFFA1]
#define  MIN       XBYTE[0xFFA2]
#define  HR        XBYTE[0xFFA3]
#define  DAY       XBYTE[0xFFA4]
#define  MTH       XBYTE[0xFFA5]
#define  YR        XBYTE[0xFFA6]
#define  WEEK      XBYTE[0xFFA7]
#define  RTCDATH   XBYTE[0xFFA8]
#define  RTCDATL   XBYTE[0xFFA9]
#define  RTCALM    XBYTE[0xFFAA]
#define  A0SEC     XBYTE[0xFFAB]
#define  A0MIN     XBYTE[0xFFAC]
#define  A0HR      XBYTE[0xFFAD]
#define  A0DAY     XBYTE[0xFFAE]
#define  A0DOW     XBYTE[0xFFAF]
//#define  A0MTH     XBYTE[0xFFAF]
#define  A1SEC     XBYTE[0xFFB0]
#define  A1YR      XBYTE[0xFFB1]
#define  A1HR      XBYTE[0xFFB2]
#define  RTCCON    XBYTE[0xFFB3]
#define  RTCWR     XBYTE[0xFFB4]
#define  RTCPSW    XBYTE[0xFFB5]
#define  RTCIE     XBYTE[0xFFB6]
#define  RTCIF     XBYTE[0xFFB7]
#define  RTCECL    XBYTE[0xFFB8]
#define  RTCECH    XBYTE[0xFFB9]
#define  RTCTMR    XBYTE[0xFFBA]
#define  RTCTMP    XBYTE[0xFFBB]

/* TEMP */
#define  TPCON       XBYTE[0xFFE8]
#define  TEMPH       XBYTE[0xFFE9]
#define  TEMPL       XBYTE[0xFFEA]
//#define  TEMPKH      XBYTE[0xFFEB]
//#define  TEMPKL      XBYTE[0xFFEC]
//#define  TEMPBH      XBYTE[0xFFED]
//#define  TEMPBL      XBYTE[0xFFEE]
#define  TPMOD0      XBYTE[0xFFE0]
#define  TPMOD1      XBYTE[0xFFE1]

#endif


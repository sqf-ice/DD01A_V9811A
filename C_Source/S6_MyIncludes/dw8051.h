#ifndef _DW8051_H_
#define _DW8051_H_

/* STANDARD 8052 */
#include "reg52.h"

/* NEWADD FOR ZEBOR */
/* DW8051 NEW_REG */
sfr	CKCON = 0x8E;	
sfr EICON = 0xd8;

sfr EIE = 0xE8;
sfr EXIF = 0x91;
sfr EIP	= 0xf8;

sfr SCON1 = 0xC0;
sfr SBUF1 = 0xC1;

sfr DPL1   = 0x84;
sfr DPH1   = 0x85;
sfr isfr   = 0xf1;
sfr debug  = 0xff;
sfr SPC_FNC  = 0x8F; //wrs



sbit F1    = PSW^1;

#endif

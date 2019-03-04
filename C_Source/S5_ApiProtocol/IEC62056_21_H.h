/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: Api_IEC62056_21_H.H
**Author: maji
**date: 
**description:  IEC62056-21 通讯规约协议程序应用层相关函数声明文件
**note: memer type  SD03A
**--------------------Version History -------------------------------------
** NO.	Date        		Ver    		By     			Description 
**==============================================================
** 1    2015-10-12  		0.1  		sosomj   		1. frist version                             
**
**==============================================================
*/

/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: IEC62056_21_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: IEC62056-21通讯协议相关函数程序H文件
**note			: 通用文件
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-20   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#ifndef _IEC62056_21_H
#define _IEC62056_21_H


/***************************************************************
*    1     立即数预定义   
****************************************************************/

///////////////////
// IEC62056-21 C   //
///////////////////
// iec 
#define	SOH		0x01
#define	STX		0x02
#define	ETX		0x03
#define	EOT		0x04
#define	ENQ		0x05
#define	ACKCOM	0x06
#define	BEL		0x07
#define	BS		0x08
#define	TAB		0x09
#define	LF		0x0A
#define	VT		0x0B
#define	FF		0x0C
#define	CR		0x0D
#define	SO		0x0E
#define	SI		0x0F
#define	DLE		0x10
#define	DC1		0x11
#define	DC2		0x12
#define	DC3		0x13
#define	DC4		0x14
#define	NAK		0x15
#define	SYN		0x16
#define	ETB		0x17
#define	CAN		0x18
#define	EM		0x19
#define	SUB		0x1A
#define	ESC		0x1B
#define	FS		0x1C
#define	GS		0x1D
#define	RS		0x1E
#define	US		0x1F

//C Mode BaudRate
#define   CBaud_3      '0'
#define	CBaud_6      '1'
#define	CBaud_12	'2'
#define	CBaud_24 	'3'
#define	CBaud_48	'4'
#define	CBaud_96 	'5'
#define	CBaud_192	'6'

/***************************************************************
*    2     数据结构声明     
****************************************************************/

/***************************************************************
*    3    变量声明       
****************************************************************/



/***************************************************************
*    4     函数声明        
****************************************************************/
extern INT8U IEC_Comm_Request_Cmd_Judge(INT8U* sptr,INT8U len,INT8U* id_ptr,INT8U meter_id_lenth);
extern INT8U IEC_Comm_Acknowledgement_Cmd_Judge(INT8U* sptr,INT8U len);
extern INT8U IEC_Comm_Command_Cmd_Judge(INT8U* sptr,INT8U len);
extern void Get_IEC_Comm_Request_Cmd_Answer(INT8U* sptr,INT8U len,INT8U* info_ptr);
extern void Get_IEC_Comm_Operand_Answer(INT8U* sptr,INT8U len,INT8U* info_ptr);
extern void Get_Error_Answer(INT8U* sptr);
extern void Get_ACK_Answer(INT8U* sptr);
extern INT8U Get_Bcc_Chk(INT8U* yourBuf, INT8U StartID, INT8U EndID);

/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/




/***************************************************************
*    END
****************************************************************/

#endif

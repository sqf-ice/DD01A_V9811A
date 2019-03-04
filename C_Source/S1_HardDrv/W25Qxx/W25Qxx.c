/************************************************************************
* 版权所有 (C)2016, 杭州西力智能科技股份有限公司。
*
* 文件名称： // W25QXX.c
* 文件标识： // 
* 内容摘要： //华邦Flash驱动程序
* 其它说明： // 
* 当前版本： // V01.00
* 作 者：    // yangxing
* 完成日期： // 2010年7月06日
*
* 修改记录1：//
* 修改日期： //
* 版 本 号： // V01
* 修 改 人： //
* 修改内容： //
************************************************************************/
#include <MyIncludes_H.h>

/************************************************************************
* 函数名 ：//Delay_W25Xxx
* 功能 ：//延时子程序
* 输入参数 ：//n：延时时间
* 输出参数 ：//无
* 返回值说明 ：//无
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void Delay_W25Xxx( unsigned char i )
{  
    for(i=0;i<3;i++)	
        nop();
}

/************************************************************************
* 函数名 ：//W25Xxx_Init
* 功能 ：//W25Xxx管脚初始化
* 输入参数 ：//无
* 输出参数 ：//无
* 返回值说明 ：//无
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void W25Xxx_Init(void)
{
	/*
    P_FLASH_CS_H;
	P_FLASH_WP_H;
    //P_FLASH_HOLD_H;
    P_FLASH_SCL_H;
    P_FLASH_SDO_H;
    P_FLASH_CS_DIROUT();
    P_FLASH_WP_DIROUT();
    //P_FLASH_HOLD_DIROUT();
    P_FLASH_SCL_DIROUT();
    P_FLASH_SDO_DIROUT();
    P_FLASH_SDI_DIRIN();
	*/
	
	P_FLASH_SCL_DIROUT();
    P_FLASH_SDO_DIROUT();
    P_FLASH_SDI_DIRIN();
	P_FLASH_CS_DIROUT();
	
	P_FLASH_CS_H;
    P_FLASH_SCL_H;
    P_FLASH_SDO_H;
}

/************************************************************************
* 函数名 ：//W25Xxx_SendByte
* 功能 ：//发送一个字节数据或地址
* 输入参数 ：//TxData:待发送的信息
* 输出参数 ：//无
* 返回值说明 ：//无
* 其他说明 ：//在CLK上升沿时，发送SD0信号
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void W25Xxx_SendByte(unsigned char TxData)
{
    unsigned char i;

    for( i = 0; i < 8; i++ )
    {
        //Delay_W25Xxx(3);
        P_FLASH_SCL_L;// CLK=0
        if( TxData & 0x80 )
            P_FLASH_SDO_H;// SD0=1
        else
            P_FLASH_SDO_L;// SD0 = 0
        //Delay_W25Xxx(3);
        P_FLASH_SCL_H;// CLK = 1
        TxData <<= 1;
    }
}

/************************************************************************
* 函数名 ：//W25Xxx_ReceiveByte
* 功能 ：//接收一个字节数据
* 输入参数 ：//无
* 输出参数 ：//MX25LXX 读取数据
* 返回值说明 ：//无
* 其他说明 ：//在CLK下降沿时，读取SDI信号
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
unsigned char W25Xxx_ReceiveByte(void)
{
    unsigned char i,RxData=0;

    //P_FLASH_SCL_H;//SCL=1
    for( i = 0; i < 8; i++ )
    {
        P_FLASH_SCL_L;//SCL=0
        Delay_W25Xxx(3);
        P_FLASH_SCL_H;//SCL=1
        //Delay_W25Xxx(3);
        RxData <<= 1;
		P_FLASH_SDI_DIRIN();
        if( P_FLASH_SDI_Read )
            RxData |= 0x01;
    }
    return(RxData);
}

/************************************************************************
* 函数名 ：//W25X_WriteEnable
* 功能 ：//允许写
* 输入参数 ：//无
* 输出参数 ：//
* 返回值说明 ：//无
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void W25X_WriteEnable(void)
{
    if( W25X_WaitForWriteEnd() == 0 )
    {
        P_FLASH_CS_L;
        W25Xxx_SendByte(W25X_WriteEn);
        P_FLASH_CS_H;
    }
}

/************************************************************************
* 函数名 ：//W25X_WriteDisable
* 功能 ：//禁止写
* 输入参数 ：//无
* 输出参数 ：//
* 返回值说明 ：//无
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void W25X_WriteDisable(void)
{
    P_FLASH_CS_L;

    W25Xxx_SendByte(W25X_WriteDis);

    P_FLASH_CS_H;
}

/************************************************************************
* 函数名 ：//W25X_WaitForWriteEnd
* 功能 ：//flash 空闲检测
* 输入参数 ：//无
* 输出参数 ：//
* 返回值说明 ：//0--正常 1---错误
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
unsigned char W25X_WaitForWriteEnd(void)
{
    unsigned char FlashSta =0x01;
    unsigned long i=0;
    
    P_FLASH_CS_L;
    W25Xxx_SendByte(W25X_ReadStatus);//读状态寄存器
    while(FlashSta & 0x01)
    {
        CLRWDT();;//清看门狗
        FlashSta = W25Xxx_ReceiveByte();
        i++;
        if( i > 120000 )//最长等待700ms，否则报FLASH故障
           break;
        if(( i%5000 )==0)
            W25Xxx_SendByte(W25X_ReadStatus);//读状态寄存器
    }
    P_FLASH_CS_H;
    if( i<120000 ) 
    {
        return 0;			
    }	
    else
    {
       // Flag.Error |= F_FLASH_Err;
        return 1;
    }	
}

/************************************************************************
* 函数名 ：//W25X_ChkWriteEn
* 功能 ：//写使能开关检测
* 输入参数 ：//无
* 输出参数 ：//
* 返回值说明 ：//0--正常 1---错误
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
unsigned char W25X_ChkWriteEn(void)
{    
    unsigned char FlashSta = 0x06;	
    unsigned long i=0;
    P_FLASH_CS_L;

    W25Xxx_SendByte(W25X_ReadStatus);//读状态寄存器
    while( (FlashSta & 0x0f)!=0x02 )
    {
        CLRWDT();//清看门狗
//		W25Xxx_SendByte(W25X_ReadStatus);//读状态寄存器
        FlashSta = W25Xxx_ReceiveByte();
        i++;
        if( i > 120000 )//最长等待700ms，否则报FLASH故障
           break;
        if(( i%5000 )==0)
            W25Xxx_SendByte(W25X_ReadStatus);//读状态寄存器
    }
    P_FLASH_CS_H;
    
    if( i<120000 ) 
    {
        return 0;			
    }	
    else
    {
        //Flag.Error |= F_FLASH_Err;
        return 1;
    }	
}
/**********************************************************************
* 函数名 ：//W25X_StatusRead
* 功能 ：//读状态寄存器		CMD=0x05
* 输入参数 ：//无
* 输出参数 ：//
* 返回值说明 ：//返回状态字
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**********************************************************************/
unsigned char W25X_StatusRead(void)
{
    unsigned char i;
    unsigned char Temp=0,Temp1=0;

    P_FLASH_CS_L;
    W25Xxx_SendByte( W25X_ReadStatus );	
    for( i=0;i<10;i++ )
    {
        Temp = W25Xxx_ReceiveByte();										
        Temp1 = W25Xxx_ReceiveByte();
        if( Temp == Temp1 ) 
            break;										
    }	
    P_FLASH_CS_H;
    return Temp;
}
/**********************************************************************
* 函数名 ：//W25X_StatusWrite
* 功能 ：//写状态寄存器		CMD=0x01
* 输入参数 ：//无
* 输出参数 ：//
* 返回值说明 ：//
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**********************************************************************/
void W25X_StatusWrite(void)
{
    W25X_WriteEnable();
    if( W25X_WaitForWriteEnd() == 0 ) 
    {
        P_FLASH_CS_L;
        W25Xxx_SendByte( W25X_WriteStatus );	
        W25Xxx_SendByte( 0x00 );	
        P_FLASH_CS_H;
    } 
}
/************************************************************************
* 函数名 ：//W25X_SectorErase_4KByte
* 功能 ：//擦除4K的扇区
* 输入参数 ：//无
* 输出参数 ：//Addr:扇区起始地址
* 返回值说明 ：//无
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void W25X_SectorErase_4KByte(unsigned long Addr)
{
    unsigned char FlashCheck = 0x00;
    CLRWDT();//清看门狗

    W25Xxx_Init();  

    FlashCheck = W25X_StatusRead();
    if(( FlashCheck & 0xFC ) != 0 ) 
        W25X_StatusWrite();
    
    W25X_WriteEnable();//允许写
    
    if( W25X_ChkWriteEn() == 0 )
    {
        P_FLASH_CS_L;
        W25Xxx_SendByte(W25X_S_Erase);
        W25Xxx_SendByte( (Addr & 0xFF0000) >> 16 );
        W25Xxx_SendByte( (Addr & 0xF000) >> 8 );//0XFF00 因为扇区起始地址低16位都是000，所以这里用0xF000
        W25Xxx_SendByte( Addr & 0x00 );//0XFF 因为扇区起始地址低16位都是000，所以这里用0x00
        P_FLASH_CS_H;
        W25X_WaitForWriteEnd();//目的是保证写结束
    }
}

/************************************************************************
* 函数名 ：//W25Xxx_HighSpeedRead
* 功能 ：//高速读
* 输入参数 ：//无
* 输出参数 ：//Dest:目的地址 Addr：源地址 Lenth：数据长度
* 返回值说明 ：//无
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void W25Xxx_HighSpeedRead( unsigned char* Dest, unsigned long Addr, unsigned short Lenth )
{
    unsigned short i;

    CLRWDT();//清看门狗

    W25Xxx_Init();

    if(W25X_WaitForWriteEnd()==0)
    {
        P_FLASH_CS_L;
        W25Xxx_SendByte( W25X_ReadDATA8 );//普通读
        //W25Xxx_SendByte( W25X_FastRead );//快速读
        W25Xxx_SendByte( (Addr & 0xFF0000) >> 16 );
        W25Xxx_SendByte( (Addr& 0xFF00) >> 8 );
        W25Xxx_SendByte( Addr & 0xFF );
        //W25Xxx_SendByte( Dummy_Byte );
        for( i = 0; i < Lenth; i++)
        {
            *Dest = W25Xxx_ReceiveByte();
            Dest++;
        }
        P_FLASH_CS_H;
    }
}

/************************************************************************
* 函数名 ：//W25Xxx_Write
* 功能 ：//连续写
* 输入参数 ：//Addr：Flash地址 Src:数据地址 Len：数据长度
* 输出参数 ：//无
* 返回值说明 ：//无
* 其他说明 ：//无
* 修改日期 :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void W25Xxx_Write( unsigned long Addr, unsigned char* Src, unsigned short Lenth )
{
    unsigned short i;
    unsigned Len = 0;
    unsigned char FlashCheck = 0x00;
    CLRWDT();//清看门狗

    W25Xxx_Init();

    FlashCheck = W25X_StatusRead();
    if(( FlashCheck & 0xFC ) != 0 ) 
        W25X_StatusWrite();
    
    while( Lenth > 0 )
    {
        if( Addr & 0x000000FF )
        {
            Len = 256 - (Addr & 0x000000FF);
            if( Len > Lenth )
                Len = Lenth;
        }
        else
        {
            if( Lenth > 256)
                Len = 256;
            else
                Len = Lenth;
        }
        W25X_WriteEnable();//允许写
        if( W25X_ChkWriteEn() == 0 )
        {
            P_FLASH_CS_L;
            W25Xxx_SendByte( W25X_Write );
            W25Xxx_SendByte( (Addr & 0xFF0000) >> 16 );
            W25Xxx_SendByte( (Addr & 0xFF00) >> 8 );
            W25Xxx_SendByte( Addr & 0xFF );
            for( i = 0; i < Len; i++ )
            {
                W25Xxx_SendByte( *(Src+i) );
            }
            P_FLASH_CS_H;
        } 
        Addr += Len;
        Src += Len;
        Lenth -= Len;
        W25X_WaitForWriteEnd();
    }
}


INT8U SPI_FLASH_BUF[1024];
void SPI_Flash_Write(INT8U *pbuf,INT32U addr, INT16U len)
{
		INT32U secpos;
		INT16U secoff;
		INT16U secremain;
		INT16U i,j;
		INT16U seclen;
//		INT16U isErase = 0;

		secpos = addr/4096;			//在第几个扇区
		secoff = addr%4096;			//在当前扇区的偏移
		secremain = 4096-secoff;	//当前扇区的剩余空间
		
		if(len <= secremain) //写入长度小于当前扇区的剩余空间，不用写入下一扇区
			secremain = len;
		while(1)
		{

			for(i=0;i<4;i++)
			{
				W25Xxx_HighSpeedRead(SPI_FLASH_BUF,(secpos*4096+1024*i),1024);
				mem_write( ADR_BLOCK180_LOAD_START_E2P+1024*i,SPI_FLASH_BUF,1024,MEM_E2P1);
			}
			W25X_SectorErase_4KByte(secpos*4096);

			if(len > secremain)
			{
				seclen = secremain;	
			}
			else
			{
				seclen = len;
			}
			for(i = 0; i < 4;i++)
			{
				mem_read( SPI_FLASH_BUF,ADR_BLOCK180_LOAD_START_E2P+1024*i,1024,MEM_E2P1);
				if(i < secoff/1024)
				{
					W25Xxx_Write(4096*secpos+1024*i,SPI_FLASH_BUF,1024);
				}
				if(i == secoff/1024)
				{
					for(j=0;j<(1024-secoff%1024);j++)
					{
						if(seclen > 0)
						{
							SPI_FLASH_BUF[secoff%1024+j] = pbuf[j];
							seclen--;
						}
					}
					W25Xxx_Write(4096*secpos+1024*i,SPI_FLASH_BUF,1024);
				}
				if(i > secoff/1024)
				{
					for(j=0;j<1024;j++)
					{
						if(seclen > 0)
						{
							SPI_FLASH_BUF[j] = pbuf[j+1024*(i-secoff/1024)];
							seclen--;
						}
						
					}
					W25Xxx_Write(4096*secpos+1024*i,SPI_FLASH_BUF,1024);
				}
				
			}
			if(len <= secremain)//字节不大于4096表示
				break;
			else
			{
				secpos++;
				if(secpos == 16)
				{
					secpos = 0;
				}
				secoff=0;
				pbuf+=secremain;
				addr += secremain;
				len -= secremain;

				if(len > 4096)
					secremain = 4096;
				else
					secremain = len;
			}
		}
}

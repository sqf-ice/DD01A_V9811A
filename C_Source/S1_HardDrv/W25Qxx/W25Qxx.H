/************************************************************************
* 版权所有 (C)2016, 杭州西力智能科技股份有限公司。
*
* 文件名称： // W25Xxx.H
* 文件标识： // W25Xxx
* 内容摘要： // 
* 其它说明： //
* 当前版本： // V01.00
* 作 者： // yangxing
* 完成日期： // 2016年08月06日
*
* 修改记录1：//
* 修改日期：
* 版 本 号：
* 修 改 人：
* 修改内容：
************************************************************************/

#ifdef	W25Xxx_H
#define	W25Xxx_EXT
#else
#define W25Xxx_EXT extern
#endif

//读写SPI口模拟模式3,时钟平时为高电平

//针对管脚的相关定义
#define P_FLASH_SDI_DIRIN() do{P9IE |= BIT0;P9OE |= BIT0;P9FC &= ~BIT0;}while(0)//使能SDI输入     P9.0
#define P_FLASH_SCL_DIROUT() do{P2OE &= ~BIT5;P25FS = 0;}while(0)//使能SCL输出    P2.5
#define P_FLASH_SDO_DIROUT() do{P9OE &= ~BIT2;P9FC &= ~BIT2;}while(0)//使能SDO输出     P9.2
#define P_FLASH_CS_DIROUT() do{P9OE &= ~BIT1;P9FC &= ~BIT1;}while(0)//使能CS输出       P9.1

#define P_FLASH_CS_H do{P9IE&= ~BIT1;P9OE&= ~BIT1;P9DO|=BIT1;  P9FC &=~BIT1;}while(0)//F_CS
#define P_FLASH_CS_L do{P9IE&= ~BIT1;P9OE&= ~BIT1;P9DO&= ~BIT1; P9FC &=~BIT1; }while(0)


#define P_FLASH_SCL_H do{P2IE&= ~BIT5;P2OE&= ~BIT5;P2OD|=BIT5; P25FS =0;}while(0)//CLK
#define P_FLASH_SCL_L do{P2IE&= ~BIT5;P2OE&= ~BIT5;P2OD&= ~BIT5;P25FS =0; }while(0)//CLK

#define P_FLASH_SDO_H do{P9IE&= ~BIT2;P9OE&= ~BIT2;P9DO|=BIT2;  P9FC &=~BIT2;}while(0)//DO
#define P_FLASH_SDO_L do{P9IE&= ~BIT2;P9OE&= ~BIT2;P9DO&= ~BIT2; P9FC &=~BIT2; }while(0)//DO

#define P_FLASH_SDI_Read (P9DI & BIT0)//DI


//Flash 操作指令
#define W25X_ReadStatus       0x05		//读状态寄存器
#define W25X_WriteStatus      0x01		//写状态寄存器
#define W25X_ReadDATA8        0x03		//普读_数据
#define W25X_FastRead         0x0B		//快读_数据
#define W25X_Write            0x02		//写_数据_0~255个字节
#define W25X_S_Erase          0x20		//扇区擦除4KB
#define W25X_B_Erase          0xD8		//块区擦除64KB
#define W25X_C_Erase          0xC7		//整片格式化
#define W25X_PowerDown        0xB9		//待机
#define W25X_PowerON_ID       0xAB		//开机或是读ID
#define W25X_JEDEC_ID         0x9F		//十六位的JEDEC_ID
#define W25X_WriteEn          0x06		//写充许
#define W25X_WriteDis         0x04		//写禁止

//自定义
#define W25X_BUSY             0x01		//FLASH忙
#define W25X_NotBUSY          0x00		//FLASH闲

extern void W25Xxx_Init(void);
extern void W25X_WriteEnable (void);
extern void W25X_WriteDisable(void);
extern void W25X_StatusWrite(void);
extern unsigned char W25X_ChkWriteEn(void);
extern unsigned char W25X_WaitForWriteEnd(void);
extern void W25X_SectorErase_4KByte(unsigned long Addr);
extern void W25Xxx_HighSpeedRead( unsigned char* Dest, unsigned long Addr, unsigned short Lenth );
extern void W25Xxx_Write( unsigned long Addr, unsigned char* Src, unsigned short Lenth );
extern void SPI_Flash_Write(INT8U *pbuf,INT32U addr, INT16U len);







/************************************************************************
* ��Ȩ���� (C)2016, �����������ܿƼ��ɷ����޹�˾��
*
* �ļ����ƣ� // W25Xxx.H
* �ļ���ʶ�� // W25Xxx
* ����ժҪ�� // 
* ����˵���� //
* ��ǰ�汾�� // V01.00
* �� �ߣ� // yangxing
* ������ڣ� // 2016��08��06��
*
* �޸ļ�¼1��//
* �޸����ڣ�
* �� �� �ţ�
* �� �� �ˣ�
* �޸����ݣ�
************************************************************************/

#ifdef	W25Xxx_H
#define	W25Xxx_EXT
#else
#define W25Xxx_EXT extern
#endif

//��дSPI��ģ��ģʽ3,ʱ��ƽʱΪ�ߵ�ƽ

//��Թܽŵ���ض���
#define P_FLASH_SDI_DIRIN() do{P9IE |= BIT0;P9OE |= BIT0;P9FC &= ~BIT0;}while(0)//ʹ��SDI����     P9.0
#define P_FLASH_SCL_DIROUT() do{P2OE &= ~BIT5;P25FS = 0;}while(0)//ʹ��SCL���    P2.5
#define P_FLASH_SDO_DIROUT() do{P9OE &= ~BIT2;P9FC &= ~BIT2;}while(0)//ʹ��SDO���     P9.2
#define P_FLASH_CS_DIROUT() do{P9OE &= ~BIT1;P9FC &= ~BIT1;}while(0)//ʹ��CS���       P9.1

#define P_FLASH_CS_H do{P9IE&= ~BIT1;P9OE&= ~BIT1;P9DO|=BIT1;  P9FC &=~BIT1;}while(0)//F_CS
#define P_FLASH_CS_L do{P9IE&= ~BIT1;P9OE&= ~BIT1;P9DO&= ~BIT1; P9FC &=~BIT1; }while(0)


#define P_FLASH_SCL_H do{P2IE&= ~BIT5;P2OE&= ~BIT5;P2OD|=BIT5; P25FS =0;}while(0)//CLK
#define P_FLASH_SCL_L do{P2IE&= ~BIT5;P2OE&= ~BIT5;P2OD&= ~BIT5;P25FS =0; }while(0)//CLK

#define P_FLASH_SDO_H do{P9IE&= ~BIT2;P9OE&= ~BIT2;P9DO|=BIT2;  P9FC &=~BIT2;}while(0)//DO
#define P_FLASH_SDO_L do{P9IE&= ~BIT2;P9OE&= ~BIT2;P9DO&= ~BIT2; P9FC &=~BIT2; }while(0)//DO

#define P_FLASH_SDI_Read (P9DI & BIT0)//DI


//Flash ����ָ��
#define W25X_ReadStatus       0x05		//��״̬�Ĵ���
#define W25X_WriteStatus      0x01		//д״̬�Ĵ���
#define W25X_ReadDATA8        0x03		//�ն�_����
#define W25X_FastRead         0x0B		//���_����
#define W25X_Write            0x02		//д_����_0~255���ֽ�
#define W25X_S_Erase          0x20		//��������4KB
#define W25X_B_Erase          0xD8		//��������64KB
#define W25X_C_Erase          0xC7		//��Ƭ��ʽ��
#define W25X_PowerDown        0xB9		//����
#define W25X_PowerON_ID       0xAB		//�������Ƕ�ID
#define W25X_JEDEC_ID         0x9F		//ʮ��λ��JEDEC_ID
#define W25X_WriteEn          0x06		//д����
#define W25X_WriteDis         0x04		//д��ֹ

//�Զ���
#define W25X_BUSY             0x01		//FLASHæ
#define W25X_NotBUSY          0x00		//FLASH��

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






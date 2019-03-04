#define  RACCOONEXT
#include <MyIncludes_H.h>


/*=========================================================================================\n
* @function_name: Init_RacCtrl
* @function_file: ComRaccoon.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   (2013-04-15)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Init_RacCtrl(void)
{
    uint8 i;
    gs_RacCtrl.ucSendLen=0;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=0;
    gs_RacCtrl.ucRevPoint=0;
    gs_RacCtrl.ucStatus=Rac_Idle;
    for(i=0;i<100;i++)
    {
        gs_RacCtrl.ucBuf[i] =0;
    }
}
/*=========================================================================================\n
* @function_name: RxReset_V9281
* @function_file: Raccoon.c
* @描述: 低电平有效 P2.5输出低电平 持续63ms
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RxReset_V9260(void)
{
    P21FS=0; 
    P20FS=0; 
    P2OE&=(~BIT1);       //TXD 允许输出
    P2IE&=(~BIT1);       
    
    P2OD |=(BIT1);

    P2OD&=(~BIT1);  
	// 去除中断 ，使用延时//
	//guc_CommDelayTime = 10;
	// while(guc_CommDelayTime >0);  //100ms
	CLRWDT(); 	
	Lib_Delay_ms(100);
	CLRWDT(); 	
     P2OD |=(BIT1);

    P21FS=2;  
    P20FS=2; 
}
/*=========================================================================================\n
* @function_name: WriteRaccoon
* @function_file: Raccoon.h
* @描述: 向raccoon发送数据
* 
* 
* @参数: 
* @param:Data  写入数据
*               字节1：帧头0xFE    字节2：高4位地址高4bit，低4位控制
*               字节3：地址低8bit  字节4~7：数据   字节8：校验
* @param:addr  写入地址
* 控制
* @返回: 
* @return: uint8 
* @作者:   lwb (2013-07-04)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  hjj
* @修改内容: 添加Raccoon物理地址，三相表一共使用3个Raccoon
* @param:Phyadd Raccoon地址

===========================================================================================*/
uint8  WriteRaccoon(uint32 Data,uint16 Addr,uint8 Phyadd)
{
    uint8 i;
    volatile uint8 ucSum;
    switch(Phyadd)
    {
    case APhy:
      {
        return SetMeterCfgACK( Data, Addr);
      }
      break;
    case BPhy:
      {   
        gs_RacCtrl.ucBuf[0] = 0xfe;
        
        gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+RacWrite+ BPhy;
        gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
        
        gs_RacCtrl.ucBuf[3] = (uint8)(Data&0x000000ff);
        gs_RacCtrl.ucBuf[4] = (uint8)((Data&0x0000ff00)>>8);
        gs_RacCtrl.ucBuf[5] = (uint8)((Data&0x00ff0000)>>16);
        gs_RacCtrl.ucBuf[6] = (uint8)((Data&0xff000000)>>24);
        //计算校验和
        gs_RacCtrl.ucBuf[7] =  gs_RacCtrl.ucBuf[0];
        for(i=1;i<7;i++)
        {
          gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
        }
        
        gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
        gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;
        
        
        gs_RacCtrl.ucStatus=Rac_Send;       //进入发送状态
        gs_RacCtrl.ucSendLen=8;
        gs_RacCtrl.ucSendPoint=0;
        gs_RacCtrl.ucRevLen=4;
        gs_RacCtrl.ucRevPoint=0;
        //gs_RacPhy.uc_Uartstat |= flgEtwrite;
         	 Uart4_Transmit();
        guc_CommDelayTime=9;
        while(gs_RacCtrl.ucStatus!=Rac_WaitPro)
        {
          CLRWDT(); 
          if(guc_CommDelayTime==0)
          {
            //gs_RacPhy.uc_Uartstat &= ~flgEtwrite;
            return FALSE;                   //如果超时 
          }
        }  
        //gs_RacPhy.uc_Uartstat &= ~flgEtwrite;
        ucSum=0;
        for(i=0;i<3;i++)
        {
          ucSum += gs_RacCtrl.ucBuf[i]; 
        }
        
        ucSum=~ucSum;
        ucSum+=0x33;
        Lib_Delay_ms(5);
        if(gs_RacCtrl.ucBuf[3]==ucSum)
        {
          return TRUE;
        }else
        {
          return FALSE;
        }
      }
      break;
    default:
      return FALSE;
      break;
    }
  //  return TRUE;
}

/*=========================================================================================\n
* @function_name: ReadRaccoon
* @function_file: Raccoon.h
* @描述: 向raccoon读取数据
* 字节0：帧头0xFE    字节1：高4位地址高4bit，低4位控制
* 字节2：地址低8bit  字节3：word个数 字节4~6：无意义 字节7：校验
* @参数: 
* @param:Addr  读取起始地址
* @param:num  读取word个数，32bit
* 
* @返回: 
*        
* @return: uint8 
* @作者:   lwb (2013-07-04)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 添加Raccoon物理地址，三相表一共使用3个Raccoon
* @param:Phyadd Raccoon地址

==========================================================================================*/
uint8 ReadRaccoon(uint16 Addr,uint8 num,uint8 Phyadd)
{
    uint8 i;
    volatile uint8 ucSum;
    ST_U32_U08 TempValue;  
	
    switch(Phyadd)
    {
    case APhy:               //读9811A
      {
        gs_RacCtrl.ucBuf[0] = 0xFE;      //与raccoon中接收的数据保持一致
        gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead+ APhy;
        gs_RacCtrl.ucBuf[2] = num;
        
        for( i=0; i<num; i++)               //读取不超过100个字节
        {
          TempValue.u32= ReadMeterParaACK( Addr+i);
          memcpy(gs_RacCtrl.ucBuf+3+4*i,(uint8*)TempValue.B08,4); 
        }
        ucSum=0;;
        for(i=0;i<num*4+3;i++)
        {
          ucSum += gs_RacCtrl.ucBuf[i]; 
        }
        ucSum = ~ucSum;
        gs_RacCtrl.ucBuf[num*4+3] = ucSum+0x33;
      }
      break;
      
    case BPhy:
      {
        gs_RacCtrl.ucBuf[0] = 0xFE;
        
        gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead+ BPhy;
        gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
        
        gs_RacCtrl.ucBuf[3] = num;                   //num：需要读取多少字的内容
        gs_RacCtrl.ucBuf[4] = 0;
        gs_RacCtrl.ucBuf[5] = 0;
        gs_RacCtrl.ucBuf[6] = 0;
        gs_RacCtrl.ucBuf[7] = 0;                    //sum 清零
        for(i=0;i<7;i++)
        {
          gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
        }
        
        gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
        gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;
        
        gs_RacCtrl.ucStatus=Rac_Send;       //进入发送状态


		
        gs_RacCtrl.ucSendLen=8;
        gs_RacCtrl.ucSendPoint=0;
        gs_RacCtrl.ucRevLen=(num*4)+4;           
        gs_RacCtrl.ucRevPoint=0;
        guc_CommDelayTime=17; 
        //gs_RacPhy.uc_Uartstat |= flgEtread;   
        Uart4_Transmit();           //开启发送
        while(gs_RacCtrl.ucStatus!=Rac_WaitPro)
        {

           CLRWDT(); 
          if(guc_CommDelayTime==0)
          {
            //gs_RacPhy.uc_Uartstat &= ~flgEtread;
            return FALSE;           //如果超时 
            
          }
        }
        //gs_RacPhy.uc_Uartstat &= ~flgEtread;
        ucSum=0;
        for(i=0;i<(num*4)+3;i++)               //读取不超过255个字节
        {
          ucSum += gs_RacCtrl.ucBuf[i]; 
        }
        
        ucSum=~ucSum;
        ucSum+=0x33;

        if(gs_RacCtrl.ucBuf[num*4+3]==ucSum)
        {
          return TRUE;
        }else
        {
          return FALSE;
        } 
      }
    default:
      return FALSE;
      break;
    }
    return TRUE;
}

/*=========================================================================================\n
* @function_name: ReadRaccoonInt
* @function_file: Raccoon.h
* @描述: 向raccoon读取电量数据
* 字节0：帧头0xFE    字节1：高4位地址高4bit，低4位控制
* 字节2：地址低8bit  字节3：word个数 字节4~6：无意义 字节7：校验
* @参数: 
* @param:Addr  读取起始地址
* @param:num  读取word个数，32bit
* 
* @返回: 
*        
* @return: uint8 
* @作者:  
* @备注: 
*-------------------------------------------------------------------------------------------

==========================================================================================*/
void ReadRaccoonInt(uint16 Addr,uint8 num,uint8 Phyadd)
{
    uint8 i;
    volatile uint8 ucSum;
 //   ST_U32_U08 TempValue;
    switch(Phyadd)
    {
    case BPhy:
      {     
        gs_RacCtrl.ucBuf[0] = 0xfe;   
        gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead+ BPhy;
        gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
        
        gs_RacCtrl.ucBuf[3] = num;                   //num：需要读取多少字的内容
        gs_RacCtrl.ucBuf[4] = 0;
        gs_RacCtrl.ucBuf[5] = 0;
        gs_RacCtrl.ucBuf[6] = 0;
        gs_RacCtrl.ucBuf[7] = 0;                    //sum 清零
        for(i=0;i<7;i++)
        {
          gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
        }
        
        gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
        gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;
        
        gs_RacCtrl.ucStatus=Rac_Send;       //进入发送状态
        gs_RacCtrl.ucSendLen=8;
        gs_RacCtrl.ucSendPoint=0;
        gs_RacCtrl.ucRevLen=(num*4)+4;           
        gs_RacCtrl.ucRevPoint=0;
        gs_RacPhy.uc_Physendtim= guc_Dy9260FOver; //端口超时保护
        gs_RacPhy.uc_Phystat = BPhy;
        gs_RacPhy.uc_Sendstat = TRUE;


    // 开启发送  200ms 接收延时//
        guc_CommDelayTime=0x20;     
         Uart4_Transmit();           //开启发送
        while(gs_RacCtrl.ucStatus!=Rac_WaitPro)   // 等待中断接收完成//
        {
          CLRWDT(); 
          if(guc_CommDelayTime==0)
          {
            goto ENDERR;
          }
        }

        ucSum=0;
        for(i=0;i<(num*4)+3;i++)               //读取不超过255个字节
        {
          ucSum += gs_RacCtrl.ucBuf[i]; 
        }
        
        ucSum=~ucSum;
        ucSum+=0x33;

        if(gs_RacCtrl.ucBuf[num*4+3]==ucSum)
        {
            gs_sys_run.font_fg |= BIT3_FONT_FG_readpower; // 读取后检验完成标识//
 
        }else
        {
ENDERR:    gs_RacPhy.uc_Sendstat = FALSE;
             gs_RacPhy.uc_Physendtim = guc_Dy9260FOver;  
             gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;  // 参数刷新
            // return FALSE;     
        } 
      }
      break;
    default:
      break;
    }

	 
}


uint8 I_Dirct = 0,Aphy_Dly = 0;    
/*=========================================================================================\n
* @function_name: ReadbackABCPhy
* @function_file: 功率、电压电流原始值读取//
* @描述: 
* 
* 
* @参数: 
* @param:Phyadd  
* @返回: 
* @作者:   zwk(2013-09-25)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void ReadbackABCPhy(void)
{
    volatile uint8 ucSum;
	uint8 i;
    ST_U32_U08 TempValue,TempValue1,TempValue2,TempValue3,TempValue4;//TempValue5;
    TempValue.u32 = 0;
    TempValue1.u32 = 0;
    TempValue2.u32 = 0;
    TempValue3.u32 = 0;
    TempValue4.u32 = 1;        

    ucSum=0;
    gs_RacPhy.uc_Physendtim = 0;
    for(i=0;i<(5*4)+3;i++)               //读取不超过255个字节
    {
        ucSum += gs_RacCtrl.ucBuf[i]; 
    }

    ucSum=~ucSum;
    ucSum+=0x33;

    if(gs_RacCtrl.ucBuf[5*4+3]==ucSum)
    {
      if((gs_RacCtrl.ucBuf[1]&0x03) != RacRead)  // 验证为读返回//
      {
        return;
      }
      else
      {
            memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
            Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// 低字节在前  倒置//
            if(TempValue.B08[3]>0x7f)          //如果是负数的数，先变成正数
            {
                TempValue.u32=~TempValue.u32+1;
            }
            memcpy((uint8*)TempValue1.B08,gs_RacCtrl.ucBuf+7,4);//无功
            Lib_Rev_Copy_Arry(&TempValue1.B08[0],4);// 低字节在前  倒置//
            if(TempValue1.B08[3]>0x7f)          //如果是负数的数，先变成正数
            {
                TempValue1.u32=~(TempValue1.u32) +1;
            }
            memcpy((uint8*)TempValue2.B08,gs_RacCtrl.ucBuf+11,4);//电压
            Lib_Rev_Copy_Arry(&TempValue2.B08[0],4);// 低字节在前  倒置//
            memcpy((uint8*)TempValue3.B08,gs_RacCtrl.ucBuf+15,4);//电流
            Lib_Rev_Copy_Arry(&TempValue3.B08[0],4);// 低字节在前  倒置//
            memcpy((uint8*)TempValue4.B08,gs_RacCtrl.ucBuf+19,4);//频率
            Lib_Rev_Copy_Arry(&TempValue4.B08[0],4);// 低字节在前  倒置//
       }
    }
    else
    {        
      
      TempValue.u32= 0;
      TempValue1.u32 = 0;
      TempValue2.u32 = 0;
      TempValue3.u32 = 0;
      TempValue4.u32 = 1;        
    }
         
         gs_RacPhy.uc_Powstat &= ~power_b;  //有电标志 
         gs_measure_var_data.gs_really[WIRE_L2].dw_p_val.u32=  (TempValue.u32+(TempValue.u32/12)); //9811A有功是9260有功的1.086倍
	     gs_measure_var_data.gs_really[WIRE_L2].dw_u_val.u32=  TempValue2.u32;
         gs_measure_var_data.gs_really[WIRE_L2].dw_i_val.u32=   TempValue2.u32;
         gs_measure_var_data.gs_really[WIRE_L2].w_freq_val.u16 =  (uint16)(((uint32)23575*divf)/TempValue4.u32);
         if(( gs_measure_var_data.gs_really[WIRE_L2].w_freq_val.u16 /gs_JbPm.ul_PG) < ((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))/500 )
         {
            gs_measure_var_data.gs_really[WIRE_L2].dw_p_val.u32   = 0;
            guc_powerdirB = 0;
         }
}

/*=========================================================================================\n
* @function_name: CheckRegSum
* @function_file: Raccoon.c
* @描述: 检测校验和
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CheckRegSum(void)
{
    uint32 ucSum=0;

    ucSum = 
          + gs_JbPm.gs_JBB.Rac_WARTI 
          + gs_JbPm.gs_JBB.Rac_WARTU 
          + gs_JbPm.gs_JBB.Rac_WAPT
          + gs_JbPm.gs_JBB.Rac_WAPT //无功比差
          + gs_JbPm.gs_JBB.Rac_WWAPT
          + gs_JbPm.gs_JBB.Rac_WWAQT
          + gs_JbPm.Rac_BPFPARA
          + gs_JbPm.Rac_ANCtrl0
          + gs_JbPm.Rac_ANCtrl1
          + gs_JbPm.Rac_ANCtrl2
          + gs_JbPm.Rac_MTPARA0
          + gs_JbPm.gs_JBB.Rac_MTPARA1;   
    ucSum = 0xFFFFFFFF-ucSum;
    WriteRaccoon(ucSum,RegCKSUM,BPhy);

}

 
/*=========================================================================================\n
* @function_name: CheckSysErr
* @function_file: Raccoon.c
* @描述: RACCOON的系统自检校验  CHSUM
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CheckSysErr(void)
{
    WriteRaccoon( 0x09FB0004,RegSysCtrl,BPhy);
}
/*=========================================================================================\n
* @function_name: V9281_InitDefaultPara
* @function_file: Raccoon.c
* @描述: 初始化默认参数
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void V9260_InitDefaultPara(void)
{
    gs_JbPm.ui_MeterC=1200;             // 表常数
    gs_JbPm.ui_Un=12700;                // 标称电压
    gs_JbPm.ui_Ib=5000;                 // 标称电流
    
    gs_JbPm.Com_EGYTH     = 0x067215D8;    //有功门限值
    gs_JbPm.Com_REEGYTH   = 0x067215D8;    //无功门限值
    
    gs_JbPm.Rac_ANCtrl0 = 0xDf000440;    //0x185 增益I:32，U:1，M:4 温度系数，全局电流偏置
    gs_JbPm.Rac_ANCtrl1 = 0x30000000;    //0x186 
    gs_JbPm.Rac_ANCtrl2 = 0x0000000F;    //0x187     
    gs_JbPm.Rac_BPFPARA = 0x811D2BA7;    //0x0125 带通滤波器系数 0x811D2BA7
    gs_JbPm.Rac_MTPARA0 = 0xE0000000;    //0x183 CF工作时钟3.2MHz 使能CF输出，能量累加，起动潜动，能量来源：平均全波有功
    
    gs_JbPm.gs_JBA.Soc_SCP = 0x00000000;         //有功功率比差寄存器  
    gs_JbPm.gs_JBA.Soc_SCQ = 0x00000000;         //无功功率比差寄存器  
    gs_JbPm.gs_JBA.Soc_SCU = 0x00000000;         //电压有效值比差寄存器
    gs_JbPm.gs_JBA.Soc_SCI1 = 0x00000000;        //电流有效值比差寄存器
    gs_JbPm.gs_JBA.Soc_PARAPC = 0x00000000;      //有功功率二次补偿值
    gs_JbPm.gs_JBA.Soc_PARAQC = 0x00000000;      //无功功率二次补偿值
    gs_JbPm.gs_JBA.Soc_PHCCtrl1 = 0x00;          //Ia通道 角差寄存器
    
    gs_JbPm.gs_JBB.Rac_MTPARA1 = 0x0a0b0900;      //0x184 M通道关闭，仅进行配置自检，使能去直高通滤波器，使能（U/I）输入，角差   
    gs_JbPm.gs_JBB.Rac_WARTU = 0x00000000;        //全波电压有效值比差   
    gs_JbPm.gs_JBB.Rac_WARTI = 0x00000000;        //全波电流有效值比差
    gs_JbPm.gs_JBB.Rac_WAPT = 0x00000000;         //全波有功功率比差
    gs_JbPm.gs_JBB.Rac_WWAPT = 0x00000000;        //全波有功功率二次补偿值
    gs_JbPm.gs_JBB.Rac_WWAQT = 0x00000000;       //全波无功功率二次补偿值
    
    gs_JbPm.ul_PG=1;               //功率比例系数
    gs_JbPm.ul_URmG=1;             //电压通道比例系数
    gs_JbPm.ul_I1RmG=1;            //电流通道1比例系数
    
    gs_JbPm.Soc_AnaG=0x03;

   gs_JbPm.u16_csck = Lib_get_csck_int16u_num((uint8*)&gs_JbPm,sizeof(S_JBPM)-2,CHECKWORD);

//    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
}
/*=========================================================================================\n
* @function_name: UpdateRaccoonPara
* @function_file: Raccoon.c
* @描述: 
* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 UpdateRaccoonPara(void)
{
    uint8 i;
    uint32 *ul_pRacReg;
//    uint32 temp = 0;
    
    ul_pRacReg=(uint32*)&gs_JbPm.gs_JBA.Soc_SCP;   //A相
    for( i=0; i<sizeof(RegAddrSoc)/sizeof(uint16)-3; i++) //写RegAddrSoc中SCP,SCQ,SCU,SCI1到9811
    {
      CLRWDT(); 
      SetMeterCfgACK( ul_pRacReg[i],RegAddrSoc[i]);
    }
    //SetMeterCfgACK( 0,SCQ);
//    //1.28S定时那边已经加进去了，这边不需要处理了
//    temp = ReadMeterParaACK( PARAPC);//B相有功功率会加到二次补偿寄存器中，所以增量处理
//    temp += gs_JbPm.gs_JBA.Soc_PARAPC;
   SetMeterCfgACK(gs_JbPm.gs_JBA.Soc_PARAPC, PARAPC);
//    temp = ReadMeterParaACK( PARAQC);//B相无功功率会加到二次补偿寄存器中，所以增量处理
//    temp += gs_JbPm.gs_JBA.Soc_PARAQC;
//    SetMeterCfgACK(temp, PARAQC);
//    SetMeterCfgACK(0X002FFFFF,PARAPC);
    SetMeterCfgACK((uint32)gs_JbPm.gs_JBA.Soc_PHCCtrl1, PHCCtrl1); //Ia通道 角差寄存器
    
    CtrlADC0 = gs_JbPm.Soc_AnaG;                         //设置ADC增益
    if((gs_JbPm.Soc_AnaG&0x07)==0x07)                    //IA增益32倍的话，增加IA 偏置电流
    {
        CtrlADC4|=BIT4;             //增加偏置电流
    }else
    {
        CtrlADC4&=~BIT4;            //减少偏置电流
    }
     
     ul_pRacReg=(uint32*)&gs_JbPm.Rac_ANCtrl0;   //B相
 //    uint8 temp2;
 //   BE_ReadP(0x2A,&temp2,1);
  //  gs_JbPm.Rac_ANCtrl0 =((((uint16)temp2)<<8)&0xff00)|0x1D2000C2;    //0x0ff0     0f00 
    //gs_JbPm.Rac_ANCtrl0 = 0x1D2004C2;       
    //  0xC2,0x64,0x20,0x1D,//模拟控制寄存器0

    //默认定值
 //   gs_JbPm.Rac_ANCtrl0 = 0x1D2064C2;

    for(i=0;i<sizeof(RegAddrRac)/sizeof(uint16) ;i++) //写RegAddrRac中寄存器到9250
    {
      CLRWDT(); 
      if(i  < 5)   //ANCTRL0,ANCTRL1,ANCTRL2,BPFPARA,MTPARA0
      {
        WriteRaccoon(ul_pRacReg[i],RegAddrRac[i], BPhy);  
        CLRWDT();  
      }
      if(i == 5)
      {
        ul_pRacReg = (uint32*)&gs_JbPm.gs_JBB.Rac_MTPARA1;
      }
      if(i >= 5)
      {
        WriteRaccoon(ul_pRacReg[i-5],RegAddrRac[i], BPhy);//MTPARA1,WARTU,WARTI,WAPT,WWAPT,WWAQT  
      }
    }
    WriteRaccoon(gs_JbPm.gs_JBB.Rac_WAPT,RegWAPT, BPhy);    //B相无功比差值
    
  return TRUE;
}
/*=========================================================================================\n
* @function_name: InitRaccoon
* @function_file: Raccoon.c
* @描述: 初始化9260F
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void InitRaccoon(void)
{
    Init_RacCtrl();   // 通讯 
    Init_9260F_EMU();    // 寄存器配置初始化//
}



/*=========================================================================================\n
* @function_name: InitRaccoon
* @function_file: Raccoon.c
* @描述:  更新9260F 寄存器值
* 
* @参数: 
* @返回: 
* @作者:   zwk (2016-05-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void  Init_9260F_EMU(void)
{
    

	 //ANCTRL0,ANCTRL1,ANCTRL2,BPFPARA,MTPARA0   //  //
        WriteRaccoon(0x1D000483 ,RegANCtrl0, BPhy);   // 模拟不增益1D0004C3   0x185 增益I:32，U:1，M:4 温度系数，全局电流偏置
		WriteRaccoon(0x30000000 ,RegANCtrl1, BPhy);    //0x186  
    
        WriteRaccoon(0x00000000 ,RegANCtrl2, BPhy);    //0x187  
        WriteRaccoon(0x811D2BA7 ,RegBPFPARA, BPhy);    //0x0125 带通滤波器系数 0x811D2BA7
        WriteRaccoon(0x00000000 ,RegMTPARA0, BPhy);  //0x183 CF工作时钟3.2MHz 使能CF输出，能量累加，起动潜动，能量来源：平均全波有功
//B09A9  0x000b0900   0x000B09A9  数字增益//  x2 U  x4 I  
//	    WriteRaccoon(0x000b09A9 ,RegMTPARA1, BPhy);  //0x184 M通道关闭，仅进行配置自检，使能去直高通滤波器，使能（U/I）输入，角差  
	    WriteRaccoon(0x000B00D9 ,RegMTPARA1, BPhy);  //0x184 M通道关闭，仅进行配置自检，使能去直高通滤波器，使能（U/I）输入，角差  

        CLRWDT(); 



	//MTPARA1,WARTU,WARTI,WAPT,WWAPT,WWAQT  
        WriteRaccoon(0x00000000 ,RegWARTU, BPhy);    // //全波电压有效值比差寄存器
        WriteRaccoon(0x00000000 ,RegWARTI, BPhy);     //全波电流有效值比差寄存器 
        WriteRaccoon(0x00000000 ,RegWAPT, BPhy);      //全波有功功率比差寄存器
        WriteRaccoon(0x00000000 ,RegWWAPT, BPhy);   //全波有功功率二次补偿值寄存器
        WriteRaccoon(0x00000000 ,RegWWAQT, BPhy);  // //全波电压有效值比差寄存器
	 CLRWDT(); 

  //	WriteRaccoon(gs_JbPm.gs_JBB.Rac_WAPT,RegWAPT, BPhy);    //B相无功比差值

  

}

/*=========================================================================================\n
* @function_name: Accumulate_abc
* @function_file: EnergyBottom.c
* @描述  分相功率累加
* 
* @参数: 
* @返回: 
* @作者:   
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Accumulate_abc(void)
{
    /*static uint8 times=0;
    uint32 tmp_egyth;
    times++;
    if(times >=4)     //40ms累加一次 由于合相门限已经/1024了,分相门限扩大1024/(6400/25)=4倍 
    {
        times = 0;      
         //各相有功能量
        tmp_egyth =  (gs_JbPm.Com_EGYTH*4)>>8;   //A相  正向功率和反向功率取绝对值之和累加
        gs_power.ul_power_all[0] += (gs_power.ul_power[0] >> 8);     //防止累加溢出 处理
        gs_power.ui_power_rem[0] += (gs_power.ul_power[0] & 0x000000ff);
        if(gs_power.ui_power_rem[0] > 255)
        {
            gs_power.ul_power_all[0]++;
            gs_power.ui_power_rem[0] -= 255;
        }

        if(gs_power.ul_power_all[0] >= tmp_egyth)
        {
            gs_power.ul_power_all[0] -= tmp_egyth;
            EnyB_IntCF(0,active);
        }
        
        tmp_egyth =  (gs_JbPm.Com_EGYTH*4)>>8;   //B相  正向功率和反向功率取绝对值之和累加    
        gs_power.ul_power_all[1] += (gs_power.ul_power[1] >> 8);     //防止累加溢出 处理
        gs_power.ui_power_rem[1] += (gs_power.ul_power[1] & 0x000000ff);
        if(gs_power.ui_power_rem[1] > 255)
        {
            gs_power.ul_power_all[1]++;
            gs_power.ui_power_rem[1] -= 255;
        }

        if(gs_power.ul_power_all[1] >= tmp_egyth)
        {
            gs_power.ul_power_all[1] -= tmp_egyth;
            EnyB_IntCF(1,active);
        }

         //各相无功能量
        tmp_egyth =  (gs_JbPm.Com_REEGYTH*4)>>8; //A相   正向功率和反向功率取绝对值之和累加
        gs_power.ul_Npower_all[0] += (gs_power.ul_Npower[0] >> 8);
        gs_power.ui_Npower_rem[0] += (gs_power.ul_Npower[0] & 0x000000ff);
        if(gs_power.ui_Npower_rem[0] > 255)
        {
            gs_power.ul_Npower_all[0]++;
            gs_power.ui_Npower_rem[0] -= 255;
        }

        if(gs_power.ul_Npower_all[0] >= tmp_egyth)
        {
            gs_power.ul_Npower_all[0] -= tmp_egyth;
            EnyB_IntCF(0,reactive);
        }
        
        tmp_egyth =  (gs_JbPm.Com_REEGYTH*4)>>8; //B相  正向功率和反向功率取绝对值之和累加   
        gs_power.ul_Npower_all[1] += (gs_power.ul_Npower[1] >> 8);
        gs_power.ui_Npower_rem[1] += (gs_power.ul_Npower[1] & 0x000000ff);
        if(gs_power.ui_Npower_rem[1] > 255)
        {
            gs_power.ul_Npower_all[1]++;
            gs_power.ui_Npower_rem[1] -= 255;
        }

        if(gs_power.ul_Npower_all[1] >= tmp_egyth)
        {
            gs_power.ul_Npower_all[1] -= tmp_egyth;
            EnyB_IntCF(1,reactive);
        }
    } */  
}

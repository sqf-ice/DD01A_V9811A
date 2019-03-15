/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_EXmem.c
**Author		: maji
**date			: 2016-04-20
**description	: 外部存储器处理相关C代码
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>

// 1 E2P 数据分区结构定义   //
const  E2P_CHECK_TAB code  mem_table[] = {
    //block-0 当前电量数据块  //
    No_BLOCK00_EC_L0_E2P,      MEM_E2P1,   ADR_BLOCK00_EC_L0_E2P,  LEN_BLOCK00_EC_L0_E2P,
    //block-1  当前月最大需量数据块  //
    No_BLOCK01_MMD_L0_E2P,      MEM_E2P1,   ADR_BLOCK01_MMD_L0_E2P, LEN_BLOCK01_MMD_L0_E2P,
    //BLOCK20  电表参数区1 //
    No_BLOCK20_METER_PARAM1_E2P,    MEM_E2P1,   ADR_BLOCK20_METER_PARAM1_E2P, LEN_BLOCK20_METER_PARAM1_E2P,
       //BLOCK21  电表参数区-显示参数区 //
    No_BLOCK21_DIS_PARAM_E2P,    MEM_E2P1,   ADR_BLOCK21_DIS_PARAM_E2P, LEN_BLOCK21_DIS_PARAM_E2P,
		//二类事件参数区
	  No_BLOCK27_EVENII_data,          MEM_E2P1,   ADR_BLOCK27_EVENII_E2P,   LEN_CLASSII_VAR,
    //BLOCK25 电表变量数据区//
    No_BLOCK25_METER_VAR_E2P,    MEM_E2P1,   ADR_BLOCK25_METER_VAR_E2P, LEN_BLOCK25_METER_VAR_E2P,
    //BLOCK70  EMU 校表参数区//
    No_BLOCK70_EMU_PARAM_E2P,    MEM_E2P1,   ADR_BLOCK70_EMU_PARAM_E2P, LEN_BLOCK70_EMU_PARAM_E2P,
    //BLOCK21  电表参数区-显示参数区 //
    No_BLOCK21_DIS_PARAM_E2P,    MEM_E2P1,   ADR_BLOCK21_DIS_PARAM_E2P, LEN_BLOCK21_DIS_PARAM_E2P,
 //block-81~84  费率相关参数数据块   //
#if (TARIFF_MAX_NUM>1)
#if(MAX_NUM_HOLIDAY_TAB>0)  
    //block-81 节假日表数据块   //
    No_BLOCK81_HILIDAY_TAB_E2P,    MEM_E2P1,    ADR_BLOCK81_HILIDAY_TAB_E2P,    LEN_HoliTab,
#endif
    //block-82 季节表数据块   //
    No_BLOCK82_SEASON_TAB_E2P,    MEM_E2P1,    ADR_BLOCK82_SEASON_TAB_E2P,    LEN_SeasonTab,
    //block-83 周表数据块   //
    No_BLOCK83_WEEK_TAB_E2P,    MEM_E2P1,    ADR_BLOCK83_WEEK_TAB_E2P,    LEN_WeekTab,
    //block-84 日时段表数据块   //
    No_BLOCK84_DAY_TAB_E2P,    MEM_E2P1,    ADR_BLOCK84_DAY_TAB_E2P,    LEN_DayTab,
#endif
    //BLOCK100~136 月结算数据区  //
#if (BILL_MAX_NUM>0)
    No_BLOCK26_BILL_VAR_E2P,       MEM_E2P1,    ADR_BLOCK26_BILL_VAR_E2P,       LEN_BLOCK26_BILL_VAR_E2P,
    No_BLOCK101_BILL1_DATA_E2P,    MEM_E2P1,    ADR_BLOCK101_BILL1_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
    No_BLOCK102_BILL2_DATA_E2P,    MEM_E2P1,    ADR_BLOCK102_BILL2_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
    No_BLOCK103_BILL3_DATA_E2P,    MEM_E2P1,    ADR_BLOCK103_BILL3_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
    No_BLOCK104_BILL4_DATA_E2P,    MEM_E2P1,    ADR_BLOCK104_BILL4_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
    No_BLOCK105_BILL5_DATA_E2P,    MEM_E2P1,    ADR_BLOCK105_BILL5_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
    No_BLOCK106_BILL6_DATA_E2P,    MEM_E2P1,    ADR_BLOCK106_BILL6_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
//    No_BLOCK107_BILL7_DATA_E2P,    MEM_E2P1,    ADR_BLOCK107_BILL7_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
//    No_BLOCK108_BILL8_DATA_E2P,    MEM_E2P1,    ADR_BLOCK108_BILL8_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
//    No_BLOCK109_BILL9_DATA_E2P,    MEM_E2P1,    ADR_BLOCK109_BILL9_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
//    No_BLOCK110_BILL10_DATA_E2P,    MEM_E2P1,    ADR_BLOCK110_BILL10_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
//    No_BLOCK111_BILL11_DATA_E2P,    MEM_E2P1,    ADR_BLOCK111_BILL11_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
//    No_BLOCK112_BILL12_DATA_E2P,    MEM_E2P1,    ADR_BLOCK112_BILL12_DATA_E2P,    LEN_BLOCK10X_BILLX_DATA_E2P,
#endif

 
};
//-------------------------------------------------------------------------------//
//  Function:      INT16U get_mem_ck_data( INT16U src, INT8U  lenth, INT8U memType )             
//  Description:   计算所给地址开始的长度为LENTH的累加校验和
//  Parameters:     
//  Returns:                                  
//  Others:  
//-------------------------------------------------------------------------------//
INT16U get_mem_ck_data( INT16U src, INT16U  lenth, INT8U memType )
{
 INT8U size;
 INT8U tmpBuf[E2P_PAGE];
 INT16U   sum = CHECKWORD;

    while( lenth > 0 )
    {
        if( lenth >= E2P_PAGE) 
        {
            size = E2P_PAGE;
        }
        else
        {
            size = lenth;
        }
        mem_read(&tmpBuf[0], src, size, memType);
        sum +=Lib_get_csck_int16u_num((INT8U*)&tmpBuf[0], size ,0x0000) ;
        src += size;
        lenth -= size;
   }
    
    return sum ;
}


/**********************************************************************
	Get Check Table Point
//上层调用的操作类型和所对应的存储的地址一致
  RAM就查找RAM的地址，E2就查找E2的地址，等等。
  地址数据类型为int 型  
**********************************************************************/
INT8U get_mem_table_Index( INT16U adr, INT8U memType )   
{
 INT8U  i;
 INT8U  len;
 INT16U blkAddr; 
	
    for( i=0; i<(sizeof(mem_table)/sizeof(E2P_CHECK_TAB)); i++ ) 
    {
        if(memType == mem_table[i].mem_type)
        {
            if(memType != MEM_E2P1)
            {
                return 0xff;
            }
            else
            {
                blkAddr =mem_table[i].u16_adr;
            }
            len = mem_table[i].u16_len;

            if( (adr>=blkAddr) && (adr<(blkAddr+len)) )
            {
                return i;
            }
        }
    }
    return 0xff;
}


/**********************************************************************
	Move Data from Memory to Memory
**********************************************************************/
void mem_to_mem( INT16U dst, INT16U src, INT16U lenth, INT8U mode )
{
	INT8U	tmp_Buffer[E2P_PAGE];
	INT8U		size;
	
	while( lenth > 0 )
	{
		if( lenth > E2P_PAGE )
		{
			size = E2P_PAGE;
		}
		else
		{
			size = lenth;
		}
		mem_read(&tmp_Buffer[0], src, size, (mode>>4)&0x0F );
		CLRWDT();
		mem_write( dst, &tmp_Buffer[0], size, mode&0x0F  );
		dst += size;
		src += size;
		lenth -= size;
	}
}


/**********************************************************************
	Fill Data to Memory(Flash, InfoFlash, FRAM, EEPROM, DataFlash)
**********************************************************************/
void mem_fill( INT16U dst, INT16U lenth, INT8U xx, INT8U memType )
{
INT8U	tmp_Buffer[E2P_PAGE];
INT8U size;

    Lib_Set_String(&tmp_Buffer[0], xx, E2P_PAGE);

	   while( lenth > 0 )
	   {
			if( lenth > E2P_PAGE )
			{
				size = E2P_PAGE;
			}
			else
			{
				size = lenth;
			}
			mem_write( dst, &tmp_Buffer[0], size, memType );
			CLRWDT();
			dst += size;
			lenth -= size;
	   }
}

/**********************************************************************
	Read Data to Memory(FRAM, EEPROM, DataFlash)
**********************************************************************/
void mem_read( INT8U* dst,  INT16U src, INT16U lenth, INT8U memType )
{
     INT8U  *ptr_adr;
	 
    switch( memType )
    {
        case MEM_RAM:
            ptr_adr = &((INT8U *)src);
            Lib_Copy_Str_TwoArry( dst, ptr_adr, lenth );
          //  Lib_FCpyTMem(dst, ptr_adr, lenth);
        break;

        case MEM_E2P1:
            Read_EEPROM(dst,src, lenth);   //E2P_Read( Dest, Src, Lenth );
        break;

        default:
            sys_err();
        break;
    }
}

/**********************************************************************
	Write Data to Memory( FRAM, EEPROM, DataFlash )
**********************************************************************/
void mem_write( INT16U dst,  INT8U *src, INT16U lenth, INT8U memType )
{
INT8U  *ptr_adr;

    switch( memType )
    {
        case MEM_RAM:
            ptr_adr = &((INT8U *)dst);
            Lib_Copy_Str_TwoArry( ptr_adr, src, lenth ); //memcpy( (char*)Dest, Src, Lenth );
        break;

        case MEM_E2P1:
            Write_EEPROM(dst,src, lenth);                              //E2P_Write( Dest, Src, Lenth );
        break;

        default:
            sys_err();

        break;
    }
}



/*
//-------------------------------------------------------------------------------//
//  Function:      void MemWrite( unsigned int dst,  char *src, int lenth, char memType )               
//  Description:  将指定的RAM区 src地址起始的数据写入用memType指定的类型器件数据区
//  Parameters:  char *dst : 目标数据区首地址
//                     unsigned int src: 源数据区首地址，RAM区
//                     int lenth  :  数据操作长度
//                     char memType  : 存储器操作类型
//  Returns:       无                            
//  Others:        
//-------------------------------------------------------------------------------//
*/
void mem_db_write( INT16U dst,  INT8U *src, INT16U len, INT8U memType )
{
INT8U index;
ST_U16_U08 checkNum;
INT16U adr_dst;
INT16U adr_csck;
INT16U lenth;
INT8U id_No;

    //获取数据索引号//
    index = get_mem_table_Index(dst, memType );
	
    if(index >= (sizeof(mem_table)/sizeof(E2P_CHECK_TAB)))
    {
        sys_err();
    }
    adr_dst = mem_table[index].u16_adr ;
    lenth = mem_table[index].u16_len-2;
    adr_csck =  adr_dst + lenth;  
    id_No = mem_table[index].u8_No;

    mem_write(dst, src, len, memType);
    checkNum.u16 = get_mem_ck_data(adr_dst, lenth, MEM_E2P1);
    mem_write(adr_csck, &checkNum.B08[0], 2, MEM_E2P1);
    if(id_No<100) 
    {  
        mem_write(dst+E2PBAK_SIZE, src, len, memType);
        mem_write(adr_csck+E2PBAK_SIZE, &checkNum.B08[0], 2, MEM_E2P1);
    }

}


/****************************************************************
Function:   		void SysInitialize(void)  
Description: 		系统上电/复位初始化函数 
Input:			no
Parameters:     	no
Returns:			no                        
Others:         
****************************************************************/
void get_default_mem_data(uint8 index)
{
 uint16 adrAry;
 uint16 lenth;
 uint8 i;

    for(i=0;i<(sizeof(mem_table)/sizeof(E2P_CHECK_TAB));i++ )
    {
        if(index == mem_table[i].u8_No)   break;
    }
 
    if(i>=(sizeof(mem_table)/sizeof(E2P_CHECK_TAB)))   return;

	
    adrAry= mem_table[i].u16_adr;
    lenth = mem_table[i].u16_len-2;
    
    switch(index)
    {
        case No_BLOCK20_METER_PARAM1_E2P:
            mem_write(adrAry, (INT8U*)&default_meter_parm1_tab, lenth,  MEM_E2P1);
        break;		

        case No_BLOCK21_DIS_PARAM_E2P:
            mem_write(adrAry, (INT8U*)&default_dis_param_tab, lenth,  MEM_E2P1);
        break;	


        case No_BLOCK70_EMU_PARAM_E2P:
            mem_write(adrAry, (INT8U*)&default_adj_emu_param_tab, lenth,  MEM_E2P1);
        break;
#if (TARIFF_MAX_NUM>1)
#if(MAX_NUM_HOLIDAY_TAB>0)  
        case No_BLOCK81_HILIDAY_TAB_E2P:
            mem_write(adrAry, (INT8U*)&default_triff_holiday_tab, lenth,  MEM_E2P1);//block-81 节假日表数据块   //
        break;
#endif
        case No_BLOCK82_SEASON_TAB_E2P:
            mem_write(adrAry, (INT8U*)&default_triff_season_tab, lenth,  MEM_E2P1); //block-82 季节表数据块   //
        break;

        case No_BLOCK83_WEEK_TAB_E2P:
            mem_write(adrAry, (INT8U*)&default_triff_week_tab, lenth,  MEM_E2P1);//block-83 周表数据块   //
        break;

        case No_BLOCK84_DAY_TAB_E2P:
            mem_write(adrAry, (INT8U*)&default_triff_day_tab, lenth,  MEM_E2P1);//block-84 日时段表数据块   //
        break;
#endif
      /* case No_BLOCK27_EVENII_data:
            mem_write(adrAry, (INT8U*)&gs_even_var, lenth,  MEM_E2P1);//block-84 日时段表数据块   //
        break;
        */
        default:
            mem_fill(adrAry, lenth, 0x00, MEM_E2P1);
        break;
    }
}


/*
//-------------------------------------------------------------------------------//
//  Function:      void MemWrite( unsigned int dst,  char *src, int lenth, char memType )               
//  Description:  将指定的RAM区 src地址起始的数据写入用memType指定的类型器件数据区
//  Parameters:  char *dst : 目标数据区首地址
//                     unsigned int src: 源数据区首地址，RAM区
//                     int lenth  :  数据操作长度
//                     char memType  : 存储器操作类型
//  Returns:       无                            
//  Others:        
//-------------------------------------------------------------------------------//
*/
void mem_db_check( void )
{
 INT8U i;
 ST_U16_U08 checkNum0;
 ST_U16_U08 checkNum1;
 INT16U adr_dst;
 INT16U adr_csck;
 INT16U lenth;
 INT8U staus;
 uint8  u8_No;

    for(i=0;i<(sizeof(mem_table)/sizeof(E2P_CHECK_TAB));i++ )
    {
        staus = 0;
        adr_dst= mem_table[i].u16_adr;
        lenth = mem_table[i].u16_len-2;
        adr_csck = adr_dst + lenth; 
        u8_No = mem_table[i].u8_No;

        CLRWDT();
        //check main aera data  //
        checkNum0.u16= get_mem_ck_data(adr_dst,lenth, MEM_E2P1);
        mem_read(&checkNum1.B08[0], adr_csck, 2,MEM_E2P1 );
        if(checkNum0.u16 != checkNum1.u16)  staus |= 0x01;
   
        if(u8_No<100)     
        {
            //check backup aera data  //
            checkNum0.u16 = get_mem_ck_data(adr_dst+E2PBAK_SIZE,lenth, MEM_E2P1);
            mem_read(&checkNum1.B08[0], adr_csck+E2PBAK_SIZE, 2,MEM_E2P1 );
            if(checkNum0.u16 != checkNum1.u16)  staus |= 0x02;
        }
            
        // deal to checked  result  //
//DEBUGE  //
//staus=0X03;
////////
        CLRWDT();
        switch(staus)
        {
            case 0:                // main aera data right, backup aera data right //

                break;

            case 1:               // main aera data error, backup aera data right //
                 if(u8_No<100)
                {     
                   mem_to_mem(adr_dst, adr_dst+E2PBAK_SIZE, lenth+2, (MEM_E2P1<<4)+MEM_E2P1);
                }
                else
                {
                    get_default_mem_data(u8_No);   
                    checkNum0.u16= get_mem_ck_data(adr_dst,lenth, MEM_E2P1);
                    mem_write(adr_csck, &checkNum0.B08[0], 2, MEM_E2P1);
                }
                CLRWDT();
                break;	

            case 2:              // main aera data right, backup aera data error //
                    if(u8_No<100)     
      			    {
            	     mem_to_mem(adr_dst+E2PBAK_SIZE,adr_dst, lenth+2, (MEM_E2P1<<4)+MEM_E2P1);
		             CLRWDT();
        			}
				//	mem_read(&checkNum1.B08[0], adr_csck+E2PBAK_SIZE, 2,MEM_E2P1 );
                break;	

            case 3:               // main aera data error, backup aera data error //
			get_default_mem_data(u8_No);
			checkNum0.u16= get_mem_ck_data(adr_dst,lenth, MEM_E2P1);
			mem_write(adr_csck, &checkNum0.B08[0], 2, MEM_E2P1);
			CLRWDT();
            if(u8_No<100)     
			{
			mem_to_mem(adr_dst+E2PBAK_SIZE,adr_dst, lenth+2, (MEM_E2P1<<4)+MEM_E2P1); 
	 		CLRWDT();
            }
            break;	             

            default:               
            	   sys_err();
                break;	                   
        }
	CLRWDT();
        
    }

}



/*
//-------------------------------------------------------------------------------//
//  Function:      mem_db_clr           
//  Description:  将所有的备份数据备份区清零，用于初始化E2P数据
//  Parameters:  char *dst : 目标数据区首地址
//                     unsigned int src: 源数据区首地址，RAM区
//                     int lenth  :  数据操作长度
//                     char memType  : 存储器操作类型
//  Returns:       无                            
//  Others:        
//-------------------------------------------------------------------------------//
*/
void mem_db_clr( uint8 type )
{
 INT8U i;
 ST_U16_U08 checkNum0;
 INT16U adr_dst;
 INT16U adr_csck;
 INT8U lenth;
 uint8  u8_No;

    for(i=0;i<(sizeof(mem_table)/sizeof(E2P_CHECK_TAB));i++ )
    {
        adr_dst= mem_table[i].u16_adr;
        lenth = mem_table[i].u16_len-2;
        adr_csck = adr_dst + lenth; 
        u8_No = mem_table[i].u8_No;

         if((u8_No !=No_BLOCK70_EMU_PARAM_E2P) ||(type ==0XA5))
          {
             CLRWDT();
            //check main aera data  //
            checkNum0.u16= 0;
            mem_write(adr_csck,&checkNum0.B08[0],  2,MEM_E2P1 );
       
            if(u8_No<100)     
            {
                //check backup aera data  //
                checkNum0.u16 = 0;
                mem_write(adr_csck+E2PBAK_SIZE,&checkNum0.B08[0],  2,MEM_E2P1 );
            }
        }
            
    }

}

/***************************************************************
*    END
****************************************************************/


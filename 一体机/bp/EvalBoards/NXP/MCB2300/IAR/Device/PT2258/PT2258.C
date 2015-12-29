#include  "includes.h" 
uint16	SEL[3]={0xffff,0xffff,0xffff};

//#pragma pack(1)
//typedef struct VGA_OUT
//{
//  uint8 ENABLE:1;
//  uint8 GAIN:1;
//  uint8 VGA_IN:4;
//}VGA_OUT_CHANNEL;


//#pragma pop()

extern struct LINK_REC_TABLE_TYPE DEVICE_LINK;
extern  struct DEVICE_STATUS_TYPE gDeviceStatus;	//一体机状态
  /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////

void	Delay(uint16	k)
{
while(--k);
}
//ic=u37Add,u42Add
//ch=CH1,CH2,CH3,CH4,CH5,CH6
//volume=1-10//10表示声音最大，1表示音量最小，就是静音
  /////////////////////////////////////////////////////////////////////
//	函数名称：音量控制
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void	SoundVolumeControl(uint8	ch,uint8	volume)
{
uint8	tmp[3]={0},IC,flag,i;
uint16 vol;
if (volume>100)  volume=100;
vol=40-volume*4/10;
if (vol>35) vol=70;
switch(ch)
	{
		case VOL_CH_DEVGF:
			IC=U37Add;		
			*tmp=0x80|(vol/10); 
			*(tmp+1)=0x90|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_DEVGF write err");
			*(tmp)=0x40|(vol/10); 
			*(tmp+1)=0x50|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_DEVGF write err");	
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol=volume;

			break;
		case VOL_CH_MIC1:
			IC=U37Add;
			*(tmp)=0x00|(vol/10); 
			*(tmp+1)=0x10|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_MIC1 write err");
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIC_Vol[0]=volume;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIC_Vol[0]=volume;
			break;
		case VOL_CH_MIC2:
			IC=U37Add;
			*(tmp)=0x20|(vol/10); 
			*(tmp+1)=0x30|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_MIC2 write err");
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIC_Vol[1]=volume;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIC_Vol[1]=volume;
			break;
		case VOL_CH_MIC3:
			IC=U37Add;
			*(tmp)=0x60|(vol/10); 
			*(tmp+1)=0x70|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_MIC3 write err");
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIC_Vol[2]=volume;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIC_Vol[2]=volume;
			break;
		case VOL_CH_MIC4:		
			IC=U37Add;
			*(tmp)=0xa0|(vol/10); 
			*(tmp+1)=0xb0|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_MIC4 write err");
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIC_Vol[3]=volume;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIC_Vol[3]=volume;
			break;
		case  VOL_CH_DEVMID:
			IC=U42Add;
			*(tmp)=0x80|(vol/10); 
			*(tmp+1)=0x90|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_DEVMID write err");
			i=0;
			*(tmp)=0x40|(vol/10); 
			*(tmp+1)=0x50|(vol%10);
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_DEVMID write err");
			break;
		case  VOL_CH_GF:
			IC=U42Add;
			*(tmp)=0x00|(vol/10); 
			*(tmp+1)=0x10|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_GF write err");
			*(tmp)=0x20|(vol/10); 
			*(tmp+1)=0x30|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>3) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_GF write err");
			break;
		case VOL_CH_MID:
			IC=U42Add;
			*(tmp)=0x60|(vol/10); 
			*(tmp+1)=0x70|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>10) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_MID write err");
			*(tmp)=0xa0|(vol/10); 
			*(tmp+1)=0xb0|(vol%10);
			i=0;
			do{
			flag=I2C0_WriteNByte(IC, ONE_BYTE_SUBA, 0, tmp, 2);						
			if (flag) break;
			if(i++>10) break;
			}while(1);
			if (i>2) IP_printf("VOL_CH_MID write err");
			break;
		default:
			break;

		}
	
}
  /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：flag=1 Mute on
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void SOUND_DEVICE_Mute(uint8 chanle,uint8 flag)
{
 switch(chanle)
 	{case VOL_CH_MIC1:
 		if (flag)
 			{SEL[2]&=~SOUND_SELECT_IO5;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL|=SMIX_CONTRLMIC1;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIX_CONTRL|=SMIX_CONTRLMIC1;
			}
		else
			{SEL[2]|=SOUND_SELECT_IO5;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL&=~SMIX_CONTRLMIC1;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIX_CONTRL&=~SMIX_CONTRLMIC1;
			}
 		SpiSendData(SEL,3);
		break;
	 case 	VOL_CH_MIC2:
	 	if (flag)
			{
	 		SEL[2]&=~SOUND_SELECT_IO6;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL|=SMIX_CONTRLMIC2;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIX_CONTRL|=SMIX_CONTRLMIC2;
			}
		else
			{SEL[2]|=SOUND_SELECT_IO6; 			
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL&=~SMIX_CONTRLMIC2;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIX_CONTRL&=~SMIX_CONTRLMIC2;
			}
 			SpiSendData(SEL,3);
		break;
	 case 	VOL_CH_MIC3:
	 	if (flag)
	 		{SEL[2]&=~SOUND_SELECT_IO7;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL|=SMIX_CONTRLMIC3;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIX_CONTRL|=SMIX_CONTRLMIC3;
			}
		else
			{SEL[2]|=SOUND_SELECT_IO7;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL&=~SMIX_CONTRLMIC3;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIX_CONTRL&=~SMIX_CONTRLMIC3;
			}
 		SpiSendData(SEL,3);
		break;		
	 case 	VOL_CH_MIC4:	
	 	if (flag)
	 		{SEL[2]&=~SOUND_SELECT_IO8;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL|=SMIX_CONTRLMIC4;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIX_CONTRL|=SMIX_CONTRLMIC4;
			}
		else
			{SEL[2]|=SOUND_SELECT_IO8;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL&=~SMIX_CONTRLMIC4;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].MIX_CONTRL&=~SMIX_CONTRLMIC4;
			}
 		SpiSendData(SEL,3);
		break;
		
	case 	VOL_CH_DEVGF:	
	case VOL_CH_GF:
	case VOL_CH_MID:
	 	if (flag)
			SEL[2]|=SOUND_SELECT_IO13; 		
		else
			SEL[2]&=~SOUND_SELECT_IO13;
 		SpiSendData(SEL,3);
		break;	
		
	/*	  
		  if (flag)
		     *(tmp)=0xf9;
		  else
		  	*(tmp)=0xf8;
		 I2C0_WriteNByte(U42Add, ONE_BYTE_SUBA, 0, tmp, 1);
		break;		
		*/
 	}
}
 
 
 /////////////////////////////////////////////////////////////////////
//	函数名称：HC595
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////

void	SpiSendData(uint16	*data,uint8	len)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif    

OS_ENTER_CRITICAL();
	IO1CLR|=HC595_CS;
	while(len>0)
		{
			SPI_send((uint8)(data[0]>>8));
			SPI_send((uint8)(data[0]));	
			data++;
			len--;
		}
	IO1SET|=HC595_CS;
	Delay(100);
	IO1CLR|=HC595_CS;
	Delay(100);
OS_EXIT_CRITICAL();    
}

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//	函数名称：MAX9675
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////

void	SpiSendData1(uint8	*data,uint8 len)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif    
OS_ENTER_CRITICAL();
  //uint16 tmp_reg=S0SPCR;
  /*S0SPCR  = (1<<2)|                             // 16位数据发送
	(0 << 3) |				// CPHA = 0, 数据在SCK 的第一个时钟沿采样
 	(0 << 4) |				// CPOL = 1, SCK 为低有效
 	(1 << 5) |				// MSTR = 1, SPI 处于主模式
 	(0 << 6) |				// LSBF = 0, SPI 数据传输MSB (位15)在先
 	(0 << 7);				// SPIE = 0, SPI 中断被禁止*/
  
        IO1CLR |= MAX9675_CE;
	//FIO3CLR|=MAX9675_UPDATA;
        while(len>0)
	{
          SPI_send(data[0]);
	  data++;
	  len--;
	}
        
        FIO3CLR|=MAX9675_UPDATA;
	FIO3SET|=MAX9675_UPDATA;
        
        IO1SET |=MAX9675_CE;
        
        //S0SPCR=tmp_reg;
OS_EXIT_CRITICAL();    

}



///////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
//	函数名称：选择视频编码开的音频源
//	函数功能：void BMK_Sound_Select(uint8 val)
//	入口参数val=0  面板MICl
//			 VAL=1 功放
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void BMK_Sound_Select(uint8 val)
{
	if (val)
		{SEL[2]&=~SOUND_SELECT_IO9;
		 SEL[2]&=~SOUND_SELECT_IO10;	
		}
	else
		{SEL[2]|=SOUND_SELECT_IO9;
		 SEL[2]|=SOUND_SELECT_IO10;
		}
	SpiSendData(SEL,3);
}
 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void BJ_Select(uint8 contrl)  
{
if (contrl)
	SEL[2]|=SOUND_SELECT_IO16;
else
	SEL[2]&=~SOUND_SELECT_IO16;
      SpiSendData(SEL,3);
}
 /////////////////////////////////////////////////////////////////////
//	函数名称：网络音频输入选择
//	函数功能：
//	入口参数：type=0 麦克输出
//			          1面板MIC
//				2媒体输出 3功放输出	
//	出口参数：无
////////////////////////////////////////////////////////////////////
void LAN_Sound_Select(uint8 type)
{
	switch(type)
		{case 0:
			SEL[2]&=~SOUND_SELECT_IO11;
			SEL[2]&=~SOUND_SELECT_IO12;
			break;
		 case 1:
		 	SEL[2]|=SOUND_SELECT_IO11;
			SEL[2]&=~SOUND_SELECT_IO12;
			break;
		case 2:	
			SEL[2]&=~SOUND_SELECT_IO11;
			SEL[2]|=SOUND_SELECT_IO12;
			break;
		case 3:
			SEL[2]|=SOUND_SELECT_IO11;
			SEL[2]|=SOUND_SELECT_IO12;
			break;
		}
SpiSendData(SEL,3);
}
 /////////////////////////////////////////////////////////////////////
//	函数名称：网络音频输出选择
//	函数功能：
//	入口参数：type=0 AIC23输出送面板扬声器
//				1 AIC23输出送功放
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void LAN_Sound_OUT(uint8 type)
 {	if (type)
	 	{SEL[2]&=~SOUND_SELECT_IO14;
	 	 SEL[2]&=~SOUND_SELECT_IO15;
		 
	 	}
	 else
	 	{SEL[2]|=SOUND_SELECT_IO14;
	 	 SEL[2]|=SOUND_SELECT_IO15;
	 	}
	SpiSendData(SEL,3);
 }
  /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_InVol(BYTE Vol,BYTE in)
{uint8 i;
if (in==DEV_SOUND_NONE) return; 
switch(in)
	{    case DEV_SOUND_MIC1:
			for (i=1;i<5;i++)
				DEVICE_LINK.Sound_Out[i].MIC_Vol[DEV_SOUND_MIC1-1]=Vol;					
			SoundVolumeControl(VOL_CH_MIC1,Vol);
			break;
		case DEV_SOUND_MIC2:
			for (i=1;i<5;i++)
				DEVICE_LINK.Sound_Out[i].MIC_Vol[DEV_SOUND_MIC2-1]=Vol;					
			SoundVolumeControl(VOL_CH_MIC2,Vol);
			break;
		case DEV_SOUND_MIC3:
			for (i=1;i<5;i++)
				DEVICE_LINK.Sound_Out[i].MIC_Vol[DEV_SOUND_MIC3-1]=Vol;					
		        SoundVolumeControl(VOL_CH_MIC3,Vol);
			break;
		case DEV_SOUND_MIC4:
			for (i=1;i<5;i++)
				DEVICE_LINK.Sound_Out[i].MIC_Vol[DEV_SOUND_MIC4-1]=Vol;					
			SoundVolumeControl(VOL_CH_MIC4,Vol);
			break;
		case DEV_SOUND_DVD:
		case DEV_SOUND_PC:	
		case DEV_SOUND_NPC:	
		case DEV_SOUND_LYJ:	
		case DEV_SOUND_KZ:	
	        case DEV_SOUND_DEV:				
                        DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].DEV_Vol=Vol;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].DEV_Vol=Vol;
			SoundVolumeControl(VOL_CH_DEVGF,Vol);
			SoundVolumeControl(VOL_CH_DEVMID,Vol);	
			Device_OutVol(95,DEV_SOUND_OUT_LMT);
                        
			break;
					
		}

}
   /////////////////////////////////////////////////////////////////////
  //  函数名称：
  //  函数功能：
  //  入口参数：
  //		  
  //		  
  //  出口参数：无
  ////////////////////////////////////////////////////////////////////
void Device_Vol_Scan(void)  
{


SoundVolumeControl(VOL_CH_GF,DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol);			
SoundVolumeControl(VOL_CH_MID,DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].OUT_Vol);

}
 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_OutVol(BYTE Vol,BYTE out)
{uint32 tmp,bak,volBack;
volBack = Vol;
if (out==DEV_SOUND_OUT_NONE) return; 
	switch(out)
		{case DEV_SOUND_OUT_GF:
		//gDeviceTable.Debug |= DEV_DEBUG_MSG;
		
		//IP_printf("volBack=%d Vol=%d",volBack,Vol);
		
		
			tmp=VOL_CH_GF;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol=volBack;
			if (gDeviceStatus.MK_Vol&0x8000)
			{LED_Set(LED_MUTE,LED_DISPLAY_OFF);
			  LED_SendFLASH();
			  bak=Device_Get_MIX();
			  Device_Set_MIX(bak&(~SMIX_OUT_GF));
			 gDeviceStatus.MK_Vol &=0x7FFF;
			 Key_TB_state(KEY_REP_MIC_MUTE,TRUE);
			 Key_TB_state( KEY_REP_DEVICE_MUTE,FALSE );
			}
			if (Get_Debug(DEV_DEBUG_MSG))
				IP_printf("volBack=%d Vol=%d",volBack,Vol);
			SoundVolumeControl(tmp,volBack);
			gDeviceTable.Debug &=  ~DEV_DEBUG_MSG;
		
			break;
		case DEV_SOUND_OUT_LMT:
			tmp=VOL_CH_MID;
			DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].OUT_Vol=Vol;
			
			SoundVolumeControl(tmp,Vol);
			break;
		case DEV_SOUND_OUT_BMK:				
			break;
		case DEV_SOUND_OUT_YSQ:				
			break;
		}


}
   /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE Device_Get_Vol(BYTE Device,BYTE Type)
{
			switch(Type)
			{case 0:
			  case 1:
			  case 2:
			  case 3:
			  	 return DEVICE_LINK.Sound_Out[Device].MIC_Vol[Type];
			  case 4:
			  	 return DEVICE_LINK.Sound_Out[Device].DEV_Vol;
			  case 5:
			  	return DEVICE_LINK.Sound_Out[Device].OUT_Vol;
			  case 6:
			  	return DEVICE_LINK.Sound_Out[Device].LAN_Vol;
			  	

			}

return 0;
}
  /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint16 Device_Get_MIX(void) 
{
	return DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL;
}

 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_Set_MIX(uint16 SContrl) 
{uint16 tmp;
	DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].MIX_CONTRL=SContrl;
	//MIC1静音
	if (SContrl&SMIX_CONTRLMIC1)
		SOUND_DEVICE_Mute(VOL_CH_MIC1,1);	
	else
		SOUND_DEVICE_Mute(VOL_CH_MIC1,0);	
	//MIC2静音
	if (SContrl&SMIX_CONTRLMIC2)
		SOUND_DEVICE_Mute(VOL_CH_MIC2,1);	
	else
		SOUND_DEVICE_Mute(VOL_CH_MIC2,0);	
	//MIC3静音
	if (SContrl&SMIX_CONTRLMIC3)
		SOUND_DEVICE_Mute(VOL_CH_MIC3,1);	
	else
		SOUND_DEVICE_Mute(VOL_CH_MIC3,0);	
	//MIC4静音
	if (SContrl&SMIX_CONTRLMIC4)
		SOUND_DEVICE_Mute(VOL_CH_MIC4,1);	
	else
		SOUND_DEVICE_Mute(VOL_CH_MIC4,0);	
	//设备输入静音
	if (SContrl&SMIX_DEVICE)
		SOUND_DEVICE_Mute(VOL_CH_DEVGF,1);	
//		SoundVolumeControl(VOL_CH_DEVGF,0);
	else
		//Device_InVol(DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].DEV_Vol,DEV_SOUND_DEV);
		SOUND_DEVICE_Mute(VOL_CH_DEVGF,0);	
	//功放静音
/*	if ((SContrl&SMIX_OUT_GF)||(SContrl&SMIX_OUT_LMT))
		{
		SOUND_DEVICE_Mute(VOL_CH_MIC1,1);	
		SOUND_DEVICE_Mute(VOL_CH_MIC2,1);	
		SOUND_DEVICE_Mute(VOL_CH_MIC3,1);	
		SOUND_DEVICE_Mute(VOL_CH_MIC4,1);	
		SOUND_DEVICE_Mute(VOL_CH_DEVGF,1);	
		}
		//SoundVolumeControl(VOL_CH_GF,0);
	else
		{
		SOUND_DEVICE_Mute(VOL_CH_MIC1,0);	
		SOUND_DEVICE_Mute(VOL_CH_MIC2,0);	
		SOUND_DEVICE_Mute(VOL_CH_MIC3,0);	
		SOUND_DEVICE_Mute(VOL_CH_MIC4,0);	
		SOUND_DEVICE_Mute(VOL_CH_DEVGF,0);	
		}
*/
		//SoundVolumeControl(VOL_CH_GF,DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol);
		//Device_OutVol(DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol,DEV_SOUND_OUT_GF);//
	//流媒体音频静音
//	if (SContrl&SMIX_OUT_LMT)
		//Device_OutVol(0,DEV_SOUND_OUT_LMT);
		//SoundVolumeControl(VOL_CH_MID,0);
//	else
		//SoundVolumeControl(VOL_CH_MID,DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].OUT_Vol);
		//Device_OutVol(DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].OUT_Vol,DEV_SOUND_OUT_LMT);

	/*
	if (SContrl&SMIX_LAN_DEVICE)
		SOUND_DEVICE_Mute(VOL_CH_DEVGF,1);
	else
		SOUND_DEVICE_Mute(VOL_CH_DEVGF,0);	
	*/	
	//编码卡输入选择
	if (SContrl&SMIX_BMK_IN_SELEC)
		BMK_Sound_Select(1);//gf
	else
		BMK_Sound_Select(0);//mk
	//网络音频输出选择
	if (SContrl&SMIX_LAN_OUT_SELEC)
		LAN_Sound_OUT(1);//gf
	else
		LAN_Sound_OUT(0);//mk
	//网络音频输入选择
	tmp=(SContrl>>12)&0x0f;
	LAN_Sound_Select(tmp);	

}
    /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_SoundMute(BYTE out_link)
{

	SOUND_DEVICE_Mute(VOL_CH_MIC1,out_link&1);
	SOUND_DEVICE_Mute(VOL_CH_MIC2,out_link&(1<<1));
	SOUND_DEVICE_Mute(VOL_CH_MIC3,out_link&(1<<2));
	SOUND_DEVICE_Mute(VOL_CH_MIC4,out_link&(1<<3));
	SOUND_DEVICE_Mute(VOL_CH_MIC1,out_link&(1<<4));
	SOUND_DEVICE_Mute(VOL_CH_DEVGF,out_link&(1<<5));
	SOUND_DEVICE_Mute(VOL_CH_MID,out_link&(1<<6));


}
/////////////////////////////////////////////////////////////////////
//	函数名称：设备音频选择
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_Sound_Contrl(uint8 type)
{
	if (type)
		SEL[2]&=~SOUND_SELECT_IO13;//U38_P6 INH=0
	else
		SEL[2]|=SOUND_SELECT_IO13;//U38_P6 INH=0
	SpiSendData(SEL,3);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：设备音频选择
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void	Device_Sound_Select(uint8	device)
{uint8 tmp;
if (DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].Device==device) return ;
DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].Device=device;
DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].Device=device; 
tmp=Device_Get_Vol(DEV_SOUND_OUT_GF,4);
Device_InVol(0,DEV_SOUND_DEV);
switch(device)
	{
	case	DEV_SOUND_DVD:
		//SEL[2]&=0x07;
		SEL[2]&=~SOUND_SELECT_IO13;//U38_P6 INH=0
		SEL[2]&=~SOUND_SELECT_IO1;
		SEL[2]&=~SOUND_SELECT_IO2;
		break;
	case	DEV_SOUND_LYJ:
		//SEL[2]&=0x07;
		SEL[2]&=~SOUND_SELECT_IO13;//U38_P6 INH=0
		SEL[2]|=SOUND_SELECT_IO1;
		SEL[2]&=~SOUND_SELECT_IO2;
		break;
	case	DEV_SOUND_PC:
		//SEL[2]&=0x07;
		SEL[2]&=~SOUND_SELECT_IO13;//U38_P6 INH=0
		SEL[2]&=~SOUND_SELECT_IO1;
		SEL[2]|=SOUND_SELECT_IO2;
		break;
	case	DEV_SOUND_NPC:
		//SEL[2]&=0x07;
		SEL[2]&=~SOUND_SELECT_IO13;//U38_P6 INH=0
		SEL[2]|=SOUND_SELECT_IO1;
		SEL[2]|=SOUND_SELECT_IO2;
		break;
	default:break;
	}
SpiSendData(SEL,3);
Device_InVol(tmp,DEV_SOUND_DEV);
}

	
    /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void	PT2258Init(void)
{BYTE i;
uint8	tmp[2]={0};
*(tmp)=0xc0;
I2C0_WriteNByte(U37Add, ONE_BYTE_SUBA, 0, tmp, 1);//先清零PT2258所有寄存器
I2C0_WriteNByte(U42Add, ONE_BYTE_SUBA, 0, tmp, 1);//先清零PT2258所有寄存器

*(tmp)=0x81;                                      //初始化SGM6501
for(uint32 j = 1; j < 10; j++)
{
  I2C0_WriteNByte1(0x06, ONE_BYTE_SUBA, j, tmp, 1);
}
*(tmp)=0xff;
I2C0_WriteNByte1(0x06, ONE_BYTE_SUBA, 0x1D, tmp, 1);
*(tmp)=0x01;
I2C0_WriteNByte1(0x06, ONE_BYTE_SUBA, 0x1E, tmp, 1);

for (i=0;i<SOUND_OUT_CHANLE_SIZE;i++)
	{DEVICE_LINK.Sound_Out[i].Device=0;
	DEVICE_LINK.Sound_Out[i].MIX_CONTRL=0;
	DEVICE_LINK.Sound_Out[i].MIC_Vol[0]=0;
	DEVICE_LINK.Sound_Out[i].MIC_Vol[1]=0;
	DEVICE_LINK.Sound_Out[i].MIC_Vol[2]=0;
	DEVICE_LINK.Sound_Out[i].MIC_Vol[3]=0;
	DEVICE_LINK.Sound_Out[i].DEV_Vol=0;
	DEVICE_LINK.Sound_Out[i].LAN_Vol=0;
	}
//关闭mic，
	SOUND_DEVICE_Mute(VOL_CH_MIC1,TRUE); 
	SOUND_DEVICE_Mute(VOL_CH_MIC2,TRUE); 
	SOUND_DEVICE_Mute(VOL_CH_MIC3,TRUE); 
	SOUND_DEVICE_Mute(VOL_CH_MIC4,TRUE); 

	Device_InVol( 0,DEV_SOUND_MIC1);
	Device_InVol( 0,DEV_SOUND_MIC2);
	Device_InVol( 0,DEV_SOUND_MIC3);
	Device_InVol( 0,DEV_SOUND_MIC4);
	Device_InVol( 40,DEV_SOUND_DEV);
	Device_OutVol(75,DEV_SOUND_OUT_GF);
	Device_OutVol(95,DEV_SOUND_OUT_LMT);

}


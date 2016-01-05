#include  "includes.h" 
//void	VGA_Select(uint8	vga_in,uint8	VGAOUT,uint8	LinkType);

/*const uint8 AV_TAB[17]=
{
0,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,
0x10,0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09
};*/
VGA_OUT VGA_BUF ={0};
const uint8 AV_IN[VIDEO_IN_CHANLE_SIZE] = 
{
0,0x05,0,0x02,0x01,0x06,0x03,0x04,0,0,0,0,0,0,0,0,0
};
const uint8 AV_OUT[VIDEO_OUT_CHANLE_SIZE]=
{
0,0x07,0x06,0x01,0x02,0x03,0x04,0,0,0x08,0x05,0,0,0,0,0x09,0
};


uint8	BakAVYAdd=0;  
uint8	BakVGAOUT=0;
uint8	BakAV0YAdd=0;
extern	uint16	SEL[3];

struct LINK_REC_TABLE_TYPE DEVICE_LINK;
 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
struct LINK_REC_TABLE_TYPE *Get_DeviceSingleLink(void)
{
return &DEVICE_LINK;
}
 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void	AVCD22M3494Reset(void)
{
	BYTE i,j;
	SEL[1]|=AV_CS0;SEL[1]|=AV_CS1;SpiSendData(SEL,3);//cs=1
	SEL[1]|=AV_RST0;SEL[1]|=AV_RST1;SpiSendData(SEL,3);//r=1
	Delay(500);
	SEL[1]&=~AV_RST0;SEL[1]&=~AV_RST1;SpiSendData(SEL,3);//r=0
	SEL[1]&=~AV_CS0;SEL[1]&=~AV_CS1;SpiSendData(SEL,3);//cs=0
	SEL[1]&=~AV_DATA0;SEL[1]&=~AV_DATA1;SpiSendData(SEL,3);
	for (i=0;i<VIDEO_OUT_CHANLE_SIZE;i++)
		DEVICE_LINK.Video_Out[i]=0;
	for (i=1;i<17;i++)
		for (j=1;j<17;j++)
			AV_Select(j,i,FALSE);
		//Device_Video_Select(i,i);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////

void	VGACD22M3494Reset(void)
{BYTE i,j;
SEL[1]|=AV_CS2;SEL[0]|=AV_CS3;SpiSendData(SEL,3);//cs=1
SEL[0]|=AV_RST2;SEL[0]|=AV_RST3;SpiSendData(SEL,3);//r=1
Delay(500);
SEL[0]&=~AV_RST2;SEL[0]&=~AV_RST3;SpiSendData(SEL,3);//r=0
SEL[1]&=~AV_CS2;SEL[0]&=~AV_CS3;SpiSendData(SEL,3);//cs=0
SEL[0]&=~AV_DATA2;SEL[0]&=~AV_DATA3;//Init D=0
for (i=0;i<VGA_OUT_CHANLE_SIZE;i++)
	DEVICE_LINK.VGA_Out[i]=0;
for (i=1;i<5;i++)
	for (j=1;j<5;j++)
		VGA_Select(j,i, FALSE);
		//Device_VGA_Select(i,i);
}
 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8	ChangeX(uint8	x)
{
uint8	tmp=0;
if(15>=x>=0)
{
		if(x==0)tmp=0x00;
else		if(x==1)tmp=0x40;
else		if(x==2)tmp=0x20;
else		if(x==3)tmp=0x60;
else		if(x==4)tmp=0x10;
else		if(x==5)tmp=0x50;
else		if(x==6)tmp=0x08;
else		if(x==7)tmp=0x48;
else		if(x==8)tmp=0x28;
else		if(x==9)tmp=0x68;
else		if(x==10)tmp=0x18;
else		if(x==11)tmp=0x58;
else		if(x==12)tmp=0x30;
else		if(x==13)tmp=0x70;
else		if(x==14)tmp=0x38;
else		tmp=0x78;
return	tmp;
}
else	
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
uint8	ChangeY(uint8	y)
{
	uint8	tmp=0;
	uint8	y0=0;
	uint8	y1=0;
	uint8	y2=0;
	y0=(y&0x01)<<2;
	y1=(y&0x02);
	y2=(y&0x04)>>2;
	tmp=y0+y1+y2;
	return tmp;
}
  
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：LinkType=0 break,=1 link
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 AV_Get_chanle(void)
{
	return BakAV0YAdd;
}
 
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：LinkType=0 break,=1 link
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void AV_Select(uint8 in,uint8 out,uint8 LinkType)
{
  
  uint8 av_in = AV_IN[in];
  uint8 av_out = AV_OUT[out];
  //uint8 av_in = in;
  //uint8 av_out = out;
  uint8 s = av_in | (LinkType << 7);
  I2C0_WriteNByte1(0x06, ONE_BYTE_SUBA, (uint32)av_out, &s, 1);
}



/*void AV_Select(uint8 av_in,uint8 av_out,uint8 LinkType)
{uint8 tmpy;
 uint8	temp1,temp2;
		if ((av_out==0x0)||(av_in==0)) return;	
		if ((av_out>=VIDEO_OUT_CHANLE_SIZE)||(av_in>=VIDEO_IN_CHANLE_SIZE)) return;
		tmpy=AV_TAB[av_out];
   		temp1=0;temp2=0;
 		if ((av_out==DEV_VIDEO_OUT_TB0)||(av_out==DEV_VIDEO_OUT_TB1))
			BakAVYAdd=av_out;
		temp1=ChangeX(av_in-1);
	

	if (tmpy>8)
		{temp2=ChangeY(tmpy-9);
		SEL[1]|=AV_CS1;SpiSendData(SEL,3);//cs=1
		SEL[1]&=~0X007F;
		SEL[1]|=temp1;
		SEL[1]|=temp2;
		SpiSendData(SEL,3);//send		Addr
		Delay(50);		
		SEL[1]|=AV_STR1;SpiSendData(SEL,3);//ST=1
		Delay(50);
		if(LinkType==1)	{SEL[1]|=AV_DATA1;SpiSendData(SEL,3);}//if connect D=1
		else		{SEL[1]&=~AV_DATA1;SpiSendData(SEL,3);}//if break D=0
		Delay(50);
		SEL[1]&=~AV_STR1;SpiSendData(SEL,3);//ST=0
		SEL[1]&=~AV_CS1;SpiSendData(SEL,3);//CS=0	

		}
	else
		{	
		temp2=ChangeY(tmpy-1);
		SEL[1]|=AV_CS0;SpiSendData(SEL,3);//cs=1
		SEL[1]&=~0X007F;
		SEL[1]|=temp1;
		SEL[1]|=temp2;
		SpiSendData(SEL,3);//send		Addr
		Delay(50);		
		SEL[1]|=AV_STR0;SpiSendData(SEL,3);//ST=1
		Delay(50);
		if(LinkType==1)	{SEL[1]|=AV_DATA0;SpiSendData(SEL,3);}//if connect D=1
		else		{SEL[1]&=~AV_DATA0;SpiSendData(SEL,3);}//if break D=0
		Delay(50);
		SEL[1]&=~AV_STR0;SpiSendData(SEL,3);//ST=0
		SEL[1]&=~AV_CS0;SpiSendData(SEL,3);//CS=0	

		}


}*/
 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////

/*void	VGA0Sel(uint8	x,uint8 y,uint8	LinkType)
{
uint8	temp1,temp2;
temp1=0;temp2=0;
temp1=ChangeX(x);
temp2=ChangeY(y);
SEL[1]|=AV_CS2;SpiSendData(SEL,3);//cs=1
SEL[1]&=~0X007F;
SEL[1]|=temp1;
SEL[1]|=temp2;
SpiSendData(SEL,3);//send		Addr
Delay(100);		
SEL[0]|=AV_STR2;SpiSendData(SEL,3);//ST=1
Delay(100);
if(LinkType==1)	{SEL[0]|=AV_DATA2;SpiSendData(SEL,3);}//if connect D=1
else		{SEL[0]&=~AV_DATA2;SpiSendData(SEL,3);}//if break D=0
Delay(100);
SEL[0]&=~AV_STR2;SpiSendData(SEL,3);//ST=0
SEL[1]&=~AV_CS2;SpiSendData(SEL,3);//CS=0	
}*/
  /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////

/*void	VGA1Sel(uint8	x,uint8 y,uint8	LinkType)
{
uint8	temp1,temp2;
temp1=0;temp2=0;
temp1=ChangeX(x);
temp2=ChangeY(y);
SEL[0]|=AV_CS3;SpiSendData(SEL,3);//cs=1
SEL[1]&=~0X007F;
SEL[1]|=temp1;
SEL[1]|=temp2;
SpiSendData(SEL,3);//send		Addr
Delay(100);		
SEL[0]|=AV_STR3;SpiSendData(SEL,3);//ST=1
Delay(100);
if(LinkType==1)	{SEL[0]|=AV_DATA3;SpiSendData(SEL,3);}//if connect D=1
else		{SEL[0]&=~AV_DATA3;SpiSendData(SEL,3);}//if break D=0
Delay(100);
SEL[0]&=~AV_STR3;SpiSendData(SEL,3);//ST=0
SEL[0]&=~AV_CS3;SpiSendData(SEL,3);//CS=0	
}*/


//CH=VGA_PC/VGA-DVD_VGA/VGA_ZHANTAI/VGA_NPC
//VGAOUT=VGAOUTTB1/VGAOUTTB2/VGAOUTTYJ
//LinkType=1 is connect;LinkType=0 is break
 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 VGA_Get_chanle(void)
{
return BakVGAOUT;
}
  /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
////////////////////////////////
//VGA矩阵切换 2012.5.14
//in
//1课件电脑3视频展台4笔记本
//分别对应VGA输入0-4 5-9 10-14
//out
//1增益卡2无动作3投影机a投影机b绑定
//分别对应VGA输出6-10 1-5 11-15
//VGA_IN1_V  0    VGA_OUT1_V  9
//VGA_IN1_H  1    VGA_OUT1_H  10
//VGA_IN1_R  2    VGA_OUT1_R  6
//VGA_IN1_G  3    VGA_OUT1_G  8
//VGA_IN1_B  4    VGA_OUT1_B  7
//VGA_IN2_V  5    VGA_OUT2_V  4
//VGA_IN2_H  6    VGA_OUT2_H  5
//VGA_IN2_R  7    VGA_OUT2_R  1    MAX9675_BUF[0] 0-5位
//VGA_IN2_G  8    VGA_OUT2_G  3
//VGA_IN2_B  9    VGA_OUT2_B  2
//VGA_IN3_V  10   VGA_OUT3_V  14
//VGA_IN3_H  11   VGA_OUT3_H  15
//VGA_IN3_R  12   VGA_OUT3_R  11
//VGA_IN3_G  13   VGA_OUT3_G  13
//VGA_IN3_B  14   VGA_OUT3_B  12
//MAX9675_BUF定义
//MAX9675_BUF[0]
//
///////////////////////////////
void VGA_Select(uint8 in,uint8 out,uint8 LinkType)
{
  //bool type==LinkType ? 1:0;
  switch(in)
  {
    case DEV_VGA_PC://计算机
      if(out == DEV_VGA_OUT_TB0)//增益卡
      {
        VGA_BUF.VGA_OUT_9_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_9_GAIN=1;                //2012.7.16 VH幅值改为1 change by liyang
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_10_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_10_GAIN=1;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_6_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_6_GAIN=1;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_8_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_8_GAIN=1;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_7_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_7_GAIN=1;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_3 = 0;
        SpiSendData1((uint8 *)&VGA_BUF, sizeof(VGA_BUF));
        break;
      }
      else if(out == DEV_VGA_OUT_TYJ)//投影机A和B
      {
        ////////////////////////////TYJA////////////////
        VGA_BUF.VGA_OUT_14_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_14_GAIN=1;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_15_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_15_GAIN=1;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_11_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_11_GAIN=1;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_13_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_13_GAIN=1;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_12_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_12_GAIN=1;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_3 = 0;
        //SpiSendData1((uint8 *)&VGA_BUF, sizeof(VGA_BUF));
        ////////////////////////TYJB/////////////////
        VGA_BUF.VGA_OUT_4_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_4_GAIN=1;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_5_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_5_GAIN=1;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_1_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_1_GAIN=1;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_3_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_3_GAIN=1;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_2_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_2_GAIN=1;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_3 = 0;
        SpiSendData1((uint8 *)&VGA_BUF, sizeof(VGA_BUF));
        break;
      }
  case DEV_VGA_SHOW: //视频展台
    if(out == DEV_VGA_OUT_TB0)
      {
        VGA_BUF.VGA_OUT_9_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_9_GAIN=1;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_10_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_10_GAIN=1;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_6_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_6_GAIN=1;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_8_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_8_GAIN=1;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_7_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_7_GAIN=1;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_3 = 1;
        SpiSendData1((uint8 *)&VGA_BUF, sizeof(VGA_BUF));
        break;
      }
      else if(out == DEV_VGA_OUT_TYJ)
      {
        ////////////////////////TYJA//////////////////
        VGA_BUF.VGA_OUT_14_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_14_GAIN=1;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_15_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_15_GAIN=1;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_11_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_11_GAIN=1;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_13_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_13_GAIN=1;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_12_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_12_GAIN=1;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_3 = 1;
        //SpiSendData1((uint8 *)&VGA_BUF, sizeof(VGA_BUF));
        ////////////////////////TYJB/////////////////
        VGA_BUF.VGA_OUT_4_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_4_GAIN=1;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_5_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_5_GAIN=1;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_1_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_1_GAIN=1;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_0 = 0;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_3_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_3_GAIN=1;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_2_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_2_GAIN=1;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_3 = 1;
        SpiSendData1((uint8 *)&VGA_BUF, sizeof(VGA_BUF));
        break;
      }
  case DEV_VGA_NPC://笔记本
    if(out == DEV_VGA_OUT_TB0)
      {
        VGA_BUF.VGA_OUT_9_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_9_GAIN=1;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_9_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_10_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_10_GAIN=1;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_10_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_6_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_6_GAIN=1;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_6_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_8_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_8_GAIN=1;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_8_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_7_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_7_GAIN=1;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_7_VGA_IN_ADDR_3 = 0;
        SpiSendData1((uint8 *)&VGA_BUF, sizeof(VGA_BUF));
        break;
      }
      else if(out == DEV_VGA_OUT_TYJ)
      {
        ////////////////////////TYJA//////////////////
        VGA_BUF.VGA_OUT_14_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_14_GAIN=1;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_14_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_15_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_15_GAIN=1;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_15_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_11_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_11_GAIN=1;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_11_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_13_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_13_GAIN=1;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_13_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_12_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_12_GAIN=1;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_12_VGA_IN_ADDR_3 = 0;
        //SpiSendData1((uint8 *)&VGA_BUF, sizeof(VGA_BUF));
        ////////////////////////TYJB/////////////////
        VGA_BUF.VGA_OUT_4_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_4_GAIN=1;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_4_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_5_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_5_GAIN=1;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_1 = 0;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_5_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_1_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_1_GAIN=1;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_1_VGA_IN_ADDR_3 = 0;
        VGA_BUF.VGA_OUT_3_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_3_GAIN=1;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_2 = 0;
        VGA_BUF.VGA_OUT_3_VGA_IN_ADDR_3 = 1;
        VGA_BUF.VGA_OUT_2_ENABLE=LinkType;
        VGA_BUF.VGA_OUT_2_GAIN=1;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_0 = 1;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_1 = 1;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_2 = 1;
        VGA_BUF.VGA_OUT_2_VGA_IN_ADDR_3 = 0;
        SpiSendData1((uint8 *)&VGA_BUF, sizeof(VGA_BUF));
        break;
      }     
  }
  
}


/*void	VGA_Select(uint8	vga_in,uint8	VGAOUT,uint8	LinkType)
{

if ((VGAOUT==DEV_VGA_OUT_TB0)||(VGAOUT==DEV_VGA_OUT_TB1))
	BakVGAOUT=VGAOUT;
if(vga_in==DEV_VGA_PC)//计算机
	{
		if(VGAOUT==DEV_VGA_OUT_TB0)	//同步0
			{
				VGA0Sel(VGA_PC_R,VGAOUTTB1_R,LinkType);//R
				VGA0Sel(VGA_PC_G,VGAOUTTB1_G,LinkType);//G
				VGA1Sel(VGA_PC_B,VGAOUTTB1_B,LinkType);//B
				VGA0Sel(8,VGAOUTTB1_H,LinkType);//H
				VGA0Sel(12,VGAOUTTB1_V,LinkType);//V
			}
		if(VGAOUT==DEV_VGA_OUT_TB1)	//同步1				
			{
				VGA0Sel(VGA_PC_R,VGAOUTTB2_R,LinkType);//R
				VGA0Sel(VGA_PC_G,VGAOUTTB2_G,LinkType);//G
				VGA1Sel(VGA_PC_B,VGAOUTTB2_B,LinkType);//B
				VGA1Sel(0,VGAOUTTB2_H,LinkType);//H
				VGA1Sel(4,VGAOUTTB2_V,LinkType);//V
			}
		if(VGAOUT==DEV_VGA_OUT_TYJ)	//投影机
			{
				VGA0Sel(VGA_PC_R,VGAOUTTYJ_R,LinkType);//R
				VGA0Sel(VGA_PC_G,VGAOUTTYJ_G,LinkType);//G
				VGA1Sel(VGA_PC_B,VGAOUTTYJ_B,LinkType);//B
				VGA1Sel(0,VGAOUTTYJ_H,LinkType);//H
				VGA1Sel(4,VGAOUTTYJ_V,LinkType);//V
			}
	}
if(vga_in==DEV_VGA_DVD)
	{
		if(VGAOUT==DEV_VGA_OUT_TB0)	
			{
				VGA0Sel(VGA_DVD_R,VGAOUTTB1_R,LinkType);//R
				VGA0Sel(VGA_DVD_G,VGAOUTTB1_G,LinkType);//G 
				VGA1Sel(VGA_DVD_B,VGAOUTTB1_B,LinkType);//B
				VGA0Sel(9,VGAOUTTB1_H,LinkType);//H
				VGA0Sel(13,VGAOUTTB1_V,LinkType);//V
			}
		if(VGAOUT==DEV_VGA_OUT_TB1)	
			{
				VGA0Sel(VGA_DVD_R,VGAOUTTB2_R,LinkType);//R
				VGA0Sel(VGA_DVD_G,VGAOUTTB2_G,LinkType);//G
				VGA1Sel(VGA_DVD_B,VGAOUTTB2_B,LinkType);//B
				VGA1Sel(1,VGAOUTTB2_H,LinkType);//H
				VGA1Sel(5,VGAOUTTB2_V,LinkType);//V
			}
		if(VGAOUT==DEV_VGA_OUT_TYJ)	
			{
				VGA0Sel(VGA_DVD_R,VGAOUTTYJ_R,LinkType);//R
				VGA0Sel(VGA_DVD_G,VGAOUTTYJ_G,LinkType);//G
				VGA1Sel(VGA_DVD_B,VGAOUTTYJ_B,LinkType);//B
				VGA1Sel(1,VGAOUTTYJ_H,LinkType);//H
				VGA1Sel(5,VGAOUTTYJ_V,LinkType);//V
			}	
	}

if(vga_in==DEV_VGA_SHOW)
	{
		if(VGAOUT==DEV_VGA_OUT_TB0)	
			{
				VGA0Sel(VGA_ZHANTAI_R,VGAOUTTB1_R,LinkType);//R
				VGA0Sel(VGA_ZHANTAI_G,VGAOUTTB1_G,LinkType);//G 
				VGA1Sel(VGA_ZHANTAI_B,VGAOUTTB1_B,LinkType);//B
				VGA0Sel(10,VGAOUTTB1_H,LinkType);//H
				VGA0Sel(14,VGAOUTTB1_V,LinkType);//V
			}
		if(VGAOUT==DEV_VGA_OUT_TB1)	
			{
				VGA0Sel(VGA_ZHANTAI_R,VGAOUTTB2_R,LinkType);//R
				VGA0Sel(VGA_ZHANTAI_G,VGAOUTTB2_G,LinkType);//G
				VGA1Sel(VGA_ZHANTAI_B,VGAOUTTB2_B,LinkType);//B
				VGA1Sel(2,VGAOUTTB2_H,LinkType);//H
				VGA1Sel(6,VGAOUTTB2_V,LinkType);//V
			}
		if(VGAOUT==DEV_VGA_OUT_TYJ)	
			{
				VGA0Sel(VGA_ZHANTAI_R,VGAOUTTYJ_R,LinkType);//R
				VGA0Sel(VGA_ZHANTAI_G,VGAOUTTYJ_G,LinkType);//G
				VGA1Sel(VGA_ZHANTAI_B,VGAOUTTYJ_B,LinkType);//B
				VGA1Sel(2,VGAOUTTYJ_H,LinkType);//H
				VGA1Sel(6,VGAOUTTYJ_V,LinkType);//V
			}
	}
if(vga_in==DEV_VGA_NPC)
	{
		if(VGAOUT==DEV_VGA_OUT_TB0)	
			{
				VGA0Sel(VGA_NPC_R,VGAOUTTB1_R,LinkType);//R
				VGA0Sel(VGA_NPC_G,VGAOUTTB1_G,LinkType);//G 
				VGA1Sel(VGA_NPC_B,VGAOUTTB1_B,LinkType);//B
				VGA0Sel(11,VGAOUTTB1_H,LinkType);//H
				VGA0Sel(15,VGAOUTTB1_V,LinkType);//V
			}
		if(VGAOUT==DEV_VGA_OUT_TB1)	
			{
				VGA0Sel(VGA_NPC_R,VGAOUTTB2_R,LinkType);//R
				VGA0Sel(VGA_NPC_G,VGAOUTTB2_G,LinkType);//G
				VGA1Sel(VGA_NPC_B,VGAOUTTB2_B,LinkType);//B
				VGA1Sel(3,VGAOUTTB2_H,LinkType);//H
				VGA1Sel(7,VGAOUTTB2_V,LinkType);//V
			}
		if(VGAOUT==DEV_VGA_OUT_TYJ)	
			{
				VGA0Sel(VGA_NPC_R,VGAOUTTYJ_R,LinkType);//R
				VGA0Sel(VGA_NPC_G,VGAOUTTYJ_G,LinkType);//G
				VGA1Sel(VGA_NPC_B,VGAOUTTYJ_B,LinkType);//B
				VGA1Sel(3,VGAOUTTYJ_H,LinkType);//H
				VGA1Sel(7,VGAOUTTYJ_V,LinkType);//V
			}
	}
//SEL[0]&=~AV_CS3;SEL[1]&=~AV_CS2;SpiSendData(SEL,3);//CS=0	
}*/
   /////////////////////////////////////////////////////////////////////
  //  函数名称：
  //  函数功能：
  //  入口参数：
  //		  
  //		  
  //  出口参数：无
  ////////////////////////////////////////////////////////////////////
uint8 Change_Encode_Num(uint8 No)  
{uint8 tmp;
switch(No)
	{case 1:tmp=8;break;
	 case 2:tmp=7;break;
	 case 3:tmp=6;break;
	 case 4:tmp=5;break;
	 case 5:tmp=4;break;
	 case 6:tmp=3;break;
	 case 15:tmp=10;break;
	 case 16:tmp=9;break;
	 default:tmp=0;
	}
return tmp;
}
 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_Video_Select(BYTE in,BYTE out)
{       uint8 i;
	if ((in>VIDEO_IN_CHANLE_SIZE)||(out>VIDEO_OUT_CHANLE_SIZE)) return;
	if ((in==0)||(out==0)) return;
	if (DEVICE_LINK.Video_Out[out]==in)
		AV_Select(in,out,TRUE);
	else
		{AV_Select(DEVICE_LINK.Video_Out[out], out,FALSE);
		 AV_Select(in, out,TRUE);	
		 DEVICE_LINK.Video_Out[out]=in;
		}
        //////////
        if(in == DEV_VIDEO_LXJ)
        {
          for(i = 0; i < 3; i++)
            Switch_HDMI(IN_ZBFWQ, i);
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
 void Device_Video_Off(BYTE out)
 {uint8 tmp;
	 if ((out==0)||(out>VIDEO_OUT_CHANLE_SIZE)) return;
	 for (tmp=1;tmp<VIDEO_OUT_CHANLE_SIZE;tmp++)
	 	AV_Select(tmp, out,FALSE);
	 DEVICE_LINK.Video_Out[out]=0;
	 
 }
 
  /////////////////////////////////////////////////////////////////////
 //  函数名称：
 //  函数功能：
 //  入口参数：
 // 		 
 // 		 
 //  出口参数：无
 ////////////////////////////////////////////////////////////////////
 void Device_VGA_Off(BYTE out)
 {BYTE tmp;
	 if ((out==0)||(out>=VGA_OUT_CHANLE_SIZE)) return;	  
	 for (tmp=1;tmp<VGA_OUT_CHANLE_SIZE;tmp++)
		  VGA_Select(tmp, out,FALSE);
	  DEVICE_LINK.VGA_Out[out-1]=0;
 }
 /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_VGA_Select(BYTE in,BYTE out)
{
      uint8 tmp_in = 0,i = 0;
	if ((in>VGA_IN_CHANLE_SIZE)||(out>=VGA_OUT_CHANLE_SIZE)) return;
	//if (out==DEV_VGA_OUT_TB1) return;
	if ((in==0)||(out==0)) return;
	if (DEVICE_LINK.VGA_Out[out-1]==in)
		VGA_Select(in, out,TRUE);
	else
		{VGA_Select(DEVICE_LINK.VGA_Out[out-1], out,FALSE);
		 VGA_Select(in, out,TRUE);	
		 DEVICE_LINK.VGA_Out[out-1]=in;
		}
        ///////////////////外置DVI切换器，红外学码
        Switch_DVI(in);//add 2016.1.5
        //////////HDMI矩阵
        if(in == DEV_VGA_PC ) tmp_in = IN_PC;
        if(in == DEV_VGA_NPC) tmp_in = IN_NPC;
        if(in == DEV_VGA_SHOW) tmp_in= IN_ZJ;        
        for(i = 0; i < 4; i++)
          Switch_HDMI(tmp_in,i);
        
}
  /////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_Link_Scan(void)
{BYTE i,tmp;

 //扫描视频设备 
/* 
for (i=0;i<VIDEO_OUT_CHANLE_SIZE;i++)
	{if(DEVICE_LINK.Video_Out[i]!=0)
		{
		 AV_Select(DEVICE_LINK.Video_Out[i],i,TRUE);
		}
	}
	*/
//扫描VGA设备
for (i=0;i<VGA_OUT_CHANLE_SIZE;i++)
{if(DEVICE_LINK.VGA_Out[i]!=0)
 	VGA_Select(DEVICE_LINK.VGA_Out[i],i+1,TRUE);
}

}

#ifndef __CD22M3494_H
#define __CD22M3494_H


//低16位切换音频
//#define SOUND_DVD	0x100F
//#define SOUND_REC	0x100A
//#define SOUND_PC	0x1005
//#define SOUND_NPC	0x1000
/*
#define X0	0x0040
#define X1	0x0020
#define X2	0x0010
#define X3	0x0008
#define Y0	0x0004
#define Y1	0x0002
#define Y2	0x0001
*/

#define AV_CS0		0x0080
#define AV_DATA0	0x0100
#define AV_RST0		0x0200
#define AV_STR0		0x0400
#define AV_CS1		0x0800

#define AV_DATA1	0x1000
#define AV_RST1		0x2000
#define AV_STR1		0x4000
#define AV_CS2		0x8000

#define AV_DATA2	0x0001
#define AV_RST2		0x0002
#define AV_STR2		0x0004
#define AV_CS3		0x0008

#define AV_DATA3	0x0010
#define AV_RST3		0x0020
#define AV_STR3		0x0040


#define DEV_DVD		0x01
#define DEV_REC		0x02
#define DEV_PC		0x03
#define DEV_NPC		0x04
//////////////////////////////////////
#define	VGA_PC_R			0
#define	VGA_PC_G			4
#define	VGA_PC_B			8

#define	VGA_DVD_R	1
#define	VGA_DVD_G	5
#define	VGA_DVD_B	9

#define	VGA_ZHANTAI_R	2
#define	VGA_ZHANTAI_G	6
#define	VGA_ZHANTAI_B	10

#define	VGA_NPC_R		3
#define	VGA_NPC_G		7
#define	VGA_NPC_B		11

#define	VGAOUTTB1_R		7
#define	VGAOUTTB1_G		4
#define	VGAOUTTB1_B		3
#define	VGAOUTTB1_H		1
#define	VGAOUTTB1_V		0

#define	VGAOUTTB2_R		6
#define	VGAOUTTB2_G		3
#define	VGAOUTTB2_B		2
#define	VGAOUTTB2_H		5
#define	VGAOUTTB2_V		7

#define	VGAOUTTYJ_R		5
#define	VGAOUTTYJ_G		2
#define	VGAOUTTYJ_B		1
#define	VGAOUTTYJ_H	       4
#define	VGAOUTTYJ_V		6
#if 0
#define DEV_SOUND_OUT_NONE			0x0 //音频输出无定义
#define DEV_SOUND_OUT_GF				0x1 //功放输出
#define DEV_SOUND_OUT_LMT				0x2//流媒体输出
#define DEV_SOUND_OUT_YSQ				0x3//扬声器输出
#define DEV_SOUND_OUT_BMK				0x4//编码卡输出
#endif
#define SMIX_CONTRLMIC1   	1		//=1mic1静音
#define SMIX_CONTRLMIC2   	(1<<1)	//=1mic2静音
#define SMIX_CONTRLMIC3   	(1<<2)	//=1mic3静音
#define SMIX_CONTRLMIC4   	(1<<3)	//=1mic4静音
#define SMIX_DEVICE   		(1<<4)	//=1设备静音
#define SMIX_OUT_GF   		(1<<5)	//=1功放设备静音
#define SMIX_OUT_LMT   		(1<<6)	//=1l设备静音
#define SMIX_LAN_DEVICE  	(1<<7)	//=1网络音频静音

#define SMIX_BMK_IN_SELEC   	(1<<8)  //==1 选择面板MIC==0选择功放
#define SMIX_LAN_OUT_SELEC (1<<9)  //==1 选择网络音频输出到面板
//==0 选择网络音频输出到媒体和功放
#define SMIX_LAN_IN_SELEC0 (1<<12)
#define SMIX_LAN_IN_SELEC1 (1<<13)
//==0 网络音频输入选择MIC2
//==1 网络音频输入选择面板MIC
//==2 网络音频输入选择媒体输出
//==3 网络音频输入选择功放输出

typedef struct MAX9675_VGA_OUT
{
    uint8 VGA_OUT_14_GAIN: 1;
    uint8 VGA_OUT_14_ENABLE: 1;
    uint8 VGA_OUT_15_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_15_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_15_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_15_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_15_GAIN: 1;
    uint8 VGA_OUT_15_ENABLE: 1;

    uint8 VGA_OUT_13_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_13_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_13_GAIN: 1;
    uint8 VGA_OUT_13_ENABLE: 1;
    uint8 VGA_OUT_14_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_14_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_14_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_14_VGA_IN_ADDR_0: 1;

    uint8 VGA_OUT_12_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_12_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_12_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_12_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_12_GAIN: 1;
    uint8 VGA_OUT_12_ENABLE: 1;
    uint8 VGA_OUT_13_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_13_VGA_IN_ADDR_2: 1;

    uint8 VGA_OUT_10_GAIN: 1;
    uint8 VGA_OUT_10_ENABLE: 1;
    uint8 VGA_OUT_11_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_11_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_11_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_11_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_11_GAIN: 1;
    uint8 VGA_OUT_11_ENABLE: 1;

    uint8 VGA_OUT_9_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_9_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_9_GAIN: 1;
    uint8 VGA_OUT_9_ENABLE: 1;
    uint8 VGA_OUT_10_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_10_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_10_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_10_VGA_IN_ADDR_0: 1;

    uint8 VGA_OUT_8_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_8_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_8_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_8_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_8_GAIN: 1;
    uint8 VGA_OUT_8_ENABLE: 1;
    uint8 VGA_OUT_9_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_9_VGA_IN_ADDR_2: 1;

    uint8 VGA_OUT_6_GAIN: 1;
    uint8 VGA_OUT_6_ENABLE: 1;
    uint8 VGA_OUT_7_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_7_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_7_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_7_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_7_GAIN: 1;
    uint8 VGA_OUT_7_ENABLE: 1;

    uint8 VGA_OUT_5_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_5_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_5_GAIN: 1;
    uint8 VGA_OUT_5_ENABLE: 1;
    uint8 VGA_OUT_6_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_6_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_6_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_6_VGA_IN_ADDR_0: 1;

    uint8 VGA_OUT_4_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_4_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_4_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_4_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_4_GAIN: 1;
    uint8 VGA_OUT_4_ENABLE: 1;
    uint8 VGA_OUT_5_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_5_VGA_IN_ADDR_2: 1;

    uint8 VGA_OUT_2_GAIN: 1;
    uint8 VGA_OUT_2_ENABLE: 1;
    uint8 VGA_OUT_3_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_3_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_3_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_3_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_3_GAIN: 1;
    uint8 VGA_OUT_3_ENABLE: 1;

    uint8 VGA_OUT_1_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_1_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_1_GAIN: 1;
    uint8 VGA_OUT_1_ENABLE: 1;
    uint8 VGA_OUT_2_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_2_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_2_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_2_VGA_IN_ADDR_0: 1;

    uint8 VGA_OUT_0_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_0_VGA_IN_ADDR_2: 1;
    uint8 VGA_OUT_0_VGA_IN_ADDR_1: 1;
    uint8 VGA_OUT_0_VGA_IN_ADDR_0: 1;
    uint8 VGA_OUT_0_GAIN: 1;
    uint8 VGA_OUT_0_ENABLE: 1;
    uint8 VGA_OUT_1_VGA_IN_ADDR_3: 1;
    uint8 VGA_OUT_1_VGA_IN_ADDR_2: 1;
} VGA_OUT;

struct SOUND_REC_LINK_TYPE
{
    uint16 MIX_CONTRL;     //混音控制
    uint8 Device;			//设备选择
    uint8 LAN_Vol;		//网络音频输出音量
    uint8 DEV_Vol;		//设备音量
    uint8 OUT_Vol;		//输出音量
    uint8 MIC_Vol[4];   	//mic音量

};
#define VIDEO_OUT_CHANLE_SIZE  		17
#define VIDEO_IN_CHANLE_SIZE  		17
#define VGA_OUT_CHANLE_SIZE 		5
#define VGA_IN_CHANLE_SIZE 			5
#define SOUND_OUT_CHANLE_SIZE 		5
#define SOUND_IN_CHANLE_SIZE 		16

struct LINK_REC_TABLE_TYPE
{
    uint8 VGA_Out[VGA_OUT_CHANLE_SIZE];
    uint8 Video_Out[VIDEO_OUT_CHANLE_SIZE];
    struct SOUND_REC_LINK_TYPE Sound_Out[SOUND_OUT_CHANLE_SIZE];

};
///////////////////////////////////////////////////////
void	AVCD22M3494Reset(void);
void	VGACD22M3494Reset(void);
void Device_Video_Select(BYTE in, BYTE out);
void Device_VGA_Select(BYTE in, BYTE out);
void Device_Link_Scan(void);
uint8 AV_Get_chanle(void);
//void AV_Select(uint8 av_in,uint8 av_out,uint8 LinkType);
void AV_Select(uint8 in, uint8 out, uint8 LinkType);
uint8 VGA_Get_chanle(void);
//void	VGA_Selelct(uint8	vga_in,uint8	VGAOUT,uint8	LinkType);
void VGA_Select(uint8 in, uint8 out, uint8 LinkType);

uint8 Change_Encode_Num(uint8 No);
void Device_Video_Off(BYTE out);
void Device_VGA_Off(BYTE out);
void Device_Vol_Scan(void);

#endif

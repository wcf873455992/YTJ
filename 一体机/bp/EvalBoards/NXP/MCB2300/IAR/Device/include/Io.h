#ifndef __Io_H
#define __Io_H
#define IO_PORT0			0x0000
#define IO_PORT1			0x0100
#define IO_PORT2			0x0200
#define IO_PORT3			0x0300
//定义输入
#define IN_ALARM1 		IO_PORT1+23
#define IN_ALARM2 		IO_PORT1+24
#define IN_ALARM3 		IO_PORT2+2
#define IN_ALARM4 		IO_PORT2+3
#define IN_ALARM5 		IO_PORT2+4
#define IN_ALARM6 		IO_PORT2+5
#define IN_PC1POWER		IO_PORT2+6
#define IN_PC2POWER		IO_PORT2+7
#define IN_TYJVGA			IO_PORT2+9
#define IN_PC_CHECK			IO_PORT1+28
#define IN_NPC_CHECK		IO_PORT2+12
//定义输出
#define OUT_ALARM1		IO_PORT1+30
#define OUT_ALARM2		IO_PORT0+29
#define OUT_ALARM3		IO_PORT0+30
#define OUT_PC1POWER	IO_PORT2+8
#define OUT_PC2POWER	IO_PORT0+16
#define OUT_DCS			IO_PORT1+31
#define OUT_165PL			IO_PORT3+25  //165邋PL
#define OUT_M4			IO_PORT3+26
#define OUT_POWER_DOG		IO_PORT0+19
#define OUT_HW			IO_PORT1+25
#define OUT_LED			IO_PORT2+10
#define OUT_AIC23_CS		IO_PORT1+20
#define OUT_AIC23_MODE		IO_PORT0+20
#define OUT_KUOZHAN_RST	IO_PORT0+21
#define OUT_IC_RST			IO_PORT0+22//(1<<22)	//P0.22
#define OUT_HC595_CS		IO_PORT1+19//(1<<19)	//P1.19
#define OUT_165EN			IO_PORT1+21//(1<<21)	//P1.21
#define OUT_IS752_CS2		IO_PORT1+22//(1<<22)	//P1.22
//#define OUT_MEM_CS			IO_PORT1+28//(1<<28)	//P1.28
#define OUT_MEM_HOLD		IO_PORT1+29//(1<<29)	//P1.29
#define OUT_POWER_M3		IO_PORT1+18
#define OUT_POWER_M4		IO_PORT2+13

/*6路报警输入*/
#define	ALARM1_IN	(1<<23)	//P1.23
#define	ALARM2_IN	(1<<24)	//P1.24
#define	ALARM3_IN	(1<<2)	//P2.2
#define	ALARM4_IN	(1<<3)	//P2.3
#define	ALARM5_IN	(1<<4)	//P2.4
#define	ALARM6_IN	(1<<5)	//P2.5

/*2路PC电源检测输入*/
#define	PC1POWER_IN	(1<<6)	//P2.6
#define	PC2POWER_IN	(1<<7)	//P2.7

/*1路投影机连线检测输入*/
#define	TYJVGA_IN	(1<<9)//P2.9

/*3路报警输出*/
#define	ALARM1_OUT	(1<<30)	//P1.30
#define	ALARM2_OUT	(1<<29)	//P0.29
#define	ALARM3_OUT	(1<<30)	//P0.30

/*2路PC电源控制输出*/
#define	PC1POWER_OUT	(1<<8)	//P2.8
#define	PC2POWER_OUT	(1<<16)	//P016

/*4路可控电源输出*/
#define	CTRL1_OUT	(1<<13)	//P2.13
#define	CTRL2_OUT	(1<<18)	//P1.18
#define	CTRL3_OUT	(1<<25)	//P3.25
#define	CTRL4_OUT	(1<<26)	//P3.26

/*1路电磁锁控制输出*/
#define	DCS_OUT	(1<<31)	//P1.31

/*1路红外输出*/
#define	HW_OUT		(1<<25)	//P1.25

/*1指示灯输出*/
#define	LED_OUT		(1<<10)	//P2.10

#define	POWER_DOG		(1<<19)	//P0.19
#define	AIC23_CS		(1<<20)	//P1.20
#define	AIC23_MODE		(1<<20)	//P0.20
#define	KUOZHAN_RST	(1<<21)	//P0.21
#define	IC_RST			(1<<22)	//P0.22
#define	HC595_CS		(1<<19)	//P1.19
#define	IS752_CS1		(1<<21)	//P1.21
#define	IS752_CS2		(1<<22)	//P1.22
#define	MEM_CS			(1<<28)	//P1.28
#define	MEM_HOLD		(1<<29)	//P1.29

//测试IO位定义

#define	TEST_ALARM1	0x01
#define	TEST_ALARM2	0x02
#define	TEST_ALARM3	0x04
#define	TEST_ALARM4	0x08
#define	TEST_ALARM5	0x10
#define	TEST_ALARM6	0x20
#define	TEST_PC1POWER	0x40
#define	TEST_PC2POWER	0x80
//#define	PROJECT

//MAX9675
#define MAX9675_UPDATA  (1<<26)   //P3.26
#define MAX9675_CE      (1<<28)   //P1.28
#define MAX9675_ADDR    0x1F

//读卡器电源
#define ICREADER_POWER  (1<<26)   //P0.26
#define ICREADER_POWER_ON  1
#define ICREADER_POWER_OFF 0

void	Io_Init(void);
void	PINInit(void);
uint8 IO_in(uint32 in);
void IO_Out(uint32  out, uint8 type);
void IO_PC_Power(BYTE device, BYTE option);
uint32  IO_Scan(void);
BYTE IO_Contrl(BYTE Device, BYTE Option);
void Device_IC_Reset(void);
void Device_KZ_Reset(void);
void Device_DCS_open(void);
void IO_FlagLingt_Flash(void);
//liyang 2012.3.16添加指纹识别//////////
void EINT3_Exception(void);
void Timer3Init(void);
void Timer3_Exception(void);
/////////////////////////
void ICReader_Power(uint8 type);

#endif

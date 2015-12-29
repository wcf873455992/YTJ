#ifndef __USER_POWER_H
#define  __USER_POWER_H

#define POWER_CHANLE_VER  		0
#define POWER_CHANLE_CONTRL 	1
#define POWER_CHANLE_SET	 	2
#define POWER_ACTIVE			0x80000000
//设备连接定义-----电源控制箱
#define DEV_POWER_NONE				0x00 //电源箱无定义
#define DEV_POWER_DEVICE			0x01//电源箱设备
#define DEV_POWER_LMTPC				0x02//电源箱流媒体
#define DEV_POWER_SHOW				0x03//电源箱展台
#define DEV_POWER_CRT				0x04//电源箱显示器
#define DEV_POWER_PC				0x05//电源箱计算机
#define DEV_POWER_TYJ				0x06//电源箱投影机
#define DEV_POWER_LIGHT				0x07//电源箱灯光
#define DEV_POWER_GF				0x08//电源箱功放
#define DEV_POWER_SXJ				0x09//电源箱摄象机
#define DEV_POWER_TYM				0x0a//电源箱投影木
#define DEV_POWER_CL				0x0b//电源箱窗帘
#define DEV_POWER_LCD				0x0c//电源箱液晶
#define DEV_POWER_TYJUP_DOWN		0x0d//电源箱吊架

//定义设备关机延时
#define DEV_TIME_PC					60
#define DEV_TIME_LMTPC				60
#define DEV_TIME_GF					0
#define DEV_TIME_LIGHT				0
#define DEV_TIME_TYJ				180
#define DEV_TIME_TYM				20
#define DEV_TIME_TYJDJ			20
#define DEV_TIME_LCD				20
//定义状态
#define DEV_POWER_BUSY			0x80
#define DEV_POWER_RUN			0x40
#define DEV_POWER_MASK			0x0f

#define POWER_FLAG_PC			1
#define POWER_FLAG_LMT			(1<<1)
#define POWER_FLAG_LIGHT		(1<<2)
#define POWER_FLAG_TYJA			(1<<3)
#define POWER_FLAG_GF			(1<<4)
#define POWER_FLAG_TYM			(1<<5)
#define POWER_FLAG_LCD			(1<<6)
#define POWER_FLAG_TYJDJ		(1<<7)
#define POWER_FLAG_CL			(1<<8)
#define POWER_FLAG_TYJB			(1<<9)
//定义设备关机重复次数
#define DEV_REPEAT_TIMES_PC			3
#define DEV_REPEAT_TIMES_LMTPC	        2

void POWER_init(void);
void POWER_Set(uint8 contrl_io, uint8 option);
void POWER_msg_rev(uint8 *pdata, uint8 sub);
uint32 ON_POWER_option(void *pdata);
uint32 On_MSG_OPTION_POWERCARD_ONOFF(void *pdata);
void LED_SendFLASH(void);
void Power_Beep(uint16 DelayTime);
uint8 LMT_Power_contrl(uint8 Device, uint8 option);
extern uint32 POWER_FLAG_STATE;

#endif

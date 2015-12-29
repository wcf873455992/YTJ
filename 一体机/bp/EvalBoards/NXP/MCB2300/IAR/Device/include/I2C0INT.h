/*
*********************************************************************************************************
** 文件名称 ： I2CINT.h
** 功能说明 ： LPC2000系列芯片硬件I2C软件包。
** 使用说明 ： 主程序要配置好I2C总线接口(I2C引脚功能和I2C中断，并已使能I2C主模式)
*********************************************************************************************************
*/

#ifndef  I2CINT_H
#define  I2CINT_H

#define	AT24C512	0XA0

#define	ONE_BYTE_SUBA	1
#define TWO_BYTE_SUBA	2
#define X_ADD_8_SUBA	3

uint8 IRcvByte0(uint8 sla, uint8 *dat);
uint8 ISendByte0(uint8 sla, uint8 dat);
uint8 I2C0_ReadNByte (uint8 sla, uint32 suba_type, uint32 suba, uint8 *s, uint32 num);
uint8 I2C0_WriteNByte(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num);
void   I2C0_Exception(void);
void I2c0Init(uint32 Fi2c);
//////////////////////////
uint8 I2C0_WriteNByte1(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num);
////////////////////////


#endif



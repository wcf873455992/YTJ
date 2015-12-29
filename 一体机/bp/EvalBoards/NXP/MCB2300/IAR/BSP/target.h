#ifndef __TARGET_H
#define __TARGET_H

#ifdef __cplusplus
extern "C" {
#endif


#define  PCLK_WDT                         0
#define  PCLK_TIMER0                      1
#define  PCLK_TIMER1                      2
#define  PCLK_UART0                       3
#define  PCLK_UART1                       4
#define  PCLK_PWM0                        5
#define  PCLK_PWM1                        6
#define  PCLK_I2C0                        7
#define  PCLK_SPI                         8
#define  PCLK_RTC                         9
#define  PCLK_SSP1                       10
#define  PCLK_DAC                        11
#define  PCLK_ADC                        12
#define  PCLK_CAN1                       13
#define  PCLK_CAN2                       14
#define  PCLK_ACF                        15
#define  PCLK_BAT_RAM                    16
#define  PCLK_GPIO                       17
#define  PCLK_PCB                        18
#define  PCLK_I2C1                       19
#define  PCLK_SSP0                       21
#define  PCLK_TIMER2                     22
#define  PCLK_TIMER3                     23
#define  PCLK_UART2                      24
#define  PCLK_UART3                      25
#define  PCLK_I2C2                       26
#define  PCLK_MCI                        27
#define  PCLK_SYSCON                     29

/* PLL Setting Table Matrix */
/*
	Main Osc.	CCLKCFG		Fcco		        Fcclk 		M 	N
	12Mhz		29		300Mhz		10Mhz		24	1
	12Mhz		35		360Mhz		10Mhz		14	0
	12Mhz		27		336Mhz		12Mhz		13	0
	12Mhz		14		300Mhz		20Mhz		24	1
	12Mhz		17		360Mhz		20Mhz		14	0
	12Mhz		13		336Mhz		24Mhz		13	0
	12Mhz		11		300Mhz		25Mhz		24	1
	12Mhz		9		300Mhz		30Mhz		24	1
	12Mhz		11		360Mhz		30Mhz		14	0
	12Mhz		9		320Mhz		32Mhz		39	2
	12Mhz		9		350Mhz		35Mhz		174	11
	12Mhz		7		312Mhz		39Mhz		12	0
 	12Mhz		8		360Mhz		40Mhz		14	0
	12Mhz		7		360Mhz		45Mhz		14	0
	12Mhz		6		336Mhz		48Mhz		13	0
	12Mhz		5		300Mhz		50Mhz		24	1
	12Mhz		5		312Mhz		52Mhz		12	0
	12Mhz		5		336Mhz		56Mhz		13	0
 	12Mhz		4		300Mhz		60Mhz		24	1
  	12Mhz		4		320Mhz		64Mhz		39	2
	12Mhz		4		350Mhz		70Mhz		174	11
	12Mhz		4		360Mhz		72Mhz		14	0
	12Mhz		3		300Mhz		75Mhz		24	1
	12Mhz		3		312Mhz		78Mhz		12	0
	12Mhz		3		320Mhz		80Mhz		39	2
	12Mhz		3		336Mhz		84Mhz		13	0
*/
#if 0
#define Fosc                12000000			//	OSC = 12MHz

#define Fcclk		 40000000//40000000//(Fosc * 5)				//4			// 主频 Fcclk = 60MHz
#define Fcco    	360000000//360000000//(Fcclk* 5)

#define Fpclk		(Fcclk / 4)//412500000//10000000//(Fcclk / 5)//4

#define PLL_NValue			0
#define PLL_MValue			(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
#define CCLKDivValue		(Fcco/Fcclk-1)
#define USBCLKDivValue		6
#endif
#define  MAIN_OSC_FRQ              12000000L
#define  IRC_OSC_FRQ                4000000L
#define  RTC_OSC_FRQ                  32768L

#define Fosc            12000000			//	OSC = 12MHz
#define Fcclk		70000000			//4			// 主频 Fcclk = 70MHz
#define Fcco          350000000
#define Fpclk		17500000

#define PLL_NValue			11
#define PLL_MValue			174//(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
#define CCLKDivValue		        4//(Fcco/Fcclk-1)
#define USBCLKDivValue		254
#ifndef __gSYS_CLK
#define __gSYS_CLK
extern uint32 gSYS_Fcclk;  //系统时钟
extern uint32 gSYS_Fpclk;//设备时钟
#endif
void Reset(void *pdata);
void Reset_BootLoader(void);
uint32 GetTickCount(void);
void SetVICIRQ(uint32 VIC_Chanle, uint32 vic_pri, uint32 pr);
void IRQ_Clr(uint32 VIC_Chanle);
void TargetInit(void);
void TargetResetInit(void);
CPU_INT32U  BSP_CPU_ClkFreq (void);
CPU_INT32U  BSP_CPU_PclkFreq (CPU_INT08U  pclk);
void _ttywrch(int ch);

#ifdef __cplusplus
}
#endif

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

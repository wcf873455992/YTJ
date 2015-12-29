
#define IN_TARGET
#include <includes.h>
#include <target.h>
#include "main.h"
uint32 gSYS_Fcclk;  //系统时钟
uint32 gSYS_Fpclk;//设备时钟
extern void uart_Debug_init(void);
void  BSP_IntDisAll (void);
void (*Reset_boot)();
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void VIC_Init(void)
{
    uint8 i;
    VICIntEnClear =  0xFFFFFFFF;                                /* Disable ALL interrupts                                   */
    // VICAddress    =  0;   	/* Acknowlege any pending VIC interrupt                     */
    VICIntSelect = 0;
    VICProtection =  0;                                         /* Allow VIC register access in User of Priviledged modes   */
    for (i = 0; i < 32; i++)
    {
        VICVectAddrs(i) = 0;
        VICVectPri(i) = 0x0f;

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
void Reset(void *pdata)
{
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    OS_CPU_ARM_ExceptResetHndlr();
    for (;;);
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void SetVICIRQ(uint32 VIC_Chanle, uint32 vic_pri, uint32 pr)
{

    VICIntSelect           &= ~(1 << VIC_Chanle);                        /* Enable interrupts                                */
    VICVectAddrs(VIC_Chanle)  = (CPU_INT32U)pr;    /* Set the vector address                           */
    VICIntEnable            =  (1 << VIC_Chanle);
    VICVectPri(VIC_Chanle) = vic_pri;


}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void IRQ_Clr(uint32 VIC_Chanle)
{

    VICIntEnClear = (1 << VIC_Chanle);

}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 GetTickCount(void)
{
    return T2TC;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Timer2_Exception(void)
{

    T2IR = 0xFF;                                                /* Clear timer #0 interrupt                                 */

    OSTimeTick();
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Timer2Init(void)
{
    CPU_INT32U  pclk_freq;
    CPU_INT32U  rld_cnts;


    SetVICIRQ(VIC_CHANLE_TIME2, 14, (uint32)Timer2_Exception);
    pclk_freq     =   BSP_CPU_PclkFreq(PCLK_TIMER2);            /* Get the peripheral clock frequency                       */

    rld_cnts      =   pclk_freq / OS_TICKS_PER_SEC;             /* Calculate the # of counts necessary for the OS ticker    */

    T2TCR         =  (1 << 1);                                  /* Disable and reset counter 0 and the prescale counter 0   */
    T2TCR         =   0;                                        /* Clear the reset bit                                      */
    T2PC          =   0;                                        /* Prescaler is set to no division                          */

    T2MR0         =   rld_cnts;
    T2MCR         =   3;                                        /* Interrupt on MR0 (reset TC), stop TC                     */

    T2CCR         =   0;                                        /* Capture is disabled.                                     */
    T2EMR         =   0;                                        /* No external match output.                                */
    T2TCR         =   1;

}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void TargetInit(void)
{
    BSP_IntDisAll();
    PINInit();
    Io_Init();
    SPI_Init();
    Device_IC_Reset();
    Device_KZ_Reset();
    I2c0Init(100000);
    I2c1Init(400000);
    Timer2Init();
    OS_CPU_SR_IRQ_En();
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Reset_BootLoader(void)
{
    Reset_boot = NULL;
    (*Reset_boot)();

}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void  BSP_IntDisAll (void)
{
    VICAddress = 0;
    VICIntEnClear = 0xFFFFFFFFL;                                /* Disable ALL interrupts                                   */
    VICAddress = 0;
}
/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Get the CPU clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  msel;
    CPU_INT32U  nsel;
    CPU_INT32U  fin;
    CPU_INT32U  pll_clk_feq;                                    /* When the PLL is enabled, this is Fcco                    */
    CPU_INT32U  clk_div;
    CPU_INT32U  clk_freq;


    switch (CLKSRCSEL & 0x03)                                   /* Determine the current clock source                       */
    {
    case 0:
        fin = IRC_OSC_FRQ;
        break;

    case 1:
        fin = MAIN_OSC_FRQ;
        break;

    case 2:
        fin = RTC_OSC_FRQ;
        break;

    default:
        fin = IRC_OSC_FRQ;
        break;
    }

    if ((PLLSTAT & (1 << 25)) > 0)                                  /* If the PLL is currently enabled and connected        */
    {
        msel        = (CPU_INT32U)(PLLSTAT & 0x3FFF) + 1;           /* Obtain the PLL multiplier                            */
        nsel        = (CPU_INT32U)((PLLSTAT >>   16) & 0x0F) + 1;   /* Obtain the PLL divider                               */
        pll_clk_feq = (2 * msel * (fin / nsel));                    /* Compute the PLL output frequency                     */
    }
    else
    {
        pll_clk_feq = (fin);                                        /* The PLL is bypassed                                  */
    }

    clk_div         = (CPU_INT32U)(CCLKCFG & 0xFF) + 1;             /* Obtain the CPU core clock divider                    */
    clk_freq        = (CPU_INT32U)(pll_clk_feq / clk_div);          /* Compute the ARM Core clock frequency                 */

    return (clk_freq);
}

/*
*********************************************************************************************************
*                                            BSP_CPU_PclkFreq()
*
* Description : Get the peripheral clock frequency for a specific peripheral.
*
* Argument(s) : pclk        The peripheral clock ID, one of PCLK_??? defined in bsp.h.
*
* Return(s)   : The peripheral's clock in Hz
*********************************************************************************************************
*/


CPU_INT32U  BSP_CPU_PclkFreq (CPU_INT08U  pclk)
{
    CPU_INT32U  clk_freq;
    CPU_INT32U  selection;


    clk_freq = BSP_CPU_ClkFreq();

    switch (pclk)
    {
    case PCLK_WDT:
    case PCLK_TIMER0:
    case PCLK_TIMER1:
    case PCLK_UART0:
    case PCLK_UART1:
    case PCLK_PWM0:
    case PCLK_PWM1:
    case PCLK_I2C0:
    case PCLK_SPI:
    case PCLK_RTC:
    case PCLK_SSP1:
    case PCLK_DAC:
    case PCLK_ADC:
    case PCLK_CAN1:
    case PCLK_CAN2:
    case PCLK_ACF:
        selection = ((PCLKSEL0 >> (pclk * 2)) & 0x03);
        if (selection == 0)
        {
            return (clk_freq / 4);
        }
        else if (selection == 1)
        {
            return (clk_freq);
        }
        else if (selection == 2)
        {
            return (clk_freq / 2);
        }
        else
        {
            return (clk_freq / 8);
        }

    case PCLK_BAT_RAM:
    case PCLK_GPIO:
    case PCLK_PCB:
    case PCLK_I2C1:
    case PCLK_SSP0:
    case PCLK_TIMER2:
    case PCLK_TIMER3:
    case PCLK_UART2:
    case PCLK_UART3:
    case PCLK_I2C2:
    case PCLK_MCI:
    case PCLK_SYSCON:
        selection = ((PCLKSEL1 >> ((pclk - 16) * 2)) & 0x03);
        if (selection == 0)
        {
            return (clk_freq / 4);
        }
        else if (selection == 1)
        {
            return (clk_freq);
        }
        else if (selection == 2)
        {
            return (clk_freq / 2);
        }
        else
        {
            return (clk_freq / 8);
        }

    default:
        return (0);
    }
}


/*
*********************************************************************************************************
*                                       MAM_Init()
*
* Description : Initialize the Memory Acceleration Module.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/
static  void  MAM_Init (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = BSP_CPU_ClkFreq();                               /* Get the current core clock frequency                     */

    MAMCR    = 0;                                               /* Disable MAM functionality                                */

    if (clk_freq < 20000000)                                    /* Compare current clock frequency with MAM modes           */
    {
        MAMTIM = 1;                                             /* Set MAM fetch cycles to 1 processor clock in duration    */
    }

    if (clk_freq < 40000000)
    {
        MAMTIM = 2;                                             /* Set MAM fetch cycles to 2 processor clock in duration    */
    }

    if (clk_freq >= 40000000)
    {
        MAMTIM = 3;                                             /* Set MAM fetch cycles to 3 processor clock in duration    */
    }

    MAMCR = 2;                                                  /* Enable full MAM functionality                            */
}

/*
*********************************************************************************************************
*                                            PLL_Init()
*
* Description : Set up and activate the PLL.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) The PLL output frequency is calculated by:
*
*                           Fcco = 2 * Fin * m / n
*
*                   where
*
*                           Fin is the PLL input clock (here, the main oscillator)
*                           M   is the PLL clock multiplier. The value (M - 1) is programmed in PLLCFG.
*                           N   is the PLL clock divider.    The value (N - 1) is programmed in PLLCFG.
*
*               (2) Fcco must be between 250 and 550 MHz. The ARM Core clock must never exceed 72 MHz.
*                   Set clk_div to divide Fcco accordingly.
*
*               (3) When using the USB device, you must choose Fcco as a multiple of 96 MHz, and then
*                   set clk_div_usb to divide Fcco to exactly 48 MHz.
*
*               (4) In this example
*
*                         Fin         = 12MHz,
*                         M           = 12,
*                         N           =  1,
*                         clk_div     =  6, and
*                         clk_div_usb =  6.
*
*                 Therefore, Fcco        = 2 * Fin * M / N      = (2 * 12 * 12 / 1) = 288MHz.
*                 The processor clock    = (Fcco / clk_div)     = (288MHz / 6)      =  48MHz.
*                 Finally, the USB clock = (Fcco / clk_div_usb) = (288MHz / 6)      =  48MHz.
*
*               (5) A PLL errata on early revisions of the part prevent Fcco from being greater than 288MHz.
*
*               (6) For later revisions, M = 20, clk_div = 8, and clk_div_usb = 10 will yield 60MHz for
*                   the processor clock and 48MHz for the USB clock.
*********************************************************************************************************
*/

static  void  PLL_Init (void)
{
#if CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL /* Allocate storage for CPU status register                 */
    CPU_SR  cpu_sr = 0;
#endif

    CPU_INT32U  m;
    CPU_INT32U  n;
    CPU_INT32U  clk_div;
    CPU_INT32U  clk_div_usb;


    m           = PLL_MValue;//11;//                                           /* PLL Multiplier = 20, MSEL bits = 12 - 1 = 11             */
    n           =  PLL_NValue;//0;    //                                       /* PLL Divider    =  1, NSEL bits =  1 - 1 =  0             */
    clk_div     = CCLKDivValue;// 5; //                                          /* Configure the  ARM Core clock div to 6. CCLKSEL =  6 - 1 */
    clk_div_usb =  5;                                           /* Configure the USB clock divider to 6, USBSEL  = 6 - 1    */

    if ((PLLSTAT & DEF_BIT_25) > 0)                             /* If the PLL is already running                            */
    {
        CPU_CRITICAL_ENTER();
        PLLCON  &= ~DEF_BIT_01;                                 /* Disconnect the PLL                                       */
        PLLFEED  =  0xAA;                                       /* PLL register update sequence, 0xAA, 0x55                 */
        PLLFEED  =  0x55;
        CPU_CRITICAL_EXIT();
    }

    CPU_CRITICAL_ENTER();
    PLLCON   &= ~DEF_BIT_00;                                    /* Disable the PLL                                          */
    PLLFEED   =  0xAA;                                          /* PLL register update sequence, 0xAA, 0x55                 */
    PLLFEED   =  0x55;
    CPU_CRITICAL_EXIT();

    SCS      &= ~DEF_BIT_04;                                    /* OSCRANGE = 0, Main OSC is between 1 and 20 Mhz           */
    SCS      |=  DEF_BIT_05;                                    /* OSCEN = 1, Enable the main oscillator                    */

    while ((SCS &  DEF_BIT_06) == 0)                            /* Wait until OSCSTAT is set (Main OSC ready to be used)    */
    {
        ;
    }

    CLKSRCSEL = DEF_BIT_00;                                     /* Select main OSC, 12MHz, as the PLL clock source          */

    CPU_CRITICAL_ENTER();
    PLLCFG    = (m << 0) | (n << 16);                           /* Configure the PLL multiplier and divider                 */
    PLLFEED   = 0xAA;                                           /* PLL register update sequence, 0xAA, 0x55                 */
    PLLFEED   = 0x55;
    CPU_CRITICAL_EXIT();

    CPU_CRITICAL_ENTER();
    PLLCON   |= DEF_BIT_00;                                     /* Enable the PLL                                           */
    PLLFEED   = 0xAA;                                           /* PLL register update sequence, 0xAA, 0x55                 */
    PLLFEED   = 0x55;
    CPU_CRITICAL_EXIT();

    CCLKCFG   = clk_div;                                        /* Configure the ARM Core Processor clock divider           */
    USBCLKCFG = clk_div_usb;                                    /* Configure the USB clock divider                          */

    while ((PLLSTAT & DEF_BIT_26) == 0)                         /* Wait for PLOCK to become set                             */
    {
        ;
    }
#if (Fpclk / (Fcclk / 4)) == 1

    PCLKSEL0 = 0x00000000;				// PCLK is 1/4 CCLK
    PCLKSEL1 = 0x00000000;

#else

#if (Fpclk / (Fcclk / 4)) == 2

    PCLKSEL0 = 0xAAAAAAAA;				// PCLK is 1/2 CCLK
    PCLKSEL1 = 0xAAAAAAAA;

#else

    PCLKSEL0 = 0x55555555;				// PCLK is the same as CCLK
    PCLKSEL1 = 0x55555555;

#endif
#endif
    // PCLKSEL0  = 0xAAAAAAAA;                                     /* Set peripheral clocks to be half of main clock           */
    // PCLKSEL1  = 0x22AAA8AA;

    CPU_CRITICAL_ENTER();
    PLLCON   |= DEF_BIT_01;                                     /* Connect the PLL. The PLL is now the active clock source  */
    PLLFEED   = 0xAA;                                           /* PLL register update sequence, 0xAA, 0x55                 */
    PLLFEED   = 0x55;
    CPU_CRITICAL_EXIT();

    while ((PLLSTAT & DEF_BIT_25) == 0)                         /* Wait PLLC, the PLL connect status bit to become set      */
    {
        ;
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
void TargetResetInit(void)
{
    BSP_IntDisAll();
    VIC_Init();
    PLL_Init();
    MAM_Init();
    gSYS_Fpclk = BSP_CPU_PclkFreq(PCLK_TIMER0);
    gSYS_Fcclk = BSP_CPU_ClkFreq();
    uart_Debug_init();
    UART_printf("User TargetResetInit--Run\n\n");
    return;
}
/*********************************************************************************************************
**                  以下为一些与系统相关的库函数的实现
**                  具体作用请ads的参考编译器与库函数手册
**                  用户可以根据自己的要求修改
********************************************************************************************************/
/*********************************************************************************************************
**                  The implementations for some library functions
**                  For more details, please refer to the ADS compiler handbook and The library
**					function manual
**                  User could change it as needed
********************************************************************************************************/

//#include <rt_sys.h>
#include <stdio.h>

//#pragma
//import(__use_no_semihosting_swi)
//#pragma import(__use_two_region_memory)

void _ttywrch(int ch)
{
    ch = ch;
}
/*
*********************************************************************************************************
*                                          OS_CPU_ExceptHndlr()
*
* Description : Handle any exceptions.
*
* Argument(s) : except_id     ARM exception type:
*
*                                  OS_CPU_ARM_EXCEPT_RESET             0x00
*                                  OS_CPU_ARM_EXCEPT_UNDEF_INSTR       0x01
*                                  OS_CPU_ARM_EXCEPT_SWI               0x02
*                                  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT    0x03
*                                  OS_CPU_ARM_EXCEPT_DATA_ABORT        0x04
*                                  OS_CPU_ARM_EXCEPT_ADDR_ABORT        0x05
*                                  OS_CPU_ARM_EXCEPT_IRQ               0x06
*                                  OS_CPU_ARM_EXCEPT_FIQ               0x07
*
* Return(s)   : none.
*
* Caller(s)   : OS_CPU_ARM_EXCEPT_HANDLER(), which is declared in os_cpu_a.s.
*********************************************************************************************************
*/

void  OS_CPU_ExceptHndlr (CPU_DATA except_id)
{
    CPU_FNCT_VOID  pfnct;
    uint32    *sp;

    /* If this exception is either an IRQ or FIQ        */
    if (except_id == OS_CPU_ARM_EXCEPT_IRQ)
    {
        pfnct = (CPU_FNCT_VOID)VICAddress;                              /* Read the interrupt vector from the VIC           */
        if  (pfnct != (CPU_FNCT_VOID)0)                       /* Make sure we don't have a NULL pointer                   */
        {
            (*pfnct)();                                           /* Execute the ISR for the interrupting device              */
            VICVectAddr = 1;                                    /* Acknowlege the VIC interrupt                             */
            // pfnct = (CPU_FNCT_VOID)VICVectAddr;                 /* Read the interrupt vector from the VIC                   */
        }

    }
    else
    {
        IP_printf("SP Err");
        sp = (uint32 *)OSTCBCur->OSTCBStkPtr;
        IP_printf("\nCPU_ARM_EXCEPTION #%d trapped.\n", except_id);
        IP_printf("R0  : 0x%08x\n", *(sp + 0x01));
        IP_printf("R1  : 0x%08x\n", *(sp + 0x02));
        IP_printf("R2  : 0x%08x\n", *(sp + 0x03));
        IP_printf("R3  : 0x%08x\n", *(sp + 0x04));
        IP_printf("R4  : 0x%08x\n", *(sp + 0x05));
        IP_printf("R5  : 0x%08x\n", *(sp + 0x06));
        IP_printf("R6  : 0x%08x\n", *(sp + 0x07));
        IP_printf("R7  : 0x%08x\n", *(sp + 0x08));
        IP_printf("R8  : 0x%08x\n", *(sp + 0x09));
        IP_printf("R9  : 0x%08x\n", *(sp + 0x0A));
        IP_printf("R10 : 0x%08x\n", *(sp + 0x0B));
        IP_printf("R11 : 0x%08x\n", *(sp + 0x0C));
        IP_printf("R12 : 0x%08x\n", *(sp + 0x0D));
        IP_printf("SP  : 0x%08x\n",   sp);
        IP_printf("LR  : 0x%08x\n", *(sp + 0x0E));
        IP_printf("PC  : 0x%08x\n", *(sp + 0x0F));
        IP_printf("CPSR: 0x%08x\n", *(sp + 0x00));


        /* Infinite loop on other exceptions.                       */
        /* Should be replaced by other behavior (reboot, etc.)      */
        while (DEF_TRUE)
        {
            (( void (*)()) 0x00000000 )();
            ;
        }

    }

}


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

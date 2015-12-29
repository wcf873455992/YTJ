/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
*
*                             (c) Copyright 2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                             NXP LPC2378
*                                                on the
*                                      Keil MCB2300 Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : EFS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_GLOBALS
#include <includes.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  HW_REV_0                                   0			/* LCD E bit is pin 30 for early MCB2300 revisions          */
/* For later hardware revisions, define to 0, pin 31.       */

/* ---------------------- GPIO0 Pins ---------------------- */
#define  GPIO0_CAN_RX1                        DEF_BIT_00
#define  GPIO0_CAN_TX1                        DEF_BIT_01
#define  GPIO0_TXD0                           DEF_BIT_02
#define  GPIO0_RXD0                           DEF_BIT_03
#define  GPIO0_CAN_RX2                        DEF_BIT_04
#define  GPIO0_CAN_TX2                        DEF_BIT_05
#define  GPIO0_TXD1                           DEF_BIT_15
#define  GPIO0_RXD1                           DEF_BIT_16
#define  GPIO0_MCICLK                         DEF_BIT_19
#define  GPIO0_MCICMD                         DEF_BIT_20
#define  GPIO0_MCIDAT0                        DEF_BIT_22
#define  GPIO0_POT                            DEF_BIT_23
#define  GPIO0_SPEAKER                        DEF_BIT_26

/* ---------------------- GPIO1 Pins ---------------------- */
#define  GPIO1_E_TXD0                         DEF_BIT_00
#define  GPIO1_E_TXD1                         DEF_BIT_01
#define  GPIO1_E_TX_EN                        DEF_BIT_04
#define  GPIO1_E_CRS0                         DEF_BIT_08
#define  GPIO1_E_RXD0                         DEF_BIT_09
#define  GPIO1_E_RXD1                         DEF_BIT_10
#define  GPIO1_E_RX_ER                        DEF_BIT_14
#define  GPIO1_E_RX_CLK                       DEF_BIT_15
#define  GPIO1_E_MDC                          DEF_BIT_16
#define  GPIO1_E_MDIO                         DEF_BIT_17
#define  GPIO1_LCD_DB4                        DEF_BIT_24
#define  GPIO1_LCD_DB5                        DEF_BIT_25
#define  GPIO1_LCD_DB6                        DEF_BIT_26
#define  GPIO1_LCD_DB7                        DEF_BIT_27
#define  GPIO1_LCD_RS                         DEF_BIT_28
#define  GPIO1_LCD_RW                         DEF_BIT_29
#if HW_REV_0
#define  GPIO1_LCD_E                          DEF_BIT_30
#else
#define  GPIO1_LCD_E                          DEF_BIT_31
#endif

/* ---------------------- GPIO2 Pins ---------------------- */
#define  GPIO2_LED0                           DEF_BIT_00
#define  GPIO2_LED1                           DEF_BIT_01
#define  GPIO2_LED2                           DEF_BIT_02
#define  GPIO2_LED3                           DEF_BIT_03
#define  GPIO2_LED4                           DEF_BIT_04
#define  GPIO2_LED5                           DEF_BIT_05
#define  GPIO2_LED6                           DEF_BIT_06
#define  GPIO2_LED7                           DEF_BIT_07
#define  GPIO2_PB1                            DEF_BIT_10
#define  GPIO2_MCIDAT1                        DEF_BIT_11
#define  GPIO2_MCIDAT2                        DEF_BIT_12
#define  GPIO2_MCIDAT3                        DEF_BIT_13

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

CPU_INT32U  VIC_SpuriousInt;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  PLL_Init         (void);
static  void  MAM_Init         (void);
static  void  GPIO_Init        (void);
static  void  VIC_Init         (void);
static  void  ADC_Init         (void);
static  void  LED_Init         (void);

#ifdef DISP_MODULE_PRESENT
static  void  DispE_High       (void);
static  void  DispE_Low        (void);
static  void  DispRW_High      (void);
static  void  DispRW_Low       (void);
#endif

static  void  Tmr_TickInit     (void);

static  void  VIC_Dummy        (void);                         /* Prototypes for dummy interrupt handlers                  */
static  void  VIC_DummyWDT     (void);
static  void  VIC_DummySW      (void);
static  void  VIC_DummyDEBUGRX (void);
static  void  VIC_DummyDEBUGTX (void);
static  void  VIC_DummyTIMER0  (void);
static  void  VIC_DummyTIMER1  (void);
static  void  VIC_DummyUART0   (void);
static  void  VIC_DummyUART1   (void);
static  void  VIC_DummyPWM01   (void);
static  void  VIC_DummyI2C0    (void);
static  void  VIC_DummySPI     (void);
static  void  VIC_DummySSP1    (void);
static  void  VIC_DummyPLL     (void);
static  void  VIC_DummyRTC     (void);
static  void  VIC_DummyEINT0   (void);
static  void  VIC_DummyEINT1   (void);
static  void  VIC_DummyEINT2   (void);
static  void  VIC_DummyEINT3   (void);
static  void  VIC_DummyAD0     (void);
static  void  VIC_DummyI2C1    (void);
static  void  VIC_DummyBOD     (void);
static  void  VIC_DummyETHERNET(void);
static  void  VIC_DummyUSB     (void);
static  void  VIC_DummyCAN01   (void);
static  void  VIC_DummyMMC     (void);
static  void  VIC_DummyGP_DMA  (void);
static  void  VIC_DummyTIMER2  (void);
static  void  VIC_DummyTIMER3  (void);
static  void  VIC_DummyUART2   (void);
static  void  VIC_DummyUART3   (void);
static  void  VIC_DummyI2C2    (void);
static  void  VIC_DummyI2S     (void);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    PLL_Init();                                    	            /* Initialize the PLL                                       */
    MAM_Init();                                               	/* Initialize the Memory Acceleration Module                */
    GPIO_Init();                                             	/* Initialize the board's I/Os                              */
    ADC_Init();                                                 /* Initialize the board's ADC                               */
    VIC_Init();                                         	    /* Initialize the Vectored Interrupt Controller             */
    LED_Init();                                                	/* Initialize the I/Os for the LED controls                 */

    Tmr_TickInit();                                         	/* Initialize the uC/OS-II tick interrupt                   */
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
    CPU_INT32U    *sp;

    /* If this exception is either an IRQ or FIQ        */
    if ((except_id == OS_CPU_ARM_EXCEPT_IRQ) || (except_id == OS_CPU_ARM_EXCEPT_FIQ))
    {
        pfnct = (CPU_FNCT_VOID)VICAddress;                              /* Read the interrupt vector from the VIC           */
        if (pfnct != (CPU_FNCT_VOID)0)                                  /* Make sure we don't have a NULL pointer           */
        {
            (*pfnct)();                                                   /* Execute the ISR for the interrupting device      */
            VICAddress = 1;                                             /* Acknowlege the VIC interrupt                     */
        }
    }
    else
    {
        sp = (CPU_INT32U *)OSTCBCur->OSTCBStkPtr;
        APP_TRACE_INFO(("\nCPU_ARM_EXCEPTION #%d trapped.\n", except_id));
        APP_TRACE_INFO(("R0  : 0x%08x\n", *(sp + 0x01)));
        APP_TRACE_INFO(("R1  : 0x%08x\n", *(sp + 0x02)));
        APP_TRACE_INFO(("R2  : 0x%08x\n", *(sp + 0x03)));
        APP_TRACE_INFO(("R3  : 0x%08x\n", *(sp + 0x04)));
        APP_TRACE_INFO(("R4  : 0x%08x\n", *(sp + 0x05)));
        APP_TRACE_INFO(("R5  : 0x%08x\n", *(sp + 0x06)));
        APP_TRACE_INFO(("R6  : 0x%08x\n", *(sp + 0x07)));
        APP_TRACE_INFO(("R7  : 0x%08x\n", *(sp + 0x08)));
        APP_TRACE_INFO(("R8  : 0x%08x\n", *(sp + 0x09)));
        APP_TRACE_INFO(("R9  : 0x%08x\n", *(sp + 0x0A)));
        APP_TRACE_INFO(("R10 : 0x%08x\n", *(sp + 0x0B)));
        APP_TRACE_INFO(("R11 : 0x%08x\n", *(sp + 0x0C)));
        APP_TRACE_INFO(("R12 : 0x%08x\n", *(sp + 0x0D)));
        APP_TRACE_INFO(("SP  : 0x%08x\n",   sp));
        APP_TRACE_INFO(("LR  : 0x%08x\n", *(sp + 0x0E)));
        APP_TRACE_INFO(("PC  : 0x%08x\n", *(sp + 0x0F)));
        APP_TRACE_INFO(("CPSR: 0x%08x\n", *(sp + 0x00)));

        /* Infinite loop on other exceptions.                       */
        /* Should be replaced by other behavior (reboot, etc.)      */
        while (DEF_TRUE)
        {
            ;
        }
    }
}


/*
*********************************************************************************************************
*                                           BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    VICIntEnClear = 0xFFFFFFFFL;                                /* Disable ALL interrupts                                   */
}


/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                       LED, PB AND ADC FUNCTIONS
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             LED_Init()
*
* Description : Initialize the I/O for the LEDs
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  LED_Init (void)
{
    LED_Off(0);                                                         /* Turn OFF all the LEDs                                    */
}


/*
*********************************************************************************************************
*                                             LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turn ON all LEDs on the board
*                       1    turn ON LED1
*                       .
*                       .
*                       .
*                       8    turn ON LED8
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  LED_On (CPU_INT08U led)
{
    if (led == 0)
    {
        FIO2SET    =    0xFF;                                           /* Turn on ALL LEDs                                        */
    }

    if ((led >= 1) && (led <= 8))
    {
        led--;                                                          /* Ex: If led is 1, then subtract 1 to indicate bit 0       */
        FIO2SET =  (1 << led);                                          /* Turn on the selected LED                                 */
    }
}


/*
*********************************************************************************************************
*                                             LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turn OFF all LEDs on the board
*                       1    turn OFF LED1
*                       .
*                       .
*                       .
*                       8    turn OFF LED8
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  LED_Off (CPU_INT08U led)
{
    if (led == 0)
    {
        FIO2CLR    =    0xFF;                                           /* Turn off ALL LEDs                                        */
    }

    if ((led >= 1) && (led <= 8))
    {
        led--;                                                          /* Ex: If led is 1, then subtract 1 to indicate bit 0       */
        FIO2CLR =  (1 << led);                                          /* Turn off the selected LED                                */
    }
}


/*
*********************************************************************************************************
*                                             LED_Toggle()
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    TOGGLE all LEDs on the board
*                       1    TOGGLE LED1
*                       .
*                       .
*                       .
*                       8    TOGGLE LED8
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  LED_Toggle (CPU_INT08U led)
{
    CPU_INT08U  status;


    if (led == 0)
    {
        status     =    FIO2PIN;
        FIO2SET    =   ~status;
        FIO2CLR    =    status;
    }

    if ((led >= 1) && (led <= 8))
    {
        led--;                                                          /* Ex: If led is 1, then subtract 1 to indicate bit 0       */
        if ((FIO2PIN & (1 << led)) == 0)                                /* If the LED is currently off                              */
        {
            FIO2SET =  (1 << 0);                                        /* Turn on the selected LED                                 */
        }
        else
        {
            FIO2CLR =  (1 << led);                                      /* Turn off the selected LED                                */
        }
    }
}


/*
*********************************************************************************************************
*                                           PB_GetStatus()
*
* Description : Get the status of a push button on the board.
*
* Argument(s) : pb      The ID of the push button to probe
*
*                       1    probe the push button INT1
*
* Return(s)   : DEF_FALSE   if the push button is pressed
*               DEF_TRUE    if the push button is not pressed
*********************************************************************************************************
*/

CPU_BOOLEAN  PB_GetStatus (CPU_INT08U pb)
{
    CPU_BOOLEAN  status;


    status = DEF_FALSE;

    switch (pb)
    {
    case 1:
        if ((FIO2PIN & GPIO2_PB1) == 0)
        {
            status =  (DEF_TRUE);
        }
        break;

    default:
        break;
    }

    return (status);
}

/*
*********************************************************************************************************
*                                           ADC_Init()
*
* Description : Initialize the board's ADC.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

static  void  ADC_Init (void)
{
    CPU_INT08U  div;
    CPU_INT32U  pinsel;


    PCONP      |=  DEF_BIT_12;

    div         = (BSP_CPU_PclkFreq(PCLK_ADC) / 4500000) + 1;
    /* Configure ADC ...                                        */
    AD0CR       = (0x01 <<  0)                                  /*  ... Sample/convert pin AD0.0 ...                        */
                  | (div  <<  8)                                  /*  ... Set divider so sample freq. < 4.5 MHz ...           */
                  |  DEF_BIT_16                                   /*  ... Use burst mode for continuous conversion ...        */
                  | (0x00 << 17)                                  /*  ... Use 11 clocks per conversion for 10-bit accuracy ...*/
                  |  DEF_BIT_21;                                  /*  ... Power up the ADC.                                   */

    /* Select AD0.0 function for P0.23                          */
    pinsel      = PINSEL1;
    pinsel     &= 0xFFFF3FFF;
    pinsel     |= 0x00004000;
    PINSEL1     = pinsel;
}


/*
*********************************************************************************************************
*                                           ADC_GetStatus()
*
* Description : Get the status of an ADC input.
*
* Argument(s) : pb      The ID of the push button to probe
*
*                       0   probes AD0.0, the potentiometer
*
* Return(s)   : The numerator of the binary fraction representing the result of the latest ADC conversion.
*               This value will be a 10-bit value between 0x0000 and 0x03FF, inclusive.
*********************************************************************************************************
*/

CPU_INT16U  ADC_GetStatus (CPU_INT08U  adc)
{
    if (adc == 0)
    {
        return ((ADDR0 >> 6) & 0x03FF);
    }
    else
    {
        return (0);
    }
}

/*
*********************************************************************************************************
*********************************************************************************************************
**                                          uC/LCD FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            DispInitPort()
*
* Description : Initializes the I/O ports used by the display driver.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) The I/Os for the LCD module are initialized in BSP_IO_Init().
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
void  DispInitPort (void)
{
}
#endif


/*
*********************************************************************************************************
*                                              DispDataWr()
*
* Description : Sends a single byte to the display device.
*
* Argument(s) : 'data'      Data to send to the display device
*
* Return(s)   : none.
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
void  DispDataWr (CPU_INT08U data)
{
    CPU_INT32U  value;


    DispRW_Low();                                                       /* Set R/W write LOW to write to the LCD module             */

    DispE_High();                                                       /* Write the UPPER nibble to the LCD module                 */
    value  =  ((data >> 4) & 0x0F) << 24;
    IO1SET = value;
    value  = (~(data >> 4) & 0x0F) << 24;
    IO1CLR = value;

    DispDly_uS(100);
    DispE_Low();

    DispDly_uS(100);                                                    /* Write the LOWER nibble to the LCD module                 */
    DispE_High();

    value  =  (data & 0x0F) << 24;
    IO1SET = value;
    value  = (~data & 0x0F) << 24;
    IO1CLR = value;

    DispDly_uS(100);
    DispE_Low();
}

#if DISP_BUS_WIDTH == 4
void DispDataWrOneNibble(CPU_INT08U data)
{
    CPU_INT32U  value;


    DispRW_Low();                                                       /* Set R/W write LOW to write to the LCD module             */

    DispE_High();                                                       /* Write the UPPER nibble to the LCD module                 */
    value  =  ((data >> 4) & 0x0F) << 24;
    IO1SET = value;
    value  = (~(data >> 4) & 0x0F) << 24;
    IO1CLR = value;

    DispDly_uS(100);
    DispE_Low();
}
#endif
#endif                                                                  /* End #ifdef DISP_MODULE_PRESENT                           */

/*
*********************************************************************************************************
*                                               DispDly_uS()
*
* Description : Delay for the specified number of microseconds.
*
* Argument(s) : us      Number of microseconds
*
* Return(s)   : none.
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
void  DispDly_uS (CPU_INT32U us)
{
    CPU_INT32U  us_per_tick;
    CPU_INT32U  ticks;


    us_per_tick = 1000000L / OS_TICKS_PER_SEC;
    ticks       = us / us_per_tick + 1;
    OSTimeDly(ticks);
}
#endif

/*
*********************************************************************************************************
*                                            DispSel()
*
* Description : Change the Register Select control line to the LCD controller to select either
*               command or data register.
*
* Argument(s) : sel         Indicates whether command or data register should be selected:
*
*                           DISP_SEL_CMD_REG    select command register
*                           DISP_SEL_DATA_REG   select data    register
*
* Return(s)   : none.
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
void  DispSel (CPU_INT08U sel)
{
    if (sel == DISP_SEL_CMD_REG)
    {
        IO1CLR = GPIO1_LCD_RS;                     	            /* Select the command register (RS low)                     */
    }
    else
    {
        IO1SET = GPIO1_LCD_RS;                           	    /* Select the data    register (RS high)                    */
    }
}
#endif

/*
*********************************************************************************************************
*                                      DISPLAY CONTROL LINE FUNCTIONS
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
static  void  DispE_High (void)
{
    IO1SET = GPIO1_LCD_E;                           	        /* Raise the LCD Enable pin high                            */
}


static  void  DispE_Low (void)
{
    IO1CLR = GPIO1_LCD_E;                                	    /* Lower the LCD Enable pin                                 */
}


static  void  DispRW_High (void)
{
    IO1SET = GPIO1_LCD_RW;                                  	/* Raise the LCD R/W pin                                    */
}


static  void  DispRW_Low (void)
{
    IO1CLR = GPIO1_LCD_RW;                               	    /* Lower the LCD R/W pin                                    */
}
#endif



/*
*********************************************************************************************************
*********************************************************************************************************
**                                          uC/OS-View FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              OSView_TmrInit()
*
* Description : Select & initialize a timer for use with uC/OS-View.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

#if (OS_VIEW_MODULE > 0)
void  OSView_TmrInit (void)
{
    T1PR  = 0;
    T1TCR = 0x00000001;                                         /* Enable the timer                                         */

}
#endif


/*
*********************************************************************************************************
*                                         OSView_TmrRd()
*
* Description : Read the current counts of a 32-bit free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 32 bit counts of the timer.
*********************************************************************************************************
*/

#if (OS_VIEW_MODULE > 0)
CPU_INT32U  OSView_TmrRd (void)
{
    return ((CPU_INT32U)T1TC);
}
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                             uC/Probe PLUG-IN FOR uC/OS-II FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       OSProbe_TmrInit()
*
* Description : Select & initialize a timer for use with the uC/Probe Plug-In for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN == 1)
void  OSProbe_TmrInit (void)
{
    T1PR  = 0;
    T1TCR = 0x00000001;                                         /* Enable the timer                                         */

}
#endif


/*
*********************************************************************************************************
*                                        OSProbe_TmrRd()
*
* Description : Read the current counts of a 32-bit free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 32bit counts of the timer.
*********************************************************************************************************
*/

#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN == 1)
CPU_INT32U  OSProbe_TmrRd (void)
{
    return ((CPU_INT32U)T1TC);
}
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     uC/OS-II TIMER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            Tmr_TickInit()
*
* Description : Initialize uC/OS-II's tick source.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

static  void  Tmr_TickInit (void)
{
    CPU_INT32U  pclk_freq;
    CPU_INT32U  rld_cnts;

    /* VIC timer #0 Initialization                              */
    VICIntSelect &= ~(1 << VIC_TIMER0);                         /* Configure the timer interrupt as an IRQ source           */
    VICVectAddr4  =  (CPU_INT32U)Tmr_TickISR_Handler;           /* Set the vector address                                   */
    VICIntEnable  =  (1 << VIC_TIMER0);                         /* Enable the timer interrupt source                        */

    pclk_freq     =   BSP_CPU_PclkFreq(PCLK_TIMER0);            /* Get the peripheral clock frequency                       */

    rld_cnts      =   pclk_freq / OS_TICKS_PER_SEC;             /* Calculate the # of counts necessary for the OS ticker    */

    T0TCR         =  (1 << 1);                                  /* Disable and reset counter 0 and the prescale counter 0   */
    T0TCR         =   0;                                        /* Clear the reset bit                                      */
    T0PC          =   0;                                        /* Prescaler is set to no division                          */

    T0MR0         =   rld_cnts;
    T0MCR         =   3;                                        /* Interrupt on MR0 (reset TC), stop TC                     */

    T0CCR         =   0;                                        /* Capture is disabled.                                     */
    T0EMR         =   0;                                        /* No external match output.                                */
    T0TCR         =   1;                                        /* Enable timer 0                                           */
}


/*
*********************************************************************************************************
*                                       Tmr_TickISR_Handler()
*
* Description : Handle the timer interrupt that is used to generate TICKs for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  Tmr_TickISR_Handler (void)
{
    T0IR = 0xFF;                                                /* Clear timer #0 interrupt                                 */

    OSTimeTick();                                               /* Call uC/OS-II's OSTimeTick()                             */
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                          LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

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


    m           = 11;                                           /* PLL Multiplier = 20, MSEL bits = 12 - 1 = 11             */
    n           =  0;                                           /* PLL Divider    =  1, NSEL bits =  1 - 1 =  0             */
    clk_div     =  5;                                           /* Configure the  ARM Core clock div to 6. CCLKSEL =  6 - 1 */
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

    PCLKSEL0  = 0xAAAAAAAA;                                     /* Set peripheral clocks to be half of main clock           */
    PCLKSEL1  = 0x22AAA8AA;

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
*                                          GPIO_Init()
*
* Description : Initializes the GPIO pins.  All the I/O pins are initialized in this function
*               so you don't have to look at multiple places for I/O initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) Refer to the LPC2378 User Manual, Chapter 9 for a detailed Pin Assignment
*********************************************************************************************************
*/

static  void  GPIO_Init (void)
{
    CPU_INT32U  value;


    IO0DIR   = 0;
    IO1DIR   = 0;
    FIO0DIR  = 0;
    FIO1DIR  = 0;
    FIO2DIR  = 0;
    FIO3DIR  = 0;
    FIO4DIR  = 0;

    FIO0MASK = 0;
    FIO1MASK = 0;
    FIO2MASK = 0;
    FIO3MASK = 0;
    FIO4MASK = 0;

    PINSEL0  = 0;
    PINSEL1  = 0;
    PINSEL2  = 0;
    PINSEL3  = 0;
    PINSEL4  = 0;
    PINSEL5  = 0;
    PINSEL6  = 0;
    PINSEL7  = 0;
    PINSEL8  = 0;
    PINSEL9  = 0;
    PINSEL10 = 0;

#ifdef DISP_MODULE_PRESENT
    value    =  (3 << 24) | (3 << 26) | (3 << 28);     	        /* Create a mask for the LCD Control function bits          */
    PINSEL3 &=  ~value;                                    	    /* Clear and configure the associated function bits as GPIO */

    value    =  (3 << 16) | (3 << 18) | (3 << 20) | (3 << 22);  /* Create a mask for the LCD Data function bits             */
    PINSEL3 &=  ~value;                                     	/* Clear and configure the associated function bits as GPIO */

    value    =   GPIO1_LCD_DB4 | GPIO1_LCD_DB5 | GPIO1_LCD_DB6 | GPIO1_LCD_DB7 | GPIO1_LCD_RS | GPIO1_LCD_RW | GPIO1_LCD_E;
    IO1DIR  |=   value;                                    	    /* Configure all of the LCD pins as outputs                 */

    DispRW_High();                                           	/* Initialize the state of the LCD RW bit to High           */
#endif

    PINSEL2 &= ~(0xFF <<  0);                                	/* Clear P2[7:0] function bits enabling GPIO for the LEDs   */
    FIO2DIR |=  (0xFF <<  0);                              	    /* Configure P2[7:0] as output bits                         */

    PINSEL2 &= ~(0x03 << 30);                              	    /* Clear P2.10 function bits, enabling GPIO function        */
    FIO2DIR &= ~(0x01 << 10);                                	/* Configure P2.10 as an input bit                          */

    PINSEL10 =   0;                                         	/* Release ETM control of PORT 2 I/O pins                   */

    PINSEL1  = ~(0x03 << 20);                                	/* Select Port 0, pin 26 as GPIO (speaker pin)              */
    IO0CLR   =  (0x01 << 26);                              	    /* Clear pin 26, prevent speaker usage humming              */
}


/*
*********************************************************************************************************
*                                     VIC_Init()
*
* Description : Initialize the Vectored Interrupt Controller
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

static  void  VIC_Init (void)
{
    VICIntEnClear =  0xFFFFFFFF;                                /* Disable ALL interrupts                                   */
    VICAddress    =  0;                                         /* Acknowlege any pending VIC interrupt                     */
    VICProtection =  0;                                         /* Allow VIC register access in User of Priviledged modes   */

    VICVectAddr0  = (CPU_INT32U)VIC_DummyWDT;                   /* Set the vector address                                   */
    VICVectAddr1  = (CPU_INT32U)VIC_DummySW;
    VICVectAddr2  = (CPU_INT32U)VIC_DummyDEBUGRX;
    VICVectAddr3  = (CPU_INT32U)VIC_DummyDEBUGTX;
    VICVectAddr4  = (CPU_INT32U)VIC_DummyTIMER0;
    VICVectAddr5  = (CPU_INT32U)VIC_DummyTIMER1;
    VICVectAddr6  = (CPU_INT32U)VIC_DummyUART0;
    VICVectAddr7  = (CPU_INT32U)VIC_DummyUART1;
    VICVectAddr8  = (CPU_INT32U)VIC_DummyPWM01;
    VICVectAddr9  = (CPU_INT32U)VIC_DummyI2C0;
    VICVectAddr10 = (CPU_INT32U)VIC_DummySPI;
    VICVectAddr11 = (CPU_INT32U)VIC_DummySSP1;
    VICVectAddr12 = (CPU_INT32U)VIC_DummyPLL;
    VICVectAddr13 = (CPU_INT32U)VIC_DummyRTC;
    VICVectAddr14 = (CPU_INT32U)VIC_DummyEINT0;
    VICVectAddr15 = (CPU_INT32U)VIC_DummyEINT1;
    VICVectAddr16 = (CPU_INT32U)VIC_DummyEINT2;
    VICVectAddr17 = (CPU_INT32U)VIC_DummyEINT3;
    VICVectAddr18 = (CPU_INT32U)VIC_DummyAD0;
    VICVectAddr19 = (CPU_INT32U)VIC_DummyI2C1;
    VICVectAddr20 = (CPU_INT32U)VIC_DummyBOD;
    VICVectAddr21 = (CPU_INT32U)VIC_DummyETHERNET;
    VICVectAddr22 = (CPU_INT32U)VIC_DummyUSB;
    VICVectAddr23 = (CPU_INT32U)VIC_DummyCAN01;
    VICVectAddr24 = (CPU_INT32U)VIC_DummyMMC;
    VICVectAddr25 = (CPU_INT32U)VIC_DummyGP_DMA;
    VICVectAddr26 = (CPU_INT32U)VIC_DummyTIMER2;
    VICVectAddr27 = (CPU_INT32U)VIC_DummyTIMER3;
    VICVectAddr28 = (CPU_INT32U)VIC_DummyUART2;
    VICVectAddr29 = (CPU_INT32U)VIC_DummyUART3;
    VICVectAddr30 = (CPU_INT32U)VIC_DummyI2C2;
    VICVectAddr31 = (CPU_INT32U)VIC_DummyI2S;
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                       DUMMY INTERRUPT HANDLERS
*********************************************************************************************************
*********************************************************************************************************
*/

static  void  VIC_Dummy (void)
{
    while (DEF_TRUE)
    {
        ;
    }
}

static  void  VIC_DummyWDT (void)
{
    VIC_SpuriousInt = VIC_WDT;
    VIC_Dummy();
}

static  void  VIC_DummySW (void)
{
    VIC_SpuriousInt = VIC_SW;
    VIC_Dummy();
}

static  void  VIC_DummyDEBUGRX (void)
{
    VIC_SpuriousInt = VIC_DEBUGRX;
    VIC_Dummy();
}

static  void  VIC_DummyDEBUGTX (void)
{
    VIC_SpuriousInt = VIC_DEBUGTX;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER0 (void)
{
    VIC_SpuriousInt = VIC_TIMER0;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER1 (void)
{
    VIC_SpuriousInt = VIC_TIMER1;
    VIC_Dummy();
}

static  void  VIC_DummyUART0 (void)
{
    VIC_SpuriousInt = VIC_UART0;
    VIC_Dummy();
}

static  void  VIC_DummyUART1 (void)
{
    VIC_SpuriousInt = VIC_UART1;
    VIC_Dummy();
}

static  void  VIC_DummyPWM01 (void)
{
    VIC_SpuriousInt = VIC_PWM01;
    VIC_Dummy();
}

static  void  VIC_DummyI2C0 (void)
{
    VIC_SpuriousInt = VIC_I2C0;
    VIC_Dummy();
}

static  void  VIC_DummySPI (void)
{
    VIC_SpuriousInt = VIC_SPI;
    VIC_Dummy();
}

static  void  VIC_DummySSP1 (void)
{
    VIC_SpuriousInt = VIC_SSP1;
    VIC_Dummy();
}

static  void  VIC_DummyPLL (void)
{
    VIC_SpuriousInt = VIC_PLL;
    VIC_Dummy();
}

static  void  VIC_DummyRTC (void)
{
    VIC_SpuriousInt = VIC_RTC;
    VIC_Dummy();
}

static  void  VIC_DummyEINT0 (void)
{
    VIC_SpuriousInt = VIC_EINT0;
    VIC_Dummy();
}

static  void  VIC_DummyEINT1 (void)
{
    VIC_SpuriousInt = VIC_EINT1;
    VIC_Dummy();
}

static  void  VIC_DummyEINT2 (void)
{
    VIC_SpuriousInt = VIC_EINT2;
    VIC_Dummy();
}

static  void  VIC_DummyEINT3 (void)
{
    VIC_SpuriousInt = VIC_EINT3;
    VIC_Dummy();
}

static  void  VIC_DummyAD0 (void)
{
    VIC_SpuriousInt = VIC_AD0;
    VIC_Dummy();
}

static  void  VIC_DummyI2C1 (void)
{
    VIC_SpuriousInt = VIC_I2C1;
    VIC_Dummy();
}

static  void  VIC_DummyBOD (void)
{
    VIC_SpuriousInt = VIC_BOD;
    VIC_Dummy();
}

static  void  VIC_DummyETHERNET (void)
{
    VIC_SpuriousInt = VIC_ETHERNET;
    VIC_Dummy();
}

static  void  VIC_DummyUSB (void)
{
    VIC_SpuriousInt = VIC_USB;
    VIC_Dummy();
}

static  void  VIC_DummyCAN01 (void)
{
    VIC_SpuriousInt = VIC_CAN12;
    VIC_Dummy();
}

static  void  VIC_DummyMMC (void)
{
    VIC_SpuriousInt = VIC_MMC;
    VIC_Dummy();
}

static  void  VIC_DummyGP_DMA (void)
{
    VIC_SpuriousInt = VIC_GP_DMA;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER2 (void)
{
    VIC_SpuriousInt = VIC_TIMER2;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER3 (void)
{
    VIC_SpuriousInt = VIC_TIMER3;
    VIC_Dummy();
}

static  void  VIC_DummyUART2 (void)
{
    VIC_SpuriousInt = VIC_UART2;
    VIC_Dummy();
}

static  void  VIC_DummyUART3 (void)
{
    VIC_SpuriousInt = VIC_UART3;
    VIC_Dummy();
}

static  void  VIC_DummyI2C2 (void)
{
    VIC_SpuriousInt = VIC_I2C2;
    VIC_Dummy();
}

static  void  VIC_DummyI2S (void)
{
    VIC_SpuriousInt = VIC_I2S;
    VIC_Dummy();
}

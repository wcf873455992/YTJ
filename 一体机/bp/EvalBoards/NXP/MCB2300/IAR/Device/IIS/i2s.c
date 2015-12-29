/*****************************************************************************
 *   i2s.c:  I2S C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "includes.h"

/* treat I2S TX and RX as a constant address, make the code and buffer
easier for both DMA and non-DMA test */

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void I2SStart( void )
{
    DWORD DAIValue, DAOValue;

    /* Audio output is the master, audio input is the slave, */
    /* 16 bit data, stereo, reset, master mode, not mute. */
    DAOValue = I2SDAO;
    DAIValue = I2SDAI;
    I2SDAO = DAOValue & (~((1 << 4) | (1 << 3)));
    /* 16 bit data, stereo, reset, slave mode, not mute. */
    I2SDAI	= DAIValue & (~((1 << 4) | (1 << 3)));
    return;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void I2SStop( void )
{
    DWORD DAIValue, DAOValue;

    /* Stop the I2S to start. Audio output is master, audio input is the slave. */
    /* 16 bit data, set STOP and RESET bits to reset the channels */
    DAOValue = I2SDAO;
    /* Switch to master mode, TX channel, no mute */
    DAOValue &= ~(1 << 15);
    DAIValue = I2SDAI;
    // DAIValue &= ~(1 << 15);
    I2SDAO = (1 << 4) | (1 << 3) | DAOValue;	/* Slave */
    I2SDAI = (1 << 4) | (1 << 3) | DAIValue;	/* Slave */
    return;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
DWORD I2SInit( void )
{
    /*enable I2S in the PCONP register. I2S is disabled on reset*/
    PCONP |= (1 << 27);

    /*connect the I2S sigals to port pins(P0.4-P0.9)*/
    PINSEL0 &= ~0x000FFF00;
    PINSEL0 |= 0x00055500;

    /* For all the test program assuming USB is used, the CCLK is set to
    57.6Mhz while all the PCLK are the same, 1/2 of CCLK = 28.8Mhz. It also
    applys to I2S. To generate 48khz sample rate for 16-bit stereo data
    requires a bit rate of 48,000x16x2=1.536Mhz, the TX and RX clock rate
    register should be 28800000/1536000=18.75=0x13.

    Please note, in order to generate accurate TX/RX clock rate for I2S,
    PCLK and CCLK needs to be carefully reconsidered. For this test
    program, the TX is looped back to RX without external I2S device,
    clock rate is not critical in this matter. */
    I2STXRATE = 0x13;
    I2SRXRATE = 0x13;
#if 0
    DAOValue = I2SDAO;
    DAIValue = I2SDAI;
    I2SDAO = DAOValue | (1 << 4) | (1 << 3) | (1 << 5); //reset
    I2SDAI = DAIValue | (1 << 4) | (1 << 3) | (1 << 5);

    DAOValue = I2SDAO;
    DAIValue = I2SDAI;
#endif
    I2SDAO = (1) | (1 << 3) | (1 << 4) | (1 << 5) | (0x1f << 6);
    I2SDAI = (1) | (1 << 3) | (1 << 4) | (1 << 5) | (0x1f << 6);
    I2SStop();
    I2SIRQ = 0; //禁止中断
    I2SSTATE = I2SSTATE;
    I2SDMA2 = (0x01 << 0) | (0x01 << 8);	/* Channel 2 is for RX, enable RX first. */
    I2SDMA1 = (0x01 << 1) | (0x01 << 16); /* Channel 1 is for TX. */

    return(TRUE);
}

/******************************************************************************
**                            End Of File
******************************************************************************/


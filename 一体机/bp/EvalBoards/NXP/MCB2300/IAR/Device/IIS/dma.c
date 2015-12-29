/*****************************************************************************
 *   dma.c:  DMA module file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "includes.h"                        /* LPC23xx/24xx definitions */
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 DMA_Get_Play_Pointer(void)
{
    uint32 reg;
    reg = (DMACC0LLI - SOUND_NEXT_TABL) / 16;
    reg = (reg + SOUND_BUFF_MAX - 1) % SOUND_BUFF_MAX;
    return reg;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 *DMA_Get_MICaddress(void)
{
    if (DMACC1LLI == SOUND_MIC_TABL)
        return (uint8 *)(SOUND_MIC_BUF + SOUND_BUFF_SIZE * 2);
    else if (DMACC1LLI == SOUND_MIC_TABL + 16)
        return (uint8 *)(SOUND_MIC_BUF);
    else
        return (uint8 *)(SOUND_MIC_BUF + SOUND_BUFF_SIZE);


}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void DMAHandler(void)
{
    DWORD regVal, val_next;

    regVal = DMACIntTCStatus;
    if ( regVal )
    {

        DMACIntTCClear |= regVal;
        if ( regVal & 0x01 )
        {
            val_next = Sound_Play_Int(); //

        };
        if ( regVal & 0x02 )
        {
            val_next = Sound_Input_Int(); //


        }
    }

    regVal = DMACIntErrorStatus;
    if ( regVal )
    {
        DMACIntErrClr |= regVal;
    }
    val_next = val_next;
    //VICVectAddr = 0;		/* Acknowledge Interrupt */
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
DWORD DMA_Init(void)
{
    uint32 *p, i;
    PCONP |= (1 << 29);	/* Enable GPDMA clock */
    DMACIntTCClear = 0x03;
    DMACIntErrClr = 0x03;
    DMACConfiguration = 0x01; //小端模式

    while ( !(DMACConfiguration & 0x01) );


    //通道0DMA发送

    /* The burst size is set to 1. Terminal Count Int enable */
    //  p=(uint32 *)DMA_DST;
    //	  memset((BYTE *)p,0,1024);
    memset((BYTE *)SOUND_BUFF_ADDRESS, 0, 8 * 1024);
    SetVICIRQ(VIC_CHANLE_GPDMA, 1, (uint32)DMAHandler);
    //////////////////////////////////////////////////
    DMACC0SrcAddr = SOUND_BUFF_ADDRESS;
    DMACC0DestAddr = DMA_I2S_TX_FIFO;
    p = (uint32 *)SOUND_NEXT_TABL;
    memset((BYTE *)p, 0, 256);
    for (i = 0; i < SOUND_BUFF_MAX; i++)
    {
        *(p + i * 4) = SOUND_BUFF_ADDRESS + SOUND_BUFF_SIZE * ((i + 1) % SOUND_BUFF_MAX);
        *(p + i * 4 + 1) = DMA_I2S_TX_FIFO;
        *(p + i * 4 + 2) = (uint32)p + ((i + 1) % SOUND_BUFF_MAX) * 16;
        *(p + i * 4 + 3) = (SOUND_BUFF_SIZE / 4) | (0x00 << 12) | (0x00 << 15) | (2 << 18) | (2 << 21) | (1 << 26) | 0x80000000;
    };
    DMACC0LLI = SOUND_NEXT_TABL;
    DMACC0Control = (SOUND_BUFF_SIZE / 4) | (0x00 << 12) | (0x00 << 15) | (2 << 18) | (2 << 21) | (1 << 26) | 0x80000000;
    DMACC0Configuration = 0x10000 | (0x00 << 1) | (0x05 << 6) | (0x01 << 11);
    DMACC0Configuration |= 0x8001;
    /* Ch1 set for P2M transfer from I2S RX FIFO to memory. */
    /* The burst size is set to 1. Terminal Count Int enable. */
    p = (uint32 *)SOUND_MIC_TABL;
    for (i = 0; i < SOUND_MIC_SIZE; i++)
    {
        *(p + i * 4) = DMA_I2S_RX_FIFO;
        *(p + i * 4 + 1) = SOUND_MIC_BUF + SOUND_BUFF_SIZE * ((i + 1) % SOUND_MIC_SIZE);
        *(p + i * 4 + 2) = (uint32)p + ((i + 1) % SOUND_MIC_SIZE) * 16;
        *(p + i * 4 + 3) = (SOUND_BUFF_SIZE / 4 ) | (0x00 << 12) | (0x00 << 15) | (2 << 18) | (2 << 21) | (1 << 27) | 0x80000000;
    };
    //////////////////////////////////////////////////////////
    DMACC1SrcAddr = DMA_I2S_RX_FIFO;
    DMACC1DestAddr = SOUND_MIC_BUF;
    DMACC1LLI = SOUND_MIC_TABL;
    DMACC1Control = (SOUND_BUFF_SIZE / 4 ) | (0x00 << 12) | (0x00 << 15) | (2 << 18) | (2 << 21) | (1 << 27) | 0x80000000;
    DMACC1Configuration = 0x08001 | (0x06 << 1) | (0x00 << 6) | (0x02 << 11);
    return( TRUE );
}



/******************************************************************************
**                            End Of File
******************************************************************************/

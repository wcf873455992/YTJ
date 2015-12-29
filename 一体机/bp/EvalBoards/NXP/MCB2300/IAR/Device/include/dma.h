/*****************************************************************************
 *   dma.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __DMA_H
#define __DMA_H

/* USB RAM is used for GPDMA operation. */
//#define DMA_SRC				SOUND_BUFF_ADDRESS
//#define DMA_DST				SOUND_MIC_BUF
#define DMA_I2S_TX_FIFO		0xE0088008
#define DMA_I2S_RX_FIFO		0xE008800C
#define SOUND_NEXT_TABL	0x7fd01F00 //SOUND_MIC_BUF+SOUND_BUFF_SIZE*2
#define SOUND_MIC_TABL		0x7fd01Fc0 //SOUND_MIC_BUF+SOUND_BUFF_SIZE*2
//#define DMA_SIZE		0x100

/* DMA mode */
#define M2M				0x00
#define M2P				0x01
#define P2M				0x02
#define P2P				0x03
struct DMALLI_TYPE
{
    uint32 src;
    uint32 des;
    uint32 next;
    uint32 config;

};

void DMAHandler( void );

DWORD DMA_Init( void);
uint8 *DMA_Get_MICaddress(void);
uint32 DMA_Get_Play_Pointer(void);

#endif /* end __DMA_H */
/****************************************************************************
**                            End Of File
****************************************************************************/

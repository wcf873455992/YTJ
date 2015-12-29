/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			LPC23XX_ADD_R.h
** Last modified Date:  2006-01-01
** Last Version:		1.0
** Descriptions:		Defines the special registers and firmware programs for 
**						LPC23XX
**
**------------------------------------------------------------------------------------------------------
** Created by:			Yehaoben
** Created date:		2006-01-01
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/
#ifndef _LPC23XX_ADD_R_H
#define _LPC23XX_ADD_R_H
/* EXTERNAL IO CONFIGURATION */
/* 外部IO配置寄存器 */

#define rPINSEL2        (*((volatile unsigned int *) 0xE002C008))       /* lpc23xx only */
#define rPINSEL3        (*((volatile unsigned int *) 0xE002C00C))       /* lpc23xx only */
#define rPINSEL4        (*((volatile unsigned int *) 0xE002C010))       /* lpc23xx only */
#define rPINSEL5        (*((volatile unsigned int *) 0xE002C014))       /* lpc23xx only */
#define rPINSEL6        (*((volatile unsigned int *) 0xE002C018))       /* lpc23xx only */
#define rPINSEL7        (*((volatile unsigned int *) 0xE002C01C))       /* lpc23xx only */
#define rPINSEL8        (*((volatile unsigned int *) 0xE002C020))       /* lpc23xx only */

#define rPCONP        (*((volatile unsigned int *) 0xE01FC0C4))       /* lpc23xx only */

/* ETHERNET REGISTER */
/* MAC REGISTER */
#define rMAC1           (*((volatile unsigned int *) 0xFFE00000))       /* lpc23xx only */
#define rMAC2           (*((volatile unsigned int *) 0xFFE00004))       /* lpc23xx only */
#define rIPGT           (*((volatile unsigned int *) 0xFFE00008))       /* lpc23xx only */
#define rIPGR           (*((volatile unsigned int *) 0xFFE0000C))       /* lpc23xx only */
#define rCLRT           (*((volatile unsigned int *) 0xFFE00010))       /* lpc23xx only */
#define rMAXF           (*((volatile unsigned int *) 0xFFE00014))       /* lpc23xx only */
#define rSUPP           (*((volatile unsigned int *) 0xFFE00018))       /* lpc23xx only */
#define rTEST           (*((volatile unsigned int *) 0xFFE0001C))       /* lpc23xx only */
#define rMCFG           (*((volatile unsigned int *) 0xFFE00020))       /* lpc23xx only */
#define rMCMD           (*((volatile unsigned int *) 0xFFE00024))       /* lpc23xx only */
#define rMADR           (*((volatile unsigned int *) 0xFFE00028))       /* lpc23xx only */
#define rMWTD           (*((volatile unsigned int *) 0xFFE0002C))       /* lpc23xx only */
#define rMRDD           (*((volatile unsigned int *) 0xFFE00030))       /* lpc23xx only */
#define rMIND           (*((volatile unsigned int *) 0xFFE00034))       /* lpc23xx only */
#define rSA0            (*((volatile unsigned int *) 0xFFE00040))       /* lpc23xx only */
#define rSA1            (*((volatile unsigned int *) 0xFFE00044))       /* lpc23xx only */
#define rSA2            (*((volatile unsigned int *) 0xFFE00048))       /* lpc23xx only */
/* CONTROL REGISTER */
#define rECOMMAND       (*((volatile unsigned int *) 0xFFE00100))       /* lpc23xx only */
#define rESTATUS        (*((volatile unsigned int *) 0xFFE00104))       /* lpc23xx only */
#define rRXDESCR        (*((volatile unsigned int *) 0xFFE00108))       /* lpc23xx only */
#define rRXSTATU        (*((volatile unsigned int *) 0xFFE0010C))       /* lpc23xx only */
#define rRXDENUM        (*((volatile unsigned int *) 0xFFE00110))       /* lpc23xx only */
#define rRXPROIN        (*((volatile unsigned int *) 0xFFE00114))       /* lpc23xx only */
#define rRXCONIN        (*((volatile unsigned int *) 0xFFE00118))       /* lpc23xx only */
#define rTXDESCR        (*((volatile unsigned int *) 0xFFE0011C))       /* lpc23xx only */
#define rTXSTATU        (*((volatile unsigned int *) 0xFFE00120))       /* lpc23xx only */
#define rTXDENUM        (*((volatile unsigned int *) 0xFFE00124))       /* lpc23xx only */
#define rTXPROIN        (*((volatile unsigned int *) 0xFFE00128))       /* lpc23xx only */
#define rTXCONIN        (*((volatile unsigned int *) 0xFFE0012C))       /* lpc23xx only */
#define rTSV0           (*((volatile unsigned int *) 0xFFE00158))       /* lpc23xx only */
#define rTSV1           (*((volatile unsigned int *) 0xFFE0015C))       /* lpc23xx only */
#define rRSV            (*((volatile unsigned int *) 0xFFE00160))       /* lpc23xx only */
#define rFLOWCTRLCNT    (*((volatile unsigned int *) 0xFFE00170))       /* lpc23xx only */
#define rFLOWCTRLSTA    (*((volatile unsigned int *) 0xFFE00174))       /* lpc23xx only */
/* RX FILTER REGISTER */
#define rFILTERCTRL     (*((volatile unsigned int *) 0xFFE00200))       /* lpc23xx only */
#define rFILTERWOLSTA   (*((volatile unsigned int *) 0xFFE00204))       /* lpc23xx only */
#define rFILTERWOLCLR   (*((volatile unsigned int *) 0xFFE00208))       /* lpc23xx only */
#define rHASHFILTER1    (*((volatile unsigned int *) 0xFFE00210))       /* lpc23xx only */
#define rHASHFILTER2    (*((volatile unsigned int *) 0xFFE00214))       /* lpc23xx only */
/* MODULE CONTROL REGISTER */
#define rEINTSTA        (*((volatile unsigned int *) 0xFFE00FE0))       /* lpc23xx only */
#define rEINTEN         (*((volatile unsigned int *) 0xFFE00FE4))       /* lpc23xx only */
#define rFINTCLR        (*((volatile unsigned int *) 0xFFE00FE8))       /* lpc23xx only */
#define rFINTSET        (*((volatile unsigned int *) 0xFFE00FEC))       /* lpc23xx only */
#define rEPOWERDOWN     (*((volatile unsigned int *) 0xFFE00FF4))       /* lpc23xx only */

#define rMAC_MODULEID   (*((volatile unsigned int *) 0xFFE00FFC))       /* lpc23xx only */

#define OLD_EMAC_MODULE_ID ( (0X3902<<16) | 0X2000 )   

#define EMAC_RAM_ADDR		0x7FE00000
#define EMAC_RAM_SIZE		0x00004000


#define PHILIPS_EMAC_MODULE_ID	((0x3902 << 16) | 0x2000)

/* The Ethernet RAM is configured as below, the starting of EMAC_DESCRIPTOR_ADDR depends
on the EMAC_DESCRIPTOR_COUNT or the TOTAL_DESCRIPTOR_SIZE, at this point, the 
EMAC_DESCRIPTOR_COUNT for both TX and RX is set to 16:

   EMAC_RAM_ADDR	0x7FE00000
   EMAC_DMA_ADDR	0x7FE00000
   .
   .
   .
   EMAC_DMA_END		EMAC_RAM_ADDR + EMAC_RAM_SIZE - TOTAL_DESCRIPTOR_SIZE 
   TX_DESCRIPTOR_ADDR = EMAC_DESCRIPTOR_ADDR = EMAC_RAM_END(EMAC_RAM_ADDR+EMAC_RAM_SIZE) - TOTAL_DESCRIPTOR 
   TX_STATUS_ADDR = TX_DESCRIPTOR_ADDR + TX_DESCRIPTOR_SIZE
   RX_DESCRIPTOR_ADDR = TX_DESCRIPTOR_ADDR + TX_DESCRIPTOR_SIZE + TX_STATUS_SIZE
   RX_STATUS_ADDR = RX_DESCRIPTOR_ADDR + RX_STATUS_SIZE
   ( RX_STATUS_ADDR + RX_STATUS_SIZE = EMAC_RAM_END )!!!!!
   EMAX_RAM_END	0x7FE04000

   Please note that, the descriptors have to be aligned to the 32 bit boundary!!!
   Below descriptor addresses have been carefully aligned to the 32-bit boundary. 
   If not, the descriptors have to be re-aligned!!! 
 */
//发送描述符数量
#define EMAC_TX_DESCRIPTOR_COUNT	 0x0004
#define TX_DESCRIPTOR_SIZE	(EMAC_TX_DESCRIPTOR_COUNT * 8)
#define TX_STATUS_SIZE		(EMAC_TX_DESCRIPTOR_COUNT * 4)
//接收描述符数量
#define EMAC_RX_DESCRIPTOR_COUNT	0x0004
#define RX_DESCRIPTOR_SIZE	(EMAC_RX_DESCRIPTOR_COUNT * 8)
#define RX_STATUS_SIZE		(EMAC_RX_DESCRIPTOR_COUNT * 8)
//全部描述符大小8*4+4*4+4*8+4*8=112
#define TOTAL_DESCRIPTOR_SIZE	(TX_DESCRIPTOR_SIZE + RX_DESCRIPTOR_SIZE + TX_STATUS_SIZE + RX_STATUS_SIZE)
//描述符地址=0x7FE03f90 (0x7FE0 0000+0x4000-)
#define EMAC_DESCRIPTOR_ADDR	(EMAC_RAM_ADDR + EMAC_RAM_SIZE - TOTAL_DESCRIPTOR_SIZE) 
//发送描述符地址=0x7FE03f90
#define TX_DESCRIPTOR_ADDR	 EMAC_DESCRIPTOR_ADDR
#define TX_STATUS_ADDR		(EMAC_DESCRIPTOR_ADDR + TX_DESCRIPTOR_SIZE)
#define RX_DESCRIPTOR_ADDR	(TX_STATUS_ADDR + TX_STATUS_SIZE)
#define RX_STATUS_ADDR		(RX_DESCRIPTOR_ADDR + RX_DESCRIPTOR_SIZE)

#define EMAC_DMA_ADDR		EMAC_RAM_ADDR
//#define EMAC_DMA_SIZE		EMAC_RAM_ADDR + EMAC_RAM_END - TOTAL_DESCRIPTOR_SIZE

#define EMAC_BLOCK_SIZE			0x600
#define EMAC_TX_BLOCK_NUM		4	
#define EMAC_RX_BLOCK_NUM		4
#define TOTAL_EMAC_BLOCK_NUM	8

#define EMAC_BUFFER_SIZE	(EMAC_BLOCK_SIZE * (EMAC_TX_BLOCK_NUM + EMAC_RX_BLOCK_NUM ))
#define EMAC_TX_BUFFER_ADDR	EMAC_RAM_ADDR
#define EMAC_RX_BUFFER_ADDR	(EMAC_RAM_ADDR + EMAC_BLOCK_SIZE * EMAC_TX_BLOCK_NUM)
/* EMAC Descriptor TX and RX Control fields */
#define EMAC_TX_DESC_INT		0x80000000  //
#define EMAC_TX_DESC_LAST		0x40000000
#define EMAC_TX_DESC_CRC		0x20000000
#define EMAC_TX_DESC_PAD		0x10000000
#define EMAC_TX_DESC_HUGE		0x08000000
#define EMAC_TX_DESC_OVERRIDE	0x04000000

#define EMAC_RX_DESC_INT		0x80000000

/* EMAC Descriptor status related definition */
#define TX_DESC_STATUS_ERR		0x80000000
#define TX_DESC_STATUS_NODESC	0x40000000
#define TX_DESC_STATUS_UNDERRUN	0x20000000
#define TX_DESC_STATUS_LCOL		0x10000000
#define TX_DESC_STATUS_ECOL		0x08000000
#define TX_DESC_STATUS_EDEFER	0x04000000
#define TX_DESC_STATUS_DEFER	0x02000000
#define TX_DESC_STATUS_COLCNT	0x01E00000	/* four bits, it's a mask, not exact count */

#define RX_DESC_STATUS_ERR		0x80000000
#define RX_DESC_STATUS_LAST		0x40000000
#define RX_DESC_STATUS_NODESC	0x20000000
#define RX_DESC_STATUS_OVERRUN	0x10000000
#define RX_DESC_STATUS_ALGNERR	0x08000000
#define RX_DESC_STATUS_RNGERR	0x04000000
#define RX_DESC_STATUS_LENERR	0x02000000
#define RX_DESC_STATUS_SYMERR	0x01000000
#define RX_DESC_STATUS_CRCERR	0x00800000
#define RX_DESC_STATUS_BCAST	0x00400000
#define RX_DESC_STATUS_MCAST	0x00200000
#define RX_DESC_STATUS_FAILFLT	0x00100000
#define RX_DESC_STATUS_VLAN		0x00080000
#define RX_DESC_STATUS_CTLFRAM	0x00040000

#define DESC_SIZE_MASK			0x000007FF	/* 11 bits for both TX and RX */

/* EMAC interrupt controller related definition */
#define EMAC_INT_RXOVERRUN	(1 << 0)
#define EMAC_INT_RXERROR		(1 << 1)
#define EMAC_INT_RXFINISHED	(1 << 2)
#define EMAC_INT_RXDONE		(1 << 3) 
#define EMAC_INT_TXUNDERRUN	(1 << 4)
#define EMAC_INT_TXERROR		(1 << 5 )
#define EMAC_INT_TXFINISHED	(1 << 6)
#define EMAC_INT_TXDONE		(1 << 7 )
#define EMAC_INT_SOFTINT		(1 << 12)
#define EMAC_INT_WOL			(1 << 13 )

///////////////////////////////////////////////////////////////
void SetMacID(uint8 * mac_ptr);
void Send_Packet(struct _pkst *TxdData);//发送数据到DMA
void InitNet(void);
void Rec_Packet(uint32 IntStatus); 
uint16 swap_int16(uint16 temp);
uint32 swap_int32(uint32 temp);
#endif

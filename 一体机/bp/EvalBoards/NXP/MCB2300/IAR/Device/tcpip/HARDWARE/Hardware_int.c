#define HARDWARE_GLOBALS
#include "includes.h"
#include "../include/cfg_net.h"
#include 	"LPC23XX_ADD_R.h"


//uint32 ETXCount=0;
//uint32 ERXCount=0;
uint16 PHYID;
/////////////////////////////////////////////////////////////////////
//	函数名称：void EMAC_TxEnable( void )
//	函数功能：网络发送允许
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void EMAC_TxEnable( void )
{
    MAC_COMMAND |= 0x02;
    return;
}

void EMAC_TxDisable( void )
{
    MAC_COMMAND &= ~0x02;
    return;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：void EMAC_RxEnable( void )
//	函数功能：网络发送允许
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void EMAC_RxEnable( void )
{
    MAC_COMMAND |= 0x01;
    MAC_MAC1 |= 0x01;
    return;
}

void EMAC_RxDisable( void )
{
    MAC_COMMAND &= ~0x01;
    MAC_MAC1 &= ~0x01;
    return;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：void Write_PHY (uint16 phyadd,int PhyReg, int Value)
//	函数功能：PHY读写
//	入口参数：phyadd  PHY物理地址
//			 phyReg  PHY寄存器地址
//			 Value         数值
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Write_PHY (uint16 phyadd, int PhyReg, int Value)
{
    unsigned int tout;
    MAC_MCMD = 0x0000;
    MAC_MADR = (phyadd << 8) | PhyReg;
    MAC_MWTD = Value;

    /* Wait utill operation completed */
    tout = 0;
    for (tout = 0; tout < 50000; tout++)
    {
        if ((MAC_MIND & 1) == 0)
        {
            break;
        }
    }
}
/////////////////////////////////////////////////////////////////////
//	函数名称：unsigned short Read_PHY ( uint16 phyadd ,unsigned char PhyReg)
//	函数功能：PHY读
//	入口参数：phyadd  PHY物理地址
//			 phyReg  PHY寄存器地址
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
unsigned short Read_PHY ( uint16 phyadd , unsigned char PhyReg)
{
    unsigned int tout;

    MAC_MCMD = 1;
    MAC_MADR = (phyadd << 8) | PhyReg;

    /* Wait until operation completed */
    for (tout = 0; tout < 50000; tout++)
    {
        if ((MAC_MIND & 1) == 0)
        {
            break;
        }
    }
    MAC_MCMD = 0;
    return (MAC_MRDD);
}
/////////////////////////////////////////////////////////////////////
//	函数名称：void EMACTxDescriptorInit( void )
//	函数功能：初始化发送描述符
//	入口参数：无
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void EMACTxDescriptorInit( void )
{
    uint32 i;
    uint32 *tx_desc_addr, *tx_status_addr;

    /*-----------------------------------------------------------------------------
     * setup the Tx status,descriptor registers --
     * Note, the actual tx packet data is loaded into the ahb2_sram16k memory as part
     * of the simulation
     *----------------------------------------------------------------------------*/
    MAC_TXDESCRIPTOR = TX_DESCRIPTOR_ADDR;	/* Base addr of tx descriptor array */
    MAC_TXSTATUS = TX_STATUS_ADDR;			/* Base addr of tx status */
    MAC_TXDESCRIPTORNUM = EMAC_TX_DESCRIPTOR_COUNT - 1;	/* number of tx descriptors, 16 */

    for ( i = 0; i < EMAC_TX_DESCRIPTOR_COUNT; i++ )
    {
        tx_desc_addr = (uint32 *)(TX_DESCRIPTOR_ADDR + i * 8);	/* two words at a time, packet and control */
        *tx_desc_addr = (uint32)(EMAC_TX_BUFFER_ADDR + i * EMAC_BLOCK_SIZE);
        *(tx_desc_addr + 1) = (uint32)(EMAC_TX_DESC_INT | (EMAC_BLOCK_SIZE - 1));	/* set size only */
    }

    for ( i = 0; i < EMAC_TX_DESCRIPTOR_COUNT; i++ )
    {
        tx_status_addr = (uint32 *)(TX_STATUS_ADDR + i * 4);	/* TX status, one word only, status info. */
        *tx_status_addr = (uint32)0;		/* initially, set status info to 0 */
    }
    MAC_TXPRODUCEINDEX = 0x0;	/* TX descriptors point to zero */
    return;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：void EMACRxDescriptorInit( void )
//	函数功能：初始化接收描述符
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void EMACRxDescriptorInit( void )
{
    uint32 i;
    uint32 *rx_desc_addr, *rx_status_addr;

    /*-----------------------------------------------------------------------------
     * setup the Rx status,descriptor registers --
     * Note, the actual rx packet data is loaded into the ahb2_sram16k memory as part
     * of the simulation
     *----------------------------------------------------------------------------*/
    MAC_RXDESCRIPTOR = RX_DESCRIPTOR_ADDR;	/* Base addr of rx descriptor array */
    MAC_RXSTATUS = RX_STATUS_ADDR;			/* Base addr of rx status */
    MAC_RXDESCRIPTORNUM = EMAC_RX_DESCRIPTOR_COUNT - 1;	/* number of rx descriptors, 16 */

    for ( i = 0; i < EMAC_RX_DESCRIPTOR_COUNT; i++ )
    {
        /* two words at a time, packet and control */
        rx_desc_addr = (uint32 *)(RX_DESCRIPTOR_ADDR + i * 8);
        *rx_desc_addr = (uint32)(EMAC_RX_BUFFER_ADDR + i * EMAC_BLOCK_SIZE);
        *(rx_desc_addr + 1) = (uint32)(EMAC_RX_DESC_INT | (EMAC_BLOCK_SIZE - 1));	/* set size only */
    }

    for ( i = 0; i < EMAC_RX_DESCRIPTOR_COUNT; i++ )
    {
        /* RX status, two words, status info. and status hash CRC. */
        rx_status_addr = (uint32 *)(RX_STATUS_ADDR + i * 8);
        *rx_status_addr = (uint32)0;	/* initially, set both status info and hash CRC to 0 */
        *(rx_status_addr + 1) = (uint32)0;
    }
    MAC_RXCONSUMEINDEX = 0x0;	/* RX descriptor points to zero */
    return;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：void SetMacID(uint8 * mac_ptr)
//	函数功能：设置物理地址
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void SetMacID(uint8 *mac_ptr)
{

    MAC_SA0 = mac_ptr[1] * 256 + mac_ptr[0];
    MAC_SA1 = mac_ptr[3] * 256 + mac_ptr[2];
    MAC_SA2 = mac_ptr[5] * 256 + mac_ptr[4];


}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Send_Packet(struct _pkst *TxdData)//
{
    uint32 *tx_desc_addr, *tx_status, count;
    uint32 TxProduceIndex;
    uint32  templen, tmp;
    struct _pkst *pd;
    BYTE *Tx_buf;

    TxProduceIndex = MAC_TXPRODUCEINDEX;
    tmp = (TxProduceIndex + 1) % EMAC_TX_DESCRIPTOR_COUNT;
    if (tmp == MAC_TXCONSUMEINDEX) //数据缓冲区满
    {
        for (count = 0; count < 80000; count++)
        {
            TxProduceIndex = MAC_TXPRODUCEINDEX;
            tmp = (TxProduceIndex + 1) % EMAC_TX_DESCRIPTOR_COUNT;
            if (tmp != MAC_TXCONSUMEINDEX) //数据缓冲区满
                break;

        }
        if (tmp == MAC_TXCONSUMEINDEX) //数据缓冲区满
            return ;
    }
    if ( TxProduceIndex >= EMAC_TX_DESCRIPTOR_COUNT )
    {
        MAC_TXPRODUCEINDEX = MAC_TXCONSUMEINDEX;
        TxProduceIndex = MAC_TXCONSUMEINDEX;
    }
    tx_status = (uint32 *)(TX_STATUS_ADDR + TxProduceIndex * 4);
    if (*tx_status & TX_DESC_STATUS_ERR)
    {
        tx_desc_addr = (uint32 *)(TX_DESCRIPTOR_ADDR + TxProduceIndex * 8);
        *tx_desc_addr = EMAC_TX_BUFFER_ADDR + TxProduceIndex * EMAC_BLOCK_SIZE; //描述符数据地址
        *(tx_desc_addr + 1) |= (EMAC_TX_DESC_INT | EMAC_TX_DESC_LAST | EMAC_TX_DESC_PAD);
        tx_status = (uint32 *)(TX_STATUS_ADDR + TxProduceIndex * 4);
        *tx_status = 0;
    }
    else
    {
        Tx_buf = (BYTE *)(EMAC_TX_BUFFER_ADDR + TxProduceIndex * EMAC_BLOCK_SIZE);
        templen = 0;
        //拷贝数据
        for (pd = TxdData; pd != NULL; pd = pd->STPTR)
        {
            memcpy((BYTE *)(Tx_buf + templen), (BYTE *)pd->DAPTR, pd->length);
            templen += pd->length;
        }
        // if (templen<64) templen=64 ;
        //发送本报
        //	ETXCount++;
        tx_status = (uint32 *)(TX_STATUS_ADDR + TxProduceIndex * 4);
        *tx_status = 0;
        tx_desc_addr = (uint32 *)(TX_DESCRIPTOR_ADDR + TxProduceIndex * 8);
        *tx_desc_addr = (uint32)(Tx_buf);//描述符数据地址
        *(tx_desc_addr + 1) = (uint32)(EMAC_TX_DESC_INT | EMAC_TX_DESC_LAST   | (templen - 1));
    }
    //修改指针
    TxProduceIndex++;		/* transmit now */
    TxProduceIndex = TxProduceIndex % EMAC_TX_DESCRIPTOR_COUNT;
    MAC_TXPRODUCEINDEX = TxProduceIndex;

}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void EMACHandler (uint32 IntStatus)
{


    if ( IntStatus & EMAC_INT_RXOVERRUN )
    {
        EMACRxDescriptorInit();

    }

    if ( IntStatus & EMAC_INT_RXERROR )
    {

    }

    if ( IntStatus & EMAC_INT_RXFINISHED )
    {
    }

    if ( IntStatus & EMAC_INT_TXUNDERRUN )
    {
    }
    if ( IntStatus & EMAC_INT_TXERROR )
    {
    }

    if ( IntStatus & EMAC_INT_TXFINISHED )
    {
    }

    return;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：uint8 Rec_Packet(uint32 IntStatus)
//	函数功能：接收数据报
//	入口参数：IntStatus=中断状态
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Rec_Packet(uint32 IntStatus)
{
    uint32  RxConsumeIndex;//,Rxs;
    uint8 *pdata;
    uint32 RxSize;
    uint32 *RX_status, *RX_desc, *RX_data;
    RxConsumeIndex = MAC_RXCONSUMEINDEX;
    //数据接收指针超界
    if ( RxConsumeIndex >= EMAC_RX_DESCRIPTOR_COUNT )
    {
        RxConsumeIndex = MAC_RXCONSUMEINDEX = 0;

    }
    for (; RxConsumeIndex !=   MAC_RXPRODUCEINDEX;) //此时表示有新的数据包在缓冲区里
    {

        RX_status = (uint32 *)(RX_STATUS_ADDR + RxConsumeIndex * 8);
        RX_desc = (uint32 *)(RX_DESCRIPTOR_ADDR + RxConsumeIndex * 8);
        RX_data = (uint32 *)(*RX_desc);
        RxSize = (*RX_status & DESC_SIZE_MASK) + 1;
        pdata = (BYTE *)RX_data;

        if (*RX_status & RX_DESC_STATUS_LAST) //一个完整的数据报
        {
            //ERXCount++;
            Ethernet_ReceivePacket(pdata);
        }
        *RX_status = 0;
        *(RX_desc + 1) = (uint32)(EMAC_RX_DESC_INT | (EMAC_BLOCK_SIZE - 1));	/* set size only */
        RxConsumeIndex++;
        RxConsumeIndex %= EMAC_RX_DESCRIPTOR_COUNT;
        MAC_RXCONSUMEINDEX = RxConsumeIndex;

    }//while( RxConsumeIndex !=   MAC_RXPRODUCEINDEX);
    RxSize = RxSize;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：网络中断处理
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Ethernet_Exception(void)
{
    uint32 intstatus;
    //extern OS_EVENT  		*RecPackedFlag;

    //OS_ENTER_CRITICAL();
    intstatus = MAC_INTSTATUS;
    if ( intstatus & EMAC_INT_RXDONE )
        Rec_Packet(intstatus);
    else
    {
        EMACHandler(intstatus);
    }
    MAC_INTCLEAR = 0XFF;
    //	OS_EXIT_CRITICAL();
    //VICVectAddr = 0;            //interrupt close 通知中断控制器中断结束
}
/////////////////////////////////////////////////////////////////////
//	函数名称：void InitNic(uint8 num)
//	函数功能：芯片初始化
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
//extern NetP NetPort;
void InitNet(void)
{
    uint32 i;
#if DBG_TEST 	>0
    PCONP |= 0X40000000;
    /* reset : soft,simulation,mcs/rx,rx,mcs/tx,tx */
    MAC_MAC1 = 0xCF00;			/* RegReset,TxReset,RxReset *//* [15],[14],[11:8] -> soft resets all MAC internal modules */
    MAC_COMMAND = 0x0038;		/* reset all datapaths and host registers */
    for (i = 0; i < 0x40; i++);	/* short delay after reset */
    MAC_MAC1 = 0x0;				/* deassert all of the above soft resets in MAC1 */
    EMAC_TxDisable();
    EMAC_RxDisable();
    MAC_MAC2 = 0x00;		/* initialize MAC2 register to default value */
    /* Non back to back inter-packet gap */
    MAC_IPGR = 0x0012;		/* use the default value recommended in the users manual */
    MAC_CLRT = 0x370F;		/* Use the default value in the users manual */
    MAC_MAXF = 0x0600;		/* Use the default value in the users manual */

    /* PHY Select RMII */
    MAC_MCFG |= 0x001c;
    /* RMII configuration */
    MAC_COMMAND = 0x0240;
    /*  (note bit 4 was set in original test, although spec says its unused) */
    MAC_SUPP = 0x0000;

    // probe phy address
    do
    {
        for(i = 0; i < 32; i++)
        {
            PHYID = Read_PHY(i , 2 );
            if(PHYID == 0X0181)
                break;
        }
        if(i < 32)  break;
        Device_IC_Reset();
    }
    while(1);
    PHYID = i;
    /// PHY RESET
    Write_PHY(PHYID, 0, 0x9200 );
    /* INPUT MAC ADDRESS */
    SetMacID(ARP_Rec_Find(Get_DesIp(DEVICE_SUB_CLASS))); //设置邋ARm EMC物理地址
    ///////////////////
    //uint8 mac[6];
    //mac[0]=0x00;//0xa1
    //mac[1]=0x1b;//0x52
    //mac[2]=0x24;//pnet->My_Ip[0];
    //mac[3]=0x82;//pnet->My_Ip[1];
    //mac[4]=0x47;//pnet->My_Ip[2];
    //mac[5]=0xcd;//pnet->My_Ip[3];
    //SetMacID( &mac [0]);
    /////////////////////
    MAC_MAC2 = 0x31;			/* full duplex, CRC and PAD enabled. */
    MAC_SUPP |= 0x0100;			/* RMII Support Reg. speed is set to 100M */
    MAC_COMMAND |= 0x0640;
    /* back to back int-packet gap */
    MAC_IPGT = 0x0015;			/* IPG setting in full duplex mode */
    EMACTxDescriptorInit();
    EMACRxDescriptorInit();
    MAC_MAC1 |= 0x0002;			/* [1]-Pass All Rx Frame */

    /* Set up RX filter, accept broadcast and perfect station */
    // MAC_RXFILTERCTRL = 0x0024;		//22	/* [1]-accept broadcast, [5]accept perfect */
    MAC_RXFILTERCTRL = 0x002b;		//22	/* [1]-accept broadcast, [5]accept perfect */
    //MAC_RXFILTERCTRL |= 0x0001;		//MULTICAST_UNICAST
    //MAC_RXFILTERCTRL |= 0x0008;		//ENABLE_HASH
    MAC_INTCLEAR = 0xFFFF;	/* clear all MAC interrupts */
    EMAC_RxEnable();
    EMAC_TxEnable();
    SetVICIRQ(VIC_CHANLE_ETHEMET, 8, (unsigned int)Ethernet_Exception);
    MAC_INTENABLE = 0x000c;	/* Enable all interrupts except SOFTINT and WOL */
#endif
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint16 swap_int16(uint16 temp)
{
    uint16 temp1;
    temp1 = (temp & 0xff00) >> 8;
    temp = (temp & 0x00ff) << 8;
    return(temp + temp1);
}

uint32 swap_int32(uint32 temp)
{
    union   w  temp232, temp132;
    temp232.dwords = temp;
    temp132.bytes.byte0 = temp232.bytes.byte3;
    temp132.bytes.byte1 = temp232.bytes.byte2;
    temp132.bytes.byte2 = temp232.bytes.byte1;
    temp132.bytes.byte3 = temp232.bytes.byte0;
    return(temp132.dwords);
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint16 Char2ToInt16(uint8 *temp)
{
    uint16 temp16;
    temp16 = ((uint16)(*temp)) << 8;
    temp++;
    temp16 = temp16 + (uint16)(*temp);
    return(temp16);
}

uint32 Char4ToInt32(uint8 *temp)
{
    uint32 temp32;
    temp32 = ((uint32)(*temp)) << 24;
    temp++;
    temp32 = temp32 + (((uint32)(*temp)) << 16);
    temp++;
    temp32 = temp32 + (((uint32)(*temp)) << 8);
    temp++;
    temp32 = temp32 + (uint32)(*temp);
    return(temp32);
}


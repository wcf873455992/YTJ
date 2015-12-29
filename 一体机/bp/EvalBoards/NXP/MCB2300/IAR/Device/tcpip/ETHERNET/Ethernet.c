#define ETHERNET_GLOBALS
#include "includes.h"
#include "../include/cfg_net.h"

/////////////////////////////////////////////////////////////////////
//	函数名称：void Ethernet_SendFrame
//	函数功能：IP、ARP数据包产生ETHERNET包头，并发送。
//	入口参数：struct _pkst *TxdData,//结构指针
//			uint8 * de_mac,			//对方的MAC地址指针
//			uint8 PROTOCOL			//IP协议或ARP协议
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Ethernet_SendFrame(
    struct _pkst *TxdData,	//结构指针
    uint8 *des_mac,		//对方的MAC地址指针
    uint8 PROTOCOL			//IP协议或ARP协议
)
{
#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register        */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                        */
#endif
    ipethernet ethernet_head; //ETHERNET处理缓存区
    struct _pkst PKethernet;
    BYTE *p;
    p = ARP_Rec_Find(Get_DesIp(DEVICE_SUB_CLASS));
    memcpy((BYTE *)ethernet_head.DestMacId, des_mac, 6); //设置对方MAC
    memcpy((BYTE *)ethernet_head.SourceMacId, p, 6);
    if(PROTOCOL == IP_PACKED)					//如果是IP包，就设为0X0800
        ethernet_head.NextProtocal = 0X0008;
    else if(PROTOCOL == ARP_PACKED)			//如果是ARP包，就设为0X0806
        ethernet_head.NextProtocal = 0X0608;	//0X0806;
    PKethernet.STPTR = TxdData;				//指向前一个结构数组
    PKethernet.length = 14;					//ETHERNET报头的长度
    PKethernet.DAPTR = (uint8 EX_RAM *)&ethernet_head; //ETHERNET报头的指针
    OS_ENTER_CRITICAL();		//保护
#if DBG_TEST >0
    Send_Packet(&PKethernet);	//发送该帧
#endif
    OS_EXIT_CRITICAL();			//恢复
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：接收中断数据报
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Ethernet_ReceivePacket(uint8 *RecData)
{
    if(((ipethernet *)RecData)->NextProtocal == 0x0608) //可以减少对数据缓冲取的利用
    {
        RecData = RecData + 14;
        ARP_ReceivePacket(RecData);
    }
    else if(((ipethernet *)RecData)->NextProtocal == 0x0008) //可以减少对数据缓冲取的利用
    {
        RecData = RecData + 14;

        IP_ReceivePacket(RecData);
    }
    return TRUE;
}

#ifndef __ETHERNET_H
#define __ETHERNET_H
#define IP_PACKED		0x01
#define ARP_PACKED		0x02


typedef struct{
			uint8 DestMacId[6];    	/*目的网卡地址*/
			uint8 SourceMacId[6];   /*源网卡地址*/
			uint16 NextProtocal;    /*下一层协议*/
}ipethernet;

typedef struct{
			uint8 IP[4];
}IP_NUMBER;

void Ethernet_SendFrame(
					 	struct _pkst *TxdData,	//结构指针
					 	uint8 * des_mac,		//对方的MAC地址指针
					 	uint8 PROTOCOL			//IP协议或ARP协议
				 		);// 硬件桢发送
uint8 Ethernet_ReceivePacket(uint8 * RecData);//硬件桢接收				 

#endif

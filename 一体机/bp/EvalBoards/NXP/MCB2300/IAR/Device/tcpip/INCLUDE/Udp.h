#ifndef __UDP_H
#define __UDP_H

#define UdpMaxRec		2
#define MAX_UDP_LINKS	2

typedef struct
{
    uint8  rev;
    uint8  NextProtocal;
    uint16 TotalLen;         	/*校验和,包括伪头部,udp头部,数据*/
    uint8  SourceIp[4];  	   /*源端口*/
    uint8  DestId[4];     	   /*目的端口*/
} udp_dhead;

typedef struct
{
    uint16 SourcePort;		/*源端口*/
    uint16 DestPort;    	/*目的端口*/
    uint16 length;			/*长度UDP头＋数据*/
    uint16 Crc;        	   	/*校验和,包括伪头部,udp头部,数据*/
    //uint8  udpdata[1478];   /*udp数据*/
} udp_data;

typedef struct
{
    udp_dhead UdpDHead;
    udp_data  Udp;
} udp_send;

typedef struct
{
    uint16 De_Port;
    uint8  De_Ip[4];
    uint16 len;
    uint8  num;
} udp_sub_socket;

typedef struct
{
    uint16 My_Port;
    uint8  My_Ip[4];
    uint8  Rec_Sta;
    udp_sub_socket info[UdpMaxRec];
    OS_EVENT *UdpSemRec;
} udp_socket;
/////////////////////////////////////////////////////////////
uint8 Udp_SendPacked(struct _pkst *TxData, BYTE *desip, uint16 desport);
uint8 UDP_ReceivePacket(BYTE *pdata);//UDP报处理;
#endif

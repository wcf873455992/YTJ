#include "includes.h"
#include "../include/cfg_net.h"
NetP NetPort;
uint8 ARP_REP_MAC[6]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
extern  void IP_Debug_init(void);
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Initial_arp(void)  
{
	uint8 i;
	// struct CLASS_INFO *pclass;
	NetPort.CMD_IP_TTL=0;
	NetPort.Gateway_IP_TTL=0;
	for(i=0;i<MAX_ARP_TERM;i++)
	{
		NetPort.ARP_TERM[i].TTL=0;
	}
	NetPort.Test_TTL=0;
	Arp_Request(NetPort.My_Gateway);
	Arp_Request(NetPort.Ser_Ip);
	UART_printf("User init net--Run\n");
	IP_Debug_init();	
	IP_printf("网络初始化完成\n\t");
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE *Get_NetPort(void)
{
	return (BYTE *)&NetPort;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE *Get_ClassID(void)
{
	return (BYTE *)NetPort.Class_ID;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void IPDword_TO_Byte(BYTE *pdata,uint32 ip)
{
	*pdata=ip>>24;
	*(pdata+1)=(ip>>16)&0xff;
	*(pdata+2)=(ip>>8)&0xff;
	*(pdata+3)=(ip)&0xff;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 IPByte_TO_Dword(BYTE *pdata)
{
	uint32 tmp;
	tmp=*pdata<<24;
	tmp+=*(pdata+1)<<16;
	tmp+=*(pdata+2)<<8;
	tmp+=*(pdata+3);
	return tmp;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：获得发送目标的邋IP地址
//	入口参数：num:编号
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE *Get_DesIp(uint8 num)
{
	switch(num)	
	{
	case DEVICE_SUB_CLASS: //一体机
		return (BYTE *)NetPort.My_Ip;
	case DEVICE_SUB_GATE://			0x26//网关	
	 	return (BYTE *)NetPort.My_Gateway;
	case  DEVICE_SUB_SER://			0x36//集控服务器
	 	return (BYTE *)NetPort.Ser_Ip;
	case  DEVICE_SUB_LMT://			0x46//流媒体
	 	return (BYTE *)NetPort.LMT_Ip;
	case DEVICE_SUB_PC://			0x56//课件计算机	
	 	return (BYTE *)NetPort.PC_Ip;
	case DEVICE_SUB_CMD:
		return (BYTE *)NetPort.CMD_IP;
	case DEVICE_SUB_CAST:		
		return (BYTE *)ARP_REP_MAC;
	case DEVICE_SUB_TEST:
		if (NetPort.Test_TTL!=0)		
			return (BYTE *)NetPort.Test_Ip;
		else
			return (BYTE *)ARP_REP_MAC;	
	case DEVICE_SUB_BJCard:
		return (BYTE *)NetPort.BJ_CardIP;  //编辑卡IP
	}		
	return NULL;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE IP_Compare(BYTE *desip1,BYTE *desip2)
{
	if ((*desip1++==*desip2++)&&(*desip1++==*desip2++)&&
		(*desip1++==*desip2++)&&(*desip1++==*desip2++))	
		{return TRUE;}
	else
		return FALSE;

}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：查找MAC地址
//	入口参数：IP
//			
//			
//	出口参数：MAC地址指针,没有找到返回NULL
////////////////////////////////////////////////////////////////////
BYTE *ARP_Rec_Find(BYTE *desip)
{
	BYTE i;
	//检查本机地址
	if (IP_Compare( desip,NetPort.My_Ip)){
		return (BYTE *)NetPort.My_Mac;
	}
	//检查广播地址
	if (IP_Compare( desip,ARP_REP_MAC))
		return (BYTE *)ARP_REP_MAC;
	//检查网关地址
	if (IP_Compare( desip,NetPort.My_Gateway)){
		if (NetPort.Gateway_IP_TTL != 0)
			return (BYTE *)NetPort.My_Gateway_Mac;
		else
		 	return NULL;
	}
	//检查本网段地址
	if (IPMASK_Compare(desip)){
		for(i=0;i<MAX_ARP_TERM;i++)
		{
			if (NetPort.ARP_TERM[i].TTL != 0) 
				if (IP_Compare(desip,(BYTE *)NetPort.ARP_TERM[i].IP_NUM))
					return (BYTE *)NetPort.ARP_TERM[i].MAC_NUM;			
		}
		return NULL;
	}
	else
		return (BYTE *)NetPort.My_Gateway_Mac;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE ARP_Rec_Save(BYTE *desip,BYTE *desMac)
{ 
	BYTE i;
	if (ARP_Rec_Find(desip) != NULL) 
		return TRUE;
	for (i=0;i<MAX_ARP_TERM;i++)
	{	
		if (NetPort.ARP_TERM[i].TTL == 0){
			memcpy(NetPort.ARP_TERM[i].IP_NUM,desip,4);
			memcpy(NetPort.ARP_TERM[i].MAC_NUM,desMac,6);
			NetPort.ARP_TERM[i].TTL=100;
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：BYTE IPMASK_Compare(BYTE *desip)
//	函数功能：比较此ip是否是本网段
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE IPMASK_Compare(BYTE *desip)
{
	if((*desip&NetPort.My_Ip_Mark[0])==(NetPort.My_Ip[0]&NetPort.My_Ip_Mark[0]))
	if((*(desip+1)&NetPort.My_Ip_Mark[1])==(NetPort.My_Ip[1]&NetPort.My_Ip_Mark[1]))
	if((*(desip+2)&NetPort.My_Ip_Mark[2])==(NetPort.My_Ip[2]&NetPort.My_Ip_Mark[2]))
	if((*(desip+3)&NetPort.My_Ip_Mark[3])==(NetPort.My_Ip[3]&NetPort.My_Ip_Mark[3]))
		return TRUE;
	
	return FALSE;
}

void Arp_reg()
{
	struct _pkst TxdArpReq;
	uint8  ARPREQ [0x60];
	uint8 SEND_ARP_MAC[6];
	uint8 i;

	if (0 == NetPort.Gateway_IP_TTL){
		NetPort.Gateway_IP_TTL = 10;
		NetPort.My_Gateway_Mac[0] = 0;
		NetPort.My_Gateway_Mac[1] = 0x1c;
		NetPort.My_Gateway_Mac[2] = 0xf0;
		NetPort.My_Gateway_Mac[3] = 0x45;
		NetPort.My_Gateway_Mac[4] = 0x01;
		NetPort.My_Gateway_Mac[5] = 0xec;
	}
	//return;
	
	for(i=0;i<6;i++)			  					//复制对方网卡地址或网关地址
	{											
		((arp*)ARPREQ)->SourceMacId[i]=NetPort.My_Mac[i];

		if (NetPort.Gateway_IP_TTL != 0){
			SEND_ARP_MAC[i] = NetPort.My_Gateway_Mac[i];
			((arp*)ARPREQ)->DestMacId[i]=NetPort.My_Gateway_Mac[i];	//arp报文的目的物理地址填为0,由arp回答报文负责填充
		}
           }
	for(i=0;i<4;i++)								
	{
		((arp*)ARPREQ)->SourceIp[i]=NetPort.My_Ip[i];//填充源IP地址
		((arp*)ARPREQ)->DestId[i]=NetPort.My_Gateway[i];//填充目的IP地址
		//ip_address++;
	}
	((arp*)ARPREQ)->HardwareType=0x0100;		//硬件类型：0x0001,以太网类型
	((arp*)ARPREQ)->ProtocalType=0x0008;		//协议类型：0x0800,对应IPv4
	((arp*)ARPREQ)->Operation=0x0200;			//操作类型：ARP请求
	((arp*)ARPREQ)->HardwareLen=0x06;			//硬件长度：即物理地址长度，单位字节
	((arp*)ARPREQ)->ProtocalLen=0x04;			//协议长度：即逻辑地址长度，单位字节
	ARPREQ[28]=0x00;							//填充数据为0x00
	//启动发送数据,发送的是一个arp请求.
	((arp*)ARPREQ)->Operation=0x0200;	//表明数据帧为ARP应答
	TxdArpReq.STPTR=NULL;
	TxdArpReq.length=0x60;
	TxdArpReq.DAPTR=ARPREQ;
	Ethernet_SendFrame(&TxdArpReq,SEND_ARP_MAC,ARP_PACKED);//发送ARP应答帧
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：ARP响应命令
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Arp_Answer(uint8 * ARP_REC_PTR) 
{
	struct _pkst TxdArp;
	uint8 SEND_ARP_MAC[6];
	uint8 i;
	//如果目标IP地址是本机IP

	if (IP_Compare(((arp*)ARP_REC_PTR)->DestId, NetPort.My_Ip)){
		if (IPMASK_Compare(((arp*)ARP_REC_PTR)->SourceIp))	//如果发送方属于本网段
			ARP_Rec_Save(((arp*)ARP_REC_PTR)->SourceIp,((arp*)ARP_REC_PTR)->SourceMacId);

		//表示是要解析本地IP的请求
		for(i=0; i<4; i++)						//复制对方IP地址，填充源地址
		{
			SEND_ARP_MAC[i]=((arp*)ARP_REC_PTR)->SourceIp[i];
			((arp*)ARP_REC_PTR)->SourceIp[i]=NetPort.My_Ip[i];
			((arp*)ARP_REC_PTR)->DestId[i]=SEND_ARP_MAC[i];
		}
		for(i=0; i<6; i++)
   		{									//复制对方物理地址或网关地址
   			SEND_ARP_MAC[i]=((arp*)ARP_REC_PTR)->SourceMacId[i];
			((arp*)ARP_REC_PTR)->SourceMacId[i]=NetPort.My_Mac[i];
			((arp*)ARP_REC_PTR)->DestMacId[i]=SEND_ARP_MAC[i];
   		}
		((arp*)ARP_REC_PTR)->Operation=0x0200;	//表明数据帧为ARP应答
		TxdArp.STPTR=NULL;
		TxdArp.length = 28;//0x60;
		TxdArp.DAPTR=ARP_REC_PTR;
		Ethernet_SendFrame(&TxdArp,SEND_ARP_MAC,ARP_PACKED);//发送ARP应答帧

		/*SEND_ARP_MAC[1] = 10;
		SEND_ARP_MAC[2] = 7;
		SEND_ARP_MAC[3] = 0;
		SEND_ARP_MAC[4] = 22;
		TxdArp.STPTR=NULL;
		TxdArp.length=1;
		TxdArp.DAPTR=&SEND_ARP_MAC[0];
		Udp_SendPacked(&TxdArp,&SEND_ARP_MAC[1],9001);
		*/
	}	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：uint8 REC_ARP_REQ(uint8 * ARP_REC_REQ_PTR,uint8 num)  
//	函数功能：保存ARP地址
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 REC_ARP_REQ(uint8 * ARP_REC_REQ_PTR)  
{
	uint8 i;
	if (IP_Compare(((arp*)ARP_REC_REQ_PTR)->SourceIp, NetPort.My_Gateway)){ 
		for (i=0; i<6; i++)			
			NetPort.My_Gateway_Mac[i]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[i];			
   		NetPort.Gateway_IP_TTL=100;	//表示网关地址已得到解析
	}		
	else{
		if (IPMASK_Compare(((arp*)ARP_REC_REQ_PTR)->SourceIp)&&IP_Compare(((arp*)ARP_REC_REQ_PTR)->DestId, NetPort.My_Ip))//如果发送方属于本网段
			ARP_Rec_Save(((arp*)ARP_REC_REQ_PTR)->SourceIp,((arp*)ARP_REC_REQ_PTR)->SourceMacId);	
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：* ARP_PTR	:接收到的ARP包的指针
//	函数功能：接受arp报，arp请求，应答
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void ARP_ReceivePacket(uint8 * ARP_PTR)
{
	if(((arp*)ARP_PTR)->Operation == 0X0100){
		Arp_Answer(ARP_PTR);
	}
	else if(((arp*)ARP_PTR)->Operation == 0X0200){
		REC_ARP_REQ(ARP_PTR);
	}
}

int crc_calc(char *frame_no_fcs, int frame_len)
{
	int i; 		// iterator
	int j; 		// another iterator
	char byte; 	// current byte
	int crc; 	// CRC result
	int q0, q1, q2, q3; // temporary variables
	
	crc = 0xFFFFFFFF;
	for (i=0; i<frame_len; i++)
	{
		byte = *frame_no_fcs++;
		for (j=0; j<2; j++) 
		{
			if (((crc >> 28) ^ (byte >> 3)) & 0x00000001){
				q3 = 0x04C11DB7;
			} 
			else{
				q3 = 0x00000000;
			}
			
			if (((crc >> 29) ^ (byte >> 2)) & 0x00000001){
				q2 = 0x09823B6E;
			} 
			else{
				q2 = 0x00000000;
			}
			
			if (((crc >> 30) ^ (byte >> 1)) & 0x00000001){
				q1 = 0x130476DC;
			} 
			else{
				q1 = 0x00000000;
			}
			
			if (((crc >> 31) ^ (byte >> 0)) & 0x00000001){
				q0 = 0x2608EDB8;
			}
			else{
				q0 = 0x00000000;
			}
			
			crc = (crc << 4) ^ q3 ^ q2 ^ q1 ^ q0;
			byte >>= 4;
		}
	}
	return crc;
}
 
/////////////////////////////////////////////////////////////////////
//	函数名称：ip_address: 要解析的IP地址
//	函数功能：解析ip地址
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Arp_Request(uint8 * ip_address)
{
	struct _pkst TxdArpReq;
	arp ArpTemp;
	uint8 ArpBuf[46];
	
	memset(ArpBuf, 0, 46);
	memset(ArpTemp.DestMacId, 0x00, 6);			//复制对方网卡地址或网关地址
	memcpy(ArpTemp.SourceMacId, NetPort.My_Mac, 6);//arp报文的目的物理地址填为0,由arp回答报文负责填充
	memcpy(ArpTemp.SourceIp, NetPort.My_Ip, 4);	//填充源IP地址
	memcpy(ArpTemp.DestId, ip_address, 4);			//填充目的IP地址

	ArpTemp.HardwareType = 0x0100;		//硬件类型：0x0001,以太网类型
	ArpTemp.ProtocalType = 0x0008;		//协议类型：0x0800,对应IPv4
	ArpTemp.Operation = 0x0100;		//操作类型：ARP请求
	ArpTemp.HardwareLen = 0x06;		//硬件长度：即物理地址长度，单位字节
	ArpTemp.ProtocalLen = 0x04;		//协议长度：即逻辑地址长度，单位字节
	memcpy(ArpBuf, &ArpTemp, sizeof(ArpTemp));

	//启动发送数据,发送的是一个arp请求.
	TxdArpReq.STPTR = NULL;
	TxdArpReq.length = 46;
	TxdArpReq.DAPTR = ArpBuf;
	Ethernet_SendFrame(&TxdArpReq,ARP_REP_MAC,ARP_PACKED);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE IP_Check(BYTE *pdata)
{
	eip  *Iprec;
	ipethernet *pether;
	Iprec=(eip *)pdata;
	pether=(ipethernet *)(pdata-14);
	
	if ((IP_Compare(Iprec->DestId,NetPort.My_Ip)==FALSE)&&(Iprec->DestId[3]!=0xff))
		 return FALSE;
	if (IPMASK_Compare(Iprec->SourceIp)&&(IP_Compare(pether->SourceMacId,ARP_REP_MAC)==FALSE) )
	{ 
		if (IP_Compare(Iprec->DestId,NetPort.My_Ip))
	 	  ARP_Rec_Save(Iprec->SourceIp,pether->SourceMacId);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void UDP_MAC_Save(BYTE *pdata)
{
	ipethernet *pn;
 	eip *pp;
 	pn=(ipethernet *)pdata;
 	pp=(eip *)(pdata+sizeof(ipethernet));
	if (IPMASK_Compare(pp->SourceIp) )
	{
		if ((IP_Compare(pp->SourceIp, ARP_REP_MAC)==FALSE)
		&&(IP_Compare(pn->SourceMacId,ARP_REP_MAC)==FALSE))
		 	ARP_Rec_Save(pp->SourceIp,pn->SourceMacId);
	}
}

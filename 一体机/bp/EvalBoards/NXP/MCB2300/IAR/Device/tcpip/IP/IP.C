#define IP_GLOBALS
#include "includes.h"
#include "../include/cfg_net.h"
extern uint8 ARP_REP_MAC[];
/**********************************************************************
**函数原型：    void 	CreateIpHeadCrc()
**入口参数:?无
**出口参数:		无
**返 回 值：	crc ：  16位的校验和        
**说    明：	产生IP首部的校验和 
************************************************************************/
uint16  CreateIpHeadCrc(uint8 * Ip) 
{
 union  w  CrcTemp;
uint8  i;
CrcTemp.dwords = 0;
	for(i=0;i<10;i++)
	{
		CrcTemp.dwords = CrcTemp.dwords + ((uint32)Ip[2*i]<<8)+(uint32)Ip[2*i+1];
	}
	while(CrcTemp.words.high>0) 
	{
    		CrcTemp.dwords = (uint32)(CrcTemp.words.high + CrcTemp.words.low);
    }
	CrcTemp.words.low = 0xffff - CrcTemp.words.low;  //取反
	return(CrcTemp.words.low);
}
 /////////////////////////////////////////////////////////////////////
//	函数名称：uint8 IP_SendFrame(struct _pkst *TxdData, //发送数据
//	函数功能：发送IP包
//	入口参数：* de_ip   	     	: IP地址指针
//			NextProtocal 		: 下一层协议，例如TCP协议或UDP协议
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 IP_SendFrame	(
				struct _pkst *TxdData, //发送数据
				uint8 * de_ip,				
				uint8 PROTOCOL
			) 
{
 union ip_rc IpHead;
uint8 IpHeadUint8[20];
struct _pkst TxdIpData,*psd;
uint8 *pMac,*pIp;
 uint16 FrameIndex=0;
uint16 sun_lg=0;
memset(IpHeadUint8,0,20);
if (PROTOCOL==1)//ICMP
	{
		//如果该包在
	if (IPMASK_Compare(de_ip))//是本网段的数据
	{pMac=ARP_Rec_Find(de_ip);
	  if (pMac!=NULL)
	  	{Ethernet_SendFrame(TxdData,pMac,IP_PACKED);
	  	  return TRUE;
	  	}else
	  		{
			Arp_Request(de_ip);//请求对方MAC
			return FALSE;
	  		}
	}else	//不是本网段的数据发给网关
		{pMac=(BYTE *)ARP_Rec_Find(Get_DesIp(DEVICE_SUB_GATE));
		 if (pMac!=NULL)
			 Ethernet_SendFrame(TxdData,pMac,IP_PACKED);
		 else
		 	{Arp_Request(Get_DesIp(DEVICE_SUB_GATE));//请求网关MAC
		 	}
		}
		return TRUE;

	}
for (psd=TxdData;psd!=NULL;psd=psd->STPTR)
	sun_lg+=psd->length;

	IpHead.e_ip.VerandIphLen=0x45;			//IP版本和头长度
	IpHeadUint8[0]=0x45;
	
	IpHead.e_ip.ServerType=0x00;                     //服务类型为0
	IpHeadUint8[1]=0x00;//服务类型
	
	IpHead.e_ip.TotalLen=sun_lg+20;//(*TxdData).length+20;			//IP数据报总长度
	IpHeadUint8[2]=((sun_lg+20)&0xff00)>>8;//IP数据报总长度高字节
	IpHeadUint8[3]=((sun_lg+20))&0x00ff;//IP数据报总长度低字节
	
	IpHead.e_ip.FrameIndex=FrameIndex+1;
	IpHeadUint8[4]=(FrameIndex&0xff00)>>8;//IP数据报标识高字节
	IpHeadUint8[5]=FrameIndex&0x00ff;//IP数据报标识低字节
	FrameIndex++;
	
	IpHeadUint8[6]=0x40;//IP数据报标志及分段偏移量
	IpHeadUint8[7]=0x00;//IP数据报标志及分段偏移量
	
	IpHead.e_ip.ttl=0x80;       //128;				//TTL
	IpHeadUint8[8]=0x80;//ttl

	IpHead.e_ip.NextProtocal=PROTOCOL;		//下层协议
    	IpHeadUint8[9]=PROTOCOL;
	
    	IpHead.e_ip.Crc=0;
	IpHeadUint8[10]=0;//CRC h
	IpHeadUint8[11]=0;//CRC l

	pIp=(BYTE *)Get_DesIp(DEVICE_SUB_CLASS);
	IpHead.e_ip.SourceIp[0]=*pIp;	//填充源IP地址
    	IpHead.e_ip.SourceIp[1]=*(pIp+1);
	IpHead.e_ip.SourceIp[2]=*(pIp+2);
	IpHead.e_ip.SourceIp[3]=*(pIp+3);
	
	IpHeadUint8[12]=*(pIp+0);	//填充源IP地址
    	IpHeadUint8[13]=*(pIp+1);
	IpHeadUint8[14]=*(pIp+2);
	IpHeadUint8[15]=*(pIp+3);

    	IpHead.e_ip.DestId[0]=de_ip[0];			//填充目的IP地址
    	IpHead.e_ip.DestId[1]=de_ip[1];
	IpHead.e_ip.DestId[2]=de_ip[2];
	IpHead.e_ip.DestId[3]=de_ip[3];

	IpHeadUint8[16]=de_ip[0];			//填充目的IP地址
    	IpHeadUint8[17]=de_ip[1];
	IpHeadUint8[18]=de_ip[2];
	IpHeadUint8[19]=de_ip[3];

    	IpHead.e_ip.Crc=CreateIpHeadCrc(IpHeadUint8);		//产生IP头的检验和
    	IpHeadUint8[10]=(IpHead.e_ip.Crc&0xff00)>>8;//IP数据报总长度高字节
	IpHeadUint8[11]=IpHead.e_ip.Crc&0x00ff;//IP数据报总长度低字节	

	TxdIpData.STPTR=TxdData;
	TxdIpData.length=20;
	TxdIpData.DAPTR=IpHeadUint8;
	//如果该包在		
	pMac=ARP_Rec_Find(de_ip);
	//pMac = NULL;
	if ( IPMASK_Compare(de_ip) )//是本网段的数据||pMac!=NULL
	{
	  if (pMac!=NULL)
	  	{Ethernet_SendFrame(&TxdIpData,pMac,IP_PACKED);
	  	  return TRUE;
	  	}else
	  		{
			Arp_Request(de_ip);//请求对方MAC
			return FALSE;
	  		}
	}else//不是广播地址	
	if (IP_Compare(de_ip,ARP_REP_MAC))
		Ethernet_SendFrame(&TxdIpData,pMac,IP_PACKED);
	else	
	//不是本网段的数据发给网关
		{
		
		pMac=(BYTE *)ARP_Rec_Find(Get_DesIp(DEVICE_SUB_GATE));
		 if (pMac!=NULL)
			 Ethernet_SendFrame(&TxdIpData,pMac,IP_PACKED);
		 else
		 	{Arp_Request(Get_DesIp(DEVICE_SUB_GATE));//请求网关MAC
		 	}
		}
		return TRUE;
}
 
/////////////////////////////////////////////////////////////////////
//	函数名称：uint8 IP_ReceivePacket(BYTE *pdata)//ip报处理
//	函数功能：ip报拆分
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
extern NetP NetPort;
uint8 IP_ReceivePacket(BYTE *pdata)//ip报处理
{
	eip  *prec;
	BYTE *p;
	NetP *pnet;
	
	prec = ( eip *)pdata;
	pnet = (NetP *)Get_NetPort();
	p = (BYTE *)(pdata+sizeof(eip));
	if (IP_Check((BYTE * )pdata) == FALSE) 
		return FALSE;
        
        for(uint8 i=0;i<4;i++)  //临时保存源IP地址，供保存ＰＡＤ的ＩＰ
        {
          NetPort.CMD_IP[i]=(*prec).SourceIp[i];          
        }
        
	switch(prec->NextProtocal)
	{	
	case 1://ICMP
		//if (IP_Compare(prec->DestId, pnet->My_Ip))
		ICMP_ReceivePacket(pdata);//icmp报处理						  
		break; 
	 case 6://TCP
	 	break;
	 case 17://UDP	 		
	 	UDP_ReceivePacket(p);//UDP报处理						  
	 	break;
	default:
		return FALSE;
	}
        pnet=pnet;
	return TRUE;
}

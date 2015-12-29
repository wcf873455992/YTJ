
#define ICMP_GLOBALS
#include "includes.h"
#include "../include/cfg_net.h"
 /////////////////////////////////////////////////////////////////////
//	函数名称：uint8 IP_ReceivePacket(BYTE *pdata)//ip报处理
//	函数功能：产生ICMP数据段的校验和 
//	入口参数：crc ：  16位的校验和        
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint16  CreateIcmpCrc(union icmp_rc icmppk) 
{
uint8 i;
union  w  CrcTemp;
	CrcTemp.dwords=0;
	for(i=0;i<20;i++)				//用于查询的ICMP报文共40字节
		{
		CrcTemp.dwords = CrcTemp.dwords + icmppk.words.wordbuf[i];
		}
	while(CrcTemp.words.high>0) 
		{
	    CrcTemp.dwords = (uint32)(CrcTemp.words.high+CrcTemp.words.low);
	    }
	CrcTemp.words.low = 0xffff - CrcTemp.words.low;
	return(CrcTemp.words.low);
}
uint16  CreateIcmpCrc1(union icmp_rc icmppk) 
{
		uint8 i;
		union  w  CrcTemp;
		CrcTemp.dwords=0;
		for(i=0;i<18;i++)				//用于查询的ICMP报文共40字节
			{
			CrcTemp.dwords = CrcTemp.dwords + icmppk.words.wordbuf[i];
			}
		while(CrcTemp.words.high>0) 
			{
		    CrcTemp.dwords = (uint32)(CrcTemp.words.high+CrcTemp.words.low);
		    }
		CrcTemp.words.low = 0xffff - CrcTemp.words.low;
		return(CrcTemp.words.low);
}
 /////////////////////////////////////////////////////////////////////
//	函数名称：void ICMP_ReceivePacket(uint8  * ICMPSENDPTR)  
//	函数功能：ping包响应
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void ICMP_ReceivePacket(uint8  * ICMPSENDPTR)  
{
uint8 i,temp;
struct _pkst SendIcmpData;  
	if(((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.type==0x08)//请求！！
	{
		((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.type=0;//回复
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.ttl--;
		for (i = 0; i < 4; i++)//IP翻转
	    		{
	        	temp = ((icmppro*)ICMPSENDPTR)->ipf.e_ip.SourceIp[i];
	        	((icmppro*)ICMPSENDPTR)->ipf.e_ip.SourceIp[i] = ((icmppro*)ICMPSENDPTR)->ipf.e_ip.DestId[i];
	        	((icmppro*)ICMPSENDPTR)->ipf.e_ip.DestId[i] = temp;
	    		}
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc=0;
	
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc=CreateIpHeadCrc(ICMPSENDPTR);//(((icmppro*)ICMPSENDPTR)->ipf);

		((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc=swap_int16(((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc);

		
		((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.Crc=0;
		((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.Crc=CreateIcmpCrc(((icmppro*)ICMPSENDPTR)->icmpf);
		SendIcmpData.length=((icmppro*)ICMPSENDPTR)->ipf.e_ip.TotalLen;

		SendIcmpData.length=swap_int16(SendIcmpData.length);

		SendIcmpData.STPTR=NULL;
		SendIcmpData.DAPTR=ICMPSENDPTR;
		IP_SendFrame(&SendIcmpData,((icmppro*)ICMPSENDPTR)->ipf.e_ip.DestId, 1);
		
	}
	else if(((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.type==0x00)//回复
	{
		i=i;
	}

}
 /////////////////////////////////////////////////////////////////////
//	函数名称：void Ping_Precess(uint8 * de_ip,uint8 * so_ip) 
//	函数功能：
//	入口参数：
//			
//			
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Ping_Precess(uint8 * de_ip) 
{
struct _pkst SendPingData;
union icmp_rc IcmpPacked;
static uint16 temp_findex=0x2345;
	IcmpPacked.e_icmp.type=0x08;
	IcmpPacked.e_icmp.option=0;
	IcmpPacked.e_icmp.Crc=0;
	IcmpPacked.e_icmp.id=0x0003;
	IcmpPacked.e_icmp.seq=temp_findex++;
	IcmpPacked.e_icmp.Crc=CreateIcmpCrc(IcmpPacked);
	SendPingData.length=40;
	SendPingData.STPTR=NULL;
	SendPingData.DAPTR=( uint8 * )&IcmpPacked;
	IP_SendFrame(&SendPingData,de_ip,1);
}

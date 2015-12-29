#ifndef __ARP_H
#define __ARP_H

typedef struct{ /* arp报文的内容总长28字节*/                     
            uint16 HardwareType;	/*以太网为0x0001*/
            uint16 ProtocalType;	/*ip 为0X0800*/
            uint8  HardwareLen;		/*=0X06*/
            uint8  ProtocalLen;		/*=0X04*/
            uint16 Operation;		/*操作  0X0001为请求   0X0002为应答  */
                               		/*0X0003为反向地址请求 0X0004为反向地址应答*/
            uint8  SourceMacId[6];	/*源网卡地址*/
            uint8  SourceIp[4];		/*源IP地址*/
            uint8  DestMacId[6];	/*目的网卡地址*/
            uint8  DestId[4];		/*目的IP地址*/
} arp ;

typedef struct{
			uint8 My_Ip[4];
			uint8 My_Gateway[4];
			uint8 My_Ip_Mark[4];
			uint8 My_Mac[6];
			uint8 My_Gateway_Mac[6];			
			uint8 LMT_Ip[4];
			uint8 Ser_Ip[4];
			uint8 PC_Ip[4];
			uint8 BJ_CardIP[4];
			uint8 Gateway_IP_TTL;
			uint8 CMD_IP[4];
			uint8 CMD_IP_TTL;
			uint8 Class_ID[8];
			uint8 Class_ID_flag;
			uint8 Test_Ip[4];
			uint8 Test_TTL;
			struct a_t ARP_TERM[MAX_ARP_TERM];
}NetP;

//Function define
void Initial_arp(void);
BYTE *Get_NetPort(void);
void UDP_MAC_Save(BYTE *pdata);
void ARP_ReceivePacket(uint8 * ARP_PTR);
BYTE *Get_ClassID(void);
uint32 IPByte_TO_Dword(BYTE *pdata);
void IPDword_TO_Byte(BYTE *pdata,uint32 ip);
BYTE *Get_DesIp(uint8 num);					//得到一体机IP
BYTE IP_Compare(BYTE *desip1,BYTE *desip2);	//比较ip相等
BYTE *ARP_Rec_Find(BYTE *desip);			//ARP登记表中查找
BYTE ARP_Rec_Save(BYTE *desip,BYTE *desMac);//ARP登记表保存
BYTE IPMASK_Compare(BYTE *desip);			//比较此ip是否是本网段
void Arp_Request(uint8 * ip_address);		//ARP请求
BYTE IP_Check(BYTE *pdata);
void Arp_reg(void);
int crc_calc(char *frame_no_fcs, int frame_len);
#endif

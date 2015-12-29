#ifndef __IP_H
#define __IP_H

union w{
		uint32 dwords;
		struct{
				uint16 low;
				uint16 high;
		}words;
		struct{
				uint8 byte0;
				uint8 byte1;
				uint8 byte2;
				uint8 byte3;
		}bytes;
};

typedef struct{ 
            uint8 	VerandIphLen;   /*版本与头长度*/
            uint8 	ServerType;     /*服务类型*/
            uint16 	TotalLen;       /*总长度*/
            uint16 	FrameIndex;     /*IP帧序号*/
            uint16 	Segment;        /*分段标志*/
            uint8 	ttl;            /*生存时间*/
            uint8 	NextProtocal;   /*下一层协议*/
            uint16 	Crc;            /*校验和*/
            uint8 	SourceIp[4];    /*源IP*/
            uint8 	DestId[4];      /*目的IP*/
}eip;

union ip_rc{
			eip e_ip;
			struct{
				uint16 wordbuf[10];
			}words;
};
//////////////////////////////////////////////////////////////
uint16 CreateIpHeadCrc(uint8 * Ip) ;
uint8 IP_SendFrame(
				struct _pkst *TxdData,	//发送数据
				uint8 * de_ip,				
				uint8 PROTOCOL
				);
uint8 IP_ReceivePacket(BYTE *pdata);	//ip报处理
#endif
//IP_EXT Rec_Ptr	RECQ[MAX_REC_BUFF];

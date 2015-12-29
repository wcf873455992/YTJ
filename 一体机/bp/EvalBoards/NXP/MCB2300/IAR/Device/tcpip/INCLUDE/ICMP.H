#ifndef __ICMP_H
#define __ICMP_H
typedef struct{/*包含在IP包中，是IP的上层为0X01的应用*/
              uint8  type;  		       	/*0X08 PING请求 0X00 PING应答*/
              uint8  option; 		   	/*0X00 PING*/
              uint16 Crc;
              uint16 id;
              uint16 seq;
              uint8  icmpdata[32];
}icmp;

union icmp_rc{
			icmp e_icmp;
			struct{
			 	uint16 wordbuf[20];
			 }words;
};

typedef struct{	//ipethernet 	etf;
			union ip_rc	  ipf;
			union icmp_rc icmpf;
}icmppro;

void ICMP_ReceivePacket(uint8  *ICMPSENDPTR);
void Ping_Precess(uint8 *de_ip); 
#endif

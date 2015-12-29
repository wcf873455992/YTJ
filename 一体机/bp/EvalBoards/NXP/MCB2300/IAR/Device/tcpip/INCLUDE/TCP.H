#ifdef TCP_GLOBALS
#define TCP_EXT
#else
#define TCP_EXT extern 
#endif 

/* TCP task SEM_NUM*/
//#define AppSemSend 	8
//#define AppSemCon 	12
//#define AppSemDisc 	40
TCP_EXT OS_EVENT  		*AppSemSend[MAX_TCP_LINKS];
TCP_EXT OS_EVENT  		*AppSemCon[MAX_TCP_LINKS];
TCP_EXT OS_EVENT  		*AppSemDisc[MAX_TCP_LINKS];	

/*tcp state define */
#define 	TCP_STATE_CLOSED        0
#define 	TCP_STATE_LISTEN        1
#define 	TCP_STATE_SYN_RCVD      2
#define 	TCP_STATE_SYN_SENT      3
#define 	TCP_STATE_ESTABLISHED   4
#define 	TCP_STATE_FIN_WAIT1     5
#define 	TCP_STATE_FIN_WAIT2     6
#define 	TCP_STATE_CLOSING       7
#define 	TCP_STATE_CLOSE_WAIT    8
#define 	TCP_STATE_LAST_ACK      9
#define 	TCP_STATE_TIME_WAIT     10
//tcp连接的标志位
#define 	TCP_FIN 0x01
#define 	TCP_SYN 0x02
#define 	TCP_RST 0x04
#define 	TCP_PSH 0x08
#define 	TCP_ACK 0x10
#define 	TCP_URG 0x20

struct tcp_data{
				uint16 	SourcePort;		//f  		/*源端口*//
	            uint16 	DestPort;		//f	   		/*目的端口*/
	            uint32 	SeqNum;  		//0-3 f		/*顺序号*/
	            uint32 	AckNum;  		//0-3 f		/*确认号*/
	            uint8 	offset;     	   			/*数据偏移量*/
	            uint8 	control;    	   			/*连接控制*/
	            uint16 	window; 		//f     	/*流控*/
	            uint16 	Crc;    		//f    	   	/*校验和 ，包括伪头部，TCP头部，数据*/
	            uint16 	urg;               			/*紧急指针*/
	            uint8 	tcpdata[MAX_TCP_DATA];     	/*TCP数据*/
};
struct tcp_dhead{  
			uint8  rev; 
			uint8  NextProtocal;
            uint16 TotalLen;          /*校验和,包括伪头部,udp头部,数据*/
            uint8  SourceIp[4];  	   /*源端口*/
            uint8  DestId[4];     	   /*目的端口*/
};
typedef struct{
			struct tcp_dhead TcpDHead;
			struct tcp_data Tcp;			
}tcp_send;
struct Socket_Type{
			uint16 My_Port; 		 	//本机端口
		   	uint16 Dest_Port;		 	//对方端口
		   	uint8  Dest_Ip[4];	 		//对方ip
		   	uint8  My_Ip[4];
		   	uint32 IRS;			 		//初始化顺序号
		   	uint32 ISS;			 		//我的初始化序列号
		   	uint32 Rcv_Next;		 	//对方的顺序号
		   	uint32 Send_Next;		 	//我的已经发送顺序号
		   	uint32 Sent_UnAck;		 	//我的还没有确认顺序号
		   	uint32 RecPassSeq;
		   	uint32 RecPassAck;
		   	uint32 SenPassSeq;
		   	uint32 SenPassAck;
		   	uint32 RecFutureAck;
		   	uint32 SenFutureSeq;
		   	//unsigned long dest_ack_number;
		   	uint16 Rcv_Window;		 	//对方的window大小
		   	uint16 Snd_Window; 		 	//我的window大小
		   	uint16 Dest_Max_Seg_Size;	//对方接受的最大的数据包大小MTU
		   	uint16 My_Max_Seg_Size; 	//我能接受的最大的数据包大小
		   	uint32 My_Wl1;		//seq
		   	uint32 My_Wl2;		//ack
		   	uint8  State;		//连接状态
		   	uint8  Open;
			uint8  ResendState;
			uint8  ResendTime;
			uint16 ResendLength;
			uint8  StaSem;	//BIT7表示该连接是LISTEN开始的，即服务器的。BIT1是表示该连接等待AppSemCon信号量。BIT0是表示正等待AppSemSend信号量
			//struct tcp_data 	resend_data;
			uint8  TcpDHeadUint8[12];
			uint8  TcpHeadUint8[MAX_TCP_DATA+20];
			uint8  TcpDataQ[MAX_TCP_DATA];
			uint16 TcpDataQWrPtr;
			uint16 TcpDataQRdPtr;
};
TCP_EXT uint16 CheckSumTcp1(uint8 num,uint16 length);
TCP_EXT void Delete_Socket(uint8 num) ;
TCP_EXT struct Socket_Type TcpStatus[MAX_TCP_LINKS];
TCP_EXT	void TcpHeadHandle(uint8 num ,uint16 Tcp_headlength_Control);
//TCP_EXT	uint8 Tcp_Connect1(uint8 aa) keilreentrant ;
TCP_EXT uint8 Process_Tcp1(Rec_Ptr * Tcp_Rec);	// USE_XDATA keilreentrant ;
TCP_EXT void Tcp_Initial(void);					// keilreentrant ;

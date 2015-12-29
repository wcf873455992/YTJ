#ifndef __USER_UDP_H
#define  __USER_UDP_H
#define 	CMD_ABLE_ICII 				0x3230424c
#define	IC2_COMMAND_PORT			7101		/// 命令端口定义
#define	IC2_COMMAND_MAXLEN		1024		/// 一条命令的最大数据长度
#define	IC2_COMMAND_BUF_COUNT	20		/// 缓存的命令条数
#define	IC2_COMMAND_HEADLEN		16		/// 命令头长度
#define 	CMD_AD_SENDMSG     		0x00010000		//一体机发送的消息
#define 	CMD_AD_RECVMSG     		0x00010001		//发送给一体机的消息
#define	 CMD_SENDER_AD   			0x14//一体机
#define	 CMD_SENDER_ACK			0x80//
/**
@brief 命令字定义：
命令字是4个字节，以下分类只为了在功能上区分操作的主体，不作强制限制，以具体定义为准
0x00000000-0x0000FFFF  为避免与先前系统的冲突，预以保留
0x00010000-0x0001FFFF  一体机硬件交互层命令
0x00020000-0x0002FFFF  集控、服务器、电视墙等控制层命令
0x00030000-0x0003FFFF  流媒体、教师机等应用层命令
*/
typedef struct tagIC2CMD_CmdID
{
    DWORD	dwCommand;		/// 命令字
    DWORD   dwFlag;			/// 命令标志，默认可定为#define FLAG_NULL 0x00000000
    /**
    @brief  GUID
    集控或电视墙管理的User ID
    */
    char 	cUserID[10];
    /**
    @brief  GUID
    当集控对一体机发送消息时，此GUID为接收或发送一体机的GUID
    */
    char 	cGUID[10];
} IC2CMD_CmdID;


typedef struct tagIC2CMD_Header
{
    DWORD dwSyncHead;			/// 同步头 #define CMD_ABLE_ICII 0x4c423032
    DWORD	dwFromIP;			/// 源IP
    DWORD	dwToIP;			/// 目的IP
    BYTE	byteFrameIndex;		/// 包序号(0-255循环,重发不增加)
    /**
    @brief 协议类型包括是命令还是回复的定义，还可区分发送者的角色
    byteProtocolType&0x7f 区分角色
    byteProtocolType&0x80 区分是响应还是命令
    */
    BYTE	byteProtocolType;		/// 协议类型
    WORD	wDataLen;			/// 下一部分的数据长度
    IC2CMD_CmdID Cmd;
} IC2CMD_Header;
#define AUDIO_PORT 		3002
#define AUDIO_CMD_LEN 	512
#define AUDIO_DATA_LEN 	(AUDIO_CMD_LEN-8)
#define CMD_ABLE_AUDIO 0xaaa55a55
/**
@brief 命令头定义
*/
typedef struct tagAudio_Header
{
    DWORD	dwSyncHead;			/// 同步头
    DWORD	dwFromIP;			/// 音频源IP
} Audio_Header;

//网络测试包响应
struct LAN_TEST_DATA
{
    uint8 flag[4];
    uint8 sip[4];

};


//////////////////////////////////////////////////////////////
void LAN_Test_Ack(uint8 *pdata, uint16 lg);
void UDP_Send_CMD(BYTE *pdata, uint16 lg, uint8 *des_ip);
void UDP_msg_rev(uint8 *p);
uint8 UDP_Read(BYTE *pdata, uint16 lg, uint8 *des_ip, uint8 WaitType, uint8 KeyWord);
uint8 Check_UDP_Paket(void *pdata, uint16 lg);
#endif

#ifndef _USER_OPTION_H
#define  _USER_OPTION_H


#define Debug_MSG_Log         	1
#define Debug_Scamera_Log  	0

///////////////////////////////////////
#define DEVICE_COMMTYPE_NONE	0
#define DEVICE_COMMTYPE_HW		1
#define DEVICE_COMMTYPE_XCOM	2
#define DEVICE_COMMTYPE_UDP		3

/////////////////功能配置参数///////////////////////////
#define Device_FunctionMode_YY    	0  //语音控制台功能
#define Device_FunctionMode_LB		1//录播功能
#define Device_FunctionMode_TB    	2//同步卡功能
#define Device_FunctionMode_KBRY  	3  //荣耀键盘
#define Device_FunctionMode_KBKR  	4//酷睿键盘
#define Device_FunctionMode_CARD  5 //校园一卡通
/////////////////上课状态////////////////////////////////////
#define CLASS_OVER            		0
#define CLASS_BEGIN_KEY		1
#define CLASS_BEGIN_RF		2
#define CLASS_BEGIN_TIME		3
#define CLASS_BEGIN_UDPCMD	4
#define CLASS_BEGIN_FINGERPRINT 5        //指纹机
/////////////////////////////////////////////////////
#define KB_TYPE_NO				    0
#define KB_TYPE_RONGYAO               1
#define KB_TYPE_KUREI			    2



struct BASE_MSG
{
    uint8 device;
    uint8 option;
};
struct BASE_TIME
{
    uint8 hour;
    uint8 min;
};
//串口通讯参数
//网络通讯参数
#define COMM_POCTL_def		0
#define COMM_POCTL_YTJ		1
#define COMM_POCTL_PD		2
#define COMM_POCTL_UDP		4
#define COMM_POCTL_ICMP	8
#define COMM_POCTL_ARP		16

struct COMM_IP
{
    uint8 IP[4];		//IP
    uint8 Mask[4];
};
struct COMM_UDP
{
    uint8 IP[4];		//IP
    //	uint8 Mask[4];
    uint16 port;		//端口
    uint16 poctl;		//协议
};
struct COMM_COM
{
    uint8 PortNum;
    uint8 address;
    uint16 BPS;
    uint16 PROTOCOL;
    uint8 linecontrl;
    uint8 Hbps;

};
//通讯参数
union COMM_VAL
{
    struct COMM_COM xcom;
    struct COMM_UDP   udp;
};

//DEV_VGA_LINK
struct DEV_VGA_LINK
{
    uint8 VGAIn;		//设备连接一体机VGA输入通道
    uint8 VGAOut;		//设备连接一体机VGA输出通道
    uint8 VGALink;		//输出通道默认连接
    uint8 vgatype;		//VGA属性
};
//DEV_VIDEO_LINK
struct DEV_VIDEO_LINK
{
    uint8 VideoIn;		//设备连接一体机VIDEO输入通道
    uint8 VideoOut;		//设备连接一体机VIDEO输出通道
    uint8 VideoLink;	//输出通道默认连接
    uint8 VideoType;	//VIDEO属性
};
//DEV_SOUND_LINK
struct DEV_SOUND_LINK
{
    uint8 SoundIn;		//设备连接一体机SOUND输入通道
    uint8 SoundOut;		//设备连接一体机SOUND输出通道
    uint8 SoundLink;	//输出通道默认连接
    uint8 SoundVol;		//音频音量
};
//DEV_IN_LINK
struct DEV_IN_LINK
{
    uint8 in_no;		//IO输入通道号
    uint8 in_type;		//IO属性
    uint8 in_sound;	//音频号
    uint8 bak;		//保留
};
//DEV_OUT_LINK
struct DEV_OUT_LINK
{
    uint8 out_no;
    uint8 out_type;
    uint8 io_sound;
    uint8 bak;
};
//DEV_POWER_LINK
struct DEV_POWER_LINK
{
    uint8	 dev_id;
    uint8 power_no;
};
//DEV_COMM_LINK
struct DEV_COMM_LINK
{
    uint16 comm_type;
    union COMM_VAL val;
};
///////设备类型定义/////////////////////////////
#define DEVICE_TYPE_CLASS_INFO      			 0
#define DEVICE_TYPE_CLASS_OPEN_INFO		 1
#define DEVICE_TYPE_CLASS_CLOSE_INFO		 2
#define DEVICE_TYPE_CLASS_TABLE_START		 3
#define DEVICE_TYPE_CLASS_TABLE_END		 9

///////////////教室基本信息///////////////////////////////
//教室基本参数------对应数据0页
struct CLASS_INFO
{
    uint8 dev_id;				//设备ID
    uint8 dev_type;				//设备类型
    uint8 school_name[32];		//学校名称
    uint8 house_name[32];		//教学楼
    uint8 class_name[32];		//教室
    uint16 class_type;			//智能教室类型
    uint8 class_ID[8];			//控制媒体类型识别码
    struct COMM_IP class_ip;	//教室
    struct COMM_IP Gateway_ip;	//网关
    struct COMM_IP Ser_ip;		//集控服务器
    struct BASE_TIME alm_on;	//报警布防
    struct BASE_TIME alm_off;	//报警布防
    uint8 configtime[4];           //配置表配置次数
    uint8 CurrsScameraPage; //监控摄像机（设备配置的页号）
    uint8 OpenType;              //开关机类型
    uint8 BJCardIP[4];          //编辑卡IP
    uint8 BMCard1IP[4];//编码卡1IP
    uint8 BMCard2IP[4];//编码卡2IP
    uint8 LMT_IP[4];     //流媒体IP
    uint8 PC_IP[4];    //教师PC机IP
    uint8 Def_IP[4];//备用邋IP
    uint8 Def_1;//备用
    uint8 Card_Contrl;//校园一卡通开关
    uint8 YuYinCtrl;//语音功能开关

};
//一体机开机操作------对应数据1页
struct CLASS_OPION_TYPE
{
    struct BASE_MSG msg;
    uint16  del_time;

};
struct CLASS_OPEN_INFO
{
    uint8 dev_id;					//设备ID
    uint8 dev_type;					//设备类型
    struct BASE_MSG  msgon1;		//开机触发消息1
    struct BASE_MSG  msgon2;		//开机触发消息2
    struct BASE_MSG  msgon3;		//开机触发消息3
    //struct CLASS_TABLE_REC *base;	//开机操作消息表
};
//一体机关机操作------对应数据2页
struct CLASS_CLOSE_INFO
{
    uint8 dev_id;						//设备ID
    uint8 dev_type;						//设备类型
    struct BASE_MSG  msgoff1;			//关机触发消息1
    struct BASE_MSG  msgoff2;			//关机触发消息2
    struct BASE_MSG  msgoff3;			//关机触发消息3
    //	struct CLASS_TABLE_REC *base;		//关机操作消息表
};

//设备基本信息
struct DEVICE_INFO
{
    uint8 dev_id;					//设备ID
    uint8 dev_type;					//设备类型
    uint8 dev_name[32];				//设备名称
    uint8 dev_type_name[32];		//设备型号
    struct DEV_VGA_LINK	 VGA;		//VGA连接信息
    struct DEV_VIDEO_LINK  VIDEO;	//视频信号连接信息
    struct DEV_SOUND_LINK SOUND; 	//音频信号临街信息
    struct DEV_IN_LINK	 IN;		//IO输入连接信息
    struct DEV_OUT_LINK	OUT;		//输出连接
    struct DEV_POWER_LINK POWER;	//电源连接鄂i
    struct DEV_COMM_LINK  COMM;		//通讯连接
    struct BASE_MSG KB_MSG;			//设备投入选择键盘消息
    struct BASE_MSG ON_MSG;			//开机发送消息
    struct BASE_MSG OFF_MSG;		//关机发送消息
    uint16 OFF_TIME;				//关机延时
    uint8  prev_val;				//设备的私有数据
};
//消息结构
struct MESSAGE_TYPE
{
    uint8  dev_id;					//设备ID
    uint8 dev_type;					//设备类型
    uint8 DEVICE;					//目标设备
    struct BASE_MSG sys_msg;		//消息
    uint8 next_id;					//扩展数据
    uint8 data_lg;					//数据长度
};
//任务TASK
#define TASK_DELAY_MAX_SIZE  20
struct TASK_REC_TYPE
{
    uint8 TimeType;
    uint8 DelayTime;
    uint8 bak_time;
    uint8 repeat;
    uint16 msg_lg;
    void *pmsg;
};
struct TASK_TYPE
{
    uint8 RecCount;
    //uint8 RunPointer;
    //uint8 SavePointer;
    struct TASK_REC_TYPE DelayRec[TASK_DELAY_MAX_SIZE];
};

//消息队列的执行
#define CLASS_TASK_FLAG_RUN	1
#define CLASS_TASK_FLAG_OVER	(1<<1)
#define CLASS_TASK_OK		0x80
#define CLASS_TASK_ERR_ID    0x81
#define CLASS_TASK_EMPTY	0x82
#define CLASS_TASK_BUSY        0x83
struct MSG_OPTION_Q_TYPE
{
    uint8 Flag;
    uint8 Curr_id;
    struct BASE_MSG ClassOpen[3];
    struct BASE_MSG ClassClose[3];
    uint8 ClassTaskNum;
    uint8 ClassCurrNUm;
    uint8 TIME_LED_TYPE;
    uint8 TIME_LED_STATE;
};
struct DEVICE_TYJ_TYPE
{
    uint8 comm;
    uint8 curr_Type;	//当前投影机类型1=VGA 0=video
    uint8 vgain;
    uint8 videoin;
};
struct POWER_STATE
{
    uint32 state;
};

struct MSG_OPTION_COMM_TYPE
{
    struct BASE_MSG msg;
    BYTE Frame;
    BYTE PortNum;
    WORD lg;
};
struct MSG_REV_TYPE
{
    uint16 Type;
    uint16 num;
    uint8 des_ip[4];

};

///应用代码启动中断向量
#define START_VIC_ADDRESS  (BYTE *)0x40000000

//EEPROM24C16地址
#define PROGRAM_FLAG_ADDRESS	0x0000
#define USER_ID_ADDRESS			0x0060
#define  DEVICE_USE_STAT_TIME	0x0080
#define CLASS_TABLE_ADDRESS		0x0100//课程表0x100-0x7ff

//Function define

//电磁锁控制
#define DCS_NONE		0
#define DCS_LCJ_OC39         1//力士坚牌邋OC39电磁锁

#define DCS_CONTRL_ON_OFF 	 	1
#define DCS_CONTRL_START_OVER   (1<<1)
#define DCS_JX_OPEN				(1<<2)

struct DEVICE_DCS_TYPE
{
    uint8 Type;			//电磁锁类型
    uint8 Contrl_State;       //控制状态
    uint8 check_state_in;   //电磁锁信号检测


};


uint32 Random (void);
BYTE BinToChar(BYTE data);
void Option_init(void);
void ClassInfo_init(void);
BYTE User_ID_Read(BYTE *buf, uint8 type);
uint8 Device_Rec_Scan(void);
void *Device_Rec_Find(BYTE *pdata, uint8 Find_Type);
uint8 Alarm_Link(void);
BYTE Class_Task_Start(BYTE id);
BYTE Task_Rec_Find(BYTE *pdata, BYTE lg);
BYTE Task_Rec_Del(BYTE *pdata, BYTE lg);
uint8 Task_Run_Delay(uint16 DelayTime, uint8 *pdata, uint16 lg, uint8 repeat, uint8 *prev);
uint8 Msg_send(uint8 *pdata, uint16 lg, uint8 *prev);
BYTE Msg_Option_Find(void *pdata);
void User_Option(void);
BYTE Get_Debug(uint16 flag);
void *Get_Device_Status(void);
uint8 Check_FunctionMode(uint8 FunctionMode);
void Run_Send_Flag(void);
void TASK_RUN(void *pdata);
void CameraPowerInit(void);
void BJ_Card_init(void);
void Run_Send_Flag(void);
uint8 Device_DCS_Check_in(uint8 val);
void Device_DCS_init(void);
void Device_DCS_Contrl(void);
void LMT_init(uint16 Power_no);
void LMT_Delay_Power(void);

uint16 Get_ver(void);
uint8 Get_Class_Stauts(void);



//Variable declare
extern struct DEVICE_STAT_USE_TYPE gStat_Rec;
extern struct DEVICE_STATUS_TYPE gDeviceStatus; //一体机状态
extern struct DEVICE_REC_TYPE gDeviceTable;	  //设备登记表
extern struct TASK_TYPE TASK_TABLE;			 //延时命令表

#endif


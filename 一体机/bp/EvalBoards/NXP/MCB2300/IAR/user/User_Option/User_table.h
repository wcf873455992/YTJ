#ifndef __USER_TABLE_H
#define __USER_TABLE_H

//////////////////////////////////////////////
#define DEV_REC_SIZE   		   256
#define DEV_REC_OPTION_ID      16
#define DEV_REC_OPTION_NOERR   0
#define DEV_REC_OPTION_NOFIND  1
#define DEV_REC_OPTION_ERR	   2
/////////////////////////////////////////////
#define DEV_FIND_ID			0
#define DEV_FIND_TYPE		1
#define DEV_FIND_SELECT		2
#define DEV_FIND_MSG		3
#define DEV_FIND_TIME		4
#define DEV_FIND_VGAIN		5
#define DEV_FIND_VGAOUT		6
#define DEV_FIND_VIDEOIN	7
#define DEV_FIND_VIDEOOUT	8
#define DEV_FIND_XCOM_PORT	9
#define DEV_FIND_XCOM_SubADDr  10
///////////////配置表数据记录////////////////
//开关机消息记录
struct CLASS_OPTION_REC
{
    struct BASE_MSG MsgRec;
    uint16 OptionTime;
};
//课程表记录
struct CLASS_TABLE_REC
{
    struct BASE_TIME  class_begin;	//上课时间
    struct BASE_TIME class_over;	//下课时间
    uint8 class_name[16];			//课程名
    uint8 teacher_name[10];			//教师名
};
//策略记录
struct STRATEGY_REC
{
    uint8 current_rec_frame; 		//当前录制场景
    uint8 current_use_frame;        //当前使用
    struct BASE_MSG touch_msg;    	//触发消息
    struct BASE_MSG decision_msg;	//决策消息
    uint16 msg_val;                 //决策消息参数
    struct STRATEGY_REC *next;
};
//跟拍数据表记录
struct TRACK_REC
{
    uint16 rang; 	//距离
    uint16 ZJ_zoom;	//中景
    uint16 JJ_zoom;	//近景
    uint16 TX_zoom;	//特写
    uint16 speed;	//速度
    struct TRACK_REC *next;
};
//特写坐标
struct TX_ZB
{
    uint16 x;	//特写坐标
    uint16 y;	//特写坐标
    uint16 d;	//特写半径
};
//无线互动记录
struct HD_REC
{
    uint8 camera_id;			//目标摄像机
    uint8 position;				//预制位号
    struct BASE_MSG sys_msg;	//系统消息
    uint8 val;					//互动位置码
    struct HD_REC *next;
};
//板书定位数据记录
struct BS_REC
{
    uint16 position1;		//板书探头1报警距离
    uint16 position2;		//板书探头2报警距离
    uint16 position3;		//板书探头3报警距离
    uint16 position4;		//板书探头4报警距离
    struct BS_REC *next;
};

//课程表基本信息-----对应数据3-9页
struct CLASS_TABLE_INFO
{
    uint8 dev_id;				//设备ID
    uint8 dev_type;				//设备类型
    uint8 week;					//星期
    //struct CLASS_TABLE_REC  rec[8];
    //struct CLASS_TABLE_REC *base;	//课程表操作消息表
};
//跟拍信息--------对应数据10页
struct TRACK_INFO
{
    uint8	dev_id;			//设备ID
    uint8	dev_type;		//设备类型
    uint8	track_name[16];	//跟拍控制名称
    uint16	track_type;		//控制类型
    uint8  	OBJcamera_id;  	//目标摄象机ID
    uint16	OBJcamera_x;	//目标摄象机坐标X
    uint16	OBJcamera_y;	//目标摄象机坐标Y
    uint16	OBJcamera_z;	//目标摄象机坐标Z
    uint16	OBJcamera_H;	//目标摄象机坐标水平角
    uint16	OBJcamera_V;	//目标摄象机坐标垂直角
    struct TX_ZB tx1;		//特写1坐标
    struct TX_ZB tx2;		//特写2坐标
    struct TX_ZB tx3;		//特写3坐标
    struct TRACK_REC *base;	//跟拍控制表
};
//板书控制基本信息---对应数据11页-
struct BS_INFO
{
    uint8	dev_id;			//设备ID
    uint8	dev_type;		//设备类型
    uint8   bs_name[16];	//板书控制名称
    uint16  bs_type;		//板书控制类型
    uint8   position;  		//板书各探头距离
    struct BS_REC *base;	//板书控制表
};
//策略机基本信息-----对应数据12-13页
struct STRATEGY_INFO
{
    uint8  dev_id;				//设备ID
    uint8  dev_type;			//设备类型
    uint8  STRATEGY_name[16];	//策略机名称
    uint16 STRATEGY_type;		//策略机类型
    struct STRATEGY_REC *base;	//策略控制表
};
//无线互动基本信息---------对应数据14-15页
struct HD_INFO
{
    uint8	dev_id;			//设备ID
    uint8	dev_type;		//设备类型
    uint8	hd_name[16];	//无线互动名称
    uint16 	hd_type;		//无线互动类型
    struct HD_REC *base;	//无线互动控制表
};
//教室ID
#define CLASS_ID_FLAG_ACTIVE	1
struct CLASS_ID_TYPE
{
    uint8  dev_id;			//设备ID
    uint8 dev_type;			//设备类型
    uint16 flag;
    uint8 class_id[8];
};

//设备统计消息，对应配置表0xfa
struct DEVICE_STAT_REC_TYPE
{
    uint8 Device_Type;
    uint8 Device_Count;
    uint8 Device_Use_Time[6];
    uint32 Device_Stat_Time;
    uint32 Device_Stat_Count;
};

#define DEVICE_FILE_BIGIN			1
#define DEVICE_FILE_ERASESECTOR  	(1<<1)
#define DEVICE_FILE_BLANKCHK		(1<<2)
#define DEVICE_FILE_HALF			(1<<3)
struct DEVICE_REC_FILE_TYPE
{
    uint16 FileType;
    uint16 FilePage;
    uint8  FileFlag;
    uint8 FileDebug;
    uint8 *FileBuf;	//申请数据指针
};

#define DEV_DEBUG_SYSTEM	1
#define DEV_DEBUG_MSG		(1<<1)
#define DEV_DEBUG_KEY		(1<<2) //无用
#define DEV_DEBUG_HW		(1<<3)  //用于红外输入输出测试
#define DEV_DEBUG_XCOM 		(1<<4) //用于串口测试
#define DEV_DEBUG_POWER		(1<<5) //电源调试信息输出
#define DEV_DEBUG_IO 		(1<<7)//用于IO输入输出测试
#define DEV_MAX_SIZE 		50
struct DEVICE_TYPE
{
    uint8 id;
    uint8 type;
    uint8 state;
};

#define DEV_FLAG_SVER_LINK   1
struct DEVICE_REC_TYPE
{
    uint8 DeviceCount;
    uint8 OptionCount;
    uint8 TB;
    uint8 Debug;
    uint16 flag_sver;
    uint16 Alarm_Type;
    uint16 Alarm_Flag;
    uint16 Device_link;
    uint8  device_link_times[4];
    struct MSG_OPTION_Q_TYPE Class;
    struct DEVICE_REC_FILE_TYPE File;
    struct DEVICE_TYJ_TYPE TYJ[2];//投影机
};

//一体机连接状态WORD
#define DEV_LINK_PCON   	1		//课件计算机关
#define DEV_LINK_PCLK     	(1<<1)	//课件计算机未连接
#define DEV_LINK_PCOK   	(1<<2)	//课件计算机监控软件未连接
#define DEV_LINK_LMTON 		(1<<4)	//流媒体计算机关
#define DEV_LINK_LMTLK		(1<<5)	//流媒体计算机未连接
#define DEV_LINK_LMTOK		(1<<6)	//流媒体计算机软件未连接
#define DEV_LINK_LIGHT		(1<<7)	//室内灯光
#define DEV_LINK_KBLK		(1<<8)	//键盘未连接
#define DEV_LINK_BJCardLK		(1<<9)	//自动跟拍卡未连接
#define DEV_LINK_TBLK		(1<<10)	//视频同步板未连接
#define DEV_LINK_YYLK		(1<<11)	//扩展语音卡未连接
#define DEV_LINK_BM1LK		(1<<12)	//视频编码卡1未连接
#define DEV_LINK_BM2LK		(1<<13)	//视频编码卡2未连接
#define DEV_LINK_GFON       (1<<14)	//功放电源
#define DEV_LINK_TYJON      (1<<15)	//投影机电源
//一体机运行状态1BYTE
#define DEV_STATE_CLASSMARK		0x0f
#define DEV_STATE_CLASSOVER		00 	//关机/下课
#define DEV_STATE_CLASSKBSTAR 	01	//上课键开机
#define DEV_STATE_CLASSRFSTAR	02	//Rf卡开机
#define DEV_STATE_CLASSTMSTAR	03	//课表/定时开机
#define DEV_STATE_CLASSYCSTAR	04	//后台命令开机

#define DEV_STATE_OPTIONMARK	0xf0
#define DEV_STATE_OPTIONSD		0x00	// 手动
#define DEV_STATE_OPTIONAT		0x10	//自动
#define DEV_STATE_OPTIONJGDEV	0x40	//接管设备
#define DEV_STATE_OPTIONJGREC	0x80	//接管录制
#define DEV_STATE_OPTIONJG		0xC0      //接管
//上课状态1BYTE
#define DEV_CLASS_START			1		//上课
#define DEV_CLASS_GP			1<<1	//跟牌
#define DEV_CLASS_CL			1<<2	//策略
#define DEV_CLASS_HD			1<<3	//互动
#define DEV_CLASS_RECSTOP		0x00	//录制停止
#define DEV_CLASS_RECPUSH		0x10	//录制展厅
#define DEV_CLASS_RECSTART		0x20	//录制开始
//当前录制设置1WORD
#define DEV_REC_ML500			0x0000	//码率500K
#define DEV_REC_ML1M			0x0001	//码率1M
#define DEV_REC_MLZDY			0x0002	//码率自定义
#define DEV_REC_LZVGA			0x0000	//VGA
#define DEV_REC_LZD1			0x0010	//D1
#define DEV_REC_LZZDY			0x0020	//自定义
#define DEV_REC_TXCG			0x0000	//常规
#define DEV_REC_TXHZH			0x0100	//画中画
#define DEV_REC_TXZDY			0x0200	//自定义
#define DEV_REC_SG160			0x0000	//身高设定160
#define DEV_REC_SG170			0x1000	//身高设定〉160
#define DEV_REC_SG180			0x2000	//身高设定〉180
//报警输入1BYTE
#define DEV_BJ_IN1				1		//报警输入1
#define DEV_BJ_IN2				1<<1	//报警输入2
#define DEV_BJ_IN3				1<<2	//报警输入3
#define DEV_BJ_IN4				1<<3	//报警输入4
#define DEV_BJ_IN5				1<<4	//报警输入5
#define DEV_BJ_IN6				1<<5	//报警输入6
#define DEV_BJ_TYJ				1<<6	//投影机断线
//报警输出1BYTE
#define DEV_BJ_OUT1				1		//报警输出1
#define DEV_BJ_OUT2				1<<1	//报警输出2
#define DEV_BJ_OUT3				1<<2	//报警输出3

struct DEVICE_STATUS_TYPE
{
    uint8 DEV_ID[8];			//一体机识别码
    uint16 ver;					//一体机版本号
    uint16 KB_ver;				//控制键盘版本号
    uint16 TRACK_ver;  			//自动跟踪板本号
    uint8  DEV_state;   		//一体机运行状态
    uint8  CLASS_state;			//上课状态
    uint16 Current_Device;		//当前使用设备
    uint16 Current_REC;			//当前流媒体录制场景
    uint32 Disk_size;			//流媒体存储容量
    uint16 Current_REC_set;		//当前录制设置
    uint8  BJ_IN;				//报警输入
    uint8  BJ_out;				//报警输出
    uint16 DEV_Link_State;		//一体机连接状态
    uint16 Curr_SCAME;			//当前的摄像机[1-6]
    uint32 power_state[4];		//电源箱状态
    uint8  configtime[4];		//工具的配置次数
    uint16 Curr_Contrl_Device; 	//当前的升降设备
    uint16 POWER_ver;			//控制电源箱版本号
    uint16 MK_Vol;        		//面板音量，D15=1 静音
    uint32 BJ_BAK;                 //扩展报警
    uint8  KB_Type;               //键盘类型
    uint8  Power_Type ;        //电源箱类型
    uint16 TB_CardVer;        //图像卡版本号
    uint16 BJ_CardVer;         //编辑卡版本号
    uint16 WD_Val;               //一体机机箱温度
};

//设备使用统计
#define STAT_DEVICE_OVER		1
#define STAT_DEVICE_TIME		2
struct DEVICE_STAT_USE_TYPE
{
    uint16 Device;
    uint16 flag[15];
    uint32 DEVICE_STAT_Start_Time[15];
    struct DEVICE_STAT_REC_TYPE Rec[15];
};

//Function define
uint32 File_Req(uint16 type, uint16 page, uint8 *des_ip);
void File_Table_Write(uint16 page, BYTE *buf, uint8 *des_ip);
void File_Table_Read(uint16 page, uint8 *des_ip);
void File_ClassTable_Write(uint16 FilePage, uint8 *buf, uint8 *des_ip);
void File_ClassTable_Write_OneDay(uint16 FilePage, uint8 *buf, uint8 *des_ip);
void File_ClassTable_Read(uint16 FilePage, uint8 *des_ip);
BYTE File_ClassTable_Read_CurDay(void);
BYTE Class_Table_Check(void);
void Device_Stat_Option(BYTE Device, BYTE option);
void Device_Stat_Save(void);
BYTE Device_Use_Stat_Read(void);
void File_UserProgram_Write(uint16 Page, BYTE *buf, uint8 *des_ip);

#endif


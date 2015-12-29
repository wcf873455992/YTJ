#ifndef __USE_KEY_H
#define  __USE_KEY_H
#define LED_CHANLE_VER		0
#define LED_CHANLE_DATA	1
#define LED_CHANLE_TYPE		2
#define KEY_SOUND_VOL		3
#define KEY_VAL				4
#define KEY_TIME				5
#define KEY_RF_DATA			6
#define KEY_CONTRL			7
#define KEY_Q_SIZE	10//键盘缓冲区大小
//第一行显示
#define LED_PC_POWER			0x00000001	//课件电脑电源
#define LED_TYM					0x00000002  //投影机电动幕
#define LED_SHOW				0x00000004 //视频展台
#define LED_REC					0x00000008 //录制
#define LED_MODE_ZD				0x00000010 //模式/自动
#define LED_METRIC_D1			0x00000020 //分辨率D1
#define LED_SPEED_ZDY			0x00000040//码率/自定义
#define LED_TAIL_160				0x00000080//身高/160
#define LED_SCAMERA2			0x00000100//摄像机2
#define LED_SCAMERA6			0x00000200//摄像机6
//第二行显示
#define LED_LIGHT_POWER		0x00010001 //室内灯光
#define LED_LCD_UP				0x00010002//液晶升降
#define LED_DVD					0x00010004 //DVD
#define LED_KZB					0x00010008//扩展B
#define LED_MODE_SD				0x00010010 //模式/手动
#define LED_METRIC_ZDY			0x00010020 //分辨率/自定义
#define LED_FRAME_CG			0x00010040 //特效/常规
#define LED_TAIL_170				0x00010080//身高/170
#define LED_SCAMERA3			0x00010100//摄像机3
#define LED_HELP					0x00010200 //求助
//第3行显示
#define LED_GF_POWER			0x00020001 //功放电源
#define LED_TYDJ					0x00020002 //投影机吊架
#define LED_RECODER				0x00020004 //设备录音机
#define LED_PC					0x00020008 //设备/计算机
#define LED_MODE_YC				0x00020010 //模式/远程
#define LED_SPEED_400			0x00020020 //码率400
#define LED_FRAME_HZH			0x00020040 //特效画中画
#define LED_TAIL_180				0x00020080 //身高180
#define LED_SCAMERA4			0x00020100 //摄像机4
#define LED_MUTE					0x00020200  //静音
//第4行显示
#define LED_TYJ_POWER			0x00030001 //投影机电源
#define LED_PELMET_POWER		0x00030002 //电动窗帘
#define LED_KZA					0x00030004//扩展A
#define LED_NPC					0x00030008 //笔记本
#define LED_METRIC_XGA			0x00030010 //分辨率XGA
#define LED_SPEED_1M			0x00030020 //码率1M
#define LED_FRAME_ZDY			0x00030040 //特效/自定义
#define LED_SCAMERA1			0x00030080//摄像机1
#define LED_SCAMERA5			0x00030100//摄像机5
//键盘
#define KEY_CLASS_BEGIN			0x0001//上课
#define KEY_MIC_MUTE			0x0002 //MIC_MUTE
#define KEY_PC_POWER			0x0003 //课件电脑电源
#define KEY_LIGHT_POWER		0x0004//课件电脑电源
#define KEY_GF_POWER			0x0005//功放电源
#define KEY_TYJ_POWER			0x0006//投影机电源
#define KEY_DEVICE_UP			0x0007//上升
#define KEY_DEVICE_STOP			0x0008//停止
#define KEY_DEVICE_DOWN		0x0009//下降
#define KEY_DEVICE_SELECT		0x000a//设备选择
#define KEY_HELP					0x000b//11 //求助
#define KEY_DEVICE_PC			0x010c//12//课件计算机
#define KEY_DEVICE_NPC			0x010d//13//笔记本
#define KEY_DEVICE_KZA			0x010e//14//
#define KEY_REC_PUSH			0x020f//15//录制/暂停
#define KEY_REC_STOP			0x0210//16 //录制停止
#define KEY_DEVICE_SHOW		0x0111//17//视频展台
#define KEY_DEVICE_DVD			0x0112//18//DVD
#define KEY_DEVICE_RECODER		0x0113//19//录音机
#define KEY_DEVICE_KZB			0x0114//20//扩展B
#define KEY_DEVICE_MUTE			0x0015//21//设备静音
#define KEY_MODE_SELECT		0x0216//22 //模式选择
#define KEY_METERIC_SELECT		0x0217//23//录制分辨率
#define KEY_FRAME_SELECT		0x0218//24//画面特效
#define KEY_SPEED_SELECT		0x0219//25//录制码率
#define KEY_TAIL_SELECT			0x021a//26//身高设定
#define KEY_SCAMERA_SELECT		0x021b//27 //摄像机设定
#define KEY_SCAMERA_ZOOM1		0x021c//28//Zoom+
#define KEY_SCAMERA_UP			0x021d//29//摄像上
#define KEY_SCAMERA_ZOOM2		0x021e//30//ZOOM-
#define KEY_SCAMERA_LEFT		0x021f//31//摄像左
#define KEY_SCAMERA_DOWN		0x0220//32//摄像下
#define KEY_SCAMERA_RIGHT		0x0221//33//摄像右
#define KEY_SCAMERA_STOP		0x0230//摄像停
#define KEY_CONTRL_BACK		0x0122//34//返回
#define KEY_CONTRL_UP			0x0123//35//控制上
#define KEY_CONTRL_ENTER		0x0124//36//确认
#define KEY_CONTRL_LEFT			0x0125//37//控制左
#define KEY_CONTRL_DOWN		0x0126//38//控制下
#define KEY_CONTRL_RIGHT		0x0127//39//控制右
#define KEY_CONTRL_SOUND		0x0128//40//声道
#define KEY_CONTRL_TITLE		0x0129//41//字幕
#define KEY_CONTRL_YY			0x012a//42//语言
#define KEY_CONTRL_PREV		0x012b//43//上一曲
#define KEY_CONTRL_QBACK		0x012c//44//快退
#define KEY_CONTRL_PLAY			0x012d//45//播放/暂停
#define KEY_CONTRL_QUIT		0x012e//46//快进
#define KEY_CONTRL_NEXT		0x012f//47//下一曲

//酷睿键盘新加入键值
#define KEY_SELECT_TYM			0x000c  //投影幕
#define KEY_SELECT_LCDUP		0x000d//液晶升降设备
#define KEY_SELECT_TYJUP		0x000e//投影机吊架
#define KEY_SELECT_PELMET		0x000f//窗帘

#define KEY_CONTRL_EJC			0x0130//DVD录像机出盒控制
#define KEY_CONTRL_STOP		0x0131//播放停止控制


#define KEY_REC_BAK                    0xE000 //荣耀面板录制备用键
#define KEY_DEV_BAK                    0xF000//荣耀面板设备备用间
//电源管理
#define KEY_REP_PC_POWER 		1		//计算机
#define KEY_REP_LIGHT_POWER	(1<<1)	//室内灯光
#define KEY_REP_GF_POWER		(1<<2)	//功放电源
#define KEY_REP_TYJ_POWER		(1<<3)	//投影机电源
//播放/暂停
#define KEY_REP_PLAY_PUSH		(1<<4)
//录制_暂停
#define KEY_REP_REC_PUSH		(1<<5)
//mic静音
#define KEY_REP_MIC_MUTE		(1<<6)
//帮助
#define KEY_REP_HELP			(1<<7)
//接管设备
#define KEY_REP_JGDEV			(1<<8)
#define KEY_REP_JGREC			(1<<9)
//设备静音
#define KEY_REP_DEVICE_MUTE	(1<<10)
//运行模式
#define KEY_REP_MODE			(1<<11)
//上课
#define KEY_REP_CLASS			(1<<12)
//预录制
#define KEY_REP_YREC			(1<<13)
//教学自习
#define KEY_REP_JXZX                   (1<<14)
//控制数据
#define KEY_CONTRL_RF			1 // RF卡有效（R）
#define KEY_CONTRL_MIC_MUTE	(1<<1)// MIC静音（R/W）
#define KEY_CONTRL_DEV_MUTE	(1<<2)//设备静音（R/W）
#define KEY_CONTRL_UP_START	(1<<3)//代码升级开始（R
#define KEY_CONTRL_UP_OVER		(1<<4)//代码升级成功（R）
struct KEY_TYPE
{
    uint16 	key_rep;            		 //二值开关
    uint8 	POWER_DEV;   		//	当前升降设备
    uint8       Curr_option;                 //当前的升降设备操作
    uint8 	CURR_DEVICE; 		//当前设备
    uint8  	MODE;				//模式
    uint8		PERSONAL_TALL;		//射高
    uint8 	CODE1_SPEED;		//码率
    uint8 	METRIC;				//分辨率
    uint8		FRAME;				//画面特效
    uint8 	CURR_SCAMERA;		//当前摄像机
    uint16      CONTRL;				//
    uint8		RF_DATA[32];		//
    uint8		RF_lg;				//
    uint32	TYJ_time;
};
#define LED_DISPLAY_ON		0//显示开
#define LED_DISPLAY_OFF		1//显示关
#define LED_DISPLAY_FLASH	2//闪烁
#define LED_ALL_CLR			3//显示全关
#define LED_ALL_SET			4//显示全开
#define LED_ALL_FLASH		5 //


#define LED_CONTRL_RF				1//RF卡有效（R）
#define LED_CONTRL_MIC_MUTE		(1<<1)// MIC静音（R/W）
#define LED_CONTRL_DEV_MUTE		(1<<2)//设备静音（R/W）
#define LED_CONTRL_TIME_OFF		(1<<3)//时钟关闭
#define LED_CONTRL_POWER_ON	(1<<4)//面板电源打开
#define LED_CONTRL_PROGRAM_BIGEN	(1<<14)//代码升级开始
#define LED_CONTRL_PROGRAM_END	(1<<15)//代码升级成功
struct LED_TYPE
{
    uint16 LED_data[4];
    uint16 LED_type[4];
    uint8   LED_time[8];
    uint16 Contrl;
};
void  key_init(void);
void key_msg_rev(uint8 *pdata);
void *Get_KeyData(void );
void Key_TB_state(uint16 flag, uint16 type);
void KEY_CMD(uint8 *pkey);
void LED_Set(uint32 val, uint8 type);
void LED_Time_Flash(void);
void LED_Time_Flash(void);
void LED_Contrl(uint16 Contrl, uint8 type);
void KEY_Set_Vol(uint8 Vol);
//Variable declare
extern struct KEY_TYPE KEY_DATA;

#endif

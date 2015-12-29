#ifndef _LSZH_MSG_H
#define _LSZH_MSG_H
//消息队列
#define MSG_MAX_SIZE   					20       //消息队列长度

//消息类型
#define MSG_UDP      						0x01
#define MSG_UART     						0x02
#define MSG_SYS       						0x04
#define MSG_KEY      						0x08
#define MSG_TIME    						0x10
//文件类型
#define FILE_TYPE_TABLE					0 //配置表
#define FILE_TYPE_USER_PRORAM			17 //用户代码
#define FILE_TYPE_CLASS_TABLE			18//课程表
#define FILE_TYPE_KB_PROGRAM			19//面板用户代码
#define FILE_TYPE_TRACK_PROGRAM		20//跟踪板用户代码
#define FILE_TYPE_TABLE_BASEINFO		21 //配置表基本信息
#define FILE_TYPE_CLASS_TABLE_ONEDAY	22//课程表一天

//设备类型
#define MSG_DEV_CLASSINFO   				0  //教室基本信息
#define MSG_DEV_OPENCLASS    			1 //开机参数
#define MSG_DEV_CLOSECLASS				2 //关机参数
#define MSG_DEV_CALSSTYPE1				3 //课程表1
#define MSG_DEV_CALSSTYPE2				4
#define MSG_DEV_CALSSTYPE3				5
#define MSG_DEV_CALSSTYPE4				6
#define MSG_DEV_CALSSTYPE5				7
#define MSG_DEV_CALSSTYPE6				8
#define MSG_DEV_CALSSTYPE7				9  //课程表7
#define MSG_DEV_AUTOTRACK    			10  //自动跟踪参数
#define MSG_DEV_FIXEDPOSITION 			11  //定位参数
#define MSG_DEV_STRATEGY1          			12  //策略1
#define MSG_DEV_STRATEGY2				13
#define MSG_DEV_HD1						14 //无线互动
#define MSG_DEV_HD2						15 //无线互动
#define MSG_DEV_MAINBARD            		16  //一体机
#define MSG_DEV_DVD   					17 //
#define MSG_DEV_LYJ						18  //录音机
#define MSG_DEV_PC						19
#define MSG_DEV_NPC						20
#define MSG_DEV_SHOW					21 //展台
#define MSG_DEV_KZA						22 //扩展
#define MSG_DEV_KZB						23
#define MSG_DEV_LMT						24 //流媒体
#define MSG_DEV_TYJA					25 //投影机
#define MSG_DEV_TYM						26//投影幕
#define MSG_DEV_GF						27 //功放
#define MSG_DEV_LIGHT					28 //室内灯光
#define MSG_DEV_PELMET					29 //窗帘
#define MSG_DEV_LCD						30 //液晶升降
#define MSG_DEV_TYJUP					31//投影机吊架
#define MSG_DEV_TRACK					32//跟拍设备
#define MSG_DEV_VIDEOCARD				33 //编码卡
#define MSG_DEV_POWER					35 //电源箱
#define MSG_DEV_ALM						36 //报警设备
#define MSG_DEV_CONTRL					37 //中控设备
#define MSG_DEV_TQCAMERA				38//教师球机
#define MSG_DEV_SQCAMERA				40//学生球机
#define MSG_DEV_BQCAMERA				41//备用球机
#define MSG_DEV_SCAMERA1				42 //板书摄象机1
#define MSG_DEV_SCAMERA2				43//板书摄象机2
#define MSG_DEV_SCAMERA3				44//板书摄象机3
#define MSG_DEV_TMIC					45 //教师麦克
#define MSG_DEV_SMIC					46//学生麦克
#define MSG_DEV_JMIC					47//界面麦克
#define MSG_DEV_DCS						48//电磁锁
#define MSG_DEV_KB						49//一体机扩展键盘
#define MSG_DEV_TB						50//一体机同步板
#define MSG_DEV_KZYY					51//一体机扩展语音板
#define MSG_DEV_TYJB					52 //投影机
#define MSG_DEV_LAN_KB					53 //网络键盘
#define MSG_DEV_CLASS_ID				0x55

#define MSG_DEV_TEACHER				170  //语音教学主控台
//设备控制消息
#define MSG_OPTION_NONE				0x00//无动作
#define MSG_OPTION_POWEROPEN			0x01 //设备打开/上升
#define MSG_OPTION_POWERCLOSE		0x03 //设备关闭/停
#define MSG_OPTION_POWERDOWN		0x02 //设备下降
#define MSG_OPTION_MUTEOPEN			0x04 //设备静音
#define MSG_OPTION_MUTECLOSE			0x05//设备静音关闭
#define MSG_OPTION_VOL					0x06 //设备音量
#define MSG_OPTION_RESET				0x07 //一体机复位
#define MSG_OPTION_OPEN_OVER			0x0b  //设备开机完成
#define MSG_OPTION_CLOSE_OVER			0x0c //设备关机完成
#define MSG_OPTION_DEV_LINK			0x0d //设备连接
#define MSG_OPTION_DEV_BREAK			0x0e //设备断开
#define MSG_OPTION_ENABLE_CLOSE		0x0f  //设备可以关闭


//播放控制消息
#define MSG_OPTION_PLAY					0x10 //播放
#define MSG_OPTION_PUSE					0x11 //暂停
#define MSG_OPTION_STOP					0x12 //停止
#define MSG_OPTION_GO						0x13 //前进
#define MSG_OPTION_BACK					0x14//后退
#define MSG_OPTION_PRE					0x15 //前一页
#define MSG_OPTION_NEXT					0x16//后一页
#define MSG_OPTION_UP						0x17//上
#define MSG_OPTION_DOWN					0x18//下
#define MSG_OPTION_LIFT					0x19//左
#define MSG_OPTION_RIGHT					0x1a//右
#define MSG_OPTION_ENTER					0x1b//确认
#define MSG_OPTION_RETURN					0x1c//返回
#define MSG_OPTION_LANGUAGE				0x1d//语言
#define MSG_OPTION_TITLE					0x1e//字幕
#define MSG_OPTION_TRACK					0x1f//声道
#define MSG_OPTION_OPEN					0x20 //开仓
#define MSG_OPTION_A						0x21//
#define MSG_OPTION_B						0x22
#define MSG_OPTION_ZOOM_ADD				0x23 //设备ZOOM+
#define MSG_OPTION_ZOOM_SUB				0x24//设备ZOOM-
#define MSG_OPTION_LT1_ON					0x25 //设备灯1打开
#define MSG_OPTION_LT1_OFF				0x26//设备灯1关闭
#define MSG_OPTION_LT2_ON					0x27//设备灯2打开
#define MSG_OPTION_LT2_OFF				0x28//设备灯2关闭
//录制操作
#define MSG_OPTION_RECORD_START			0x40  //录像开始
#define MSG_OPTION_RECORD_PAUSE			0x41 //录像暂停
#define MSG_OPTION_RECORD_STOP			0x42 //录像停止
#define MSG_OPTION_CODE1_SPEED1			0x43 // 码速1 500k
#define MSG_OPTION_CODE1_SPEED2			0x44// 码速2 1M
#define MSG_OPTION_CODE1_SPEED3			0x45 // 码速1 自定义
#define MSG_OPTION_METRIC1				0x46 //分辨率XGA
#define MSG_OPTION_METRIC2				0x47 //分辨率D1
#define MSG_OPTION_METRIC3				0x48 ////分辨率自定义
#define MSG_OPTION_FRAME1					0x49//画中画
#define MSG_OPTION_FRAME2					0x4a//常规
#define MSG_OPTION_FRAME3					0x4b //自定义
#define MSG_OPTION_REC_Freez				0x4c//录制通道画面冻结
#define MSG_OPTION_REC_UnFreez			0x4d//录制通道画面解冻
#define MSG_OPTION_REC_ClassName			0x4e//录制课程名称
#define MSG_OPTION_REC_Teachername		0x4f//录制教师名称
//摄象操作
#define MSG_OPTION_PERSONAL_TALL1       	0x30  //身高设定160
#define MSG_OPTION_PERSONAL_TALL2		0x31 //身高设定170
#define MSG_OPTION_PERSONAL_TALL3		0x32//身高设定180
#define MSG_OPTION_SELECT_SCAMERA1		0x33 //摄象机选择1
#define MSG_OPTION_SELECT_SCAMERA2		0x34 //摄象机选择2
#define MSG_OPTION_SELECT_SCAMERA3		0x35//摄象机选择3
#define MSG_OPTION_SELECT_SCAMERA4		0x36//摄象机选择4
#define MSG_OPTION_SELECT_SCAMERA5		0x37//摄象机选择5
#define MSG_OPTION_SELECT_SCAMERA6		0x38//摄象机选择6
#define MSG_OPTION_SCAMERA_ZOOMADD		0x39//摄象机ZOOM+
#define MSG_OPTION_SCAMERA_ZOOMSUB		0x3a//摄象机ZOOM-
#define MSG_OPTION_SCAMERA_UP			0x3b//摄象机上
#define MSG_OPTION_SCAMERA_DOWN			0x3c//摄象机下
#define MSG_OPTION_SCAMERA_LIFT			0x3d//摄象机左
#define MSG_OPTION_SCAMERA_RIGHT		0x3e//摄象机右
//zjj add1
#define MSG_OPTION_SCAMERA_STOP				0x3f //摄像机停
#define MSG_OPTION_SCAMERA_LEFTUP			0x29 //摄像机左上
#define MSG_OPTION_SCAMERA_LEFTDOWN		0x2a //摄像机左下
#define MSG_OPTION_SCAMERA_RIGHTUP			0x2b //摄像机右上
#define MSG_OPTION_SCAMERA_RIGHTDOWN		0x2c //摄像机右下
#define MSG_OPTION_SCAMERA_FOCUSNEAR		0x2d //摄像机变焦远
#define MSG_OPTION_SCAMERA_FOCUSFAR			0x2e //摄像机变焦近
#define MSG_OPTION_SCAMERA_PREPOSITION		0x2f //摄像机掉预置位
#define MSG_OPTION_SCAMERA_SETPREPOSITION 	0X5f//摄像机设置预置位

//zjj add1
//录制动作检测系统消息
#define MSG_OPTION_MOUSE_ACTIVE		0x50 //鼠标动作
#define MSG_OPTION_MOUSE_NONE		0x51//鼠标无动作
#define MSG_OPTION_KB_ACTIVE			0x52//键盘动作
#define MSG_OPTION_KB_NONE			0x53//键盘无动作
#define MSG_OPTION_PICTURE_ACTIVE	0x55//画面有变化
#define MSG_OPTION_PICTURE_NONE		0x56//画面无变化
#define MSG_OPTION_SOUND_ACTIVE		0x57//声音有变化
#define MSG_OPTION_SOUND_NONE		0x58//声音无变化
//业务操作
#define MSG_OPTION_CLASS_BEGIN     		0x70 //上课
#define MSG_OPTION_CLASS_OVER		0x71//下课
#define MSG_OPTION_RF_ACTIVE			0x72//rf卡有效
#define MSG_OPTION_HELP_ON			0x73//帮助开
#define MSG_OPTION_HELP_OFF			0x74//帮助管
#define MSG_OPTION_AUTO_ON			0x75//自动
#define MSG_OPTION_AUTO_OFF			0x76//手动
#define MSG_OPTION_DEV_REMOTE_ON	0x77//远程设备控制开
#define MSG_OPTION_REC_REMOTE_ON	0x78//远程设备控制开
#define MSG_OPTION_DEV_REMOTE_OFF	0x79//远程设备控制关
#define MSG_OPTION_REC_REMOTE_OFF	0x7a//远程录像控制关
#define MSG_OPTION_HW_BIGIN			0x7b//红外学码开始
#define MSG_OPTION_HW_END			0x7c//红外学码结束
//设备使用统计
#define MSG_OPTION_STAT_START		0x7d //0x4c  //设备统计开始
#define MSG_OPTION_STAT_END			0x7e //0x4d  //设备统计结束

#define MSG_OPTION_HELP_LINK			0x80//帮助通话连接
#define MSG_OPTION_HELP_BREAK			0x81//帮助通话断开
#define MSG_OPTION_AUTOTRACK_ON		0x82//自动跟拍开
#define MSG_OPTION_AUTOTRACK_OFF		0x83//自动跟拍关
#define MSG_OPTION_STRATEGY_ON		0x84//自动策略开
#define MSG_OPTION_STRATEGY_OFF		0x85//自动策略关
#define MSG_OPTION_HD_ON				0x86//无线互动开
#define MSG_OPTION_HD_OFF				0x87//无线互动关
#define MSG_OPTION_HD_VAL				0x88//无线互动按键值
#define MSG_OPTION_SCAMERA_CONTRL	0x89 //摄象机控制邋（参数）
#define MSG_OPTION_SCAMERA_BackVAL	0x8a//摄象机返回参数
#define MSG_OPTION_REC_ReqStatus		0x8b//录制通道信息查询?煊?
#define MSG_OPTION_REC_Contrl			0x8c//录制通道控制
#define MSG_OPTION_LMT_ReqStatus         0x8d//请求/返回流媒体状态
#define MSG_OPTION_TB_ReqStatus            0x8e //请求/返回同步卡信号状态

//信道选择控制
#define MSG_OPTION_SOUND_SELECT		0x90  //声音通道控制
#define MSG_OPTION_SOUND_VOL			0x91 //音量控制
#define MSG_OPTION_VIDEO_SELECT		0x92 //视频控制
#define MSG_OPTION_VGA_SELECT			0x93//VGA控制
#define MSG_OPTION_ARLM				0x94 //报警状态改变
#define MSG_OPTION_IO_OUT				0x95//报警IO输出
#define MSG_OPTION_JT_ON				0x96//监听开
#define MSG_OPTION_JT_OFF				0x97//监听关
#define MSG_OPTION_POWER				0x98 //电源控制消息
#define MSG_OPTION_IPPlay				0x99 //IP音频播放控制
#define MSG_OPTION_IP_DPlay                    0x9a //ip对讲+回声抑制
#define MSG_OPTION_IP_MICVol			0x9b //ip对讲mic 音量控制
#define MSG_OPTION_IP_HPVol			0x9c //ip对讲扬声器音量控制


//场景
#define MSG_OPTION_FRAME_SELECT		0xa0 //场景选择
#define MSG_OPTION_LMT_Exit			0xa1//流媒体软件退出
#define MSG_OPTION_LMT_BackPlay		0xa2 //流媒体软件回放
#define MSG_OPTION_YY_Teather			0xa3 //语音教学模式
#define MSG_OPTION_YY_NoTeather		0xa4 //语音自习模式
#define MSG_OPTION_YREC_Begin			0xa5//预录制开始
#define MSG_OPTION_YREC_Stop			0xa6//预录制停止
//杂项
#define MSG_OPTION_SVRLINK_REQ		0xb0  //服务器连接
#define MSG_OPTION_SEVER_ACK			0xb1//服务器应答
#define MSG_OPTION_FILE_REQ			0xb2//文件请求
#define MSG_OPTION_FILE_SEND			0xb3//文件传送
#define MSG_OPTION_CLASSLINK_REQ		0xb4//连接请求
#define MSG_OPTION_DEBUG				0xb5//调试信号输出
#define MSG_OPTION_STATUS_CHANGE		0xb6 //一体机状态改变
#define MSG_OPTION_DEV_RESET                   0xb7 //设备复位
#define MSG_OPTION_LAN_KEY				0xb8 //网络键盘
#define MSG_OPTION_LAN_LED				0xb9//网络键盘led
#define MSG_OPTION_DEVICE_BUSY		0xba //一体机设备忙，带已经连接IP
//zjj add
#define MSG_OPTION_REPEATPOWERCLOSE 	0xbb
#define MSG_OPTION_SERVERWRITE			0xbc
#define MSG_OPTION_SERVERWRITE_ACK		0xbd
#define MSG_OPTION_CardContrl			0xbe //校园一卡通控制身份认证


//定时报警
#define MSG_OPTION_TIMEACTIVE			0xc0//一秒消息
#define MSG_OPTION_ALM_TIME			0xc1//报警时间到
#define MSG_OPTION_PBELL_OPEN			0xc2//电源箱报警开
#define MSG_OPTION_PBELL_COLSE		0xc3//电源箱报警关
#define MSG_OPTION_ASOUND_PLAY		0xc4//报警音频音频播放
#define MSG_OPTION_ASOUND_PUSH		0xc5//报警音频音频暂停
#define MSG_OPTION_ASOUND_STOP		0xc6//报警音频音频停止
#define MSG_OPTION_COMM				0xc7//设备通讯(用于系统的延时端口发送)
#define MSG_OPTION_HW_SEND			0xc8//红外发送
#define MSG_OPTION_MSG_RUN			0xc9//电源开关延时消息
#define MSG_OPTION_DEVTABLE_NEW		0xca//设备列表更新
#define MSG_OPTION_CLASSTABLE_ON		0xcb//课表/定时开
#define MSG_OPTION_CLASSTABLE_OFF		0xcc//课表/定时关
#define MSG_OPTION_POWERCARD_ONOFF   0xcd//电源管理卡操作
#define MSG_OPTION_TYJ_DIS_VGA		0xce//投影机显示VGA
#define MSG_OPTION_TYJ_DIS_VIDEO		0xcf//投影机显示VIDEO

//以前的测试IO口输入输出
#define MSG_OPTION_TEST_IO			0xf0
//串口测试
#define MSG_OPTION_UART_TEST			0xf1
//io和投影机连接测试
#define MSG_OPTION_IO_TEST			0xf2
//外部网络控制面板灯消息
#define MSG_OPTION_LEDContrl			0xf3



//智能教师基本信息定义
#define TCLASSINFO_ID                  		0   //设备ID数值型【1字节】
#define TCLASSINFO_DeviceType    		1   //=16 设备类型数值型【1字节】
#define TCLASSINFO_SchoolName   		2  //学校名称字符串【32字节】
#define TCLASSINFO_HouseName   			34 //楼宇名称字符串【32字节】
#define TCLASSINFO_ClassName    			66 //教室名称字符串【32字节】
#define TCLASSINFO_ClassType     			98 //智能教室类型数值【2字节】
#define TCLASSINFO_ClassID	    			100 //智能教室识别码     数值【8字节】
#define TCLASSINFO_ClassIP          			108  //智能教室IP地址     数值【8字节】
#define TCLASSINFO_ClassGateIP   			116//智能教室网关IP地址     数值【8字节】
#define TCLASSINFO_JKSeverIP      		124 //集控软件邋IP数值【8字节】
#define TCLASSINFO_AlarmSet        		132 //报警布防控制数值时间【4字节】
#define TCLASSINFO_ConfigTimes   		136 //配置表配置次数  数值【4字节】
#define TCLASSINFO_WatchNo        		140 //监控通道配置    数值【1字节】
#define TCLASSINFO_ContrlType          		141 //开机控制类型 数值【1字节】
#define TCLASSINFO_ContrlType_Manual    		0  //手动开机
#define TCLASSINFO_ContrlType_Time        		1  //定时开机
#define TCLASSINFO_ContrlType_ClassTable    	2  //课程表开机
#define TCLASSINFO_PictureEditIP      		142   //图像编辑卡邋IP数值【8字节】
#define TCLASSINFO_PictureCoding1     		146  //视频编码卡邋IP
#define TCLASSINFO_PictureCoding2    		150  //视频编码卡邋IP
#define TCLASSINFO_LMT_IP		   		154  //流媒体邋IP
#define TCLASSINFO_TeacherPC_IP    		158  //教师计数机邋IP
#define TCLASSINFO_nPC_IP		   		162 //笔记本邋IP【未使用】
#define TCLASSINFO_WatchType             	166 //监控型号
#define TCLASSINFO_AlarmContrl             	167 //报警布防开关设置
#define TCLASSINFO_TT_Set                    	168 //语音主控台TT
#define CLASSINFO_TT_Set_Def	0  //未定义
#define CLASSINFO_TT_Set_ON	1  //TT主控台打开
#define CLASSINFO_TT_Set_OFF	2 //TT主控台关闭
//设备管理表地址定义
#define TDevice_ID						0//
#define TDevice_Type						1//
#define TDevice_DeviceName				2//
#define TDevice_TypeName				34//
#define TDevice_VGALink					66//
#define TDevice_VGA_Input				66//
#define TDevice_VGA_Output				67//
#define TDevice_VGA_DefLink				68//
#define TDevice_VGA_BAK					69//
#define TDevice_VideoLink					70//
#define TDevice_Video_Input				70//
#define TDevice_Video_Output				71//
#define TDevice_Video_DefLink				72//
#define TDevice_Video_BAK				73//
#define TDevice_SoundLink				74//
#define TDevice_Sound_Input				74//
#define TDevice_Sound_Output				75//
#define TDevice_Sound_DefLink			76//
#define TDevice_Sound_Vol				77//
#define TDevice_IO_InputLink				78//
#define TDevice_Input_No					78//
#define TDevice_Input_Property			79//
#define TDevice_Input_Property_High		(1<<0)//
#define TDevice_Input_Property_AlarmOn    (1<<1)//
#define TDevice_Input_Property_ALarmPlay (1<<2)//
#define TDevice_Input_Property_AlarmRep  (1<<3)//
#define TDevice_Input_Property_AlarmGF	(1<<4)//
#define TDevice_Input_LinkSoundNo		80//
#define TDevice_Input_BAK				81//
#define TDevice_IO_OutputLink			82//
#define TDevice_Output_No				82//
#define TDevice_Output_Property			83//
#define TDevice_Output_Property_JDQOn	(1<<0)//
#define TDevice_Output_Property_JDQ
#define TDevice_Output_LinkSoundNo		84//
#define TDevice_Output_BAK				85//
#define TDevice_PowerLink				86//
#define TDevice_PowerLink_SUB		86//
#define TDevice_PowerLink_No              87
#define TDevice_XCOMLink						88//
#define TDevice_XCOMLink_Type			88
#define TDevice_XCOMLink_Val				90
#define TDevice_SelectMsg						98
#define TDevice_DevicePowerOnMsg				100
#define TDevice_DevicePowerOffMsg				102
#define TDevice_DevicePowerOffTime				104

////////流媒体软件状态/////////////////////////////////////////////
#define LMT_STATUS_NONE                                 0 //0：空闲
#define LMT_STATUS_REC_Push				  1////1：暂停录制
#define LMT_STATUS_REC_Stop				  2////2：停止录制
#define LMT_STATUS_BackPlay					  3////3：回放录制
#define LMT_STATUS_RECing					  4//// 	录制
#define LMT_STATUS_Pushing					  5///5：正在暂停（收到暂停命令，未执行完）
#define LMT_STATUS_Stoping					  6////6：正在停止（收到停止命令，未执行完）
#define LMT_STATUS_Exiting					  7////7：正在退出				
#define LMT_STATUS_Exit				               8//////8：退出完成（退出前发送）一体机收到次状态后，延时1分钟关闭流媒体计算机


#define LMT_CONTRL_PWon			1
#define LMT_CONTRL_PWoff		(1<<1)
#define LMT_CONTRL_Set			(1<<2) //流媒体安装
#define LMT_CONTRL_EXIT			(1<<3)//流媒体软件退出

#define LMT_CONTRL_time			10
#define LMT_CONTRL_Rep				3
struct LMT_CONTRL_TYPE
{
    uint8 Device;
    uint8 option;
    uint8 option_time;
    uint8 status;
    uint8 reptimes;
    uint8 LMT_status;
    uint16 power_no;//电源通道
};



#endif

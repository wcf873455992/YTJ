#ifndef _LSZH_HARDWARE_H
#define _LSZH_HARDWARE_H
//设备连接定义----通讯
#define DEV_XCOM_DW					0x00//定位板串口(编辑卡)
#define DEV_XCOM_TYJ					0x01//投影机串口RS232
#define DEV_XCOM_SOUND					0x02//音频级联板(新版一体机3.0以后用作读卡器串口)
#define DEV_XCOM_TB						0x03//同步板串口(增益卡)(新版一体机3.0以后用做球机控制)

#define DEV_XCOM_MB					0x04//面板串口
#define DEVICE_SUB_MB				0x08 //面板子地址
#define DEVICE_SUB_POWER1			0x00//电源箱1子地址
#define DEVICE_SUB_POWER2			0x01//电源箱2子地址
#define DEVICE_SUB_POWER3			0x02//电源箱3子地址
#define DEV_XCOM_SCAMERA				0x05//球机控制串口		
//#define DEV_XCOM_TB						0x03//同步板串口
//#define DEV_XCOM_SOUND					0x04//音频级联板
//#define DEV_XCOM_TYJ					0x05//投影机串口
//#define DEV_XCOM_RS485					0x06//扩展RS485
//#define DEV_XCOM_SCAMERA				0x07//球机控制串口
#define DEV_XCOM_UDP					0x06 //网络通道
#define DEVICE_SUB_CLASS		0x16 //一体机
#define DEVICE_SUB_GATE			0x26//网关
#define DEVICE_SUB_SER			0x36//集控服务器
#define DEVICE_SUB_LMT			0x46//流媒体
#define DEVICE_SUB_PC			0x56//课件计算机
#define DEVICE_SUB_CMD			0x66//上次命令
#define DEVICE_SUB_CAST			0x76//广播
#define DEVICE_SUB_TEST			0x86 //调试软件
#define DEVICE_SUB_BJCard			0x96 //调试软件
#define DEV_XCOM_MAX_SIZE				7
#define DEV_XCOM_DEBUG					7
#define DEV_XCOM_HW					        0x81 //红外通道
#define DEV_XCOM_SELF					0x82 //一体机发出的消息
#define DEV_XCOM_RF					        0x83 //RF卡程序发出的消息
#define DEV_XCOM_TIME				        0x84 //定时器发出的消息
#define DEV_XCOM_TABLE					0x85 //开关机序列发出的消息
#define DEV_XCOM_FINGERPRINT                            0x90 //指纹识别发出的消息

//通讯端口
#define UDP_SOUND_PORT       				8898   //音频端口
#define UDP_COMMAND_PORT  				8006 //命令端口
#define UDP_DEBUG_PORT            			5050
#define UDP_DEBUG_LOG_PORT            		7777 //Log 
#define UDP_DEBUG_CMD_PORT				4002 //		
#define UDP_LAN_TEST_PORT				8889

//设备连接定义----音频输入通道
#define DEV_SOUND_NONE					0x00 //声音无定义
#define DEV_SOUND_MIC1					0x01//麦克1(面板会议，鹅颈，头戴)
#define DEV_SOUND_MIC2					0x02//麦克2
#define DEV_SOUND_MIC3					0x03//麦克3
#define DEV_SOUND_MIC4					0x04//麦克4
#define DEV_SOUND_DVD					0x05 //DVD
#define DEV_SOUND_LYJ					0x06//录音机
#define DEV_SOUND_PC					0x07 //计算机
#define DEV_SOUND_NPC					0x08//笔记本
#define DEV_SOUND_KZ					0x09//音频扩展
#define DEV_SOUND_DEV					0x0A//设备
//设备连接定义----音频输出通道
#define DEV_SOUND_OUT_NONE			0x0 //音频输出无定义
#define DEV_SOUND_OUT_GF				0x1 //功放输出
#define DEV_SOUND_OUT_LMT				0x2//流媒体输出
#define DEV_SOUND_OUT_YSQ				0x3//扬声器输出
#define DEV_SOUND_OUT_BMK				0x4//编码卡输出
//设备连接定义----视频输入通道
#define DEV_VIDEO_NONE					0x00 //视频无定义
#define DEV_VIDEO_DVD					0x01 //DVD
#define DEV_VIDEO_LXJ					0x02//录音机
#define DEV_VIDEO_SHOW					0x03//站台
#define DEV_VIDEO_QSCAMERA1			0x04//教师球机
#define DEV_VIDEO_QSCAMERA2			0x05//学生球机
#define DEV_VIDEO_QSCAMERA3			0x06//备用球机
#define DEV_VIDEO_SCAMERA1				0x07 //板书1
#define DEV_VIDEO_SCAMERA2				0x08//板书2	
#define DEV_VIDEO_SCAMERA3				0x09//板书3
#define DEV_VIDEO_VGATOAV				0x0a //VGATOAV
#define DEV_VIDEO_IN1					0x0b//视频输入1
#define DEV_VIDEO_IN2					0x0c//视频输入2
#define DEV_VIDEO_IN3					0x0d//视频输入3
#define DEV_VIDEO_IN4					0x0e//视频输入4
#define DEV_VIDEO_IN5					0x0f//视频输入5
#define DEV_VIDEO_IN6					0x10//视频输入6
//设备连接定义----视频输出通道
#define DEV_VIDEO_OUT_NONE			0x0 //视频输出无定义
#define DEV_VIDEO_OUT_TB0				0x01//同步板0
#define DEV_VIDEO_OUT_TB1				0x02//同步板1
#define DEV_VIDEO_OUT_BM1_1			0x03//编码板1-1
#define DEV_VIDEO_OUT_BM1_2			0x04//编码板1-2
#define DEV_VIDEO_OUT_BM1_3			0x05//编码板1-3
#define DEV_VIDEO_OUT_BM1_4			0x06//编码板1-4
#define DEV_VIDEO_OUT_BM2_1			0x07//编码板2-1
#define DEV_VIDEO_OUT_BM2_2			0x08//编码板2-2
#define DEV_VIDEO_OUT_BM2_3			0x09//编码板2-3
#define DEV_VIDEO_OUT_BM2_4			0x0a//编码板2-4
#define DEV_VIDEO_OUT_FZ1				0x0b//辅助输出1
#define DEV_VIDEO_OUT_FZ2				0x0c//辅助输出2
#define DEV_VIDEO_OUT_FZ3				0x0d//辅助输出3
#define DEV_VIDEO_OUT_TYJA			0x0e//辅助输出4/投影机A
#define DEV_VIDEO_OUT_TYJB			0x0f//辅助输出5投影机B
#define DEV_VIDEO_OUT_FZ6				0x10//辅助输出6
//设备连接定义----VGA输入通道
#define DEV_VGA_NONE					0x00 //VGA无定义
#define DEV_VGA_PC						0x01 //计算机
#define DEV_VGA_DVD						0x02//DVD
#define DEV_VGA_SHOW					0x03//展台
#define DEV_VGA_NPC						0x04//笔记本
//设备连接定义----VGA输出通道
#define DEV_VGA_OUT_NONE				0x0 //VGA输出无定义
#define DEV_VGA_OUT_TB0				0x1//同步0
#define DEV_VGA_OUT_TB1				0x2//同步1				
#define DEV_VGA_OUT_TYJ					0x3//投影机
//设备连接定义----IO输入通道
#define DEV_IO_NONE						0x00  //输入无定义
#define DEV_IO_IN_ALARM1				0x01	//IN_ALARM1//报警1
#define DEV_IO_IN_ALARM2				0x02	//IN_ALARM2//报警2
#define DEV_IO_IN_ALARM3				0x03	//IN_ALARM3//报警3
#define DEV_IO_IN_ALARM4				0x04	//IN_ALARM4//报警4
#define DEV_IO_IN_ALARM5				0x05	//IN_ALARM5//报警5
#define DEV_IO_IN_ALARM6				0x06	//IN_ALARM6//报警6
#define DEV_IO_IN_PC1POWER				0x07	//IN_PC1POWER//课件计算机控制
#define DEV_IO_IN_PC2POWER				0x08	//IN_PC2POWER//流媒体计算机控制
#define DEV_IO_IN_VGA					0x09	//IN_TYJVGA//投影机断线
//设备连接定义----IO输出通道
#define DEV_IO_OUT_NONE				0x00
#define DEV_IO_OUT_ALARM1				0x01	//OUT_ALARM1//报警输出1 
#define DEV_IO_OUT_ALARM2				0x02	//OUT_ALARM2//报警输出2
#define DEV_IO_OUT_ALARM3				0x03	//OUT_ALARM3//报警输出3
#define DEV_IO_OUT_PC1POWER			0x04	//OUT_PC1POWER//课件计算机开机
#define DEV_IO_OUT_PC2POWER			0x05	//OUT_PC2POWER//流媒体计算机开机
#define DEV_IO_OUT_DCS					0x06	//OUT_DCS//电磁锁控制
#define DEV_IO_OUT_M3					0x07
#define DEV_IO_OUT_M4					0x08


//设备音量控制
#define VOL_CH_DEVGF			0x01   //设备音量TO_GF  U37_in1
#define VOL_CH_MIC1				0x02	//MIC1	U37_IN3	
#define VOL_CH_MIC2				0x03	//MIC2	U37_IN4	
#define VOL_CH_MIC3				0x04	//MIC3	U37_IN5	
#define VOL_CH_MIC4				0x05	//MIC4	U37_IN6	
#define VOL_CH_DEVMID			0x06	//U42_IN1
#define VOL_CH_GF				0x07	//U42_IN3
#define VOL_CH_MID				0x08	//U42_IN5
#endif

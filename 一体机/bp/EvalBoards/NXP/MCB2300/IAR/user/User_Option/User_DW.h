#ifndef __USER_DW_H
#define __USER_DW_H

#define BJC_SUB                           0x0b

#define  BJC_CMD_Ver   		0  //读版本号
#define  BJC_CMD_CSContrl 		2//超声波定位器控制通道
#define  BJC_CMD_DATA		3//数据测量通道
#define  BJC_CMD_CSReturn		4//超声波返回信号数值
#define  BJC_CMD_JDATA		5//测距校准数据
#define  BJC_CMD_WDVal		6//读温度值
#define  BJC_CMD_WXval		7//读/写无线MIC的设置
#define  BJC_CMD_WXStutas		8//无线MIC状态（RO只读）当状态改变时发送核心板
#define  BJC_CMD_WXSet	        9//无线遥控器设置（包括迅捷改造的无线MIC接收机）
#define  BJC_CMD_WXREVStatus 10//无线遥控器状态
#define  BJC_CMD_Vol			11//设置/读取MIC音量值
#define  BJC_CMD_WXPower		12//读取无线MIC载波强度
#define  BJC_CMD_CoreIP		13//核心板读写底板IP
#define  BJC_CMD_CardNo		14//校园一卡通卡号
#define  XCOM_CMD_PRINT          85 //串口打印消息
#define XCOM_CMD_TEST	        170 //串口测试指令


#define RF_CARD_BIGEN    			1
#define RF_CARD_TIME_OVER	    (1<<1)
/////////////RF卡身份认证////////////////////
#define RF_ATT_ERR		0  	//RF 认证错误，没有用户
#define RF_ATT_REQ		1	//RF一体机认证请求
#define RF_ATT_OK		2	//RF认证通过

#define K103_Head		0xbb
#define K103_sHead		0xFF

struct YXJ_TYPE
{
    uint8 type;
    uint8 time;
};

struct RF_CARD_TYPE
{
    uint16 state;
    uint16 CMD_Time;
    uint8 Card_No[22];
};

struct UART_TR_TYPE
{
    uint8 Type;  //
    uint8 port;
    uint8 lg;
    uint8 bak;
    uint8 *pdata;


};

struct RFIC_DATA_TYPE
{
    uint8 buf[32];  		//卡数据缓冲区
    uint8 Card_No[32];	//卡号，字符
    uint8 lg;			//缓冲区长度
    uint8 RFIC_TYPE;	//卡类型
    //uint8 State;			//读卡器状态
    //uint8 Power_count;    //电源复位计数
    //uint16 Power_Reset_time;//电源复位时间
    //uint16 LED_Flash_time;  //LED闪烁时间连接0.5s，未连接0.2s
    //uint16 Check_Time;	//读卡器连接时间检测
    //OS_EVENT  *SendFlag;
    //uint32 Count;
};

void DW_init(void);
void DWQ_Contrl(uint8 val);
void DW_Read_WD(void);
void DW_CMD_Contrl(uint8 *pdata);
void RF_CARD_Check(void);


static uint8 _K103_sum_check(uint8 *pdata, uint8 lg);
static uint8  _ic_find_syn(uint8 ic_type, uint8 *pdata, uint8 lg);
int RFIC_K103_Mode(uint8 *pdata);











#endif

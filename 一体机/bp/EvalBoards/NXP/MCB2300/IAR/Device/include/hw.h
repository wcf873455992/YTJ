#ifndef  __hw_H
#define  __hw_H


#define HW_FRME_TIME_MIN    5 //ms 

/////////////////////////////////////////////
#define DEV_HW_FLAG_BUSY 		1
#define DEV_HW_FLAG_XM       		(1<<1)  //红外学码
#define DEV_HW_FLAG_SEND   		(1<<2) //红外发码
#define DEV_HW_FLAG_XMFIRST	(1<<3)
#define DEV_HW_FLAG_XMDWON	(1<<4)
#define DEV_HW_FLAG_XM_OVER	(1<<5)
#define DEV_HW_FLAG_XM_OK		(1<<6)
#define HW_DATA_SEND 	0x8000  //红外数据高电平标志
#define HW_DATA_SIZE  128
/////////////////////////////////////////////////////
#define HW_PR_TIME10us		Fpclk / 100000-1
#define HW_TIME10MS		100*10
#define HW_TIME200MS		HW_TIME10MS*20
#define HW_TIME1MIN		HW_TIME10MS *100*60
//红外编码采用128位码每位4字节128*4=512
//编码间隔为200ms，
struct HW_SEND_TYPE
{
    uint16 Currlength;
    uint16 CurrSendLg;
    uint8  Curr_id;
    uint8  id_buf[8];
};
struct HW_XM_TYPE
{
    uint16	lg;
    uint32	cap0_time;
    uint8 	*buf;
};
struct DEV_HW_TYPE
{
    uint16 	flag;
    struct HW_XM_TYPE  XM;
    struct HW_SEND_TYPE send;
};

void HW_init(void);
void HW_XM_start(void *pdata);
BYTE  HW_send(uint8 id);
void On_MSG_OPTION_HW_END(void *pdata);
void  Time0_Exception(void);
#endif

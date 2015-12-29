#ifndef __USER_ON_MSG_H
#define __USER_ON_MSG_H
/////////////////////////////////////////////
struct STIME_LINK_TYPE//服务器连接响应数据
{
    uint8 yearH;
    uint8 yearL;
    uint8 mon;
    uint8 day;
    uint8 week;
    uint8 hour;
    uint8  min;
    uint8 sec;
};
/*
文件请求（2字节）+文件类型（2字节）+文件页号（2字节）
文件发送（2字节）+文件类型（2字节）+文件页号（2字节）+256数据
*/
struct SFILE_TRANSLATE_TYPE
{
    uint16 FileType;
    uint16 FilePage;
};

#define STATUS_CHANGE			1
////////////////////////////////////////////
uint32 On_MSG_OPTION_MSG_RUN(void *pdata);
uint32 On_MSG_OPTION_SVRLINK_REQ(void *pdata);
uint32 On_MSG_OPTION_SEVER_ACK(void *pdata);
uint32 On_MSG_OPTION_FILE_REQ(void *pdata);
uint32 On_MSG_OPTION_FILE_SEND(void *pdata);
uint32 On_MSG_OPTION_SERVER_FILE_SEND(void *pdata);
uint32 ON_Change_State(struct BASE_MSG *pmsg);
uint32 ON_Device_select(void *pdata);
uint32 On_MSG_OPTION(void *pdata);
uint32 On_MSG_OPTION_TIMEACTIVE(BYTE *pdata);
uint32 On_MSG_OPTION_HW_SEND(BYTE *pdata);
uint32 On_MSG_OPTION_SOUND_VOL(void *pdata);
uint32 On_MSG_OPTION_VIDEO_SELECT(void *pdata);
uint32  On_MSG_OPTION_VGA_SELECT(void *pdata);
uint32 On_MSG_OPTION_ARLM(void *pdata);
uint32 On_MSG_OPTION_IO_OUT(void *pdata);
uint32 On_MSG_OPTION_JT(void *pdata);
uint32 On_MSG_OPTION_SOUND_SELECT(void *pdata);
uint32 On_MSG_OPTION_SCAMERA(void *pdata);
uint32 On_MSG_OPTION_SELECT_SCAMERA(void *pdata);
uint32 On_MSG_OPTION_PERSONAL_TALL(void *pdata);
uint32 On_MSG_OPTION_CODE1_SPEED(void *pdata);
uint32 On_MSG_OPTION_FRAME(void *pdata);
uint32 On_MSG_OPTION_METRIC(void *pdata);
uint32 On_MSG_OPTION_RECORD(void *pdata);
uint32 On_MSG_OPTION_CLASS_BEGIN(void *pdata);
uint32 On_MSG_OPTION_CLASS_OVER(void *pdata);
uint32 On_MSG_OPTION_RF_ACTIVE(void *pdata);
uint32 On_MSG_OPTION_HELP_ON(void *pdata);
uint32 On_MSG_OPTION_HELP_OFF(void *pdata);
uint32 On_MSG_OPTION_AUTO_ON(void *pdata);
uint32 On_MSG_OPTION_CardContrl(void *pdata);

uint32 On_MSG_OPTION_AUTO_OFF(void *pdata);
uint32 On_MSG_OPTION_DEV_REMOTE_ON(void *pdata);
uint32 On_MSG_OPTION_REC_REMOTE_ON(void *pdata);
uint32 On_MSG_OPTION_DEV_REMOTE_OFF(void *pdata);
uint32 On_MSG_OPTION_REC_REMOTE_OFF(void *pdata);
uint32 On_MSG_OPTION_HELP_LINK(void *pdata);
uint32 On_MSG_OPTION_HELP_BREAK(void *pdata);
uint32 On_MSG_OPTION_AUTOTRACK_ON(void *pdata);
uint32 On_MSG_OPTION_AUTOTRACK_OFF(void *pdata);
uint32 On_MSG_OPTION_STRATEGY_ON(void *pdata);
uint32 On_MSG_OPTION_STRATEGY_OFF(void *pdata);
uint32 On_MSG_OPTION_HD_ON(void *pdata);
uint32 On_MSG_OPTION_HD_OFF(void *pdata);
uint32 On_MSG_OPTION_HD_VAL(void *pdata);
uint32 On_MSG_OPTION_SCAMERA_CONTRL(void *pdata);
uint32 On_MSG_OPTION_SCAMERA_VAL(void *pdata);
uint32 On_MSG_OPTION_DEBUG(void *pdata);
uint32 On_MSG_OPTION_DEV_RESET(void *pdata);
uint32 On_MSG_OPTION_POWER(void *pdata);
uint32 On_MSG_Power_OPTION(void *pdata);
uint32 On_MSG_RepeatPower_OPTION(void *pdata);
uint32 On_MSG_OPTION_HELP_TEST(void *pdata);
void  On_MSG_OPTION_STATUS_CHANGE(BYTE *pdata);
uint32 ON_Sound_Mute(void *pdata);
//zjj add
uint32 Send_State(void);
//zjj add
uint32 On_MSG_OPTION_TEST_IO(void *pdata);
uint32 On_MSG_OPTION_IO_TEST(void *pdata);
uint32 On_MSG_OPTION_LEDContrl(void *pdata);
uint32 On_MSG_OPTION_REC_Contrl(void *pdata);
uint32 On_MSG_OPTION_REC_ReqStatus(void *pdata);
uint32 On_MSG_OPTION_LMT_ReqStatus(void *pdata);
uint32 On_MSG_OPTION_IPPlay(void *pdata);
uint32 on_MSG_OPTION_TB_ReqStatus(void *pdata);
uint32 On_MSG_OPTION_YY_Teather(void *pdata);
uint32 On_MSG_OPTION_TYJ_DIS_VGA(void *pdata);
uint32 On_MSG_OPTION_CardContrl(void *pdata);
void Config_printf(uint8 page_id);
uint32 on_MSG_OPTION_IP_MICVol(void *pdata);
uint32  On_MSG_OPTION_IP_DPlay(void *pdata);
uint32 on_MSG_OPTION_IP_HPVol(void *pdata);





#endif

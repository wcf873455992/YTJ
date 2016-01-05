#include "includes.h"
#include "PelcoD.h"
#include "User_DW.h"
extern struct LMT_CONTRL_TYPE  gLMT_Power;
extern struct LINK_REC_TABLE_TYPE DEVICE_LINK;
extern struct COMM_XCOM XCOM_PORT[DEV_XCOM_MAX_SIZE];
const uint8 Encode_TAB[8] =
{6, 5, 4, 3, 2, 1, 16, 15};
static uint8 ViscaCount = 0;
extern struct YXJ_TYPE gYXJ;
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：flag=1 Mute on
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 ON_Sound_Mute(void *pdata)
{
    struct BASE_MSG *pmsg;
    uint16 tmp;
    uint32 flag = 0;
    struct MSG_REV_TYPE *prev;

    prev = (struct MSG_REV_TYPE *)pdata;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    tmp = Device_Get_MIX();
    if (pmsg->option == MSG_OPTION_MUTEOPEN)
    {
        switch(pmsg->device)
        {
        case MSG_DEV_TMIC:
            Device_InVol( 0, DEV_SOUND_MIC1);
            LED_Contrl(LED_CONTRL_MIC_MUTE, TRUE);
            break;
        case MSG_DEV_SMIC:
            Device_Set_MIX(tmp | SMIX_CONTRLMIC2);
            break;
        case MSG_DEV_JMIC:
            Device_Set_MIX(tmp | SMIX_CONTRLMIC3);
            break;
        case MSG_DEV_GF:
            SoundVolumeControl(VOL_CH_GF, 0);
            gDeviceStatus.MK_Vol |= 0x8000;
            flag = 1;
            break;
        case MSG_DEV_DVD:
        case MSG_DEV_LYJ:
        case MSG_DEV_PC:
        case MSG_DEV_NPC:
        case MSG_DEV_KZA:
        case MSG_DEV_KZB:
            Device_Set_MIX(tmp | SMIX_DEVICE);
            break;
        }
    }
    else
    {
        switch(pmsg->device)
        {
        case MSG_DEV_TMIC:
            LED_Contrl(LED_CONTRL_MIC_MUTE, FALSE);
            Device_InVol( 98, DEV_SOUND_MIC1);
            break;
        case MSG_DEV_SMIC:
            Device_Set_MIX(tmp & (~SMIX_CONTRLMIC2));
            break;
        case MSG_DEV_JMIC:
            Device_Set_MIX(tmp & (~SMIX_CONTRLMIC3));
            break;
        case MSG_DEV_GF:
            SoundVolumeControl(VOL_CH_GF, DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol);
            gDeviceStatus.MK_Vol &= 0x7FFF;
            flag = 1;
            break;
        case MSG_DEV_DVD:
        case MSG_DEV_LYJ:
        case MSG_DEV_PC:
        case MSG_DEV_NPC:
        case MSG_DEV_KZA:
        case MSG_DEV_KZB:
            Device_Set_MIX(tmp & (~SMIX_DEVICE));
            break;
        }
    }
    prev = prev;
    return flag;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：void On_MSG_OPTION_MSG_RUN(void *pdata)
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_MSG_RUN(void *pdata)
{
    struct CLASS_OPTION_REC classmsg;
    uint8 *p, buf[20];
    struct MSG_REV_TYPE Rev;
    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Rev.Type = DEV_XCOM_TABLE;
    if ((gDeviceTable.Class.Flag & CLASS_TASK_FLAG_RUN) != CLASS_TASK_FLAG_RUN)
        return 0;
    p = (BYTE *)Get_ClassInfo_Table(gDeviceTable.Class.Curr_id);
    p += sizeof(struct CLASS_OPEN_INFO) + gDeviceTable.Class.ClassCurrNUm * sizeof(struct CLASS_OPTION_REC);
    memcpy((BYTE *)&classmsg, p, sizeof(struct CLASS_OPTION_REC));

    if (gDeviceTable.Class.ClassCurrNUm < gDeviceTable.Class.ClassTaskNum)
    {
        if ((classmsg.MsgRec.device >= 16) && (classmsg.MsgRec.device <= 100))
        {
            buf[0] = classmsg.MsgRec.device;
            buf[1] = classmsg.MsgRec.option;
            if (classmsg.MsgRec.option == 0) classmsg.MsgRec.option = 1;
            buf[2] = classmsg.OptionTime & 0xff;
            //Task_Run_Delay(REALTIME_CIIR_SEC*256+buf[2], buf,2, 1,rev_buf);
            Msg_send(buf, 3, (uint8 *)&Rev);

            if((buf[0] == MSG_DEV_TYJA) && (buf[1] == MSG_OPTION_POWEROPEN)) //2014.9.4 投影机A开机吗延迟重复发送
                Task_Run_Delay(REALTIME_CIIR_SEC * 256 + buf[2], buf, 2, 1, (uint8 *)&Rev);

            LED_Contrl(LED_CONTRL_TIME_OFF, gDeviceTable.Class.TIME_LED_STATE);
            if (gDeviceTable.Class.TIME_LED_STATE == FALSE)
                gDeviceTable.Class.TIME_LED_STATE = TRUE;
            else
                gDeviceTable.Class.TIME_LED_STATE = FALSE;
        }
        gDeviceTable.Class.ClassCurrNUm++;

    }
    else
    {
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_POWERCARD_ONOFF;
        if (Task_Rec_Find(buf, 2) != 0xff)
        {
            buf[0] = MSG_DEV_MAINBARD;
            buf[1] = MSG_OPTION_MSG_RUN;
            if (Task_Rec_Find(buf, 2) == 0xff)
                Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 1, buf, 2, 1, (uint8 *)&Rev);
            LED_Contrl(LED_CONTRL_TIME_OFF, gDeviceTable.Class.TIME_LED_STATE);
            if (gDeviceTable.Class.TIME_LED_STATE == FALSE)
                gDeviceTable.Class.TIME_LED_STATE = TRUE;
            else
                gDeviceTable.Class.TIME_LED_STATE = FALSE;
            return 0;
        }

        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_MSG_RUN;
        Task_Rec_Del(buf, 2);
        if (gDeviceTable.Class.Curr_id == 1) //开机
        {
            gDeviceTable.Class.Flag &= ~CLASS_TASK_FLAG_OVER;
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("开机序列执行完");
            LED_Contrl(LED_CONTRL_TIME_OFF, FALSE);
            buf[0] = MSG_DEV_PC;
            buf[1] = MSG_OPTION_NONE;
            buf[2] = 0;
            Msg_send(buf, 2, (uint8 *)&Rev);
        }
        else if (gDeviceTable.Class.Curr_id == 2)
        {
            gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_OVER;
            LED_Set(0, LED_ALL_CLR);
            LED_SendFLASH();
            LED_Contrl(LED_CONTRL_TIME_OFF, TRUE);
            Key_TB_state(KEY_REP_CLASS, TRUE);
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("关机序列执行完");
        }
        gDeviceTable.Class.Flag &= ~CLASS_TASK_FLAG_RUN;
    }
    return 1;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_POWER(void *pdata)
{
    uint8 port, option, *p;
    p = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE);
    port = *(p + sizeof(struct BASE_MSG));
    option = *(p + sizeof(struct BASE_MSG) + 1);
    POWER_Set(port,  option);

    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_CLASS_BEGIN(void *pdata)
{
    uint8 flag, buf[10], *ip; //,tmp;
    struct DEVICE_STATUS_TYPE *ps;
    struct MSG_REV_TYPE *prev;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    prev = (struct MSG_REV_TYPE *)pdata;
    IP_printf("执行开机序列1");
    if ((gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER) == 0)
        return 0;
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_RUN)
        return 0;

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    gDeviceTable.Class.Flag &= ~CLASS_TASK_FLAG_OVER;
    gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_RUN;

    //if (Get_Debug(DEV_DEBUG_SYSTEM))


    LED_Contrl(LED_CONTRL_TIME_OFF, FALSE);
    LED_Contrl(LED_CONTRL_POWER_ON, TRUE);
    gDeviceTable.Class.TIME_LED_STATE = FALSE;
    flag = Class_Task_Start(1);
    key_init();//执行完开机序列
    if (flag == CLASS_TASK_OK)
        IP_printf("执行开机序列");
    //修改状态表

    switch(prev->Type)
    {
    case DEV_XCOM_MB://消息来自面板
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_KEY;

        break;
    case DEV_XCOM_RF://上课消息来自邋RF卡
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_RF;

        break;
    case DEV_XCOM_TIME:	//上课消息来自定时课表
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_TIME;
        break;
    case DEV_XCOM_UDP://来自后台的命令
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_UDPCMD;
        break;
    case DEV_XCOM_FINGERPRINT://指纹识别机的命令
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_FINGERPRINT;
        break;

    };
    gDeviceStatus.CLASS_state |= 1;
    ViscaCount = 0;
    //通知流媒体
    Key_TB_state(KEY_REP_CLASS, FALSE);
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_CLASS_BEGIN;
    if (ps->DEV_Link_State & DEV_LINK_BJCardLK)
    {
        ip = Get_DesIp(DEVICE_SUB_BJCard);
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
            goto nclass;
            //return 1;
        }
        else
            UDP_Send_CMD(buf, 2, ip);
        buf[0] = MSG_DEV_PC;
        buf[1] = MSG_OPTION_NONE;
        UDP_Send_CMD(buf, 2, ip);
    }
nclass:
    //通知集控
    ip = Get_DesIp(DEVICE_SUB_SER);
    if (ARP_Rec_Find(ip) == NULL)
    {
        Arp_Request(ip);
        return 1;
    }
    else
        UDP_Send_CMD(buf, 2, ip);
    return 1;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_CLASS_OVER(void *pdata)
{
    uint8 flag, buf[10] , *ip;
    struct DEVICE_STATUS_TYPE *ps;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    //prev=(struct MSG_REV_TYPE * )pdata;

    if ((gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER) == CLASS_TASK_FLAG_OVER)
        return 0;
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_RUN)
        return 0;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("执行关机序列");
    DWQ_Contrl(0);//打开超声波定位器
    gDeviceTable.Class.TIME_LED_TYPE = 1;
    LED_Contrl(LED_CONTRL_TIME_OFF, TRUE);
    gDeviceTable.Class.TIME_LED_STATE = TRUE;

    gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_OVER;
    gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_RUN;
    flag = Class_Task_Start(2);
    switch(flag)
    {
    case CLASS_TASK_OK:
        break;
    case CLASS_TASK_ERR_ID:
        break;
    case CLASS_TASK_EMPTY:
        LED_Set(0, LED_ALL_CLR);
        gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_OVER;
        //关闭统计
        Device_Stat_Save();
        //关闭键盘显示
        LED_Set(0, LED_ALL_CLR);
        LED_SendFLASH();
        LED_Contrl(LED_CONTRL_TIME_OFF, TRUE);
        Key_TB_state(KEY_REP_CLASS, TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("关机序列执行完毕");
        DWQ_Contrl(0);//打开超声波定位器
        break;
    }
    //修改状态表
    gDeviceStatus.DEV_state &= 0xf0;
    gDeviceStatus.CLASS_state &= 0xfe;
    //关闭面板电源
    LED_Contrl(LED_CONTRL_POWER_ON, FALSE);
    //关闭定位器
    gYXJ.type = 1;
    //if (ps->DEV_Link_State&DEV_LINK_BJCardLK)
    // DWQ_Contrl(0);//打开超声波定位器
    //通知刘媒体
    ViscaCount = 0;
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_CLASS_OVER;
    if (ps->DEV_Link_State & DEV_LINK_BJCardLK)
    {
        ip = Get_DesIp(DEVICE_SUB_BJCard);
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
            goto pclass;
            //return 1;
        }
        else
            UDP_Send_CMD(buf, 2, ip);
    }
pclass:
    //通知集控
    ip = Get_DesIp(DEVICE_SUB_SER);
    if (ARP_Rec_Find(ip) == NULL)
    {
        Arp_Request(ip);
        return 1;
    }
    else
        UDP_Send_CMD(buf, 2, ip);

    return 1;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_RF_ACTIVE(void *pdata)
{
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HELP_ON(void *pdata)
{

    uint8 buf[20];
    uint16 tmp;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    memset(buf, 0, 20);
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_HELP_LINK;
    memcpy(buf + 2, Get_DesIp(DEVICE_SUB_CLASS), 4);
    UDP_Send_CMD(buf, 20, Get_DesIp(DEVICE_SUB_SER));
    ///调试
    buf[0] = 192;
    buf[1] = 168;
    buf[2] = 0;
    buf[3] = 230;
    //Sound_Open(buf,AUDIO_PORT,0,SOUND_CONTRL_SEND);
    tmp = Device_Get_MIX();
    tmp |= 1 << 12;
    Device_Set_MIX(tmp);

    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HELP_OFF(void *pdata)
{
    uint8 buf[20];

#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    memset(buf, 0, 20);
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_HELP_BREAK;
    memcpy(buf + 2, Get_DesIp(DEVICE_SUB_CLASS), 4);
    UDP_Send_CMD(buf, 20, Get_DesIp(DEVICE_SUB_SER));
    Sound_Close(NULL, SOUND_OPTION_ALL);
    Key_TB_state(KEY_REP_HELP, FALSE);
    LED_Set(LED_HELP, LED_DISPLAY_OFF);
    LED_SendFLASH();
    return 0;
}
/////////////////////////////////////////////////////////////////////
//  函数名称：
//  函数功能：
//  入口参数：
//
//
//  出口参数：无
////////////////////////////////////////////////////////////////////
uint32  On_MSG_OPTION_IP_DPlay(void *pdata)
{
    uint8 *des_ip;
    uint16 tmp;
    //	 struct MSG_REV_TYPE *prev;
    des_ip = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    Sound_Close(NULL, SOUND_OPTION_ALL);
    Sound_Open(des_ip, AUDIO_PORT, 0, SOUND_CONTRL_ECHO | SOUND_CONTRL_SEND | SOUND_IP_ASK);
    IP_printf("收到回音消除对讲模式指令");
    Key_TB_state(KEY_REP_HELP, TRUE);
    LED_Set(LED_HELP, LED_DISPLAY_ON);
    LED_SendFLASH();
    tmp = Device_Get_MIX();
    tmp &= ~(3 << 12);
    tmp |= 1 << 12;
    tmp &= ~SMIX_LAN_OUT_SELEC;
    Device_Set_MIX(tmp);

    return 0;


}
/////////////////////////////////////////////////////////////////////
//  函数名称：
//  函数功能：
//  入口参数：
//
//
//  出口参数：无
////////////////////////////////////////////////////////////////////

uint32 on_MSG_OPTION_IP_MICVol(void *pdata)
{
    uint8 *micvol;
    micvol = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    TMS320AIC23_Set_Vol(TMS320AIC23_MIC_VOL, *micvol);
    IP_printf("设置mic音量(0-100)%d", *micvol);
    return 0;


}
/////////////////////////////////////////////////////////////////////
//  函数名称：
//  函数功能：
//  入口参数：
//
//
//  出口参数：无
////////////////////////////////////////////////////////////////////
uint32 on_MSG_OPTION_IP_HPVol(void *pdata)
{
    uint8 *micvol;
    micvol = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    TMS320AIC23_Set_Vol(TMS320AIC23_HP_VOL, *micvol);
    IP_printf("设置mic音量(0-100)%d", *micvol);
    return 0;


}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HELP_TEST(void *pdata)
{
    uint8 *des_ip;

#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    des_ip = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    Sound_Open(des_ip, AUDIO_PORT, 0, SOUND_CONTRL_ECHO | SOUND_CONTRL_SEND | SOUND_IP_hASK);
    /*Key_TB_state(KEY_REP_HELP,TRUE);
    LED_Set(LED_HELP,LED_DISPLAY_ON);
    LED_SendFLASH();
    tmp=Device_Get_MIX();
    tmp|=1<<12;
    Device_Set_MIX(tmp);*/
    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////

uint32 On_MSG_OPTION_IPPlay(void *pdata)
{


    uint8 *des_ip, cmd;
    uint32 tmp;

#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    cmd = *((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
    if (cmd == 1)
    {
        des_ip = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + 1;
        Sound_Close( des_ip, SOUND_OPTION_IP );
        Sound_Open(des_ip, AUDIO_PORT, 0, SOUND_CONTRL_SEND | SOUND_IP_mASK);
        tmp = Device_Get_MIX();
        tmp &= ~(3 << 12);
        tmp |= 3 << 12;
        tmp |= SMIX_LAN_OUT_SELEC;
        Device_Set_MIX(tmp);
        IP_printf("收到语音教学系统打开通道命令");
    }
    else
    {
        Sound_Close(NULL, SOUND_OPTION_ALL);
        IP_printf("收到语音教学系统关闭通道命令");
    }

    return 0;

}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HELP_LINK(void *pdata)
{
    uint8 *des_ip;
    uint32 tmp;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    des_ip = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    Sound_Close( des_ip, SOUND_OPTION_ALL);
    Sound_Open(des_ip, AUDIO_PORT, 0, SOUND_CONTRL_SEND);
    Key_TB_state(KEY_REP_HELP, TRUE);
    LED_Set(LED_HELP, LED_DISPLAY_ON);
    LED_SendFLASH();
    tmp = Device_Get_MIX();
    tmp &= ~(3 << 12);
    tmp |= 1 << 12;
    tmp &= ~SMIX_LAN_OUT_SELEC;

    Device_Set_MIX(tmp);

    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HELP_BREAK(void *pdata)
{
    uint8 buf[20];
    memset(buf, 0, 20);
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_HELP_BREAK;
    memcpy(buf, Get_DesIp(DEVICE_SUB_CLASS), 4);
    UDP_Send_CMD(buf, 20, Get_DesIp(DEVICE_SUB_SER));
    Sound_Close(NULL, SOUND_OPTION_ALL);
    Key_TB_state(KEY_REP_HELP, FALSE);
    LED_Set(LED_HELP, LED_DISPLAY_OFF);
    LED_SendFLASH();

    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_AUTO_ON(void *pdata)
{
    //zjj add
    if (Get_Class_Stauts() == CLASS_OVER) return 0;
    gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONAT;

    LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_ZD, LED_DISPLAY_ON);
    LED_SendFLASH();
    KEY_DATA.MODE = MSG_OPTION_AUTO_ON;
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_AUTO_OFF(void *pdata)
{
    if (Get_Class_Stauts() == CLASS_OVER) return 0;
    gDeviceStatus.DEV_state &= 0xf ;
    gDeviceStatus.DEV_state |= DEV_STATE_OPTIONSD;
    LED_Set(LED_MODE_SD, LED_DISPLAY_ON);
    LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    LED_SendFLASH();
    KEY_DATA.MODE = MSG_OPTION_AUTO_OFF;
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_DEV_REMOTE_ON(void *pdata)
{
    BYTE bState = gDeviceStatus.DEV_state & 0xf0 ;
    bState = bState >> 4;
    if ( bState  &  8 )
    {
        gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONJG;//接管设备
    }
    else
    {
        gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONJGDEV;//接管设备
    }
    if (Get_Class_Stauts() == CLASS_OVER) return  STATUS_CHANGE;
    LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_YC, LED_DISPLAY_ON);
    LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_REC_REMOTE_ON(void *pdata)
{
    BYTE bState = gDeviceStatus.DEV_state & 0xf0 ;
    bState = bState >> 4;
    if ( bState  &  4 )
    {
        gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONJG;//接管录制
    }
    else
    {
        gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONJGREC;//接管录制
    }
    if (Get_Class_Stauts() == CLASS_OVER) return  STATUS_CHANGE;
    LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_YC, LED_DISPLAY_ON);
    LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_DEV_REMOTE_OFF(void *pdata)
{
    gDeviceStatus.DEV_state &= (~(DEV_STATE_OPTIONJGDEV)) ;
    if (Get_Class_Stauts() == CLASS_OVER) return  STATUS_CHANGE;
    if (KEY_DATA.MODE == MSG_OPTION_AUTO_ON)
    {
        LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_ZD, LED_DISPLAY_ON);
    }
    else
    {

        LED_Set(LED_MODE_SD, LED_DISPLAY_ON);
        LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_REC_REMOTE_OFF(void *pdata)
{
    gDeviceStatus.DEV_state &= (~(DEV_STATE_OPTIONJGREC)) ;
    if (Get_Class_Stauts() == CLASS_OVER) return  STATUS_CHANGE;
    if (KEY_DATA.MODE == MSG_OPTION_AUTO_ON)
    {
        LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_ZD, LED_DISPLAY_ON);
    }
    else
    {

        LED_Set(LED_MODE_SD, LED_DISPLAY_ON);
        LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_AUTOTRACK_ON(void *pdata)
{

    gDeviceStatus.CLASS_state |= (1 << 1);
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_AUTOTRACK_OFF(void *pdata)
{
    gDeviceStatus.CLASS_state &= (~(1 << 1));
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_STRATEGY_ON(void *pdata)
{
    gDeviceStatus.CLASS_state |= (1 << 2);
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_STRATEGY_OFF(void *pdata)
{
    gDeviceStatus.CLASS_state &= (~(1 << 2));
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HD_ON(void *pdata)
{
    gDeviceStatus.CLASS_state |= (1 << 3);
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HD_OFF(void *pdata)
{
    gDeviceStatus.CLASS_state &= (~(1 << 3));
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HD_VAL(void *pdata)
{
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SCAMERA_CONTRL(void *pdata)
{
    uint8 LMT_Cmd, *p, buf[16], ID, tmp;
    uint16 hSpeed, vSpeed;
    uint8 dwPos = 0;
    uint32 bps;
    uint8 portNum;
    struct DEVICE_INFO *pdev;
    struct COMM_XCOM  *pxcom;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    p = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    LMT_Cmd = *p;
    ID = *(p + 1);
    hSpeed = *(p + 2);
    vSpeed = *(p + 3);
    dwPos = *(p + 3);
    pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(ID);
    if (pdev->dev_id != ID)
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("摄像机控制错误的设备号:%d", ID);
        return 0;
    }
    if ((pdev->dev_type != MSG_DEV_TQCAMERA) &&
            (pdev->dev_type != MSG_DEV_SQCAMERA) &&
            (pdev->dev_type != MSG_DEV_BQCAMERA) &&
            (pdev->dev_type != MSG_DEV_SCAMERA1) &&
            (pdev->dev_type != MSG_DEV_SCAMERA2) &&
            (pdev->dev_type != MSG_DEV_SCAMERA3))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("摄像机控制错误的设备类型:%d", pdev->dev_type);
        return 0;

    }
    if(pdev->COMM.val.xcom.PortNum == 6)
    {
        portNum = DEV_XCOM_TB;
    }
    else
    {
        portNum = DEV_XCOM_SCAMERA;
    }
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("摄像机控制串口号:%d", portNum);

    buf[0] = 0xff;
    buf[1] = pdev->COMM.val.xcom.address;
    SetAddress(pdev->COMM.val.xcom.address, pdev->COMM.val.xcom.PROTOCOL);
    pxcom = Get_XCOM_Port(portNum);
    bps = pdev->COMM.val.xcom.BPS + pdev->COMM.val.xcom.Hbps * 0x10000;
    if (Get_Debug(DEV_DEBUG_MSG))
    {
        IP_printf("摄像机控制设备号:%d", ID);
        IP_printf("控制摄像机BPS:%d", bps);
        IP_printf("控制摄像机端口:%d地址:%d", pdev->COMM.val.xcom.PortNum, pdev->COMM.val.xcom.address);
    }

    if ((bps == 2400 ) || (bps == 4800) || (bps == 9600) || (bps == 19200) || (bps == 38400))
        if (bps != pxcom->BPS)
            Xcom_set(portNum, bps, 0, pdev->COMM.val.xcom.linecontrl);
    if(ViscaCount == 0)
    {
        buf[0] = 0x88;
        buf[1] = 0x30;
        buf[2] = 0x01;
        buf[3] = 0xff;
        UART_Write(portNum, buf, 4);
        ViscaCount++;
    }
    switch(LMT_Cmd)
    {
    case 1://up控制球机转动(上)
        TurnUpCmd( buf, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机转动(上)", LMT_Cmd);
        break;
    case 2://控制球机转动(下)
        TurnDownCmd( buf, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机转动(下)", LMT_Cmd);
        break;
    case 3://控制球机转动(左)
        TurnLeftCmd( buf, hSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机转动(左)", LMT_Cmd);
        break;
    case 4://控制球机转动(右)
        TurnRightCmd( buf, hSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机转动(右)", LMT_Cmd);
        break;
    case 5://控制球机转动(左上)
        TurnLeftUpCmd( buf, hSpeed, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机转动(左上)", LMT_Cmd);
        break;
    case 6://控制球机转动(左下)
        TurnLeftDownCmd( buf, hSpeed, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机转动(左下)", LMT_Cmd);
        break;
    case 7://控制球机转动(右上)
        TurnRightUpCmd( buf, hSpeed, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机转动(右上)", LMT_Cmd);
        break;
    case 8:	//控制球机转动(右下)
        TurnRightDownCmd( buf, hSpeed, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机转动(右下)", LMT_Cmd);
        break;
    case 9://停止转动
        StopCmd(portNum, buf , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机停止转动", LMT_Cmd);
        if(pdev->COMM.val.xcom.PROTOCOL == DEV_PROTOCOL_VISCA)
            return 0;
        break;
    case 10://球机Zoom +
        ZoomInCmd( buf, hSpeed, pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机Zoom +", LMT_Cmd);
        break;
    case 11://球机Zoom -
        ZoomOutCmd( buf, hSpeed, pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机Zoom -", LMT_Cmd);
        break;
    case 12://球机Focus +
        FocusNearCmd(  buf , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机Focus +", LMT_Cmd);
        break;
    case 13://球机Focus -
        FocusFarCmd(  buf  , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机Focus -", LMT_Cmd);
        break;
    case 14://调用球机预置位
        GotoPrepositionCmd( buf, (BYTE)dwPos , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d控制球机调用球机预置位:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 15://查询球机当前角度
        buf[2] = 0x00;
        buf[3] = 0x51;
        buf[4] = 0;
        buf[5] = 0;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d查询球机当前角度", LMT_Cmd);
        break;
    //16
    case 17://设置球机预置位
        SetPrepositionCmd(  buf, (BYTE)dwPos , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d设置球机预置位:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 18://删除球机预置位
        buf[2] = 0x00;
        buf[3] = 0x05;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d删除球机预置位:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 19://水平角旋转绝对角度
        buf[2] = 0x00;
        buf[3] = 0x4b;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d水平角旋转绝对角度:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 20://	垂直角旋转绝对角度
        buf[2] = 0x00;
        buf[3] = 0x4d;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d垂直角旋转绝对角度:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 21://获取球机的三维坐标
        if(DEV_PROTOCOL_PD == pdev->COMM.val.xcom.PROTOCOL )
        {
            buf[0] = 0x55;
            buf[1] = 0x01;
            buf[2] = pdev->COMM.val.xcom.address;
            for (tmp = 3; tmp < 10; tmp++)
                buf[tmp] = 0;
            buf[10] = 0;
            for (tmp = 1; tmp < 10; tmp++)
                buf[10] += buf[tmp];
            UART_Write(portNum, buf, 11);
        }
        else if(DEV_PROTOCOL_VISCA == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[0] = 0x80 + pdev->COMM.val.xcom.address;
            buf[1] = 0x09;
            buf[2] = 0x06;
            buf[3] = 0x12;
            buf[4] = 0xff;
            UART_Write(portNum, buf, 5);
            OSTimeDly(10);
            buf[1] = 0x09;
            buf[2] = 0x04;
            buf[3] = 0x47;
            buf[4] = 0xff;
            UART_Write(portNum, buf, 5);
        }

        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d获取球机的三维坐标", LMT_Cmd);
        return  0;
    // case 22://
    case 23://设置球机的三维坐标
        if(DEV_PROTOCOL_PD == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[0] = 0x55;
            buf[1] = 0xfc;
            buf[2] = pdev->COMM.val.xcom.address;
            for (tmp = 3; tmp < 10; tmp++)
                buf[tmp] = *(p + tmp - 1);
            buf[10] = 0;
            for (tmp = 1; tmp < 10; tmp++)
                buf[10] += buf[tmp];
            UART_Write(portNum, buf, 11);
        }
        else if(DEV_PROTOCOL_VISCA == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[0] = 0x80 + pdev->COMM.val.xcom.address;
            buf[1] = 0x01;
            buf[2] = 0x06;
            buf[3] = 0x02;
            buf[4] = hSpeed;
            buf[5] = vSpeed;
            for(tmp = 6; tmp < 14; tmp++)
                buf[tmp] = *(p + tmp - 2);
            buf[14] = 0xff;
            UART_Write(portNum, buf, 15);
            buf[1] = 0x01;
            buf[2] = 0x04;
            buf[3] = 0x47;
            for(tmp = 4; tmp < 8; tmp++)
                buf[tmp] = *(p + tmp + 8);
            buf[8] = 0xff;
            UART_Write(portNum, buf, 9);
        }
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d设置球机的三维坐标", LMT_Cmd);
        return  0;
    case 24://查询球机当前垂直角度
        buf[2] = 0x00;
        buf[3] = 0x53;
        buf[4] = 0;
        buf[5] = 0;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d查询球机当前垂直角度", LMT_Cmd);
        break;
    case 25://设置zoom值
        if(DEV_PROTOCOL_PD == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[2] = 0x00;
            buf[3] = 0x4f;
            buf[4] = hSpeed;
            buf[5] = vSpeed;
        }
        else if(DEV_PROTOCOL_VISCA == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[1] = 0x01;
            buf[2] = 0x04;
            buf[3] = 0x47;
            buf[8] = 0xff;
            //UART_Write(portNum,buf,9);
            return 0;
        }
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d设置zoom值:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 26://设置zoom速度
        buf[2] = *(p + 4);
        buf[3] = 0x79;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d设置zoom速度:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 27://设置水平速度
        buf[2] = 0x00;
        buf[3] = 0x57;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d设置水平速度%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    default:
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("控制摄像机命令:%d未定义的球机控制命令", LMT_Cmd);
        return 0;
    }
    if(DEV_PROTOCOL_PD == pdev->COMM.val.xcom.PROTOCOL )
    {
        buf[6] = 0;
        for (tmp = 1; tmp < 6; tmp++)
            buf[6] += buf[tmp];
        UART_Write(portNum, buf, 7);
    }
    else if(DEV_PROTOCOL_VISCA == pdev->COMM.val.xcom.PROTOCOL)
    {
        UART_Write(portNum, buf, 9);
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SCAMERA_VAL(void *pdata)
{
    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_DEBUG(void *pdata)
{
    struct BASE_MSG *pmsg;
    NetP *pnet;
    uint8 tmp;
    pnet = (NetP *)Get_NetPort();
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));

    if (pmsg->device == MSG_DEV_MAINBARD)
    {
        gDeviceTable.Debug = *((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
        if (Get_Debug(DEV_DEBUG_SYSTEM))
        {
            gDeviceTable.Debug &= ~DEV_DEBUG_SYSTEM;
            tmp = *((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + 1);
            Config_printf(tmp);
        }
        pnet->Test_TTL = 9;
    }
    IP_printf("调试状态:%2x", gDeviceTable.Debug);
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_DEV_RESET(void *pdata)
{
    Device_KZ_Reset();
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能： MSG_OPTION_FILE_REQ://			0xb2//文件请求
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_FILE_REQ(void *pdata)
{
    struct SFILE_TRANSLATE_TYPE *pfile;
    struct MSG_REV_TYPE *prev;

    prev = (struct MSG_REV_TYPE *)pdata;
    pfile = (struct SFILE_TRANSLATE_TYPE *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
    switch(pfile->FileType)
    {
    case FILE_TYPE_TABLE:
        File_Table_Read(pfile->FilePage, prev->des_ip);
        break;
    case FILE_TYPE_CLASS_TABLE:
        File_ClassTable_Read(pfile->FilePage, prev->des_ip);
        break;
    case FILE_TYPE_USER_PRORAM:
        break;
    case FILE_TYPE_KB_PROGRAM:
        break;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能:MSG_OPTION_FILE_SEND://			0xb3//文件传送
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_FILE_SEND(void *pdata)
{
    struct SFILE_TRANSLATE_TYPE *pfile;
    struct MSG_REV_TYPE *prev;

    prev = (struct MSG_REV_TYPE *)pdata;
    pfile = (struct SFILE_TRANSLATE_TYPE *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
    switch(pfile->FileType)
    {
    case FILE_TYPE_TABLE:
        File_Table_Write(pfile->FilePage, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), prev->des_ip);
        break;
    case FILE_TYPE_USER_PRORAM:
        File_UserProgram_Write(pfile->FilePage, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), prev->des_ip);
        break;
    case FILE_TYPE_KB_PROGRAM:
        break;
    case FILE_TYPE_CLASS_TABLE:
        File_ClassTable_Write(pfile->FilePage, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), prev->des_ip);
        break;

    }
    return 0;
}

uint32 On_MSG_OPTION_SERVER_FILE_SEND(void *pdata)
{
    struct SFILE_TRANSLATE_TYPE *pfile;
    struct MSG_REV_TYPE *prev;
    uint8 *p, *pBuf;
    uint32 resault = 0xffff;

#if OS_CRITICAL_METHOD == 3              /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;                   /* Prevent compiler warning                      */
#endif
    prev = (struct MSG_REV_TYPE *)pdata;
    pfile = (struct SFILE_TRANSLATE_TYPE *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
    switch(pfile->FileType)
    {
    case FILE_TYPE_TABLE_BASEINFO:
        p = (uint8 *)User_mem_allco(500);
        if (NULL != p)
        {
            memcpy( p, (uint8 *)Get_ClassInfo_Table(0), 256 );
            pBuf = ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE );
            /*memcpy( p+2,pBuf+2,32 );
            memcpy( p+ 2+32,pBuf+2+32,32 );
            memcpy( p + 2 + 32 +32,pBuf+2+32+32,32 );
            memcpy( p + 132,pBuf+132,4 );*/
            memset( p + 2, '3', 20 );
            memset( p + 2 + 32, '5', 20 );
            OS_ENTER_CRITICAL();
            InitIAP();
            // resault=SelSector(ARM_TABLE_SECTOR_START,ARM_TABLE_SECTOR_END);
            // resault=EraseSector(ARM_TABLE_SECTOR_START,ARM_TABLE_SECTOR_END);
            // resault=BlankCHK(ARM_TABLE_SECTOR_START,ARM_TABLE_SECTOR_END);
            resault = SelSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
            resault = RamToFlash(ARM_TABLE_ADDRESS, (uint32)p, 256);
            OS_EXIT_CRITICAL();

            pBuf = (uint8 *) Get_ClassInfo_Table(0);
            memcpy( p + 6, p, 256 );
            *p = MSG_DEV_MAINBARD;
            *(p + 1) = MSG_OPTION_SERVERWRITE_ACK;
            memcpy( p + 2, (uint8 *)pfile, 4 );
            UDP_Send_CMD(p, 256 + sizeof(struct SFILE_TRANSLATE_TYPE ) + 2, prev->des_ip);
            //File_Table_Write(0,( BYTE *)p,prev->des_ip);
            User_mem_free(p);
        }
        break;
    case FILE_TYPE_CLASS_TABLE_ONEDAY:
        File_ClassTable_Write_OneDay(pfile->FilePage, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), prev->des_ip);
        p = (uint8 *)User_mem_allco(500);
        if (p != NULL)
        {
            memcpy( p + 6, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), 256 );
            *p = MSG_DEV_MAINBARD;
            *(p + 1) = MSG_OPTION_SERVERWRITE_ACK;
            memcpy( p + 2, (uint8 *)pfile, 4 );
            UDP_Send_CMD(p, 256 + sizeof(struct SFILE_TRANSLATE_TYPE ) + 2, prev->des_ip);
            User_mem_free(p);
        }
        break;
    }
    resault = resault;
    pBuf = pBuf;
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：服务器返回On_MSG_OPTION_SVRLINK_REQ()命令
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////

uint32 On_MSG_OPTION_SEVER_ACK(void *pdata)
{
    struct STIME_LINK_TYPE *pstime;
    struct REALTIME_TYPE realtime;

    pstime = (struct STIME_LINK_TYPE *)((BYTE *)pdata + sizeof(struct BASE_MSG) + sizeof( struct MSG_REV_TYPE));
    realtime.year = pstime->yearH + pstime->yearL * 256;
    realtime.mon = pstime->mon;
    realtime.day = pstime->day;
    realtime.week = pstime->week;
    realtime.hour = pstime->hour;
    realtime.min = pstime->min;
    realtime.sec = pstime->sec;
    RealTime_Set((BYTE *)&realtime);
    LED_Time_Flash();
    if ((realtime.week - 1) != gClassT.week)
        File_ClassTable_Read_CurDay();
    gDeviceTable.flag_sver |= DEV_FLAG_SVER_LINK;

    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void  On_MSG_OPTION_STATUS_CHANGE(BYTE *pdata)
{
    BYTE buf[80], tmp, *ip;
    struct MSG_REV_TYPE Rev;
    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Rev.Type = DEV_XCOM_SELF;
    pdata = pdata;
    memset(buf, 0, 16);
    ip = Get_DesIp(DEVICE_SUB_SER);
    if (ARP_Rec_Find(ip) == NULL)
    {
        Arp_Request(ip);
    }
    else if (IP_Compare(ip, buf) == FALSE)
    {
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_STATUS_CHANGE;
        memcpy(buf + sizeof(struct BASE_MSG), (BYTE *)Get_Device_Status(), sizeof(struct DEVICE_STATUS_TYPE));
        UDP_Send_CMD(buf, sizeof(struct BASE_MSG) + sizeof(struct DEVICE_STATUS_TYPE), ip);
    }

    if (gDeviceStatus.DEV_Link_State & DEV_LINK_PCON)
    {
        memset(buf, 0, 16);
        ip = Get_DesIp(DEVICE_SUB_PC);
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
        }
        else if (IP_Compare(ip, buf) == FALSE)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_PCLK;
            buf[0] = MSG_DEV_MAINBARD;
            buf[1] = MSG_OPTION_DEV_LINK;
            UDP_Send_CMD(buf, sizeof(struct BASE_MSG), ip);
        }
    }
    if ((gDeviceTable.flag_sver & DEV_FLAG_SVER_LINK) == 0)
        tmp = 10;
    else
        tmp = 180;
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_STATUS_CHANGE;
    if (Task_Rec_Find(buf, 2) == 0xff)
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + tmp, buf, 2, 0, (uint8 *)&Rev);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：void On_MSG_OPTION_SEVER_LINK(void *pdata)
//	函数功能：服务器连接0xb1
//	入口参数：pdata=消息+STIME_LINK_TYPE数据
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SVRLINK_REQ(void *pdata)
{
    BYTE buf[80];
    NetP *pnet;
    struct MSG_REV_TYPE *prev;
    prev = (struct MSG_REV_TYPE *)pdata;
    pnet = (NetP *)Get_NetPort();

    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_SVRLINK_REQ;
    memcpy(buf + sizeof(struct BASE_MSG), (BYTE *)Get_Device_Status(), sizeof(struct DEVICE_STATUS_TYPE));
    UDP_Send_CMD(buf, sizeof(struct BASE_MSG) + sizeof(struct DEVICE_STATUS_TYPE), prev->des_ip);
    pnet = pnet;
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			MSG_OPTION_HW_SEND
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HW_SEND(BYTE *pdata)
{
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			MSG_OPTION_TIMEACTIVE
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_TIMEACTIVE(BYTE *pdata)
{
    uint8 tmp, tciir, rev_buf[10];
    struct REALTIME_TYPE *ptime;
    struct TASK_REC_TYPE *ptask;
#if OS_CRITICAL_METHOD == 3                /* Allocate storage for CPU status register        */
    OS_CPU_SR  cpu_sr;                    /* Prevent compiler warning                        */
#endif
    tciir = *(pdata + sizeof(struct BASE_MSG) + sizeof(struct MSG_REV_TYPE));
    rev_buf[0] = DEV_XCOM_SELF;
    //更新课程表
    if (tciir & REALTIME_CIIR_WEEK)
    {
        ptime = RealTime_Get();
        if ((ptime->week != gClassT.week) && (ptime->week < 7))
        {
            gClassT.week = ptime->week;
            File_ClassTable_Read_CurDay();
        }
    }

    if  (tciir & REALTIME_CIIR_MIN)
    {
        //if (DEV_STATE_OPTIONSD != gDeviceStatus.DEV_state & 0xf) //设备配置成手动
        //{
        // if (Check_FunctionMode(Device_FunctionMode_CARD)==0) //设备没有设置RF开机
        Class_Table_Check();
        //}
    }

    if (TASK_TABLE.RecCount == 0)
        return  0;
    OS_ENTER_CRITICAL();
    for (tmp = 0; tmp < TASK_DELAY_MAX_SIZE; tmp++)
    {
        ptask = (struct TASK_REC_TYPE *)&TASK_TABLE.DelayRec[tmp];
        if ((ptask->TimeType & tciir) && (ptask->pmsg != NULL))
        {
            if (ptask->DelayTime != 0)
            {
                ptask->DelayTime--;
                if (ptask->DelayTime == 0)
                    ptask->TimeType = REALTIME_CIIR_SEC;
            }
            else
            {
                memcpy(rev_buf, ptask->pmsg, sizeof(struct MSG_REV_TYPE));
                Msg_send((uint8 *)ptask->pmsg + sizeof(struct MSG_REV_TYPE), ptask->msg_lg, rev_buf);
                if (ptask->repeat == 0)
                {
                    User_mem_free(ptask->pmsg);
                    if (TASK_TABLE.RecCount != 0)
                        TASK_TABLE.RecCount--;
                    ptask->DelayTime = 0;
                    ptask->pmsg = NULL;
                    ptask->TimeType = 0;
                }
                else
                {
                    ptask->repeat--;
                    ptask->DelayTime = ptask->bak_time;
                }
            }
        }
    }
    OS_EXIT_CRITICAL();
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_RepeatPower_OPTION(void *pdata)
{
    uint8 *p, id, *pbuf, buf[5] = {0};
    struct DEVICE_INFO *pdev;
    struct MESSAGE_TYPE *poption;

    pbuf = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE);
    //zjj add
    if (MSG_OPTION_REPEATPOWERCLOSE == pbuf[1])
    {
        pbuf[1] = pbuf[2];
    }
    //zjj add
    pdev = (struct DEVICE_INFO *)Device_Rec_Find(pbuf, DEV_FIND_TYPE);
    if (pdev == NULL)
        return FALSE;//没有该设备登记

    id = Msg_Option_Find(pbuf);
    if (id == 0)
        return FALSE; //没有该操作码记录
    poption = (struct MESSAGE_TYPE *)Get_ClassInfo_Table(id);
    p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
    switch (pdev->COMM.comm_type)
    {
    case DEVICE_COMMTYPE_NONE:
        break;
    case DEVICE_COMMTYPE_HW:
        HW_send(poption->dev_id);
        break;
    case DEVICE_COMMTYPE_XCOM:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        //		UART_Send_Data(pdev->COMM.val.xcom.PortNum,p, poption->data_lg);
        UART_Write(pdev->COMM.val.xcom.PortNum, p, poption->data_lg);
        break;
    case DEVICE_COMMTYPE_UDP:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        break;
    }

    if (MSG_DEV_TYJA == pdev->dev_type)
    {
        buf[0] = MSG_DEV_TYJB;
        buf[1] = pbuf[1];
        //buf[2] = MSG_OPTION_POWERCLOSE;
        buf[2] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 5, &buf[0], 2, 0, &buf[2]);
    }

    //memcpy( &id,(uint8 *)pdata + 3,1 );
    id = pbuf[3];
    if ((MSG_DEV_TYJA == pdev->dev_type || MSG_DEV_TYJB == pdev->dev_type) && id > 1  && MSG_OPTION_POWERCLOSE == pbuf[1])
    {
        //memcpy( &buf[0],pbuf,2 );
        buf[0] = pdev->dev_type;
        buf[1] = MSG_OPTION_REPEATPOWERCLOSE;
        buf[2] = pbuf[2];
        buf[3] = id - 1;
        id = (pdev->prev_val >> 3);
        buf[4] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + id, &buf[0], 4, 3, &buf[4]);
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_Power_OPTION(void *pdata)
{
    uint8 *p, id, *pbuf, buf[5] = {0};
    struct DEVICE_INFO *pdev;
    struct MESSAGE_TYPE *poption;

    pbuf = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE);
    pdev = (struct DEVICE_INFO *)Device_Rec_Find(pbuf, DEV_FIND_TYPE);
    if (pdev == NULL)
        return FALSE;//没有该设备登记

    id = Msg_Option_Find(pbuf);
    if (id == 0)
        return FALSE; //没有该操作码记录
    poption = (struct MESSAGE_TYPE *)Get_ClassInfo_Table(id);
    p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
    switch (pdev->COMM.comm_type)
    {
    case DEVICE_COMMTYPE_NONE:
        break;
    case DEVICE_COMMTYPE_HW:
        HW_send(poption->dev_id);
        break;
    case DEVICE_COMMTYPE_XCOM:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        //		UART_Send_Data(pdev->COMM.val.xcom.PortNum,p, poption->data_lg);
        if(pdev->COMM.val.xcom.PortNum == DEV_XCOM_TYJ)
            Xcom_set(DEV_XCOM_TYJ, pdev->COMM.val.xcom.BPS + (pdev->COMM.val.xcom.Hbps * 0x10000), DEV_PROTOCOL_NONE, pdev->COMM.val.xcom.linecontrl);
        UART_Write(pdev->COMM.val.xcom.PortNum, p, poption->data_lg);
        break;
    case DEVICE_COMMTYPE_UDP:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        break;
    }

    if (MSG_DEV_TYJA == pdev->dev_type)
    {
        buf[0] = MSG_DEV_TYJB;
        buf[1] = pbuf[1];
        buf[2] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 5, &buf[0], 2, 0, &buf[2]);
    }

    id = pdev->prev_val & 0x7;
    if ((MSG_DEV_TYJA == pdev->dev_type || MSG_DEV_TYJB == pdev->dev_type) && id > 1 && MSG_OPTION_POWERCLOSE == pbuf[1])
    {
        buf[0] = pdev->dev_type;
        buf[1] = MSG_OPTION_REPEATPOWERCLOSE;
        buf[2] = MSG_OPTION_POWERCLOSE;
        buf[3] = id - 1;

        id = (pdev->prev_val >> 3);
        buf[4] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + id, &buf[0], 4, 0, &buf[4]);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION(void *pdata)
{
    uint8 *p, id, *pbuf;
    struct DEVICE_INFO *pdev;
    struct MESSAGE_TYPE *poption;

    pbuf = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE);
    pdev = (struct DEVICE_INFO *)Device_Rec_Find(pbuf, DEV_FIND_SELECT);
    if (pdev == NULL)
        return FALSE;//没有该设备登记
    id = Msg_Option_Find(pbuf);
    if (id == 0)
        return FALSE; //没有该操作码记录
    poption = (struct MESSAGE_TYPE *)Get_ClassInfo_Table(id);
    p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
    switch (pdev->COMM.comm_type)
    {
    case DEVICE_COMMTYPE_NONE:
        break;
    case DEVICE_COMMTYPE_HW:
        HW_send(poption->dev_id);
        break;
    case DEVICE_COMMTYPE_XCOM:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        //		UART_Send_Data(pdev->COMM.val.xcom.PortNum,p, poption->data_lg);
        UART_Write(pdev->COMM.val.xcom.PortNum, p, poption->data_lg);
        break;
    case DEVICE_COMMTYPE_UDP:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        break;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：外置DVI切换器，通过DVD红外学码实现,change 2016.1.5
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Switch_DVI(uint8 in)
{
    struct BASE_MSG keymsg;
    struct MSG_REV_TYPE  ReV;
    uint8  buf[15];

    if((in == DEV_VGA_PC) || (in == DEV_VGA_NPC) || (in == DEV_VGA_SHOW) || (in == DEV_VGA_DVD))
    {
        keymsg.device = MSG_DEV_DVD;
        ReV.Type = DEVICE_SUB_CLASS;
        switch(in)
        {
        case DEV_VGA_PC:
            keymsg.option = MSG_OPTION_UP;
            break;
        case DEV_VGA_NPC:
            keymsg.option = MSG_OPTION_DOWN;
            break;
        case DEV_VGA_SHOW:
            keymsg.option = MSG_OPTION_LIFT;
            break;
        case DEV_VGA_DVD:
            keymsg.option = MSG_OPTION_RIGHT;
            break;
        }
        memset(buf, 0, 5);
        memcpy(buf, (uint8 *)&keymsg, 2);
        Msg_send(buf, 3, (uint8 *)&ReV);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("Switch_DVI %d",in);
    }
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 ON_Device_select(void *pdata)
{
    uint8 tmp, buf[16];
    struct DEVICE_INFO *pdev;
    struct BASE_MSG bmsg, *pmsg;
    struct DEVICE_STATUS_TYPE *ps;

    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    //Switch_DVI(pmsg->device);//add 2015.3.19
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    //是当前设备
    if ((ps->Current_Device != pmsg->device) && (pmsg->device != MSG_DEV_PC))
    {
        if (ps->Current_Device != MSG_DEV_PC)
            Device_Stat_Option(ps->Current_Device, MSG_OPTION_STAT_END);
        Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_START);
    }
    ps->Current_Device = pmsg->device;
    pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg, DEV_FIND_SELECT);
    //发送设备切换消息--在设备表中定义
    if (pdev != NULL)
    {
        if (pdev->VGA.VGAIn != DEV_VGA_NONE)  //有VGA连接
        {
            //检查投影机A  VGA是否有连接
            if (gDeviceTable.TYJ[0].vgain	!=	DEV_VGA_NONE)
            {
                if (gDeviceTable.TYJ[0].curr_Type == 0)
                {
                    gDeviceTable.TYJ[0].curr_Type = 1; //发送显示VGA
                    bmsg.device = MSG_DEV_TYJA;
                    bmsg.option = MSG_OPTION_TYJ_DIS_VGA;
                    Mem_Copy(buf + sizeof(struct MSG_REV_TYPE), (uint8 *)&bmsg, sizeof(struct BASE_MSG));
                    On_MSG_OPTION(&buf);
                };
                if (pdev->VGA.VGALink	!=	DEV_VGA_OUT_NONE)
                {
                    Device_VGA_Select(pdev->VGA.VGAIn, pdev->VGA.VGALink);
                    if (pdev->VGA.VGALink  == DEV_VGA_OUT_TB1)
                        TB_SelectVGA(TB_InputVGA_OUT_TB1, TB_OutputVGA_TYJ);
                }
                else
                    Device_VGA_Select(pdev->VGA.VGAIn, DEV_VGA_OUT_TYJ);
            };
            //检查投影机B  VGA是否有连接
            if (gDeviceTable.TYJ[1].vgain != DEV_VGA_NONE)
            {
                if (gDeviceTable.TYJ[1].curr_Type == 0)
                {
                    gDeviceTable.TYJ[1].curr_Type = 1; //发送显示VGA
                    bmsg.device = MSG_DEV_TYJB;
                    bmsg.option = MSG_OPTION_TYJ_DIS_VGA;
                    Mem_Copy(buf + sizeof(struct MSG_REV_TYPE), (uint8 *)&bmsg, sizeof(struct BASE_MSG));
                    On_MSG_OPTION(&buf);

                }
            }

        }
        //检查同步输出
        if (pdev->VIDEO.VideoIn	!= DEV_VIDEO_NONE)
        {
            if (gDeviceTable.TYJ[0].videoin != DEV_VIDEO_OUT_NONE)
            {
                if (gDeviceTable.TYJ[0].curr_Type == 1)
                {
                    gDeviceTable.TYJ[0].curr_Type = 0;
                    bmsg.device = MSG_DEV_TYJA;
                    bmsg.option = MSG_OPTION_TYJ_DIS_VIDEO;
                    Mem_Copy(buf + sizeof(struct MSG_REV_TYPE), (uint8 *)&bmsg, sizeof(struct BASE_MSG));
                    On_MSG_OPTION(&buf);

                }
                Device_Video_Select(pdev->VIDEO.VideoIn, DEV_VIDEO_OUT_TYJA);
            }
            if (gDeviceTable.TYJ[1].videoin != DEV_VIDEO_OUT_NONE)
            {
                if (gDeviceTable.TYJ[1].curr_Type == 1)
                {
                    gDeviceTable.TYJ[1].curr_Type = 0;
                    bmsg.device = MSG_DEV_TYJB;
                    bmsg.option = MSG_OPTION_TYJ_DIS_VIDEO;
                    Mem_Copy(buf + sizeof(struct MSG_REV_TYPE), (uint8 *)&bmsg, sizeof(struct BASE_MSG));
                    On_MSG_OPTION(&buf);

                }
                Device_Video_Select(pdev->VIDEO.VideoIn, DEV_VIDEO_OUT_TYJB);
            }
        }
        //音频信号
        if (pdev->SOUND.SoundIn != DEV_SOUND_NONE)
        {
            Device_Sound_Contrl(TRUE);
            Device_Sound_Select(pdev->SOUND.SoundIn);
            Device_InVol(pdev->SOUND.SoundVol, DEV_SOUND_DEV);
            //zjj add6
            //SOUND_DEVICE_Mute(VOL_CH_MIC1,0);
            //SOUND_DEVICE_Mute(VOL_CH_MIC2,0);
            //SOUND_DEVICE_Mute(VOL_CH_MIC3,0);
            //SOUND_DEVICE_Mute(VOL_CH_MIC4,0);
            SoundVolumeControl(VOL_CH_GF, DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol);
            KEY_Set_Vol(DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol);
        }
        else
            Device_Sound_Contrl(FALSE);
    }
    else
    {
        //没有设备表
        pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
        switch(pmsg->device)
        {
        case MSG_DEV_PC:
            Device_VGA_Select(DEV_VGA_PC, DEV_VGA_OUT_TYJ);
            tmp = Device_Get_Vol(DEV_SOUND_OUT_GF, 4);
            Device_Sound_Select(DEV_SOUND_PC);
            Device_Sound_Contrl(TRUE);
            break;
        case MSG_DEV_DVD:
            Device_VGA_Select(DEV_VGA_DVD, DEV_VGA_OUT_TYJ);
            Device_Video_Select(DEV_VIDEO_DVD, DEV_VIDEO_OUT_TYJA);
            Device_Video_Select(DEV_VIDEO_DVD, DEV_VIDEO_OUT_TYJB);

            Device_Sound_Contrl(TRUE);
            Device_Sound_Select(DEV_SOUND_DVD);
            break;
        case MSG_DEV_NPC:
            Device_VGA_Select(DEV_VGA_NPC, DEV_VGA_OUT_TYJ);
            Device_Sound_Select(DEV_SOUND_NPC);

            break;
        case MSG_DEV_SHOW:
            Device_VGA_Select(DEV_VGA_SHOW, DEV_VGA_OUT_TYJ);
            Device_Video_Select(DEV_VIDEO_SHOW, DEV_VIDEO_OUT_TYJA);
            Device_Video_Select(DEV_VIDEO_SHOW, DEV_VIDEO_OUT_TYJB);
            Device_Sound_Contrl(FALSE);
            break;
        case MSG_DEV_LYJ:
            Device_Sound_Select(DEV_SOUND_LYJ);
            Device_Video_Select(DEV_VIDEO_LXJ, DEV_VIDEO_OUT_TYJA);
            Device_Video_Select(DEV_VIDEO_LXJ, DEV_VIDEO_OUT_TYJB);
            Device_Sound_Contrl(TRUE);
            break;
        case MSG_DEV_KZA:
        case MSG_DEV_KZB:
            Device_Sound_Contrl(TRUE);
            break;
        }
        Device_InVol(95, DEV_SOUND_DEV);
    }
    tmp = tmp;
    return STATUS_CHANGE;

}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
extern uint8 PAD_ip[4];
uint32 Send_State(void)
{
    BYTE *desip;
    BYTE buf[80] = {0};
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_STATUS_CHANGE;
    desip = Get_DesIp(DEVICE_SUB_SER);
    memcpy(buf + sizeof(struct BASE_MSG), (BYTE *)Get_Device_Status(), sizeof(struct DEVICE_STATUS_TYPE));
    UDP_Send_CMD(buf, sizeof(struct BASE_MSG) + sizeof(struct DEVICE_STATUS_TYPE), desip);
    //if (Get_Debug(DEV_DEBUG_MSG))
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("向IP:%d.%d.%d.%d发送状态", *desip, *(desip + 1), *(desip + 2), *(desip + 3));

    desip = PAD_ip;
    UDP_Send_CMD(buf, sizeof(struct BASE_MSG) + sizeof(struct DEVICE_STATUS_TYPE), desip);
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("向PAD，IP:%d.%d.%d.%d发送状态", *desip, *(desip + 1), *(desip + 2), *(desip + 3));

    IO_FlagLingt_Flash();
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 ON_Change_State(struct BASE_MSG *pmsg)
{
    uint8 flag = FALSE;
    if (pmsg->device == MSG_DEV_PC)
    {
        switch(pmsg->option)
        {
        case MSG_OPTION_OPEN_OVER:
            gDeviceStatus.DEV_Link_State |= DEV_LINK_PCON;
            flag = TRUE;
            break;
        case MSG_OPTION_POWERCLOSE:
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_PCON;
            flag = TRUE;
            break;
        case MSG_OPTION_DEV_LINK:
            gDeviceStatus.DEV_Link_State |= DEV_LINK_PCOK;
            flag = TRUE;
            break;
        case MSG_OPTION_DEV_BREAK:
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_PCLK;
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_PCOK;
            flag = TRUE;
            break;
        }
    }
    else if (pmsg->device == MSG_DEV_LMT)
    {
        switch(pmsg->option)
        {
        case MSG_OPTION_OPEN_OVER:
            gDeviceStatus.DEV_Link_State |= DEV_LINK_LMTON;
            flag = TRUE;
            break;
        case MSG_OPTION_POWERCLOSE:
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_LMTON;
            flag = TRUE;
            break;
        case MSG_OPTION_DEV_LINK:
            gDeviceStatus.DEV_Link_State |= DEV_LINK_LMTLK;
            flag = TRUE;
            break;
        case MSG_OPTION_DEV_BREAK:
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_LMTLK;
            flag = TRUE;
            break;
        }
    }
    else if (pmsg->device == MSG_DEV_KB)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_KBLK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_KBLK;
            flag = TRUE;
        }
    }
    else if (pmsg->device == MSG_DEV_TRACK)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_BJCardLK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_BJCardLK;
            flag = TRUE;
        }
    }
    else if (pmsg->device == MSG_DEV_TB)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_TBLK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_TBLK;
            flag = TRUE;
        }
    }
    else if (pmsg->device == MSG_DEV_VIDEOCARD)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_BM1LK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_BM1LK;
            flag = TRUE;
        }
    }
    else if (pmsg->device == MSG_DEV_KZYY)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_YYLK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_YYLK;
            flag = TRUE;
        }
    }
    flag = flag;
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_RECORD(void *pdata)
{
    //uint8 buf[10];
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    switch(pmsg->option)
    {
    //录制操作
    case MSG_OPTION_RECORD_START:	//0x40  //录像开始
        gDeviceStatus.CLASS_state &= 0x0f;
        gDeviceStatus.CLASS_state |= DEV_CLASS_RECSTART;
        KEY_DATA.key_rep |= KEY_REP_REC_PUSH;
        LED_Set(LED_REC, LED_DISPLAY_ON);
        if (Check_FunctionMode(Device_FunctionMode_LB) == TRUE)
            LED_Set(LED_KZB, LED_DISPLAY_OFF);
        DWQ_Contrl(1);//打开超声波定位器
        /////////////////////////2011.3.2 test by liyang/////////////////
        /*BYTE ip[4] = {0,0,0,0};
        BYTE *des_ip = ip;
        Sound_Close(NULL,SOUND_OPTION_ALL);
        Sound_Open(des_ip,AUDIO_PORT,1,SOUND_CONTRL_SEND);
                ////////////////////////////////
        BYTE pcontrol = 1;
        BYTE blank_buf[512],new_buf[512];
        memset(blank_buf, 0xb, 512);
        memset(new_buf, 0, 512);
        BYTE *buff1 = blank_buf;
        BYTE *buff2 = new_buf;
        for(int i=0;i<10000;i++)
        {
          Sound_Save_PlayData(buff1, &pcontrol, 1);
          Sound_Save_PlayData(buff2, &pcontrol, 1);
        }
        Sound_Close(NULL,SOUND_OPTION_ALL); */
        ////////////////////////////////////////////////
        ////////////////////////////////////////////////////
        break;
    case MSG_OPTION_RECORD_PAUSE:	//0x41 //录像暂停
        gDeviceStatus.CLASS_state &= 0x0f;
        gDeviceStatus.CLASS_state |= DEV_CLASS_RECPUSH;
        KEY_DATA.key_rep &= ~KEY_REP_REC_PUSH;
        LED_Set(LED_REC, LED_DISPLAY_FLASH);
        if (Check_FunctionMode(Device_FunctionMode_LB) == TRUE)
        {
            LED_Set(LED_KZB, LED_DISPLAY_OFF);
        }
        break;
    case MSG_OPTION_RECORD_STOP:	//0x42 //录像停止
        gDeviceStatus.CLASS_state &= 0x0f;
        gDeviceStatus.CLASS_state |= DEV_CLASS_RECSTOP;
        KEY_DATA.key_rep &= ~KEY_REP_REC_PUSH;
        LED_Set(LED_REC, LED_DISPLAY_OFF);
        if (Check_FunctionMode(Device_FunctionMode_LB) == TRUE)
        {
            LED_Set(LED_KZB, LED_DISPLAY_OFF);
        }
        DWQ_Contrl(0);//打开超声波定位器
        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_METRIC(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));

    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    if ((pmsg->option < MSG_OPTION_METRIC1) ||
            (pmsg->option > MSG_OPTION_METRIC3)) return 0;
    LED_Set(LED_METRIC_XGA, LED_DISPLAY_OFF);
    LED_Set(LED_METRIC_D1, LED_DISPLAY_OFF);
    LED_Set(LED_METRIC_ZDY, LED_DISPLAY_OFF);

    switch(pmsg->option)
    {
    case MSG_OPTION_METRIC1:	//0x46 //分辨率XGA
        gDeviceStatus.Current_REC_set &= 0xff0f;
        gDeviceStatus.Current_REC_set |= DEV_REC_LZVGA;
        LED_Set(LED_METRIC_XGA, LED_DISPLAY_ON);
        KEY_DATA.METRIC = MSG_OPTION_METRIC1;
        break;
    case MSG_OPTION_METRIC2:	//0x47 //分辨率D1
        gDeviceStatus.Current_REC_set &= 0xff0f;
        gDeviceStatus.Current_REC_set |= DEV_REC_LZD1;
        LED_Set(LED_METRIC_D1, LED_DISPLAY_ON);
        KEY_DATA.METRIC = MSG_OPTION_METRIC2;
        break;
    case MSG_OPTION_METRIC3:	//0x48 ////分辨率自定义
        gDeviceStatus.Current_REC_set &= 0xff0f;
        gDeviceStatus.Current_REC_set |= DEV_REC_LZZDY;
        LED_Set(LED_METRIC_ZDY, LED_DISPLAY_ON);
        KEY_DATA.METRIC = MSG_OPTION_METRIC3;
        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_FRAME(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));

    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    if ((pmsg->option < MSG_OPTION_FRAME1) ||
            (pmsg->option > MSG_OPTION_FRAME3)) return 0;
    LED_Set(LED_FRAME_CG, LED_DISPLAY_OFF);
    LED_Set(LED_FRAME_HZH, LED_DISPLAY_OFF);
    LED_Set(LED_FRAME_ZDY, LED_DISPLAY_OFF);
    switch(pmsg->option)
    {
    case MSG_OPTION_FRAME1:	//0x49//常规
        gDeviceStatus.Current_REC_set &= 0xf0ff;
        gDeviceStatus.Current_REC_set |= DEV_REC_TXCG;
        KEY_DATA.FRAME = MSG_OPTION_FRAME1;
        LED_Set(LED_FRAME_CG, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_FRAME2:	//0x4a//画中画
        gDeviceStatus.Current_REC_set &= 0xf0ff;
        gDeviceStatus.Current_REC_set |= DEV_REC_TXHZH;
        KEY_DATA.FRAME = MSG_OPTION_FRAME2;
        LED_Set(LED_FRAME_HZH, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_FRAME3:	//0x4b //自定义
        gDeviceStatus.Current_REC_set &= 0xf0ff;
        gDeviceStatus.Current_REC_set |= DEV_REC_TXZDY;
        KEY_DATA.FRAME = MSG_OPTION_FRAME3;
        LED_Set(LED_FRAME_ZDY, LED_DISPLAY_ON);
        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_CODE1_SPEED(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));

    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    if ((pmsg->option < MSG_OPTION_CODE1_SPEED1) ||
            (pmsg->option > MSG_OPTION_CODE1_SPEED3)) return 0;
    LED_Set(LED_SPEED_400, LED_DISPLAY_OFF);
    LED_Set(LED_SPEED_1M, LED_DISPLAY_OFF);
    LED_Set(LED_SPEED_ZDY, LED_DISPLAY_OFF);
    switch(pmsg->option)
    {
    case MSG_OPTION_CODE1_SPEED1:	//0x43 // 码速1 500k
        gDeviceStatus.Current_REC_set &= 0xfff0;
        gDeviceStatus.Current_REC_set |= DEV_REC_ML500;
        KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED1;
        LED_Set(LED_SPEED_400, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_CODE1_SPEED2:	//0x44// 码速2 1M
        gDeviceStatus.Current_REC_set &= 0xfff0;
        gDeviceStatus.Current_REC_set |= DEV_REC_ML1M;
        KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED2;
        LED_Set(LED_SPEED_1M, LED_DISPLAY_ON);

        break;
    case MSG_OPTION_CODE1_SPEED3:	//0x45 // 码速1 自定义
        gDeviceStatus.Current_REC_set &= 0xfff0;
        gDeviceStatus.Current_REC_set |= DEV_REC_MLZDY;
        KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED3;
        LED_Set(LED_SPEED_ZDY, LED_DISPLAY_ON);
        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_PERSONAL_TALL(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    switch(pmsg->option)
    {
    //摄象操作
    case MSG_OPTION_PERSONAL_TALL1:	//0x30  //身高设定160
        gDeviceStatus.Current_REC_set &= 0x0fff;
        gDeviceStatus.Current_REC_set |= DEV_REC_SG160;
        KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL1;
        LED_Set(LED_TAIL_170, LED_DISPLAY_OFF);
        LED_Set(LED_TAIL_180, LED_DISPLAY_OFF);
        LED_Set(LED_TAIL_160, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_PERSONAL_TALL2:	//0x31 //身高设定170
        gDeviceStatus.Current_REC_set &= 0x0fff;
        gDeviceStatus.Current_REC_set |= DEV_REC_SG170;
        KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL2;
        LED_Set(LED_TAIL_170, LED_DISPLAY_ON);
        LED_Set(LED_TAIL_180, LED_DISPLAY_OFF);
        LED_Set(LED_TAIL_160, LED_DISPLAY_OFF);
        break;
    case MSG_OPTION_PERSONAL_TALL3:	//0x32 //身高设定180
        gDeviceStatus.Current_REC_set &= 0x0fff;
        gDeviceStatus.Current_REC_set |= DEV_REC_SG180;
        KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL3;
        LED_Set(LED_TAIL_170, LED_DISPLAY_OFF);
        LED_Set(LED_TAIL_180, LED_DISPLAY_ON);
        LED_Set(LED_TAIL_160, LED_DISPLAY_OFF);

        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SELECT_SCAMERA(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if ((pmsg->option < MSG_OPTION_SELECT_SCAMERA1) ||
            (pmsg->option > MSG_OPTION_SELECT_SCAMERA6)) return 0;
    LED_Set(LED_SCAMERA1, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA2, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA3, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA4, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA5, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA6, LED_DISPLAY_OFF);
    switch(pmsg->option)
    {
    case MSG_OPTION_SELECT_SCAMERA1:	//0x33 //摄象机选择1
        gDeviceStatus.Curr_SCAME = 1;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA1;
        LED_Set(LED_SCAMERA1, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA2:	//0x34 //摄象机选择2
        gDeviceStatus.Curr_SCAME = 2;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA2;
        LED_Set(LED_SCAMERA2, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA3:	//0x35//摄象机选择3
        gDeviceStatus.Curr_SCAME = 3;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA3;
        LED_Set(LED_SCAMERA3, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA4:	//0x36//摄象机选择4
        gDeviceStatus.Curr_SCAME = 4;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA4;
        LED_Set(LED_SCAMERA4, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA5:	//0x37//摄象机选择5
        gDeviceStatus.Curr_SCAME = 5;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA5;
        LED_Set(LED_SCAMERA5, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA6:	//0x38//摄象机选择6
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA6;
        gDeviceStatus.Curr_SCAME = 6;
        LED_Set(LED_SCAMERA6, LED_DISPLAY_ON);
        break;
    }
    if (Get_Class_Stauts() != CLASS_OVER)
        LED_SendFLASH();
    return STATUS_CHANGE;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_REC_ReqStatus(void *pdata)
{


    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_REC_Contrl(void *pdata)
{
    uint8 *p, tmpVGA, tmpAV, tmp;
    struct TBCard_RECNo_State_Type *pRecState;
    struct TB_RECSingle_ContrlType   RecCtrl;
    struct DEVICE_INFO *pdev;

    pRecState = Get_TB_REC_State();
    p = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    Mem_Copy((uint8 *)&RecCtrl, p, sizeof( struct TB_RECSingle_ContrlType));
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("录制控制类型:%d,输出:%d,ID:%d", RecCtrl.SingleType, RecCtrl.SingleNo, RecCtrl.SingleID);
    p += sizeof( struct TB_RECSingle_ContrlType);
    Mem_Copy((uint8 *)pRecState, p, sizeof( struct TBCard_RECNo_State_Type));
    pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(RecCtrl.SingleID);
    if ((pdev->dev_id != RecCtrl.SingleID) || (RecCtrl.SingleID < 16))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("录制通道控制-错误的ID%d", RecCtrl.SingleID);
        return 0;
    }
    if ((pdev->dev_type == 0xfe) || (pdev->dev_type < 16))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("录制通道控制-错误的设备类型%d", pdev->dev_type);
        return 0;
    }
    tmpVGA = pdev->VGA.VGAIn;
    tmpAV = pdev->VIDEO.VideoIn;
    if (RecCtrl.SingleType == TBCard_Option_VGA)
    {
        if (tmpVGA == DEV_VGA_OUT_NONE) return 0;
        TB_SelectVGA(TB_InputVGA_OUT_TB0, TB_OutputVGA_LMT);
        Device_VGA_Select(tmpVGA, DEV_VGA_OUT_TB0);
        TB_REC_UnFreez(TBCard_Option_VGA, RecCtrl.SingleNo, RecCtrl.SingleID);

    }
    else
    {
        if (tmpAV == DEV_VIDEO_NONE)	 return 0;
        TB_SelectAV(RecCtrl.SingleNo, RecCtrl.SingleNo);
        tmp = TB_SingleChage(TBCard_Option_AV, RecCtrl.SingleNo);
        Device_Video_Select(tmpAV, tmp);
        TB_REC_UnFreez(TBCard_Option_AV, RecCtrl.SingleNo, RecCtrl.SingleID);
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_LMT_ReqStatus(void *pdata)
{
    //struct BASE_MSG *pmsg;
    struct DEVICE_STATUS_TYPE *ps;

    uint8 *p, tmp;
    uint32 *pint;
    // pmsg = (struct BASE_MSG *)((uint8 *)pdata+sizeof(struct MSG_REV_TYPE));
    p = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    tmp = *p;
    pint = (uint32 *)(p + 2);
    gDeviceStatus.Disk_size = *pint;
    gDeviceStatus.Current_REC = tmp;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    ps->Current_REC = tmp;
    LMT_Delay_Power();
    switch(tmp)
    {
    case LMT_STATUS_NONE:
        break;
    case LMT_STATUS_REC_Push:
    case LMT_STATUS_Pushing:
        Key_TB_state(KEY_REP_REC_PUSH, FALSE);
        LED_Set(LED_REC, LED_DISPLAY_FLASH);
        DWQ_Contrl(1);//打开超声波定位器
        break;
    case LMT_STATUS_BackPlay:
        LED_Set(LED_KZB, LED_DISPLAY_OFF);
        Key_TB_state(KEY_REP_YREC, FALSE);
        break;
    case LMT_STATUS_RECing:
        LED_Set(LED_REC, LED_DISPLAY_ON);
        Key_TB_state(KEY_REP_REC_PUSH, TRUE);
        DWQ_Contrl(1);//打开超声波定位器
        break;
    case LMT_STATUS_Stoping:
        Key_TB_state(KEY_REP_REC_PUSH, FALSE);
        LED_Set(LED_REC, LED_DISPLAY_FLASH);
        DWQ_Contrl(1);//打开超声波定位器
        break;
    case LMT_STATUS_REC_Stop:
        Key_TB_state(KEY_REP_REC_PUSH, FALSE);
        LED_Set(LED_REC, LED_DISPLAY_OFF);
        DWQ_Contrl(0);//打开超声波定位器
        break;
    case LMT_STATUS_Exit:
        Key_TB_state(KEY_REP_REC_PUSH, FALSE);
        LED_Set(LED_REC, LED_DISPLAY_OFF);
        DWQ_Contrl(0);//打开超声波定位器
        break;
    case LMT_STATUS_Exiting:
        DWQ_Contrl(0);//打开超声波定位器
        break;

    }
    LED_SendFLASH();
    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SCAMERA(void *pdata)
{

    BYTE *pBuf, id;
    BYTE data[10] = {0};
    BYTE hspeed, vspeed;
    struct BASE_MSG *pmsg;
    struct DEVICE_INFO *pdev;
    struct MSG_REV_TYPE *prev;

    uint8 com;
    uint8 portNum;
    uint16 baudRate = 2400;
    BYTE addr = 1;
    uint16 protocol = 3;
    BYTE devid = 0;
    DWORD dwPos = 0;
    DWORD dwNetData = 0;
#if  Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    pBuf = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    memcpy(&dwNetData, pBuf + sizeof( struct BASE_MSG ) + 4, 4);
    memcpy(&dwPos, pBuf + sizeof( struct BASE_MSG ), 4);
    memcpy(&hspeed, pBuf + sizeof( struct BASE_MSG ) + 2, 1);
    memcpy(&vspeed, pBuf + sizeof( struct BASE_MSG ) + 3, 1);

    if (hspeed <= 0)
    {
        hspeed = 32;
    }
    if (hspeed > 64)
    {
        hspeed = 64;
    }
    if (vspeed <= 0)
    {
        vspeed = 32;
    }
    if (vspeed > 64)
    {
        vspeed = 64;
    }

    //gDeviceTable.Debug |= DEV_DEBUG_MSG;
#if 0   //2011年6月15日修改
    if (0xa5a6a5a6 == dwNetData)//是集控发过来得数据
    {
        pBuf = Get_ClassInfo_Table(0);
        devid = *(pBuf + 140 );//读出监控
        if (devid > 7 ) return 0;
        devid = Encode_TAB[devid];
        for (id = MSG_DEV_MAINBARD + 1; id < 254; id++)
        {
            pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(id);
            if (pdev->dev_type == MSG_DEV_VIDEOCARD)
            {
                if (pdev->VIDEO.VideoOut == devid) break;
            }
        }
        if (id >= 254)  return 0;
        devid = pdev->VIDEO.VideoLink;
        if ((devid < DEV_VIDEO_OUT_BM1_1) || (devid > DEV_VIDEO_OUT_BM2_4)) return 0;
        pdev = (struct DEVICE_INFO *)Device_Rec_Find(&devid, DEV_FIND_VIDEOIN);
        if (pdev->dev_type == 0xfe) return 0;
        pBuf = (uint8 *)pdev;
        if (NULL == pBuf)
            return 0;

    }
    else
    {
        hspeed = vspeed = 32;//不是网络发过来得，就是键盘发的消息，速度固定死32
        //将原来按设备类型查找改为按连接端口设置查找
        devid = DEV_VIDEO_QSCAMERA1 + gDeviceStatus.Curr_SCAME - 1;
        pBuf = Device_Rec_Find(&devid, DEV_FIND_VIDEOIN);
        if (NULL == pBuf)
            return 0;
    }
#endif
    if (0xa5a6a5a6 == dwNetData)//是集控发过来得数据
    {
        devid = DEV_VIDEO_QSCAMERA1 + gDeviceStatus.Curr_SCAME - 1;
        pBuf = Device_Rec_Find(&devid, DEV_FIND_VIDEOIN);
        if (NULL == pBuf)
            return 0;

    }
    else
    {
        hspeed = vspeed = 32;//不是网络发过来得，就是键盘发的消息，速度固定死32
        //将原来按设备类型查找改为按连接端口设置查找
        devid = DEV_VIDEO_QSCAMERA1 + gDeviceStatus.Curr_SCAME - 1;
        pBuf = Device_Rec_Find(&devid, DEV_FIND_VIDEOIN);
        if (NULL == pBuf)
            return 0;
        memcpy(&protocol, pBuf + 94, 2);
        if(protocol == DEV_PROTOCOL_VISCA)
        {
            if(pmsg->option == MSG_OPTION_SCAMERA_ZOOMADD || pmsg->option == MSG_OPTION_SCAMERA_ZOOMSUB)
                hspeed = vspeed = 4;
            else
                hspeed = vspeed = 10;
        }
    }
    memcpy(&com, pBuf + 90, 1);
    if(com == 6)
    {
        portNum = DEV_XCOM_TB;
    }
    else
    {
        portNum = DEV_XCOM_SCAMERA;
    }
    memcpy(&addr, pBuf + 91, 1);
    memcpy(&baudRate, pBuf + 92, 2);
    memcpy(&protocol, pBuf + 94, 2);
    if ((baudRate != 2400) && (baudRate != 4800) && (baudRate != 9600) && (baudRate != 19200) && (baudRate != 38400)) return 0;
    //baudRate+=0x10000*(*(pBuf+97));//高位波特率
    if (XCOM_PORT[portNum].BPS != baudRate)
    {
        Xcom_set(portNum, baudRate, DEV_PROTOCOL_NONE, 0x03);
    }
    SetAddress( addr , protocol);
    if(ViscaCount == 0)
    {
        data[0] = 0x88;
        data[1] = 0x30;
        data[2] = 0x01;
        data[3] = 0xff;
        UART_Write(portNum, (uint8 *)&data[0], 4);
        ViscaCount++;
    }
    switch(pmsg->option)
    {
    case MSG_OPTION_SCAMERA_ZOOMADD:	//0x39//摄象机ZOOM+
        ZoomInCmd( &data[0], vspeed, protocol);
        break;
    case MSG_OPTION_SCAMERA_ZOOMSUB:	//0x3a//摄象机ZOOM-
        ZoomOutCmd( &data[0], vspeed, protocol);
        break;
    case MSG_OPTION_SCAMERA_UP:			//0x3b//摄象机上
        TurnUpCmd( &data[0], vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_DOWN:		//0x3c//摄象机下
        TurnDownCmd( &data[0], vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_LIFT:		//0x3d//摄象机左
        TurnLeftCmd( &data[0], hspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_RIGHT:		//0x3e//摄象机右
        TurnRightCmd( &data[0], hspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_STOP:
        StopCmd(portNum, &data[0] , protocol);
        if(protocol == DEV_PROTOCOL_VISCA)
            return 0;
        break;
    case MSG_OPTION_SCAMERA_LEFTUP:
        TurnLeftUpCmd( &data[0], hspeed, vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_LEFTDOWN:
        TurnLeftDownCmd( &data[0], hspeed, vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_RIGHTUP:
        TurnRightUpCmd( &data[0], hspeed, vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_RIGHTDOWN:
        TurnRightDownCmd( &data[0], hspeed, vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_FOCUSNEAR:
        FocusNearCmd(  &data[0] , protocol);
        break;
    case MSG_OPTION_SCAMERA_FOCUSFAR:
        FocusFarCmd(  &data[0]  , protocol);
        break;
    case MSG_OPTION_SCAMERA_PREPOSITION:
        GotoPrepositionCmd( &data[0], (BYTE)dwPos , protocol);
        break;
    case MSG_OPTION_SCAMERA_SETPREPOSITION:
        SetPrepositionCmd(  &data[0], (BYTE)dwPos , protocol);
        break;
    }
    if(protocol == DEV_PROTOCOL_PD)
    {
        UART_Write(portNum, (uint8 *)&data[0], PELCOD_LENGTH );
    }
    else
    {
        UART_Write(portNum, (uint8 *)&data[0], VISCA_LENGTH );
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SOUND_SELECT(void *pdata)
{

    BYTE *p;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);

    Device_Sound_Select(*(p + 2));

    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：设置设备音量
//	函数功能：设置设备音量
//	入口参数：*(pdata)=(struct MSG_REV_TYPE)接收路径8字节
//			             *(pdata+8)=(struct BASE_MSG)设备号+操作码(2字节)
//			             *(pdata+8+2)=Vol音量
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SOUND_VOL(void *pdata)
{
    struct BASE_MSG *pmsg;
    BYTE *p, Vol;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    Vol = *(p + 2);
    switch(pmsg->device)
    {
    case MSG_DEV_GF:
        Device_OutVol(Vol, DEV_SOUND_OUT_GF);
        //Vol=DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].OUT_Vol;
        //Device_OutVol(Vol, DEV_SOUND_OUT_LMT);

        break;
    case MSG_DEV_LMT:
        Device_OutVol(Vol, DEV_SOUND_OUT_LMT);
        break;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_VIDEO_SELECT(void *pdata)
{
    struct BASE_MSG *pmsg;
    BYTE *p;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if (pmsg->device == MSG_DEV_TB)
        TB_SelectAV(*(p + 2), *(p + 3));
    else
    {
        if (*(p + 4) == 0x55)
        {
            Device_Video_Off(*(p + 3));
            IP_printf("Video信号关闭");
        }
        else
            Device_Video_Select(*(p + 2), *(p + 3));
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_VGA_SELECT(void *pdata)
{
    BYTE *p;
    struct BASE_MSG *pmsg;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if (pmsg->device == MSG_DEV_TB)
        TB_SelectVGA(*(p + 2), *(p + 3));
    else
    {
        if (*(p + 4) == 0x55)
        {
            Device_VGA_Off(*(p + 3));
            IP_printf("VGA信号关闭");
        }
        else
            Device_VGA_Select(*(p + 2), *(p + 3));
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_ARLM(void *pdata)
{
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_IO_OUT(void *pdata)
{
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_JT(void *pdata)
{
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_TEST_IO(void *pdata)
{
    // BYTE *p;
    // struct BASE_MSG *pmsg;
    // p=(BYTE *)pdata+sizeof(struct MSG_REV_TYPE);
    // pmsg=(struct BASE_MSG *)((uint8 *)pdata+sizeof(struct MSG_REV_TYPE));
    // if(pmsg->device==MSG_DEV_ALM) //36
    // {
    if (IO_in(IN_ALARM1))
    {
        IO_Out(OUT_ALARM1, FALSE);
    }
    else
    {
        IO_Out(OUT_ALARM1, TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("报警输入1,输出1:有效");
    }

    if (IO_in(IN_ALARM2))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IO_Out(OUT_ALARM2, FALSE);
        //IP_printf("报警输入1,输出1:无效");
    }
    else
    {
        IO_Out(OUT_ALARM2, TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("报警输入2,输出2:有效");
    }

    if (IO_in(IN_ALARM3))
    {
        IO_Out(OUT_ALARM3, FALSE);
        //IP_printf("报警输入1,输出1:无效");
    }
    else
    {
        IO_Out(OUT_ALARM3, TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("报警输入3,输出3:有效");
    }

    if (IO_in(IN_ALARM4))
    {
        IO_Out(OUT_DCS, FALSE);
        //IP_printf("报警输入1,输出1:无效");
    }
    else
    {
        IO_Out(OUT_DCS, TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("报警输入4,输出电磁锁:有效");
    }

    if (IO_in(IN_ALARM5))
    {
        //IO_Out(OUT_DCS,FALSE);
        //IP_printf("报警输入1,输出1:无效");
    }
    else
    {
        IO_Out(OUT_ALARM1, TRUE);
        //IO_Out(OUT_ALARM2,TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("报警输入5,输出1:有效");
    }

    if (IO_in(IN_ALARM6))
    {
        //IO_Out(OUT_DCS,FALSE);
        //IP_printf("报警输入1,输出1:无效");
    }
    else
    {
        //IO_Out(OUT_ALARM3,TRUE);
        IO_Out(OUT_ALARM2, TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("报警输入6,输出2:有效");
    }

    if (IO_in(IN_PC1POWER))
    {
        IO_Out(OUT_PC1POWER, FALSE);
        //IP_printf("报警输入1,输出1:无效");
    }
    else
    {
        IO_Out(OUT_PC1POWER, TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("计算机1检测,开机有效");
    }

    if (IO_in(IN_PC2POWER))
    {
        IO_Out(OUT_PC2POWER, FALSE);
        //IP_printf("报警输入1,输出1:无效");
    }
    else
    {
        IO_Out(OUT_PC2POWER, TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("计算机2检测,开机有效");
    }

    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_IO_TEST(void *pdata)
{
    uint8 tyjflag = 0, tmp;
    uint8 gTestIOFlag = 0;

    if (IO_in(IN_ALARM1))gTestIOFlag |= TEST_ALARM1;
    else	gTestIOFlag &= ~TEST_ALARM1;
    if (IO_in(IN_ALARM2))gTestIOFlag |= TEST_ALARM2;
    else	gTestIOFlag &= ~TEST_ALARM2;
    if (IO_in(IN_ALARM3))gTestIOFlag |= TEST_ALARM3;
    else	gTestIOFlag &= ~TEST_ALARM3;
    if (IO_in(IN_ALARM4))gTestIOFlag |= TEST_ALARM4;
    else	gTestIOFlag &= ~TEST_ALARM4;
    if (IO_in(IN_ALARM5))gTestIOFlag |= TEST_ALARM5;
    else	gTestIOFlag &= ~TEST_ALARM5;
    if (IO_in(IN_ALARM6))gTestIOFlag |= TEST_ALARM6;
    else	gTestIOFlag &= ~TEST_ALARM6;
    if (IO_in(IN_PC1POWER))gTestIOFlag |= TEST_PC1POWER;
    else	gTestIOFlag &= TEST_PC1POWER;
    if (IO_in(IN_PC2POWER))gTestIOFlag |= TEST_PC2POWER;
    else	gTestIOFlag &= TEST_PC2POWER;

    if (IO_in(IN_TYJVGA))tyjflag = 0;
    else	tyjflag = 1;

    IP_printf("报警输入口测试:0x%2x,投影机报警%d", gTestIOFlag, tyjflag);
    PINSEL3 = 0x00f00005;
    for(tmp = 0; tmp < 5; tmp++)
    {

        IO_Out(OUT_HW, FALSE); // 输出低
        IO_Out(OUT_ALARM1, TRUE);
        IO_Out(OUT_ALARM2, TRUE);
        IO_Out(OUT_ALARM3, TRUE);
        IO_Out(OUT_DCS, TRUE);
        IO_Out(OUT_PC1POWER, TRUE);
        IO_Out(OUT_PC2POWER, TRUE);

        OSTimeDly(50);
        IO_Out(OUT_HW, TRUE); // 输出高
        IO_Out(OUT_ALARM1, FALSE);
        IO_Out(OUT_ALARM2, FALSE);
        IO_Out(OUT_ALARM3, FALSE);
        IO_Out(OUT_DCS, FALSE);
        IO_Out(OUT_PC1POWER, FALSE);
        IO_Out(OUT_PC2POWER, FALSE);
        OSTimeDly(50);

    }
    PINSEL3 = 0x00fc0005;


    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////

uint32 On_MSG_OPTION_LEDContrl(void *pdata)
{
    BYTE *p;
    uint32	leddata = 0;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    leddata = (*(p + 2) << 24) + (*(p + 3) << 16) + (*(p + 4) << 8) + (*(p + 5));
    LED_Set(leddata, *(p + 6));
    LED_SendFLASH();
    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_YY_Teather(void *pdata)
{
    struct BASE_MSG *pmsg;
    uint8 *p;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    pmsg = (struct BASE_MSG *)p;
    if (pmsg->option == MSG_OPTION_YY_Teather)
    {
        LED_Set(LED_KZA, LED_DISPLAY_ON);
        Key_TB_state(KEY_REP_JXZX, TRUE);
        LED_SendFLASH();
    }
    else
    {
        LED_Set(LED_KZA, LED_DISPLAY_OFF);
        Key_TB_state(KEY_REP_JXZX, FALSE);
        LED_SendFLASH();
    }
    return TRUE;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_TYJ_DIS_VGA(void *pdata)
{

    On_MSG_OPTION(pdata);
    return FALSE;

}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_CardContrl(void *pdata)
{
    uint8 buf[32], *p, tmp, *ip;
    struct BASE_MSG *pmsg;
    struct MSG_REV_TYPE Rev;

#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif

    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Rev.Type = DEV_XCOM_RF;
    //没有配置RF开机
    if ( Check_FunctionMode(Device_FunctionMode_CARD) != 1) return     0;

    if (Get_Class_Stauts() != CLASS_OVER)
    {
        //已经是开机状态
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        Task_Rec_Del(buf, 2);
        return  0;
    }

    p = ((uint8 *)pdata + sizeof(struct MSG_REV_TYPE ));
    pmsg = (struct BASE_MSG *)p;
    switch(*(p + 2))
    {
    case RF_ATT_ERR://认证未通过
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        Task_Rec_Del(buf, 2);
        //已经下课，则关机
        if (Get_Class_Stauts() == CLASS_OVER) break;
        //其他方式开机，则不关闭
        if (Get_Class_Stauts() != CLASS_BEGIN_RF) break;
        //没有配置RF开机
        if ( Check_FunctionMode(Device_FunctionMode_CARD) != 1) break;
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_OVER;
        Msg_send(buf, 3, (uint8 *)&Rev);
        IP_printf("RF认证未通过 读卡消息，卡号:%s", p + 3);
        break;
    case RF_ATT_REQ://认证请求
        //已经上课则取消认证请求
        if (Get_Class_Stauts() != CLASS_OVER) break;
        Mem_Set(buf, 0, 32);
        buf[0] = pmsg->device;
        buf[1] = pmsg->option;
        buf[2] = RF_ATT_REQ;
        for (tmp = 0; tmp < 28; tmp++)
        {
            buf[3 + tmp] = *(p + 3 + tmp);
            if (*(p + 2 + tmp) == 0) break;
        }
        ip = Get_DesIp(DEVICE_SUB_SER);
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
        }
        UDP_Send_CMD(buf, 32, ip);
        Mem_Clr(buf, 12);
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        buf[2] = 2;
        //队列中加入开机命令，如5秒没有收到认证命令自动开机
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 5, buf, 3, 0, (uint8 *)&Rev);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("RF 读卡消息，卡号:%s", p + 3);
        break;
    case RF_ATT_OK: 		//认证通过
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        Task_Rec_Del(buf, 2);	//队列中删除开机命令
        //已经上课则取消2次开机
        if (Get_Class_Stauts() != CLASS_OVER) break;
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        buf[2] = 2;
        Msg_send(buf, 3, (uint8 *)&Rev);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("RF认证通过 读卡消息，卡号:%s", p + 3);
        break;
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 on_MSG_OPTION_TB_ReqStatus(void *pdata)
{

    TB_Read_Csingle();
    TB_Read_state();
    return 0;

}



#include "includes.h"

const char power_ostr[][10] =
{
    "打开上升",
    "下降",
    "关闭停止",
};
const char power_str[][10] =
{
    "设备",
    "流媒体",
    "展台",
    "显示器",
    "计算机",
    "投影机",
    "灯光",
    "功放",
    "摄象机",
    "投影幕",
    "窗帘",
    "液晶",
    "吊架",
};
extern struct KEY_TYPE KEY_DATA;
uint32 POWER_FLAG, POWER_FLAG_STATE;
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void POWER_init(void)
{
    uint8 buf[10], err, lg, time;
    IO_Out(OUT_POWER_M3, 0);
    IO_Out(OUT_POWER_M4, 0);
    for (time = 0; time < 10; time++)
    {
        buf[0] = 0;
        lg = 1;
        IO_FlagLingt_Flash();
        err = XCOM_Data_Read(DEV_XCOM_MB, 0, XCOM_CMD_GET_INFO, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
        if (err != OS_NO_ERR)
        {
            UART_printf("User Power Error  Times:%d\n", time);
            IP_printf("电源箱自检错误");
        }
        else
        {
            UART_printf("User Power OK Ver=%d.%d\n", buf[4], buf[5]);
            IP_printf("电源箱自检通过，版本:%d.%d", buf[4], buf[5]);
            Power_Beep(200);
            //			OSTimeDlyHMSM(0,0, 1,0);
            //			Power_Beep(200);
            break;
        }
        IO_FlagLingt_Flash();
    }
    POWER_FLAG = 0;
    POWER_FLAG_STATE = 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：监控球机转到全景(预置位1)
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void CameraInit( BYTE id)
{
    BYTE buf[18] = {0};
    /*buf[0] = 0;
    buf[1] = 1;
    buf[2] = 2;
    buf[3] = 3;
    buf[4] = 4;
    buf[5] = 5;
    buf[9] = MSG_OPTION_SCAMERA_PREPOSITION;
    buf[10] = 1;
    buf[14] =buf[16]= 0xa6;
    buf[15] = buf[17] = 0xa5;
    On_MSG_OPTION_SCAMERA( &buf[0] );*/
    BYTE devid, *pBuf;
    pBuf = (uint8 *) Get_ClassInfo_Table(0);
    devid = *(pBuf + 166);//读出监控设备ID

    if (id != devid)
        return;

    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_SCAMERA_PREPOSITION;
    buf[2] = 1;
    //buf[3]=option;
    buf[6] = buf[8] = 0xa6;
    buf[7] = buf[9] = 0xa5;
    buf[17] = DEV_XCOM_SELF;
    Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 30, &buf[0], 10, 0, &buf[17]);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：POWER_Set
//	函数功能：控制一体机高级电源箱
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void POWER_Set(uint8 contrl_io, uint8 option)
{
    struct DEVICE_STATUS_TYPE *ps;
    uint8 buf[5], sub, port, err, lg;
    uint16 time;

    if (contrl_io == 0)
        return;
    ps = ( struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    sub = contrl_io / 16;
    port = contrl_io % 16;

    if (ps->Power_Type == 2)
        time = DEV_XCOM_RPTIME;
    else
        time = 1;
    do
    {
        buf[0] = POWER_CHANLE_CONTRL;
        buf[1] = port;
        buf[2] = option;
        lg = 3;
        err = XCOM_Data_Read(DEV_XCOM_MB, sub, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
        if (err != OS_NO_ERR)
        {
            UART_printf("Power Error\n");
            OSTimeDlyHMSM(0, 0, 0, 1000);
        }
        else
            break;
    }
    while(time--);

    ps->power_state[sub] &= ~(3 << port);
    ps->power_state[sub] |= option << port;

    Send_State();
    if (Get_Debug(DEV_DEBUG_POWER))
        IP_printf("电源箱操作:%s设备:%s编号:%d", &power_ostr[option - 1][0], &power_str[contrl_io - 1][0], contrl_io - 1);

}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void SetPowerDeltimemsg(BYTE device, BYTE option, BYTE time, uint8 repeat)
{
    BYTE buf[4], rev_buf[10];
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_POWERCARD_ONOFF;
    buf[2] = device;
    buf[3] = option;
    rev_buf[0] = DEV_XCOM_SELF;
    Task_Run_Delay(REALTIME_CIIR_SEC * 256 + time, buf, 4, repeat, rev_buf);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void DelPowerDeltimemsg(BYTE device, BYTE option, BYTE lg)
{
    BYTE buf[4];
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_POWERCARD_ONOFF;
    buf[2] = device;
    buf[3] = option;
    Task_Rec_Del(buf, lg);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void IO_PC_Power(BYTE device, BYTE option)
{
    //BYTE buf[8];
    if (device == MSG_DEV_PC)
    {
        IO_Out(OUT_PC1POWER, 1);
        OSTimeDly(150);
        if  (Get_Debug(DEV_DEBUG_POWER))
            IP_printf("课件计算机软开机");
        IO_Out(OUT_PC1POWER, 0);
    }
    else if (device == MSG_DEV_LMT)
    {
        IO_Out(OUT_PC2POWER, 1);
        OSTimeDly(150);
        if  (Get_Debug(DEV_DEBUG_POWER))
            IP_printf("流媒体计算机软开机");
        IO_Out(OUT_PC2POWER, 0);
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
uint32 ON_POWER_option(void *pdata)
{
    uint8 *p, time, buf[10], rev_buf[12];
    struct DEVICE_INFO *pdev;
    struct BASE_MSG msg, *pmsg;
    struct DEVICE_STATUS_TYPE *ps;
    struct KEY_TYPE *pkey;

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    msg.device = *p;
    msg.option = 0;
    time = *(p + 2);
    pmsg = (struct BASE_MSG *)p;
    pkey = (struct KEY_TYPE *)Get_KeyData();
    rev_buf[0] = DEV_XCOM_SELF;
    pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)&msg, DEV_FIND_TYPE);
    switch(pmsg->device)
    {
    case MSG_DEV_PC://课件PC
        if (POWER_FLAG_STATE & POWER_FLAG_PC)
            break;
        //zhw add 2009-09-07
        if  (pmsg->option == MSG_OPTION_POWEROPEN)
        {
            if  (!IO_in(IN_PC1POWER)) //开机状态
            {
                LED_Set(LED_PC_POWER, LED_DISPLAY_ON); //更改led
                LED_SendFLASH();

                Key_TB_state(KEY_REP_PC_POWER, TRUE);
                ps->DEV_Link_State |= DEV_LINK_PCON;
                //开机成功，开始统计时间
                Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_START);
                return STATUS_CHANGE;
            }
        }
        else
        {
            if  (IO_in(IN_PC1POWER)) //关机状态
            {
                LED_Set(LED_PC_POWER, LED_DISPLAY_OFF); //更改led
                LED_SendFLASH();

                Key_TB_state(KEY_REP_PC_POWER, FALSE);
                ps->DEV_Link_State &= ~DEV_LINK_PCON;
                //关机成功，开始统计使用时间
                Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_END);
                return STATUS_CHANGE;
            }
        }
        //zhw add end
        POWER_FLAG_STATE |= POWER_FLAG_PC;
        LED_Set(LED_PC_POWER, LED_DISPLAY_FLASH);
        if (pmsg->option == MSG_OPTION_POWEROPEN)
        {
            POWER_Set(DEV_POWER_PC, pmsg->option);
            POWER_Set(DEV_POWER_CRT, pmsg->option);
            time = 10;
        }
        else
        {
            time = DEV_TIME_PC;
        }
        IO_PC_Power(pmsg->device, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, time, DEV_REPEAT_TIMES_PC);
        break;
    case MSG_DEV_LMT://流媒体PC
        //	if (POWER_FLAG_STATE & POWER_FLAG_LMT)
        //		break;
        POWER_FLAG_STATE |= POWER_FLAG_LMT;
        /*
        		if (pmsg->option == MSG_OPTION_POWEROPEN){
        		//	time = 10;
        		//	POWER_Set(DEV_POWER_LMTPC,pmsg->option);

        		}
        		else{
        			time=DEV_TIME_LMTPC;
        			Device_Stat_Option(MSG_DEV_LMT,MSG_OPTION_STAT_END);

        		}		*/
        LMT_Power_contrl(MSG_DEV_LMT, pmsg->option);
        time = 1;
        SetPowerDeltimemsg(pmsg->device, pmsg->option, time, DEV_REPEAT_TIMES_LMTPC);
        break;
    case MSG_DEV_TYJA://投影机A
        if (POWER_FLAG_STATE & POWER_FLAG_TYJA)
            break;
        POWER_FLAG_STATE |= POWER_FLAG_TYJA;
        goto loop2;
    case MSG_DEV_TYJB://投影机B
        if (POWER_FLAG_STATE & POWER_FLAG_TYJB)
            break;
        POWER_FLAG_STATE |= POWER_FLAG_TYJB;
loop2:
        LED_Set(LED_TYJ_POWER, LED_DISPLAY_FLASH);
        buf[0] = MSG_DEV_TYM;
        buf[1] = 0;
        //pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)buf,DEV_FIND_TYPE);
        if (pmsg->option == MSG_OPTION_POWERCLOSE) //发送投影机关机
        {
            time = DEV_TIME_TYJ;
            SetPowerDeltimemsg(pmsg->device, pmsg->option, time, 0);
            pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)buf, DEV_FIND_TYPE);
            if (pdev != NULL)
            {
                if (pdev->prev_val)
                {
                    buf[0] = MSG_DEV_TYM;
                    buf[1] = MSG_OPTION_POWEROPEN;
                    buf[2] = 0;
                    Msg_send(buf, 2, rev_buf);
                }
            }
        }
        else
        {
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
            else
                POWER_Set(DEV_POWER_TYJ, pmsg->option);
            SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
            pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)buf, DEV_FIND_TYPE);
            if (pdev != NULL)
            {
                if (pdev->prev_val)
                {
                    buf[0] = MSG_DEV_TYM;
                    buf[1] = MSG_OPTION_POWERDOWN;
                    Msg_send(buf, 2, rev_buf);
                }
            }
        }
        break;
    case MSG_DEV_GF://功放
        if (pmsg->option == MSG_OPTION_POWEROPEN)
        {
            LED_Set(LED_GF_POWER, LED_DISPLAY_ON);
            ps->DEV_Link_State |= DEV_LINK_GFON;
            KEY_DATA.key_rep |= KEY_REP_GF_POWER;
            Key_TB_state(KEY_REP_GF_POWER, TRUE);
        }
        else
        {
            LED_Set(LED_GF_POWER, LED_DISPLAY_OFF);
            ps->DEV_Link_State &= ~DEV_LINK_GFON;
        }
        //关闭功放音量
        if (pdev != NULL)
            POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        else
            POWER_Set(DEV_POWER_GF, pmsg->option);

        Device_Stat_Option(MSG_DEV_GF, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_GF, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_LIGHT://室内灯光
        if (pmsg->option == MSG_OPTION_POWEROPEN)
        {
            LED_Set(LED_LIGHT_POWER, LED_DISPLAY_ON);
            ps->DEV_Link_State |= DEV_LINK_LIGHT;

            //zjj add6
            KEY_DATA.key_rep |= KEY_REP_LIGHT_POWER;
            Key_TB_state(KEY_REP_LIGHT_POWER, TRUE);
        }
        else
        {
            LED_Set(LED_LIGHT_POWER, LED_DISPLAY_OFF);
            ps->DEV_Link_State &= ~DEV_LINK_LIGHT;
        }
        if (pdev != NULL)
            POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        else
            POWER_Set(DEV_POWER_LIGHT, pmsg->option);

        Device_Stat_Option(MSG_DEV_LIGHT, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_LIGHT, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_TYM://投影幕
        DelPowerDeltimemsg(MSG_DEV_TYM, 0, 3);
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            if (pkey->POWER_DEV == MSG_DEV_TYM)
                LED_Set(LED_TYM, LED_DISPLAY_ON);
            else
                LED_Set(LED_TYM, LED_DISPLAY_OFF);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
            else
                POWER_Set(DEV_POWER_TYM, pmsg->option);
            break;
        }
        LED_Set(LED_TYM, LED_DISPLAY_FLASH);
        if (pdev != NULL)
        {
            if ((pdev->OUT.out_no != DEV_IO_NONE) && (pdev->POWER.power_no == DEV_POWER_NONE))
            {
                if (pmsg->option == MSG_OPTION_POWERDOWN)
                    IO_Contrl(pdev->OUT.out_no, TRUE);
                else
                    IO_Contrl(pdev->OUT.out_no, FALSE);
            }
            else
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        }
        else
            POWER_Set(DEV_POWER_TYM, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
        break;
    case MSG_DEV_PELMET://窗帘
        DelPowerDeltimemsg(MSG_DEV_PELMET, 0, 3);
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            if (pkey->POWER_DEV == MSG_DEV_PELMET)
                LED_Set(LED_PELMET_POWER, LED_DISPLAY_ON);
            else
                LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, MSG_OPTION_POWERCLOSE);
            else
                POWER_Set(DEV_POWER_CL, MSG_OPTION_POWERCLOSE);
            break;
        }
        LED_Set(LED_PELMET_POWER, LED_DISPLAY_FLASH);
        if (pdev != NULL)
        {
            if ((pdev->OUT.out_no != DEV_IO_NONE) && (pdev->POWER.power_no == DEV_POWER_NONE))
            {
                if (pmsg->option == MSG_OPTION_POWERDOWN)
                    IO_Contrl(pdev->OUT.out_no, TRUE);
                else
                    IO_Contrl(pdev->OUT.out_no, FALSE);
            }
            else
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        }
        else
            POWER_Set(DEV_POWER_CL, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
        break;
    case MSG_DEV_LCD://液晶升降
        DelPowerDeltimemsg(MSG_DEV_LCD, 0, 3);
        LED_Set(LED_LCD_UP, LED_DISPLAY_FLASH);
        if (pdev != NULL)
        {
            if ((pdev->OUT.out_no != DEV_IO_NONE) && (pdev->POWER.power_no == DEV_POWER_NONE))
            {
                if (pmsg->option == MSG_OPTION_POWERDOWN)
                    IO_Contrl(pdev->OUT.out_no, TRUE);
                else
                    IO_Contrl(pdev->OUT.out_no, FALSE);
            }
            else
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        }
        else
            POWER_Set(DEV_POWER_LCD, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            if (pkey->POWER_DEV == MSG_DEV_LCD)
                LED_Set(LED_LCD_UP, LED_DISPLAY_ON);
            else
                LED_Set(LED_LCD_UP, LED_DISPLAY_OFF);

            if (pdev != NULL)
            {
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, MSG_OPTION_POWERCLOSE);
            }
            else
                POWER_Set(DEV_POWER_LCD, MSG_OPTION_POWERCLOSE);
            break;
        }
        break;
    case MSG_DEV_TYJUP://投影机吊架
        DelPowerDeltimemsg(MSG_DEV_TYJUP, 0, 3);
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            LED_Set(LED_TYDJ, LED_DISPLAY_ON);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, MSG_OPTION_POWERCLOSE);
            else
                POWER_Set(DEV_POWER_TYJUP_DOWN, MSG_OPTION_POWERCLOSE);
            break;
        }
        LED_Set(LED_TYDJ, LED_DISPLAY_FLASH);
        if (pdev != NULL)
        {
            if ((pdev->OUT.out_no != DEV_IO_NONE) && (pdev->POWER.power_no == DEV_POWER_NONE))
            {
                if (pmsg->option == MSG_OPTION_POWERDOWN)
                    IO_Contrl(pdev->OUT.out_no, TRUE);
                else
                    IO_Contrl(pdev->OUT.out_no, FALSE);
            }
            else
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        }
        else
            POWER_Set(DEV_POWER_TYJUP_DOWN, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
        break;
    case MSG_DEV_SCAMERA1://板书摄象机1
    case MSG_DEV_SCAMERA2://板书摄象机2
    case MSG_DEV_SCAMERA3://板书摄象机3
    case MSG_DEV_SQCAMERA://学生球机
    case MSG_DEV_TQCAMERA://教师球机
    case MSG_DEV_BQCAMERA://备用球机
        if (pmsg->option == MSG_OPTION_POWEROPEN)
            Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_START);
        else
            Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_END);
        if (pdev != NULL)
            POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        else
            POWER_Set(DEV_POWER_SXJ, pmsg->option);

        CameraInit(pmsg->device);
        break;
    case MSG_DEV_DCS://电磁锁
        Device_DCS_open();
        break;
    default:
        if (pmsg->option == MSG_OPTION_POWEROPEN)
            Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_START);
        else
            Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_END);

        if (pdev != NULL)
            POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        else
            POWER_Set(DEV_POWER_DEVICE, pmsg->option);
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
uint32 On_MSG_OPTION_POWERCARD_ONOFF(void *pdata)
{
    uint8 tmp, buf[20];
    struct DEVICE_STATUS_TYPE *ps;
    struct BASE_MSG *pmsg;
    struct DEVICE_INFO *pdev;
    struct KEY_TYPE *pkey;

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    pkey = (struct KEY_TYPE *)Get_KeyData();
    pmsg = (struct BASE_MSG *)((BYTE *)pdata + sizeof(struct BASE_MSG) + sizeof(struct MSG_REV_TYPE));
    pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg, DEV_FIND_TYPE);
    switch(pmsg->device)
    {
    case MSG_DEV_PC://课件PC
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_END);
            if (!IO_in(IN_PC1POWER))
            {
                //关闭没有完成,重复关机
                IO_PC_Power(pmsg->device, pmsg->option);
                if (0xff != Task_Rec_Find((BYTE *)pdata + sizeof(struct MSG_REV_TYPE), 4))return FALSE;
            }
            LED_Set(LED_PC_POWER, LED_DISPLAY_OFF); //更改led
            pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg, DEV_FIND_TYPE);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
            else
                POWER_Set(DEV_POWER_PC, pmsg->option);

            POWER_Set(DEV_POWER_CRT, pmsg->option);
            Key_TB_state(KEY_REP_PC_POWER, FALSE);
            ps->DEV_Link_State &= ~DEV_LINK_PCON;
            Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_END);
        }
        else
        {
            if (IO_in(IN_PC1POWER))
            {
                //开机没有完成，重复操作
                IO_PC_Power(pmsg->device, pmsg->option);
                tmp = Task_Rec_Find((BYTE *)pdata + sizeof(struct MSG_REV_TYPE), 4);
                if (tmp != 0xff)
                {
                    if (Get_Debug(DEV_DEBUG_POWER))
                        IP_printf("课件计算机软开机失败!重试:%d次", tmp);

                    return FALSE;
                }
                LED_Set(LED_PC_POWER, LED_DISPLAY_OFF); //更改led
                Key_TB_state(KEY_REP_PC_POWER, FALSE);
                ps->DEV_Link_State &= ~DEV_LINK_PCON;
            }
            else
            {
                LED_Set(LED_PC_POWER, LED_DISPLAY_ON); //更改led
                Key_TB_state(KEY_REP_PC_POWER, TRUE);
                ps->DEV_Link_State |= DEV_LINK_PCON;
                //开机成功，开始统计时间
                Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_START);
            }
        }
        DelPowerDeltimemsg(MSG_DEV_PC, 0, 3);
        POWER_FLAG_STATE &= ~POWER_FLAG_PC;
        break;
    case MSG_DEV_LMT://流媒体PC
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {

            /*
            pdev=(struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg,DEV_FIND_TYPE);
            if (pdev != NULL)
            	POWER_Set(pdev->POWER.dev_id*16+pdev->POWER.power_no, pmsg->option);
            else
            	POWER_Set(DEV_POWER_LMTPC,pmsg->option);
            if (!IO_in(IN_PC2POWER)){
            	IO_PC_Power(pmsg->device, pmsg->option);
            	if (0xff != Task_Rec_Find((BYTE *)pdata+sizeof(struct MSG_REV_TYPE),4))
            		return FALSE;
            }
            */
            ps->DEV_Link_State &= ~DEV_LINK_LMTON;
            Device_Stat_Option(MSG_DEV_LMT, MSG_OPTION_STAT_END);
        }
        else
        {
            /*
            	pdev=(struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg,DEV_FIND_TYPE);
            	if (pdev != NULL)
            		POWER_Set(pdev->POWER.dev_id*16+pdev->POWER.power_no, pmsg->option);
            	else
            		POWER_Set(DEV_POWER_LMTPC,pmsg->option);
            	LMT_Power_contrl(MSG_DEV_LMT,MSG_OPTION_POWEROPEN);
            	*/

            /*
            if (IO_in(IN_PC2POWER)){
            	tmp = Task_Rec_Find((BYTE *)pdata+sizeof(struct MSG_REV_TYPE),4);
            	if (tmp != 0xff){
            		if (Get_Debug(DEV_DEBUG_POWER))
            			IP_printf("流媒体计算机软开机失败!重试:%d次",tmp);
            	 	IO_PC_Power(pmsg->device,pmsg->option);
            		return 0;;
            	}
            	else
            		ps->DEV_Link_State &= ~DEV_LINK_LMTON;
            }
            else{
            	ps->DEV_Link_State |= DEV_LINK_LMTON;
             	Device_Stat_Option(MSG_DEV_LMT, MSG_OPTION_STAT_START);
            }
            */
        }
        DelPowerDeltimemsg(MSG_DEV_LMT, 0, 3);
        POWER_FLAG_STATE &= ~POWER_FLAG_LMT;
        break;
    case MSG_DEV_TYJA://投影机A
        POWER_FLAG_STATE &= ~POWER_FLAG_TYJA;
        goto loop1;
    case MSG_DEV_TYJB://投影机B
        POWER_FLAG_STATE &= ~POWER_FLAG_TYJB;
loop1:
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            LED_Set(LED_TYJ_POWER, LED_DISPLAY_OFF);//更改led
            Key_TB_state(KEY_REP_TYJ_POWER, FALSE);
            pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg, DEV_FIND_TYPE);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
            else
                POWER_Set(DEV_POWER_TYJ, pmsg->option);
            ps->DEV_Link_State &= ~DEV_LINK_TYJON;
            Device_Stat_Option(MSG_DEV_TYJA, MSG_OPTION_STAT_END);
        }
        else
        {
            LED_Set(LED_TYJ_POWER, LED_DISPLAY_ON);//更改led
            Device_Stat_Option(MSG_DEV_TYJA, MSG_OPTION_STAT_START);
            Key_TB_state(KEY_REP_TYJ_POWER, TRUE);
            if (0 == (ps->DEV_Link_State & DEV_LINK_TYJON))
            {
                //发送投影机开命令
                ps->DEV_Link_State |= DEV_LINK_TYJON;
                tmp = sizeof(struct MSG_REV_TYPE);
                buf[tmp] = MSG_DEV_TYJA;
                buf[tmp + 1] = MSG_OPTION_POWEROPEN;
                //On_MSG_Power_OPTION((void *)buf);
                On_MSG_RepeatPower_OPTION((void *)buf);
                //buf[tmp]=MSG_DEV_TYJB;
                //buf[tmp+1]=MSG_OPTION_POWEROPEN;
                //On_MSG_Power_OPTION((void *)buf);
                //On_MSG_RepeatPower_OPTION((void *)buf);
            }
        }
        break;
    case MSG_DEV_TYM://投影幕
        if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
            LED_Set(LED_TYM, LED_DISPLAY_OFF); //更改led
        else if (pkey->POWER_DEV == MSG_DEV_TYM)
            LED_Set(LED_TYM, LED_DISPLAY_ON); //更改led
        else
            LED_Set(LED_TYM, LED_DISPLAY_OFF); //更改led
        Device_Stat_Option(MSG_DEV_TYM, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_TYM, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_PELMET://窗帘
        if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
            LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF); //更改led
        else if (pkey->POWER_DEV == MSG_DEV_PELMET)
            LED_Set(LED_PELMET_POWER, LED_DISPLAY_ON); //更改led
        else
            LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF); //更改led
        Device_Stat_Option(MSG_DEV_PELMET, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_PELMET, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_TYJUP://投影机吊架
        if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
            LED_Set(LED_TYDJ, LED_DISPLAY_OFF); //更改led
        else if (pkey->POWER_DEV == MSG_DEV_TYJUP)
            LED_Set(LED_TYDJ, LED_DISPLAY_ON); //更改led
        else
            LED_Set(LED_TYDJ, LED_DISPLAY_OFF); //更改led
        Device_Stat_Option(MSG_DEV_TYJUP, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_TYJUP, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_LCD: //液晶升降
        if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
            LED_Set(LED_LCD_UP, LED_DISPLAY_OFF); //更改led
        else if (pkey->POWER_DEV == MSG_DEV_LCD)
            LED_Set(LED_LCD_UP, LED_DISPLAY_ON); //更改led
        else
            LED_Set(LED_LCD_UP, LED_DISPLAY_OFF); //更改led
        Device_Stat_Option(MSG_DEV_LCD, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_LCD, MSG_OPTION_STAT_END);
        break;
    };
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
void POWER_msg_rev(uint8 *pdata, uint8 sub)
{
    uint8 chanle;
    struct DEVICE_STATUS_TYPE *ps;
    chanle = *pdata;

    if (chanle == POWER_CHANLE_VER)
    {
        ps = ( struct DEVICE_STATUS_TYPE *)Get_Device_Status();
        ps->power_state[sub] |= 0x80000000;
    }
    else if (chanle == POWER_CHANLE_CONTRL)
    {
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
void Power_Beep(uint16 DelayTime)
{

    uint8 buf[10], err, lg;
    buf[0] = POWER_CHANLE_SET;
    buf[1] = 0x03;
    lg = 2;
    err = XCOM_Data_Read(DEV_XCOM_MB, 0, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
    if (err != OS_NO_ERR)
        UART_printf("Power  Contr Beep Error !\n");
    OSTimeDlyHMSM(0, 0, 0, DelayTime);
    buf[0] = POWER_CHANLE_SET;
    buf[1] = 0x1;
    err = XCOM_Data_Read(DEV_XCOM_MB, 0, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
    if (err != OS_NO_ERR)
        UART_printf("Power  Contr Beep Error !\n");

}

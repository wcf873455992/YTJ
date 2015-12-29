#include "includes.h"
#include "Os_api.h"
#include "ARP.H"

#define LSZH_VER		0x0305//v3.05
//2011.3.8		0x020d     v2.13调试双工对讲
//2011.3.14		0x020E     v2.14RF卡开机
//2011.3.17            0x020f       V2.15  调整看门狗复位
//2011.3.26            0x0210      V2.16   手动时刻表开机有效
//2011.4.16            0x0212      V2.18   定位器录制时打开
//2011.5.5            0x0213      V2.19   流媒体磁盘容量修改
//2011.5.17           0x0214      V2.20   切换设备跟随面板声音同步，投影机开机码延迟10秒发送
//2011.5.20           0x0215      V2.21   南农专用，新增开关机优先级
//2011.6.15           0x0216      V2.22   南农专用，新增双球机控制
//2011.7.12           0x0217      V2.23   新增球机图像识别模块
//2011.8.24           0x0218      V2.24   新增对索尼VISCA协议的支持
//2012.2.28           0x0219      V2.25   放开投影机电源控制
//2012.3.20           0x021a      V2.26   新增指纹识别开机

//2012.5.30           0x0300      V3.00   新版一体机  兼容2.26一体机功能
//2012.6.25                               兼容一体机2.28功能 修正visca协议zoom不能修改速度问题
//2012.7.16                               VGA切换行场幅值调大一倍
//2012.7.27                               新一体机增加232球机控制串口，修正启用球机串口号
//                                        (pdev->COMM.val.xcom.PortNum)项后老工具配置引起的数据异常
//2012.8.20                               一体机添加刷卡开机功能
//2012.9.18           0x0301      V3.01   修正刷卡开机验证bug
//2012.10.22          0x0302      V3.02   修正232球机串口返回处理错误
//2012.11.30          0x0303      V3.03   修正南农刷卡开机缓冲区溢出问题
//2013.2.28           0x0305      V3.05   解决周日自动开机和程序无法升级问题，user_table.c和I2C1INT.c文件

OS_EVENT *MSG_PQ;			//消息队列
void *MSG_Q[MSG_MAX_SIZE];	//消息队列
const  unsigned char def_classname[] = "卓越测试教室";
const  unsigned char def_schoolname[] = "卓越电子有限公司";
const  unsigned char def_housename[] = "设备记录空";
const  uint8 def_ip[4] = {192, 168, 7, 101};
const  uint8 def_mark[4] = {255, 255, 0, 0};
const  uint8 def_gateip[4] = {192, 168, 0, 190};

struct DEVICE_STATUS_TYPE gDeviceStatus;
struct DEVICE_REC_TYPE gDeviceTable;
struct TASK_TYPE TASK_TABLE;
struct DEVICE_STAT_USE_TYPE gStat_Rec;
struct DEVICE_DCS_TYPE gDCS;
extern struct LINK_REC_TABLE_TYPE DEVICE_LINK;


uint8 Alarmdev_id[9];

extern uint8 Encode_TAB[8];
const uint32 IO_IN_TAB[9] =
{
    0,
    IN_ALARM1,
    IN_ALARM2,
    IN_ALARM3,
    IN_ALARM4,
    IN_ALARM5,
    IN_ALARM6,
    IN_PC1POWER,
    IN_PC2POWER,
};
unsigned char DCS_TYPE_Str[] = "OC3901L";
extern void Reset(void *pdata);
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint16 Get_ver(void)
{
    return LSZH_VER;

}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：1
////////////////////////////////////////////////////////////////////
uint8 Get_Class_Stauts(void)
{
    return gDeviceStatus.DEV_state & 0x0f;

}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_DCS_init(void)
{
    memset((BYTE *)&gDCS, 0, sizeof(struct DEVICE_DCS_TYPE));
    IO_Out(OUT_DCS, FALSE);
    gDCS.Contrl_State &= ~DCS_CONTRL_ON_OFF;



}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Device_DCS_Check_in(uint8 val)
{
    if (gDCS.Type != DCS_LCJ_OC39 ) return 1;
    if (gDCS.check_state_in == val ) return 0;
    else
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
void Device_DCS_Contrl(void)
{
    uint8 buf[10];
    if (gDCS.Type == DCS_NONE) return;
    if((gDCS.Type == DCS_LCJ_OC39) && (gDCS.check_state_in == 0)) return;
    if (gDCS.Type == DCS_LCJ_OC39)
    {
        if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER) //下课
        {
            //下课状态，锁关闭
            if (gDCS.Contrl_State & DCS_CONTRL_ON_OFF)
            {
                IO_Out(OUT_DCS, FALSE);
                Device_Link_Scan();
                gDCS.Contrl_State &= ~DCS_CONTRL_ON_OFF;
            };
            //下课状态，机箱关闭，设置机箱状态
            if (IO_in(IO_IN_TAB[gDCS.check_state_in]) == FALSE)
            {
                gDCS.Contrl_State &= ~DCS_JX_OPEN;
            }
        }
        else
        {
            // 上课状态，机箱未打开，开锁
            if (IO_in(IO_IN_TAB[gDCS.check_state_in]) == FALSE)
            {
                if (gDCS.Contrl_State & DCS_JX_OPEN)
                {
                    if (gDCS.Contrl_State & DCS_CONTRL_ON_OFF)
                    {
                        IO_Out(OUT_DCS, FALSE);
                        Device_Link_Scan();
                        gDCS.Contrl_State &= ~DCS_CONTRL_ON_OFF;
                    };
                    buf[0] = MSG_DEV_MAINBARD;
                    buf[1] = MSG_OPTION_CLASS_OVER;
                    Msg_send(buf, 2, &buf[3]);
                }
                else
                {
                    if ((gDCS.Contrl_State & DCS_CONTRL_ON_OFF) == FALSE)
                    {
                        IO_Out(OUT_DCS, TRUE);
                        Device_Link_Scan();
                        gDCS.Contrl_State |= DCS_CONTRL_ON_OFF;
                    }
                    gDCS.Contrl_State &= ~DCS_JX_OPEN;
                }
            }
            else
            {
                //// 上课状态，机箱打开设置机箱打开状态，锁关闭
                if (gDCS.Contrl_State & DCS_CONTRL_ON_OFF)
                {
                    IO_Out(OUT_DCS, FALSE);
                    Device_Link_Scan();
                    gDCS.Contrl_State &= ~DCS_CONTRL_ON_OFF;
                };
                gDCS.Contrl_State |= DCS_JX_OPEN;
            }

        }




    };




}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 Random(void)
{
    uint32 res = 0, *pint, i;
    pint = (uint32 *)EMAC_RX_BUFFER_ADDR;
    for (i = 0; i < 0x600; i++)
        res += *pint++;
    pint = (uint32 *)Get_ClassInfo_Table(0);
    for (i = 0; i < 64; i++)
        res += *pint++;
    return res;
}

BYTE BinToChar(BYTE data)
{
    if (data < 10)
        return data + '0';
    else
        return (data - 10) + 'A';
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Check_FunctionMode(uint8 FunctionMode)
{
    struct CLASS_INFO *pinfo;
    uint8 tmp = FALSE;
    switch(FunctionMode)
    {
    case Device_FunctionMode_YY:
        pinfo = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
        tmp = pinfo->YuYinCtrl;
        tmp = TRUE;
        break;
    case Device_FunctionMode_LB:
        if (gDeviceStatus.DEV_Link_State & DEV_LINK_BJCardLK)
            tmp = TRUE;
        break;
    case Device_FunctionMode_TB:
        if (gDeviceStatus.DEV_Link_State & DEV_LINK_TBLK)
            tmp = TRUE;
        break;
    case Device_FunctionMode_KBRY:
        if (gDeviceStatus.KB_Type == KB_TYPE_RONGYAO)
            tmp = TRUE;
        break;
    case Device_FunctionMode_KBKR:
        if (gDeviceStatus.KB_Type == KB_TYPE_KUREI)
            tmp = TRUE;
        break;
    case Device_FunctionMode_CARD:
        pinfo = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
        tmp = pinfo->Card_Contrl;
        //tmp=3;
        //tmp=0;
        break;
    default:
        tmp = FALSE;

    }
    return tmp;


}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Option_init(void)
{
    uint16 tmp;
    MSG_PQ = OSQCreate(MSG_Q, MSG_MAX_SIZE);

    memset((uint8 *)&gDeviceStatus, 0, sizeof(struct DEVICE_STATUS_TYPE));
    memset((uint8 *)&gDeviceTable, 0, sizeof(struct DEVICE_REC_TYPE));
    memset((uint8 *)&TASK_TABLE, 0, sizeof(struct TASK_TYPE));
    memset((BYTE *)&gStat_Rec, 0, sizeof(struct DEVICE_STAT_USE_TYPE));
    memset((BYTE *)&gClassT, 0, sizeof(struct CLASS_TABLE_TYPE));
    memset((BYTE *)&gDCS, 0, sizeof(struct DEVICE_DCS_TYPE));
    ClassInfo_init();
    tmp = Device_Get_MIX();
    tmp &= ~(3 << 12);
    tmp |= 1 << 12;
    tmp &= ~SMIX_LAN_OUT_SELEC;
    Device_Set_MIX(tmp);

}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void ClassInfo_init(void)
{
    struct CLASS_INFO *pinfo;
    struct DEVICE_STATUS_TYPE *ps;
    NetP *pnet;
    BYTE buf[16];
    uint32 resault;
#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                     */
#endif
    pinfo = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
    pnet = (NetP *)Get_NetPort();
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    memset(buf, 0, 16);
    pnet->Class_ID_flag = User_ID_Read(buf, 0);
    memcpy(ps->DEV_ID, buf, 8);
    if ((pinfo->dev_id != 0) || (pinfo->dev_type != MSG_DEV_CLASSINFO))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("Config Table is Error!!\n\t");
        pinfo = (struct CLASS_INFO *)User_mem_allco(256);
        if (pinfo == NULL)
            return;
        pinfo->dev_id = 0;
        pinfo->dev_type = 0;
        strcpy((char *)pinfo->class_name, (char *)def_classname);
        strcpy((char *)pinfo->school_name, (char *)def_schoolname);
        strcpy((char *)pinfo->house_name, (char *)def_housename);
        pinfo->alm_off.hour = 20;
        pinfo->alm_off.min = 0;
        pinfo->alm_on.hour = 8;
        pinfo->alm_on.min = 0;
        memcpy((BYTE *)pinfo->class_ID, &buf[8], 8);
        memcpy((BYTE *)pinfo->class_ip.IP, (BYTE *)def_ip, 4);
        memcpy((BYTE *)pinfo->class_ip.Mask, (BYTE *)def_mark, 4);
        memcpy((BYTE *)pinfo->Gateway_ip.IP, (BYTE *)def_gateip, 4);
        OS_ENTER_CRITICAL();
        InitIAP();
        //Test_Flash();
        resault = SelSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
        resault = EraseSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
        resault = BlankCHK(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
        resault = SelSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
        resault = RamToFlash(ARM_TABLE_ADDRESS, (uint32)pinfo, 256);
        OS_EXIT_CRITICAL();
        IP_printf("空的配置表被擦除,配置表初始化!");
        User_mem_free((void *)pinfo);
        pinfo = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
    };
    ////////////////////////
    /*BYTE IP[4];
    IP[0] = 0xC0;//192
    IP[1] = 0xA8;//168
    IP[2] = 0x7;//7
    IP[3] = 0x7D;//125
    BYTE Mask[4];
    Mask[0] = 0xff;
    Mask[1] = 0xff;
    Mask[2] = 0x0;
    Mask[3] = 0x0;
    BYTE Gateway[4];
    Gateway[0] = 0xC0;
    Gateway[1] = 0xA8;
    Gateway[2] = 0x0;
    Gateway[3] = 0xbe;

    memcpy((BYTE *)pnet->My_Ip,IP,4);
    memcpy((BYTE *)pnet->My_Ip_Mark,Mask,4);
    memcpy((BYTE *)pnet->My_Gateway,Gateway,4);*/
    //////////////////////
    memcpy((BYTE *)pnet->My_Ip, (BYTE *)pinfo->class_ip.IP, 4);
    memcpy((BYTE *)pnet->My_Ip_Mark, (BYTE *)pinfo->class_ip.Mask, 4);
    memcpy((BYTE *)pnet->My_Gateway, (BYTE *)pinfo->Gateway_ip.IP, 4);
    memcpy( (BYTE *)pnet->Ser_Ip, (BYTE *)pinfo->Ser_ip.IP, 4);
    memcpy( (BYTE *)pnet->BJ_CardIP, (BYTE *)pinfo->BJCardIP, 4);
    memcpy((BYTE *)pnet->Class_ID, (BYTE *)ps->DEV_ID, 8);
    memcpy(ps->configtime, (BYTE *)pinfo->configtime, 4);
    pnet->Class_ID_flag = TRUE;
    pnet->Gateway_IP_TTL = 0;
    pnet->My_Mac[0] = 0xa2; //0xa1
    pnet->My_Mac[1] = 0x52; //0x52
    pnet->My_Mac[2] = pnet->My_Ip[0];
    pnet->My_Mac[3] = pnet->My_Ip[1];
    pnet->My_Mac[4] = pnet->My_Ip[2];
    pnet->My_Mac[5] = pnet->My_Ip[3];
    ps->ver = LSZH_VER;
    resault = resault;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE User_ID_Read(BYTE *buf, uint8 type)
{
    BYTE abuf[16], i, flag = 0, tmp, sbuf[16];
    uint32 rdata;

    memset(abuf, 0, 16);
    for (tmp = 0; tmp < 3; tmp++)
    {
        if  (AT24C16_Read( abuf, USER_ID_ADDRESS, 16))
        {
            if ((abuf[0] != 101) && (type))
            {
                if (tmp < 2)
                    continue;
                IP_printf("Read EEPROM ERROR!");
                rdata = Random();
                for (i = 0; i < 4; i++)
                {
                    abuf[i * 2 + 1] = BinToChar(rdata & 0x0f);
                    abuf[i * 2 + 2] = BinToChar((rdata & 0xf0) >> 4);
                    rdata = rdata >> 8;
                }
                // ID_Change((char *)&abuf[1],"%8x",rdata);
                abuf[0] = 101;
                do
                {
                    if ( AT24C16_Write(abuf, USER_ID_ADDRESS, 16))
                    {
                        AT24C16_Read( sbuf, USER_ID_ADDRESS, 16);
                        if (sbuf[0] == 101)
                        {
                            IP_printf("用户ID更新:%8x", rdata);
                            break;
                        }
                        else
                            IP_printf("用户ID更新sbuf=%d,%s,abu=%s", sbuf[0], &sbuf[1], &abuf[1]);
                    }
                    else
                        IP_printf("EEPROM Write ERR!");
                    IO_FlagLingt_Flash();
                }
                while(1);

                flag = 0x55;
            }
            memcpy(buf, &abuf[1], 8);
            return flag;
        }
    }
    IP_printf("用户ID读错误!");
    return 0xaa;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE Msg_Option_Find(void *pdata)
{
    uint8 id;
    struct MESSAGE_TYPE *poption;
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)pdata;

    if (gDeviceTable.OptionCount == 0)
        return NULL;
    for (id = MSG_DEV_MAINBARD; id < 255; id++)
    {
        poption = (struct MESSAGE_TYPE *)Get_ClassInfo_Table( id);
        if (poption->dev_id != id)
            continue;
        if ((poption->dev_type == 0xfe) && (pmsg->device == poption->sys_msg.device)
                && (pmsg->option == poption->sys_msg.option))
            return id;
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
void *Device_Rec_Find(BYTE *pdata, uint8 Find_Type)
{
    uint8 id, *p;
    struct BASE_MSG *pmsg;
    struct DEVICE_INFO *pdev;

    if (gDeviceTable.DeviceCount == 0)
        return NULL;

    for (id = MSG_DEV_MAINBARD; id < 255; id++)
    {
        pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(id);
        if (pdev->dev_id != id)
            continue;
        switch(Find_Type)
        {
        case DEV_FIND_ID:
            p = (BYTE *)pdata;
            if (*p == pdev->dev_id)	return pdev;
            break;
        case DEV_FIND_TYPE:
            p = (BYTE *)pdata;
            if (*p == pdev->dev_type) return pdev;
            break;
        case DEV_FIND_SELECT:
            pmsg = (struct BASE_MSG *)pdata;
            if (pmsg->device == pdev->KB_MSG.device) return pdev;
            break;
        case DEV_FIND_VGAIN:
            p = (BYTE *)pdata;
            if (*p == pdev->VGA.VGAIn) 	return pdev;
            break;
        case DEV_FIND_VGAOUT:
            p = (BYTE *)pdata;
            if (*p == pdev->VGA.VGAOut) return pdev;
            break;
        case DEV_FIND_VIDEOIN:
            p = (BYTE *)pdata;
            if (*p == pdev->VIDEO.VideoIn) 	return pdev;
            break;
        case DEV_FIND_VIDEOOUT:
            p = (BYTE *)pdata;
            if (*p == pdev->VIDEO.VideoOut) return pdev;
            break;
        //按串口连接设备查询
        case DEV_FIND_XCOM_PORT:
            p = (BYTE *)pdata;
            if ((pdev->COMM.comm_type == DEVICE_COMMTYPE_XCOM)
                    && (*p == pdev->COMM.val.xcom.PortNum)) return pdev;
            break;
        //按串口子地址查询
        case DEV_FIND_XCOM_SubADDr:
            p = (BYTE *)pdata;
            if ((pdev->COMM.comm_type == DEVICE_COMMTYPE_XCOM)
                    && (*p == pdev->COMM.val.xcom.PortNum)
                    && (*(p + 1) == pdev->COMM.val.xcom.address)) return pdev;
            break;

        }
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：Alarm_Link
//	函数功能：报警联动
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Alarm_Link(void)
{
    uint8 i;
    struct DEVICE_INFO *pdev = NULL;
    struct DEVICE_STATUS_TYPE *ps = NULL;

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    for (i = 0; i < 9; i++)
    {
        if (0 == Alarmdev_id[i])
            continue;

        pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(Alarmdev_id[i]);
        if (NULL == pdev)
            continue;

        if (pdev->IN.in_type == ((ps->BJ_IN >> i) & 0x01))
        {
            switch (pdev->OUT.out_no)
            {
            case 1:
                IO_Out(OUT_ALARM1, pdev->OUT.out_type);
                break;
            case 2:
                IO_Out(OUT_ALARM2, pdev->OUT.out_type);
                break;
            case 3:
                IO_Out(OUT_ALARM3, pdev->OUT.out_type);
                break;
            default:
                break;
            }
        }
        else
        {
            switch (pdev->OUT.out_no)
            {
            case 1:
                IO_Out(OUT_ALARM1, !pdev->OUT.out_type);
                break;
            case 2:
                IO_Out(OUT_ALARM2, !pdev->OUT.out_type);
                break;
            case 3:
                IO_Out(OUT_ALARM3, !pdev->OUT.out_type);
                break;
            default:
                break;
            }
        }
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
BYTE Class_Task_Start(BYTE id)
{
    //struct BASE_MSG msg;
    struct CLASS_OPTION_REC classmsg;
    uint8 *p, count, buf[12], rev_buf[10];
    struct CLASS_OPEN_INFO *popen;

    popen = ( struct CLASS_OPEN_INFO *)Get_ClassInfo_Table(id);
    if (popen->dev_id != id)
    {
        gDeviceTable.Class.Flag &= ~CLASS_TASK_FLAG_RUN;
        return CLASS_TASK_ERR_ID; //无效的消息队列
    }
    /*
    if (gDeviceTable.Class.Flag&CLASS_TASK_FLAG_RUN)
    	{gDeviceTable.Class.Flag&=~CLASS_TASK_FLAG_RUN;
    	return CLASS_TASK_BUSY;  //队列在执行
    	}
    */
    gDeviceTable.Class.ClassTaskNum = 0;
    gDeviceTable.Class.ClassCurrNUm = 0;
    p = ((BYTE *)popen + sizeof( struct CLASS_OPEN_INFO ));
    for (count = 0; count < 60; count++)
    {
        memcpy((BYTE *)&classmsg, (p + sizeof(struct CLASS_OPTION_REC)*count), sizeof(struct CLASS_OPTION_REC));
        if ((classmsg.MsgRec.device >= 16) && (classmsg.MsgRec.device <= 100) && (classmsg.MsgRec.option != MSG_OPTION_NONE))
        {
            gDeviceTable.Class.ClassTaskNum++;
        }
    }
    if (gDeviceTable.Class.ClassTaskNum == 0)
    {
        gDeviceTable.Class.Flag &= ~CLASS_TASK_FLAG_RUN;
        return CLASS_TASK_EMPTY; // 没有消息执行
    }
    gDeviceTable.Class.Curr_id = id;
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_MSG_RUN;
    rev_buf[0] = DEV_XCOM_SELF;
    Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 1, buf, 2, gDeviceTable.Class.ClassTaskNum, rev_buf);
    return CLASS_TASK_OK;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Device_Rec_Scan(void)
{
    uint8 index, flag, flag_dev, buf[5];
    //uint16 tmp;
    struct DEVICE_INFO *pdev;
    struct CLASS_OPEN_INFO *pClass;
    struct CLASS_INFO *pinfo;
    NetP *pnet;

    flag = 0;
    flag_dev = 0;
    Mem_Set((uint8 *)Alarmdev_id, 0, sizeof(Alarmdev_id));
    pinfo = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
    pnet = (NetP *)Get_NetPort();
    //设备登记表清零
    //memset((BYTE *)&gDeviceTable,0,sizeof(struct DEVICE_REC_TYPE));
    gDeviceTable.Alarm_Type = 0;

    gDeviceTable.TYJ[0].comm = DEVICE_COMMTYPE_XCOM;
    gDeviceTable.TYJ[0].curr_Type = 1;//VGA
    gDeviceTable.TYJ[0].vgain = DEV_VGA_OUT_TYJ;
    gDeviceTable.TYJ[0].videoin = DEV_VIDEO_OUT_TYJA;

    gDeviceTable.TYJ[1].comm = DEVICE_COMMTYPE_XCOM;
    gDeviceTable.TYJ[1].curr_Type = 1;//VGA
    gDeviceTable.TYJ[1].vgain = DEV_VGA_OUT_TYJ;
    gDeviceTable.TYJ[1].videoin = DEV_VIDEO_OUT_TYJB;
    SOUND_DEVICE_Mute(VOL_CH_MIC1, TRUE);
    SOUND_DEVICE_Mute(VOL_CH_MIC2, TRUE);
    SOUND_DEVICE_Mute(VOL_CH_MIC3, TRUE);
    SOUND_DEVICE_Mute(VOL_CH_MIC4, TRUE);

    Device_InVol( 0, DEV_SOUND_MIC1);
    Device_InVol( 0, DEV_SOUND_MIC2);
    Device_InVol( 0, DEV_SOUND_MIC3);
    Device_InVol( 0, DEV_SOUND_MIC4);
    for (index = MSG_DEV_MAINBARD; index < 255; index++)
    {
        IO_FlagLingt_Flash();
        pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(index);
        //////////操作码扫描///////////////////////////
        if (index != pdev->dev_id)
            continue;
        if (0xfe == pdev->dev_type)
            gDeviceTable.OptionCount++;
        //--------设备码扫描------------------------------
        if (pdev->dev_type < 100)
        {
            gDeviceTable.DeviceCount++;
            if (pdev->VGA.VGAIn != DEV_VGA_OUT_NONE)
                Device_VGA_Select(pdev->VGA.VGAIn, pdev->VGA.VGALink);

            if (pdev->VIDEO.VideoIn != DEV_VIDEO_OUT_NONE)
                Device_Video_Select(pdev->VIDEO.VideoIn, pdev->VIDEO.VideoLink);

            if (pdev->VIDEO.VideoOut != DEV_VIDEO_OUT_NONE)
                if (pdev->dev_type == MSG_DEV_VIDEOCARD)
                    Device_Video_Select(pdev->VIDEO.VideoLink, Change_Encode_Num(pdev->VIDEO.VideoOut));
                else
                    Device_Video_Select(pdev->VIDEO.VideoLink, pdev->VIDEO.VideoOut);

            if (pdev->dev_type == MSG_DEV_TB)
                gDeviceTable.TB = 1;
            if (pdev->dev_type == MSG_DEV_TYJA)
            {
                gDeviceTable.TYJ[0].comm = pdev->COMM.comm_type;
                gDeviceTable.TYJ[0].curr_Type = 1;
                gDeviceTable.TYJ[0].vgain = pdev->VGA.VGAOut;
                gDeviceTable.TYJ[0].videoin = pdev->VIDEO.VideoOut;
                Xcom_set(DEV_XCOM_TYJ, pdev->COMM.val.xcom.BPS + (pdev->COMM.val.xcom.Hbps * 0x10000), DEV_PROTOCOL_NONE, pdev->COMM.val.xcom.linecontrl);
                //UART_set_bps(DEV_XCOM_TYJ,);
            }
            if (pdev->dev_type == MSG_DEV_TYJB)
            {
                gDeviceTable.TYJ[1].comm = pdev->COMM.comm_type;
                gDeviceTable.TYJ[1].curr_Type = 0;
                gDeviceTable.TYJ[1].vgain = pdev->VGA.VGAOut;
                gDeviceTable.TYJ[1].videoin = pdev->VIDEO.VideoOut;
            }
            if (pdev->SOUND.SoundIn != DEV_SOUND_NONE)
            {
                Device_InVol(pdev->SOUND.SoundVol, pdev->SOUND.SoundIn);
                switch(pdev->SOUND.SoundIn)
                {
                case DEV_SOUND_MIC1:
                    SOUND_DEVICE_Mute(VOL_CH_MIC1, FALSE);
                    break;
                case DEV_SOUND_MIC2:
                    SOUND_DEVICE_Mute(VOL_CH_MIC2, FALSE);
                    break;
                case DEV_SOUND_MIC3:
                    SOUND_DEVICE_Mute(VOL_CH_MIC3, FALSE);
                    break;
                case DEV_SOUND_MIC4:
                    SOUND_DEVICE_Mute(VOL_CH_MIC4, FALSE);
                    break;
                }
                //SOUND_DEVICE_Mute(VOL_CH_MIC1,0);
                //SOUND_DEVICE_Mute(VOL_CH_MIC2,0);
                //SOUND_DEVICE_Mute(VOL_CH_MIC3,0);
                //SOUND_DEVICE_Mute(VOL_CH_MIC4,0);
            }

            if (pdev->SOUND.SoundOut != DEV_SOUND_OUT_NONE)
            {
                Device_OutVol(pdev->SOUND.SoundVol, pdev->SOUND.SoundOut);
                //Device_Set_MIX(pdev->SOUND.SoundLink);
                if (pdev->SOUND.SoundOut == DEV_SOUND_OUT_GF)
                    KEY_Set_Vol(pdev->SOUND.SoundVol);
            }
            if (pdev->IN.in_no != DEV_IO_NONE)
            {
                if (pdev->IN.in_type)
                    gDeviceTable.Alarm_Type |= (1 << (pdev->IN.in_no - 1));
                else
                    gDeviceTable.Alarm_Type &= ~(1 << (pdev->IN.in_no - 1));
            }
            if (pdev->dev_type == MSG_DEV_PC)
            {
                memcpy(pnet->PC_Ip, pdev->COMM.val.udp.IP, 4);
                Arp_Request(pnet->PC_Ip);
            }
            if (pdev->dev_type == MSG_DEV_LMT)
            {
                memcpy(pnet->LMT_Ip, pdev->COMM.val.udp.IP, 4);
                Arp_Request(pnet->LMT_Ip);
            }
            if (pdev->dev_type == MSG_DEV_VIDEOCARD)
            {
                /*tmp=Device_Get_MIX();
                if (pdev->SOUND.SoundLink)
                   Device_Set_MIX(tmp|SMIX_BMK_IN_SELEC);
                else
                   Device_Set_MIX(tmp&(~SMIX_BMK_IN_SELEC));
                   */
            }
            //zhw add 2009-09-14
            if (pdev->dev_type == MSG_DEV_ALM)
            {
                if (pdev->IN .in_no != 0)
                    Alarmdev_id[pdev->IN .in_no - 1] = pdev->dev_id;
            }
            /////电磁锁
            if(pdev->dev_type == MSG_DEV_DCS)
            {
                if (Str_Cmp(DCS_TYPE_Str, pdev->dev_name))
                {
                    gDCS.Type = DCS_LCJ_OC39;
                    gDCS.check_state_in = pdev->IN.in_no;
                };


            }
            //////流媒体设备
            if (pdev->dev_type == MSG_DEV_LMT)
            {
                LMT_init(pdev->POWER.dev_id * 16 + pdev->POWER.power_no);
            }
        }
    }
    ///开机列表////////
    pClass = (struct CLASS_OPEN_INFO *)Get_ClassInfo_Table(1);
    if ((pClass->dev_id == 1) && (pClass->dev_type == 1))
    {
        gDeviceTable.Class.ClassOpen[0].device = pClass->msgon1.device;
        gDeviceTable.Class.ClassOpen[0].option = pClass->msgon1.option;
        gDeviceTable.Class.ClassOpen[1].device = pClass->msgon2.device;
        gDeviceTable.Class.ClassOpen[1].option = pClass->msgon2.option;
        gDeviceTable.Class.ClassOpen[2].device = pClass->msgon3.device;
        gDeviceTable.Class.ClassOpen[2].option = pClass->msgon3.option;
        //gDeviceTable.Class.Flag=0;
    }
    ////关机列表 ///////
    pClass = (struct CLASS_OPEN_INFO *)Get_ClassInfo_Table(2);
    if ((pClass->dev_id == 1) && (pClass->dev_type == 1))
    {
        gDeviceTable.Class.ClassClose[0].device = pClass->msgon1.device;
        gDeviceTable.Class.ClassClose[0].option = pClass->msgon1.option;
        gDeviceTable.Class.ClassClose[1].device = pClass->msgon2.device;
        gDeviceTable.Class.ClassClose[1].option = pClass->msgon2.option;
        gDeviceTable.Class.ClassClose[2].device = pClass->msgon3.device;
        gDeviceTable.Class.ClassClose[2].option = pClass->msgon3.option;
        //gDeviceTable.Class.Flag=0;
    }
    //读课程表数据
    File_ClassTable_Read_CurDay();
    //读开机模式
    if ( Check_FunctionMode(Device_FunctionMode_CARD) == 1)
    {
        ICReader_Power(ICREADER_POWER_ON);
    }
    else
    {
        ICReader_Power(ICREADER_POWER_OFF);
    }
    //更新统计数据
    Device_Use_Stat_Read();
    gDeviceStatus.MK_Vol = 80;
    pinfo = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
    memcpy(gDeviceStatus.configtime, pinfo->configtime, 4);
    flag = flag;
    flag_dev = flag_dev;
    ///////////////
    buf[0] = MSG_DEV_PC;
    buf[1] = MSG_OPTION_NONE;
    buf[2] = 0;
    Msg_send(buf, 2, &buf[2]);
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
uint8 Msg_send(uint8 *pdata, uint16 lg, uint8 *prev)
{
    uint8 err = 1, *p;

#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register    */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                    */
#endif
    if ((lg == 0) || (pdata == NULL))
        return 1;
    OS_ENTER_CRITICAL();
    p = (uint8 *)User_mem_allco(lg);
    if (p != NULL)
    {
        Mem_Clr(p, lg);
        memcpy(p, prev, sizeof(struct MSG_REV_TYPE));
        memcpy(p + sizeof(struct MSG_REV_TYPE), pdata, lg);
        err = OSQPost(MSG_PQ, p);
        if (err != OS_NO_ERR)
            User_mem_free(p);
    };
    OS_EXIT_CRITICAL();
    return err;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE Task_Rec_Del(BYTE *pdata, BYTE lg)
{
    BYTE tmp, i, *p;
    struct TASK_REC_TYPE *ptask;
#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif
    if ((lg == 0) || (pdata == NULL))
        return 1;
    OS_ENTER_CRITICAL();
    for (tmp = 0; tmp < TASK_DELAY_MAX_SIZE; tmp++)
    {
        ptask = (struct TASK_REC_TYPE *)&TASK_TABLE.DelayRec[tmp];
        if (ptask->pmsg != NULL)
        {
            p = (BYTE *)ptask->pmsg + sizeof(struct MSG_REV_TYPE);
            for (i = 0; i < lg; i++)
                if (*(p + i) != *(pdata + i))
                    break;
            if (i == lg)
            {
                User_mem_free((BYTE *)ptask->pmsg);
                ptask->pmsg = NULL;
                ptask->DelayTime = 0;
                ptask->TimeType = 0;
                if (TASK_TABLE.RecCount != 0)
                    TASK_TABLE.RecCount--;
                break;
            }
        }
    }
    OS_EXIT_CRITICAL();
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
BYTE Task_Rec_Find(BYTE *pdata, BYTE lg)
{
    BYTE tmp, i, *p;
    struct TASK_REC_TYPE *ptask;
    for (tmp = 0; tmp < TASK_DELAY_MAX_SIZE; tmp++)
    {
        ptask = (struct TASK_REC_TYPE *)&TASK_TABLE.DelayRec[tmp];
        if (ptask->pmsg != NULL)
        {
            p = (BYTE *)ptask->pmsg + sizeof(struct MSG_REV_TYPE);
            for (i = 0; i < lg; i++)
                if (*(p + i) != *(pdata + i))
                    break;
            if (i >= lg)
                return ptask->repeat;
        }
    }
    return 0xff;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：uint8 Task_Run_Delay(uint16 DelayTime,uint8 *pdata,uint16 lg)
//	函数功能：消息被延时执行
//	入口参数：delayTime :高8位中断类型秒，分时低8位延时时间0-255
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Task_Run_Delay(uint16 DelayTime, uint8 *pdata, uint16 lg, uint8 repeat, uint8 *prev)
{
    uint8 *p, tmp;

#if OS_CRITICAL_METHOD == 3                /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;                     /* Prevent compiler warning                      */
#endif
    if ((lg == 0) || (pdata == NULL))
        return FALSE;
    for (tmp = 0; tmp < TASK_DELAY_MAX_SIZE; tmp++)
        if (TASK_TABLE.DelayRec[tmp].pmsg == NULL) break;
    if (tmp >= TASK_DELAY_MAX_SIZE)
        return FALSE;
    p = (uint8 *)User_mem_allco(lg);
    if (p == NULL)
        return FALSE;

    OS_ENTER_CRITICAL();
    memcpy(p, prev, sizeof(struct MSG_REV_TYPE));
    memcpy(p + sizeof( struct MSG_REV_TYPE), pdata, lg);
    TASK_TABLE.DelayRec[tmp].TimeType = DelayTime >> 8;
    TASK_TABLE.DelayRec[tmp].DelayTime = DelayTime & 0xff;
    TASK_TABLE.DelayRec[tmp].pmsg = p;
    TASK_TABLE.DelayRec[tmp].msg_lg = lg;
    TASK_TABLE.DelayRec[tmp].bak_time = DelayTime & 0xff;
    TASK_TABLE.DelayRec[tmp].repeat = repeat;
    TASK_TABLE.RecCount++;
    OS_EXIT_CRITICAL();

    return TRUE;
}

void CameraPowerInit(void)
{
    //uint8 buf[3] = {0};
    BYTE devid, *pBuf, id;
    struct DEVICE_INFO *pdev;
    pBuf = Get_ClassInfo_Table(0);
    devid = *(pBuf + 140);//读出监控设备ID
    if (devid > 7 ) return ;
    devid = Encode_TAB[devid];
    for (id = MSG_DEV_MAINBARD + 1; id < 250; id++)
    {
        pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(id);
        if (pdev->dev_type == MSG_DEV_VIDEOCARD)
        {
            if (pdev->VIDEO.VideoOut == devid) break;
        }
    }
    if (id >= 250)  return ;
    Device_Video_Select(pdev->VIDEO.VideoLink, Change_Encode_Num(pdev->VIDEO.VideoOut));

    devid = pdev->VIDEO.VideoLink;
    if ((devid < DEV_VIDEO_OUT_BM1_1) || (devid > DEV_VIDEO_OUT_BM2_4)) return ;
    pdev = (struct DEVICE_INFO *)Device_Rec_Find(&devid, DEV_FIND_VIDEOIN);
    if (pdev->dev_type == 0xfe) return ;
    POWER_Set(pdev->POWER.power_no, MSG_OPTION_POWEROPEN);

}


/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：任务的接收和一级分配
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void User_Option(void)
{
    struct BASE_MSG *pmsg;
    struct MSG_REV_TYPE *prev;
    uint8 err, *p, *pdata, buf[10], tmp;
    NetP *pnet;
    uint32 flag;



    PT2258Init();
#if   DBG_TEST	>	0
    TB_init();
    DW_init();
#endif
    key_init();
    HW_init();
    POWER_init();
    IO_FlagLingt_Flash();
    RealTime_init();

#if DBG_TEST >0
    IO_FlagLingt_Flash();
    Sound_init();
#endif
    IO_FlagLingt_Flash();
    LED_Set(0, LED_ALL_CLR);
    LED_SendFLASH();
    LED_Contrl(LED_CONTRL_TIME_OFF, TRUE);
    On_MSG_OPTION_STATUS_CHANGE(NULL);
    CameraPowerInit();
    IO_FlagLingt_Flash();
#if DBG_TEST >0
    Arp_Request( Get_DesIp( DEVICE_SUB_GATE ));
#endif
    OSTimeDly(10);
    IO_FlagLingt_Flash();
    SOUND_DEVICE_Mute(VOL_CH_MIC1, TRUE);
    IO_FlagLingt_Flash();
    SOUND_DEVICE_Mute(VOL_CH_MIC2, TRUE);
    IO_FlagLingt_Flash();
    SOUND_DEVICE_Mute(VOL_CH_MIC3, TRUE);
    IO_FlagLingt_Flash();
    SOUND_DEVICE_Mute(VOL_CH_MIC4, TRUE);
    Device_OutVol(95, DEV_SOUND_OUT_LMT);
    tmp = Device_Get_MIX();
    Device_Set_MIX(tmp | SMIX_BMK_IN_SELEC);
    //Device_OutVol(75,DEV_SOUND_OUT_GF);
    pnet = (NetP *)Get_NetPort();
    memset(buf, 0, 10);
    gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_OVER;
    User_ID_Read(buf, 1);
    pnet->Test_TTL = 6; //关闭调试信息
    IP_printf("用户ID:%s\t本机IP=%d.%d.%d.%d", buf, pnet->My_Ip[0], pnet->My_Ip[1], pnet->My_Ip[2], pnet->My_Ip[3]);
    pnet->Test_TTL = 0; //关闭调试信息
    ClassTableWeekInit();
    Device_OutVol(75, DEV_SOUND_OUT_GF);
    //Device_OutVol(75,DEV_SOUND_OUT_LMT);
    Device_Rec_Scan();
    for (;;)
    {
        pdata = (uint8 *)OSQPend(MSG_PQ, 0, &err);
        if (err == OS_NO_ERR)
        {
            prev = (struct MSG_REV_TYPE *)pdata;
            prev->Type &= 0xff;
            pmsg = (struct BASE_MSG *)(pdata + sizeof(struct MSG_REV_TYPE));
            if (Get_Debug(DEV_DEBUG_MSG) && (pmsg->option != 0xc0))
            {
                if (prev->Type != DEV_XCOM_UDP)
                    IP_printf("消息来源:%d设备号:%d操作码:%x", prev->Type & 0xff, pmsg->device, pmsg->option);
                else
                    IP_printf("网络消息,来源IP:%d.%d.%d.%d设备号:%d操作码:%x", prev->des_ip[0], prev->des_ip[1], prev->des_ip[2], prev->des_ip[3], pmsg->device, pmsg->option);
            }
            flag = 0;
            if ((pmsg->option >= MSG_OPTION_PLAY) && (pmsg->option < MSG_OPTION_LT2_OFF + 1))
                flag = On_MSG_OPTION(pdata);
            else
            {
                switch(pmsg->option)
                {
                case MSG_OPTION_NONE:			//0x00 //无动作
                    flag = ON_Device_select(pdata);
                    break;
                case MSG_OPTION_POWEROPEN:		//0x01 //设备打开/上升
                case MSG_OPTION_POWERCLOSE:		//0x02 //设备关闭
                case MSG_OPTION_POWERDOWN:		//0x03 //设备下降
                    flag = On_MSG_Power_OPTION((void *)pdata); //先发送关闭红外/串口命令
                    flag = ON_POWER_option((void *)pdata);
                    flag = On_MSG_Power_OPTION((void *)pdata); //先发送关闭红外/串口命令
                    break;
                case MSG_OPTION_MUTEOPEN:		//0x04 //设备静音
                case MSG_OPTION_MUTECLOSE:		//0x05 //设备静音关闭
                    flag = ON_Sound_Mute(pdata);
                    break;
                case MSG_OPTION_VOL:			//0x06 //设备音量
                    tmp = *((BYTE *)pdata + sizeof(struct BASE_MSG)) + sizeof(struct MSG_REV_TYPE);
                    Device_OutVol(tmp, DEV_SOUND_OUT_GF);
                    //Device_OutVol(tmp,DEV_SOUND_OUT_LMT);
                    //Device_OutVol(DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].OUT_Vol, DEV_SOUND_OUT_LMT);
                    break;
                case MSG_OPTION_RESET:			//0x07一体机复位
                    Reset(pdata);
                    IP_printf("系统复位命令");
                    break;
                case MSG_OPTION_OPEN_OVER:		//0x0b //设备开机完成
                case MSG_OPTION_CLOSE_OVER:		//0x0c //设备关机完成
                case MSG_OPTION_DEV_LINK:		//0x0d //设备连接
                case MSG_OPTION_DEV_BREAK:		//0x0e //设备断开
                case MSG_OPTION_ENABLE_CLOSE:	//0x0f //设备可以关闭
                    flag = ON_Change_State(pmsg);
                    break;
                //摄象操作
                case MSG_OPTION_PERSONAL_TALL1:	//0x30 //身高设定160
                case MSG_OPTION_PERSONAL_TALL2:	//0x31 //身高设定170
                case MSG_OPTION_PERSONAL_TALL3:	//0x32 //身高设定180
                    flag = On_MSG_OPTION_PERSONAL_TALL(pdata);
                    break;
                case MSG_OPTION_SELECT_SCAMERA1://0x33 //摄象机选择1
                case MSG_OPTION_SELECT_SCAMERA2://0x34 //摄象机选择2
                case MSG_OPTION_SELECT_SCAMERA3://0x35 //摄象机选择3
                case MSG_OPTION_SELECT_SCAMERA4://0x36 //摄象机选择4
                case MSG_OPTION_SELECT_SCAMERA5://0x37 //摄象机选择5
                case MSG_OPTION_SELECT_SCAMERA6://0x38 //摄象机选择6
                    flag = On_MSG_OPTION_SELECT_SCAMERA(pdata);
                    break;
                case MSG_OPTION_SCAMERA_ZOOMADD://0x39 //摄象机ZOOM+
                case MSG_OPTION_SCAMERA_ZOOMSUB://0x3a //摄象机ZOOM-
                case MSG_OPTION_SCAMERA_UP:		//0x3b //摄象机上
                case MSG_OPTION_SCAMERA_DOWN:	//0x3c //摄象机下
                case MSG_OPTION_SCAMERA_LIFT:	//0x3d //摄象机左
                case MSG_OPTION_SCAMERA_RIGHT:	//0x3e //摄象机右
                case MSG_OPTION_SCAMERA_STOP:
                case MSG_OPTION_SCAMERA_LEFTUP:		   //摄像机左上
                case MSG_OPTION_SCAMERA_LEFTDOWN:	   //摄像机左下
                case MSG_OPTION_SCAMERA_RIGHTUP	:	   //摄像机右上
                case MSG_OPTION_SCAMERA_RIGHTDOWN:	   //摄像机右下
                case MSG_OPTION_SCAMERA_FOCUSNEAR:         //
                case MSG_OPTION_SCAMERA_FOCUSFAR:
                case MSG_OPTION_SCAMERA_PREPOSITION:
                case MSG_OPTION_SCAMERA_SETPREPOSITION:
                    flag = On_MSG_OPTION_SCAMERA(pdata);
                    break;
                //录制操作
                case MSG_OPTION_RECORD_START:	//0x40 //录像开始
                case MSG_OPTION_RECORD_PAUSE:	//0x41 //录像暂停
                case MSG_OPTION_RECORD_STOP:	//0x42 //录像停止
                    flag = On_MSG_OPTION_RECORD(pdata);
                    break;
                case MSG_OPTION_CODE1_SPEED1:	//0x43 // 码速1 500k
                case MSG_OPTION_CODE1_SPEED2:	//0x44 // 码速2 1M
                case MSG_OPTION_CODE1_SPEED3:	//0x45 // 码速1 自定义
                    flag = On_MSG_OPTION_CODE1_SPEED(pdata);
                    break;
                case MSG_OPTION_METRIC1:		//0x46 //分辨率XGA
                case MSG_OPTION_METRIC2:		//0x47 //分辨率D1
                case MSG_OPTION_METRIC3:		//0x48 //分辨率自定义
                    flag = On_MSG_OPTION_METRIC(pdata);
                    break;
                case MSG_OPTION_FRAME1:			//0x49 //画中画
                case MSG_OPTION_FRAME2:			//0x4a //常规
                case MSG_OPTION_FRAME3:			//0x4b //自定义
                    flag = On_MSG_OPTION_FRAME(pdata);
                    break;
                case MSG_OPTION_CLASS_BEGIN: 	//0x70 //上课
                    flag = On_MSG_OPTION_CLASS_BEGIN(pdata);
                    break;
                case MSG_OPTION_CLASS_OVER:		//0x71 //下课
                    flag = On_MSG_OPTION_CLASS_OVER(pdata);
                    break;
                case MSG_OPTION_RF_ACTIVE:		//0x72 //rf卡有效
                    flag = On_MSG_OPTION_RF_ACTIVE(pdata);
                    break;
                case MSG_OPTION_HELP_ON:		//0x73 //帮助开
                    flag = On_MSG_OPTION_HELP_ON(pdata);
                    break;
                case MSG_OPTION_HELP_OFF:		//0x74 //帮助管
                    flag = On_MSG_OPTION_HELP_OFF(pdata);
                    break;
                case MSG_OPTION_AUTO_ON	:		//0x75 //自动
                    flag = On_MSG_OPTION_AUTO_ON(pdata);
                    break;
                case MSG_OPTION_AUTO_OFF:		//0x76 //手动
                    flag = On_MSG_OPTION_AUTO_OFF(pdata);
                    break;
                case MSG_OPTION_DEV_REMOTE_ON:	//0x77 //远程设备控制开
                    flag = On_MSG_OPTION_DEV_REMOTE_ON(pdata);
                    break;
                case MSG_OPTION_REC_REMOTE_ON:	//0x78 //远程设备控制开
                    flag = On_MSG_OPTION_REC_REMOTE_ON(pdata);
                    break;
                case MSG_OPTION_DEV_REMOTE_OFF:	//0x79 //远程设备控制关
                    flag = On_MSG_OPTION_DEV_REMOTE_OFF(pdata);
                    break;
                case MSG_OPTION_REC_REMOTE_OFF:	//0x7a //远程录像控制关
                    flag = On_MSG_OPTION_REC_REMOTE_OFF(pdata);
                    break;
                case MSG_OPTION_HW_BIGIN:		//0x7b //红外学码开始
                    HW_XM_start(pdata);
                    break;
                case MSG_OPTION_HW_END:
                    On_MSG_OPTION_HW_END(pdata);
                    break;
                case 0x7d:
                    On_MSG_OPTION_HELP_TEST(pdata);
                    break;
                case MSG_OPTION_HELP_LINK:		//0x80 //帮助通话连接
                    flag = On_MSG_OPTION_HELP_LINK(pdata);
                    break;
                case MSG_OPTION_HELP_BREAK:		//0x81 //帮助通话断开
                    flag = On_MSG_OPTION_HELP_BREAK(pdata);
                    break;
                case MSG_OPTION_AUTOTRACK_ON:	//0x82 //自动跟拍开
                    flag = On_MSG_OPTION_AUTOTRACK_ON(pdata);
                    break;
                case MSG_OPTION_AUTOTRACK_OFF:	//0x83 //自动跟拍关
                    flag = On_MSG_OPTION_AUTOTRACK_OFF(pdata);
                    break;
                case MSG_OPTION_STRATEGY_ON:	//0x84 //自动策略开
                    flag = On_MSG_OPTION_STRATEGY_ON(pdata);
                    break;
                case MSG_OPTION_STRATEGY_OFF:	//0x85 //自动策略关
                    flag = On_MSG_OPTION_STRATEGY_OFF(pdata);
                    break;
                case MSG_OPTION_HD_ON:			//0x86 //无线互动开
                    flag = On_MSG_OPTION_HD_ON(pdata);
                    break;
                case MSG_OPTION_HD_OFF:			//0x87 //无线互动关
                    flag = On_MSG_OPTION_HD_OFF(pdata);
                    break;
                case MSG_OPTION_HD_VAL:			//0x88 //无线互动按键值
                    flag = On_MSG_OPTION_HD_VAL(pdata);
                    break;
                case MSG_OPTION_SCAMERA_CONTRL:	//0x89 //摄象机控制邋（参数）
                    flag = On_MSG_OPTION_SCAMERA_CONTRL(pdata);
                    break;
                case MSG_OPTION_SCAMERA_BackVAL:	//0x8a //摄象机返回参数
                    flag = On_MSG_OPTION_SCAMERA_VAL(pdata);
                    break;
                case MSG_OPTION_REC_ReqStatus://0x8b//查询/回应当前录制通道状态
                    flag = On_MSG_OPTION_REC_ReqStatus(pdata);
                    break;
                case MSG_OPTION_REC_Contrl://0x8c//录制通道控制
                    flag = On_MSG_OPTION_REC_Contrl(pdata);
                    break;
                case MSG_OPTION_LMT_ReqStatus://         0x8d//请求/返回流媒体状态
                    flag = On_MSG_OPTION_LMT_ReqStatus(pdata);
                    break;

                case MSG_OPTION_TB_ReqStatus://            0x8e //请求/返回同步卡信号状态
                    flag = on_MSG_OPTION_TB_ReqStatus(pdata);
                    break;
                case MSG_OPTION_SOUND_SELECT:	//0x90  //声音通道控制
                    flag = On_MSG_OPTION_SOUND_SELECT(pdata);
                    break;
                case MSG_OPTION_SOUND_VOL:		//0x91 //音量控制
                    flag = On_MSG_OPTION_SOUND_VOL(pdata);
                    gDeviceStatus.MK_Vol &= 0xFF00;
                    gDeviceStatus.MK_Vol += *((BYTE *)pdata + sizeof(struct BASE_MSG) + sizeof(struct MSG_REV_TYPE));
                    flag = 1;
                    break;
                case MSG_OPTION_VIDEO_SELECT:	//0x92 //视频控制
                    flag = On_MSG_OPTION_VIDEO_SELECT(pdata);
                    break;
                case MSG_OPTION_VGA_SELECT:		//0x93 //VGA控制
                    flag = On_MSG_OPTION_VGA_SELECT(pdata);
                    break;
                case MSG_OPTION_ARLM:			//0x94 //报警状态改变
                    flag = On_MSG_OPTION_ARLM(pdata);
                    break;
                case MSG_OPTION_IO_OUT:			//0x95 //报警IO输出
                    flag = On_MSG_OPTION_IO_OUT(pdata);
                    break;
                case MSG_OPTION_JT_ON:			//0x96 //监听开
                case MSG_OPTION_JT_OFF:			//0x97 //监听关
                    flag = On_MSG_OPTION_JT(pdata);
                    break;
                case MSG_OPTION_POWER:
                    flag = On_MSG_OPTION_POWER(pdata);
                    break;
                case MSG_OPTION_IPPlay://				0x99 //IP音频播放控制
                    flag = On_MSG_OPTION_IPPlay(pdata);
                    break;
                case  MSG_OPTION_IP_DPlay://                    0x9a //ip对讲+回声抑制
                    flag = On_MSG_OPTION_IP_DPlay(pdata);
                    break;
                case MSG_OPTION_IP_MICVol://			0x9b //ip对讲mic 音量控制
                    flag = on_MSG_OPTION_IP_MICVol(pdata);
                    break;
                case MSG_OPTION_IP_HPVol://			0x9c //ip对讲扬声器音量控制
                    flag = on_MSG_OPTION_IP_HPVol(pdata);
                    break;
                case MSG_OPTION_YY_Teather://			0xa3 //语音教学模式
                case MSG_OPTION_YY_NoTeather://		0xa4 //语音自习模式
                    flag = On_MSG_OPTION_YY_Teather(pdata);
                    break;
                case MSG_OPTION_SVRLINK_REQ:	//0xb0 //服务器连接
                    flag = On_MSG_OPTION_SVRLINK_REQ(pdata);
                    break;
                case MSG_OPTION_SEVER_ACK:		//0xb1 //服务器应答
                    flag = On_MSG_OPTION_SEVER_ACK(pdata);
                    break;
                case MSG_OPTION_FILE_REQ:		//0xb2 //文件请求
                    flag = On_MSG_OPTION_FILE_REQ(pdata);
                    break;
                case MSG_OPTION_FILE_SEND:	    //0xb3 //文件传送
                    flag = On_MSG_OPTION_FILE_SEND(pdata);
                    break;
                case MSG_OPTION_CLASSLINK_REQ:	//0xb4 //连接请求
                    flag = On_MSG_OPTION_SVRLINK_REQ(pdata);
                    break;
                case MSG_OPTION_DEBUG:			//0xb5 //调试信号输出
                    flag = On_MSG_OPTION_DEBUG(pdata);
                    break;
                case MSG_OPTION_STATUS_CHANGE:
                    On_MSG_OPTION_STATUS_CHANGE(pdata);
                    break;
                case MSG_OPTION_DEV_RESET:		//0xb7 //设备复位
                    flag = On_MSG_OPTION_DEV_RESET(pdata);
                    break;
                case MSG_OPTION_LAN_KEY:        //0xb8 //网络键盘
                    p = (BYTE *)pdata + 2 + sizeof(struct MSG_REV_TYPE);
                    if (*p == KEY_SOUND_VOL)
                    {
                        KEY_Set_Vol(*(p + 1));	//发向面板的音量控制
                    }
                    key_msg_rev((BYTE *)pdata + 2 + sizeof(struct MSG_REV_TYPE));
                    break;
                case MSG_OPTION_SERVERWRITE:	//0xbc
                    Device_Link_Scan();
                    flag = On_MSG_OPTION_SERVER_FILE_SEND(pdata);
                    break;
                case  MSG_OPTION_CardContrl://			0xbe //校园一卡通控制身份认证
                    On_MSG_OPTION_CardContrl(pdata);
                    break;
                case MSG_OPTION_TIMEACTIVE:		//0xc0 //一秒消息
                    flag = On_MSG_OPTION_TIMEACTIVE((BYTE *)pdata);
                    break;
                case MSG_OPTION_ALM_TIME:		//0xc1 //报警时间到
                    ON_MSG_OPTION_ALM_TIME((BYTE *)pdata);
                    break;
                case MSG_OPTION_PBELL_OPEN:		//0xc2 //电源箱报警开
                case MSG_OPTION_PBELL_COLSE:	//0xc3 //电源箱报警关
                case MSG_OPTION_ASOUND_PLAY:	//0xc4 //报警音频音频播放
                case MSG_OPTION_ASOUND_PUSH:	//0xc5 //报警音频音频暂停
                case MSG_OPTION_ASOUND_STOP:	//0xc6 //报警音频音频停止
                    break;
                case MSG_OPTION_COMM:			//0xc7 //设备通讯
                    flag = On_MSG_OPTION_COMM(pdata);
                    break;
                case MSG_OPTION_HW_SEND:		//0xc8 //红外发送
                    flag = On_MSG_OPTION_HW_SEND((BYTE *)pdata);
                    break;
                case MSG_OPTION_MSG_RUN:		//0xc9 //消息序列执行
                    flag = On_MSG_OPTION_MSG_RUN(pdata);
                    break;
                case MSG_OPTION_DEVTABLE_NEW:	//0xc0 //设备列表更新
                    flag = Device_Rec_Scan();
                    break;
                case MSG_OPTION_CLASSTABLE_ON:	//0xcb //课表/定时开
                    On_MSG_OPTION_CLASSTABLE_ON();
                    break;
                case MSG_OPTION_CLASSTABLE_OFF:	//0xcc //课表/定时关
                    On_MSG_OPTION_CLASSTABLE_OFF();
                    break;
                case MSG_OPTION_POWERCARD_ONOFF://0xcd //电源管理卡操作
                    flag = On_MSG_OPTION_POWERCARD_ONOFF(pdata);
                    break;
                case  MSG_OPTION_TYJ_DIS_VGA://		0xce//投影机显示VGA
                case  MSG_OPTION_TYJ_DIS_VIDEO://		0xcf//投影机显示VIDEO
                    On_MSG_OPTION_TYJ_DIS_VGA(pdata);
                    break;
                case MSG_OPTION_REPEATPOWERCLOSE:
                    flag = On_MSG_RepeatPower_OPTION((void *)pdata); //发送关闭红外/串口命令
                    break;
                case MSG_OPTION_TEST_IO:
                    //p=(BYTE *)pdata+sizeof(struct MSG_REV_TYPE);
                    //if (((*(p+2))==0xaa)&&((*(p+3))==0x55))

                    //flag=On_MSG_OPTION_TEST_IO(pdata);
                    break;
                case MSG_OPTION_UART_TEST:

                    break;
                case MSG_OPTION_IO_TEST:
                    On_MSG_OPTION_IO_TEST(pdata);
                    break;
                case  MSG_OPTION_LEDContrl:
                    On_MSG_OPTION_LEDContrl(pdata);
                    break;
                case 0xff:
                    flag = STATUS_CHANGE;
                    break;
                default:
                    break;
                }
            }

            if  (flag & STATUS_CHANGE)
                Send_State();
        }
        //清看门狗，LED闪烁
        IO_FlagLingt_Flash();

        //释放内存
        User_mem_free(pdata);


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
BYTE Get_Debug(uint16 flag)
{
    if (gDeviceTable.Debug & flag)
        return TRUE;
    else
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
void *Get_Device_Status(void)
{
    return &gDeviceStatus;
}

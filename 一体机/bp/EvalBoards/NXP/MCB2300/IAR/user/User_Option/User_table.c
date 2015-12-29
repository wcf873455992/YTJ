#include "includes.h"

/////////////////////////////////////////////////////////////////////
//	函数名称： void File_Req(uint16 type, uint16 page)
//	函数功能:文件申请
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 File_Req(uint16 type, uint16 page, uint8 *des_ip)
{
    uint8 buf[10];

    /*if (page==0xffff) //文件申请完成或写文件错误
    {
    	gDeviceTable.Debug=gDeviceTable.File.FileDebug;
    }*/
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_FILE_REQ;
    buf[2] = type % 256;
    buf[3] = type / 256;
    buf[4] = page % 256;
    buf[5] = page / 256;
    UDP_Send_CMD(buf, 6, des_ip);
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：void File_Table_Write(BYTE page,BYTE *buf)
//	函数功能： 配置表文件读
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void File_Table_Write(uint16 page, BYTE *buf, uint8 *des_ip)
{
    uint8 str[128], err, *pw;
    uint32 resault,	 tmp;
    struct CLASS_INFO *pinfo;
#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register       */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                       */
#endif

    //设备表擦除
    if (page == 0)
    {
        //	gDeviceTable.File.FileDebug=gDeviceTable.Debug;
        //	gDeviceTable.Debug&=~DEV_DEBUG_MSG;
        OS_ENTER_CRITICAL();
        InitIAP();
        resault = SelSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
        resault = EraseSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
        resault = BlankCHK(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
        OS_EXIT_CRITICAL();
        IP_printf("配置表被擦除!");
        if (resault  !=  CMD_SUCCESS)
        {
            File_Req(0, 0xffff, des_ip);
            return;
        }
    }

    if (page < 256)
    {
        if ((page % 10) == 0)
            IP_printf("文件写:%d\n\n\n", page);
        InitIAP();
        //p=(BYTE *)(buf+sizeof (struct BASE_MSG)+sizeof(struct SFILE_TRANSLATE_TYPE ));
        pw = (BYTE *)User_mem_allco(256);
        if (pw == NULL)
        {
            File_Req(0, page, des_ip);
            return;
        }

        memcpy(pw, buf, 256);
        if (page == 0)
        {
            pinfo = (struct CLASS_INFO *)pw;
            User_ID_Read(str, 1);
            memcpy((BYTE *)pinfo->class_ID, str, 8);
        }

        if (page == 0xfa)
        {
            err = 0;
            memset(str, 0, 128);
            while(AT24C16_Write(str, DEVICE_USE_STAT_TIME, 128) == FALSE)
            {
                if (err++ > 10 ) break;
            };
        }
        if ((page > 2) && (page < 10)) //课程表
        {
            err = 0;
            while(AT24C16_Write(pw, CLASS_TABLE_ADDRESS + (page - 3) * 256, 256) == FALSE)
            {
                if (err++ > 10) break;
            };
        }
        else
        {
            tmp = ARM_TABLE_ADDRESS + page * 256;
            OS_ENTER_CRITICAL();
            resault = SelSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
            resault = RamToFlash(tmp, (uint32)pw, 256);
            OS_EXIT_CRITICAL();
        }
        User_mem_free(pw);

        //申请下一包
        if ((page + 1) < 256)
        {
            File_Req(0, page + 1, des_ip);
        }
        else
        {
            File_Req(0, 0xffff, des_ip);
            Device_Rec_Scan();
        }
    }
    else
    {
        File_Req(0, 0xffff, des_ip);
        Device_Rec_Scan();
    }
}

/////////////////////////////////////////////////////////////////////
//	函数名称：void File_Table_Read(BYTE page)
//	函数功能： 配置表文件读
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void File_Table_Read(uint16 page, uint8 *des_ip)
{
    BYTE *p, err, i, *pstat;
    struct CLASS_INFO *pinfo = NULL;

    if ((page % 10) == 0)
        IP_printf("文件读-页号:%d", page);
    p = (BYTE *)User_mem_allco(500);
    if (p == NULL)
        return;
    *p = MSG_DEV_MAINBARD;
    *(p + 1) = MSG_OPTION_FILE_SEND;
    *(p + 2) = 0;
    *(p + 3) = 0;
    if (page > 255)
    {
        *(p + 4) = 0xff;
        *(p + 5) = 0xff;
    }
    else
    {
        *(p + 4) = page & 0xff;
        *(p + 5) = page / 256;
        if (page == 0xfa)
        {
            memcpy(p + 6, Get_ClassInfo_Table(page & 0xff), 256);
            for (i = 0; i < gStat_Rec.Device; i++)
            {
                pstat = (BYTE *)(p + 6 + 4 + i * sizeof(struct DEVICE_STAT_REC_TYPE));
                memcpy(pstat, (BYTE *)&gStat_Rec.Rec[i], sizeof( struct DEVICE_STAT_REC_TYPE));
            }
        }
        else if ((page > 2) && (page < 10)) //课程表
        {
            err = 0;
            while (AT24C16_Read(p + 6, CLASS_TABLE_ADDRESS + (page - 3) * 256, 256) == FALSE)
            {
                if (err++ > 10) break;
            };
        }
        else
        {
            memcpy(p + 6, Get_ClassInfo_Table(page & 0xff), 256);
            if(page == 0)
            {
                pinfo = (struct CLASS_INFO *)(p + 6);
                memcpy(pinfo->class_ID, gDeviceStatus.DEV_ID, 8);
            }
        }
    }
    UDP_Send_CMD(p, 256 + sizeof(struct SFILE_TRANSLATE_TYPE ) + 2, des_ip);
    User_mem_free(p);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：File_ClassTable_Write
//	函数功能：课程表写
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void File_ClassTable_Write(uint16 FilePage, uint8 *buf, uint8 *des_ip)
{
    if (FilePage > 6)
    {
        File_Req(18, 0xffff, des_ip);
        return;
    }
    AT24C16_Write(buf, CLASS_TABLE_ADDRESS + FilePage * 256, 256);

    File_Req(18, FilePage + 1, des_ip);
    if (FilePage == 6)
        File_ClassTable_Read_CurDay();
}

/////////////////////////////////////////////////////////////////////
//	函数名称：File_ClassTable_Write_OneDay
//	函数功能：写一天的课程信息
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void File_ClassTable_Write_OneDay(uint16 FilePage, uint8 *buf, uint8 *des_ip)
{
    AT24C16_Write(buf, CLASS_TABLE_ADDRESS + FilePage * 256, 256);
    File_ClassTable_Read_CurDay();
}

/////////////////////////////////////////////////////////////////////
//	函数名称：File_ClassTable_Read
//	函数功能：读课程表
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void File_ClassTable_Read(uint16 FilePage, uint8 *des_ip)
{
    BYTE *p = NULL;
    p = (BYTE *)User_mem_allco(500);
    if (p == NULL)
        return ;

    *p = MSG_DEV_MAINBARD;
    *(p + 1) = MSG_OPTION_FILE_SEND;
    *(p + 2) = FILE_TYPE_CLASS_TABLE;
    *(p + 3) = 0;
    if (FilePage > 6)
    {
        *(p + 4) = 0xff;
        *(p + 5) = 0xff;;
    }
    else
    {
        *(p + 4) = FilePage & 0xff;
        *(p + 5) = 0;
        AT24C16_Read(p + 6, CLASS_TABLE_ADDRESS + FilePage * 256, 256);
    }
    UDP_Send_CMD(p, 256 + sizeof(struct SFILE_TRANSLATE_TYPE ) + 2, des_ip);
    User_mem_free(p);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：Class_Table_Read
//	函数功能：获得当天课程表信息
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE File_ClassTable_Read_CurDay(void)
{
    uint8 err, i = 0;
    struct CLASS_TABLE_REC *pClass;
    struct REALTIME_TYPE *ptime;

    ptime = RealTime_Get();
    gClassT.week = ptime->week;	//0--6;
    //if(gClassT.week>6)
    //      return FALSE;
    if ((gClassT.week > 6) || (gClassT.week == 0)) //2013.2.28解决周日自动开机问题
        return FALSE;
    memset(gClassT.Class_Table_Rec, 0, 256);
    do
    {
        err = AT24C16_Read(gClassT.Class_Table_Rec, CLASS_TABLE_ADDRESS + gClassT.week * 256, 256);
        if (i++ > 5)
            return FALSE;
    }
    while(err == FALSE);

    gClassT.Curr_rec = 0;
    i = 0;
    while(1)//检查定时有效
    {
        pClass = (struct CLASS_TABLE_REC *)(gClassT.Class_Table_Rec + 3 + i * sizeof(struct CLASS_TABLE_REC));
        //if ((pClass->class_begin.hour==0)||(pClass->class_begin.hour>=24))
        if  (pClass->class_begin.hour >= 24)
            break;
        //if ((pClass->class_over.hour==0)||(pClass->class_over.hour>=24))
        if  (pClass->class_over.hour >= 24)
            break;
        if  ((pClass->class_begin.min > 59) || (pClass->class_over.min > 59))
            break;
        if  (i  >=  7)
            break;
        i++;
        gClassT.Curr_rec++;
        IO_FlagLingt_Flash();
    }
    return err;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：Class_Table_Check
//	函数功能：课程表检测,检测是否有定时开关机信息
//	入口参数：
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE Class_Table_Check(void)
{
    struct CLASS_TABLE_REC *pClass;
    struct REALTIME_TYPE *ptime;
    BYTE *pBase, i, buf[3];
    struct MSG_REV_TYPE Rev;
    if ((gClassT.Curr_rec == 0) || (gClassT.Curr_rec > 7))
        return FALSE;
    Rev.Type = DEV_XCOM_TIME;
    pBase = (BYTE *)(gClassT.Class_Table_Rec + 3);
    ptime = RealTime_Get();

    for (i = 0; i <= gClassT.Curr_rec; i++)
    {
        pClass = (struct CLASS_TABLE_REC *)(pBase + i * sizeof(struct CLASS_TABLE_REC));
        if ((pClass->class_begin.hour == ptime->hour) && (pClass->class_begin.min == ptime->min))
        {
            if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
            {
                buf[0] = MSG_DEV_MAINBARD;
                buf[1] = MSG_OPTION_CLASS_BEGIN;
                buf[2] = 3;
                Msg_send(buf, 3, (uint8 *)&Rev);
                return TRUE;
            }
        }
        else if ((pClass->class_over.hour == ptime->hour) && (pClass->class_over.min == ptime->min))
        {
            if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
                return FALSE;
            buf[0] = MSG_DEV_MAINBARD;
            buf[1] = MSG_OPTION_CLASS_OVER;
            buf[2] = 0;
            Msg_send(buf, 3, (uint8 *)&Rev);
            return TRUE;
        }
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：Device_Stat_Option
//	函数功能：设备动作信息统计(只计数，不写FLASH)
//	入口参数：
//			BYTE Device:设备码  BYTE option:动作码
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_Stat_Option(BYTE Device, BYTE option)
{
    uint8 i;
    uint32 tmp;
    if (gStat_Rec.Device == 0)
        return;
    if ((option != MSG_OPTION_STAT_START) && (option != MSG_OPTION_STAT_END))
        return;
    for (i = 0; i < gStat_Rec.Device; i++)
    {
        if (gStat_Rec.Rec[i].Device_Type == Device)
        {
            if (option == MSG_OPTION_STAT_START)
            {
                gStat_Rec.flag[i] &= ~STAT_DEVICE_OVER;
                gStat_Rec.DEVICE_STAT_Start_Time[i] = RealTime_Get_Minute();
                break;
            }
            else
            {
                if (gStat_Rec.flag[i]&STAT_DEVICE_OVER)
                    break;
                gStat_Rec.flag[i] |= STAT_DEVICE_OVER;
                tmp = RealTime_Get_Minute();
                gStat_Rec.Rec[i].Device_Stat_Count++;
                gStat_Rec.Rec[i].Device_Stat_Time += tmp - gStat_Rec.DEVICE_STAT_Start_Time[i];
                break;
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////
//	函数名称：Device_Stat_Save
//	函数功能：保存写统计表(设备统计表)
//	入口参数：
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Device_Stat_Save(void)
{
    uint8 i, *p;
    uint32 *pint;
    p = (BYTE *)User_mem_allco(256);
    if (p == NULL)
        return ;
    memset(p, 0, 128);
    for (i = 0; i < gStat_Rec.Device; i++)
    {
        if ((gStat_Rec.flag[i]&STAT_DEVICE_OVER) != STAT_DEVICE_OVER)
        {
            Device_Stat_Option(gStat_Rec.Device, MSG_OPTION_STAT_END);
        }
        pint = (uint32 *)(p + i * 8);
        *pint = gStat_Rec.Rec[i].Device_Stat_Count;
        *(pint + 1) = gStat_Rec.Rec[i].Device_Stat_Time;
    }
    AT24C16_Write(p, DEVICE_USE_STAT_TIME, 128);
    User_mem_free(p);
}

/////////////////////////////////////////////////////////////////////
//	函数名称：Device_Use_Stat_Read
//	函数功能：读统计信息表(设备统计表)
//	入口参数：
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE Device_Use_Stat_Read(void)
{
    uint8 *p, i = 0, err, *pbuf;
    uint32 *pint;
    struct DEVICE_STAT_REC_TYPE *pStat;
    p = (BYTE *)User_mem_allco(256);
    if (p == NULL)
        return FALSE;
    do
    {
        err = AT24C16_Read(p, DEVICE_USE_STAT_TIME, 128);
        if (i++ > 5)
        {
            User_mem_free(p);
            return FALSE;
        }
    }
    while(err == FALSE);
    i = 0;
    memset((BYTE *)&gStat_Rec, 0, sizeof(struct DEVICE_STAT_REC_TYPE));
    pbuf = (BYTE *)Get_ClassInfo_Table(0xfa) + 4;
    while(1)
    {
        pStat = (struct DEVICE_STAT_REC_TYPE *)(pbuf + i * sizeof(struct DEVICE_STAT_REC_TYPE));
        if ((pStat->Device_Type < 16) || (pStat->Device_Type > 100))
            break;
        gStat_Rec.flag[i] = 0;
        gStat_Rec.Rec[i].Device_Count = pStat->Device_Count;
        gStat_Rec.Rec[i].Device_Type = pStat->Device_Type;
        pint = (uint32 *)(p + i * 8);
        gStat_Rec.Rec[i].Device_Stat_Count = pStat->Device_Stat_Count + *pint;
        gStat_Rec.Rec[i].Device_Stat_Time = pStat->Device_Stat_Time + *(pint + 1);
        memcpy((BYTE *)gStat_Rec.Rec[i].Device_Use_Time, (BYTE *)pStat->Device_Use_Time, 6);
        if (i++ > 14)
            break;
        gStat_Rec.Device++;
        IO_FlagLingt_Flash();
    };
    User_mem_free(p);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：void File_UserProgram_Write(BYTE Page,BYTE *buf)
//	函数功能： 用户代码的烧写(升级)
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void File_UserProgram_Write(uint16 Page, BYTE *buf, uint8 *des_ip)
{
    uint32 *pint;
    uint8 *p;

#if OS_CRITICAL_METHOD == 3        /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr;             /* Prevent compiler warning                     */
#endif

    pint = (uint32 *)0x00000000;
    p = (uint8 *)0x00000004;
    OS_ENTER_CRITICAL();
    *pint = CMD_ABLE_AUDIO;
    memcpy(p, des_ip, 4);
    UART_printf("User UpData--Run\n\n");
    MEMMAP = 0x01;
    Reset_BootLoader();
    cpu_sr = cpu_sr;
}

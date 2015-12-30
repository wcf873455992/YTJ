#include "includes.h"


OS_EVENT *QXCOM_rev;//通讯队列
void *QXCOM[XCOM_QMSG_SIZE];//通讯队列


void *QXCOM0_Sbuf[XCOM_QMSG_SIZE];//通讯队列
void *QXCOM1_Sbuf[XCOM_QMSG_SIZE];//通讯队列
void *QXCOM2_Sbuf[XCOM_QMSG_SIZE];//通讯队列
void *QXCOM3_Sbuf[XCOM_QMSG_SIZE];//通讯队列
void *QXCOM4_Sbuf[XCOM_QMSG_SIZE];//通讯队列
void *QXCOM5_Sbuf[XCOM_QMSG_SIZE];//通讯队列
//通讯端口
struct COMM_XCOM XCOM_PORT[DEV_XCOM_MAX_SIZE];

unsigned char CoordinateBuf[8];

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void *Get_XCOM_Port(uint8 no)
{
    return &XCOM_PORT[no];
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 XCOM_Wait_Ack(uint8 PortNum, uint16 WaitTime)
{
    uint8 err = 0x07;
    if (PortNum > DEV_XCOM_SCAMERA) return err;
    if (XCOM_PORT[PortNum].flag != NULL)
        OSSemPend(XCOM_PORT[PortNum].flag, WaitTime, &err);
    return err;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Xcom_sum(uint8 *pdata, uint8 lg)
{
    uint8 i;
    uint8 sum = 0;
    for (i = 0; i < lg; i++)
        sum += *(pdata + i);
    sum = ~sum;
    sum += 1;
    return sum;
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_COMM(void *pdata)
{
    BYTE *p;
    struct MSG_OPTION_COMM_TYPE *pc;
    struct _pkst  TxData;

    struct MSG_REV_TYPE *prev;
    prev = ( struct MSG_REV_TYPE *)pdata;
    pc = (struct MSG_OPTION_COMM_TYPE *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    p = (BYTE *)((BYTE *)pdata + sizeof(struct MSG_OPTION_COMM_TYPE ) + sizeof(struct MSG_REV_TYPE));
    if (pc->PortNum == DEV_XCOM_UDP)
    {
        TxData.DAPTR = (BYTE *)((BYTE *)pdata + sizeof(struct MSG_OPTION_COMM_TYPE) + sizeof(struct MSG_REV_TYPE));
        TxData.length = pc->lg;
        TxData.STPTR = NULL;
        //OSAPIBlockEnter(XCOM_PORT[pc->PortNum].flag, 0);
        Udp_SendPacked(&TxData, prev->des_ip, IC2_COMMAND_PORT);
        //OSAPIBlockExit(XCOM_PORT[pc->PortNum].flag);//串口解锁
    }
    else
    {
        //OSAPIBlockEnter(XCOM_PORT[pc->PortNum].flag, 0);
        //UART_Send_Data(pc->PortNum,p,pc->lg);
        UART_Write(pc->PortNum, p, pc->lg);
        //OSAPIBlockExit(XCOM_PORT[pc->PortNum].flag);//串口解锁
    }
    return 0;
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：PortNum串口号pdata数据指针lg数据长度CMD命令sub子抵制
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void XCOM_Data_send(uint8	PortNum, uint8 *pdata, uint8 lg, uint8 CMD, uint8 sub, uint8 repeat)
{
    uint8 *p, rev_buf[10];
    struct ZY_XCOM  *pzy;
    struct MSG_OPTION_COMM_TYPE *pc;

    if (PortNum > DEV_XCOM_MAX_SIZE)		 return;
    if (XCOM_PORT[PortNum].PROTOCOL != DEV_PROTOCOL_ZYCOM)
    {
        UART_Write(PortNum, pdata, lg);
        return ;
    }
    else
    {
        p = (uint8 *)User_mem_allco(sizeof(struct MSG_OPTION_COMM_TYPE) + ZY_XCOM_PROTOCOL_SIZE + 1 + lg);
        if (p == NULL)	return; //内存溢出
        pc = (struct MSG_OPTION_COMM_TYPE *)p;
        pc->PortNum = PortNum;
        pc->Frame = (sub << 4) + XCOM_PORT[PortNum].frame_count;
        pc->msg.device = MSG_DEV_MAINBARD;
        pc->msg.option = MSG_OPTION_COMM;
        pc->lg = ZY_XCOM_PROTOCOL_SIZE + 1 + lg;
        pzy = (struct ZY_XCOM *)(p + sizeof(struct MSG_OPTION_COMM_TYPE));
        pzy->flag = ZY_XCOM_FLAG_START;
        pzy->frame_CMD = CMD + (XCOM_PORT[PortNum].frame_count << 4);
        XCOM_PORT[PortNum].frame_count++;
        XCOM_PORT[PortNum].frame_count &= 0x0f;
        pzy->type_lg = (sub << 4) + lg;
        memcpy(p + sizeof(struct MSG_OPTION_COMM_TYPE) + sizeof(struct ZY_XCOM), pdata, lg);
        *(p + sizeof(struct MSG_OPTION_COMM_TYPE) + sizeof(struct ZY_XCOM) + lg) = Xcom_sum((uint8 *)pzy, lg + ZY_XCOM_PROTOCOL_SIZE);
        if (repeat)
        {
            rev_buf[0] = PortNum;
            if (CMD != XCOM_CMD_RECEIVE_DATA)
                Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 2, p, pc->lg + sizeof(struct MSG_OPTION_COMM_TYPE), 1, rev_buf);
        }
        //		UART_Send_Data(PortNum,(uint8 *)pzy,lg+ZY_XCOM_PROTOCOL_SIZE+1);
        UART_Write(PortNum, (uint8 *)pzy, lg + ZY_XCOM_PROTOCOL_SIZE + 1);
        User_mem_free(p);
    }
    //	OSAPIBlockExit(XCOM_PORT[PortNum].flag);//串口解锁
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：PortNum: 串口号            Sub: 子地址     CMD:  命令           Chanle :等待数据通道号等待响应=0
//			             buf:发送数据返回数据         lg: 发送/返回数据长度 waittype
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 XCOM_Data_Read(uint8 PortNum, uint8 sub, uint8 cmd, uint8 *buf, uint8 *plg, uint8 WaitType)
{
    uint8 tmp;
    struct COMM_XCOM *pxcom;
    pxcom = (struct COMM_XCOM *)&XCOM_PORT[PortNum];
    if (pxcom->PROTOCOL != DEV_PROTOCOL_ZYCOM)  return 0xfe;
    pxcom->Rflag = WaitType;
    XCOM_Data_send(PortNum, buf, *plg, cmd, sub, FALSE);
    if (WaitType == XCOM_READ_FLAG_WAIT_DATA)
    {


        pxcom->Rdata_lg = 0;
        pxcom->pbuf = buf;
        pxcom->RCMDNo = *buf;
        tmp = XCOM_Wait_Ack(PortNum, 80);
        if (tmp == OS_NO_ERR)
            *plg = pxcom->Rdata_lg;
        else
            *plg = 0;
        pxcom->Rflag = 0;
        pxcom->Rdata_lg = 0;
        pxcom->pbuf = NULL;
        pxcom->RCMDNo = 0;
        return tmp;
    }
    else
    {
        tmp = XCOM_Wait_Ack(PortNum, 10);
        pxcom->Rflag = 0;
        pxcom->Rdata_lg = 0;
        pxcom->pbuf = NULL;
        pxcom->RCMDNo = 0;
        return tmp;

    }
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Xcom_set(uint8 PortNum, uint32 Baudrate, uint8 poctl, uint8 LineContrl)
{
    if (PortNum > DEV_XCOM_MAX_SIZE) return ;
    XCOM_PORT[PortNum].PortNum = PortNum;
    XCOM_PORT[PortNum].BPS = Baudrate;
    XCOM_PORT[PortNum].frame_count = 0;
    XCOM_PORT[PortNum].address = 0;
    XCOM_PORT[PortNum].PROTOCOL = poctl;
    XCOM_PORT[PortNum].linecontrl = LineContrl;
    UART_set_bps(PortNum, Baudrate, LineContrl);


}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void XCOM_init(void)
{
    uint8 i;

    QXCOM_rev = OSQCreate(&QXCOM[0], XCOM_QMSG_SIZE);
    for (i = 0; i < DEV_XCOM_MAX_SIZE; i++)
        Mem_Set((uint8 *)&XCOM_PORT[i], 0, sizeof(struct COMM_XCOM));

    for (i = 0; i < 6; i++)
    {
        XCOM_PORT[i].flag = OSSemCreate(0);
        XCOM_PORT[i].UART_send_end = OSSemCreate(0);
    }

    XCOM_PORT[6].flag = OSSemCreate(0);
    XCOM_PORT[6].UART_send_end = NULL;
    XCOM_PORT[6].PROTOCOL = DEV_PROTOCOL_ZYUDP;

    XCOM_PORT[0].Q_send = OSQCreate(&QXCOM0_Sbuf[0], XCOM_QMSG_SIZE);
    XCOM_PORT[1].Q_send = OSQCreate(&QXCOM1_Sbuf[0], XCOM_QMSG_SIZE);
    XCOM_PORT[2].Q_send = OSQCreate(&QXCOM2_Sbuf[0], XCOM_QMSG_SIZE);
    XCOM_PORT[3].Q_send = OSQCreate(&QXCOM3_Sbuf[0], XCOM_QMSG_SIZE);
    XCOM_PORT[4].Q_send = OSQCreate(&QXCOM4_Sbuf[0], XCOM_QMSG_SIZE);
    XCOM_PORT[5].Q_send = OSQCreate(&QXCOM5_Sbuf[0], XCOM_QMSG_SIZE);
    Xcom_set(DEV_XCOM_DW, 19200, DEV_PROTOCOL_ZYCOM, 0x03);
    Xcom_set(DEV_XCOM_TB, 19200, DEV_PROTOCOL_ZYCOM, 0x03);
    Xcom_set(DEV_XCOM_TYJ, 19200, DEV_PROTOCOL_NONE, 0x03);
    Xcom_set(DEV_XCOM_SOUND, 9600, DEV_PROTOCOL_NONE, 0x03);
    Xcom_set(DEV_XCOM_MB, 9600, DEV_PROTOCOL_ZYCOM, 0x03);
    Xcom_set(DEV_XCOM_SCAMERA, 2400, DEV_PROTOCOL_NONE, 0x03);



}
////////////////////////////////////////////////////////////////////
//	函数名称：定位跟踪
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void XCOM_Send_ACK(uint8 *p)
{
    struct ZY_XCOM_DATA dbuf;
    struct ZY_XCOM *pxcom;
    struct XCOM_REV_TYPE *prev;
    uint8 PortNum;
    prev = (struct XCOM_REV_TYPE *)p;
    PortNum = prev->XcomPort;
    if (PortNum > DEV_XCOM_UDP) return;
    pxcom = ( struct ZY_XCOM *)(p + sizeof(struct XCOM_REV_TYPE));
    if (PortNum != DEV_XCOM_UDP)
    {
        //OSAPIBlockEnter(XCOM_PORT[PortNum].flag, 0);
        dbuf.flag = ZY_XCOM_FLAG_ACK;
        dbuf.frame_CMD = pxcom->frame_CMD;
        dbuf.type_lg = pxcom->type_lg & 0xf0;
        dbuf.data[0] = Xcom_sum((BYTE *)&dbuf, 3);
        // UART_Send_Data( PortNum,(BYTE *)&dbuf,4);
        UART_Write( PortNum, (BYTE *)&dbuf, 4);
        //OSAPIBlockExit(XCOM_PORT[PortNum].flag);
    }

}
////////////////////////////////////////////////////////////////////
//	函数名称：定位跟踪
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void XCOM_DW(uint8 *pdata)
{
    uint8 *p, buf[10], frame; //,cmd;
    struct ZY_XCOM *pxcom;
    struct XCOM_REV_TYPE *prev;
    struct DEVICE_STATUS_TYPE *ps;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    prev = ( struct XCOM_REV_TYPE *)pdata;
    pxcom = ( struct ZY_XCOM *)(pdata + sizeof( struct XCOM_REV_TYPE));
    p = pdata + sizeof( struct XCOM_REV_TYPE) + sizeof(struct ZY_XCOM);
    frame = pxcom->frame_CMD;
    if (pxcom->flag == ZY_XCOM_FLAG_ACK)
    {
        if ((pxcom->type_lg >> 4) < 4)
            ps->Power_Type = 2;
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_COMM;
        buf[2] = ((pxcom->frame_CMD >> 4) & 0x0f) + (pxcom->type_lg & 0xf0);
        buf[3] = prev->XcomPort;
        Task_Rec_Del(buf, 4); //删除消息列表中的重发消息

    }
    else if (pxcom->flag == ZY_XCOM_FLAG_START)
    {
        XCOM_Send_ACK(pdata);
        DW_CMD_Contrl(p);
    }
    frame = frame;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void XCOM_MB(uint8 *pdata)
{
    uint8 *p, sub, buf[8], frame;
    struct ZY_XCOM *pxcom;
    struct XCOM_REV_TYPE *prev;
    struct DEVICE_STATUS_TYPE *ps;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    prev = ( struct XCOM_REV_TYPE *)pdata;
    pxcom = ( struct ZY_XCOM *)(pdata + sizeof( struct XCOM_REV_TYPE));
    p = pdata + sizeof( struct XCOM_REV_TYPE) + sizeof(struct ZY_XCOM);
    sub = pxcom->type_lg >> 4;
    frame = pxcom->frame_CMD;
    if (pxcom->flag == ZY_XCOM_FLAG_ACK)
    {
        if ((pxcom->type_lg >> 4) < 4)
            ps->Power_Type = 2;
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_COMM;
        buf[2] = ((pxcom->frame_CMD >> 4) & 0x0f) + (pxcom->type_lg & 0xf0);
        buf[3] = prev->XcomPort;
        Task_Rec_Del(buf, 4); //删除消息列表中的重发消息
        //IP_printf("响应桢号:%d子地址:%d",(pxcom->frame_CMD>>4),(pxcom->type_lg&0xf0));
    }
    else if (pxcom->flag == ZY_XCOM_FLAG_START)
    {
        XCOM_Send_ACK(pdata);
        if (sub == DEVICE_SUB_MB)
        {
            key_msg_rev(p);
            ps->DEV_Link_State |= DEV_LINK_KBLK;
        }
        else
        {
            POWER_msg_rev(p, sub);
        };

    }
    frame = frame;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void XCOM_SCAMERA(uint8 *pdata)
{
    uint8 *ip, buf[16], tmp, *p, i, j;
    struct XCOM_REV_TYPE *prev;
    struct DEVICE_INFO *pdev;
    Mem_Set(buf, 0, 16);
    p = (uint8 *)pdata + sizeof(struct XCOM_REV_TYPE);
    prev = (struct XCOM_REV_TYPE *)pdata;
    ip = Get_DesIp(DEVICE_SUB_BJCard);
    if (ARP_Rec_Find(ip) == NULL)
    {
        Arp_Request(ip);
    }
    for (i = 0; i < prev->XcomLengh; i++)
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("camera recv = %x", *(p + i));
    }
    for (i = 0; i < prev->XcomLengh; i++)
    {
        if (*(p + i) == 0xff) //检查球机返回参数是否是Pelco D协议
        {
            if(IsImageRecognition(p + i + 1))
            {
                if(*(p + i + 5) == 0xD1 || *(p + i + 5) == 0x51)
                {
                    buf[0] = MSG_DEV_MAINBARD;
                    buf[1] = MSG_OPTION_SCAMERA_BackVAL;
                    buf[3] = 1;
                    UDP_Send_CMD(buf, 16, ip);
                    return;
                }
                else if(*(p + i + 5) == 0xD0 || *(p + i + 5) == 0x50)
                {
                    buf[0] = MSG_DEV_MAINBARD;
                    buf[1] = MSG_OPTION_SCAMERA_BackVAL;
                    buf[3] = 0;
                    UDP_Send_CMD(buf, 16, ip);
                    return;
                }
            }

            tmp = 0;
            //检查Pelco D校验和
            for (j = 1; j < 6; j++)
                tmp += *(p + i + j);
            if (tmp == *(p + i + 6))
            {
                Mem_Copy(buf + 4, p + i, 7);
                buf[3] = *(p + i + 1);
                break;
            }
        }
        else if (*(p + i) == 0x55) //
        {
            tmp = 0;
            Mem_Copy(buf + 4, p + i, 11);
            buf[3] = *(p + i + 2);
            break;
            //  for (j=1;j<10;j++)
            //	tmp+=*(p+i+j);
            // if (tmp==*(p+i+10))
            //	{Mem_Copy(buf+4,p+i,11);
            //	 tmp=*(p+i+2);
            //		 break;
            //	    	}
        }
        else if(0 == (*(p + i) & 0x0f))
        {
            if(*(p + i + 1) == 0x50)
            {
                if(*(p + i + 6) == 0xff)
                {
                    buf[0] = MSG_DEV_MAINBARD;
                    buf[1] = MSG_OPTION_SCAMERA_BackVAL;
                    buf[2] = DEV_XCOM_SCAMERA - 1;
                    buf[3] = (*(p + i) >> 4) - 8;
                    pdev = (struct DEVICE_INFO *)Device_Rec_Find(&buf[2], DEV_FIND_XCOM_SubADDr);
                    if (pdev == NULL)
                    {
                        if (Get_Debug(DEV_DEBUG_MSG))
                            IP_printf("设备表中未发现%d地址的球机", buf[3]);
                        return ;
                    }
                    buf[2] = 22;
                    buf[3] = pdev->dev_id;
                    Mem_Copy(buf + 4, CoordinateBuf, 8);
                    Mem_Copy(buf + 12, p + i + 2, 4);
                    UDP_Send_CMD(buf, 16, ip);
                    return;
                }
                else if(*(p + i + 10) == 0xff)
                {
                    if (Get_Debug(DEV_DEBUG_MSG))
                        IP_printf("收到球机");
                    Mem_Set(CoordinateBuf, 0, 8);
                    Mem_Copy(CoordinateBuf, p + i + 2, 8);
                    return;
                }
            }
        }
    }
    if ((buf[4] != 0x55) && (buf[4] != 0xff))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("错误的球机返回参数%2x", buf[4]);
        return ;
    }
    buf[2] = DEV_XCOM_SCAMERA - 1;
    pdev = (struct DEVICE_INFO *)Device_Rec_Find(&buf[2], DEV_FIND_XCOM_SubADDr);
    if (pdev == NULL)
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("设备表中未发现%d地址的球机", buf[3]);
        return ;
    }

    if (Get_Debug(DEV_DEBUG_MSG))
    {
        IP_printf("球机返回参数长度%d", prev->XcomLengh);
        IP_printf("球机返回参数发送到编辑卡IP:%d.%d.%d.%d", *ip, *(ip + 1), *(ip + 2), *(ip + 3));
    }

    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_SCAMERA_BackVAL;
    buf[3] = pdev->dev_id;
    if (buf[4] == 0xff)
    {
        if ((buf[6] == 0) && (buf[7] == 0x59)) //当前水平角度
        {
            buf[2] = 16;
            buf[4] = buf[8];
            buf[5] = buf[9];
        }
        if ((buf[6] == 0) && (buf[7] == 0x5b))
        {
            buf[2] = 28;
            buf[4] = buf[8];
            buf[5] = buf[9];
        }
    }

    UDP_Send_CMD(buf, 16, ip);
    return;

}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void XCOM_SCAMERA1(uint8 *pdata)
{
    uint8 *ip, buf[16], tmp, *p, i, j;
    struct XCOM_REV_TYPE *prev;
    struct DEVICE_INFO *pdev;
    Mem_Set(buf, 0, 16);
    p = (uint8 *)pdata + sizeof(struct XCOM_REV_TYPE);
    prev = (struct XCOM_REV_TYPE *)pdata;
    ip = Get_DesIp(DEVICE_SUB_BJCard);
    if (ARP_Rec_Find(ip) == NULL)
    {
        Arp_Request(ip);
    }
    for (i = 0; i < prev->XcomLengh; i++)
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("camera recv = %x", *(p + i));
    }
    for (i = 0; i < prev->XcomLengh; i++)
    {
        if (*(p + i) == 0xff) //检查球机返回参数是否是Pelco D协议
        {
            if(IsImageRecognition(p + i + 1))
            {
                if(*(p + i + 5) == 0xD1 || *(p + i + 5) == 0x51)
                {
                    buf[0] = MSG_DEV_MAINBARD;
                    buf[1] = MSG_OPTION_SCAMERA_BackVAL;
                    buf[3] = 1;
                    UDP_Send_CMD(buf, 16, ip);
                    return;
                }
                else if(*(p + i + 5) == 0xD0 || *(p + i + 5) == 0x50)
                {
                    buf[0] = MSG_DEV_MAINBARD;
                    buf[1] = MSG_OPTION_SCAMERA_BackVAL;
                    buf[3] = 0;
                    UDP_Send_CMD(buf, 16, ip);
                    return;
                }
            }

            tmp = 0;
            //检查Pelco D校验和
            for (j = 1; j < 6; j++)
                tmp += *(p + i + j);
            if (tmp == *(p + i + 6))
            {
                Mem_Copy(buf + 4, p + i, 7);
                buf[3] = *(p + i + 1);
                break;
            }
        }
        else if (*(p + i) == 0x55) //
        {
            tmp = 0;
            Mem_Copy(buf + 4, p + i, 11);
            buf[3] = *(p + i + 2);
            break;
            //  for (j=1;j<10;j++)
            //	tmp+=*(p+i+j);
            // if (tmp==*(p+i+10))
            //	{Mem_Copy(buf+4,p+i,11);
            //	 tmp=*(p+i+2);
            //		 break;
            //	    	}
        }
        else if(0 == (*(p + i) & 0x0f))
        {
            if(*(p + i + 1) == 0x50)
            {
                if(*(p + i + 6) == 0xff)
                {
                    if (Get_Debug(DEV_DEBUG_MSG))
                        IP_printf("收到球机返回ZOOM");
                    buf[0] = MSG_DEV_MAINBARD;
                    buf[1] = MSG_OPTION_SCAMERA_BackVAL;
                    buf[2] = 6;
                    buf[3] = (*(p + i) >> 4) - 8;
                    pdev = (struct DEVICE_INFO *)Device_Rec_Find(&buf[2], DEV_FIND_XCOM_SubADDr);
                    if (pdev == NULL)
                    {
                        if (Get_Debug(DEV_DEBUG_MSG))
                            IP_printf("设备表中未发现%d地址的球机", buf[3]);
                        return ;
                    }
                    buf[2] = 22;
                    buf[3] = pdev->dev_id;
                    Mem_Copy(buf + 4, CoordinateBuf, 8);
                    Mem_Copy(buf + 12, p + i + 2, 4);
                    UDP_Send_CMD(buf, 16, ip);
                    return;
                }
                else if(*(p + i + 10) == 0xff)
                {
                    if (Get_Debug(DEV_DEBUG_MSG))
                        IP_printf("收到球机返回XY坐标");
                    Mem_Set(CoordinateBuf, 0, 8);
                    Mem_Copy(CoordinateBuf, p + i + 2, 8);
                    return;
                }
            }
        }
    }
    if ((buf[4] != 0x55) && (buf[4] != 0xff))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("错误的球机返回参数%2x", buf[4]);
        return ;
    }
    buf[2] = DEV_XCOM_SCAMERA - 1;
    pdev = (struct DEVICE_INFO *)Device_Rec_Find(&buf[2], DEV_FIND_XCOM_SubADDr);
    if (pdev == NULL)
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("设备表中未发现%d地址的球机", buf[3]);
        return ;
    }

    if (Get_Debug(DEV_DEBUG_MSG))
    {
        IP_printf("球机返回参数长度%d", prev->XcomLengh);
        IP_printf("球机返回参数发送到编辑卡IP:%d.%d.%d.%d", *ip, *(ip + 1), *(ip + 2), *(ip + 3));
    }

    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_SCAMERA_BackVAL;
    buf[3] = pdev->dev_id;
    if (buf[4] == 0xff)
    {
        if ((buf[6] == 0) && (buf[7] == 0x59)) //当前水平角度
        {
            buf[2] = 16;
            buf[4] = buf[8];
            buf[5] = buf[9];
        }
        if ((buf[6] == 0) && (buf[7] == 0x5b))
        {
            buf[2] = 28;
            buf[4] = buf[8];
            buf[5] = buf[9];
        }
    }

    UDP_Send_CMD(buf, 16, ip);
    return;

}

////////////////////////////////////////////////////////////////////
//	函数名称：刷卡开机处理
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void XCOM_ICREADER(uint8 *pdata)
{
    RFIC_K103_Mode(pdata);
}

void XCOM_HDMI(uint8 *pdata)
{
    uint8 *p; //,cmd;
    struct XCOM_REV_TYPE *prev;
    
    prev = ( struct XCOM_REV_TYPE *)pdata;   
    p = ( uint8 *)(pdata + sizeof( struct XCOM_REV_TYPE));
    //if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("COM HDMI  receive state:%s",*p, *(p+1),*(p+2) );
    
}

void Switch_HDMI(uint8 in, uint8 out)
{
  uint8 data[5]={0x05, 0x55, 0x19, 0xff, 0x77};
  uint8 cmd[4][4]={ 0x09, 0x1D, 0x1F, 0x0D,     //out1,in 1-4
                    0x17, 0x12, 0x59, 0x08,     //out2,in 1-4
                    0x5E, 0x06, 0x05, 0x03,     //out3,in 1-4
                    0x18, 0x44, 0x0F, 0x51      //out4,in 1-4
                    };
  if( (in > IN_ZBFWQ)||(out > OUT_ZBFWQ)) return;
  
  data[3] = cmd[out][in];
  //if (Get_Debug(DEV_DEBUG_MSG))
      IP_printf("send HDMI cmd:0x%2x 0x%x 0x%x 0x%2x 0x%x\n",data[0],data[1],data[2],data[3],data[4]);
  UART_Write(DEV_XCOM_SOUND, (uint8 *)&data, sizeof(data));
  OSTimeDlyHMSM(0, 0, 1, 0);
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void XCOM_Q_Send(uint8 PortNum, uint8 *pdata, uint16 lg)
{
    uint8 *p, err;
    struct XCOM_REV_TYPE *pxrev;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif
    if ((lg == 0) || (pdata == NULL)) return;
    p = (uint8 *)User_mem_allco(lg + sizeof( struct XCOM_REV_TYPE));
    if (p == NULL) return;
    OS_ENTER_CRITICAL();
    pxrev = ( struct XCOM_REV_TYPE *)p;
    pxrev->XcomPort = PortNum;
    pxrev->XcomLengh = lg;
    memcpy(p + sizeof( struct XCOM_REV_TYPE), pdata, lg);
    err = OSQPost(QXCOM_rev, (void *) p);
    if (err != OS_NO_ERR)	User_mem_free(p);
    OS_EXIT_CRITICAL();
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 UART_Write(uint8 Port, uint8 *pdata, uint8 lg)
{
    uint8 *p, err;
    struct XCOM_REV_TYPE *pxrev;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif
    if ((lg == 0) || (pdata == NULL)) return FALSE;
    p = (uint8 *)User_mem_allco(lg + sizeof( struct XCOM_REV_TYPE));
    if (p == NULL) return  FALSE;
    Mem_Clr(p, lg + sizeof( struct XCOM_REV_TYPE));
    OS_ENTER_CRITICAL();
    pxrev = ( struct XCOM_REV_TYPE *)p;
    pxrev->XcomPort = Port;
    pxrev->XcomLengh = lg;
    Mem_Copy(p + sizeof( struct XCOM_REV_TYPE), pdata, lg);
    err = OSQPost(XCOM_PORT[Port].Q_send, (void *) p);
    if (err != OS_NO_ERR)
    {
        User_mem_free(p);
        OS_EXIT_CRITICAL();
        return FALSE;
    }
    OS_EXIT_CRITICAL();
    return TRUE;

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void TASK_UART0Send(void *pdata)
{
    uint8 err, *p;
    struct XCOM_REV_TYPE *pxrev;
    pdata = pdata;
    for (;;)
    {
        p = (uint8 *)OSQPend(XCOM_PORT[0].Q_send, 0, &err);
        if ((err == OS_NO_ERR) && (p != NULL))
        {
            pxrev = (struct XCOM_REV_TYPE *)p;
            UART_Send_Data(pxrev->XcomPort, p + sizeof(struct XCOM_REV_TYPE), pxrev->XcomLengh);
            OSTimeDly(1);
            User_mem_free(p);
        }

    }

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void TASK_UART1Send(void *pdata)
{
    uint8 err, *p;
    struct XCOM_REV_TYPE *pxrev;
    pdata = pdata;
    for (;;)
    {
        p = (uint8 *)OSQPend(XCOM_PORT[1].Q_send, 0, &err);
        if ((err == OS_NO_ERR) && (p != NULL))
        {
            pxrev = (struct XCOM_REV_TYPE *)p;
            UART_Send_Data(pxrev->XcomPort, p + sizeof(struct XCOM_REV_TYPE), pxrev->XcomLengh);
            //OSTimeDly(1);
            OSTimeDlyHMSM(0, 0, 1, 0);//add wcf 修复投影机串口码发送间隔太短导致关机失败
            User_mem_free(p);
        }

    }

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void TASK_UART2Send(void *pdata)
{
    uint8 err, *p;
    struct XCOM_REV_TYPE *pxrev;
    pdata = pdata;
    for (;;)
    {
        p = (uint8 *)OSQPend(XCOM_PORT[2].Q_send, 0, &err);
        if ((err == OS_NO_ERR) && (p != NULL))
        {
            pxrev = (struct XCOM_REV_TYPE *)p;
            UART_Send_Data(pxrev->XcomPort, p + sizeof(struct XCOM_REV_TYPE), pxrev->XcomLengh);
            OSTimeDly(1);
            User_mem_free(p);
        }

    }

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void TASK_UART3Send(void *pdata)
{
    uint8 err, *p;
    struct XCOM_REV_TYPE *pxrev;
    pdata = pdata;
    for (;;)
    {
        p = (uint8 *)OSQPend(XCOM_PORT[3].Q_send, 0, &err);
        if ((err == OS_NO_ERR) && (p != NULL))
        {
            pxrev = (struct XCOM_REV_TYPE *)p;
            UART_Send_Data(pxrev->XcomPort, p + sizeof(struct XCOM_REV_TYPE), pxrev->XcomLengh);
            OSTimeDly(1);
            User_mem_free(p);
        }

    }

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void TASK_UART4Send(void *pdata)
{
    uint8 err, *p;
    uint16 tmp;
    struct XCOM_REV_TYPE *pxrev;
    struct ZY_XCOM *pable;
    struct COMM_XCOM *pxcom;
    pdata = pdata;
    pxcom = (struct COMM_XCOM *)Get_XCOM_Port(DEV_XCOM_MB);
    for (;;)
    {
        p = (uint8 *)OSQPend(XCOM_PORT[4].Q_send, 0, &err);
        if ((err == OS_NO_ERR) && (p != NULL))
        {
            pxrev = (struct XCOM_REV_TYPE *)p;
            pable = (struct ZY_XCOM *)((uint8 *)p + sizeof(struct XCOM_REV_TYPE));
            UART_Send_Data(DEV_XCOM_MB, p + sizeof(struct XCOM_REV_TYPE), pxrev->XcomLengh);
            if (pable->flag == ZY_XCOM_FLAG_START)
                if ((pable->type_lg & 0xf0) < 0x70) //
                {
                    tmp = (pxcom->BPS / 2400) * 10 * pxrev->XcomLengh;
                    OSTimeDlyHMSM(0, 0, 0, tmp);
                }
            OSTimeDly(1);
            User_mem_free(p);
        }

    }

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void TASK_UART5Send(void *pdata)
{
    uint8 err, *p;
    uint16 tmp;
#if Debug_Scamera_Log  >0
    char *sbuf;
    uint16 strlen;
#endif
    struct XCOM_REV_TYPE *pxrev;
    struct COMM_XCOM *pxcom;


    pdata = pdata;
    pxcom = (struct COMM_XCOM *)Get_XCOM_Port(DEV_XCOM_SCAMERA);

    for (;;)
    {
        p = (uint8 *)OSQPend(XCOM_PORT[5].Q_send, 0, &err);
        if ((err != OS_NO_ERR) || (p == NULL)) continue;

        pxrev = (struct XCOM_REV_TYPE *)p;
        tmp = (pxcom->BPS / 2400) * 20 * pxrev->XcomLengh;
#if Debug_Scamera_Log  >0
        sbuf = (char *)Log_Create(&strlen);
        if (sbuf != NULL)
        {
            strlen += DBG_Str_printf((char *)(sbuf + strlen), "RS485SendCmd Len%d", pxrev->XcomLengh);
            strlen += DBG_HexToChar((uint8 *)p + sizeof(struct MSG_REV_TYPE), (uint8 *)sbuf + strlen, 8);
            Log_Close(sbuf);
        }
#endif
        UART_Send_Data(pxrev->XcomPort, p + sizeof(struct XCOM_REV_TYPE), pxrev->XcomLengh);
        User_mem_free(p);

        if ((tmp > 10) && (pxcom->Rflag & XCOM_READ_FLAG_WAIT_DATA))
            OSTimeDly(tmp / 10);



    }

}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////

void TASK_UART(void *pdata)
{
    uint8 err, *p;
    struct XCOM_REV_TYPE *prev;
    struct BASE_MSG *pmsg;
    struct MSG_REV_TYPE Rev;
    UART_printf("TASK_UART Runing.....\n\t");
    //	IP_printf("通讯线程启动");
    for (;;)
    {
        p = (uint8 *)OSQPend(QXCOM_rev, 0, &err);
        if ((err == OS_NO_ERR) && (p != NULL))
        {
            prev = (struct XCOM_REV_TYPE *)p;
            Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
            switch(prev->XcomPort)
            {
            case DEV_XCOM_DW:
                XCOM_DW(p);
                break;
            case DEV_XCOM_TYJ:
                break;
            case DEV_XCOM_SOUND://音频级联板用作刷卡开机
                //XCOM_Send_ACK(p);
                //XCOM_ICREADER(p);  
                  XCOM_HDMI(p);
                break;
            case DEV_XCOM_TB:
                XCOM_SCAMERA1(p);//同步板(增益卡)用作球机控制
                //TB_msg_rev(p);
                break;
            case DEV_XCOM_MB:
                XCOM_MB(p);
                break;
            case DEV_XCOM_SCAMERA:
                XCOM_SCAMERA(p);
                break;
            case DEV_XCOM_UDP:
                UDP_msg_rev(p);
                break;
            case DEV_XCOM_DEBUG:
                IP_printf("调试信息\n\t");
                Rev.Type = DEV_XCOM_DEBUG;
                pmsg = (struct BASE_MSG *)(p + sizeof(struct XCOM_REV_TYPE) + sizeof(struct MSG_REV_TYPE));
                if ((pmsg->option == MSG_OPTION_DEBUG) || (Get_Debug(DEV_DEBUG_MSG)))
                {
                    Msg_send((uint8 *) (p + sizeof(struct XCOM_REV_TYPE) + sizeof(struct MSG_REV_TYPE)), prev->XcomLengh, (uint8 *)&Rev);
                }
                break;
            }
            User_mem_free(p);

        }

    }

}


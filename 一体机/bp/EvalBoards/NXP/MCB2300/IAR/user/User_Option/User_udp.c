#include "includes.h"
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：一体机命令发送
//	入口参数：pdata:数据  lg=数据长度 CMD_TYPE=CMD_SENDER_ACK 响应
//			Device_Sub:发送设备的IP
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
const uint8 Lan_TestFlag[4] = {0x55, 0x5a, 0xaa, 0xa5};
void LAN_Test_Ack(uint8 *pdata, uint16 lg)
{
    struct LAN_TEST_DATA *ptest;
    struct _pkst ptu;

    ptest = (struct LAN_TEST_DATA *)pdata;

    if (IP_Compare((BYTE *)Lan_TestFlag, ptest->flag))
    {
        ptu.STPTR = NULL;
        ptu.length = lg;
        ptu.DAPTR = pdata;
        Udp_SendPacked(&ptu, ptest->sip, UDP_LAN_TEST_PORT);
        //Udp_SendPacked(&ptu,Get_DesIp(DEVICE_SUB_TEST),UDP_LAN_TEST_PORT);

    }
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：一体机命令发送
//	入口参数：pdata:数据  lg=数据长度 CMD_TYPE=CMD_SENDER_ACK 响应
//			Device_Sub:发送设备的IP
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
extern uint8 PAD_ip[4];
void UDP_Send_CMD(BYTE *pdata, uint16 lg, uint8 *des_ip)
{
    struct  _pkst udata, udatah, *pst;
    struct COMM_XCOM  *pxcom;
    NetP *pnet;
    IC2CMD_Header hcmd;
    struct MSG_OPTION_COMM_TYPE *pc;
    struct MSG_REV_TYPE *prev;
    BYTE *p, *p2, rev_buf[20];
    //封装命令头
    pxcom = (struct COMM_XCOM *)Get_XCOM_Port(DEV_XCOM_UDP);
    p = Get_DesIp(DEVICE_SUB_CLASS); //一体机IP
    hcmd.byteFrameIndex = pxcom->frame_count++;
    hcmd.dwFromIP = IPByte_TO_Dword(p);
    hcmd.byteProtocolType = CMD_SENDER_AD; //cmd or ack
    hcmd.dwSyncHead = CMD_ABLE_ICII;
    hcmd.dwToIP = IPByte_TO_Dword(des_ip);
    hcmd.wDataLen = lg + 28;
    hcmd.Cmd.dwCommand = CMD_AD_SENDMSG;
    memset(hcmd.Cmd.cGUID, 0, 10);
    memcpy(hcmd.Cmd.cGUID, Get_ClassID(), 8);
    //封装数据链
    udatah.DAPTR = (BYTE *)&hcmd;
    udatah.length = sizeof(IC2CMD_Header);
    if (lg != 0)
    {
        udata.STPTR = NULL;
        udata.length = lg;
        udata.DAPTR = pdata,
              udatah.STPTR = &udata;
    }
    else
        udatah.STPTR = NULL;
    Udp_SendPacked(&udatah, des_ip, IC2_COMMAND_PORT);

    //发送协议数据
    pnet = (NetP *)Get_NetPort();
    if (IP_Compare(des_ip, pnet->CMD_IP) && (pnet->CMD_IP_TTL == 0)) return;
    if (IP_Compare(des_ip, pnet->Ser_Ip)) return;
    if (IP_Compare(des_ip, PAD_ip)) return;

    p = (uint8 *)User_mem_allco(sizeof(struct MSG_OPTION_COMM_TYPE) + lg + sizeof(IC2CMD_Header));
    if (p == NULL) return;

    pc = (struct MSG_OPTION_COMM_TYPE *)p;
    pc->msg.device = MSG_DEV_MAINBARD;
    pc->msg.option = MSG_OPTION_COMM;
    pc->Frame = hcmd.byteFrameIndex;
    pc->PortNum = DEV_XCOM_UDP;
    pc->lg = lg + sizeof(IC2CMD_Header);
    prev = (struct MSG_REV_TYPE *)rev_buf;
    prev->Type = DEV_XCOM_UDP;
    memcpy(prev->des_ip, des_ip, 4);
    p2 = p + sizeof(struct MSG_OPTION_COMM_TYPE);
    for (pst = &udatah; pst != NULL; pst = pst->STPTR)
    {
        memcpy(p2, pst->DAPTR, pst->length);
        p2 += pst->length;
    }
    Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 1, p, pc->lg + sizeof(struct MSG_OPTION_COMM_TYPE), 2, rev_buf);
    User_mem_free(p);



    ///OSAPIBlockExit(pxcom->flag);

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void UDP_Send_ACK(BYTE *pdata)
{
    struct  _pkst udatah;
    //struct COMM_XCOM  *pxcom;
    IC2CMD_Header hcmd;
    BYTE *p, buf[4];
    //pxcom=(struct COMM_XCOM  *)Get_XCOM_Port(DEV_XCOM_UDP);
    //OSAPIBlockEnter(pxcom->flag,0);
    memcpy((BYTE *)&hcmd, pdata + sizeof( struct XCOM_REV_TYPE), sizeof(IC2CMD_Header));
    p = Get_DesIp(DEVICE_SUB_CLASS); //一体机IP
    hcmd.byteProtocolType = CMD_SENDER_ACK | CMD_SENDER_AD; //cmd or ack
    hcmd.dwToIP = hcmd.dwFromIP;
    hcmd.dwFromIP = IPByte_TO_Dword(p);
    IPDword_TO_Byte(buf, hcmd.dwToIP);
    hcmd.wDataLen = 28;
    hcmd.Cmd.dwCommand = CMD_AD_RECVMSG;
    memset(hcmd.Cmd.cGUID, 0, 10);
    memcpy(hcmd.Cmd.cGUID, Get_ClassID(), 8);
    udatah.DAPTR = (BYTE *)&hcmd;
    udatah.length = sizeof(IC2CMD_Header);
    udatah.STPTR = NULL;
    Udp_SendPacked(&udatah, buf, IC2_COMMAND_PORT);
    //OSAPIBlockExit(pxcom->flag);
}
////////////////////////////////////////////////////////////////////
//	函数名称：void UDP_msg_rev(uint8 *p)
//	函数功能：网络命令解析
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void UDP_msg_rev(uint8 *pdata)
{
    struct tagIC2CMD_Header cmd;
    NetP *pnet;
    BYTE buf[8], rev_buf[12], *p;
    struct BASE_MSG *pmsg;
    struct MSG_REV_TYPE *prev;
    memcpy((BYTE *)&cmd, pdata + sizeof( struct XCOM_REV_TYPE), sizeof(struct tagIC2CMD_Header));
    pnet = (NetP *)Get_NetPort();
    if (cmd.dwSyncHead != CMD_ABLE_ICII) return;
    IPDword_TO_Byte(buf, cmd.dwToIP);
    if ((cmd.byteProtocolType & CMD_SENDER_ACK) == CMD_SENDER_ACK)
    {

        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_COMM;
        buf[2] = cmd.byteFrameIndex;
        buf[3] = DEV_XCOM_UDP;
        Task_Rec_Del(buf, 4); //删除消息列表中的重发消息
    }
    else
    {
        if (buf[3] == 0xff)
        {
            pmsg = (struct BASE_MSG *)(pdata + sizeof(struct XCOM_REV_TYPE) + sizeof(struct tagIC2CMD_Header));
            if (pmsg->option == MSG_OPTION_FILE_REQ) return;
        }
        UDP_Send_ACK(pdata);
        prev = (struct MSG_REV_TYPE *)rev_buf;
        prev->Type = DEV_XCOM_UDP;
        IPDword_TO_Byte(buf, cmd.dwFromIP);
        memcpy(prev->des_ip, buf, 4);
        /*
        pmsg=(struct BASE_MSG)pdata+sizeof(struct XCOM_REV_TYPE)+sizeof(struct tagIC2CMD_Header);
           p=(uint8 *)pmsg+2;
           if ((pmsg->device==MSG_DEV_MAINBARD)&&(pmsg->option==MSG_OPTION_CLASS_BEGIN))
           	{*p=4;
           	}
           if ((pmsg->device==MSG_DEV_MAINBARD)&&(pmsg->option==MSG_OPTION_CLASS_OVER))
           	{*p=0;
           	}
                      */
        Msg_send((uint8 *)pdata + sizeof(struct XCOM_REV_TYPE) + sizeof(struct tagIC2CMD_Header), cmd.wDataLen, rev_buf);
    }
    pnet = pnet;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Check_UDP_Paket(void *pdata, uint16 lg)
{
    uint8 *p;
    struct COMM_XCOM *pxcom;
    struct tagIC2CMD_Header cmd;
    struct BASE_MSG *pmsg;
    p = (uint8 *)pdata;
    pxcom = (struct COMM_XCOM *)Get_XCOM_Port(DEV_XCOM_UDP);
    Mem_Copy((uint8 *)&cmd, p, sizeof( struct tagIC2CMD_Header));
    if (cmd.dwSyncHead != CMD_ABLE_ICII) return 0;
    if ((cmd.byteProtocolType & CMD_SENDER_ACK) == CMD_SENDER_ACK)
    {
        if ((pxcom->Rflag & XCOM_READ_FLAG_WAIT_ACK) && (pxcom->flag != NULL))
            OSSemPost(pxcom->flag);
    }
    else
    {
        if ((pxcom->Rflag & XCOM_READ_FLAG_WAIT_DATA) != XCOM_READ_FLAG_WAIT_DATA) return 0;
        if (pxcom->flag == NULL) return 0;
        pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct XCOM_REV_TYPE) + sizeof(struct tagIC2CMD_Header));
        if (pmsg->option != pxcom->RCMDNo) return 0;
        pxcom->Rdata_lg = lg - sizeof(struct tagIC2CMD_Header);
        Mem_Copy(pxcom->pbuf, p + sizeof(struct tagIC2CMD_Header), pxcom->Rdata_lg);
        OSSemPost(pxcom->flag);
        UDP_Send_ACK(pdata);
        return TRUE;
    }
    return FALSE;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 UDP_Read(BYTE *pdata, uint16 lg, uint8 *des_ip, uint8 WaitType, uint8 KeyWord)
{
    struct COMM_XCOM *pxcom;
    uint8 err, tmp;
    pxcom = ( struct COMM_XCOM *)Get_XCOM_Port(DEV_XCOM_UDP);
    if (WaitType == XCOM_READ_FLAG_WAIT_DATA)
    {
        if (pxcom->flag != NULL)  return 0;
        pxcom->RCMDNo = KeyWord;
        pxcom->pbuf = pdata;
        pxcom->Rflag = WaitType;
        UDP_Send_CMD(pdata, lg, des_ip);
        OSSemPend(pxcom->flag, 100, &err);
        if (err != OS_NO_ERR) return 0;
        pxcom->RCMDNo = 0;
        pxcom->pbuf = NULL;
        pxcom->Rflag = 0;
        tmp = pxcom->Rdata_lg;
        pxcom->Rdata_lg = 0;
        return tmp;
    }
    else if (WaitType == XCOM_READ_FLAG_WAIT_ACK)
    {
        UDP_Send_CMD(pdata, lg, des_ip);
        if (pxcom->flag == NULL) return FALSE;
        OSSemPend(pxcom->flag, 100, &err);
        pxcom->Rflag = 0;
        return err;
    }
    return 0;
}



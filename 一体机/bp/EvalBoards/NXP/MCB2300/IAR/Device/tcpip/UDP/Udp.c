
#include "includes.h"
#include "../include/cfg_net.h"
/**********************************************************************
**函数原型:	uint16 CheckSumUdp(uint8 *check,uint16 length)
**入口参数:?	check	:	被校验数据起始地址
		length	:	被校验数据长度，单位字
**出口参数:	无
**返 回 值:	~((sum)&0xffff))
**说    明:	计算校验和
************************************************************************/
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint16 CheckSumUdp(uint8 *check, uint16 length)
{
    uint32 sum = 0;
    uint16 i;
    for (i = 0; i < (length) / 2; i++)
    {
        sum = sum + ((uint32)check[2 * i] << 8) + (uint32)check[2 * i + 1];
    }
    if(length & 0x0001) //长度为奇数个时，要进行该操作
    {
        sum = sum + ((uint32)check[2 * i] << 8);
    }
    sum = (sum & 0xffff) + ((sum >> 16) & 0xffff); //高16位和低16位相加
    if(sum & 0xffff0000)
    {
        //表示有进位
        sum++;
    }
    return ( (uint16)(~((sum) & 0xffff))  );
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 UDP_ReceivePacket(BYTE *pdata)//UDP报处理
{
    uint16 lg, DestPort;
    udp_data *pudpk;
    IC2CMD_Header pcmd;
    struct SOUND_HEADER_TYPE  paudio;
    NetP *parp_table;
    eip *pip;
    BYTE *p, tmp;
    uint8 rev_buf[80];
    struct MSG_REV_TYPE *prev;

    pudpk = (udp_data *)pdata;
    p = (BYTE *)(pdata + 8);
    pip = (eip *)((BYTE *)pdata - sizeof(eip));
    lg = swap_int16(pudpk->length);
    DestPort = swap_int16(pudpk->DestPort);
    if (DestPort == IC2_COMMAND_PORT) //UDP命令端口
    {
        memcpy((BYTE *)&pcmd, p, sizeof(IC2CMD_Header));
        if ((pcmd.dwSyncHead == CMD_ABLE_ICII) && (pcmd.Cmd.dwCommand == CMD_AD_RECVMSG))
        {
            if ( Check_UDP_Paket(p, lg) == FALSE)
                XCOM_Q_Send(DEV_XCOM_UDP, p, lg); //发送
        }
    }
    else if (DestPort == AUDIO_PORT)	//音频数据
    {
        memcpy((BYTE *)&paudio, p, sizeof(Audio_Header));
        if (paudio.Flag == FLAG_SOUND_HEADER)
        {
            p = p + sizeof(struct SOUND_HEADER_TYPE);
            Sound_Echo_Contrl((uint16 *)p + 2);
            for (tmp = 0; tmp < lg / 512; tmp++)
                Sound_Save_PlayData(p + tmp * 512, (BYTE *)&paudio.OWIP, 0);
        }
    }
    else if (DestPort == UDP_LAN_TEST_PORT) //测试命令
    {
        LAN_Test_Ack(p, lg);
    }

    else if (DestPort == UDP_DEBUG_CMD_PORT) //测试命令
    {
        parp_table = (NetP *)Get_NetPort();
        memcpy(parp_table->Test_Ip, pip->SourceIp, 4);
        parp_table->Test_TTL = 3;

        prev = (struct MSG_REV_TYPE *)rev_buf;
        prev->Type = DEV_XCOM_DEBUG;
        prev->num = 0;
        Mem_Copy((uint8 *)&prev->des_ip[0], pip->SourceIp, 4);
        Mem_Copy(rev_buf + sizeof(struct MSG_REV_TYPE), p, lg);
        XCOM_Q_Send(DEV_XCOM_DEBUG, rev_buf, lg + sizeof(struct MSG_REV_TYPE));



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
uint8 Udp_SendPacked(struct _pkst *TxData, BYTE *desip, uint16 desport)
{
    struct _pkst TxdData1, *ptx;

    udp_send Udp_Packed;
    uint16 tmp_lg = 0;
    BYTE  *p;

    p = Get_DesIp(DEVICE_SUB_CLASS);
    //计算整个发送数据长度
    for (ptx = TxData; ptx != NULL; ptx = ptx->STPTR)
        tmp_lg += ptx->length;
    memset((BYTE *)&Udp_Packed, 0, sizeof(udp_send));
    Udp_Packed.UdpDHead.rev = 0;
    Udp_Packed.UdpDHead.NextProtocal = 17;
    Udp_Packed.UdpDHead.TotalLen = swap_int16(8 + tmp_lg); //伪头＋头＋数据
    Udp_Packed.UdpDHead.SourceIp[0] = *p;
    Udp_Packed.UdpDHead.SourceIp[1] = *(p + 1);
    Udp_Packed.UdpDHead.SourceIp[2] = *(p + 2);
    Udp_Packed.UdpDHead.SourceIp[3] = *(p + 3);
    Udp_Packed.UdpDHead.DestId[0] = *desip;
    Udp_Packed.UdpDHead.DestId[1] = *(desip + 1);
    Udp_Packed.UdpDHead.DestId[2] = *(desip + 2);
    Udp_Packed.UdpDHead.DestId[3] = *(desip + 3);
    Udp_Packed.Udp.DestPort = swap_int16(desport);
    Udp_Packed.Udp.SourcePort = swap_int16(desport);
    Udp_Packed.Udp.length = Udp_Packed.UdpDHead.TotalLen; //头＋数据
    Udp_Packed.Udp.Crc = 0;
    //	Udp_Packed.Udp.Crc=CheckSumUdp((uint16 EX_RAM *)&Udp_Packed,(uint16 EX_RAM *)UDPSEDN.data_ptr,UDPSEDN.length);
    TxdData1.STPTR = TxData;
    TxdData1.length = 8;
    TxdData1.DAPTR = (uint8 *)&Udp_Packed.Udp;
    IP_SendFrame(&TxdData1, Udp_Packed.UdpDHead.DestId, 17);
    return TRUE;
}


#define SOCKET_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"

uint8 close(uint8 num)
{
    uint8 err, i;
    struct _pkst TxdData;
    if(TcpStatus[num].State == TCP_STATE_CLOSED)
    {
        return (1);
    }
    else if(TcpStatus[num].State == TCP_STATE_LISTEN)
    {
        Delete_Socket(num);
        return (1);
    }
    else if(TcpStatus[num].State == TCP_STATE_ESTABLISHED)
    {
        TcpStatus[num].SenPassSeq = TcpStatus[num].SenFutureSeq;
        TcpStatus[num].SenPassAck = TcpStatus[num].SenPassAck;
        TcpStatus[num].SenFutureSeq = TcpStatus[num].SenPassSeq + 1;
        TcpStatus[num].RecFutureAck = TcpStatus[num].SenPassSeq + 1;
        TcpHeadHandle(num , 0x5000 + TCP_ACK + TCP_FIN); //headlength 0x20,
        TcpStatus[num].TcpDHeadUint8[2] = 0;
        TcpStatus[num].TcpDHeadUint8[3] = 20;
        TxdData.length = CheckSumTcp1(num, 32); //12+28
        TcpStatus[num].TcpHeadUint8[16] = (TxdData.length & 0xff00) >> 8;;
        TcpStatus[num].TcpHeadUint8[17] = TxdData.length & 0x00ff;
        TxdData.STPTR = NULL;
        TxdData.length = 20;
        TxdData.DAPTR = TcpStatus[num].TcpHeadUint8;
        Send_Ip_Frame
        (
            &TxdData,
            TcpStatus[num].Dest_Ip,
            TcpStatus[num].My_Ip,
            6
        );

    }
    else if(TcpStatus[num].State == TCP_STATE_SYN_RCVD)
    {
        TcpStatus[num].SenPassSeq = TcpStatus[num].SenFutureSeq;
        TcpStatus[num].SenPassAck = TcpStatus[num].SenPassAck;
        TcpStatus[num].SenFutureSeq = TcpStatus[num].SenPassSeq + 1;
        TcpStatus[num].RecFutureAck = TcpStatus[num].SenPassSeq + 1;
        TcpHeadHandle(num , 0x5000 + TCP_FIN); //headlength 0x20,
        TcpStatus[num].TcpDHeadUint8[2] = 0;
        TcpStatus[num].TcpDHeadUint8[3] = 20;
        TxdData.length = CheckSumTcp1(num, 32); //12+28
        TcpStatus[num].TcpHeadUint8[16] = (TxdData.length & 0xff00) >> 8;
        TcpStatus[num].TcpHeadUint8[17] = TxdData.length & 0x00ff;
        TxdData.STPTR = NULL;
        TxdData.length = 20;
        TxdData.DAPTR = TcpStatus[num].TcpHeadUint8;
        Send_Ip_Frame
        (
            &TxdData,
            TcpStatus[num].Dest_Ip,
            TcpStatus[num].My_Ip,
            6
        );
    }
    else
    {
        Delete_Socket(num);
        return (1);
    }
    TcpStatus[num].State = TCP_STATE_FIN_WAIT1;
    for(i = 0; i < 5; i++)
    {
        OSSemPend(AppSemDisc[num], 100, &err);
        if(err == OS_NO_ERR)
        {
            Delete_Socket(num);
            return (1);
        }
        else
        {
            Send_Ip_Frame
            (
                &TxdData,
                TcpStatus[num].Dest_Ip,
                TcpStatus[num].My_Ip,
                6
            );

        }
    }
    Delete_Socket(num);
    return(0);
}
uint8 connect(SOCKET *s, struct sockaddr *addr, uint16 addrlen)   //
{
    struct _pkst TxdData;
    uint8 i, j, err;

    for(i = 0; i < MAX_TCP_LINKS; i++)
    {
        if(TcpStatus[i].State >= TCP_STATE_LISTEN)
        {
            if(TcpStatus[i].Dest_Ip[0] == addr->sin_addr[0])
                if(TcpStatus[i].Dest_Ip[1] == addr->sin_addr[1])
                    if(TcpStatus[i].Dest_Ip[2] == addr->sin_addr[2])
                        if(TcpStatus[i].Dest_Ip[3] == addr->sin_addr[3])
                            if(TcpStatus[i].My_Ip[0] == s->My_Ip[0])
                                if(TcpStatus[i].My_Ip[1] == s->My_Ip[1])
                                    if(TcpStatus[i].My_Ip[2] == s->My_Ip[2])
                                        if(TcpStatus[i].My_Ip[3] == s->My_Ip[3])
                                            if(s->My_Port == TcpStatus[i].My_Port)
                                                if(addr->sin_port == TcpStatus[i].Dest_Port)
                                                {
                                                    if(TcpStatus[i].State == TCP_STATE_ESTABLISHED)
                                                        return (i);
                                                    else
                                                        close (i);
                                                    //Delete_Socket(i);
                                                }
        }
    }
    j = 0;
    do
    {
        OS_ENTER_CRITICAL();
        for(i = 0; i < MAX_TCP_LINKS; i++)
        {
            if(TcpStatus[i].State == TCP_STATE_CLOSED)
            {
                TcpStatus[i].State = TCP_STATE_SYN_SENT;
                break;
            }
        }
        OS_EXIT_CRITICAL();
        if(i >= MAX_TCP_LINKS)
            return (MAX_TCP_LINKS);//连接已经满了
        TcpStatus[i].My_Port = s->My_Port;
        TcpStatus[i].Dest_Port = addr->sin_port; //对方端口
        TcpStatus[i].Dest_Ip[0] = addr->sin_addr[0];
        TcpStatus[i].Dest_Ip[1] = addr->sin_addr[1];
        TcpStatus[i].Dest_Ip[2] = addr->sin_addr[2];
        TcpStatus[i].Dest_Ip[3] = addr->sin_addr[3];
        TcpStatus[i].My_Ip[0] = s->My_Ip[0];
        TcpStatus[i].My_Ip[1] = s->My_Ip[1];
        TcpStatus[i].My_Ip[2] = s->My_Ip[2];
        TcpStatus[i].My_Ip[3] = s->My_Ip[3];
        TcpStatus[i].RecPassSeq = 0;
        TcpStatus[i].RecPassAck = 0;
        TcpStatus[i].SenPassSeq = 781010;
        TcpStatus[i].SenPassAck = 0;
        TcpStatus[i].RecFutureAck = TcpStatus[i].SenPassSeq + 1;
        TcpStatus[i].SenFutureSeq = TcpStatus[i].SenPassSeq + 1;
        TcpStatus[i].My_Wl2 = TcpStatus[i].SenPassSeq;
        TcpStatus[i].Snd_Window = MAX_TCP_DATA; //通知对方本地最大接收1024字节的包,用于流控
        TcpStatus[i].My_Max_Seg_Size = MAX_TCP_DATA; //本地机可以接受最大的以太网数据包
        TcpHeadHandle(i , 0x7000 + TCP_SYN);
        //******添加TCP头选项*************//
        TcpStatus[i].TcpHeadUint8[20] = 0X02;
        TcpStatus[i].TcpHeadUint8[21] = 0X04;
        TcpStatus[i].TcpHeadUint8[22] = MAX_TCP_DATA / 256;
        TcpStatus[i].TcpHeadUint8[23] = MAX_TCP_DATA % 256;
        TcpStatus[i].TcpHeadUint8[24] = 0X01;
        TcpStatus[i].TcpHeadUint8[25] = 0X01;
        TcpStatus[i].TcpHeadUint8[26] = 0X04;
        TcpStatus[i].TcpHeadUint8[27] = 0X02;

        TcpStatus[i].TcpDHeadUint8[2] = 0;
        TcpStatus[i].TcpDHeadUint8[3] = 28;
        TxdData.length = CheckSumTcp1(i, 40); //12+28
        TcpStatus[i].TcpHeadUint8[16] = (TxdData.length & 0xff00) >> 8;;
        TcpStatus[i].TcpHeadUint8[17] = TxdData.length & 0x00ff;
        TxdData.STPTR = NULL;
        TxdData.length = 28;
        TxdData.DAPTR = TcpStatus[i].TcpHeadUint8;
        TcpStatus[i].StaSem = 0x02;
        Send_Ip_Frame
        (
            &TxdData,
            TcpStatus[i].Dest_Ip,
            TcpStatus[i].My_Ip,
            6
        );
        OSSemPend(AppSemCon[i], 100, &err); //挂起，等待SYN_ACK信号的成功接收
        if(err == OS_NO_ERR)
        {
            //TcpStatus[i].State=TCP_STATE_ESTABLISHED;
            return(i);
        }
        if(TcpStatus[i].State == TCP_STATE_SYN_SENT)
            if(TcpStatus[i].Dest_Ip[0] == addr->sin_addr[0])
                if(TcpStatus[i].Dest_Ip[1] == addr->sin_addr[1])
                    if(TcpStatus[i].Dest_Ip[2] == addr->sin_addr[2])
                        if(TcpStatus[i].Dest_Ip[3] == addr->sin_addr[3])
                            if(TcpStatus[i].My_Ip[0] == s->My_Ip[0])
                                if(TcpStatus[i].My_Ip[1] == s->My_Ip[1])
                                    if(TcpStatus[i].My_Ip[2] == s->My_Ip[2])
                                        if(TcpStatus[i].My_Ip[3] == s->My_Ip[3])
                                            if(s->My_Port == TcpStatus[i].My_Port)
                                                if(addr->sin_port == TcpStatus[i].Dest_Port)
                                                {
                                                    Delete_Socket(i);
                                                }
        j++;
    }
    while(j < 3);
    Delete_Socket(i);
    return(MAX_TCP_LINKS);//退出返回连接失败。
}

SOCKET *socket( uint16 af, uint16 type, uint16 protocol)
{
    uint8 i, j, k;
    static uint16 port = 1025;
    OS_ENTER_CRITICAL();
    for(k = 0; k < MAX_SOCKETS; k++)
        if(Zlg_Socket[k].State == 0)
            break;
    Zlg_Socket[k].State = 0x50;
    Zlg_Socket[k].af = af;
    Zlg_Socket[k].type = type;
    Zlg_Socket[k].protocol = protocol;
    i = 0;
    if(protocol == TCP_PROTOCOL)
    {
        do
        {
            if(TcpStatus[i].My_Port == port)
            {
                port++;
            }
            i++;
        }
        while(i < MAX_TCP_LINKS);
    }
    else if(protocol == UDP_PROTOCOL)
    {
        j = 0;
        do
        {
            if(UdpStatus[i].My_Port == port)
            {
                port++;
            }
            i++;
        }
        while(i < MAX_UDP_LINKS);
        do
        {
            if(UdpStatus[j].My_Port == 0)
            {
                break;
            }
            j++;
        }
        while(j < MAX_UDP_LINKS);
        if(j >= MAX_UDP_LINKS)
        {
            OS_EXIT_CRITICAL();
            return ((SOCKET * )INVALID_SOCKET);
        }
        Zlg_Socket[k].Udp_Queue = j;
        UdpStatus[j].My_Port = port;
        UdpStatus[j].My_Ip[0] = NetPort[0].My_Ip[0];
        UdpStatus[j].My_Ip[1] = NetPort[0].My_Ip[1];
        UdpStatus[j].My_Ip[2] = NetPort[0].My_Ip[2];
        UdpStatus[j].My_Ip[3] = NetPort[0].My_Ip[3];
    }
    else
    {
        OS_EXIT_CRITICAL();
        return((SOCKET * )INVALID_SOCKET);
    }
    Zlg_Socket[k].My_Port = port;
    port++;
    if(port >= 5000)
        port = 1025;
    Zlg_Socket[k].My_Ip[0] = NetPort[0].My_Ip[0];
    Zlg_Socket[k].My_Ip[1] = NetPort[0].My_Ip[1];
    Zlg_Socket[k].My_Ip[2] = NetPort[0].My_Ip[2];
    Zlg_Socket[k].My_Ip[3] = NetPort[0].My_Ip[3];
    OS_EXIT_CRITICAL();
    return (&Zlg_Socket[k]);
}
int bind( SOCKET *s, struct sockaddr *name, uint16 namelen)
{
    s->My_Ip[0] = ((struct sockaddr *)name)->sin_addr[0];
    s->My_Ip[1] = ((struct sockaddr *)name)->sin_addr[1];
    s->My_Ip[2] = ((struct sockaddr *)name)->sin_addr[2];
    s->My_Ip[3] = ((struct sockaddr *)name)->sin_addr[3];
    s->My_Port = ((struct sockaddr *)name)->sin_port;
    if(s->protocol == UDP_PROTOCOL)
    {
        UdpStatus[s->Udp_Queue].My_Port = ((struct sockaddr *)name)->sin_port;
        UdpStatus[s->Udp_Queue].My_Ip[0] = ((struct sockaddr *)name)->sin_addr[0];
        UdpStatus[s->Udp_Queue].My_Ip[1] = ((struct sockaddr *)name)->sin_addr[1];
        UdpStatus[s->Udp_Queue].My_Ip[2] = ((struct sockaddr *)name)->sin_addr[2];
        UdpStatus[s->Udp_Queue].My_Ip[3] = ((struct sockaddr *)name)->sin_addr[3];
        return 1;
    }
    return 0;
}
int listen( SOCKET *s, uint16 backlog )
{
    uint16 i;
    if(backlog > MAX_TCP_LINKS)
        return -1;
    else if(backlog == 0)
        return -2;
    else if(s->protocol != 0)
        return -3;
    s->Max_Queue = backlog;
    OS_ENTER_CRITICAL();
    for(i = 0; i < MAX_TCP_LINKS; i++)
    {
        if(TcpStatus[i].State == TCP_STATE_CLOSED)
        {

            TcpStatus[i].State = TCP_STATE_LISTEN;
            s->queue[s->Max_Queue - backlog] = i;
            backlog--;
        }
        if(backlog == 0)
            break;//如果刚好backlog＝MAX_TCP_LINKS，i＝MAX_TCP_LINK-1;
    }
    if(i >= MAX_TCP_LINKS)
    {
        if(backlog == s->Max_Queue)
        {
            OS_EXIT_CRITICAL();
            return -4;
        }
        i = 0;
        do
        {
            TcpStatus[s->queue[i]].State = TCP_STATE_CLOSED;
            backlog++;
            i++;
        }
        while(backlog != s->Max_Queue);
        memset(&s, 0, sizeof(s));
        OS_EXIT_CRITICAL();
        return -4;
    }
    else
    {
        s->State = 1;
        for(i = 0; i < s->Max_Queue; i++)
        {
            TcpStatus[s->queue[i]].My_Ip[0] = s->My_Ip[0];
            TcpStatus[s->queue[i]].My_Ip[1] = s->My_Ip[1];
            TcpStatus[s->queue[i]].My_Ip[2] = s->My_Ip[2];
            TcpStatus[s->queue[i]].My_Ip[3] = s->My_Ip[3];
            TcpStatus[s->queue[i]].My_Port = s->My_Port;
            TcpStatus[s->queue[i]].StaSem = 0x82;
        }
    }
    OS_EXIT_CRITICAL();
    return (s->Max_Queue);
}
uint8 accept( SOCKET *s, struct sockaddr *addr, int *addrlen )
{
    uint16 i;
    uint8  err;
    if(s->protocol != 0)
        return (0XFF);
    if(s->Max_Queue == 0)
        return (0XFF);
    for(i = 0; i < s->Max_Queue; i++)
    {
        if(OSSemAccept(AppSemCon[s->queue[i]]))
        {
            break;
        }
    }
    if(i >= s->Max_Queue)
    {
        for(i = 0; i < s->Max_Queue; i++)
        {
            if(TcpStatus[s->queue[i]].State == TCP_STATE_LISTEN)
            {
                //OSSemPend(AppSemCon[s->queue[i]],0,&err);
                OSSemPend(AppSemCon[s->queue[i]], 400, &err);
                if(err == OS_NO_ERR || err == OS_TIMEOUT) //等
                    break;
                else
                    return (0XFF);
            }
        }
        if(err == OS_TIMEOUT)
        {
            for(i = 0; i < s->Max_Queue; i++)
            {
                if(TcpStatus[s->queue[i]].State == TCP_STATE_ESTABLISHED)
                    break;//连接中有已经建立连接的了
            }
            if(i >= s->Max_Queue)
            {
                return (0XFF);//连接已经满了
            }
        }

    }
    addr->sin_addr[0] = TcpStatus[s->queue[i]].Dest_Ip[0];
    addr->sin_addr[1] = TcpStatus[s->queue[i]].Dest_Ip[0];
    addr->sin_addr[2] = TcpStatus[s->queue[i]].Dest_Ip[0];
    addr->sin_addr[3] = TcpStatus[s->queue[i]].Dest_Ip[0];
    addr->sin_port = TcpStatus[s->queue[i]].Dest_Port;
    *addrlen = sizeof(*addr);
    return (s->queue[i]);
}

uint16 recv( uint8 num, uint8 *buf, uint16 len, int flags )
{
    uint16	i, temp, sum, tempwindow;
    struct _pkst TxdData;
    flags = flags;
    tempwindow = TcpStatus[num].Snd_Window;
    if((TcpStatus[num].TcpDataQWrPtr == TcpStatus[num].TcpDataQRdPtr) && (TcpStatus[num].Snd_Window != 0))
        return (0);
    else 	if(TcpStatus[num].TcpDataQRdPtr >= TcpStatus[num].TcpDataQWrPtr) //队列已经翻转
    {
        temp = MAX_TCP_DATA - TcpStatus[num].TcpDataQRdPtr;
        if(temp >= len) //len没有翻转
        {
            for(i = 0; i < len; i++)
            {
                *buf = TcpStatus[num].TcpDataQ[TcpStatus[num].TcpDataQRdPtr];
                buf++;
                TcpStatus[num].TcpDataQRdPtr++;
            }
            TcpStatus[num].Snd_Window = TcpStatus[num].Snd_Window + len;
            temp = len;
        }
        else	////len翻转
        {
            for(i = 0; i < temp; i++)
            {
                *buf = TcpStatus[num].TcpDataQ[TcpStatus[num].TcpDataQRdPtr];
                buf++;
                TcpStatus[num].TcpDataQRdPtr++;
            }
            TcpStatus[num].TcpDataQRdPtr = 0;
            sum = temp;
            temp = TcpStatus[num].TcpDataQWrPtr;
            if(temp >= (len - sum)) //数据足够
                temp = len - sum;
            for(i = 0; i < temp; i++)
            {
                *buf = TcpStatus[num].TcpDataQ[i];
                buf++;
                TcpStatus[num].TcpDataQRdPtr++;
            }
            sum = sum + temp;
            TcpStatus[num].Snd_Window = TcpStatus[num].Snd_Window + sum;
            temp = sum;
        }
    }
    else	//队列未翻转
    {
        temp = TcpStatus[num].TcpDataQWrPtr - TcpStatus[num].TcpDataQRdPtr;
        if(temp >= len) //数据足够
            temp = len;
        for(i = 0; i < temp; i++)
        {
            *buf = TcpStatus[num].TcpDataQ[TcpStatus[num].TcpDataQRdPtr];
            buf++;
            TcpStatus[num].TcpDataQRdPtr++;
        }
        TcpStatus[num].Snd_Window = TcpStatus[num].Snd_Window + temp;
    }
    TcpHeadHandle(num , 0x5000 + TCP_ACK); //headlength 0x20,
    TcpStatus[num].TcpDHeadUint8[2] = 0;
    TcpStatus[num].TcpDHeadUint8[3] = 20;
    sum = CheckSumTcp1(num, 32); //12+28
    TcpStatus[num].TcpHeadUint8[16] = (sum & 0xff00) >> 8;;
    TcpStatus[num].TcpHeadUint8[17] = sum & 0x00ff;
    TxdData.STPTR = NULL;
    TxdData.length = 20;
    TxdData.DAPTR = TcpStatus[num].TcpHeadUint8;
    Send_Ip_Frame
    (
        &TxdData,
        TcpStatus[num].Dest_Ip,
        TcpStatus[num].My_Ip,
        6
    );
    return (temp);
}

uint16 send( uint8 num, uint8 *buf, uint16 len, int flags )
{
    struct _pkst TxdData;
    uint16 temp;
    uint8 err, i;
    flags = flags;
    if((TcpStatus[num].Dest_Max_Seg_Size >= len) && (TcpStatus[num].State == TCP_STATE_ESTABLISHED))
    {
        TcpStatus[num].SenPassSeq = TcpStatus[num].SenFutureSeq;
        TcpStatus[num].SenPassAck = TcpStatus[num].SenPassAck;
        TcpStatus[num].SenFutureSeq = TcpStatus[num].SenPassSeq + len;
        TcpStatus[num].RecFutureAck = TcpStatus[num].SenPassSeq + len;
        TcpHeadHandle(num , 0x5000 + TCP_PSH + TCP_ACK); //headlength 0x20,
        TcpStatus[num].TcpDHeadUint8[2] = ((len + 20) & 0XFF00) >> 8;
        TcpStatus[num].TcpDHeadUint8[3] = (len + 20) & 0X00FF;
        OS_ENTER_CRITICAL();
        memcpy (&(TcpStatus[num].TcpHeadUint8[20]), buf, len);
        temp = len + 32;
        TxdData.length = CheckSumTcp1(num, temp); //12+20
        OS_EXIT_CRITICAL();
        TcpStatus[num].TcpHeadUint8[16] = (TxdData.length & 0xff00) >> 8;
        TcpStatus[num].TcpHeadUint8[17] = TxdData.length & 0x00ff;
        TxdData.STPTR = NULL;
        TxdData.length = 20 + len;
        TxdData.DAPTR = TcpStatus[num].TcpHeadUint8;
        i = 0;
        do
        {
            i++;
            if(Send_Ip_Frame
                    (
                        &TxdData,
                        TcpStatus[num].Dest_Ip,
                        TcpStatus[num].My_Ip,
                        6
                    ))
            {
                TcpStatus[num].StaSem = TcpStatus[num].StaSem | 0x01;
                OSSemPend(AppSemSend[num], 400, &err);
                if((err == OS_NO_ERR)) //等
                    return(len);
                if(TcpStatus[num].State != TCP_STATE_ESTABLISHED)
                    return 0;
            }
        }
        while(i < 2);
        //Delete_Socket(num);
        return 0;
    }
    else
    {
        return (0);
    }
}
uint16 sendto( SOCKET s, uint8 *buf, uint16 len, int flags, struct sockaddr *to, uint16 tolen )
{
    //uint16 temp;
    udp_send UdpPacked;
    struct _pkst TxdData;
    if(len > 1478)
        return 0;
    UdpPacked.UdpDHead.rev = 0;
    UdpPacked.UdpDHead.NextProtocal = 17;

    UdpPacked.UdpDHead.TotalLen = 8 + len;
#ifdef Little_End
    UdpPacked.UdpDHead.TotalLen = swap_int16(UdpPacked.UdpDHead.TotalLen);
#endif
    UdpPacked.UdpDHead.SourceIp[0] = s.My_Ip[0];
    UdpPacked.UdpDHead.SourceIp[1] = s.My_Ip[1];
    UdpPacked.UdpDHead.SourceIp[2] = s.My_Ip[2];
    UdpPacked.UdpDHead.SourceIp[3] = s.My_Ip[3];
    UdpPacked.UdpDHead.DestId[0] = ((struct sockaddr *)to)->sin_addr[0];
    UdpPacked.UdpDHead.DestId[1] = ((struct sockaddr *)to)->sin_addr[1];
    UdpPacked.UdpDHead.DestId[2] = ((struct sockaddr *)to)->sin_addr[2];
    UdpPacked.UdpDHead.DestId[3] = ((struct sockaddr *)to)->sin_addr[3];
    UdpPacked.Udp.SourcePort = s.My_Port;
    UdpPacked.Udp.DestPort = ((struct sockaddr *)to)->sin_port;
    UdpPacked.Udp.length = 8 + len;
#ifdef Little_End
    UdpPacked.Udp.SourcePort = swap_int16(UdpPacked.Udp.SourcePort);
    UdpPacked.Udp.DestPort = swap_int16(UdpPacked.Udp.DestPort);
    UdpPacked.Udp.length = swap_int16(UdpPacked.Udp.length);
#endif
    OS_ENTER_CRITICAL();
    memcpy (&(UdpPacked.Udp.udpdata), buf, len);
    OS_EXIT_CRITICAL();
    UdpPacked.Udp.Crc = 0;
    UdpPacked.Udp.Crc = CheckSumUdp((uint8 *)(&UdpPacked), len + 20);
#ifdef Little_End
    UdpPacked.Udp.Crc = swap_int16(UdpPacked.Udp.Crc);
#endif
    TxdData.STPTR = NULL;
    TxdData.length = 8 + len;
    TxdData.DAPTR = (uint8 *)(&(UdpPacked.Udp));
    if(Send_Ip_Frame
            (
                &TxdData,
                UdpPacked.UdpDHead.DestId,
                UdpPacked.UdpDHead.SourceIp,
                17
            ))
    {
        return(len);
    }
    else
        return(0);
}

uint16 recvfrom( SOCKET s, uint8 *buf, uint16 len, int flags, struct sockaddr *from, uint16 *fromlen )
{
    uint16 temp;
    uint8 j, err;
    if(s.Udp_Queue >= MAX_UDP_LINKS)
        return 0;
    if(s.My_Port != UdpStatus[s.Udp_Queue].My_Port)
        return 0;
    OSSemPend(UdpStatus[s.Udp_Queue].UdpSemRec, 40, &err);
    if(err == OS_TIMEOUT)
        return 0;
    else if(err == OS_NO_ERR)
    {
        j = 0;
        do
        {
            if(UdpStatus[s.Udp_Queue].info[j].num != UdpMaxRec)
                break;
            j++;
        }
        while(j < UdpMaxRec);
        if(j >= UdpMaxRec)
        {
            return (0);
        }
        from->sin_addr[0] = UdpStatus[s.Udp_Queue].info[j].De_Ip[0];
        from->sin_addr[1] = UdpStatus[s.Udp_Queue].info[j].De_Ip[1];
        from->sin_addr[2] = UdpStatus[s.Udp_Queue].info[j].De_Ip[2];
        from->sin_addr[3] = UdpStatus[s.Udp_Queue].info[j].De_Ip[3];
        from->sin_port = UdpStatus[s.Udp_Queue].info[j].De_Port;
        OS_ENTER_CRITICAL();
        UdpStatus[s.Udp_Queue].info[j].len = UdpStatus[s.Udp_Queue].info[j].len - 8;
        if(UdpStatus[s.Udp_Queue].info[j].len >= len)
        {
            memcpy (buf, (uint8 *) & (UdpRecBuff[UdpStatus[s.Udp_Queue].info[j].num]) + 8, len);
            temp = len;
        }
        else
        {
            memcpy (buf, (uint8 *) & (UdpRecBuff[UdpStatus[s.Udp_Queue].info[j].num]) + 8, UdpStatus[s.Udp_Queue].info[j].len);
            temp = UdpStatus[s.Udp_Queue].info[j].len;
        }
        UdpStatus[s.Udp_Queue].info[j].num = UdpMaxRec;
        UdpStatus[s.Udp_Queue].info[j].len = 0;
        OS_EXIT_CRITICAL();
        *fromlen = sizeof(*from);
        return (temp);


    }
    else
        return 0;
}
int closesocket( SOCKET s )
{
    uint8 i;
    if(s.protocol == TCP_PROTOCOL)
    {
        s.Max_Queue = 0;
        for(i = 0; i < MAX_TCP_LINKS; i++)
            s.queue[i] = 0xff;
    }
    else if(s.protocol == UDP_PROTOCOL)
    {
        OS_ENTER_CRITICAL();
        UdpStatus[s.Udp_Queue].My_Port = 0;
        UdpStatus[s.Udp_Queue].My_Ip[0] = 0xff;
        UdpStatus[s.Udp_Queue].My_Ip[1] = 0xff;
        UdpStatus[s.Udp_Queue].My_Ip[2] = 0xff;
        UdpStatus[s.Udp_Queue].My_Ip[3] = 0xff;
        for(i = 0; i < UdpMaxRec; i++)
        {
            UdpStatus[s.Udp_Queue].info[i].num = UdpMaxRec;
            UdpStatus[s.Udp_Queue].info[i].De_Port = 0;
            UdpStatus[s.Udp_Queue].info[i].len = 0;
        }
        OS_EXIT_CRITICAL();
        do
        {
            i = OSSemAccept(UdpStatus[s.Udp_Queue].UdpSemRec);
        }
        while(i != 0);
    }
    s.State = 0;
    s.protocol = 0xff;
    s.af = 0xff;
    s.type = 0xff;
    s.My_Ip[0] = 0xff;
    s.My_Ip[1] = 0xff;
    s.My_Ip[2] = 0xff;
    s.My_Ip[3] = 0xff;
    s.My_Port = 0;
    s.Udp_Queue = MAX_UDP_LINKS;
    return 1;
}

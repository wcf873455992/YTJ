

#include "includes.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


struct XCOM_PRINT_TYPE *pDGB_XCOM;

uint8 Debug_SubCastIP[4] = {255, 255, 255, 255};
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void DBG_Log_write(char *pbuf)
{
    struct _pkst  TxData;
    TxData.DAPTR = (uint8 *)pbuf;
    TxData.length = Str_Len((uint8 *)pbuf);
    if (TxData.length > 512)
    {
        IP_printf("打印信息超长%d", TxData.length);
    }
    TxData.STPTR = NULL;
    Udp_SendPacked(&TxData, Get_DesIp(DEVICE_SUB_SER), UDP_DEBUG_LOG_PORT);
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint16 DBG_Str_printf(char *pbuf, char *fmt, ...)
{
    uint16 i;

    va_list ap;
    va_start(ap, fmt);
    vsprintf(pbuf, fmt, ap);
    va_end(ap);
    i = strlen(pbuf);
    *(pbuf + i) = 0x0d;
    *(pbuf + i + 1) = 0;
    return i + 1;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint16 DBG_HexToChar(uint8 *pdata, uint8 *sbuf, uint16 len)
{
    uint8 i, tmp;

    for (i = 0; i < len; i++)
    {
        tmp = *(pdata + i);
        tmp = (tmp & 0xf0) >> 4;
        if (tmp < 10) tmp = '0' + tmp;
        else
            tmp = 'A' + (tmp - 10);
        *(sbuf + i * 3) = tmp;
        tmp = *(pdata + i);
        tmp = tmp & 0x0f;
        if (tmp < 10) tmp = '0' + tmp;
        else
            tmp = 'A' + (tmp - 10);
        *(sbuf + i * 3 + 1) = tmp;
        *(sbuf + i * 3 + 2) = 0x20;
    }
    *(sbuf + i * 3) = 0x0d;
    return (len * 3 + 1);
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void uart_Debug_init(void)
{
    uint32 Baudrate, pclk;
    Baudrate = 9600;
    pclk = BSP_CPU_PclkFreq(PCLK_UART0);
    U0LCR = 0x83;// DLAB = 1，可设置波特率
    U0DLL = ((pclk / 16) / Baudrate) % 256;
    U0DLM = ((pclk / 16) / Baudrate) / 256;
    U0LCR = 0x03;
    U0FCR = 0xc7; // 使能FIFO，并复位R,T FIFO，R触发8 Byte
    U0IER = 0x00;  // 允许RBR中断和超时接收中断
    pDGB_XCOM = NULL;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void IP_Debug_init(void)
{
    NetP *pnet;
    pnet = (NetP *)Get_NetPort();
    memcpy(pnet->Test_Ip, Debug_SubCastIP, 4);
    pnet->Test_TTL = 6;
}


/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////

void   UART_printf(char   *fmt, ...)
{
    uint32 i;
    char buf[80];
    va_list ap;
    va_start(ap, fmt);
    memset(buf, 0, 80);
    vsprintf(buf, fmt, ap);
    va_end(ap);
    for (i = 0; i < strlen(buf); i++)
    {
        U0THR = buf[i];			    // 发送数据
        IO_FlagLingt_Flash();
        while( (U0LSR & 0x40) == 0 );	 // 等待数据发送完毕


    }
    //    UART_Send_Data(XCOM_DEBUG_PORT,(BYTE *)&buf[0],strlen(buf));
    return ;   /* Dummy return value */
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void ID_Change(char *buf, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void IP_printf(char *fmt, ...)
{
    //int i,d;

    uint16 strln = 0;
    struct _pkst  TxData;
    char *buf, *p;
    va_list ap;
    NetP *pnet;
    pnet = (NetP *)Get_NetPort();
    if (pnet->Test_TTL == 0) return;
    buf = (char *)User_mem_allco(256);
    if (buf == NULL ) return ;
    memset(buf, 0, 256);
    strln = DBG_Str_printf(buf, "IP%d.%d:", pnet->My_Ip[2], pnet->My_Ip[3]);
    va_start(ap, fmt);
    vsprintf(buf + strln, fmt, ap);
    va_end(ap);
    p = &buf[strlen(buf)];
    *p++ = 0x0a;
    *p++ = 0x0d;
    TxData.STPTR = NULL;
    TxData.length = strlen(buf) + 1;
    TxData.DAPTR = (BYTE *)&buf[0];
    Udp_SendPacked(&TxData, Get_DesIp(DEVICE_SUB_TEST), UDP_DEBUG_PORT);
    User_mem_free(buf);
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
#define XCOM_DGB_PBUF_LN           256
void DBG_XCOM_Printf(uint8 port, uint8 *pbuf)
{
    uint8 *pchar, lg;
    struct _pkst  TxData;


    pchar = (uint8 *)User_mem_allco(XCOM_DGB_PBUF_LN);
    if (pchar == NULL) return;
    Mem_Clr(pchar, XCOM_DGB_PBUF_LN);
    lg = DBG_Str_printf((char *)pchar, "串口调试信息:%d", port);
    Mem_Copy(pchar + lg, pbuf, 15);
    lg += 15;
    TxData.STPTR = NULL;
    TxData.length = lg;
    TxData.DAPTR = (BYTE *)pchar;
    Udp_SendPacked(&TxData, Get_DesIp(DEVICE_SUB_TEST), UDP_DEBUG_PORT);
    User_mem_free(pchar);

}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
char *Log_Create(uint16 *strlen)
{
    uint16 str_len;
    struct CLASS_INFO *pclass;

    struct REALTIME_TYPE *ptime;
    char *sbuf;
    str_len = 0;
    sbuf = (char *)User_mem_allco(512);
    if (sbuf == NULL) return NULL;
    Mem_Clr((char *)sbuf, 512);

    ptime = ( struct REALTIME_TYPE *)RealTime_Get();
    pclass = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
    str_len = DBG_Str_printf(sbuf, "%s", &pclass->school_name[0]);
    str_len += DBG_Str_printf(sbuf + str_len, "%s", &pclass->house_name[0]);
    str_len += DBG_Str_printf(sbuf + str_len, "%s", &pclass->class_name[0]);
    str_len += DBG_Str_printf(sbuf + str_len, "%d.%d %d:%d:%d", ptime->mon, ptime->day, ptime->hour, ptime->min, ptime->sec);
    *strlen = str_len;
    return sbuf;
}
/////////////////////////////////////////////////////////////////////
//  函数名称：
//  函数功能：
//  入口参数：
//
//
//  出口参数：无
////////////////////////////////////////////////////////////////////
void Print_MSG_info(void *pdata)
{

    uint16 strlen;
    char *sbuf;
    struct MSG_REV_TYPE *prev;
    struct BASE_MSG *pmsg;

    sbuf = (char *)Log_Create(&strlen);
    if (sbuf != NULL)
    {
        prev = (struct MSG_REV_TYPE * )pdata;

        switch(prev->Type)
        {
        case DEV_XCOM_MB://
            strlen += DBG_Str_printf(sbuf + strlen, "消息来源:键盘");
            break;
        case DEV_XCOM_TIME://
            strlen += DBG_Str_printf(sbuf + strlen, "消息来源:课表/定时器");
            break;
        case DEV_XCOM_RF:
            strlen += DBG_Str_printf(sbuf + strlen, "消息来源:RF控制");
            break;
        case DEV_XCOM_DEBUG:
            strlen += DBG_Str_printf(sbuf + strlen, "消息来源:调试端口");
            break;
        case DEV_XCOM_SELF://
            strlen = DBG_Str_printf(sbuf + strlen, "消息来源:一体机");
            break;
        case DEV_XCOM_TABLE://
            strlen = DBG_Str_printf(sbuf + strlen, "消息来源:开关机序列");
            break;
        case DEV_XCOM_UDP://
            strlen += DBG_Str_printf(sbuf + strlen, "消息来源:网络");
            strlen += DBG_Str_printf(sbuf + strlen, "IP地址:%d.%d.%d.%d",
                                     prev->des_ip[0], prev->des_ip[1], prev->des_ip[2], prev->des_ip[3]);
            break;
        default:
            strlen += DBG_Str_printf(sbuf + strlen, "消息来源:未知");
            strlen += DBG_Str_printf(sbuf + strlen, "消息来源%d,端口%d,IP地址%d.%d.%d.%d",
                                     prev->Type, prev->num, prev->des_ip[0], prev->des_ip[1], prev->des_ip[2], prev->des_ip[3]);
            break;

        }

        pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
        strlen += DBG_Str_printf(sbuf + strlen, "设备号%d消息号%d", pmsg->device, pmsg->option);
        strlen += DBG_HexToChar((uint8 *)pdata + sizeof(struct BASE_MSG), (uint8 *)sbuf + strlen, 5);
        Log_Close(sbuf);
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
void Log_Close(char *sbuf)
{
    DBG_Log_write(sbuf);
    User_mem_free(sbuf);
}

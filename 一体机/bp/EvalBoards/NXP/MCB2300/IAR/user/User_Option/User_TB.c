#include "includes.h"
const uint8 gTB_VGAChage[5] = {0, 0, DEV_VGA_OUT_TB0, DEV_VGA_OUT_TB1, 0};
const uint8 gTB_VideoChage[8] = {0, DEV_VIDEO_OUT_TB0, DEV_VIDEO_OUT_TB1, DEV_VIDEO_OUT_BM2_3, DEV_VIDEO_OUT_BM2_4, 0, 0, 0};
struct DEVICE_TB_TYPE TB_LINK;
struct TBCard_RECNo_State_Type gTB_State;
struct TB_LinKState_Type  gTBLink;
struct TB_SingleState_TYPE  gTBSingle;
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_SingleChage(uint8 SingleType, uint8 Single)
{
    if (SingleType == TBCard_Option_VGA)
        return gTB_VGAChage[Single];
    else
        return gTB_VideoChage[Single];

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_ReadVer(uint8 *pdata)
{
    uint8 lg;
    *pdata = TB_CMD_Ver;
    lg = 1;
    return XCOM_Data_Read(DEV_XCOM_TB, 3, XCOM_CMD_GET_INFO, pdata, &lg, XCOM_READ_FLAG_WAIT_DATA);
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_Read_Csingle(void)
{
    uint8 lg, buff[10];

    buff[0] = TB_CMD_CheckSingle;
    lg = 1;
    return XCOM_Data_Read(DEV_XCOM_TB, 3, XCOM_CMD_GET_DATA, buff, &lg, XCOM_READ_FLAG_WAIT_ACK);
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_Read_state(void)
{
    uint8 lg, buff[10];
    buff[0] = TB_CMD_State;
    lg = 1;
    return XCOM_Data_Read(DEV_XCOM_TB, 3, XCOM_CMD_GET_DATA, buff, &lg, XCOM_READ_FLAG_WAIT_ACK);
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_ReadSingleState(void)
{
    uint8 buf[16], lg;
    buf[0] = 4;
    lg = 1;
    return XCOM_Data_Read(DEV_XCOM_TB, 3, XCOM_CMD_GET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_Check_VGA_InputSingle(uint8 input)
{
    uint8 tmp;
    if ((input > 4) || (input == 0)) return FALSE;
    tmp = input - 1;
    if (gTBSingle.VGASingle & (1 << tmp))
    {
        tmp += 4;
        if (gTBSingle.VGASingle & (1 << tmp)) return TRUE;
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
uint8 TB_Check_AV_InputSingle(uint8 input)
{
    uint8 tmp;
    if ((input > 4) || (input == 0)) return FALSE;
    tmp = input - 1;
    if (gTBSingle.VGASingle & (1 << tmp)) return TRUE;
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
uint8 TB_SelectVGA(uint8 Input, uint8 Output)
{
    uint8 buf[16], lg;
    buf[0] = TB_CMD_SelectVGA;
    buf[1] = Output;
    buf[2] = Input;
    lg = 3;
    return XCOM_Data_Read(DEV_XCOM_TB, 3, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_BreakVGA(uint8 Output)
{
    uint8 buf[16], lg;
    buf[0] = TB_CMD_BreakVGA;
    buf[1] = Output;
    lg = 2;
    return XCOM_Data_Read(DEV_XCOM_TB, 3, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_BreakAV(uint8 Output)
{
    uint8 buf[16], lg;
    buf[0] = TB_CMD_BreakAV;
    buf[1] = Output;
    lg = 2;
    return XCOM_Data_Read(DEV_XCOM_TB, 3, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);

}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_SelectAV(uint8 Input, uint8 Output)
{
    uint8 buf[16], lg;
    buf[0] = TB_CMD_SelectAV;
    buf[1] = Output;
    buf[2] = Input;
    lg = 3;
    return XCOM_Data_Read(DEV_XCOM_TB, 3, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);

}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
struct TBCard_RECNo_State_Type *Get_TB_REC_State(void)
{
    return &gTB_State;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
struct TB_LinKState_Type *Get_TBLinkState(void)
{
    return &gTBLink;
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TB_REC_Freez(uint8 num_type, uint8 num, uint8 ID)
{
    uint8 buf[20], *ip, tmp;
    struct TBCard_RECNo_State_Type *pTstate;
    if ( Check_FunctionMode(Device_FunctionMode_LB) == FALSE) return FALSE;
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_REC_Freez;
    buf[2] = num_type;
    buf[3] = num;
    buf[4] = ID;
    ip = Get_DesIp(DEVICE_SUB_BJCard);
    tmp = UDP_Read(buf, 4, ip, XCOM_READ_FLAG_WAIT_DATA, MSG_OPTION_REC_ReqStatus);
    if (tmp == 0) return FALSE;
    pTstate = Get_TB_REC_State();
    Mem_Copy((uint8 *)pTstate, buf + 2, sizeof(struct TBCard_RECNo_State_Type));
    if (num_type == TBCard_Option_VGA)
    {
        if (pTstate->SingleInfo[0].State == TBCard_state_Freez)	return TRUE;
        else return FALSE;
    }
    else if (pTstate->SingleInfo[num].State == TBCard_state_Freez) return TRUE;
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
uint8 TB_REC_UnFreez(uint8 num_type, uint8 num, uint8 ID)
{
    uint8 buf[20], *ip;
    //struct TBCard_RECNo_State_Type *pTstate;
    if ( Check_FunctionMode(Device_FunctionMode_LB) == FALSE)
    {
        IP_printf("编辑卡没有安装");
        return FALSE;
    }
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_REC_UnFreez;
    buf[2] = num_type;
    buf[3] = num;
    buf[4] = ID;
    ip = Get_DesIp(DEVICE_SUB_BJCard);
    UDP_Send_CMD(buf, 4, ip);
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
void TB_msg_rev(uint8 *pdata)
{
    uint8 *p, buf[16], *ip, tmp, i;
    struct ZY_XCOM *pable;
    struct DEVICE_STATUS_TYPE *ps;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    pable = ( struct ZY_XCOM *)(pdata + sizeof( struct XCOM_REV_TYPE));
    p = pdata + sizeof( struct XCOM_REV_TYPE) + sizeof(struct ZY_XCOM);
    if (pable->flag == ZY_XCOM_FLAG_ACK)
    {
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_COMM;
        buf[2] = ((pable->frame_CMD >> 4) & 0x0f) + (pable->type_lg & 0xf0);
        buf[3] = DEV_XCOM_SCAMERA;
        Task_Rec_Del(buf, 4); //删除消息列表中的重发消息
        return;
    }
    XCOM_Send_ACK(pdata);
    switch(*p)
    {
    case TB_CMD_Ver:
        ps->TB_CardVer = *(p + 1) * 256 + *(p + 2);
        ps->DEV_Link_State |= DEV_LINK_TBLK;
        break;
    case TB_CMD_CheckSingle:
        Mem_Copy((uint8 *)&gTBSingle, p + 1, sizeof(struct TB_SingleState_TYPE));
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_TB_ReqStatus;
        if ( gTBLink.VgaOutSingleLink[0] != 0)
        {
            tmp = (0x11 << (gTBLink.VgaOutSingleLink[0] - 1));
            if ((gTBSingle.VGASingle & tmp) == tmp)
                buf[2] = 1;
            else
                buf[2] = 0;
        }
        for (i = 0; i < 4; i++)
        {
            if (gTBLink.AVOutSingleLink[i] != 0)
            {
                tmp = (0x1 << (gTBLink.AVOutSingleLink[i] - 1));
                if ((gTBSingle.AVSingle & tmp) == tmp)
                    buf[3 + i] = 1;
                else
                    buf[3 + i] = 0;
            }
        }
        if (ps->DEV_Link_State & DEV_LINK_BJCardLK)
        {
            ip = Get_DesIp(DEVICE_SUB_BJCard);
            if (ARP_Rec_Find(ip) == NULL)
            {
                Arp_Request(ip);
                break;
                //return 1;
            }
            else
                UDP_Send_CMD(buf, 10, ip);
        }
        break;
    case TB_CMD_State:
        Mem_Copy((uint8 *)&gTBLink, p + 1, sizeof(struct TB_LinKState_Type));
        break;
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
void TB_init(void)
{
    uint8 i, j, buf[16];
    struct DEVICE_STATUS_TYPE *ps;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    for (i = 1; i < 17; i++)
    {
        for (j = 1; j < 17; j++)
            AV_Select(j, i, FALSE);
    }

    for (i = 0; i < 2; i++)
    {
        j = TB_ReadVer(buf);
        if (j   !=   OS_NO_ERR)
            IP_printf("TB Read Error !");
        else
        {
            ps->DEV_Link_State |= DEV_LINK_TBLK;
            ps->TRACK_ver = buf[4] * 256 + buf[5];
            IP_printf("图像卡版本号:%d.%d", buf[4], buf[5]);
            break;
        }
        IO_FlagLingt_Flash();
    }
    if (ps->DEV_Link_State & DEV_LINK_TBLK)
    {
        TB_SelectAV(TB_InputSinle_OUT_TB0, TB_OutputAV_LMTAV1);
        TB_SelectAV(TB_InputSinle_OUT_TB1, TB_OutputAV_LMTAV2);
        TB_SelectAV(TB_InputSinle_OUT_FZ1, TB_OutputAV_LMTAV3);
        TB_SelectAV(TB_InputSinle_OUT_FZ2, TB_OutputAV_LMTAV4);
        TB_SelectVGA(TB_InputVGA_OUT_TB0, TB_OutputVGA_LMT);
        TB_SelectVGA(TB_InputVGA_OUT_TB1, TB_OutputVGA_TYJ);
    }

    //Device_VGA_Select(DEV_VGA_DVD,DEV_VGA_OUT_TB1);
    Device_VGA_Off(DEV_VGA_OUT_TB0);
    Device_VGA_Off(DEV_VGA_OUT_TB1);
    Device_VGA_Select(DEV_VGA_PC, DEV_VGA_OUT_TB0);
    Device_VGA_Select(DEV_VGA_NPC, DEV_VGA_OUT_TYJ);
    Device_Video_Select(DEV_VIDEO_DVD, DEV_VIDEO_OUT_TB0);
    Device_Video_Select(DEV_VIDEO_DVD, DEV_VIDEO_OUT_TB1);
    Device_Video_Select(DEV_VIDEO_DVD, DEV_VIDEO_OUT_BM2_3);
    Device_Video_Select(DEV_VIDEO_DVD, DEV_VIDEO_OUT_BM2_4);
}



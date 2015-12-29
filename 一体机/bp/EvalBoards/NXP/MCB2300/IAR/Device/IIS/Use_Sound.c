#include "includes.h"
struct SOUND_CONTRL_TYPE Sound_Contrl;
/*const uint8 blank_buf[512]=
{
0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,
0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,
0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,
0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,
0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,
0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,
0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,
0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};*/
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Sound_Echo_Contrl(uint16 *data)
{
    uint16 tmp;
    uint32 *pdata;
    pdata = (uint32 *)data;
    for (tmp = 0; tmp < 32; tmp++)
    {
        if (*pdata != 0)
        {
            if ((Sound_Contrl.play_contrl & SOUND_CONTRL_ECHO) == 0)
            {
                Sound_Contrl.play_contrl |= SOUND_CONTRL_ECHO;
                //IP_printf("SOUND_CONTRL_ECHO=1%4x",*pdata);
            }
            return  1;
        }
        pdata++;
    }
    Sound_Contrl.play_contrl &= ~SOUND_CONTRL_ECHO;
    //IP_printf("SOUND_CONTRL_ECHO=0");
    return 0;
    /*
    if ((Sound_Contrl.play_contrl&SOUND_CONTRL_ECHO)==0)
    {emax=MIC_ECHO_MAX1;
     emin=MIC_ECHO_MIN1;
    }else
    {emax=MIC_ECHO_MAX2;
     emin=MIC_ECHO_MIN2;
    }
    for (tmp=0;tmp<64;tmp++)
    	{ldata=*data++;
    	if ((ldata<emax)&&(ldata>emin))
    		flag++;
    	if (flag>8) break;
    	}
    if (flag>8)
    	{if ((Sound_Contrl.play_contrl&SOUND_CONTRL_ECHO)==0)
    		{ Sound_Contrl.play_contrl|=SOUND_CONTRL_ECHO;
    		 Sound_Contrl.sound_echo_time=150;
    		// TMS320AIC23_Set_Vol(TMS320AIC23_MIC_VOL,10);
    		//IP_printf("SOUND_CONTRL_ECHO=1");
    		}

    	}
    else
    	{if (Sound_Contrl.sound_echo_time>0)
    		Sound_Contrl.sound_echo_time--;
    	if ((Sound_Contrl.sound_echo_time==0)&&(Sound_Contrl.play_contrl&SOUND_CONTRL_ECHO))
    		{Sound_Contrl.play_contrl&=~SOUND_CONTRL_ECHO;
    		// TMS320AIC23_Set_Vol(TMS320AIC23_MIC_VOL,50);
    		//IP_printf("SOUND_CONTRL_ECHO=0");
    		}
    		}
    */
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：
////////////////////////////////////////////////////////////////////
BYTE Sound_Open(BYTE *desip, uint16 Number, uint16 seat, uint16 mic_Contrl)
{
    uint8 tmp;
    for (tmp = 0; tmp < SOUND_OPEN_SIZE; tmp++)
    {
        if (Sound_Contrl.Rec[tmp].contrl & SOUND_CONTRL_ACTIVE) continue;
        memcpy(Sound_Contrl.Rec[tmp].play_ip, desip, 4);
        Sound_Contrl.Rec[tmp].snumber = Number;
        Sound_Contrl.Rec[tmp].seat = seat;
        Sound_Contrl.Rec[tmp].contrl |= SOUND_CONTRL_ACTIVE;
        if (mic_Contrl & SOUND_CONTRL_SEND)
        {
            Sound_Contrl.Rec[tmp].contrl |= mic_Contrl;
            Sound_Contrl.Input_contrl |= SOUND_CONTRL_ACTIVE;


        }
        IP_printf("打开声音%d:%d.%d.%d.%d NUM=%d Seat=%d", tmp, *desip, *(desip + 1), *(desip + 2), *(desip + 3), Number, seat);
        return tmp;
    }

    return 0xff;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：
////////////////////////////////////////////////////////////////////
BYTE Sound_Close(void *pdata , uint8 option)
{
    uint8 tmp, flag = 0;
    switch(option)
    {
    case SOUND_OPTION_IP:
        for (tmp = 0; tmp < SOUND_OPEN_SIZE; tmp++)
        {
            if ((Sound_Contrl.Rec[tmp].contrl & SOUND_CONTRL_ACTIVE) &&
                    (IP_Compare(Sound_Contrl.Rec[tmp].play_ip, (BYTE *)pdata) == TRUE))
            {
                Sound_Contrl.Rec[tmp].contrl = 0;
                flag = TRUE;
                IP_printf("声音通道:%d关闭", tmp);
                break;
            }
        }
        break;
    case SOUND_OPTION_SEAT:
        for (tmp = 0; tmp < SOUND_OPEN_SIZE; tmp++)
        {
            if ((Sound_Contrl.Rec[tmp].contrl & SOUND_CONTRL_ACTIVE) &&
                    (Sound_Contrl.Rec[tmp].seat == *((BYTE *)pdata)))
            {
                Sound_Contrl.Rec[tmp].contrl = 0;
                flag = TRUE;
                IP_printf("声音通道:%d关闭", tmp);
                break;
            }
        }
        break;
    case SOUND_OPTION_NUMBER:
        for (tmp = 0; tmp < SOUND_OPEN_SIZE; tmp++)
        {
            if ((Sound_Contrl.Rec[tmp].contrl & SOUND_CONTRL_ACTIVE) &&
                    (Sound_Contrl.Rec[tmp].snumber == *((uint16 *)pdata)))
            {
                Sound_Contrl.Rec[tmp].contrl = 0;
                flag = TRUE;
                IP_printf("声音通道:%d关闭", tmp);
                break;
            }
        }
        break;
    case SOUND_OPTION_ALL:
        for (tmp = 0; tmp < SOUND_OPEN_SIZE; tmp++)
        {
            Sound_Contrl.Rec[tmp].contrl &= 0;
        }
        Sound_Contrl.Input_contrl = 0;
        Sound_Contrl.play_contrl = 0;
        flag = TRUE;
        IP_printf("声音通道全部关闭", tmp);
        break;

    }
    for (tmp = 0; tmp < SOUND_OPEN_SIZE; tmp++)
    {
        if (Sound_Contrl.Rec[tmp].contrl & SOUND_CONTRL_ACTIVE) break;
    }
    if (tmp >= SOUND_OPEN_SIZE)
    {
        Sound_Contrl.Input_contrl = 0;
        Sound_Contrl.play_contrl = 0;
    }

    return flag;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：
////////////////////////////////////////////////////////////////////
BYTE Sound_Get_Play_No(BYTE *pdata, BYTE Type)
{
    uint8  i;
    for (i = 0; i < SOUND_OPEN_SIZE; i++)
    {
        if (Type == SOUND_OPTION_IP)
        {
            if (IP_Compare(Sound_Contrl.Rec[i].play_ip, pdata)) return i;

        }
        else if (Type == SOUND_OPTION_SEAT)
        {
            if (Sound_Contrl.Rec[i].seat == *pdata) return i;
        }
        if (Type == SOUND_OPTION_NUMBER)
        {
            if (Sound_Contrl.Rec[i].snumber == *((uint16 *)pdata)) return i;
        }
    }
    return 0xff;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：buff 音频数据
//			pdata 播放控制
//			type控制类型
//	出口参数：
////////////////////////////////////////////////////////////////////
void Sound_Save_PlayData(BYTE *buff, BYTE *pContrl, BYTE type)
{
    uint32 i, tmp;
    uint16 *pp;
    i = Sound_Get_Play_No(pContrl, type);
    if (i == 0xff)
        return;
    if ((Sound_Contrl.Rec[i].contrl & SOUND_CONTRL_ACTIVE) == 0)
        return;
    Sound_Contrl.play_buff_flag[Sound_Contrl.Rec[i].save_pointer] = 1;
    if (Sound_Contrl.Rec[i].contrl & SOUND_IP_ASK) //双声道合成单声道
    {
        pp = (uint16 *)buff;
        for (tmp = 0; tmp < SOUND_BUFF_SIZE / 4; tmp++)
            *(pp + tmp * 2 + 1) = 0;
        memcpy((BYTE *)(SOUND_BUFF_ADDRESS + Sound_Contrl.Rec[i].save_pointer * SOUND_BUFF_SIZE), buff, SOUND_BUFF_SIZE);
    }
    else
        memcpy((BYTE *)(SOUND_BUFF_ADDRESS + Sound_Contrl.Rec[i].save_pointer * SOUND_BUFF_SIZE), buff, SOUND_BUFF_SIZE);
    Sound_Contrl.Rec[i].save_pointer++;
    Sound_Contrl.Rec[i].save_pointer = Sound_Contrl.Rec[i].save_pointer % SOUND_BUFF_MAX;
}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：
////////////////////////////////////////////////////////////////////
void Sound_Pointer_Scan(void)
{
    uint8 tmp, next;
    next = (Sound_Contrl.play_pointer + 1) % SOUND_BUFF_MAX;
    for (tmp = 0; tmp < SOUND_OPEN_SIZE; tmp++)
    {
        if ((Sound_Contrl.Rec[tmp].save_pointer == next) || (Sound_Contrl.Rec[tmp].save_pointer == Sound_Contrl.play_pointer))
        {
            Sound_Contrl.Rec[tmp].save_pointer += SOUND_BUFF_MAX / 2;
            Sound_Contrl.Rec[tmp].save_pointer = Sound_Contrl.Rec[tmp].save_pointer % SOUND_BUFF_MAX;

        }
        Sound_Contrl.play_buff_flag[Sound_Contrl.play_pointer] = 0;
    }


}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：下一次DMA地址
////////////////////////////////////////////////////////////////////
uint32 Sound_Play_Int(void)
{
    uint32 *p, tmp, last, *pbuf;
    Sound_Contrl.play_pointer = DMA_Get_Play_Pointer();
    last = (Sound_Contrl.play_pointer + SOUND_BUFF_MAX - 1) % SOUND_BUFF_MAX;
    Sound_Pointer_Scan();
    Sound_Contrl.play_contrl |= SOUND_DMA1_OK;
    p = (uint32 *)(SOUND_BUFF_ADDRESS + last * SOUND_BUFF_SIZE);
    pbuf = (uint32 *)(SOUND_BUFF_ADDRESS + SOUND_BUFF_MAX * SOUND_BUFF_SIZE);
    //Sound_Echo_Contrl((uint16 *)p);
    //Mem_Copy(pbuf, p, SOUND_BUFF_SIZE/4);
    for (tmp = 0; tmp < SOUND_BUFF_SIZE / 4; tmp++)  *(pbuf + tmp) = *(p + tmp);
    for (tmp = 0; tmp < SOUND_BUFF_SIZE / 4; tmp++)  *p++ = 0;
    Sound_Contrl.play_buff_flag[Sound_Contrl.play_pointer] = 0;
    Sound_Contrl.play_pointer++;
    Sound_Contrl.play_pointer = Sound_Contrl.play_pointer % SOUND_BUFF_MAX;
    return 0;
}

/////////////////////////////////////////////////////////////////////
//  函数名称：
//  函数功能：
//  入口参数：
//
//
//  出口参数：下一次DMA地址
////////////////////////////////////////////////////////////////////
void mic_play_add(uint8 *mbuf, uint8 *pbuf)
{
    uint16 *mp, *pp, i;
    mp = (uint16 *)mbuf;
    pp = (uint16 *)pbuf;
    for (i = 0; i < SOUND_BUFF_SIZE / 4; i++)
    {
        *(mp + 1 + i * 2) = *(pp + i * 2);
    }


}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：下一次DMA地址
////////////////////////////////////////////////////////////////////
uint32 Sound_Input_Int(void)
{
    uint8 *pbuf, *p;
    uint32 i;
    struct _pkst micst, dst;
    struct SOUND_HEADER_TYPE shead;
    NetP *pnet;

    Sound_Contrl.play_contrl |= SOUND_DMA2_OK;
    if ((Sound_Contrl.Input_contrl & SOUND_CONTRL_ACTIVE) == 0)
        return  0;
    pnet = (NetP *)Get_NetPort();
    for (i = 0; i < SOUND_OPEN_SIZE; i++)
    {
        if (Sound_Contrl.Rec[i].contrl & SOUND_CONTRL_SEND)
        {

            if ((Sound_Contrl.Rec[i].contrl & SOUND_IP_ASK) != 0)
            {
                pbuf = (BYTE *)DMA_Get_MICaddress();
                p = (uint8 *)(SOUND_BUFF_ADDRESS + SOUND_BUFF_MAX * SOUND_BUFF_SIZE);
                mic_play_add(pbuf, p);
                dst.DAPTR = pbuf;
            }
            else if ((Sound_Contrl.Rec[i].contrl & SOUND_IP_hASK) != 0)
            {
                pbuf = (BYTE *)DMA_Get_MICaddress();
                dst.DAPTR = pbuf;
                if ((Sound_Contrl.Rec[i].contrl & SOUND_CONTRL_ECHO) != 0)
                {
                    //dst.DAPTR=(BYTE *)blank_buf;
                    Mem_Clr(pbuf, SOUND_BUFF_SIZE);
                }
                //else

            }
            else
                dst.DAPTR = (BYTE *)DMA_Get_MICaddress();
            dst.STPTR = NULL;
            dst.length = SOUND_BUFF_SIZE;

            shead.Flag = FLAG_SOUND_HEADER;
            shead.Seat = 0;
            //shead.OWIP=pnet->My_Ip;//IPByte_TO_Dword(pnet->My_Ip);
            memcpy( &shead.OWIP, &pnet->My_Ip[0], 4 );
            micst.STPTR = &dst;
            micst.DAPTR = (BYTE *)&shead;
            micst.length = sizeof( struct SOUND_HEADER_TYPE);
            Udp_SendPacked(&micst, Sound_Contrl.Rec[i].play_ip, Sound_Contrl.Rec[i].snumber);
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
uint8 Sound_init(void)
{
    NetP *pnet;
    uint8 tmp;
    uint16 i;
    // memset(blank_buf,0,512);
    memset((BYTE *)&Sound_Contrl, 0, sizeof(struct SOUND_CONTRL_TYPE));
    TMS320AIC23_init();
    I2SInit();
    DMA_Init();
    I2SStart();
    pnet = (NetP *)Get_NetPort();
    tmp = pnet->Test_TTL;
    pnet->Test_TTL = 6;
    for (i = 0; i < 20000; i++)
    {
        if ( Sound_Contrl.play_contrl & SOUND_DMA1_OK)
        {
            IP_printf("IP:%d.%d.%d.%d声卡DMA1打开成功", pnet->My_Ip[0], pnet->My_Ip[1], pnet->My_Ip[2], pnet->My_Ip[3]);
            break;
        }
        IO_FlagLingt_Flash();
    }
    if (( Sound_Contrl.play_contrl & SOUND_DMA1_OK) == 0)
        IP_printf("IP:%d.%d.%d.%d声卡DMA1打开失败", pnet->My_Ip[0], pnet->My_Ip[1], pnet->My_Ip[2], pnet->My_Ip[3]);
    for (i = 0; i < 10000; i++)
    {
        if ( Sound_Contrl.play_contrl & SOUND_DMA2_OK)
        {
            IP_printf("IP:%d.%d.%d.%d声卡DMA2打开成功", pnet->My_Ip[0], pnet->My_Ip[1], pnet->My_Ip[2], pnet->My_Ip[3]);

            break;
        }
        IO_FlagLingt_Flash();
    }
    if (( Sound_Contrl.play_contrl & SOUND_DMA1_OK) == 0)
        IP_printf("IP:%d.%d.%d.%d声卡DMA2打开失败", pnet->My_Ip[0], pnet->My_Ip[1], pnet->My_Ip[2], pnet->My_Ip[3]);


    pnet->Test_TTL = tmp;
    return 0;
}





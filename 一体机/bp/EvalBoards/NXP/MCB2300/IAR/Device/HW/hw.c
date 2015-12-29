#include  "includes.h"
struct DEV_HW_TYPE  HW_DEVICE;
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void On_MSG_OPTION_HW_END(void *pdata)
{

}

/////////////////////////////////////////////////////////////////////
//	函数名称：void HW_XM_start(struct BASE_MSG *keymsg)
//	函数功能：红外学码
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void HW_XM_start(void *pdata)
{
    struct BASE_MSG *keymsg, *pmsg;
    struct MSG_REV_TYPE *prev;
    BYTE *p, *pcap, flag;
    uint16 tmp;
    uint32 Treg;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif
    prev = (struct MSG_REV_TYPE *)pdata;
    keymsg = (struct BASE_MSG *)pdata + sizeof(struct MSG_REV_TYPE);
    pmsg = (struct BASE_MSG *)((BYTE *)pdata + 2 + sizeof(struct MSG_REV_TYPE));
    HW_DEVICE.XM.cap0_time = 0;
    HW_DEVICE.XM.buf = (BYTE *)User_mem_allco(600);
    if (HW_DEVICE.XM.buf == NULL)
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("红外学码内存申请失败!");
        HW_DEVICE.flag = 0;
        return;
    }
    HW_DEVICE.XM.lg = 7;
    p = (BYTE *)HW_DEVICE.XM.buf;
    *p = MSG_DEV_MAINBARD;
    *(p + 1) = MSG_OPTION_HW_END;
    *(p + 2) = pmsg->device;
    *(p + 3) = pmsg->option;
    *(p + 4) = 0;
    *(p + 5) = 0;
    *(p + 6) = 0;
    T0TCR = 0;
    T0CTCR = 0; //计时模式
    T0PR = HW_PR_TIME10us; //预分频10us TC+1计数
    T0TC = 0;
    T0MR0 = HW_TIME1MIN / 4; //  1/2分钟定时
    T0MCR = 1; //MAT0 中断
    T0CCR = 0x38; //上升捕获时中断
    T0TCR = 2; //定时器0复位
    T0IR = 0xff;
    T0TCR = 1; //定时器0计时
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("红外学码启动，请按遥控器\n\n\n");

    OS_ENTER_CRITICAL();
    //检测第一个
    do
    {
        flag = T0IR;
        T0IR = flag;
        Treg = T0TC;
        if ((flag & 0x01) || (Treg >= T0MR0))
        {
            //
            T0TCR = 0;
            *(p + 4) = 0x01;
            *(p + 5) = 0;
            *(p + 6) = 0;
            UDP_Send_CMD(p, HW_DEVICE.XM.lg, prev->des_ip);
            HW_DEVICE.flag = 0;
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("红外学码超时\n\n\n");
            User_mem_free(p);
            HW_DEVICE.XM.buf = NULL;
            HW_DEVICE.XM.cap0_time = 0;
            OS_EXIT_CRITICAL();
            return;
        }
        else if (flag & 0x20)
        {
            T0MR0 = HW_TIME10MS * 2 + T0TC; //
            //T0CCR=0x07;//捕获下降
            HW_DEVICE.flag |= DEV_HW_FLAG_XMDWON;
            HW_DEVICE.XM.cap0_time = T0CR1; //下降沿时间
            pcap = HW_DEVICE.XM.buf;
            *(pcap + HW_DEVICE.XM.lg) = 0x7c; //38k
            HW_DEVICE.XM.lg++;
            *(pcap + HW_DEVICE.XM.lg) = 1;
            HW_DEVICE.XM.lg++;
            break;
        }
        IO_FlagLingt_Flash();
    }
    while(1);
    do
    {
        flag = T0IR;
        T0IR = flag;
        if (flag & 0x20)
        {
            T0MR0 = HW_TIME10MS * 10 + T0TC; //
            tmp = (uint16)(T0CR1 - HW_DEVICE.XM.cap0_time);
            HW_DEVICE.XM.cap0_time = T0CR1;
            *(pcap + HW_DEVICE.XM.lg) = tmp & 0xff;
            HW_DEVICE.XM.lg++;
            *(pcap + HW_DEVICE.XM.lg) = tmp >> 8;
            HW_DEVICE.XM.lg++;
            if (HW_DEVICE.XM.lg > HW_DATA_SIZE * 4)
            {
                break;
            };//学满128位码

        }
        else
        {
            Treg = T0TC;
            if ((flag & 0x01) || (Treg >= T0MR0))	 break;
        }
        IO_FlagLingt_Flash();
    }
    while(1);
    OS_EXIT_CRITICAL();
    *(p + 4) = 0;
    *(p + 5) = HW_DEVICE.XM.lg & 0xff;
    *(p + 6) = HW_DEVICE.XM.lg >> 8;
    UDP_Send_CMD(p, HW_DEVICE.XM.lg, prev->des_ip);
    HW_DEVICE.flag = 0;
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("红外学码完成-长度:%d\n\n\n", HW_DEVICE.XM.lg);
    User_mem_free(p);
    HW_DEVICE.XM.buf = NULL;
    HW_DEVICE.XM.cap0_time = 0;
    T0TCR = 0;
    T0CCR = 0;
    T0MCR = 0;
    keymsg = keymsg;
    return;

}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE  HW_send(uint8 id)
{
    uint16 tmp, flag;
    uint8 *p, *pend;
    struct MESSAGE_TYPE *phw;
    uint32 Treg;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif

    HW_DEVICE.send.Curr_id = id;
    phw = (struct MESSAGE_TYPE *)(ARM_TABLE_ADDRESS + 256 * HW_DEVICE.send.Curr_id);
    HW_DEVICE.send.Currlength = phw->data_lg;
    HW_DEVICE.send.CurrSendLg = 0;
    p = (BYTE *)(ARM_TABLE_ADDRESS + 256 * HW_DEVICE.send.Curr_id + sizeof(struct MESSAGE_TYPE ));
    pend = p +	phw->data_lg;
    tmp = *(p + 1) * 256 + *p; //载波频率38K
    if (tmp == 0)  return 0;
    p += 2;

    T1CTCR = 0;
    T1PR = 0; //Fpclk计数
    T1MR0 = Fpclk / (tmp * 200) - 1; //76k
    T1MCR = 2; //匹配计数器清零
    T1EMR = 0xc2; //MT0翻转
    T1TCR = 2;


    T0TCR = 0;
    T0CTCR = 0; //计时模式
    T0PR = HW_PR_TIME10us; //预分频10us TC+1计数
    T0TC = 0; //计数器
    T0TCR = 2; //复位计数器
    T0MCR = 3; //匹配产生中断，计数器清零Tc=0;
    T0CCR = 0;
    tmp = *(p + 1) * 256 + *p;
    p += 2;
    if (tmp == 0) return 0;
    OS_ENTER_CRITICAL();
    T0MR0 = tmp;
    HW_DEVICE.flag |= DEV_HW_FLAG_SEND;
    T0IR = 0xff;
    T0TCR = 1;
    T1TCR = 1;
    for (;;)
    {
        flag = T0IR;
        Treg = T0TC;
        if ((flag & 0x01) || (Treg >= T0MR0))
        {
            T0IR = 0x01;
            T0TC = 0;
            if (HW_DEVICE.flag & DEV_HW_FLAG_SEND)
            {
                HW_DEVICE.flag &= ~DEV_HW_FLAG_SEND;
                T1TCR = 0;
            }
            else
            {
                HW_DEVICE.flag |= DEV_HW_FLAG_SEND;
                T1TCR = 1;
            }
            tmp = *(p + 1) * 256 + *p;
            p += 2;
            if (tmp == 0) break;
            T0MR0 = tmp;	//T0TC=0;
            if (p >= pend)
            {
                T0TCR = 0;
                T1EMR = 0x81;
                T1TCR = 1;
                for (tmp = 0; tmp < 500; tmp++);
                T1TCR = 0;
                break;
            }

        }
        IO_FlagLingt_Flash();
    }
    flag = flag;

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
void Time1_Exception(void)
{
    //VICVectAddr=0x00;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void  Time0_Exception(void)
{
#if 0
    uint8 buf[10], *p;
    uint32 pdata;
    uint32 flag;

fiq1:
    flag = T0IR;
    T0IR = T0IR;
    if (flag & 0x10)
    {
        if  (( HW_DEVICE.flag & DEV_HW_FLAG_XM) == DEV_HW_FLAG_XM)
        {
            T0MR0 = HW_TIME10MS * 2 + T0TC; //
            pdata = (T0CR0 - HW_DEVICE.XM.cap0_time) | HW_DATA_SEND;
            HW_DEVICE.XM.cap0_time = T0CR0;
            p = (BYTE *)HW_DEVICE.XM.buf;
            *(p + HW_DEVICE.XM.lg) = pdata & 0xff;
            HW_DEVICE.XM.lg++;
            *(p + HW_DEVICE.XM.lg) = pdata >> 8;
            HW_DEVICE.XM.lg++;
            if (HW_DEVICE.XM.lg > HW_DATA_SIZE * 4)
            {
                //学满128位码
                HW_DEVICE.flag |= DEV_HW_FLAG_XM_OK;
            }
        }
    }
    else if (flag & 0x20) //捕获中断
    {
        if  (( HW_DEVICE.flag & DEV_HW_FLAG_XM) == DEV_HW_FLAG_XM)
        {
            //学码
            if ( (HW_DEVICE.flag & DEV_HW_FLAG_XMFIRST) == DEV_HW_FLAG_XMFIRST)
            {
                //红外学码启动
                T0MR0 = HW_TIME10MS * 2 + T0TC; //
                //T0CCR=0x07;//捕获下降
                HW_DEVICE.flag &= ~DEV_HW_FLAG_XMFIRST;
                HW_DEVICE.flag |= DEV_HW_FLAG_XMDWON;
                HW_DEVICE.XM.cap0_time = T0CR1; //下降沿时间

            }
            else
            {
                //
                T0MR0 = HW_TIME10MS * 2 + T0TC; //
                pdata = (T0CR1 - HW_DEVICE.XM.cap0_time);
                HW_DEVICE.XM.cap0_time = T0CR1;
                p = (BYTE *)HW_DEVICE.XM.buf;
                *(p + HW_DEVICE.XM.lg) = pdata & 0xff;
                HW_DEVICE.XM.lg++;
                *(p + HW_DEVICE.XM.lg) = pdata >> 8;
                HW_DEVICE.XM.lg++;
                if (HW_DEVICE.XM.lg > HW_DATA_SIZE * 4)
                {
                    //学满128位码
                    HW_DEVICE.flag |= DEV_HW_FLAG_XM_OK;
                }

            }
        }
    }
    else if (flag & 01) //匹配中断
    {
        if  (( HW_DEVICE.flag & DEV_HW_FLAG_XM) == DEV_HW_FLAG_XM)
        {
            //学码
            if ( (HW_DEVICE.flag & DEV_HW_FLAG_XMFIRST) == DEV_HW_FLAG_XMFIRST)
            {
                //一分钟超时中断
                HW_DEVICE.flag |= DEV_HW_FLAG_XM_OVER;
                T0CCR = 0x36; //上升捕获时中断
            }
            else
            {
                //20ms中断学码完成
                HW_DEVICE.flag |= DEV_HW_FLAG_XM_OK;
                T0CCR = 0x36; //上升捕获时中断
            }

        }
        else if (HW_DEVICE.flag & DEV_HW_FLAG_SEND)
        {
            //发码
            T1TCR = 2;
            pdata = HW_Get_data();
            if (pdata == 0)
            {
                T0TCR = 0;
            }
            else
            {
                if ((pdata & (~HW_DATA_SEND)) > T0TC)
                    T0MR0 = (pdata & (~HW_DATA_SEND));
                else
                    T0MR0 = (pdata & (~HW_DATA_SEND)) + T0TC;

                if (pdata & HW_DATA_SEND)
                    T1TCR = 1; //启动定时器TIME2
                else
                    T1TCR = 0; //
            }
        }

    }
    //T0IR=0xff;
    if (T0IR != 0) goto fiq1;
    //V/ICVectAddr=0x00;
    return ;
#endif
    //VICVectAddr=0x00;
}
//////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void HW_init(void)
{
    memset((BYTE *)&HW_DEVICE, 0, sizeof (struct DEV_HW_TYPE));
    T0TCR = 0;
    T1TCR = 0;

}

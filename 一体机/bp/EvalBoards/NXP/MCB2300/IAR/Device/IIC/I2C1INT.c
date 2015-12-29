
/*
---------------------------------------------------------------------------------------------------------
*********************************************************************************************************
** 文件名称 ：	I2C1INT.c
** 功能说明 ：	LPC2000硬件I2C1中断方式软件包。
** 使用说明 ：  主程序要配置好I2C1总线接口(I2C1引脚功能和I2C1中断，并已使能I2C1主模式)
*********************************************************************************************************
---------------------------------------------------------------------------------------------------------
*/
#include  "includes.h"

/* 定义用于和I2C中断传递信息的全局变量 */
uint8 	I2C1_sla;			/* I2C1器件从地址 					*/
uint32	I2C1_suba;			/* I2C1器件内部子地址 				*/
uint8 	I2C1_suba_num;		/* I2C1子地址字节数					*/
uint8 	*I2C1_buf;        	/* 数据缓冲区指针  					*/
uint32 I2C1_num;			/* 要读取/写入的数据个数 			*/
uint8 	I2C1_end;			/* I2C1总线结束标志：结束总线是置1 	*/
uint8 	I2C1_suba_en;		/* 	子地址控制。
										0--子地址已经处理或者不需要子地址
										1--读取操作
										2--写操作
									*/

/*
**********************************************************************************************************
** 函数名称：ISendByte()
** 函数功能：向无子地址器件发送1字节数据。
** 入口参数：sla		器件地址
**           dat		要发送的数据
** 出口参数：返回值为0时表示出错，为1时表示操作正确。
** 说明：	使用前要初始化好I2C1引脚功能和I2C1中断，并已使能I2C1主模式
*********************************************************************************************************
*/
uint8  ISendByte1(uint8 sla, uint8 dat)
{
    /* 参数设置 */
    uint32 i = 0;
    I2C1_sla     = sla;		// 写操作的器件地址
    I2C1_buf     = &dat;		// 待发送的数据
    I2C1_num     = 1;			// 发送1字节数据
    I2C1_suba_en = 0;		 	// 无子地址
    I2C1_end     = 0;

    I21CONCLR = 0x2C;
    I21CONSET = 0x60;             // 设置为主机，并启动总线

    while(0 == I2C1_end)
    {
        i++;
        if (i > 100000) break;
    };

    if(1 == I2C1_end) return(1);
    else return(0);
}

/*
*********************************************************************************************************
** 函数名称：IRcvByte()
** 函数功能：向无子地址器件读取1字节数据。
** 入口参数：sla		器件地址
**           dat		接收数据的变量指针
** 出口参数：返回值为0时表示操作出错，为1时表示操作正确。
** 说明：使用前要初始化好I2C1引脚功能和I2C1中断，并已使能I2C1主模式
*********************************************************************************************************
*/
uint8 IRcvByte1(uint8 sla, uint8 *dat)
{
    /* 参数设置 */
    uint32 i = 0;
    I2C1_sla     = sla + 1;		// 读操作的器件地址
    I2C1_buf     = dat;
    I2C1_num     = 1;
    I2C1_suba_en = 0;			// 无子地址
    I2C1_end     = 0;

    I21CONCLR = 0x2C;
    I21CONSET = 0x60;         // 设置为主机，并启动总线

    while(0 == I2C1_end)
    {
        i++;
        if (i > 100000) break;
    };
    return I2C1_end;
}

/*
*********************************************************************************************************
** 函数名称 ：I2C_ReadNByte()
** 函数功能 ：从有子地址器件任意地址开始读取N字节数据
** 入口参数 ：	sla			器件从地址
**				suba_type	子地址结构	1－单字节地址	2－8+X结构	2－双字节地址
**				suba		器件子地址
**				s			数据接收缓冲区指针
**				num			读取的个数
** 出口参数 ：	TRUE		操作成功
**				FALSE		操作失败
*********************************************************************************************************
*/
uint8 I2C1_ReadNByte (uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num)
{
    uint32 i = 0;
    if (num > 0)	/* 判断num个数的合法性 */
    {
        /* 参数设置 */
        if (suba_type == 1)
        {
            /* 子地址为单字节 */
            I2C1_sla     	= sla + 1;							/* 读器件的从地址，R=1 	*/
            I2C1_suba    	= suba;								/* 器件子地址 			*/
            I2C1_suba_num	= 1;								/* 器件子地址为1字节 	*/
        }
        if (suba_type == 2)
        {
            /* 子地址为2字节 */
            I2C1_sla     	= sla + 1;							/* 读器件的从地址，R=1 	*/
            I2C1_suba   	 	= suba;							/* 器件子地址 			*/
            I2C1_suba_num	= 2;								/* 器件子地址为2字节 	*/
        }
        if (suba_type == 3)
        {
            /* 子地址结构为8+X*/
            I2C1_sla			= sla + ((suba >> 7 ) & 0x0e) + 1; /* 读器件的从地址，R=1	*/
            I2C1_suba		= suba & 0x0ff;						/* 器件子地址	 		*/
            I2C1_suba_num	= 1;								/* 器件子地址为8+x	 	*/
        }
        I2C1_buf     = s;										/* 数据接收缓冲区指针 	*/
        I2C1_num     = num;										/* 要读取的个数 		*/
        I2C1_suba_en = 1;										/* 有子地址读 			*/
        I2C1_end     = 0;

        /* 清除STA,SI,AA标志位 */
        I21CONCLR = (1 << 2) |	/* AA 		*/
                    (1 << 3) |	/* SI 		*/
                    (1 << 5);	/* STA 		*/

        /* 置位STA,启动I2C1总线 */
        I21CONSET = (1 << 5) |	/* STA 		*/
                    (1 << 6);	/* I2CEN 	*/

        /* 等待I2C操作完成 */
        while (I2C1_end == 0)
        {
            //if (i++ > 10000*num)
            //	return FALSE;
            if (i++ > 1000 * num)    //2013.2.28解决程序无法升级问题
                return FALSE;
            //IO_FlagLingt_Flash();   //2013.2.28解决程序无法升级问题
        }
        return I2C1_end ;
    }
    return (FALSE);
}

/*
*********************************************************************************************************
** 函数名称 ：I2C_WriteNByte()
** 函数功能 ：向有子地址器件写入N字节数据
** 入口参数 ：	sla			器件从地址
**				suba_type	子地址结构	1－单字节地址	3－8+X结构	2－双字节地址
**			  	suba		器件内部物理地址
**			  	*s			将要写入的数据的指针
**			  	num			将要写入的数据的个数
** 出口参数 ：	TRUE		操作成功
**			  	FALSE		操作失败
*********************************************************************************************************
*/
uint8 I2C1_WriteNByte(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num)
{
    uint32 i = 0;
    if (num > 0)/* 如果读取的个数为0，则返回错误 */
    {
        /* 设置参数 */
        if (suba_type == 1)
        {
            /* 子地址为单字节 */
            I2C1_sla     	= sla & 0xFE;								/* 读器件的从地址	 	*/
            I2C1_suba    	= suba;								/* 器件子地址 			*/
            I2C1_suba_num	= 1;								/* 器件子地址为1字节 	*/
        }
        if (suba_type == 2)
        {
            /* 子地址为2字节 */
            I2C1_sla     	= sla;								/* 读器件的从地址 		*/
            I2C1_suba   	 	= suba;							/* 器件子地址 			*/
            I2C1_suba_num	= 2;								/* 器件子地址为2字节 	*/
        }
        if (suba_type == 3)
        {
            /* 子地址结构为8+X */
            I2C1_sla			= sla + ((suba >> 7 ) & 0x0e);	/* 读器件的从地址		*/
            I2C1_suba		= suba & 0x0ff;						/* 器件子地址			*/
            I2C1_suba_num	= 1;								/* 器件子地址为8+X	 	*/
        }

        I2C1_buf     = s;										/* 数据 				*/
        I2C1_num     = num;										/* 数据个数 			*/
        I2C1_suba_en = 2;										/* 有子地址，写操作 	*/
        I2C1_end     = 0;

        /* 清除STA,SI,AA标志位 */
        I21CONCLR = (1 << 2) |	/* AA 	*/
                    (1 << 3) |	/* SI 	*/
                    (1 << 5);	/* STA 	*/

        /* 置位STA,启动I2C总线 */
        I21CONSET = (1 << 5) |	/* STA 	*/
                    (1 << 6);	/* I2CEN*/

        /* 等待I2C操作完成 */
        while (I2C1_end == 0)
        {
            if (i++ > 1000000)
                return FALSE;
            IO_FlagLingt_Flash();
        }
        return I2C1_end;
    }
    return (FALSE);
}

/*
*********************************************************************************************************
** 函数名称 ：__irq IRQ_I2C1()
** 函数名次 ：硬件I2C1中断服务程序。
** 入口参数 ：无
** 出口参数 ：无
** 说明     ：注意处理子地址为2字节的情况。
*********************************************************************************************************
*/
void I2C1_Exception(void)
{
    /* 读取I2C状态寄存器I2C1DAT */
    /* 按照全局变量的设置进行操作及设置软件标志 */
    /* 清除中断逻辑,中断返回 */

    switch (I21STAT & 0xF8)
    {
    /* 根据状态码进行相应的处理 */
    case 0x08:	/* 已发送起始条件 */				/* 主发送和主接收都有 		*/
        /* 装入SLA+W或者SLA+R */
        if(I2C1_suba_en == 1)/* SLA+R */			/* 指定子地址读 			*/
        {
            I21DAT = I2C1_sla & 0xFE; 				/* 先写入地址 				*/
        }
        else				/* SLA+W */
        {
            I21DAT = I2C1_sla;        				/* 否则直接发送从机地址 	*/
        }
        /* 清零SI位 */
        I21CONCLR =	(1 << 3) |						/* SI 						*/
                    (1 << 5);						/* STA 						*/
        break;
    case 0x10:	/*已发送重复起始条件 */ 			/* 主发送和主接收都有 		*/
        /* 装入SLA+W或者SLA+R */
        I21DAT = I2C1_sla;							/* 重起总线后，重发从地址 	*/
        I21CONCLR = 0x28;							/* 清零SI,STA */
        break;
    case 0x18:
    case 0x28:	/* 已发送I2C1DAT中的数据，已接收ACK */
        if (I2C1_suba_en == 0)
        {
            if (I2C1_num > 0)
            {
                I21DAT = *I2C1_buf++;
                I21CONCLR = 0x28;					/* 清零SI,STA 				*/
                I2C1_num--;
            }
            else	/* 没有数据发送了 */
            {
                /* 停止总线 */
                I21CONSET = (1 << 4);				/* STO 						*/
                I21CONCLR = 0x28;					/* 清零SI,STA 				*/
                I2C1_end = 1;						/* 总线已经停止 			*/
            }
        }

        if(I2C1_suba_en == 1)	/* 若是指定地址读，则重新启动总线 				*/
        {
            if (I2C1_suba_num == 2)
            {
                I21DAT = ((I2C1_suba >> 8) & 0xff);
                I21CONCLR = 0x28;					/* 清零SI,STA 				*/
                I2C1_suba_num--;
                break;
            }

            if(I2C1_suba_num == 1)
            {
                I21DAT = (I2C1_suba & 0xff);
                I21CONCLR = 0x28;					/* 清零SI,STA 				*/
                I2C1_suba_num--;
                break;
            }

            if (I2C1_suba_num == 0)
            {
                I21CONSET = 0x20;
                I21CONCLR = 0x08;
                I2C1_suba_en = 0;     				/* 子地址己处理 			*/
                break;
            }
        }

        if (I2C1_suba_en == 2)/* 指定子地址写,子地址尚未指定,则发送子地址 		*/
        {
            if (I2C1_suba_num > 0)
            {
                if (I2C1_suba_num == 2)
                {
                    I21DAT = ((I2C1_suba >> 8) & 0xff);
                    I21CONCLR = 0x28;
                    I2C1_suba_num--;
                    break;
                }
                if (I2C1_suba_num == 1)
                {
                    I21DAT    = (I2C1_suba & 0xff);
                    I21CONCLR = 0x28;
                    I2C1_suba_num--;
                    I2C1_suba_en  = 0;
                    break;
                }
            }
        }
        break;
    case 0x40:	/* 已发送SLA+R,已接收ACK */
        if (I2C1_num <= 1)	/* 如果是最后一个字节 */
        {
            I21CONCLR = 1 << 2;      				/* 下次发送非应答信号 		*/
        }
        else
        {
            I21CONSET = 1 << 2;					/* 下次发送应答信号 		*/
        }
        I21CONCLR = 0x28;							/* 清零SI,STA 				*/
        break;
    case 0x20:	/* 已发送SLA+W,已接收非应答              */
    case 0x30:	/* 已发送I2C1DAT中的数据，已接收非应答   */
    case 0x38:	/* 在SLA+R/W或数据字节中丢失仲裁         */
    case 0x48:	/* 已发送SLA+R,已接收非应答              */
        I21CONCLR = 0x28;
        I2C1_end = 0x0;
        break;
    case 0x50:	/* 已接收数据字节，已返回ACK */
        if (I2C1_buf != NULL)
            *I2C1_buf++ = I21DAT;
        I2C1_num--;
        if (I2C1_num == 1)/* 接收最后一个字节 */
        {
            I21CONCLR = 0x2C;						/* STA,SI,AA = 0 			*/
        }
        else
        {
            I21CONSET = 0x04;						/* AA=1 					*/
            I21CONCLR = 0x28;
        }
        break;
    case 0x58:	/* 已接收数据字节，已返回非应答 */
        if (I2C1_buf != NULL)
            *I2C1_buf++ = I21DAT;     					/* 读取最后一字节数据 		*/
        I21CONSET = 0x10;        					/* 结束总线 				*/
        I21CONCLR = 0x28;
        I2C1_end = 1;
        break;
    default:
        break;
    }
    //VICVectAddr = 0x00;              					/* 中断处理结束 			*/
}

//////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE AT24C16_Read(BYTE *buf, uint16 address, uint16 lg)
{
    uint8 dev_address, sub_address;
    dev_address = 0xa0 + ((address >> 7) & 0xFE);
    sub_address = address & 0xff;
    return I2C1_ReadNByte(dev_address, ONE_BYTE_SUBA, sub_address, buf, lg);
}

//////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE AT24C16_Write( BYTE *buf, uint16 address, uint16 lg)
{
    uint8 dev_address, sub_address, flag;
    uint16 tmp, tmp_add, tmp_lg;
    uint8 rbuf[2];
    uint32 etime;
#if 0
    if (lg <= 16)
    {
        dev_address = 0xa0 + ((address >> 7) & 0xFE);
        sub_address = address & 0xff;
        flag = I2C1_WriteNByte(dev_address, ONE_BYTE_SUBA, sub_address, buf, lg);
        etime == 0;
        do
        {
            flag = I2C1_ReadNByte(dev_address, ONE_BYTE_SUBA, sub_address, rbuf, 1);
            if (etime++ > 1000) break;
        }
        while(flag == FALSE);
        return  flag;
    }
#endif

    tmp_lg = lg;
    tmp_add = address;
    tmp = 0;
    for (;;)
    {
        dev_address = 0xa0 + ((address >> 7) & 0xFE);
        sub_address = tmp_add & 0xff;
        if (tmp_lg >= 16)
        {
            flag = I2C1_WriteNByte(dev_address, ONE_BYTE_SUBA, sub_address, buf + tmp, 16);
            etime = 0;
            do
            {
                flag = I2C1_ReadNByte(dev_address, ONE_BYTE_SUBA, sub_address, rbuf, 1);
                if (etime++ > 1000)
                    break;
                IO_FlagLingt_Flash();
            }
            while(flag != TRUE);
            if (tmp_lg <= 16)
                return flag;
        }
        else
        {
            flag = I2C1_WriteNByte(dev_address, ONE_BYTE_SUBA, sub_address, buf + tmp, tmp_lg);
            etime = 0;
            do
            {
                flag = I2C1_ReadNByte(dev_address, ONE_BYTE_SUBA, sub_address, rbuf, 1);
                if (etime++ > 1000)
                    break;
            }
            while(flag != TRUE);
        }

        tmp += 16;
        tmp_add += 16;
        if ((tmp_lg > 16) && (flag))
            tmp_lg -= 16;
        else
            return flag;
    }
}

//////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
/////////////////////////////////////////////////////////
void I2c1Init(uint32 Fi2c)
{
    if (Fi2c > 400000)
        Fi2c = 400000;

    I21SCLH = (Fpclk / Fi2c + 1) / 2;						/* 设定I2C时钟 		*/
    I21SCLL = (Fpclk / Fi2c) / 2;
    I21CONCLR = 0x6C;
    I21CONSET = 0x40;									/* 使能主I2C 		*/
    SetVICIRQ(VIC_CHANLE_I2C1, 10, (uint32)I2C1_Exception);
}

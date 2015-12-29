#include  "includes.h"
//UART_BAN  uart_config[4];
extern struct COMM_XCOM XCOM_PORT[DEV_XCOM_MAX_SIZE];
/********************************************************************************************************
** 函数名称：MSPI_Init()
** 函数功能：初始化SPI接口，设置为主机。
** 入口参数：无
** 出口参数：无
*********************************************************************************************************/

void  SPI_Init(void)
{
    S0SPCCR = 0x52;		            // 设置SPI时钟分频
    S0SPCR  = 	 (0 << 2) |
                 (0 << 3) |				// CPHA = 0, 数据在SCK 的第一个时钟沿采样
                 (1 << 4) |				// CPOL = 1, SCK 为低有效
                 (1 << 5) |				// MSTR = 1, SPI 处于主模式
                 (0 << 6) |				// LSBF = 0, SPI 数据传输MSB (位7)在先
                 (0 << 7);				// SPIE = 0, SPI 中断被禁止
}
/********************************************************************************************************
** 函数名称：MSPI_SendData()
** 函数功能：向SPI总线发送数据。
** 入口参数：data   待发送的数据
** 出口参数：返回值为读取的数据
********************************************************************************************************/
uint8  SPI_send(uint8 data)
{



    S0SPDR = data;
    while( 0 == (S0SPSR & 0x80));		// 等待SPIF置位，即等待数据发送完毕

    return(S0SPDR);
}
/********************************************************************************************************
** 函数名称：MSPI_SendData()
** 函数功能：向SPI总线发送数据。
** 入口参数：data   待发送的数据
** 出口参数：返回值为读取的数据
********************************************************************************************************/
uint16  SPI_send1(uint16 data)
{



    S0SPDR = data;
    while( 0 == (S0SPSR & 0x80));		// 等待SPIF置位，即等待数据发送完毕

    return(S0SPDR);
}

//---------------------------------------
//	函数名称：SPI_read
//	函数功能：读752寄存器的值
//	入口参数：uint8	reg：752寄存器地址；NCOM=4,5,6,7
//	出口参数：RegValue：返回寄存器的值
//---------------------------------------
uint8	SPI_read(uint8	reg, uint8	ncom)
{
    uint8 k;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif

    OS_ENTER_CRITICAL();
    if((ncom == DEV_XCOM_MB) || (ncom == DEV_XCOM_SCAMERA))
    {
        IO1CLR |= IS752_CS2;
    }
    else
    {
        OS_EXIT_CRITICAL();
        return 0;
    }
    if(ncom == DEV_XCOM_MB)
        k = CHA;
    else
        k = CHB;
    SPI_send(((reg << 3) + k) | 0x80);
    reg = SPI_send(0);
    IO1SET |= IS752_CS2;
    OS_EXIT_CRITICAL();
    return	reg;
}

//---------------------------------------
//	函数名称：SPI_write－ok
//	函数功能：mcu将一个字节date通过SPI总线发送到752，并发送到外部设备。
//	入口参数：uint8	add：寄存器地址字节
//	出口参数：无
//---------------------------------------
void	SPI_write(uint8	add, uint8	ncom, uint8	date)
{
    uint8 k;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif
    if ((ncom != DEV_XCOM_MB) && (ncom != DEV_XCOM_SCAMERA)) return;
    OS_ENTER_CRITICAL();

    if((ncom == DEV_XCOM_MB) || (ncom == DEV_XCOM_SCAMERA))
    {
        IO1CLR |= IS752_CS2;
    }
    if(ncom == DEV_XCOM_MB)
        k = CHA;
    else
        k = CHB;
    SPI_send((add << 3) + k);
    SPI_send(date);
    IO1SET |= IS752_CS2;
    OS_EXIT_CRITICAL();
}


//---------------------------------------
//	函数名称：SPI_writeData－ok
//	函数功能：mcu将多个字节通过SPI总线发送到752，并发送到外部设备。
//	入口参数：
//							uint8	channel：串口号，＝1-8
//							unsigned int	*Value：需要发送数据指针首地址
//							uint8	num：需要发送数据字节数
//	出口参数：无
//---------------------------------------
void	SPI_writeData(uint8	*Value, uint8	num, uint8	ncom)
{
    uint8 k;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif

    OS_ENTER_CRITICAL();

    if((ncom == DEV_XCOM_MB) || (ncom == DEV_XCOM_SCAMERA))
    {
        IO1CLR |= IS752_CS2;
    }
    if(ncom == DEV_XCOM_MB)
        k = CHA;
    else
        k = CHB;
    SPI_send((THR << 3) + k);
    while(num > 0)
    {
        SPI_send(*Value);
        Value++;
        num--;
    }
    IO1SET |= IS752_CS2;
    OS_EXIT_CRITICAL();
}
//---------------------------------------
//	函数名称：
//	函数功能：读串口缓冲区数据,长度小于32
//	入口参数: buf缓冲区指针
//			  ncom串口号1-8
//	出口参数：数据长度
//---------------------------------------

uint8 ReadXuartBuff(uint8 *buf, uint8 ncom)
{
    uint8	tmp, k, len;
    tmp = SPI_read(IIR, ncom) & 0x0f;
    if( tmp == 0x0C) 	// check interrupt status register
    {
        len = SPI_read(RXLVL, ncom); //读FIFO中数据字节长度
        if (len > 64) len = 64;
        for(k = 0; k < len; k++)
        {
            buf[k] = SPI_read(RHR, ncom);	//读出FIFO中数据存入a[]中
        }
        return len;
    }
    else
        return 0;
}
//	函数名称：set_UART_bps(uint8 no)
//	函数功能：设置串口的波特率
//入口参数:no=4/5/6/7
//	出口参数：无
//---------------------------------------
//	函数名称：init_SC16IS752
//	函数功能：752寄存器初始化
//	入口参数：无
//	出口参数：无
//---------------------------------------
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void IS752_Scan(void)
{
    uint8 tmp;
    tmp = SPI_read(MCR, DEV_XCOM_MB);
    if (tmp != 0x02) goto rest1;
    tmp = SPI_read(EFCR, DEV_XCOM_MB);
    if (tmp != 0x31) goto rest1;
    //tmp=SPI_read(FCR,DEV_XCOM_MB);
    //if (tmp!=0xc1) goto rest1;
    tmp = SPI_read(IER, DEV_XCOM_MB);
    if (tmp != 0x01) goto rest1;
    ///////////////////////////////////
    tmp = SPI_read(MCR, DEV_XCOM_SCAMERA);
    if (tmp != 0x02) goto rest2;
    tmp = SPI_read(EFCR, DEV_XCOM_SCAMERA);
    if (tmp != 0x31) goto rest2;
    //tmp=SPI_read(FCR,DEV_XCOM_SCAMERA);
    //if (tmp!=0xc1) goto rest2;
    tmp = SPI_read(IER, DEV_XCOM_SCAMERA);
    if (tmp != 0x01) goto rest2;
    return;
rest1:
    Xcom_set(DEV_XCOM_MB, 9600, DEV_PROTOCOL_ZYCOM, 0x03);
    return;
rest2:
    Xcom_set(DEV_XCOM_SCAMERA, XCOM_PORT[DEV_XCOM_SCAMERA].BPS, DEV_PROTOCOL_NONE, 0x03);
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
void set_IS752_bps(uint8 no, uint32 Baudrate, uint8 LineContrl)
{
    uint16 tmp;
    tmp = Fxcom / Baudrate / 16;

    if ((no == DEV_XCOM_MB) || (no == DEV_XCOM_SCAMERA))
    {
        SPI_write(LCR, no, 0X80);		//波特率的除数锁存使能
        SPI_write(DLL, no, tmp % 256);
        SPI_write(DLH, no, tmp / 256);		//DLH的初值为0x0
        SPI_write(LCR, no, 0XBF);		//使能串口增强功能
        SPI_write(EFR, no, 0X10);		//使能增强型功能IER[7:4], FCR[5:4]和MCR[7:5]使其可被修改
        SPI_write(LCR, no, LineContrl & 0x7f); //0X03);		//8 data bit,1 stop bit, no 校验
        SPI_write(MCR, no, 0X02);		//开去自动485方向识别功能
        SPI_write(EFCR, no, 0X31);		//开去自动485方向识别功能
        SPI_write(FCR, no, 0X87);		//清空，复位接收发送的FIFO,FIFO触发56个字符，使能FIFO
        SPI_write(IER, no, 0X01);		//使IRQ产生中断信号
    }
}
//---------------------------------------
//	函数名称：init_SC16IS752
//	函数功能：752寄存器初始化
//	入口参数：无
//	出口参数：无
//---------------------------------------
void init_SC16IS752(void)
{

}



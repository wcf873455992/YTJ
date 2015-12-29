#ifndef _SC16IS752
#define  _SC16IS752
//-------------------------
//General register set
//-------------------------
#define Fxcom		1843200       //  1.8432MHZ

#define RHR 0x00			//接收保存寄存器
#define THR 0x00			//发送保存寄存器

#define IER 0x01			//中断使能寄存器

#define FCR 0x02			//FIFO控制寄存器
#define IIR 0x02			//中断识别寄存器

#define LCR 0x03			//线控制寄存器
#define MCR 0x04			//Modem控制寄存器
#define LSR 0x05			//线状态寄存器

#define MSR 0x06			//Modem状态寄存器
#define SPR 0x07			//暂存寄存器
#define TCR 0x06			//发送控制寄存器
#define TLR 0x07			//触发点寄存器

#define TXLVL 0x08		//发送FIFO电平寄存器
#define RXLVL 0x09		//接收FIFO电平寄存器

#define IODir 0x0A		//I/O方向寄存器
#define IOState 0x0B	//I/O状态寄存器
#define IOIntEna 0x0C //I/O中断使能寄存器
#define IOControl 0x0E //I/O脚控制寄存器

#define EFCR 0x0F			//额外特性寄存器

//-------------------------
//Special register set
//-------------------------
#define DLL 0x00			//除数寄存器LSB
#define DLH 0x01			//除数寄存器MSB

//-------------------------
//Enhanced register set
//-------------------------
#define EFR 0x02			//增强型特性寄存器
#define Xon1 0x04			//Xon1字
#define Xon2 0x05			//Xon2字
#define Xoff1 0x06		//Xoff1字
#define Xoff2 0x07		//Xoff1字

//-------------------------
//Function:
//-------------------------
#define SC16IS752 0x9A	//SC16IS752地址设置

#define CHA 0x00		//串口0
#define CHB 0x02		//串口1

typedef struct
{
    uint8 BAND_H;
    uint8 BAND_L;
} UART_BAN;
#define	COM0	1
#define	COM1	2
#define	COM2	3
#define	COM3	4
#define	COM4	5
#define	COM5	6
#define	COM6	7
#define	COM7	8

void  SPI_Init(void);
uint8    SPI_send(uint8 data);
uint8	SPI_read(uint8	reg, uint8	ncom);
void	SPI_write(uint8	add, uint8	ncom, uint8	date);
void	SPI_writeData(uint8	*Value, uint8	num, uint8	ncom);
uint8 ReadXuartBuff(uint8 *buf, uint8 ncom);
void set_IS752_bps(uint8 no, uint32 Baudrate, uint8 LineContrl);
void init_SC16IS752(void);
void IS752_Scan(void);
//////////////////////////////
uint16    SPI_send1(uint16 data);
#endif

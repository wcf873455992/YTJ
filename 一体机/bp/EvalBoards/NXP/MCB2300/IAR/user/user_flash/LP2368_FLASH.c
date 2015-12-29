/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			main.c
** Last modified Date:  2004-09-16
** Last Version:		1.0
** Descriptions:		The main() function example template
**
**------------------------------------------------------------------------------------------------------
** Created by:			Chenmingji
** Created date:		2004-09-16
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			Chenxibing
** Modified date:		2005-02-16
** Version:
** Descriptions:
**
*******************************************************************************************************
*/
#include  "includes.h"

//#define BEEP    (1 << 7)


/* 定义函数指针 */
#include "LP2368_flash.h"
void (*IAP_Entry)(uint32 param_tab[], uint32 result_tab[]);

uint32  paramin[8];                         // IAP入口参数缓冲区
uint32  paramout[8];                        // IAP出口参数缓冲区


void InitIAP( void )
{
    IAP_Entry = (void(*)())IAP_ENTER_ADRR;           // 初始化函数指针IAP_Entry

    //  ReadParID();                                    // 读器件ID
    // BootCodeID();                                   // 读Boot版本号

}


/*
*********************************************************************************************************
** 函数名称：SelSector()
** 函数功能：IAP操作扇区选择，命令代码50。
** 入口参数：sec1        起始扇区
**           sec2        终止扇区
** 出口参数：IAP操作状态码
**           IAP返回值(paramout缓冲区)
*******************************************************************************************************
*/
uint32  SelSector(uint8 sec1, uint8 sec2)
{
    paramin[0] = IAP_SELECTOR;               // 设置命令字
    paramin[1] = sec1;                       // 设置参数
    paramin[2] = sec2;
    (*IAP_Entry)(paramin, paramout);         // 调用IAP服务程序

    return(paramout[0]);                     // 返回状态码
}

/*
*******************************************************************************************************
** 函数名称：RamToFlash()
** 函数功能：复制RAM的数据到FLASH，命令代码51。
** 入口参数：dst        目标地址，即FLASH起始地址。以512字节为分界
**           src        源地址，即RAM地址。地址必须字对齐
**           no         复制字节个数，为512/1024/4096/8192
** 出口参数：IAP操作状态码
**           IAP返回值(paramout缓冲区)
*******************************************************************************************************
*/
uint32  RamToFlash(uint32 dst, uint32 src, uint32 no)
{
    paramin[0] = IAP_RAMTOFLASH;             // 设置命令字
    paramin[1] = dst;                        // 设置参数
    paramin[2] = src;
    paramin[3] = no;
    paramin[4] = IAP_FCCLK;
    (*IAP_Entry)(paramin, paramout);         // 调用IAP服务程序

    return(paramout[0]);                     // 返回状态码
}

/*
*******************************************************************************************************
** 函数名称：EraseSector()
** 函数功能：扇区擦除，命令代码52。
** 入口参数：sec1       起始扇区
**           sec2       终止扇区
** 出口参数：IAP操作状态码
**           IAP返回值(paramout缓冲区)
*******************************************************************************************************
*/
uint32  EraseSector(uint8 sec1, uint8 sec2)
{
    paramin[0] = IAP_ERASESECTOR;            // 设置命令字
    paramin[1] = sec1;                       // 设置参数
    paramin[2] = sec2;
    paramin[3] = IAP_FCCLK;
    (*IAP_Entry)(paramin, paramout);         // 调用IAP服务程序

    return(paramout[0]);                     // 返回状态码
}

/*
*******************************************************************************************************
** 函数名称：BlankCHK()
** 函数功能：扇区查空，命令代码53。
** 入口参数：sec1       起始扇区
**           sec2       终止扇区
** 出口参数：IAP操作状态码
**           IAP返回值(paramout缓冲区)
*******************************************************************************************************
*/
uint32  BlankCHK(uint8 sec1, uint8 sec2)
{
    paramin[0] = IAP_BLANKCHK;               // 设置命令字
    paramin[1] = sec1;                       // 设置参数
    paramin[2] = sec2;
    (*IAP_Entry)(paramin, paramout);         // 调用IAP服务程序

    return(paramout[0]);                     // 返回状态码
}

/*
*******************************************************************************************************
** 函数名称：ReadParID()
** 函数功能：扇区查空，命令代码53。
** 入口参数：无
** 出口参数：IAP操作状态码
**           IAP返回值(paramout缓冲区)
*******************************************************************************************************
*/
uint32  ReadParID(void)
{
    paramin[0] = IAP_READPARTID;             // 设置命令字
    (*IAP_Entry)(paramin, paramout);         // 调用IAP服务程序

    return(paramout[0]);                     // 返回状态码
}

/*
*******************************************************************************************************
** 函数名称：BootCodeID()
** 函数功能：读取boot代码版本号，命令代码55。
** 入口参数：无
** 出口参数：IAP操作状态码
**           IAP返回值(paramout缓冲区)
*******************************************************************************************************
*/
uint32 BootCodeID(void)
{
    paramin[0] = IAP_BOOTCODEID;             // 设置命令字
    (*IAP_Entry)(paramin, paramout);         // 调用IAP服务程序

    return(paramout[0]);                     // 返回状态码
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
BYTE *Get_ClassInfo_Table(uint8 id)
{
    return (BYTE *)(ARM_TABLE_ADDRESS + id * 256);
}

/*
*******************************************************************************************************
** 函数名称：Compare()
** 函数功能：校验数据，命令代码56。
** 入口参数：dst        目标地址，即RAM/FLASH起始地址。地址必须字对齐
**           src        源地址，即FLASH/RAM地址。地址必须字对齐
**           no         复制字节个数，必须能被4整除
** 出口参数：IAP操作状态码
**           IAP返回值(paramout缓冲区)
*******************************************************************************************************
*/
uint32 Compare(uint32 dst, uint32 src, uint32 no)
{
    paramin[0] = IAP_COMPARE;                // 设置命令字
    paramin[1] = dst;                        // 设置参数
    paramin[2] = src;
    paramin[3] = no;
    (*IAP_Entry)(paramin, paramout);         // 调用IAP服务程序

    return(paramout[0]);                     // 返回状态码
}

void Test_Flash(void)
{
    BYTE buf[256];
    uint32 resualt;
    BYTE *p;
    InitIAP();
    resualt = ReadParID();                                  // 读器件ID
    resualt = BootCodeID();
    // 读Boot版本号

    resualt = SelSector(13, 13);                            // 选择扇区1
    resualt = EraseSector(13, 13);                          // 擦除扇区1
    p = (BYTE *)0x30000;
    memcpy(buf, p, 256);
    //  resualt= BlankCHK(13,13) ;      // 查空扇区1
    memset(buf, 0xaa, 256);
    resualt = SelSector(13, 13);                            // 选择扇区1
    resualt = RamToFlash(0x30000, (uint32)&buf[0], 256);		// 写数据到扇区1
    resualt = Compare(0x30000, (uint32)&buf[0], 256);
    memcpy(buf, p, 256);
    //	resualt=SelSector(13,13);                               // 选择扇区1
    //  resualt=EraseSector(13,13);                             // 擦除扇区1
    resualt = resualt;
}

/*
*******************************************************************************************************
** 函数名称：main()
** 函数功能：IAP函数调用，测试
*******************************************************************************************************
*/
/*
int  main(void)
{
    uint8 err;
    uint32 i;

    PINSEL0 = 0x00000000;			// 管脚连接GPIO
    IO0DIR = BEEP;
    IO0SET = BEEP;

    IAP_Entry = (void(*)())IAP_ENTER_ADR;           // 初始化函数指针IAP_Entry

    ReadParID();                                    // 读器件ID
    BootCodeID();                                   // 读Boot版本号

    SelSector(1,1);                                 // 选择扇区1
    EraseSector(1,1);                               // 擦除扇区1
    BlankCHK(1,1) ;                                 // 查空扇区1

    SelSector(1,1);                                 // 选择扇区1
    RamToFlash(0x00001000, 0x40001000, 256);        // 写数据到扇区1
    err = Compare(0x00001000, 0x40001000, 256);     // 比较数据

    if(err == CMD_SUCCESS)
    {	// 成功，蜂鸣一声 //
        IO0CLR = BEEP;
        for (i=0; i<0x3FFFFF; i++);
        IO0SET = BEEP;
    }
    else
    {	// 失败，一直蜂鸣 //
        IO0CLR = BEEP;
    }

    while(1);
    return(0);
}
*/

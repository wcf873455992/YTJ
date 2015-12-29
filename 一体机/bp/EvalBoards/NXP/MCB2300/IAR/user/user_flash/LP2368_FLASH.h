#ifndef _LP2368_FLASH_H_
#define _LP2368_FLASH_H_

//参照 LPC23xx_us_en.pdf 第27章 table 483 定义使用flash情况
// 0 - 128K 放bootloader	0-10 扇区 (0x0000 0000 -0x0001FFFF)
// 128K - 496K 放主程序	11-25扇区 (0x0002 0000 -0x0007BFFF)
// 最后4K放配置信息		26-27扇区 (0x0007 C000 -0x0007DFFF)


#define ARM_TABLE_ADDRESS			0x30000
#define ARM_TABLE_SECTOR_START	13
#define ARM_TABLE_SECTOR_END		14



#define IAP_ENTER_ADRR   0x7FFFFFF1  // IAP入口地址定义

/* 定义IAP命令字 */
//   命令           参数
#define     IAP_SELECTOR        50  // 选择扇区     【起始扇区号、结束扇区号】
#define     IAP_RAMTOFLASH      51  // 拷贝数据     【FLASH目标地址、RAM源地址、写入字节数、系统时钟频率】
#define     IAP_ERASESECTOR     52  // 擦除扇区     【起始扇区号、结束扇区号、系统时钟频率】
#define     IAP_BLANKCHK        53  // 查空扇区     【起始扇区号、结束扇区号】
#define     IAP_READPARTID      54  // 读器件ID     【无】
#define     IAP_BOOTCODEID      55  // 读Boot版本号 【无】
#define     IAP_COMPARE         56  // 比较命令     【Flash起始地址、RAM起始地址、需要比较的字节数】

/* 定义IAP返回状态字 */
#define     CMD_SUCCESS          0
#define     INVALID_COMMAND      1
#define     SRC_ADDR_ERROR       2
#define     DST_ADDR_ERROR       3
#define     SRC_ADDR_NOT_MAPPED  4
#define     DST_ADDR_NOT_MAPPED  5
#define     COUNT_ERROR          6
#define     INVALID_SECTOR       7
#define     SECTOR_NOT_BLANK     8
#define     SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION 9
#define     COMPARE_ERROR        10
#define     BUSY                 11


/* 定义CCLK值大小，单位为KHz */
#define  IAP_FCCLK           (Fcclk/1000)//40000// Fcclk
void InitIAP( void );

uint32  SelSector(uint8 sec1, uint8 sec2);
uint32  RamToFlash(uint32 dst, uint32 src, uint32 no);
uint32  EraseSector(uint8 sec1, uint8 sec2);
uint32  BlankCHK(uint8 sec1, uint8 sec2);
uint32  Compare(uint32 dst, uint32 src, uint32 no);
uint32  ReadParID(void);
uint32  BootCodeID(void);
BYTE *Get_ClassInfo_Table(uint8 id);
void Test_Flash(void);
#endif//_LP2368_FLASH_H_

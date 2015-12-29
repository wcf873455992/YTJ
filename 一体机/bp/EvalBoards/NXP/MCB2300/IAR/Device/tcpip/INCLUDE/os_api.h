#ifndef	__os_api_h
#define	__os_api_h

//*------------------------------- 宏、常量、变量、类型及结构体定义 ---------------------------------
typedef	OS_EVENT	*PHANDLER;

//* 创建一个新的信号量
OS_EVENT *OSAPIBlockNew(UBYTE ubPIP);
//* 退出阻塞资源
#define		OSAPIBlockExit(hBlock)			OSMutexPost(hBlock)
BYTE OSAPIBlockEnter(PHANDLER hBlock, UWORD uwWaitMS);
#endif

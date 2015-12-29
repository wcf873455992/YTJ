#include	"includes.h"
//*------------------------------------------------------------------------------------------------
//* 函数名称 : OSAPIBlockNew
//* 功能描述 : 建立一个新的阻塞信号
//* 入口参数 : <ubPIP>[in] 指定在出现优先级反转时（高优先级的任务因为低优先级的任务占用同一个阻塞量而不得不
//*			 :             暂时中断正常运行以等待该阻塞量被释放从而造成事实上的优先级降低的问题），正在占用
//*			 :             该阻塞量的任务使用的较高优先级以便能够快速完成访问，释放该阻塞量
//* 出口参数 : 建立成功返回阻塞信号句柄，否则为NULL
//*------------------------------------------------------------------------------------------------
OS_EVENT *OSAPIBlockNew(UBYTE ubPIP)
{
    UBYTE		__ubErr;

    return (OS_EVENT *)OSMutexCreate(ubPIP, &__ubErr);
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : OSAPIBlockEnter
//* 功能描述 : 进入阻塞资源，如果该资源仍然被其它任务占用，则会阻塞当前任务进程，直至指定的时间溢出或者资源
//*          : 可用
//* 入口参数 :   <hBlock>[in] 指定要使用的阻塞信号量
//*			 : <uwWaitMS>[in] 指定被阻塞的最长时间，单位为毫秒
//* 出口参数 : - OS_NO_ERR         : 信号量可用
//*			 : - OS_TIMEOUT        : 时间溢出，信号量不可用
//*			 : - OS_ERR_EVENT_TYPE : hSem参数携带的事件类型并不是信号量而是其它类型
//*			 : - OS_ERR_PEND_ISR   : 不能在ISR中调用该函数
//*			 : - OS_ERR_PEVENT_NULL: hSem是一个空指针
//*------------------------------------------------------------------------------------------------
UBYTE OSAPIBlockEnter(OS_EVENT *hBlock, UWORD uwWaitMS)
{
    UWORD	__uwDelayTicks;
    UBYTE	__ubRtnVal;

    if(uwWaitMS != 0)
    {
        __uwDelayTicks = (uwWaitMS * OS_TICKS_PER_SEC) / 1000;
        if(__uwDelayTicks < 1)
            __uwDelayTicks = 1;
        else if(__uwDelayTicks >= 65535)
            __uwDelayTicks = 65535;
        else;
    }
    else
        __uwDelayTicks = 0;

    OSMutexPend(hBlock, __uwDelayTicks, &__ubRtnVal);

    return __ubRtnVal;
}

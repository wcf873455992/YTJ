#include "includes.h"
/////////////////////////////////
uint8 __mem256[20][256];
uint8 __mem512[10][600];
OS_MEM *__pm256, *__pm512;
/*********************************************
**
**
**
**********************************************/
void User_mem_init(void)
{
    uint8 err;
    __pm256 = OSMemCreate(__mem256, 20, 256, &err);
    __pm512 = OSMemCreate(__mem512, 10, 600, &err);
}
/*********************************************
**
**
**
**********************************************/
void *User_mem_allco(uint16  lg)
{
    uint8 err = OS_MEM_NO_FREE_BLKS;
    void *pmem;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif
    OS_ENTER_CRITICAL();
    if (lg <= 256)
    {
        pmem = (void *)OSMemGet(__pm256, &err);
        if (pmem == NULL)
            pmem = (void *)OSMemGet(__pm512, &err);
    }
    else if (lg <= 600)
        pmem = (void *)OSMemGet(__pm512, &err);
    else
        pmem = NULL;
    OS_EXIT_CRITICAL();
    return pmem;


}
/*********************************************
**
**
**
**********************************************/
BYTE User_mem_free(void *memadd)
{
    uint8 err;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif
    if (memadd == NULL)  return 1;
    OS_ENTER_CRITICAL();
    if (((uint32)memadd >= (uint32 )(__pm256->OSMemAddr)) && ((uint32)memadd < ((uint32)__pm256->OSMemAddr + __pm256->OSMemBlkSize * __pm256->OSMemNBlks)))
        err = OSMemPut(__pm256, memadd);
    else if (((uint32)memadd >= (uint32 )(__pm512->OSMemAddr)) && ((uint32)memadd < ((uint32)__pm512->OSMemAddr + __pm512->OSMemBlkSize * __pm512->OSMemNBlks)))
        err = OSMemPut(__pm512, memadd);
    OS_EXIT_CRITICAL();
    return err;
}



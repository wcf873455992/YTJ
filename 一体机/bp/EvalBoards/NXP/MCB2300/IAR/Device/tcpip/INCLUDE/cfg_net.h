#ifndef __CON_F_H
#define __CON_F_H
#define IN_RAM
#define EX_RAM
#define IN_CODE
#define USE_DATA

struct _pkst
{
    struct _pkst  *STPTR;
    unsigned int  length;
    unsigned char *DAPTR;
};//typedef

struct a_t
{
    unsigned char TTL;
    unsigned char IP_NUM[4];
    unsigned char MAC_NUM[6];
};

#define MAX_ARP_TERM	30
#define	MAX_TCP_DATA	1460
#define MAX_TCP_LINKS   2
#define keilreentrant


#define Little_End

//#include "..\include\hardware.h"
#include "Lpc23xx_add_r.h"
#include "..\include\ethernet.h"
#include "..\include\ARP.h"

//#include "mcu_hardware.h"
#include "..\include\ip.h"
#include "..\include\icmp.h"
#include "..\include\udp.h"

#endif

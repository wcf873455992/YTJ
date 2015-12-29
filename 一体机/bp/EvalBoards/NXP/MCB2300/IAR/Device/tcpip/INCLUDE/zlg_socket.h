#ifdef SOCKET_GLOBALS
#define SOCKET_EXT
#else
#define SOCKET_EXT extern
#endif

#define PF_INET		0
#define AF_INET		1

#define SOCK_STREAM	0
#define SOCK_DGRAM	1

#define TCP_PROTOCOL	0
#define UDP_PROTOCOL	1

#define INVALID_SOCKET	NULL

#ifndef	MAX_TCP_LINKS
#define MAX_TCP_LINKS	0
#endif

#ifndef	MAX_UDP_LINKS
#define MAX_UDP_LINKS	0
#endif

#define MAX_SOCKETS	MAX_TCP_LINKS+MAX_UDP_LINKS

typedef struct
{
    uint8	  State;
    uint16 protocol;
    uint16 af;
    uint16 type;
    uint8	  My_Ip[4];
    uint16 My_Port;
    uint16 Max_Queue;
    uint8	  queue[MAX_TCP_LINKS];
    uint8	  Udp_Queue;
} SOCKET;
struct sockaddr
{
    uint16 sin_family;
    uint8	  sin_addr[4];
    uint16 sin_port;
};

SOCKET_EXT SOCKET Zlg_Socket[MAX_SOCKETS];
SOCKET_EXT uint8 close(uint8 num);
SOCKET_EXT uint8 connect(SOCKET *s, struct sockaddr *addr, uint16 addrlen); // USE_XDATA  keilreentrant; // ;
SOCKET_EXT SOCKET *socket( uint16 af, uint16 type, uint16 protocol);
SOCKET_EXT int bind( SOCKET *s, struct sockaddr *name, uint16 namelen);
SOCKET_EXT int listen( SOCKET *s, uint16 backlog );
SOCKET_EXT uint8 accept( SOCKET *s, struct sockaddr *addr, int *addrlen );
SOCKET_EXT uint16 recv( uint8 num, uint8 *buf, uint16 len, int flags );
SOCKET_EXT uint16 send( uint8 num, uint8 *buf, uint16 len, int flags );
SOCKET_EXT uint16 recvfrom( SOCKET s, uint8 *buf, uint16 len, int flags, struct sockaddr *from, uint16 *fromlen );
SOCKET_EXT uint16 sendto( SOCKET s, uint8 *buf, uint16 len, int flags, struct sockaddr *to, uint16 tolen );
SOCKET_EXT int closesocket( SOCKET s );
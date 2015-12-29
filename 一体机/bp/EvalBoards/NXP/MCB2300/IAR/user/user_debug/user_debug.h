#ifndef __USER_DEBUG_H
#define __USER_DEBUG_H

#define DBG_TEST		1   //=0ÀÕºÏÕ£÷πÕ¯¬Á“Ù∆µ
void IP_Debug_init(void);
void uart_Debug_init(void);
void   UART_printf(char   *fmt, ...);
void IP_printf(char *fmt, ...);
void IP_Debug_init(void);
void ID_Change(char *buf, char *fmt, ...);
void DBG_Log_write(char *pbuf);
uint16 DBG_Str_printf(char *pbuf, char *fmt, ...);
uint16 DBG_HexToChar(uint8 *pdata, uint8 *sbuf, uint16 len);
void Log_Close(char *sbuf);
char *Log_Create(uint16 *strlen);
void Print_MSG_info(void *pdata);
void DBG_XCOM_Printf(uint8 port, uint8 *pbuf);

#endif

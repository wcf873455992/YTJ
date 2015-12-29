#ifndef __UART_H
#define __UART_H

void UART_init(void);
void UART_Send_Data(uint8	PortNum, uint8 *pdata, uint8 len);
void UART_set_bps(uint8	PortNum, uint32	Baudrate, uint8  LineContrl);
#endif



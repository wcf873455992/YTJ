#include  "includes.h"
#include "string.h"
#include "Uart.h"
//���ڳ�ʼ����PortNumΪ0����1,2,3
extern struct COMM_XCOM XCOM_PORT[DEV_XCOM_MAX_SIZE];
extern	uint8	gTestUartFlag;
//////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void UART_set_bps(uint8	PortNum, uint32	Baudrate, uint8  LineContrl)
{
    switch(PortNum)
    {
    case DEV_XCOM_DW://��λ�崮��
        U0LCR = 0x83;// DLAB = 1�������ò�����
        U0DLL = ((Fpclk / 16) / Baudrate) % 256;
        U0DLM = ((Fpclk / 16) / Baudrate) / 256;

        U0LCR = LineContrl & 0x7f; //0x03;
        U0FCR = 0xc7; // ʹ��FIFO������λR,T FIFO��R����8 Byte
        U0IER = 0x01;  // ����RBR�жϺͳ�ʱ�����ж�
        break;
    case DEV_XCOM_TYJ ://ͶӰ������
        //PINSEL0=(PINSEL0&(~(0X0F<<16)))|(0X05<<16);//����P0.8,P0.9Ϊ����1
        U1LCR = 0x83;// DLAB = 1�������ò�����
        U1DLL = ((Fpclk / 16) / Baudrate) % 256;
        U1DLM = ((Fpclk / 16) / Baudrate) / 256;
        U1LCR = LineContrl & 0x7f; //0x03;
        U1FCR = 0xc7; // ʹ��FIFO������λR,T FIFO��R����8 Byte
        U1IER = 0x01;  // ����RBR�жϺͳ�ʱ�����ж�
        break;
    case DEV_XCOM_SOUND://��Ƶ������
        U2LCR = 0x83;// DLAB = 1�������ò�����
        U2DLL = ((Fpclk / 16) / Baudrate) % 256;
        U2DLM = ((Fpclk / 16) / Baudrate) / 256;
        U2LCR = LineContrl & 0x7f; //0x03;
        U2FCR = 0xc7; // ʹ��FIFO������λR,T FIFO��R����8 Byte
        U2IER = 0x01;  // ����RBR�жϺͳ�ʱ�����ж�
        break;
    case DEV_XCOM_TB://ͬ���崮��
        U3LCR = 0x83;// DLAB = 1�������ò�����
        U3DLL = ((Fpclk / 16) / Baudrate) % 256;
        U3DLM = ((Fpclk / 16) / Baudrate) / 256;
        U3LCR = LineContrl & 0x7f; // 0x03;
        U3FCR = 0xc7; // ʹ��FIFO������λR,T FIFO��R����8 Byte
        U3IER = 0x01;  // ����RBR�жϺͳ�ʱ�����ж�
        break;
    case DEV_XCOM_MB://��崮��
        set_IS752_bps(DEV_XCOM_MB, Baudrate, LineContrl & 0x7f);
        break;
    case DEV_XCOM_SCAMERA://������ƴ���
        set_IS752_bps( DEV_XCOM_SCAMERA, Baudrate, LineContrl & 0x7f);
        break;
    default:
        break;

    };



}
//////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
//����һ���ֽ�
void UART_PutChar ( uint8	PortNum, uint8 data)
{
    if(PortNum == DEV_XCOM_DW)
    {
        U0THR = data;			    // ��������
        while( (U0LSR & 0x40) == 0 );	 // �ȴ����ݷ������
    }
    else if(PortNum == DEV_XCOM_TYJ)
    {
        U1THR = data;			    // ��������
        while( (U1LSR & 0x40) == 0 );	 // �ȴ����ݷ������
    }
    else		if(PortNum == DEV_XCOM_SOUND)
    {
        U2THR = data;			    // ��������
        while( (U2LSR & 0x40) == 0 );	 // �ȴ����ݷ������
    }
    else if(PortNum == DEV_XCOM_TB)
    {
        U3THR = data;			    // ��������
        while( (U3LSR & 0x40) == 0 );	 // �ȴ����ݷ������
    }
}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ������ַ���
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////

void  UART_PutString(uint8	PortNum, uint8 *str)
{
    while(*str != '\0')
    {
        UART_PutChar(PortNum, *str++);
    }
}

//---------------------------------------
//	�������ƣ�void UART_Send_Data(uint8	PortNum,uint8 *pdata,uint8 len)
//	�������ܣ���������
//	��ڲ�����				uint8 	PortNum�����ںţ���1-8
//							unsigned int	*pdata����Ҫ��������ָ���׵�ַ
//							uint8 	len����Ҫ���������ֽ���
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void UART_Send_Data(uint8	PortNum, uint8 *pdata, uint8 len)
{
    uint8 i = 0, a, b, j, err;


    a = len / 16;
    b = len % 16;

    switch(PortNum)
    {
    case DEV_XCOM_DW:
        //����0��������
        for (i = 0; i < a; i++)
        {
            for (j = 0; j < 16; j++)
            {
                U0THR = pdata[j + i * 16];
            }
            U0IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_DW].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_DW].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_DW, XCOM_PORT[DEV_XCOM_DW].BPS,
                             XCOM_PORT[DEV_XCOM_DW].PROTOCOL, XCOM_PORT[DEV_XCOM_DW].linecontrl);
            }

        }
        if (b > 0)
        {

            for (j = 0; j < b; j++)
                U0THR = pdata[j + a * 16];
            U0IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_DW].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_DW].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_DW, XCOM_PORT[DEV_XCOM_DW].BPS, XCOM_PORT[DEV_XCOM_DW].PROTOCOL, XCOM_PORT[DEV_XCOM_DW].linecontrl);

            }
        }
        break;
    case DEV_XCOM_TYJ:
        //����1��������
        for (i = 0; i < a; i++)
        {
            for (j = 0; j < 16; j++)
                U1THR = pdata[j + i * 16];
            U1IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_TYJ].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_TYJ].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_TYJ, XCOM_PORT[DEV_XCOM_TYJ].BPS, XCOM_PORT[DEV_XCOM_TYJ].PROTOCOL, XCOM_PORT[DEV_XCOM_TYJ].linecontrl);
            }

        }

        if (b > 0)
        {

            for (j = 0; j < b; j++)
                U1THR = pdata[j + a * 16];
            U1IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_TYJ].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_TYJ].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_TYJ, XCOM_PORT[DEV_XCOM_TYJ].BPS, XCOM_PORT[DEV_XCOM_TYJ].PROTOCOL, XCOM_PORT[DEV_XCOM_TYJ].linecontrl);
            }
        }

        break;

    case DEV_XCOM_SOUND:
        //����2��������
        for (i = 0; i < a; i++)
        {

            for (j = 0; j < 16; j++)
                U2THR = pdata[j + i * 16];
            U2IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_SOUND].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_SOUND].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_SOUND, XCOM_PORT[DEV_XCOM_SOUND].BPS, XCOM_PORT[DEV_XCOM_SOUND].PROTOCOL, XCOM_PORT[DEV_XCOM_SOUND].linecontrl);

            }

        }

        if (b > 0)
        {

            for (j = 0; j < b; j++)
                U2THR = pdata[j + a * 16];
            U2IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_SOUND].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_SOUND].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_SOUND, XCOM_PORT[DEV_XCOM_SOUND].BPS, XCOM_PORT[DEV_XCOM_SOUND].PROTOCOL, XCOM_PORT[DEV_XCOM_SOUND].linecontrl);

            }

        }
        break;

    case DEV_XCOM_TB:
        //����3��������
        for (i = 0; i < a; i++)
        {
            for (j = 0; j < 16; j++)
            {
                U3THR = pdata[j + i * 16];
                while( (U3LSR & 0x40) == 0 );	 // �ȴ����ݷ������
            }
            U3IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_TB].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_TB].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_TB, XCOM_PORT[DEV_XCOM_TB].BPS,
                             XCOM_PORT[DEV_XCOM_TB].PROTOCOL, XCOM_PORT[DEV_XCOM_TB].linecontrl);

            }

        }
        if (b > 0)
        {

            for (j = 0; j < b; j++)
            {
                U3THR = pdata[j + a * 16];
                while( (U3LSR & 0x40) == 0 );	 // �ȴ����ݷ������

            }
            U3IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_TB].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_TB].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_TB, XCOM_PORT[DEV_XCOM_TB].BPS, XCOM_PORT[DEV_XCOM_TB].PROTOCOL, XCOM_PORT[DEV_XCOM_TB].linecontrl);
            }
        }
        break;
    case DEV_XCOM_MB:
        if (len > 64) len = 64;
        SPI_writeData(pdata, len, PortNum);
        SPI_write(IER, DEV_XCOM_MB, 0X03);		//�رշ����ж�
        if (XCOM_PORT[DEV_XCOM_MB].UART_send_end != NULL)
        {
            OSSemPend(XCOM_PORT[DEV_XCOM_MB].UART_send_end, UART_SEND_WAIT_TIME, &err);
            if (err != OS_NO_ERR)
                Xcom_set(DEV_XCOM_MB, XCOM_PORT[DEV_XCOM_MB].BPS,
                         XCOM_PORT[DEV_XCOM_MB].PROTOCOL, XCOM_PORT[DEV_XCOM_MB].linecontrl);


        }
        break;
    case DEV_XCOM_SCAMERA:
        if (len > 64) len = 64;

        SPI_writeData(pdata, len, PortNum);
        OSTimeDly(len);
        SPI_write(IER, DEV_XCOM_SCAMERA, 0X03); //�رշ����ж�
        if (XCOM_PORT[DEV_XCOM_SCAMERA].UART_send_end != NULL)
        {
            OSSemPend(XCOM_PORT[DEV_XCOM_SCAMERA].UART_send_end, 500, &err);
            if (err != OS_NO_ERR)
                Xcom_set(DEV_XCOM_SCAMERA, XCOM_PORT[DEV_XCOM_SCAMERA].BPS,
                         XCOM_PORT[DEV_XCOM_SCAMERA].PROTOCOL, XCOM_PORT[DEV_XCOM_SCAMERA].linecontrl);

        }
        break;
    }


}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�����ʣ�µ����ݳ���
////////////////////////////////////////////////////////////////////
uint8 Able_CMD_TB(uint8 *pdata, int lg)
{
    uint8 i, tmp_lg, tmp = 0;
    if (lg == 0) return lg;
    tmp_lg = lg;
    /////////Ѱ��ͬ��ͷ
    for (;;)
    {
        if ((*pdata == ZY_XCOM_FLAG_START) || (*pdata == ZY_XCOM_FLAG_ACK)) break;

        for (i = 0; i < tmp_lg; i++)
            *(pdata + i) = *(pdata + i + 1);
        tmp_lg--;
        tmp++;
        if (tmp_lg == 0) break;

    }
    return lg - tmp;
}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint8 XCOM_DEV_Read(uint8 XCOM_DEV_Chanle, uint8 *pdata, uint8 lg)
{
    uint8 i, tmp;
    i = lg;
    switch(XCOM_DEV_Chanle)
    {
    //CPU_UART0
    case DEV_XCOM_DW:
        if ((U0LSR & 1) == 0)
            tmp = U0RBR;
        for (i = lg; i < 32 && (U0LSR & 1); i++)
            *(pdata + i) = U0RBR;
        break;
    //CPU_UART1
    case DEV_XCOM_TYJ:

        if ((U1LSR & 1) == 0)
            tmp = U1RBR;
        for (i = lg; i < 32 && (U1LSR & 1); i++)
            *(pdata + i) = U1RBR;
        break;
    //CPU_UART2
    case DEV_XCOM_SOUND:

        if ((U2LSR & 1) == 0)
            tmp = U2RBR;
        for (i = lg; i < 32 && (U2LSR & 1); i++)
            *(pdata + i) = U2RBR;
        break;
    //CPU_UART3
    case DEV_XCOM_TB:

        if ((U3LSR & 1) == 0)
            tmp = U3RBR;
        for (i = lg; i < 32 && (U3LSR & 1); i++)
            *(pdata + i) = U3RBR;
        break;
    //
    case DEV_XCOM_MB:
        tmp = SPI_read(RXLVL, DEV_XCOM_MB); //��FIFO�������ֽڳ���
        if (tmp != 0)
        {
            for (i = lg; i < 32 && (tmp != 0); i++)
            {
                *(pdata + i) = SPI_read(RHR, DEV_XCOM_MB);	//����FIFO�����ݴ���a[]��
                tmp--;
            }
        }
        break;
    case DEV_XCOM_SCAMERA:
        tmp = SPI_read(RXLVL, DEV_XCOM_SCAMERA); //��FIFO�������ֽڳ���
        if (tmp != 0)
        {
            for (i = lg; i < 32 && (tmp != 0); i++)
            {
                *(pdata + i) = SPI_read(RHR, DEV_XCOM_SCAMERA);	//����FIFO�����ݴ���a[]��
                tmp--;

            }
        }
        break;
    default:
        return i;
    }
    return i;
}
uint8 sum_check(uint8 *pdata, uint8 lg)
{
    uint8 sum = 0, i;
    for (i = 0; i < lg; i++)
        sum += *(pdata + i);
    return sum;
}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
int  UART_REV(uint8 Xcom)
{
    uint8 sum = 0, i, tmp, *prev;
    struct ZY_XCOM *pable;
    struct COMM_XCOM *pxcom;
    struct DEVICE_STATUS_TYPE *ps;

    if (Xcom > DEV_XCOM_SCAMERA)  return 0; //���ںŴ���

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    prev = (uint8 *)&XCOM_PORT[Xcom].Rev_buf[0];
    pxcom = (struct COMM_XCOM *)&XCOM_PORT[Xcom];

    if (Get_Debug(DEV_DEBUG_XCOM))
    {
        pxcom->Rev_lg = XCOM_DEV_Read(Xcom, prev, pxcom->Rev_lg);
        UART_Write(Xcom, (uint8 *)&pxcom->Rev_buf[0], pxcom->Rev_lg);
        pxcom->Rev_lg = 0;
        return  0;
    }
    if (Xcom == DEV_XCOM_SCAMERA || Xcom == DEV_XCOM_TB) goto clp1;
    if (pxcom->PROTOCOL == DEV_PROTOCOL_ZYCOM)
    {
xcom_loop01:
        pxcom->Rev_lg = XCOM_DEV_Read(Xcom, prev, pxcom->Rev_lg);
        pxcom->Rev_lg = Able_CMD_TB((uint8 *)&pxcom->Rev_buf[0], pxcom->Rev_lg);
        if (pxcom->Rev_lg < 4) return 0; //û�н��յ���С֡
        pable = (struct ZY_XCOM *)&pxcom->Rev_buf[0];
        tmp = (pable->type_lg & 0x0f) + 4;
        if (tmp > pxcom->Rev_lg)
        {
            return 0;
        };//û�н�������֡
        //��������������ʹ��󲻼���У���
        if (Xcom == DEV_XCOM_MB)
            if ((ps->KB_Type == 1) && (pable->flag == ZY_XCOM_FLAG_START) && (pxcom->Rev_buf[3] == 3))
            {
                tmp--;
                goto err1;
            }
        if (sum_check(&pxcom->Rev_buf[0], tmp))
        {
            pxcom->Rev_buf[0] = 0; //
            pxcom->Rev_lg = Able_CMD_TB((uint8 *)&pxcom->Rev_buf[0], pxcom->Rev_lg);
            goto xcom_loop01;
        }
err1:
        //�������ݵ����ն���

        XCOM_Q_Send(Xcom, (uint8 *)&pxcom->Rev_buf[0], tmp);
        //֪ͨ�ȴ�����
        if ((pxcom->flag != NULL) && (pable->flag == ZY_XCOM_FLAG_ACK)
                && (pxcom->Rflag & XCOM_READ_FLAG_WAIT_ACK))
            OSSemPost(pxcom->flag);
        if ((pxcom->Rflag & XCOM_READ_FLAG_WAIT_DATA)
                && (pxcom->pbuf != NULL)
                && (pable->flag == ZY_XCOM_FLAG_START)
                && (pxcom->flag != NULL)
                && (pxcom->RCMDNo == *(prev + sizeof(struct ZY_XCOM))))
        {
            pxcom->Rdata_lg = (pable->type_lg & 0xf) + 4;
            Mem_Copy(pxcom->pbuf, prev, pxcom->Rdata_lg);
            OSSemPost(pxcom->flag);
        }
        //�������ջ�����
        sum = pxcom->Rev_lg - tmp;
        for (i = 0; i < sum; i++)
            pxcom->Rev_buf[i] = pxcom->Rev_buf[i + tmp];
        pxcom->Rev_lg = sum;
        if (sum > 4)
            goto xcom_loop01;

    }
    else
    {
clp1:
        pxcom->Rev_lg = XCOM_DEV_Read(Xcom, prev, pxcom->Rev_lg);
        XCOM_Q_Send(Xcom, (uint8 *)&pxcom->Rev_buf[0], pxcom->Rev_lg);
        if (pxcom->flag != NULL)
            OSSemPost(pxcom->flag);
        pxcom->Rev_lg = 0;

    }
    return pxcom->Rev_lg;
}

////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
//uint8	UartSendPOWERFlag=0;
//uint8	UartSendQIUFlag=0;

void   EINT1_Exception(void)//u21,com6,7
{
    uint32 UxIIR;
    UxIIR = SPI_read(IIR, DEV_XCOM_MB) & 0x0f;
Uxint1:
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_MB);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_MB].UART_send_end != NULL))
    {
        SPI_write(IER, DEV_XCOM_MB, 0X01);		//�رշ����ж�
        OSSemPost(XCOM_PORT[DEV_XCOM_MB].UART_send_end);
    }
    UxIIR = SPI_read(IIR, DEV_XCOM_MB) & 0x0f;
    if((UxIIR == 0X04) || (UxIIR == 0x0c) || (UxIIR == 0x02))
        goto Uxint1;
    ////////////////////////////////////////////////

    UxIIR = SPI_read(IIR, DEV_XCOM_SCAMERA) & 0x0f;
Uxint2:
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_SCAMERA);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_SCAMERA].UART_send_end != NULL))
    {
        SPI_write(IER, DEV_XCOM_SCAMERA, 0X01);		//�رշ����ж�
        OSSemPost(XCOM_PORT[DEV_XCOM_SCAMERA].UART_send_end);
    }
    UxIIR = SPI_read(IIR, DEV_XCOM_SCAMERA) & 0x0f;
    if((UxIIR == 0X04) || (UxIIR == 0x0c) || (UxIIR == 0x02))
        goto Uxint2;

    EXTINT = 0x02;				/* ���EINT0�жϱ�־		*/
    if ((EXTINT & 0x02) != 0)    IS752_Scan();

}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ���λ��DEV_XCOM_DW
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void   UART0_Exception(void)
{
    uint32 UxIIR;
    UxIIR = U0IIR & 0x0f;
    //IP_printf("uart 0");
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_DW);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_DW].UART_send_end != NULL))
        OSSemPost(XCOM_PORT[DEV_XCOM_DW].UART_send_end);
    else
        UxIIR = U0RBR;

}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ���չRS232 DEV_XCOM_RS232 ��չ���
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////

void   UART1_Exception(void)
{
    uint32 UxIIR;
    UxIIR = U1IIR & 0x0f;
    //IP_printf("uart 1");
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_TYJ);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_TYJ].UART_send_end != NULL))
        OSSemPost(XCOM_PORT[DEV_XCOM_TYJ].UART_send_end);
    else
        UxIIR = U1RBR;
}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ����
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void   UART2_Exception(void)
{
    uint32 UxIIR;
    UxIIR = U2IIR & 0x0f;
    //IP_printf("uart 2");
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_SOUND);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_SOUND].UART_send_end != NULL))
        OSSemPost(XCOM_PORT[DEV_XCOM_SOUND].UART_send_end);
    else
        UxIIR = U2RBR;
}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�ͬ����
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void   UART3_Exception(void)
{

    uint32 UxIIR;
    UxIIR = U3IIR & 0x0f;
    //IP_printf("uart 3");
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_TB);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_TB].UART_send_end != NULL))
        OSSemPost(XCOM_PORT[DEV_XCOM_TB].UART_send_end);
    else
        UxIIR = U3RBR;
}

////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�ͬ����
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////

void UART_init(void)
{
    EXTMODE = 0X00; //����4���ⲿ�ж�Ϊ��ƽ����
    EXTPOLAR = 0X00; //����4���ⲿ�ж�Ϊ�͵�ƽ����
    EXTINT    = 0x0f;			/* ���EINT�жϱ�־				*/
    SetVICIRQ(VIC_CHANLE_UART0, 3, (uint32)UART0_Exception);
    SetVICIRQ(VIC_CHANLE_UART1, 4, (uint32)UART1_Exception);
    SetVICIRQ(VIC_CHANLE_UART2, 5, (uint32)UART2_Exception);
    SetVICIRQ(VIC_CHANLE_UART3, 6, (uint32)UART3_Exception);
    SetVICIRQ(VIC_CHANLE_EINT1, 7, (uint32)EINT1_Exception); 	//Uart4,5
}



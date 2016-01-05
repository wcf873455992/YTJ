#include "includes.h"
#include "PelcoD.h"


unsigned char pPelcodData[PELCOD_LENGTH];
unsigned char pViscaData[VISCA_LENGTH];



void SetAddress( unsigned char bAddress, unsigned char cProtocol)
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[0] = 0xff;
        pPelcodData[1] = bAddress;
    }
    else
    {
        pViscaData[0] = 0x80;
        pViscaData[0] += bAddress;
    }
}
unsigned char CheckSum1(void)
{
    unsigned int nSum = 0;
    unsigned int i = 1;
    for(i = 1; i < PELCOD_LENGTH - 1; i++)
    {
        nSum += pPelcodData[i];
    }

    pPelcodData[PELCOD_LENGTH - 1] = nSum & 0xff;

    return pPelcodData[PELCOD_LENGTH - 1];
}
void  TurnRightCmd( unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol)
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x02;
        pPelcodData[4] = nSpeed;
        pPelcodData[5] = 0x0;
        pPelcodData[PELCOD_LENGTH - 1] =  CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x06;
        pViscaData[3] = 0x01;
        pViscaData[4] = nSpeed;
        pViscaData[5] = 0x0;
        pViscaData[6] = 0x02;
        pViscaData[7] = 0x03;
        pViscaData[8] = 0xff;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}
void  TurnLeftCmd( unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x04;
        pPelcodData[4] = nSpeed;
        pPelcodData[5] = 0x0;
        pPelcodData[PELCOD_LENGTH - 1] =  CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x06;
        pViscaData[3] = 0x01;
        pViscaData[4] = nSpeed;
        pViscaData[5] = 0x0;
        pViscaData[6] = 0x01;
        pViscaData[7] = 0x03;
        pViscaData[8] = 0xff;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void  TurnUpCmd( unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x08;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = nSpeed;
        pPelcodData[PELCOD_LENGTH - 1] =  CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x06;
        pViscaData[3] = 0x01;
        pViscaData[4] = 0x0;
        pViscaData[5] = nSpeed;
        pViscaData[6] = 0x03;
        pViscaData[7] = 0x01;
        pViscaData[8] = 0xff;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void  TurnDownCmd( unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x10;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = nSpeed;
        pPelcodData[PELCOD_LENGTH - 1] =  CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x06;
        pViscaData[3] = 0x01;
        pViscaData[4] = 0x0;
        pViscaData[5] = nSpeed;
        pViscaData[6] = 0x03;
        pViscaData[7] = 0x02;
        pViscaData[8] = 0xff;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void  ZoomInCmd( unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x20;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = 0x0;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        if(nSpeed > 7 || nSpeed < 2)
        {
            nSpeed = 4;
        }
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x04;
        pViscaData[3] = 0x07;
        pViscaData[4] = 0x20 | nSpeed;
        pViscaData[5] = 0xff;
        pViscaData[6] = 0x0;
        pViscaData[7] = 0x0;
        pViscaData[8] = 0x0;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void  ZoomOutCmd( unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x40;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = 0x0;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        if(nSpeed > 7 || nSpeed < 2)
        {
            nSpeed = 4;
        }
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x04;
        pViscaData[3] = 0x07;
        pViscaData[4] = 0x30 | nSpeed;
        pViscaData[5] = 0xff;
        pViscaData[6] = 0x0;
        pViscaData[7] = 0x0;
        pViscaData[8] = 0x0;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void TurnLeftUpCmd( unsigned char *pbuf, unsigned int nHSpeed, unsigned int nVSpeed, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x0C;
        pPelcodData[4] = nHSpeed;
        pPelcodData[5] = nVSpeed;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x06;
        pViscaData[3] = 0x01;
        pViscaData[4] = nHSpeed;
        pViscaData[5] = nVSpeed;
        pViscaData[6] = 0x01;
        pViscaData[7] = 0x01;
        pViscaData[8] = 0xff;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void TurnLeftDownCmd( unsigned char *pbuf, unsigned int nHSpeed, unsigned int nVSpeed, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x14;
        pPelcodData[4] = nHSpeed;
        pPelcodData[5] = nVSpeed;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x06;
        pViscaData[3] = 0x01;
        pViscaData[4] = nHSpeed;
        pViscaData[5] = nVSpeed;
        pViscaData[6] = 0x01;
        pViscaData[7] = 0x02;
        pViscaData[8] = 0xff;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void TurnRightUpCmd( unsigned char *pbuf, unsigned int nHSpeed, unsigned int nVSpeed, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x0a;
        pPelcodData[4] = nHSpeed;
        pPelcodData[5] = nVSpeed;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x06;
        pViscaData[3] = 0x01;
        pViscaData[4] = nHSpeed;
        pViscaData[5] = nVSpeed;
        pViscaData[6] = 0x02;
        pViscaData[7] = 0x01;
        pViscaData[8] = 0xff;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void TurnRightDownCmd( unsigned char *pbuf, unsigned int nHSpeed, unsigned int nVSpeed, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x12;
        pPelcodData[4] = nHSpeed;
        pPelcodData[5] = nVSpeed;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x06;
        pViscaData[3] = 0x01;
        pViscaData[4] = nHSpeed;
        pViscaData[5] = nVSpeed;
        pViscaData[6] = 0x02;
        pViscaData[7] = 0x02;
        pViscaData[8] = 0xff;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}
void StopFocus(uint8 portNum)
{
    pViscaData[1] = 0x01;
    pViscaData[2] = 0x04;
    pViscaData[3] = 0x08;
    pViscaData[4] = 0x0;
    pViscaData[5] = 0xff;
    pViscaData[6] = 0x0;
    pViscaData[7] = 0x0;
    pViscaData[8] = 0x0;
    //UART_Write(portNum,(uint8*)&pViscaData[0],VISCA_LENGTH-3 );
    UART_Write(portNum, (uint8 *)&pViscaData[0], VISCA_LENGTH - 3 ); //change 2015.10.14
}

void StopZoom(uint8 portNum)
{
    pViscaData[1] = 0x01;
    pViscaData[2] = 0x04;
    pViscaData[3] = 0x07;
    pViscaData[4] = 0x0;
    pViscaData[5] = 0xff;
    pViscaData[6] = 0x0;
    pViscaData[7] = 0x0;
    pViscaData[8] = 0x0;
    //UART_Write(portNum,(uint8*)&pViscaData[0],VISCA_LENGTH-3 );
    UART_Write(portNum, (uint8 *)&pViscaData[0], VISCA_LENGTH - 3 ); //change 2015.10.14
}

void StopPanTilt(uint8 portNum)
{
    pViscaData[1] = 0x01;
    pViscaData[2] = 0x06;
    pViscaData[3] = 0x01;
    pViscaData[4] = 0x0;
    pViscaData[5] = 0x0;
    pViscaData[6] = 0x03;
    pViscaData[7] = 0x03;
    pViscaData[8] = 0xff;
    UART_Write(portNum, (uint8 *)&pViscaData[0], VISCA_LENGTH );
}
void StopCmd(uint8 portNum, unsigned char *pbuf, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x0;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = 0x0;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        StopPanTilt(portNum);
        OSTimeDlyHMSM(0, 0, 0, 100); //add 2015.10.14
        StopZoom(portNum);
        OSTimeDlyHMSM(0, 0, 0, 100); //add 2015.10.14
        StopFocus(portNum);

    }
}

void GotoPrepositionCmd( unsigned char *pbuf, unsigned char cPos, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x07;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = cPos;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x04;
        pViscaData[3] = 0x3f;
        pViscaData[4] = 0x02;
        pViscaData[5] = cPos;
        pViscaData[6] = 0xff;
        pViscaData[7] = 0x0;
        pViscaData[8] = 0x0;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void SetPrepositionCmd( unsigned char *pbuf, unsigned char cPos, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x03;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = cPos;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x04;
        pViscaData[3] = 0x3f;
        pViscaData[4] = 0x01;
        pViscaData[5] = cPos;
        pViscaData[6] = 0xff;
        pViscaData[7] = 0x0;
        pViscaData[8] = 0x0;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void ClearPrepositionCmd( unsigned char *pbuf, unsigned char cPos, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x05;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = cPos;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x04;
        pViscaData[3] = 0x3f;
        pViscaData[4] = 0x00;
        pViscaData[5] = cPos;
        pViscaData[6] = 0xff;
        pViscaData[7] = 0x0;
        pViscaData[8] = 0x0;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void FocusNearCmd( unsigned char *pbuf, unsigned char cProtocol  )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x0;
        pPelcodData[3] = 0x80;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = 0x0;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x04;
        pViscaData[3] = 0x08;
        pViscaData[4] = 0x03;
        pViscaData[5] = 0xff;
        pViscaData[6] = 0x0;
        pViscaData[7] = 0x0;
        pViscaData[8] = 0x0;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}

void FocusFarCmd( unsigned char *pbuf, unsigned char cProtocol )
{
    if(DEV_PROTOCOL_PD == cProtocol)
    {
        pPelcodData[2] = 0x01;
        pPelcodData[3] = 0x0;
        pPelcodData[4] = 0x0;
        pPelcodData[5] = 0x0;
        pPelcodData[PELCOD_LENGTH - 1] = CheckSum1();
        memcpy( pbuf, pPelcodData, PELCOD_LENGTH );
    }
    else
    {
        pViscaData[1] = 0x01;
        pViscaData[2] = 0x04;
        pViscaData[3] = 0x08;
        pViscaData[4] = 0x03;
        pViscaData[5] = 0xff;
        pViscaData[6] = 0x0;
        pViscaData[7] = 0x0;
        pViscaData[8] = 0x0;
        memcpy( pbuf, pViscaData, VISCA_LENGTH );
    }
}


uint8 IsImageRecognition( const uint8 *pbuf )
{
    uint8 Imagebuf[4] = {0x02, 0x0, 0x07, 0x0};
    uint8 i;
    for(i = 0; i < 4; i++)
    {
        if(pbuf[i] != Imagebuf[i])
        {
            return 0;
        }
    }
    return 1;
}


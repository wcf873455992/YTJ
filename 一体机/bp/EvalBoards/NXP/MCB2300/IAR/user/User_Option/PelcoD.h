#ifndef _CAMERA_H

#define _CAMERA_H
#define PELCOD_LENGTH   7
#define VISCA_LENGTH    9

void TurnRightCmd(unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol);
void TurnLeftCmd(unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol);
void TurnUpCmd(unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol);
void TurnDownCmd(unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol);
void ZoomInCmd(unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol);
void ZoomOutCmd(unsigned char *pbuf, unsigned int nSpeed, unsigned char cProtocol);
void TurnLeftUpCmd(unsigned char *pbuf, unsigned int nHSpeed, unsigned int nVSpeed, unsigned char cProtocol);
void TurnLeftDownCmd(unsigned char *pbuf, unsigned int nHSpeed, unsigned int nVSpeed, unsigned char cProtocol);
void TurnRightUpCmd(unsigned char *pbuf, unsigned int nHSpeed, unsigned int nVSpeed, unsigned char cProtocol);
void TurnRightDownCmd(unsigned char *pbuf, unsigned int nHSpeed, unsigned int nVSpeed, unsigned char cProtocol);

void SetAddress(unsigned char bAddress, unsigned char cProtocol);
unsigned char CheckSum1(void);
void StopCmd(uint8 portNum, unsigned char *pbuf, unsigned char cProtocol);
void StopFocus(uint8 portNum);
void StopPanTilt(uint8 portNum);
void StopZoom(uint8 portNum);
void GotoPrepositionCmd(unsigned char *pbuf, unsigned char cPos, unsigned char cProtocol);
void SetPrepositionCmd(unsigned char *pbuf, unsigned char cPos, unsigned char cProtocol);
void ClearPrepositionCmd(unsigned char *pbuf, unsigned char cPos, unsigned char cProtocol);
void FocusNearCmd(unsigned char *pbuf, unsigned char cProtocol);
void FocusFarCmd(unsigned char *pbuf, unsigned char cProtocol);


uint8 IsImageRecognition(const uint8 *pbuf);
#endif

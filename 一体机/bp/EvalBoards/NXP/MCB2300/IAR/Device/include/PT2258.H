#ifndef __PT2258_H 
#define __PT2258_H

#define	U37Add	  0x80
#define	U42Add	  0x84
#define AllMUTE	       0xF9
#define AllSoundOn	0xF8

#define AllCH		0xe0
#define CH3		0x10
#define CH4		0x30
#define CH2		0x50
#define CH5		0x70
#define CH1		0x90
#define CH6		0xb0

#define DEV_DVD		0x01
#define DEV_REC		0x02
#define DEV_PC		0x03
#define DEV_NPC		0x04


#define SOUND_SELECT_IO1	1
#define SOUND_SELECT_IO2 	(1<<1)
#define SOUND_SELECT_IO3  	(1<<2)
#define SOUND_SELECT_IO4 	(1<<3)
#define SOUND_SELECT_IO5  	(1<<4)
#define SOUND_SELECT_IO6 	(1<<5)
#define SOUND_SELECT_IO7  	(1<<6)
#define SOUND_SELECT_IO8 	(1<<7)
#define SOUND_SELECT_IO9  	(1<<8)
#define SOUND_SELECT_IO10 	(1<<10)
#define SOUND_SELECT_IO11 	(1<<9)
#define SOUND_SELECT_IO12 	(1<<11)
#define SOUND_SELECT_IO13 	(1<<12)
#define SOUND_SELECT_IO14 	(1<<13)
#define SOUND_SELECT_IO15 	(1<<14)
#define SOUND_SELECT_IO16 	(1<<15)




void	Delay(uint16	k);
//ÒôÁ¿¿ØÖÆ
void	SoundVolumeControl(uint8	ch,uint8	volume);
void	PT2258Init(void);
void	SpiSendData(uint16	*data,uint8	len);
//Éè±¸ÒôÆµÑ¡Ôñ
void	Device_Sound_Select(uint8	device);
//Ñ¡ÔñÊÓÆµ±àÂë¿ªµÄÒôÆµÔ´
void BMK_Sound_Select(uint8 val);//ÍøÂçÒôÆµÊäÈëÑ¡Ôñ
void LAN_Sound_Select(uint8 type);
//ÍøÂçÒôÆµÊä³öÑ¡Ôñ
void LAN_Sound_OUT(uint8 type);
//¾²Òô
void SOUND_DEVICE_Mute(uint8 chanle,uint8 flag);
void Device_SoundMute(BYTE out_link);

void Device_InVol(BYTE Vol,BYTE in);
void Device_OutVol(BYTE Vol,BYTE out);
uint16 Device_Get_MIX(void);
BYTE Device_Get_Vol(BYTE Device,BYTE Type);
void Device_Set_MIX(uint16 SContrl);
void Device_Sound_Contrl(uint8 type);
void BJ_Select(uint8 contrl) ; 
struct LINK_REC_TABLE_TYPE *Get_DeviceSingleLink(void);
/////////////////////////////////////////
void SpiSendData1(uint8 *data,uint8 len);
////////////////////////////////////////
#endif

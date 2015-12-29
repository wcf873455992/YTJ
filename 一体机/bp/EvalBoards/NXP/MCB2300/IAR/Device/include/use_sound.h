#ifndef __USE_SOUND_H
#define __USE_SOUND_H

#define SOUND_BUFF_ADDRESS		0x7fd00000
#define SOUND_BUFF_SIZE			0x200
#define SOUND_OPEN_SIZE		10
#define SOUND_BUFF_MAX			10
#define SOUND_MIC_SIZE			3
#define SOUND_MIC_BUF			0x7fd01800//	SOUND_BUFF_ADDRESS+SOUND_BUFF_SIZE*SOUND_BUFF_MAX 


#define SOUND_22K  0x22
#define SOUND_11K  0x42
#define SOUND_44K  0x3e


#define MIC_ECHO_MAX1	0xfe00
#define MIC_ECHO_MIN1	0x200
#define MIC_ECHO_MAX2	0xff00
#define MIC_ECHO_MIN2	0x100

#define MICECHO_TIME	50
//////////////////////////////////
#define SOUND_OPTION_IP		0
#define SOUND_OPTION_SEAT		1
#define SOUND_OPTION_NUMBER	2
#define SOUND_OPTION_ALL		3
//////////////////////////////////

#define SOUND_CONTRL_ACTIVE		1
#define SOUND_CONTRL_TTL			(1<<1)
#define SOUND_CONTRL_SEND			(1<<2) //发送有效
#define SOUND_CONTRL_ECHO			(1<<3) //半双工标志
#define SOUND_DMA1_OK				(1<<4)
#define SOUND_DMA2_OK				(1<<5)
#define SOUND_IP_ASK					(1<<6)  //全双工回声消除模式，左MIC右PLAY
#define SOUND_IP_hASK				(1<<7)  //半双工模式:play=0，MIC数据发送，否则=0
#define SOUND_IP_mASK				(1<<8)  //正常采样模式:正常双声道



struct SOUND_OPTION_TYPE
{
    uint8 play_ip[4];
    uint16 snumber;
    uint8 contrl;
    uint8 save_pointer;
    uint8 seat;

};
struct SOUND_CONTRL_TYPE
{

    uint8 play_pointer;
    uint8 Input_pointer;
    uint16 Input_contrl;
    uint16 play_contrl;
    uint16 sound_echo_time;
    uint8 play_buff_flag[SOUND_BUFF_MAX];

    struct SOUND_OPTION_TYPE Rec[SOUND_OPEN_SIZE];
};

#define FLAG_SOUND_HEADER   0xa5aa
struct SOUND_HEADER_TYPE
{
    uint16 Flag;
    uint16 Seat;
    uint32 OWIP;
};



uint8 Sound_init(void);
uint32 Sound_Play_Int(void);
uint32 Sound_Input_Int(void);
void Sound_Save_PlayData(BYTE *buff, BYTE *pContrl, BYTE type);
BYTE Sound_Open(BYTE *desip, uint16 Number, uint16 seat, uint16 mic_Contrl);
BYTE Sound_Close(void *pdata , uint8 option);
uint8 Sound_Echo_Contrl(uint16 *data);
#endif

#ifndef __TMS320AIC23_H
#define __TMS320AIC23_H

#define TMS320AIC23_LEFT_INPUT_VOL   	0x00
#define TMS320AIC23_RIGHT_INPUT_VOL 	0x01
#define TMS320AIC23_LHP_VAL			 0x02
#define TMS320AIC23_RHP_VAL			 0x03
#define TMS320AIC23_ANL_PATH_CON	 	0x04
#define TMS320AIC23_DIG_PATH_CON	 	0x05
#define TMS320AIC23_POWER_CON		 	0x06
#define TMS320AIC23_DIG_INTERFACE     	0x07
#define TMS320AIC23_SAMPLE_RATE		 0x08
#define TMS320AIC23_ACTIVE			 	0x09
#define TMS320AIC23_AIC_RST			 0x0f

#define TMS320AIC23_MIC_VOL			0
#define TMS320AIC23_HP_VOL				1


struct  TMS320AIC23_TYPE
{
    unsigned short left_line_vol;
    unsigned short right_line_vol;
    unsigned short left_hp_vol;
    unsigned short right_hp_vol;
    unsigned short analog_path_con;
    unsigned short digital_path_con;
    unsigned short power_con;
    unsigned short digital_format;
    unsigned short sample_rate_con;
    unsigned short interface_active;

};
uint8 TMS320AIC23_init(void);
uint8 TMS320AIC23_Set_Vol(uint8 no, uint8 Vol);

#endif

#include "includes.h"
//uint8 save_hp_vol,save_input_vol;
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void aic23_delay(uint32 val)
{
    WORD tmp;

    for (tmp = 0; tmp < val; tmp++);

}

/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TMS320AIC23_Reg_Write(uint16 reg, uint16 val)
{
    uint8 k;
    uint16 tmp;
    uint32 tmp_reg;
#if OS_CRITICAL_METHOD == 3           /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif

    OS_ENTER_CRITICAL();
    tmp_reg = S0SPCR;
    S0SPCR  = (0 << 2) |
              (0 << 3) |				// CPHA = 0, 数据在SCK 的第一个时钟沿采样
              (0 << 4) |				// CPOL = 1, SCK 为低有效
              (1 << 5) |				// MSTR = 1, SPI 处于主模式
              (0 << 6) |				// LSBF = 0, SPI 数据传输MSB (位7)在先
              (0 << 7);				// SPIE = 0, SPI 中断被禁止
    IO_Out(OUT_AIC23_CS, FALSE);
    aic23_delay(10);
    tmp = (reg << 9	) + val;
    k = SPI_send(tmp >> 8);
    k = SPI_send(tmp & 0xff);
    aic23_delay(10);
    IO_Out(OUT_AIC23_CS, TRUE);
    S0SPCR = tmp_reg;
    OS_EXIT_CRITICAL();
    return	k;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//			vol 0-100
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TMS320AIC23_Set_Vol(uint8 no, uint8 Vol)
{
    uint32 save_input_vol, save_hp_vol;
    if (Vol > 100) Vol = 100;
    switch(no)
    {
    case TMS320AIC23_MIC_VOL:
        save_input_vol = Vol * 127 / 100; //0x18;
        //save_input_vol|=0x100;
        TMS320AIC23_Reg_Write(TMS320AIC23_LEFT_INPUT_VOL, save_input_vol); //save_input_vol);
        TMS320AIC23_Reg_Write(TMS320AIC23_RIGHT_INPUT_VOL, save_input_vol); //save_input_vol);
        break;
    case TMS320AIC23_HP_VOL:
        save_hp_vol = Vol * 120 / 100;
        TMS320AIC23_Reg_Write(TMS320AIC23_LHP_VAL, save_hp_vol); //save_hp_vol);
        TMS320AIC23_Reg_Write(TMS320AIC23_RHP_VAL, save_hp_vol); //save_hp_vol);
        break;

    };

    return TRUE;
}
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 TMS320AIC23_init(void)
{
    IO_Out(OUT_AIC23_MODE, TRUE); //mode=1 spi
    IO_Out(OUT_AIC23_CS, TRUE);
    //for (;;)

    TMS320AIC23_Reg_Write(TMS320AIC23_AIC_RST, 0); //reset
    //	aic23_delay(3000);
    TMS320AIC23_Set_Vol(TMS320AIC23_MIC_VOL, 24);
    TMS320AIC23_Set_Vol(TMS320AIC23_HP_VOL, 100);
    TMS320AIC23_Reg_Write(TMS320AIC23_ANL_PATH_CON, 0x30); //0x138);
    TMS320AIC23_Reg_Write(TMS320AIC23_DIG_PATH_CON, 0x40);
    TMS320AIC23_Reg_Write(TMS320AIC23_POWER_CON, 0x40);
    TMS320AIC23_Reg_Write(TMS320AIC23_DIG_INTERFACE, 0x42);
    TMS320AIC23_Reg_Write(TMS320AIC23_SAMPLE_RATE, SOUND_22K);
    TMS320AIC23_Reg_Write(TMS320AIC23_ACTIVE, 1);

    return TRUE;
    /*
    aic23.left_line_vol=0x80;  //left line input muted
    aic23.right_line_vol=0x80; //left line input muted
    aic23.left_hp_vol=0x70;
    aic23.right_hp_vol=0x70;

    aic23.analog_path_con=0x112;//0x16;//0x135;//0x135;
    aic23.digital_path_con=0x00 ;
    aic23.power_con=0x40;   //0x41
    aic23.digital_format=0x52;//0x53;0x520x5e
    aic23.sample_rate_con=SOUND_22K;
    aic23.interface_active=1;
    */



}


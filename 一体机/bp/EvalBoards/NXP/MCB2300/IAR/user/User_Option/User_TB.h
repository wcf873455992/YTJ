#ifndef __USER_TB_H
#define  __USER_TB_H

#define TB_CMD_Ver			0
#define TB_CMD_SelectAV		1
#define TB_CMD_SelectVGA	2
#define TB_CMD_CheckSingle	3
#define TB_CMD_State		4
#define TB_CMD_BreakVGA	5
#define TB_CMD_BreakAV		6
///////////////////////////////////
#define TB_InputSinle_No			0 //
#define TB_InputSinle_OUT_TB0	1 //AV_BUF0底板视频输出通道1
#define TB_InputSinle_OUT_TB1	2 //AV_BUF1底板视频输出通道2
#define TB_InputSinle_OUT_FZ1      3 //AV_BUF8底板视频输出通道8
#define TB_InputSinle_OUT_FZ2	4 //
#define TB_InputSinle_VGA2AV		5 //
#define TB_InputSinle_TBout		6//
#define TB_InputSinle_S_TB_Y		7//
#define TB_InputSinle_S_TB_C		8//

#define TB_OutputAV_No			0
#define TB_OutputAV_LMTAV1		1
#define TB_OutputAV_LMTAV2		2
#define TB_OutputAV_LMTAV3		3
#define TB_OutputAV_LMTAV4		4

/////////////////////////////////////
#define TB_InputVGA_No			0
#define TB_InputVGA_BJCardVGA	1
#define TB_InputVGA_OUT_TB0	2
#define TB_InputVGA_OUT_TB1	3
#define TB_InputVGA_TBCard		4

#define TB_OutputVGA_No			0
#define TB_OutputVGA_LMT		1
#define TB_OutputVGA_TYJ		2
//////////////////////////////////////////
#define TBCard_state_NoBusy   	0//通道空闲
#define TBCard_state_Recding   	1//通道正在录制
#define TBCard_state_Freez		2//通道冻结
#define TBCard_state_Ready 		3//通道准备就绪

#define TBCard_Option_VGA		0
#define TBCard_Option_AV		1



struct  SingleInfo_TYPE
{
    uint8 State;
    uint8 DeviceID;
};
struct DEVICE_TB_TYPE
{
    uint8 flag;
    uint8 curr_Type;  //同步i板当前输出类型
    uint8 vgaout;
    uint8 videoout;

};

struct TBCard_RECNo_State_Type
{
    struct  SingleInfo_TYPE SingleInfo[5];
};
struct TB_SingleState_TYPE
{
    uint8 AVSingle;
    uint8 VGASingle;

};
struct TB_LinKState_Type
{
    uint8 AVOutSingleLink[4];
    uint8 VgaOutSingleLink[2];

};
struct TB_RECSingle_ContrlType
{
    uint8 SingleType;//录制通道类型0：VGA  1 VIDEO通道
    uint8 SingleNo;//通道号：VGA（1-2）AV(1-4)
    uint8 SingleID;//连接设备号（ID）

};
uint8 TB_ReadVer(uint8 *pdata);
uint8 TB_ReadSingleState(void);
uint8 TB_Read_Csingle(void);
uint8 TB_Read_state(void);

uint8 TB_Check_VGA_InputSingle(uint8 input);
uint8 TB_Check_AV_InputSingle(uint8 input);
uint8 TB_SelectVGA(uint8 Input, uint8 Output);
uint8 TB_SelectAV(uint8 Input, uint8 Output);
struct TBCard_RECNo_State_Type *Get_TB_REC_State(void);
struct TB_LinKState_Type *Get_TBLinkState(void);
uint8 TB_REC_Freez(uint8 num_type, uint8 num, uint8 ID);
uint8 TB_REC_UnFreez(uint8 num_type, uint8 num, uint8 ID);
uint8 TB_SingleChage(uint8 SingleType, uint8 Single);
void TB_msg_rev(uint8 *pdata);
uint8 TB_BreakVGA(uint8 Output);
uint8 TB_BreakAV(uint8 Output);
void TB_init(void);
#endif

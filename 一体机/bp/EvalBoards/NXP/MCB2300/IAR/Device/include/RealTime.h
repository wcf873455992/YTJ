#ifndef __REALTIME_H
#define __REALTIME_H
//////////CIIR控制中断
#define REALTIME_CIIR_SEC	(1)
#define REALTIME_CIIR_MIN	(1<<1)
#define REALTIME_CIIR_HOUR	(1<<2)
#define REALTIME_CIIR_DAY	(1<<3)
#define REALTIME_CIIR_WEEK	(1<<4)
#define REALTIME_CIIR_YEAR	(1<<5)
#define REALTIME_CIIR_MON	(1<<6)

///////AMR控制定时
#define REALTIME_AMRSEC		(1)
#define REALTIME_AMRMIN		(1<<1)
#define REALTIME_AMRHOUR	(1<<2)
#define REALTIME_AMRDOM		(1<<3)
#define REALTIME_AMRDOW		(1<<4)
#define REALTIME_AMRDOY		(1<<5)
#define REALTIME_AMRMON		(1<<6)
#define REALTIME_AMRYEAR	(1<<7)

#define CLASS_TABLE_FLAG_START	(1)
#define CLASS_TABLE_FLAG_OVER	(1<<1)

struct CLASS_TABLE_TYPE
{
    BYTE week;
    BYTE Curr_rec;
    uint16 flag;
    uint8 Class_Table_Rec[256];
};
struct REALTIME_TYPE
{
    uint16 year;//
    uint8   mon;//
    uint8   day;//
    uint8   week;//
    uint8   hour;//
    uint8   min;//
    uint8   sec;//
};
void On_MSG_OPTION_CLASSTABLE_OFF(void);
void On_MSG_OPTION_CLASSTABLE_ON(void);
struct REALTIME_TYPE *RealTime_Get(void);
uint32 RealTime_Get_Minute(void);
void RealTime_ALM_Set(BYTE cmd, uint8 *time_data);
void ON_MSG_OPTION_ALM_TIME(BYTE *pdata);
uint8 RealTime_Compare(BYTE *pdata);
void RealTime_Set(BYTE *time_val);
void RealTime_init(void);
void ClassTableWeekInit(void);

//Variable declare
extern struct CLASS_TABLE_TYPE gClassT;
#endif

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus

extern "C" {
#endif

//这一段无需改动
#include "includes.h"
#define __USER_DEBUG_EN  1
/**********************************************************************************
**  		   应用程序配置
**		Application Program Configurations
**********************************************************************************/
//以下根据需要改动

#define  TRACE_LEVEL_OFF                       0
#define  TRACE_LEVEL_INFO                      1
#define  TRACE_LEVEL_DEBUG                     2

#define  APP_TRACE_LEVEL                TRACE_LEVEL_DEBUG
#define  APP_TRACE

#define  APP_TRACE_INFO(x)            ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DEBUG(x)           ((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE x) : (void)0)



#ifdef __cplusplus
}
#endif
#endif
/*******************************************************************************************
                              End Of File
*******************************************************************************************/

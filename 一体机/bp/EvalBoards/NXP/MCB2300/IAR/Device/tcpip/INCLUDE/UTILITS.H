#ifdef UTILITS_GLOBALS
#define UTILITS_EXT
#else
#define UTILITS_EXT extern 
#endif 
UTILITS_EXT void Delay_MS(unsigned int ms_number);
UTILITS_EXT void Port_Hex2Str(unsigned int portnum);    //IP地址值转换到IP字符串
UTILITS_EXT unsigned int CheckSum(unsigned int xdata *check,unsigned int length);
//UTILITS_EXT
//UTILITS_EXT

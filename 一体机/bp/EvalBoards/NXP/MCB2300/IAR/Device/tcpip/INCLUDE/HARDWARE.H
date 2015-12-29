#ifdef HARDWARE_GLOBALS
#define HARDWARE_EXT
#else
#define HARDWARE_EXT extern 
#endif 

#define MAX_REC_BUFF 	4
union  REC_BUFF_UNION
{
	uint32	Dwords[384];
	uint16	words[768];
	uint8 	bytes[1536];
};


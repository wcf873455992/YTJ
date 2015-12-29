#ifndef _USER_MEM_H
#define _USER_MEM_H
void User_mem_init(void);
void *User_mem_allco(uint16  lg);
BYTE  User_mem_free(void *memadd);
#endif

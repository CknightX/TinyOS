#include "debug.h"
#include "types.h"
extern uint8_t k_reenter; 
int printf(char* fmt,...);

void assertion_failure(char* exp,char* file,char* base_file,int line)
{
#define DBG_MSG "assert(%s) failed: file: %s, base_file: %s, ln%d",\
	exp, file, base_file, line


	if (k_reenter+1==0) //ring<1-3>
	{
		printf(DBG_MSG);
		while(1){}
	}
	else if (k_reenter+1>0) // ring0
	{
		printk(DBG_MSG);
		__asm__ __volatile__("hlt");
		while(1){}
	}
	else
	{

	}
}

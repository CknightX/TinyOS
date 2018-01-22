#include "debug.h"
#include "types.h"
#include "stdio.h"
int8_t k_reenter; 

void assertion_failure(char* exp,char* file,char* base_file,int line)
{
	printf("assert(%s) failed: file: %s, base_file: %s, line: %d\n",\
			exp, file, base_file, line);

	if (k_reenter==-1) //ring<1-3>
	{
		while(1){}
	}
	else if (k_reenter>-1) // ring0
	{
		__asm__ __volatile__("hlt");
		while(1){}
	}
	else
	{

	}
}

void panic(const char* err)
{
	if ((char)(k_reenter+1)==0) //ring<1-3>
	{
		printf(err);
		while(1){}
	}
	else if ((char)(k_reenter+1)>0) // ring0
	{
		printk(err);
		__asm__ __volatile__("hlt");
		while(1){}
	}
	else
	{

	}
}

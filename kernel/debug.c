#include "debug.h"
#include "types.h"
#include "stdio.h"
#include "vargs.h"
int8_t k_reenter; 
extern void printx(const char* msg);
int vsprintf(char *buff, const char *format, va_list args);

void assertion_failure(char* exp,char* file,char* base_file,int line)
{
	printl("%cassert(%s) failed: file: %s, base_file: %s, line: %d\n",
			MAGIC_ASSERT,
			exp, file, base_file, line);
	spin("assertion!!!");
	__asm__ __volatile__("ud2");
}

void printl(const char* fmt,...)
{
	static char buff[1024];
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsprintf(buff, fmt, args);
	va_end(args);
	buff[i]='\0';
	printx(buff);
}
void spin(const char* msg)
{
	printl("\nspinning in %s ...\n",msg);
	while(1);
}
void panic(char* msg)
{
	printl("%c%s panic!!!",MAGIC_PANIC,msg);
}

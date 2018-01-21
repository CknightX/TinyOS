#include "screen.h"
#include "string.h"
#include "vargs.h"

int vsprintf(char *buff, const char *format, va_list args);
void write(const char* str,int len);

int printf(const char *format, ...)
{
	// 避免频繁创建临时变量，内核的栈很宝贵
	static char buff[1024];
	va_list args;
	int i;

	va_start(args, format);
	i = vsprintf(buff, format, args);
	va_end(args);

	write(buff,i);
	
	return i;
}



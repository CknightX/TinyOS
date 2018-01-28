#ifndef _DEBUG_H
#define _DEBUG_H
#include "screen.h"
#include "stdio.h"

#define DEBUG
#ifdef DEBUG
void assertion_failure(char* exp,char* file,char* base_file,int line);
#define assert(exp) if (exp) ;\
	else assertion_failure(#exp,__FILE__,__BASE_FILE__,__LINE__)

/*
#define log(msg,args...) do{\
	printf("--------------");\
	printf(msg,##args);\
	printf("\nfile: %s, line: %d\n",__FILE__,__LINE__);\
}while(0);
*/
#define log(msg) 
#else
#define assert(exp)
#endif

#define MAGIC_ASSERT '\003'
#define MAGIC_PANIC '\002'

// 内核的打印函数
void printk(const char *format, ...);

// 内核的打印函数 带颜色
void printk_color(real_color_t back, real_color_t fore, const char *format, ...);


void panic(char* msg);
void spin(const char* msg);




#endif

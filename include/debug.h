#ifndef _DEBUG_H
#define _DEBUG_H
#include "screen.h"

// 内核的打印函数
void printk(const char *format, ...);

// 内核的打印函数 带颜色
void printk_color(real_color_t back, real_color_t fore, const char *format, ...);


#endif
#ifndef _COMMON_h
#define _COMMON_h
#include "types.h"

// 端口写一个字节
void outb(uint16_t port,uint8_t value);
// 端口读一个字节
uint8_t inb(uint16_t port);
// 端口读一个字
uint16_t inw(uint16_t port);

void delay();

#endif

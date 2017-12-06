#include "debug.h"
#include "string.h"
#include "idt.h"
char str[100]="van:";

int main(void)
{
	delay();
	printk("OS now\n");
	//asm volatile ("int $0x4");
	return 0;
}

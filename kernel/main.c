#include "debug.h"
#include "string.h"
#include "idt.h"
char str[100]="van:";

int main(void)
{
	delay();
	printk("OS now\n");
	return 0;
}

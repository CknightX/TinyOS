#include "debug.h"
#include "string.h"
#include "idt.h"
#include "proc.h"

extern void restart();

void test1()
{
	int i=0;
	while(1)
	{
		printk("A%d.",i++);
		delay(1);
	}
}

void test2()
{
	int i=0x1000;
	while(1)
	{
		printk("B%d.",i++);
		delay(1);
	}
	
}

int main(void)
{
	printk("OS now\n");
	delay(2);
	p_proc_ready=proc_table;

	restart(); //进入进程
	while(1){}
}

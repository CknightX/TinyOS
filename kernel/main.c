#include "debug.h"
#include "string.h"
#include "idt.h"
#include "proc.h"

extern void clock_handler();
extern void restart();
extern int get_ticks();

void test1()
{
	while(1)
	{
		printk("A%d.",get_ticks());
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

int _osmain(void)
{
	ticks=0;
	printk("OS now\n");
	delay(2);
	p_proc_ready=proc_table;

	set_irq_handler(CLOCK_IRQ,clock_handler);
	enable_irq(CLOCK_IRQ);

	restart(); //进入进程
	while(1){}
}

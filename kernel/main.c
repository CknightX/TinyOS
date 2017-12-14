#include "debug.h"
#include "string.h"
#include "idt.h"
#include "proc.h"

extern void clock_handler();
extern void restart();
extern int get_ticks();
extern void milli_delay(int milli_sec);
extern void set_8253();

void test1()
{
	while(1)
	{
		printk("A.");
		milli_delay(200);
	}
}

void test2()
{
	while(1)
	{
		printk("B.");
		milli_delay(200);
	}
}
void test3()
{
	while(1)
	{
		printk("C.");
		milli_delay(200);
	}
}

void init()
{
	set_8253();
	ticks=0;
	printk("OS now\n");
	delay(2);
	p_proc_ready=proc_table;

	set_irq_handler(CLOCK_IRQ,clock_handler);
	enable_irq(CLOCK_IRQ);

	proc_table[0].ticks=proc_table[0].priority=150;
	proc_table[1].ticks=proc_table[1].priority=50;
	proc_table[2].ticks=proc_table[2].priority=30;
}

int _osmain(void)
{
	init();
	restart(); //进入进程
	while(1){}
}

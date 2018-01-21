#include "debug.h"
#include "stdio.h"
#include "string.h"
#include "idt.h"
#include "proc.h"

extern void clock_handler();
extern void restart();
extern int get_ticks();
extern void milli_delay(int milli_sec);
extern void set_8253();
extern void init_keyboard();
extern void init_clock();

/*link.ld*/
extern uint8_t kernel_start[];
extern uint8_t kernel_end[];
extern uint8_t code[];
extern uint8_t data[];

void test1()
{
	while(1)
	{
		printf("A.");
		milli_delay(200);
	}
}

void test2()
{
	while(1)
	{
		printf("B.");
		milli_delay(200);
	}
}
void test3()
{
	while(1)
	{
		printf("C.");
		milli_delay(200);
	}
}

void init()
{
	init_clock();
	printk("OS now\n");
	printk("kernel start:  0x%08X\n",kernel_start);
	printk("code start:    0x%08X\n",code);
	printk("data start:    0x%08X\n",data);
	printk("kernel end:    0x%08X\n",kernel_end);
	printk("kernel used:   %d KB\n\n",(kernel_end-kernel_start+1023)/1024);
	p_proc_ready=proc_table;


	proc_table[0].ticks=proc_table[0].priority=150;
	proc_table[1].ticks=proc_table[1].priority=50;
	proc_table[2].ticks=proc_table[2].priority=30;
}

int _osmain(void)
{
	init();
	init_keyboard();

	restart(); //进入进程
	while(1){}
}

#include "debug.h"
#include "proc.h"
void clock_handler(int irq)
{
	++ticks;
	printk("#");
	if (k_reenter!=0)
	{
		printk("!");
		return;
	}

	p_proc_ready++;
	if (p_proc_ready>=proc_table+NR_TASKS)
		p_proc_ready=proc_table; //回到第一个进程处
	
}

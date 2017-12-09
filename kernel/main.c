#include "debug.h"
#include "string.h"
#include "idt.h"
#include "proc.h"
char str[100]="van:";

extern Process* p_proc_ready;
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

int main(void)
{
	printk("OS now\n");
	delay(2);
	p_proc_ready=proc_table;
	restart();
	while(1){}
}

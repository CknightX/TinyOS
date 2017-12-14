#include "debug.h"
#include "proc.h"
#include "common.h"
#include "types.h"
#include "const.h"

// syscall.asm
extern int get_ticks();

void set_8253()
{

	outb(TIMER_MODE, RATE_GENERATOR);
	outb(TIMER0, (uint8_t) (TIMER_FREQ/HZ) );
	outb(TIMER0, (uint8_t) ((TIMER_FREQ/HZ) >> 8));
}

// 毫秒为单位的延迟函数
void milli_delay(int milli_sec)
{
	int t=get_ticks();
	while((get_ticks()-t)*1000/HZ<milli_sec){}
}


void clock_handler(int irq)
{
	++ticks;
//	printk("#");
	// 中断重入
	--p_proc_ready->ticks;
	if (k_reenter!=0)
	{
//		printk("!");
		return;
	}

	// 当一个进程ticks还未到0时，其他进程就不会被调度
	if (p_proc_ready->ticks>0)
		return;

	schedule();
	
}

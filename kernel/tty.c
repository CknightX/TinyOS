#include "types.h"
#include "debug.h"
#include "keyboard.h"
#include "screen.h"
#include "proc.h"
#include "ipc.h"
extern void keyboard_read();
extern void disable_int();

void task_tty()
{
	assert(0);
	while(1)
		keyboard_read();
}

void in_process(uint32_t key)
{
	char output[2]={'\0','\0'};
	if (!(key&FLAG_EXT))
	{
		output[0]=key&0xff;
		printk("%s",output);
	}
	else
	{
		int raw_code=key&MASK_RAW;
		switch(raw_code)
		{
			case ENTER:
				printk("\n");
				break;
			case BACKSPACE:
				printk("\b \b");
				break;
			default:
				break;
		}
	}
}

void tty_write(char* buf,int len)
{
	while(len--)
		console_putc_color(*buf++,rc_black,rc_white);
}

int sys_write(char* buf, int len, Process* p_proc)
{
	tty_write(buf,len);
	return 0;
}

int sys_printx(int _unused1,int unused2,char* s,Process* p_proc)
{
	const char* p;
	char reenter_err[]="? reenter is incorrect";
	reenter_err[0]=MAGIC_PANIC;

	if (k_reenter==0) // ring<1-3>
		p=(const char*)va2la(proc2pid(p_proc),s);
	else if (k_reenter>0) // ring<0>
		p=s;
	else
		p=reenter_err;


	if ((*p==MAGIC_PANIC)||
			(*p==MAGIC_ASSERT && p_proc_ready<&proc_table[NR_TASKS])) // TASK
	{
		disable_int();
		const char* q=p+1; // 跳过magic char
		printk(q);
		__asm__ __volatile__("hlt");
	}
	printk(p);


	return 0;

}

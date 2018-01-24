#include "types.h"
#include "debug.h"
#include "keyboard.h"
#include "screen.h"
#include "proc.h"
extern void keyboard_read();

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

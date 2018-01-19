#include "types.h"
#include "debug.h"
#include "keyboard.h"
extern void keyboard_read();

void task_tty()
{
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

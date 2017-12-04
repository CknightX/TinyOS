#include "screen.h"
int main(void)
{
	console_clear();
	int i=0;
	for (;i<10;++i)
	{
		console_putc_color(i+'0',rc_black,rc_white);
		console_putc_color('\n',rc_black,rc_white);
	}
	console_write("The deep dark fantasy!");
	for(;;);

}

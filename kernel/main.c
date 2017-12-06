#include "debug.h"
#include "string.h"
char str[100]="van:";

int main(void)
{
	const char* ddf="the deep dark fantasy\n";
	const char* bnd="boy next door\n";
	strcat(str,ddf);
	strcat(str,bnd);

	console_write(str);

	printk("ddf size=%d\n",strlen(ddf));




	for(;;);

}

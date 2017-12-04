int main(void)
{
	char* video=(char*)0xb8000;
	int i=0;
	for (;i<10;i+=2)
		video[i+(80*3+0)*2]='H';
		
	for(;;);

}

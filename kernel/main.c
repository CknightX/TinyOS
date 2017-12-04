int main(void)
{
	char* video=(char*)0xb8000;
	const char* hehe="The deep dark fantasy!";
	int i=0;
	int j=0;
	while(hehe[i]!='\0')
	{
		video[j+(80*3+0)*2]=hehe[i];
		j+=2;
		i++;
	}
	for(;;);

}

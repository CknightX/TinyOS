#include "string.h"
inline void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
	for (;len!=0;--len)
		*dest++=*src++;
}
void memset(void *dest, uint8_t val, uint32_t len)
{
	uint8_t* _dest=(uint8_t*)dest;
	for (;len!=0;--len)
		*_dest++=val;
}
void bzero(void *dest, uint32_t len)
{
	memset(dest,0,len);
}
int strcmp(const char *str1, const char *str2)
{
	while(*str1!='\0'&&*str2!='\0')
	{
		if (*str1>*str2)
			return 1;
		else if (*str1<*str2)
			return -1;
		else
		{
			str1++;
			str2++;
		}
	}
	if (*str1)
		return 1;
	if (*str2)
		return -1;
	return 0;
}
char *strcpy(char *dest, const char *src)
{
	char* _dest=dest;
	while(*src!='\0')
	{
		*dest++=*src++;
	}
	*dest=*src; // \0

	return _dest;
}
char *strcat(char *dest, const char *src)
{
	char* _dest=dest;
	while(*dest++!='\0');
	--dest;

	strcpy(dest,src);
	return _dest;
}
int strlen(const char *src)
{
	int len=0;
	while(*src++)
		++len;
	return len;
}

#ifndef _MESSAGE_H
#define _MESSAGE_H
#include "types.h"
typedef struct _msg1
{
	int imsg1;
	int imsg2;
	int imsg3;
} msg1;

typedef struct _msg2
{
	void* pmsg1;
	void* pmsg2;
	void* pmsg3;
} msg2;

typedef struct _msg3
{
	int imsg1;
	int imsg2;
	int imsg3;
	int imsg4;

	uint64_t umsg1;
	uint64_t umsg2;

	void* pmsg1;
	void* pmsg2;
	 
} msg3;

typedef struct _MESSAGE
{
	int source;
	int type;
	union _u
	{
		msg1 m1;
		msg2 m2;
		msg3 m3;
	}u;
} MESSAGE;

#define RETVAL u.m3.imsg1
#endif

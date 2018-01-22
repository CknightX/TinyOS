#ifndef _IPC_H
#define _IPC_H
#include "proc.h"
struct msg1
{
	int imsg1;
	int imsg2;
	int imsg3;
};

struct msg2
{
	void* pmsg1;
	void* pmsg2;
	void* pmsg3;
};

struct msg3
{
	int imsg1;
	int imsg2;
	int imsg3;
	int imsg4;

	uint64_t umsg1;
	uint64_t umsg2;

	void* pmsg1;
	void* pmsg2;
	 
};

struct MESSAGE
{
	int source;
	int type;
	union u
	{
		struct msg1 m1;
		struct msg2 m2;
		struct msg3 m3;
	}
};


int sys_sendrec(int function,int src_dest,MESSAGE* m,struct proc* p);
#endif

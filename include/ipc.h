#ifndef _IPC_H
#define _IPC_H
#include "types.h"
#include "message.h"
#include "ipc.h"
#include "const.h"

// status
#define SENDING 0x02
#define RECEIVING 0x04

// func no
#define SEND 1
#define RECEIVE 2
#define BOTH 3

#define ANY		(NR_TASKS + NR_PROCS + 10)
#define NO_TASK		(NR_TASKS + NR_PROCS + 20)

#define INTERRUPT -10

enum msgtype
{
	HARD_INT=1,
	GET_TICKS,
};



int sys_sendrec(int function,int src_dest,MESSAGE* m,struct proc* p);
int msg_send(struct proc* current,int dest,MESSAGE* m);
int msg_receive(struct proc* current,int src,MESSAGE* m);
#endif

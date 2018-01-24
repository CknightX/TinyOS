#ifndef _IPC_H
#define _IPC_H
#include "types.h"
#include "message.h"
#include "ipc.h"
#include "const.h"
#include "proc.h"

// status
#define SENDING 0x02
#define RECEIVING 0x04

// func no
#define SEND 1
#define RECEIVE 2
#define BOTH 3



#define INTERRUPT -10

extern int sendrec(int function,int src_dest,MESSAGE* m);

enum msgtype
{
	HARD_INT=1,
	GET_TICKS,
};


void reset_msg(MESSAGE* p);
int send_recv(int function,int src_dest,MESSAGE* m);
int sys_sendrec(int function,int src_dest,MESSAGE* m,struct proc* p);
int msg_send(struct proc* current,int dest,MESSAGE* m);
int msg_receive(struct proc* current,int src,MESSAGE* m);
#endif

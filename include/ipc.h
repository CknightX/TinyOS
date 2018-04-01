#ifndef _IPC_H
#define _IPC_H
#include "types.h"
#include "message.h"
#include "ipc.h"
#include "const.h"
#include "proc.h"

// 通信状态
#define SENDING 0x02
#define RECEIVING 0x04

// 通信模式
#define SEND 1
#define RECEIVE 2
#define BOTH 3


// 预设任务
#define INTERRUPT -10
#define TASK_TTY 0
#define TASK_HD 2
#define TASK_FS 3
#define TASK_MM 4

#define ANY		(NR_TASKS + NR_PROCS + 10)
#define NO_TASK		(NR_TASKS + NR_PROCS + 20)


extern int sendrec(int function,int src_dest,MESSAGE* m);

enum msgtype
{
	HARD_INT=1,
	GET_TICKS,

	DEV_OPEN=1001,
	DEV_CLOSE,
	DEV_READ,
	DEV_WRITE,
	DEV_IOCTL,

};


void reset_msg(MESSAGE* p);
int send_recv(int function,int src_dest,MESSAGE* m);
int sys_sendrec(int function,int src_dest,MESSAGE* m,struct proc* p);
int msg_send(struct proc* current,int dest,MESSAGE* m);
int msg_receive(struct proc* current,int src,MESSAGE* m);
void* va2la(int pid,void* va);
#endif

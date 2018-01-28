#include "message.h"
#include "types.h"
#include "debug.h"
#include "stdio.h"
#include "ipc.h"
void printx(char* str);
void task_sys()
{
	MESSAGE msg;
	while(1)
	{
		send_recv(RECEIVE,ANY,&msg);
		int src=msg.source;

		switch(msg.type)
		{
			case GET_TICKS:
				log("Someone want to get ticks.");
				msg.RETVAL=ticks;
				send_recv(SEND,src,&msg);
				break;
			default:
				panic("unknown msg type");
				break;
		}
	}
}

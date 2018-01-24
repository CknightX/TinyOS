#include "message.h"
#include "types.h"
#include "debug.h"
#include "stdio.h"
#include "ipc.h"

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
				msg.RETVAL=ticks;
				send_recv(SEND,src,&msg);
				break;
			default:
				panic("unknown msg type");
				break;
		}
	}
}

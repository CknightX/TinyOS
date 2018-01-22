#include "ipc.h"
#include "debug.h"
#include "proc.h"

// 根据索引idx来得到p进程的IDT对应项的段基址
int ldt_seg_linear(struct proc* p,int idx)
{
	struct Descriptor* d=&p->ldts[idx];
	return d->base_high<<24 | d->base_mid<<16 | d->base_low;
}

// 虚拟地址转换成线性地址
void* va2la(int pid,void* va)
{
	struct proc* p=&proc_table[pid];
	uint32_t seg_base=ldt_seg_linear(p,INDEX_LDT_RW);
	uint32_t la=seg_base+(uint32_t)va;

	if (pid<NR_TASKS+NR_PROCS)
	{
		
	}
	return (void*)la;
}

// 将msg置0
void reset_msg(MESSAGE* p)
{
	memset(p,0,sizeof(MESSAGE));
}


// 收发信息
int sys_sendrec(int function,int src_dest,MESSAGE* m,struct proc* p)
{
	assert(k_reenter==0); // 不处于ring0
	assert((src_dest>=0&&src_dest<NR_TASKS+NR_PROCS)||
			src_dest==ANY||
			src_dest==INTERRUPT);

	int ret=0;
	int caller=proc2pid(p);
	MESSAGE* mla=(MESSAGE*)va2la(caller,m);
	mla->source=caller;

	assert(mla->source!=src_dest); // 禁止自己给自己发信息

	switch(function)
	{
		case SEND:
			ret=msg_send(p,src_dest,m);
			if (ret!=0)
				return ret;
			break;
		case RECEIVE:
			ret=msg_receive(p,src_dest,m);
			if (ret!=0)
				return ret;
			break;
		default:
			break;
	}
	return 0;
}
// 阻塞一个进程
void block(struct proc* p)
{
	assert(p->p_flags);
	schedule();
}
// 解除一个线程的阻塞
void unblock(struct proc* p)
{
	assert(p->p_flags==0);
}

// 判断消息发送有没有形成死锁,即A->B->C->A
int deadlock(int src,int dest)
{
	struct proc* p=proc_table+dest;
	while(1)
	{
		if (p->p_flags & SENDING) // 目标进程在发送状态
		{
			if (p->p_sendto==src) // 且目标是源
		}
	}
}

// 发送消息
int msg_send(struct proc* current,int dest,MESSAGE* m)
{
	struct proc* sender=current;
	struct proc* receiver=proc_table+dest;

	assert(proc2pid(sender)!=dest); // 禁止自己给自己发消息

	if (deadlock(proc2pid(sender),dest)) // 死锁判断
		panic("DEADLOCK");

	if ((receiver->p_flags & RECEIVING)&& // 接收方处于接收信息的状态
			receiver->p_recvfrom==proc2pid(sender)||
			receiver->p_recvfrom==ANY)
	{
		assert(receiver->p_msg); // 消息体不为NULL
		assert(m);

		phys_copy(va2la(dest,receiver->p_msg), // 拷贝消息
				va2la(proc2pid(sender),m),
				sizeof(MESSAGE));

		receiver->p_msg=0;
		receiver->p_flags&=~RECEIVING;
		p_dest->p_recvfrom=NO_TASK;
		unblock(receiver);

		assert(receiver->p_flags==0);
		assert(receiver->p_msg==0);
		assert(receiver->p_recvfrom==NO_TASK);
		assert(receiver->p_sendto==NO_TASK);

		assert(sender->p_flags==0);
		assert(sender->p_msg==0);
		assert(sender->p_recvfrom==NO_TASK);
		assert(sender->p_sendto==NO_TASK);

	}
	else // 目标进程不处于接收消息的状态
	{
		sender->p_flags|=SENDING;
		sender->p_sendto=dest;
		sender->p_msg=m;

		struct proc* p;
		if (receiver->q_sending)
		{
			p=receiver->q_sending;
			while(p->next_sending)
				p=p->next_sending;
			p->next_sending=sender;
		}
		else
		{
			receiver->q_sending=sender;
		}
		sender->next_sending=0;
		block(sender);

		assert(sender->p_flags==SENDING);
		assert(sender->p_msg!=0);
		assert(sender->p_recvfrom==NO_TASK);
		assert(sender->p_sendto==dest);
	}

	return 0;
}

int msg_receive(struct proc* current,int src,MESSAGE* m)
{
	struct proc* receiver=current;
	struct proc* p_from=0;
	struct proc* prev=0;
	int copyok=0;

	assert(proc2pid(receiver)!=src);

	if ((receiver->has_int_msg)&& // 来自中断的消息
			((src==ANY)||(src==INTERRUPT)))
	{
		MESSAGE msg;
		reset_msg(msg);
		msg.source=INTERRUPT;
		msg.type=HARD_INT;

		assert(m);

		phys_copy(va2la(proc2pid(receiver),m),&msg,sizeof(MESSAGE));
		
		receiver->has_int_msg=0;
		assert(receiver->p_flags==0);
		assert(receiver->p_msg==0);
		assert(receiver->p_sendto==NO_TASK);
		assert(receiver->has_int_msg==0);

		return 0;
	}
	
	if (src==ANY) // 消息来源任意
	{
		if (receiver->q_sending)
		{
			p_from=receiver->q_sending; // 从队列中取出第一个
			copyok=1;

			assert(receiver->p_flags==0);
			assert(receiver->p_msg==0);
			assert(receiver->p_recvfrom==NO_TASK);
			assert(receiver->p_sendto==NO_TASK);
			assert(receiver->q_sending!=0);

			assert(p_from->p_flags==SENDING);
			assert(p_from->p_msg!=0);
			assert(p_from->p_recvfrom==NO_TASK);
			assert(p_from->p_sendto==proc2pid(receiver));
		}
	}
	else // 消息来自一个确定的进程
	{
		p_from=&proc_table[src];

		if (p_from->p_flags & SENDING &&
				(p_from->p_sendto==proc2pid(receiver)))
		{
			copyok=1;
			struct proc* p=receiver->q_sending;
			assert(p);

			while(p)
			{
				assert(p_from->p_flags & SENDING);
				if (proc2pid(p)==src)
				{
					p_from=p; // ?
					break;
				}
				prev=p;
				p=p->next_sending;
			}
			
			assert(receiver->p_flags==0);
			assert(receiver->msg==0);
			assert(receiver->p_recvfrom==NO_TASK);
			assert(receiver->p_sendto==NO_TASK);
			assert(receiver->q_sending!=0);

			assert(p_from->p_flags==SENDING);
			assert(p_from->p_msg!=0);
			assert(p_from->p_recvfrom==NO_TASK);
			assert(p_from->p_sendto==proc2pid(receiver));


		}
	}

	if (copyok)
	{
		if (p_from==receiver->q_sending) // 第一个
		{
			assert(prev==0);
			receiver->q_sending=p_from->next_sending; // 从队列中删除
		}
		else
		{
			assert(prev);
			prev->next_sending=p_from->next_sending;
		}
		p_from->next_sending=0;

		assert(m);
		assert(p_from->p_msg);

		phys_copy(va2la(proc2pid(receiver),m),
				va2la(proc2pid(p_from),p_from->p_msg),
				sizeof(MESSAGE));

		p_from->p_msg=0;
		p_from->sendto=NO_TASK;
		p_from->p_flags &= ~SENDING;
		unblock(p_from);
	}
	else // 没有消息
	{
		receiver->p_flags|=RECEIVING;
		receiver->p_msg=m;
		if (src==ANY)
			receiver->p_recvfrom=ANY;
		else
			receiver->p_recvfrom=proc2pid(p_from);

		block(receiver); // 将自身阻塞

		assert(receiver->p_flags==RECEIVING);
		assert(receiver->p_msg!=0);
		assert(receiver->p_recvfrom!=NO_TASK);
		assert(receiver->p_sendto==NO_TASK);
		assert(receiver->has_int_msg==0);
	}
}

#include "proc.h"
#include "gdt.h"
#include "string.h"
#include "screen.h"
extern Descriptor gdt[];
void test1();
void test2();
void test3();
void task_tty();
// 系统调用

// 系统调用表
system_call sys_call_table[NR_SYS_CALL]={sys_get_ticks,sys_write};

Process* p_proc_ready;
Process proc_table[NR_TASKS+NR_PROCS];  //进程表

char task_stack[STACK_SIZE_TOTAL];

// 任务表
TASK task_table[NR_TASKS]={
	{task_tty,STACK_SIZE_TTY,"TTY"},
};

TASK user_proc_table[NR_PROCS]={
	{test1,STACK_SIZE_TESTA,"A"},
	{test2,STACK_SIZE_TESTB,"B"},
	{test3,STACK_SIZE_TESTB,"C"},
};

// TSS
TSS tss;

/*======================================================================*
  init_descriptor
 *----------------------------------------------------------------------*
 初始化段描述符
 *======================================================================*/
static void init_descriptor(Descriptor *p_desc,uint32_t base,uint32_t limit,uint16_t attribute)
{
	p_desc->limit_low	= limit & 0x0FFFF;
	p_desc->base_low	= base & 0x0FFFF;
	p_desc->base_mid	= (base >> 16) & 0x0FF;
	p_desc->attrl		= attribute & 0xFF;
	p_desc->limit_high_attr2= (((limit>>16) & 0x0F) | (attribute>>8) )& 0xF0;
	p_desc->base_high	= (base >> 24) & 0x0FF;
}

/*======================================================================*
  seg2phys
 *----------------------------------------------------------------------*
 由段名求绝对地址
 *======================================================================*/
uint32_t seg2phys(uint16_t seg)
{
	Descriptor* p_dest = &gdt[seg >> 3];
	return (p_dest->base_high<<24 | p_dest->base_mid<<16 | p_dest->base_low);
}

// 初始化进程表
void init_proc_table()
{
	TASK* p_task=task_table;
	Process* p_proc=proc_table;
	char* p_task_stack=task_stack+STACK_SIZE_TOTAL;
	uint16_t selector_ldt=SELECTOR_LDT_FIRST;

	uint8_t privilege;
	uint8_t rpl;
	uint16_t eflags;

	for (int i=0;i<NR_PROCS+NR_TASKS;++i)
	{
		// 任务(内核态)
		if (i<NR_TASKS)
		{
			p_task=task_table+i;
			privilege=PRIVILEGE_TASK;
			rpl=RPL_TASK;
			eflags=0x1202;  // IF=0,IOPL=1
		}
		// 用户进程(用户态)
		else
		{
			p_task=user_proc_table+(i-NR_TASKS);
			privilege=PRIVILEGE_USER;
			rpl=RPL_USER;
			eflags=0x202;   //IOPL=0 禁止用户进程的IO权限

		}

		strcpy(p_proc->p_name,p_task->name);
		p_proc->pid=i;
		p_proc->ldt_sel=selector_ldt;
		//拷贝GDT代码段描述符到IDT
		memcpy((uint8_t*)&p_proc->ldts[0],(uint8_t*)&gdt[SELECTOR_KERNEL_CS>>3],sizeof(Descriptor)); 
		p_proc->ldts[0].attrl=DA_C|privilege<<5; //改变DPL
		//拷贝GDT数据段描述符到IDT
		memcpy((uint8_t*)&p_proc->ldts[1],(uint8_t*)&gdt[SELECTOR_KERNEL_DS>>3],sizeof(Descriptor)); 
		p_proc->ldts[1].attrl=DA_DRW|privilege<<5; //改变DPL

		p_proc->regs.cs=(0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds=(8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es=(8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs=(8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss=(8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs=(SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

		p_proc->regs.eip=(uint32_t)p_task->initial_eip;
		p_proc->regs.esp=(uint32_t)p_task_stack;
		p_proc->regs.eflags=eflags; 

		p_task_stack-=p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1<<3;
	}
}

void init_proc()
{

	/* 填充 GDT 中 TSS 这个描述符 */
	memset(&tss, 0, sizeof(tss));
	tss.ss0 = SELECTOR_KERNEL_DS;
	init_descriptor(&gdt[INDEX_TSS],
			vir2phys(seg2phys(SELECTOR_KERNEL_DS), &tss),
			sizeof(tss) - 1,
			DA_386TSS);
	tss.iobase = sizeof(tss); /* 没有I/O许可位图 */

	Process* p_proc = proc_table;
	uint16_t selector_ldt=INDEX_LDT_FIRST << 3;    // * 8
	for (int i=0;i<NR_TASKS+NR_PROCS;++i)
	{
		/* 填充 GDT 中进程的 LDT 的描述符 */
		init_descriptor(&gdt[selector_ldt>>3],
				vir2phys(seg2phys(SELECTOR_KERNEL_DS), proc_table[i].ldts),
				LDT_SIZE * sizeof(Descriptor) - 1,
				DA_LDT);
		p_proc++;
		selector_ldt+= 1<<3; // + 8
	}

	init_proc_table();

}

// 进程调度
void schedule()
{
	Process* p;
	int greatest_ticks=0;

	while(!greatest_ticks)
	{
		for (p=proc_table;p<proc_table+NR_TASKS+NR_PROCS;++p)
		{
			if (p->ticks>greatest_ticks)
			{
				greatest_ticks=p->ticks;
				p_proc_ready=p;
			}
		}
		if (!greatest_ticks)
		{
			for (p=proc_table;p<proc_table+NR_TASKS+NR_PROCS;++p)
				p->ticks=p->priority;
		}
	}
}

/*
 * 实现IPC相关函数
 */

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

void block(struct proc* p)
{
	
}

/* 
 * ------------------
 * 系统调用 - 系统级
 * ------------------
 */

// 获得时钟
int sys_get_ticks()
{
	return ticks;
}
// IPC
int sys_sendrec(int function,int src_dest,MESSAGE* m,struct proc* p)
{
	int ret=0;
	int caller=proc2pid(p);
	MESSAGE* mla=(MESSAGE*)va2la(caller,m);
	mla->source=caller;

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

#ifndef _PROC_H
#define _PROC_H
#include "gdt.h"
#include "const.h"
#include "types.h"

#define NR_TASKS 1 // 最大任务数
#define NR_PROCS 3 // 最大用户进程数

int ticks;

typedef struct s_task
{
	task_f initial_eip;
	int stacksize;
	char name[32];
}__attribute__((packed)) TASK;

typedef struct stackframe
{
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t kernel_esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t retaddr;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
}__attribute__((packed)) StackFrame;

typedef struct proc
{
	StackFrame regs; 			//寄存器
	uint16_t ldt_sel; 			//进程对应的GDT中的LDT选择子
	Descriptor ldts[LDT_SIZE];  //进程LDT中的代码和数据段选择子

	int ticks;	// 剩下的ticks
	int priority; // 优先级

	uint32_t pid; 				//进程的id
	char p_name[16]; 			//进程名
	

}Process;

// TSS
typedef struct s_tss {
	uint32_t	backlink;
	uint32_t	esp0;	/* stack pointer to use during interrupt */
	uint32_t	ss0;	/*   "   segment  "  "    "        "     */
	uint32_t	esp1;
	uint32_t	ss1;
	uint32_t	esp2;
	uint32_t	ss2;
	uint32_t	cr3;
	uint32_t	eip;
	uint32_t	flags;
	uint32_t	eax;
	uint32_t	ecx;
	uint32_t	edx;
	uint32_t	ebx;
	uint32_t	esp;
	uint32_t	ebp;
	uint32_t	esi;
	uint32_t	edi;
	uint32_t	es;
	uint32_t	cs;
	uint32_t	ss;
	uint32_t	ds;
	uint32_t	fs;
	uint32_t	gs;
	uint32_t	ldt;
	uint16_t	trap;
	uint16_t	iobase;	/* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
}__attribute__((packed)) TSS;

void init_proc();
void init_proc_table();
uint32_t seg2phys(uint16_t seg);
extern Process proc_table[];
extern char task_stack[];
extern int8_t k_reenter;
extern Process* p_proc_ready;
// 进程表
extern TASK task_table[NR_TASKS];
// 系统调用表
extern system_call sys_call_table[];
// 系统调用
int sys_get_ticks();
// 进程调度
void schedule();
#endif 

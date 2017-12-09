#include "proc.h"
#include "gdt.h"
#include "string.h"
extern Descriptor gdt[];
void test1();

Process* p_proc_ready;
Process proc_table[NR_TASKS];  //进程表
char task_stack[STACK_SIZE_TOTAL];
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


void init_proc_table()
{
	Process* p_proc=proc_table;
	p_proc->ldt_sel=SELECTOR_LDT_FIRST;
	//拷贝GDT代码段描述符到IDT
	memcpy((uint8_t*)&p_proc->ldts[0],(uint8_t*)&gdt[SELECTOR_KERNEL_CS>>3],sizeof(Descriptor)); 
	p_proc->ldts[0].attrl=DA_C|PRIVILEGE_TASK<<5; //改变DPL
	//拷贝GDT数据段描述符到IDT
	memcpy((uint8_t*)&p_proc->ldts[1],(uint8_t*)&gdt[SELECTOR_KERNEL_DS>>3],sizeof(Descriptor)); 
	p_proc->ldts[1].attrl=DA_DRW|PRIVILEGE_TASK<<5; //改变DPL

	p_proc->regs.cs=(0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.ds=(8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.es=(8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.fs=(8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.ss=(8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
	p_proc->regs.gs=(SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;

	p_proc->regs.eip=(uint32_t)test1;
	p_proc->regs.esp=(uint32_t)task_stack+STACK_SIZE_TOTAL;
	p_proc->regs.eflags=0x1202; //IF=1 IOPL=1
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
 
	/* 填充 GDT 中进程的 LDT 的描述符 */
	init_descriptor(&gdt[INDEX_LDT_FIRST],
			vir2phys(seg2phys(SELECTOR_KERNEL_DS), proc_table[0].ldts),
			LDT_SIZE * sizeof(Descriptor) - 1,
			DA_LDT);

	init_proc_table();

}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  protect.c
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Forrest Yu, 2005
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "types.h"
#include "const.h"
#include "idt.h"
#include "debug.h"
#include "screen.h"
uint8_t		idt_ptr[6];	/* 0~15:Limit  16~47:Base */
GATE		idt[IDT_SIZE];


void init_idt_desc(unsigned char vector, uint8_t desc_type,
		int_handler handler, unsigned char privilege);



void init_8259A()
{
	/* Master 8259, ICW1. */
	outb(INT_M_CTL,	0x11);

	/* Slave  8259, ICW1. */
	outb(INT_S_CTL,	0x11);

	/* Master 8259, ICW2. 设置 '主8259' 的中断入口地址为 0x20. */
	outb(INT_M_CTLMASK,	INT_VECTOR_IRQ0);

	/* Slave  8259, ICW2. 设置 '从8259' 的中断入口地址为 0x28 */
	outb(INT_S_CTLMASK,	INT_VECTOR_IRQ8);

	/* Master 8259, ICW3. IR2 对应 '从8259'. */
	outb(INT_M_CTLMASK,	0x4);

	/* Slave  8259, ICW3. 对应 '主8259' 的 IR2. */
	outb(INT_S_CTLMASK,	0x2);

	/* Master 8259, ICW4. */
	outb(INT_M_CTLMASK,	0x1);

	/* Slave  8259, ICW4. */
	outb(INT_S_CTLMASK,	0x1);

	/* Master 8259, OCW1.  */
	outb(INT_M_CTLMASK,	0xFC); //开启键盘和时钟中断

	/* Slave  8259, OCW1.  */
	outb(INT_S_CTLMASK,	0xFF);
}

/* 中断处理函数 */
void	divide_error();
void	single_step_exception();
void	nmi();
void	breakpoint_exception();
void	overflow();
void	bounds_check();
void	inval_opcode();
void	copr_not_available();
void	double_fault();
void	copr_seg_overrun();
void	inval_tss();
void	segment_not_present();
void	stack_exception();
void	general_protection();
void	page_fault();
void	copr_error();
// 外部中断
void    hwint00();
void    hwint01();
void    hwint02();
void    hwint03();
void    hwint04();
void    hwint05();
void    hwint06();
void    hwint07();
void    hwint08();
void    hwint09();
void    hwint10();
void    hwint11();
void    hwint12();
void    hwint13();
void    hwint14();
void    hwint15();
/*======================================================================*
  init_intp_
 *======================================================================*/
void init_intp()
{

	// 全部初始化成中断门(没有陷阱门)
	init_idt_desc(INT_VECTOR_DIVIDE,	DA_386IGate,
			divide_error,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_DEBUG,		DA_386IGate,
			single_step_exception,	PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_NMI,		DA_386IGate,
			nmi,			PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_BREAKPOINT,	DA_386IGate,
			breakpoint_exception,	PRIVILEGE_USER);

	init_idt_desc(INT_VECTOR_OVERFLOW,	DA_386IGate,
			overflow,			PRIVILEGE_USER);

	init_idt_desc(INT_VECTOR_BOUNDS,	DA_386IGate,
			bounds_check,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_INVAL_OP,	DA_386IGate,
			inval_opcode,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_COPROC_NOT,	DA_386IGate,
			copr_not_available,	PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_DOUBLE_FAULT,	DA_386IGate,
			double_fault,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_COPROC_SEG,	DA_386IGate,
			copr_seg_overrun,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_INVAL_TSS,	DA_386IGate,
			inval_tss,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_SEG_NOT,	DA_386IGate,
			segment_not_present,	PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_STACK_FAULT,	DA_386IGate,
			stack_exception,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_PROTECTION,	DA_386IGate,
			general_protection,	PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_PAGE_FAULT,	DA_386IGate,
			page_fault,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_COPROC_ERR,	DA_386IGate,
			copr_error,		PRIVILEGE_KRNL);

	// 中断

	init_idt_desc(INT_VECTOR_IRQ0 + 0,      DA_386IGate,
			hwint00,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 1,      DA_386IGate,
			hwint01,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 2,      DA_386IGate,
			hwint02,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 3,      DA_386IGate,
			hwint03,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 4,      DA_386IGate,
			hwint04,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 5,      DA_386IGate,
			hwint05,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 6,      DA_386IGate,
			hwint06,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 7,      DA_386IGate,
			hwint07,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 0,      DA_386IGate,
			hwint08,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 1,      DA_386IGate,
			hwint09,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 2,      DA_386IGate,
			hwint10,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 3,      DA_386IGate,
			hwint11,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 4,      DA_386IGate,
			hwint12,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 5,      DA_386IGate,
			hwint13,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 6,      DA_386IGate,
			hwint14,                  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 7,      DA_386IGate,
			hwint15,                  PRIVILEGE_KRNL);
}

/*======================================================================*
  init_idt_desc
 *----------------------------------------------------------------------*
 初始化 386 中断门
 *======================================================================*/
void init_idt_desc(unsigned char vector, uint8_t desc_type,
		int_handler handler, unsigned char privilege)
{
	GATE *	p_gate	= &idt[vector];
	uint32_t	base	= (uint32_t)handler;
	p_gate->offset_low	= base & 0xFFFF;
	p_gate->selector	= SELECTOR_KERNEL_CS;
	p_gate->dcount		= 0;
	p_gate->attr		= desc_type | (privilege << 5);
	p_gate->offset_high	= (base >> 16) & 0xFFFF;
}

/*======================================================================*
  exception_handler
 *----------------------------------------------------------------------*
 异常处理
 *======================================================================*/
void exception_handler(int vec_no,int err_code,int eip,int cs,int eflags)
{
	char * err_msg[] = {"#DE Divide Error",
		"#DB RESERVED",
		"--  NMI Interrupt",
		"#BP Breakpoint",
		"#OF Overflow",
		"#BR BOUND Range Exceeded",
		"#UD Invalid Opcode (Undefined Opcode)",
		"#NM Device Not Available (No Math Coprocessor)",
		"#DF Double Fault",
		"    Coprocessor Segment Overrun (reserved)",
		"#TS Invalid TSS",
		"#NP Segment Not Present",
		"#SS Stack-Segment Fault",
		"#GP General Protection",
		"#PF Page Fault",
		"--  (Intel reserved. Do not use.)",
		"#MF x87 FPU Floating-Point Error (Math Fault)",
		"#AC Alignment Check",
		"#MC Machine Check",
		"#XF SIMD Floating-Point Exception"
	};
	real_color_t back_cor=rc_blue,fore_cor=rc_red;
	console_write_color("Exception! --> ",back_cor,fore_cor );
	console_write_color(err_msg[vec_no], back_cor,fore_cor);
	console_write_color("\n", back_cor,fore_cor);
	console_write_color("EFLAGS : ", back_cor,fore_cor);
	console_write_hex(eflags,rc_black,rc_white);
	console_write_color("\nCS : ", back_cor,fore_cor);
	console_write_hex(cs,rc_black,rc_white);
	console_write_color("\nEIP : ", back_cor,fore_cor);
	console_write_hex(eip,rc_black,rc_white);
	if (err_code!=0xFFFFFFFF)
	{
		console_write_color("\nError Code : ", back_cor,fore_cor);
		console_write_hex(eip,back_cor,fore_cor);
	}
	console_write_color("\n\n", back_cor,fore_cor);
}

/*======================================================================*
  exception_handler
 *----------------------------------------------------------------------*
 中断处理
 *======================================================================*/
void spurious_irq(int irq)
{
	real_color_t back_cor=rc_blue,fore_cor=rc_red;
	console_write_color("spurious_irq : ", back_cor,fore_cor);
	console_write_hex(irq,back_cor,fore_cor);
	console_write_color("\n\n", back_cor,fore_cor);
}

void init_idt()
{
	init_8259A();
	
	/* idt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sidt/lidt 的参数。*/
	uint16_t* p_idt_limit = (uint16_t*)(&idt_ptr[0]);
	uint32_t* p_idt_base  = (uint32_t*)(&idt_ptr[2]);
	*p_idt_limit = 256 * sizeof(GATE) - 1;
	*p_idt_base  = (uint32_t)&idt;

	init_intp();
	printk("IDT inited.\n");
}

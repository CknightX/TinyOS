SELECTOR_KERNEL_CS equ 8
; 导出函数
global start
global idt_flush
; 
extern idt_ptr
extern gdt_ptr
extern init_gdt
extern init_idt
extern exception_handler
extern main
; 导出中断处理函数
global	divide_error
global	single_step_exception
global	nmi
global	breakpoint_exception
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error


[BITS 32]
ALIGN 4
; 代码段
[SECTION .text]

start:
	mov ah,0fh
	mov al,'K'
	mov [gs:((80*0+39)*2)],ax

	; 堆栈从loader移动到kernel 
	mov esp,StackTop

	sgdt [gdt_ptr]
	call init_gdt
	lgdt [gdt_ptr]

	; 强制使用刚刚初始化的结构
	call init_idt
	lidt [idt_ptr]
	jmp SELECTOR_KERNEL_CS:csinit

csinit:
	call main
	int 1
	jmp $

; 中断和异常 -- 异常
divide_error:
	push	0xFFFFFFFF	; no err code
	push	0		; vector_no	= 0
	jmp	exception
single_step_exception:
	push	0xFFFFFFFF	; no err code
	push	1		; vector_no	= 1
	jmp	exception
nmi:
	push	0xFFFFFFFF	; no err code
	push	2		; vector_no	= 2
	jmp	exception
breakpoint_exception:
	push	0xFFFFFFFF	; no err code
	push	3		; vector_no	= 3
	jmp	exception
overflow:
	push	0xFFFFFFFF	; no err code
	push	4		; vector_no	= 4
	jmp	exception
bounds_check:
	push	0xFFFFFFFF	; no err code
	push	5		; vector_no	= 5
	jmp	exception
inval_opcode:
	push	0xFFFFFFFF	; no err code
	push	6		; vector_no	= 6
	jmp	exception
copr_not_available:
	push	0xFFFFFFFF	; no err code
	push	7		; vector_no	= 7
	jmp	exception
double_fault:
	push	8		; vector_no	= 8
	jmp	exception
copr_seg_overrun:
	push	0xFFFFFFFF	; no err code
	push	9		; vector_no	= 9
	jmp	exception
inval_tss:
	push	10		; vector_no	= A
	jmp	exception
segment_not_present:
	push	11		; vector_no	= B
	jmp	exception
stack_exception:
	push	12		; vector_no	= C
	jmp	exception
general_protection:
	push	13		; vector_no	= D
	jmp	exception
page_fault:
	push	14		; vector_no	= E
	jmp	exception
copr_error:
	push	0xFFFFFFFF	; no err code
	push	16		; vector_no	= 10h
	jmp	exception
; 中断服务程序
exception:
	call exception_handler
	add esp,4*2
	hlt

[SECTION .bss]
StackSpace resb 2*1024
StackTop: ;栈顶


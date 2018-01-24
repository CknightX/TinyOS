%include "sconst.inc"
jmp start

ALIGN 4
; 导出函数
global start
global idt_flush
global restart
; 导入函数 
extern idt_ptr
extern gdt_ptr
extern init_gdt
extern init_idt
extern init_proc
extern exception_handler
extern spurious_irq 
extern _osmain
extern printk
extern delay
extern clock_handler
; 导入变量
extern tss
extern k_reenter
extern irq_table
extern sys_call_table

; 导出异常处理函数
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
; 硬件中断
global  hwint00
global  hwint01
global  hwint02
global  hwint03
global  hwint04
global  hwint05
global  hwint06
global  hwint07
global  hwint08
global  hwint09
global  hwint10
global  hwint11
global  hwint12
global  hwint13
global  hwint14
global  hwint15

global sys_call

[BITS 32]
; 堆栈段
[SECTION .bss]
StackSpace resb 2*1024
StackTop: ;栈顶

; 数据段
[SECTION .data]
clock_int_msg db '^',0

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
	jmp SELECTOR_KERNEL_CS:csinit ;这里为什么不用+kernelphyaddr？

csinit:
	;sti
	call init_proc
	xor eax,eax
	mov ax,SELECTOR_TSS ; TSS 选择子
	ltr ax
	jmp _osmain ;直接跳到main执行



; 中断和异常 -- 硬件中断
; ---------------------------------
%macro  hwint_master    1
	call save
	; 屏蔽当前中断
	in al,INT_M_CTLMASK
	or al,(1 << %1) 
	out INT_M_CTLMASK,al
	; 置EOI
	mov al,EOI 
	out INT_M_CTL,al
	; 中断处理程序
	sti
	push %1
	call [irq_table+4 * %1]
	pop ecx
	cli
	; 允许中断
	in al,INT_M_CTLMASK
	and al, ~(1 << %1)
	out INT_M_CTLMASK,al
	ret
%endmacro
; ---------------------------------

save:
	pushad
	push ds
	push es
	push fs
	push gs

	mov esi,edx

	mov dx,ss
	mov ds,dx
	mov es,dx
	mov fs,dx

	mov edx,esi

	mov esi,esp ; 进程表起始地址
	inc byte [k_reenter]
	cmp byte [k_reenter],0
	jne .reenter
	mov esp,StackTop  ; 切换到内核栈
	push restart
	jmp [esi+RETADR-P_STACKBASE] ; 返回到call save的下一条语句执行

.reenter:
	push restart_reenter
	jmp [esi+RETADR-P_STACKBASE] 


ALIGN   16
hwint00:                ; Interrupt routine for irq 0 (the clock).
		hwint_master 	0
	
ALIGN   16
hwint01:                ; Interrupt routine for irq 1 (keyboard)
        hwint_master    1

ALIGN   16
hwint02:                ; Interrupt routine for irq 2 (cascade!)
        hwint_master    2

ALIGN   16
hwint03:                ; Interrupt routine for irq 3 (second serial)
        hwint_master    3

ALIGN   16
hwint04:                ; Interrupt routine for irq 4 (first serial)
        hwint_master    4

ALIGN   16
hwint05:                ; Interrupt routine for irq 5 (XT winchester)
        hwint_master    5

ALIGN   16
hwint06:                ; Interrupt routine for irq 6 (floppy)
        hwint_master    6

ALIGN   16
hwint07:                ; Interrupt routine for irq 7 (printer)
        hwint_master    7

; ---------------------------------
%macro  hwint_slave     1
        push    %1
        call    spurious_irq
        add     esp, 4
        hlt
%endmacro
; ---------------------------------

ALIGN   16
hwint08:                ; Interrupt routine for irq 8 (realtime clock).
        hwint_slave     8

ALIGN   16
hwint09:                ; Interrupt routine for irq 9 (irq 2 redirected)
        hwint_slave     9

ALIGN   16
hwint10:                ; Interrupt routine for irq 10
        hwint_slave     10

ALIGN   16
hwint11:                ; Interrupt routine for irq 11
        hwint_slave     11

ALIGN   16
hwint12:                ; Interrupt routine for irq 12
        hwint_slave     12

ALIGN   16
hwint13:                ; Interrupt routine for irq 13 (FPU exception)
        hwint_slave     13

ALIGN   16
hwint14:                ; Interrupt routine for irq 14 (AT winchester)
        hwint_slave     14

ALIGN   16
hwint15:                ; Interrupt routine for irq 15
        hwint_slave     15

;  异常处理
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
; 进程的入口
restart:
	extern p_proc_ready
	mov esp,[p_proc_ready] ;进程表
	lldt [esp+P_LDT_SEL]   ;装载LDT
	lea eax,[esp+P_STACKTOP] ;计算栈顶地址
	mov dword [tss+TSS3_S_SP0],eax ; 将ring0的栈顶保存到tss ring0处
restart_reenter:
	dec byte [k_reenter]
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp,4
	iretd


; 系统调用中断
sys_call:
	call save

	sti
	push esi

	push dword [p_proc_ready] ; 哪一个进程引发的中断
	push edx
	push ecx
	push ebx
	call [sys_call_table+eax*4]
	add esp,4*4

	pop esi
	mov [esi+EAXREG-P_STACKBASE],eax
	cli
	ret

SELECTOR_KERNEL_CS equ 8


[BITS 32]
ALIGN 4
; 导出start
global start
extern gdt_ptr
extern init_gdt
extern main

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
	jmp dword SELECTOR_KERNEL_CS:csinit

csinit:

	call main
	jmp $

[SECTION .data]

[SECTION .rdata]

[SECTION .bss]
StackSpace resb 2*1024
StackTop: ;栈顶


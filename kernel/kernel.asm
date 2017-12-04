[BITS 32]
ALIGN 4
[GLOBAL start]
[SECTION .text]
start:
	;mov ah,0fh
	;mov al,'K'
	;mov [gs:((80*0+39)*2)],ax
	extern main
	call main
	jmp $

[SECTION .data]

[SECTION .rdata]

[SECTION .bss]

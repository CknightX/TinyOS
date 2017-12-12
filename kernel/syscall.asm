%include "sconst.inc"

_NR_get_ticks equ 0
INT_VECTOR_SYS_CALL equ 0x90

global get_ticks

[BITS 32]
[SECTION .text]
get_ticks:
	mov eax,_NR_get_ticks
	int INT_VECTOR_SYS_CALL
	ret

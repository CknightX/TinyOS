[SECTION .text]
[BITS 32]
start:
	mov ah,0fh
	mov al,'K'
	mov [gs:((80*0+39)*2)],ax
	jmp $

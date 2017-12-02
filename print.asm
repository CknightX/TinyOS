
CLEAR: ;清屏
	push ax
	push bx
	push cx
	push dx
	mov ax,0600h
	mov bx,0700h
	mov cx,0
	mov dx,0184fh
	int 10h
	mov ax,0200h ;置光标位置为屏幕左上角
	mov bh,0
	mov dh,0
	mov dl,0
	int 10h
	pop dx
	pop cx
	pop bx
	pop ax
	ret

PRT_MSG: ;输出si指向的字符串
	push ax
	push bx
loop:
	lodsb ; ds:si->al
	test al,al
	je done ;al=0
	mov ah,0x0e
	mov bx,0007h
	int 0x10
	jmp loop
done:
	pop ax
	pop bx
	ret



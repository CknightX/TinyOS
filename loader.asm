org 0100h

		mov ax,0B800h
		mov gs,ax
		mov ah,0fh
		mov al,'F'
		mov [gs:((80*0+39)*2)],ax  ;屏幕第0行，39列

		jmp $
; TODO 加载GDT 进入保护模式

org 0100h

; TODO 加载GDT 进入保护模式
jmp LABEL_BEGIN

%include "pm.inc"
; GDT段
[SECTION .gdt]
LABEL_GDT:				Descriptor     0,0,0     					;空描述符
LABEL_DESC_CODE32:		Descriptor     0,SegCode32Len-1,DA_C+DA_32  ;非一致32位代码段
LABEL_DESC_DATA32:      Descriptor     0,SegData32Len-1,DA_DRW      ;32位数据段
LABEL_DESC_STACK:		Descriptor	   0,TopofStack,DA_DRWA+DA_32   ;32位堆栈段
LABEL_DESC_VIDEO:		Descriptor	   0xb8000,0xffff,DA_DRW		;显存

GdtLen equ $-LABEL_GDT  ; GDT长度(占用字节数)
GdtPtr dw  GdtLen-1     ; GDT界限
	   dd  0			; GDT基地址


; GDT 选择子
SelectorData32 equ LABEL_DESC_DATA32-LABEL_GDT   ; 32位数据段选择子
SelectorCode32 equ LABEL_DESC_CODE32-LABEL_GDT   ; 代码段选择子
SelectorStack  equ LABEL_DESC_STACK-LABEL_GDT    ; 堆栈段选择子
SelectorVideo  equ LABEL_DESC_VIDEO-LABEL_GDT    ; 显存段选择子
; END of [SECTION .gdt]

; 数据段
[SECTION .data1]
ALIGN 32
[BITS 32]
LABEL_SEG_DATA32:
PMMessage: db "In Protect mode now :)",0
OffsetPMMessage equ PMMessage-$$

SegData32Len equ $-LABEL_SEG_DATA32

; 全局堆栈段
[SECTION .gs]
ALIGN 32
[BITS 32]
LABEL_STACK:
times 512 db 0
TopofStack equ $-LABEL_STACK-1
; END of [SECTION .gs]


; 16位代码段
[SECTION .s16]
[BITS 16] 

LABEL_BEGIN:
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,0x0100  ; 栈底

	; 初始化32位代码描述符
	xor eax,eax
	mov ax,cs   ; 代码段地址
	shl eax,4	; *16
	add eax,LABEL_SEG_CODE32 ; +offset
	; eax中现在为LABEL_SEG_CODE32处的物理地址
	mov word [LABEL_DESC_CODE32+2],ax
	shr eax,16
	mov byte [LABEL_DESC_CODE32+4],al
	mov byte [LABEL_DESC_CODE32+7],ah

	; 初始化32位数据描述符
	xor eax,eax
	mov ax,ds   ; 代码段地址
	shl eax,4	; *16
	add eax,LABEL_SEG_DATA32 ; +offset
	mov word [LABEL_DESC_DATA32+2],ax
	shr eax,16
	mov byte [LABEL_DESC_DATA32+4],al
	mov byte [LABEL_DESC_DATA32+7],ah

	;为加载GDTR作准备
	xor eax,eax
	mov ax,ds
	shl eax,4
	add eax,LABEL_GDT         ; GDT基地址-->eax
	mov dword [GdtPtr+2],eax  ; GDT基地址-->[GdtPtr+2]

	; 加载GDTR
	lgdt [GdtPtr]

	; 关中断
	cli

	; 打开地址线A20
	in al,0x92
	or al,00000010b
	out 0x92,al

	; 准备切换到保护模式
	mov eax,cr0
	or eax,1
	mov cr0,eax

	; 进入保护模式
	jmp dword SelectorCode32:0 ; 将SelectorCode32 装入cs
; END of [SECTION .s16]


; 以下为32位代码段
[SECTION .s32]
[BITS 32]
LABEL_SEG_CODE32:
	; 视频段选择子
	mov ax,SelectorVideo
	mov gs,ax
	; 数据段选择子
	mov ax,SelectorData32
	mov ds,ax
	; 堆栈段选择子
	mov ax,SelectorStack
	mov ss,ax 

	call PRINT_PMMODE
	jmp $  ; 到此结束


; 输出函数
PRINT_PMMODE:
	mov edi,(80*1+0)*2   ;2行0列
	mov ah,0x0c

	mov esi,OffsetPMMessage
	loop:
	mov al,[esi]
	test al,al
	je end
	mov [gs:edi],ax
	add esi,1
	add edi,2
	jmp loop
end:
	ret

SegCode32Len equ $-LABEL_SEG_CODE32
; End of [SECTION .s32]
















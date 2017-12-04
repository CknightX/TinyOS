org 0100h

; TODO 加载GDT 进入保护模式
jmp LABEL_BEGIN

; GDT段
%include "load.inc"

FLAT_C_ATTR  equ DA_CR | DA_32 | DA_LIMIT_4K
FLAT_RW_ATTR equ DA_DRW | DA_32 | DA_LIMIT_4K
VIDEO_ATTR   equ DA_DRW | DA_DPL3

%include "pm.inc"

[SECTION .gdt]
LABEL_GDT:				Descriptor     0,0,0     				   ; 空描述符
LABEL_DESC_FLAT_C:		Descriptor     0,0xfffff,FLAT_C_ATTR       ; 0-4G 可执行段
LABEL_DESC_FLAT_RW:     Descriptor     0,0xfffff,FLAT_RW_ATTR      ; 0-4G 可读写段
LABEL_DESC_VIDEO:		Descriptor	   0xb8000,0xffff,VIDEO_ATTR   ; 显存

GdtLen equ $-LABEL_GDT  ; GDT长度(占用字节数)
GdtPtr dw  GdtLen-1     ; GDT界限
	   dd  BaseOfLoaderPhyAddr+LABEL_GDT			; GDT基地址


; GDT 选择子
SelectorData equ LABEL_DESC_FLAT_RW-LABEL_GDT   ; 32位数据段选择子
SelectorCode equ LABEL_DESC_FLAT_C-LABEL_GDT   ; 代码段选择子
SelectorVideo  equ LABEL_DESC_VIDEO-LABEL_GDT    ; 堆栈段选择子

BaseofStack equ 0100h
PageDirBase equ 100000h
PageTblBase equ 101000h


; 16位代码段
[SECTION .s16]
[BITS 16] 

LABEL_BEGIN:
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,0x0100  ; 栈底
	call READ_KERNEL
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
	jmp dword SelectorCode:(BaseOfLoaderPhyAddr+LABEL_SEG_CODE32) ; 将SelectorCode32 装入cs

READ_KERNEL:
	mov bx,0
	mov ax,BaseOfKernelFile
	mov es,ax      ; 从磁盘将数据读入es:bx指向的缓冲区中
	mov ch,1 ; 柱面为0
	mov dh,0 ; 磁头为1
	mov cl,2 ; 起始扇区号为17

READ_SECTOR:
mov ah,0x02 ; 读取模式
mov al,2 ; 读2个扇区
mov dl,0x00 ; A盘
int 0x13

jnc READ_SUCCESS ;cf=0 代表成功读取
jmp READ_SECTOR
READ_SUCCESS:
	ret	
; END of [SECTION .s16]


; 以下为32位代码段
[SECTION .s32]
[BITS 32]
LABEL_SEG_CODE32:
	; 视频段选择子
	mov ax,SelectorVideo
	mov gs,ax
	; 数据段选择子
	mov ax,SelectorData
	mov ds,ax

	call PRINT_PM

	call SetupPageing
	call PRINT_PAGE

	; 跳转到内核
	jmp SelectorCode:BaseOfKernelPhyAddr

; 启动分页机制,所有线性地址都等于物理地址
SetupPageing:
	; 初始化页目录
	mov ax,SelectorData
	mov es,ax
	mov ecx,1024 ; 1024个表项
	mov edi,PageDirBase ;首地址
	xor eax,eax
	mov eax,PageTblBase|PG_P|PG_USU|PG_RWW
.1:
	stosd ; eax-->es:edi
	add eax,4096 ; 页表在内存中连续，跳到下一个页表的起始地址
	loop .1


	; 初始化所有页表
	mov ecx,1024*1024 ; 1M个页表项
	mov edi,PageTblBase
	xor eax,eax
	mov eax,PG_P|PG_USU|PG_RWW ; 从内存地址0开始
.2:
	stosd ; eax-->es:edi
	add eax,4096 ; 每一个页指向4K的空间
	loop .2

	; 正式启动分页机制
	mov eax,PageDirBase
	mov cr3,eax ; cr3指向页目录表
	mov eax,cr0
	or eax,0x8000000
	mov cr0,eax
	jmp short .3
.3:
	nop
	ret
	



; 输出函数
PRINT_MSG:
	;mov edi,(80*1+0)*2   ;2行0列
	mov ah,0x0c

	;mov esi,BaseOfLoaderPhyAddr+MSG
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
PRINT_PM:
	mov edi,(80*1+0)*2
	mov esi,BaseOfLoaderPhyAddr+PM_MSG
	call PRINT_MSG
	ret
PRINT_PAGE:
	mov edi,(80*2+0)*2
	mov esi,BaseOfLoaderPhyAddr+PAGE_MSG
	call PRINT_MSG
	ret

SegCode32Len equ $-LABEL_SEG_CODE32
; End of [SECTION .s32]

[section .data1]
PM_MSG: db "In protect mode now!",0
PAGE_MSG: db "Page Setuped!",0


; 栈放在数据区末端
StackSpace: times 1024 db 0
TopofStack equ BaseOfLoaderPhyAddr+$
; End of [secion .data1]

org 07c00h
jmp short BOOT_START
nop ; 占位

	; FAT12 磁盘头
	BS_OEMName	DB 'CKTINYOS'	; OEM String, 必须 8 个字节
	BPB_BytsPerSec	DW 512		; 每扇区字节数
	BPB_SecPerClus	DB 1		; 每簇多少扇区
	BPB_RsvdSecCnt	DW 1		; Boot 记录占用多少扇区
	BPB_NumFATs	DB 2			; 共有多少 FAT 表
	BPB_RootEntCnt	DW 224		; 根目录文件数最大值
	BPB_TotSec16	DW 2880		; 逻辑扇区总数
	BPB_Media	DB 0xF0			; 媒体描述符
	BPB_FATSz16	DW 9			; 每FAT扇区数
	BPB_SecPerTrk	DW 18		; 每磁道扇区数
	BPB_NumHeads	DW 2		; 磁头数(面数)
	BPB_HiddSec	DD 0			; 隐藏扇区数
	BPB_TotSec32	DD 0		; wTotalSectorCount为0时这个值记录扇区数
	BS_DrvNum	DB 0			; 中断 13 的驱动器号
	BS_Reserved1	DB 0		; 未使用
	BS_BootSig	DB 29h			; 扩展引导标记 (29h)
	BS_VolID	DD 0			; 卷序列号
	BS_VolLab	DB "FLOPPYCK   "; 卷标, 必须 11 个字节
	BS_FileSysType	DB "FAT12   "; 文件系统类型, 必须 8个字节  

	; 磁盘头设置好后，Linux就可以识别该磁盘，可以进行挂载等操作

BOOT_START:
		call CLEAR
		mov si,BOOT_MSG
		call PRT_MSG
		mov ax,cs
		mov ds,ax
		mov es,ax
		mov ss,ax
		mov sp,100h

; 加载LOADER到内存中
; 这里通过对磁盘的二进制分析，发现loader被放到了第二个目录项，即从第三个簇开始存放。
; 对应的扇区号为34
LOAD_LOADER:
		mov bx,0100h
		mov ax,09000h
		mov es,ax      ; 从磁盘将数据读入es:bx指向的缓冲区中
		mov ch,0 ; 柱面为0
		mov dh,1 ; 磁头为1
		mov cl,17 ; 起始扇区号为17

READ_SECTOR:
mov ah,0x02 ; 读取模式
mov al,2 ; 读2个扇区
mov dl,0x00 ; A盘
int 0x13

jnc READ_SUCCESS ;cf=0 代表成功读取

READ_SUCCESS:
jmp 09000h:0100h

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



BOOT_MSG:
db "BOOTING..."
db 0ah ;\n
db 0dh ;\r
db 0

resb 510-($-$$)   	;剩余空间填充

db 0x55,0xaa 		;结束标志



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
mov ax,cs
mov ds,ax
mov es,ax
call PRT_MSG
jmp $


PRT_MSG:
		mov ax,BOOT_MSG
		mov bp,ax
		mov cx,13  			;串长度
		mov ax,01301h 		;AH=13h ,AL=01h
		mov bx,0xc
		mov dl,0
		int 10h
		ret


BOOT_MSG:
db "BOOT LOADED!"
db 0x0a ;\n

resb 510-($-$$)   	;剩余空间填充

db 0x55,0xaa 		;结束标志



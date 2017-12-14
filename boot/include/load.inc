BaseOfLoader equ 0x9000
OffsetOfLoader equ 0x100

BaseOfLoaderPhyAddr equ BaseOfLoader*0x10

BaseOfKernelFile equ 0x4000
OffsetOfKernelFile equ 0x0

BaseOfKernelPhyAddr equ BaseOfKernelFile * 16

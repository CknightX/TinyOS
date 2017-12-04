ASM = nasm
CC = gcc
VM = bochs
LD = ld

TARGET = boot.bin loader.bin kernel.bin
OBJS = kernel.o main.o
CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c
LD_FLAGS = -T link.ld -nostdlib



all : clean buildimage

run : 
	$(VM)	

disp_data : 
	xxd -u -a -g 1 -c 16 -s +0x4200 -l 0x11 a.img
disp_root : 
	xxd -u -a -g 1 -c 16 -s +0x2600 -l 512 a.img


buildimage : $(TARGET)
	bximage a.img -fd -size=1.44 -q
	dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy
	sudo cp loader.bin /mnt/floppy/ -v
	sudo cp kernel.bin /mnt/floppy/ -v
	sudo umount /mnt/floppy


loader.bin : loader.asm
	$(ASM) $< -o $@


boot.bin :  boot.asm
	$(ASM) $< -o $@

kernel.o : kernel.asm
	$(ASM) $< -o $@ -f elf
main.o : main.c 
	$(CC) $< -o $@ $(CFLAGS)
kernel.bin : $(OBJS)
	$(LD) $(OBJS) -o $@ $(LD_FLAGS)



clean : 
	rm *.bin -f
	rm *.o -f
	rm *.img -f

ASM = nasm
CC = gcc
VM = bochs
LD = ld

TARGET = bin/boot.bin bin/loader.bin bin/kernel.bin
OBJS = kernel/kernel.o kernel/main.o
CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c
LD_FLAGS = -T script/link.ld -nostdlib



all : clean buildimage

run : 
	$(VM) -f ./config/bochsrc

disp_data : 
	xxd -u -a -g 1 -c 16 -s +0x4200 -l 0x11 a.img
disp_root : 
	xxd -u -a -g 1 -c 16 -s +0x2600 -l 512 a.img


buildimage : $(TARGET)
	bximage a.img -fd -size=1.44 -q
	dd if=bin/boot.bin of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy
	sudo cp bin/loader.bin /mnt/floppy/ -v
	sudo cp bin/kernel.bin /mnt/floppy/ -v
	sudo umount /mnt/floppy

bin/kernel.bin : $(OBJS)
	$(LD) $(OBJS) -o $@ $(LD_FLAGS)

bin/loader.bin : boot/loader.asm
	$(ASM) $< -o $@ -I boot/include/

bin/boot.bin :  boot/boot.asm
	$(ASM) $< -o $@

kernel/kernel.o : kernel/kernel.asm
	$(ASM) $< -o $@ -f elf

kernel/main.o : kernel/main.c 
	$(CC) $< -o $@ $(CFLAGS)



clean : 
	rm ./bin/*.bin -f
	rm ./kernel/*.o -f
	rm *.img -f

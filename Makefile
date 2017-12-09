ASM = nasm
CC = gcc -I include/
VM = bochs
LD = ld

CFLAGS = -Wall -Werror -nostdinc -fno-builtin -fno-stack-protector -funsigned-char \
		 		 -finline-functions -finline-small-functions -findirect-inlining \
				 		 -finline-functions-called-once -m32 -g -c

TARGET = bin/boot.bin bin/loader.bin bin/kernel.bin
OBJS = kernel/kernel.o kernel/main.o kernel/screen.o kernel/common.o kernel/string.o\
	   kernel/printk.o kernel/gdt.o kernel/idt.o kernel/proc.o

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
	$(ASM) $< -o $@ -f elf -I include/

kernel/main.o : kernel/main.c 
	$(CC) $< -o $@ $(CFLAGS)

kernel/screen.o : kernel/screen.c
	$(CC) $< -o $@ $(CFLAGS)

kernel/common.o : kernel/common.c
	$(CC) $< -o $@ $(CFLAGS)

kernel/string.o : kernel/string.c
	$(CC) $< -o $@ $(CFLAGS)

kernel/printk.o : kernel/printk.c
	$(CC) $< -o $@ $(CFLAGS)

kernel/gdt.o : kernel/gdt.c
	$(CC) $< -o $@ $(CFLAGS)

kernel/idt.o : kernel/idt.c
	$(CC) $< -o $@ $(CFLAGS)
	
kernel/proc.o : kernel/proc.c
	$(CC) $< -o $@ $(CFLAGS)
	


debug : 
	qemu -S -s -fda a.img -boot a &
	sleep 1
	cgdb -x script/gdbinit


clean : 
	rm ./bin/*.bin -f
	rm ./kernel/*.o -f
	rm *.img -f

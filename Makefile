ASM = nasm
CC = gcc
VM = bochs
LD = ld

TARGET=boot.bin loader.bin



all : buildimage

run : 
	$(VM)	

disp_data : 
	xxd -u -a -g 1 -c 16 -s +0x4200 -l 0x11 a.img
disp_root : 
	xxd -u -a -g 1 -c 16 -s +0x2600 -l 512 a.img


buildimage : $(TARGET)
	dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy
	sudo cp loader.bin /mnt/floppy/ -v
	sudo umount /mnt/floppy


loader.bin : loader.asm
	$(ASM) $< -o $@


boot.bin :  boot.asm
	$(ASM) $< -o $@


clean : 
	rm *.bin

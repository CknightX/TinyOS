all : buildimage

run : 
	bochs

buildimage : boot.bin loader.bin
	dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy
	sudo cp loader.bin /mnt/floppy/ -v
	sudo umount /mnt/floppy

loader.bin : loader.asm
	nasm loader.asm -o loader.bin


boot.bin :  boot.asm
	nasm boot.asm -o boot.bin


clean : 
	rm *.bin

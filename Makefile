all : boot.bin
	dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc

run : 
	bochs

clean : 
	rm *.bin

boot.bin : 
	nasm boot.asm -o boot.bin

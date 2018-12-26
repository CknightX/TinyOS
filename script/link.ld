OUTPUT_FORMAT("binary")
ENTRY(start)
	phys = 0x10000;
	SECTIONS
{
	. = phys;
	PROVIDE( kernel_start = . );
	.text : AT(phys) {
		code = .;
		*(.text)
		. = ALIGN(4096); 
	}
	.data : AT(phys + (data - code)){
		data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096); 
	}
	.bss : AT(phys + (bss - code)){
		bss = .;
		*(.bss)
		. = ALIGN(4096); 
	}

	PROVIDE( kernel_end = . );

}


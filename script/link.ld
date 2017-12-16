OUTPUT_FORMAT("binary")
ENTRY(start)
	phys = 0x40000;
	SECTIONS
{
	. = phys;
	PROVIDE( kernel_start = . );
	.text : {
		code = .;
		*(.text)
			/*. = ALIGN(4096); */
	}
	.data : {
		data = .;
		*(.data)
			*(.rodata)
			/*. = ALIGN(4096); */
	}
	.bss : {
		bss = .;
		*(.bss)
			/*. = ALIGN(4096); */
	}

	PROVIDE( kernel_end = . );
}


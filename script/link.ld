OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x80000;
SECTIONS
{

	. = phys;

	.text :
	{
		code = .;
		*(.text)
		. = ALIGN(4096);
	}

	.data :
	{
		data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}

	.bss : 
	{
	
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}
	end = .;
}

OUTPUT_FORMAT("binary")
ENTRY(start)
SECTIONS
{
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

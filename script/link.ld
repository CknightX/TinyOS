OUTPUT_FORMAT("binary")
ENTRY(start)
	phys = 0x80000;
	SECTIONS
{

	. = phys;

	.text :
	{
		*(.text)
			. = ALIGN(4096);
	}

	.data :
	{
		*(.data)
			*(.rodata)
			. = ALIGN(4096);
	}

	.bss : 
	{

		*(.bss)
			. = ALIGN(4096);
	}
	end = .;
}

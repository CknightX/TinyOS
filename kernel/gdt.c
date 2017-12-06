#include "gdt.h"
#include "string.h"
#include "const.h"
#include "debug.h"
GdtPtr gdt_ptr;
Descriptor gdt[GDT_SIZE];

void init_gdt()
{
	console_clear();
	// 将loader中的GDT复制到新的GDT中
	memcpy((uint8_t*)&gdt,(uint8_t*)gdt_ptr.base,(uint32_t)gdt_ptr.limit+1);

	gdt_ptr.base=(uint32_t)&gdt;
	gdt_ptr.limit=GDT_SIZE*sizeof(Descriptor)-1;
	console_write("\nGDT inited\n");

}
/*
uint8_t		gdt_ptr[6];	
Descriptor	gdt[GDT_SIZE];

void init_gdt()
{

	printk("%d\n",123456);

	memcpy((uint8_t*)gdt,
			(uint8_t*)(*((uint32_t*)(&gdt_ptr[2]))),    
			*((uint16_t*)(&gdt_ptr[0])) + 1	
		  );
	for (int i=0;i<4;++i)
	{
		printk("gdt:%d%d\n",gdt[i].base_high,gdt[i].base_low);
	}

	uint16_t* p_gdt_limit = (uint16_t*)(&gdt_ptr[0]);
	uint32_t* p_gdt_base  = (uint32_t*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(Descriptor) - 1;
	*p_gdt_base  = (uint32_t)gdt;
}
*/

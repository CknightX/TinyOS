#include "pmm.h"
#include "debug.h"
void test()
{
}
void show_mmap()
{
	uint32_t MCR_count=*(uint32_t*)0x400;
	struct mmap_entry* mmap_arr=(struct mmap_entry*)0x500;
	uint32_t memsize=0;
	int i=0;
	for (i=0;i<MCR_count;++i)
	{
		printk("base addr=0x%X%08X  ",mmap_arr[i].base_addr_high,mmap_arr[i].base_addr_low);
		printk("length=0x%X%08X  ",mmap_arr[i].len_high,mmap_arr[i].len_low);
		printk("type: :0X%08X\n",mmap_arr[i].type);
		if (mmap_arr[i].type==ADDR_RANGE_MEMORY
				&& mmap_arr[i].base_addr_low+mmap_arr[i].len_low>memsize)
			memsize=mmap_arr[i].base_addr_low+mmap_arr[i].len_low;
	}
	printk("memory size: %dMB\n",memsize/(1024*1024)+1);
}

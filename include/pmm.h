#ifndef _PMM_H
#define _PMM_H
#include "types.h"

#define ADDR_RANGE_MEMORY 1
#define ADDR_RANGE_RESERVED 2
#define ADDR_RANGE_UNDEFINE 3
struct mmap_entry
{
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t len_low;
	uint32_t len_high;
	uint32_t type;
}__attribute__((packed));

void show_mmap();

#endif

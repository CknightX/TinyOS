#ifndef _GDT_h
#define _GDT_h
#include "types.h"

typedef struct _Descriptor
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t attrl;
	uint8_t limit_high_attr2;
	uint8_t base_high;

}__attribute__((packed)) Descriptor;

typedef struct _GdtPtr
{
	uint16_t limit;
	uint32_t base;

}__attribute__((packed)) GdtPtr;

#endif

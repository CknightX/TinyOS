#ifndef _TYPES_H
#define _TYPES_H
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

typedef void (*task_f)();
typedef void (*irq_handler)(int irq);
typedef void* system_call;




#endif

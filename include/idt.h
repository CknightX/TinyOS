

#ifndef	_IDT_H
#define	_IDT_H
#include "const.h"
#include "types.h"

#define CLOCK_IRQ 0
#define KEYBOARD_IRQ 1
#define INT_VECTOR_SYS_CALL 0x90

int8_t k_reenter;

void enable_irq (int irq);
void disable_irq (int irq);
void set_irq_handler(int irq,irq_handler handler);

typedef	void	(*int_handler)	();

void 	init_idt();
void	init_prot();
void	init_8259A();

/* 存储段描述符/系统段描述符 */
typedef struct s_descriptor		/* 共 8 个字节 */
{
	uint16_t	limit_low;		/* Limit */
	uint16_t	base_low;		/* Base */
	uint8_t	base_mid;		/* Base */
	uint8_t	attr1;			/* P(1) DPL(2) DT(1) TYPE(4) */
	uint8_t	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	uint8_t	base_high;		/* Base */
}__attribute__((packed)) DESCRIPTOR;

/* 门描述符 */
typedef struct s_gate
{
	uint16_t	offset_low;	/* Offset Low */
	uint16_t	selector;	/* Selector */
	uint8_t	dcount;		/* 该字段只在调用门描述符中有效。如果在利用
				   调用门调用子程序时引起特权级的转换和堆栈
				   的改变，需要将外层堆栈中的参数复制到内层
				   堆栈。该双字计数字段就是用于说明这种情况
				   发生时，要复制的双字参数的数量。*/
	uint8_t	attr;		/* P(1) DPL(2) DT(1) TYPE(4) */
	uint16_t	offset_high;	/* Offset High */
}__attribute__((packed)) GATE;









#endif /* _ORANGES_PROTECT_H_ */

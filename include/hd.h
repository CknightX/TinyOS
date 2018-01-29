#ifndef _HD_H
#define _HD_H
// Device寄存器操作字合成
#define MAKE_DEVICE_REG(lba,drv,lba_highest) (((lba)<<6) | \
											  ((drv)<<4) | \
								`			  (lba_highest & 0xf) | 0xa0)

void task_hd();
void init_hd();
void hd_identify();
void print_identify_info();
void hd_cmd_out();
void interrupt_wait();
void waitfor();
void hd_handler();
#endif

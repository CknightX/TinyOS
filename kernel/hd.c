#include "message.h"
#include "debug.h"
#include "ipc.h"
#include "idt.h"
#include "types.h"
void task_hd()
{
	MESSAGE msg;
	init_hd();
	while(1)
	{
		send_recv(RECEIVE,ANY,&msg);
		int src=msg.source;
		switch(msg.type)
		{
			case DEV_OPEN:
				hd_identify(0);
				break;
			default:
		}
		send_recv(SEND,src,&msg);
	}

}
// 初始化硬盘
void init_hd()
{
	uint8_t* pNrDrives=(uint8_t*)(0x475); // 获取系统内硬盘数量
	printl("NrDrives: %d.\n",*pNrDrives);
	assert(*pNrDrives);
	put_irq_handler(AT_WINI_IRQ,hd_handler);
}
// 获取硬盘参数
void hd_identify(int drive)
{
	struct hd_cmd cmd;
	cmd.device=MAKE_DEVICE_REG(0,drive,0);
	cmd.command=ATA_IDENTIFY;
	hd_cmd_out(&cmd); // 向硬盘驱动器发送命令
	interrupt_wait(); // 等待中断发生
	port_read(REG_DATA,hdbuf,SECTOR_SIZE); // 从DATA端口读取参数
	print_identify_info((uint16_t*)hdbuf);
}

void print_identify_info(uint16_t* hdinfo)
{
	
}

void hd_cmd_out(struct hd_cmd* cmd)
{
	
}
// 等待中断的发生
void interrupt_wait()
{
	MESSAGE msg;
	send_recv(RECEIVE,INTERRUPT,&msg);
}

int waitfor(int mask,int val,int timeout)
{
	int t=get_ticks();
	while(((get_ticks()-t)*1000/HZ)<timeout)
	{
		
	}
	return 0;
}

// 硬盘中断处理程序
void hd_handler(int irq)
{
	hd_status=in_byte(REG_STATUS);
	inform_int(TASK_HD);
}

#include "screen.h"
static uint16_t* video_memory=(uint16_t*)0xb8000;
static uint8_t cursor_x=0;
static uint8_t cursor_y=0;

// 改变光标位置
static void move_cursor()
{
	uint16_t cursor_loc=cursor_y*80+cursor_x;
	outb(0x3D4, 14);                    // 告诉 VGA 我们要设置光标的高字节
	outb(0x3D5,cursor_loc>> 8);   // 发送高 8 位
	outb(0x3D4, 15);                    // 告诉 VGA 我们要设置光标的低字节
	outb(0x3D5,cursor_loc);        // 发送低 8 位
}

// 屏幕滚动
static void scroll()
{
	uint8_t attr_byte=(0<<4)|(15&0x0f); //颜色
	uint16_t blank = 0x20 | (attr_byte<<8);  //高位为颜色，低位为字符

	if (cursor_y>=25) //光标到达屏幕最下端
	{
		// 将所有行上移动，第一行被覆盖
		int i;
		for(i=0*80;i<24*80;++i)
			video_memory[i]=video_memory[i+80];
		// 擦除最后一行
		for (i=24*80;i<25*80;++i)
			video_memory[i]=blank;
		// 光标上移一行
		cursor_y=24;
	}

}

// 清屏操作
void console_clear()
{
	uint8_t attr_byte=(0<<4)|(15&0x0f); //颜色
	uint16_t blank = 0x20 | (attr_byte<<8);  //高位为颜色，低位为字符
	int i;
	for (i=0;i<80*25;++i)
	{
		video_memory[i]=blank;
	}
	cursor_x=0;
	cursor_y=0;
	move_cursor();
}


void console_putc_color(char c, real_color_t back, real_color_t fore)
{
	uint8_t back_color = (uint8_t)back;
	uint8_t fore_color = (uint8_t)fore;

	uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
	uint16_t attribute = attribute_byte << 8;

	// 0x08 是退格键的 ASCII 码
	// 0x09 是tab 键的 ASCII 码
	if (c == 0x08 && cursor_x) {
		cursor_x--;
	} else if (c == 0x09) {
		cursor_x = (cursor_x+8) & ~(8-1);
	} else if (c == '\r') {
		cursor_x = 0;
	} else if (c == '\n') {
		cursor_x = 0;
		cursor_y++;
	} else if (c >= ' ') {
		video_memory[cursor_y*80 + cursor_x] = c | attribute;
		cursor_x++;
	}

	// 每 80 个字符一行，满80就必须换行了
	if (cursor_x >= 80) {
		cursor_x = 0;
		cursor_y ++;
	}

	// 如果需要的话滚动屏幕显示
	scroll();

	// 移动硬件的输入光标
	move_cursor();
}

void console_write(char *cstr)
{
	while (*cstr) {
		console_putc_color(*cstr++, rc_black, rc_white);
	}
}

void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
	while (*cstr) {
		console_putc_color(*cstr++, back, fore);
	}
}


// 屏幕输出一个十六进制的整型数
void console_write_hex(uint32_t n, real_color_t back, real_color_t fore)
{

}

// 屏幕输出一个十进制的整型数
void console_write_dec(uint32_t n, real_color_t back, real_color_t fore)
{

}

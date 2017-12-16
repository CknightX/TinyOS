#include "debug.h"
#include "idt.h"
#include "common.h"
#include "keyboard.h"
#include "keymap.h"
#include "string.h"
KB_INPUT	kb_in;

int	code_with_E0;
int	shift_l;	/* l shift state */
int	shift_r;	/* r shift state */
int	alt_l;		/* l alt state	 */
int	alt_r;		/* r left state	 */
int	ctrl_l;		/* l ctrl state	 */
int	ctrl_r;		/* l ctrl state	 */
int	caps_lock;	/* Caps Lock	 */
int	num_lock;	/* Num Lock	 */
int	scroll_lock;	/* Scroll Lock	 */
int	column;

uint8_t	get_byte_from_kbuf();

/*utils.asm*/
extern void disable_int();
extern void enable_int();

/*tty.c*/
extern void in_process(uint32_t key);

/*======================================================================*
  keyboard_handler
 *======================================================================*/
void keyboard_handler(int irq)
{
	uint8_t scan_code = inb(KB_DATA);

	if (kb_in.count < KB_IN_BYTES) {
		*(kb_in.p_head) = scan_code;
		kb_in.p_head++;
		if (kb_in.p_head == kb_in.buf + KB_IN_BYTES) {
			kb_in.p_head = kb_in.buf;
		}
		kb_in.count++;
	}
}


/*======================================================================*
  init_keyboard
 *======================================================================*/
void init_keyboard()
{
	kb_in.count = 0;
	kb_in.p_head = kb_in.p_tail = kb_in.buf;

	shift_l	= shift_r = 0;
	alt_l	= alt_r   = 0;
	ctrl_l	= ctrl_r  = 0;

	set_irq_handler(KEYBOARD_IRQ, keyboard_handler);/*设定键盘中断处理程序*/
	enable_irq(KEYBOARD_IRQ);                       /*开键盘中断*/
}


/*======================================================================*
  keyboard_read
 *======================================================================*/
void keyboard_read()
{
	uint8_t	scan_code;
	//char	output[2];
	int	make;	/* 1: make;  0: break. */

	uint32_t	key = 0;/* 用一个整型来表示一个键。比如，如果 Home 被按下，
				 * 则 key 值将为定义在 keyboard.h 中的 'HOME'。
				 */
	uint32_t*	keyrow;	/* 指向 keymap[] 的某一行 */

	if(kb_in.count > 0){
		code_with_E0 = 0;

		scan_code = get_byte_from_kbuf();

		/* 下面开始解析扫描码 */
		if (scan_code == 0xE1) {
			int i;
			uint8_t pausebrk_scode[] = {0xE1, 0x1D, 0x45,
				0xE1, 0x9D, 0xC5};
			int is_pausebreak = 1;
			for(i=1;i<6;i++){
				if (get_byte_from_kbuf() != pausebrk_scode[i]) {
					is_pausebreak = 0;
					break;
				}
			}
			if (is_pausebreak) {
				key = PAUSEBREAK;
			}
		}
		else if (scan_code == 0xE0) {
			scan_code = get_byte_from_kbuf();

			/* PrintScreen 被按下 */
			if (scan_code == 0x2A) {
				if (get_byte_from_kbuf() == 0xE0) {
					if (get_byte_from_kbuf() == 0x37) {
						key = PRINTSCREEN;
						make = 1;
					}
				}
			}
			/* PrintScreen 被释放 */
			if (scan_code == 0xB7) {
				if (get_byte_from_kbuf() == 0xE0) {
					if (get_byte_from_kbuf() == 0xAA) {
						key = PRINTSCREEN;
						make = 0;
					}
				}
			}
			/* 不是PrintScreen, 此时scan_code为0xE0紧跟的那个值. */
			if (key == 0) {
				code_with_E0 = 1;
			}
		}
		if ((key != PAUSEBREAK) && (key != PRINTSCREEN)) {
			/* 首先判断Make Code 还是 Break Code */
			make = (scan_code & FLAG_BREAK ? 0 : 1);

			/* 先定位到 keymap 中的行 */
			keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS];

			column = 0;
			if (shift_l || shift_r) {
				column = 1;
			}
			if (code_with_E0) {
				column = 2; 
				code_with_E0 = 0;
			}

			key = keyrow[column];

			switch(key) {
				case SHIFT_L:
					shift_l = make;
					break;
				case SHIFT_R:
					shift_r = make;
					break;
				case CTRL_L:
					ctrl_l = make;
					break;
				case CTRL_R:
					ctrl_r = make;
					break;
				case ALT_L:
					alt_l = make;
					break;
				case ALT_R:
					alt_l = make;
					break;
				default:
					break;
			}

			if (make) { /* 忽略 Break Code */
				key |= shift_l	? FLAG_SHIFT_L	: 0;
				key |= shift_r	? FLAG_SHIFT_R	: 0;
				key |= ctrl_l	? FLAG_CTRL_L	: 0;
				key |= ctrl_r	? FLAG_CTRL_R	: 0;
				key |= alt_l	? FLAG_ALT_L	: 0;
				key |= alt_r	? FLAG_ALT_R	: 0;

				in_process(key);
			}
		}
	}
}

/*======================================================================*
  get_byte_from_kbuf
 *======================================================================*/
uint8_t get_byte_from_kbuf()       /* 从键盘缓冲区中读取下一个字节 */
{
	uint8_t scan_code;

	while (kb_in.count <= 0) {}   /* 等待下一个字节到来 */

	disable_int();
	scan_code = *(kb_in.p_tail);
	kb_in.p_tail++;
	if (kb_in.p_tail == kb_in.buf + KB_IN_BYTES) {
		kb_in.p_tail = kb_in.buf;
	}
	kb_in.count--;
	enable_int();

	return scan_code;
}


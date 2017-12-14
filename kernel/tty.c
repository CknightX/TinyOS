extern void keyboard_read();

void task_tty()
{
	while(1)
		keyboard_read();
}

#include <todo.h>
/*
user proc

*/
void init_pic(void)
/* PIC初始化 */
{
	 /* 禁止中断 */
	io_out8(PIC0_IMR,  0xff  );
	io_out8(PIC1_IMR,  0xff  );
	io_out8(PIC0_ICW1, 0x11  );
	io_out8(PIC0_ICW2, 0x20  );
	io_out8(PIC0_ICW3, 1 << 2);
	io_out8(PIC0_ICW4, 0x01  );
	io_out8(PIC1_ICW1, 0x11  );
	io_out8(PIC1_ICW2, 0x28  );
	io_out8(PIC1_ICW3, 2     );
	io_out8(PIC1_ICW4, 0x01  );
	/* 禁止其他中断 */
	io_out8(PIC0_IMR,  0xfb  );
	io_out8(PIC1_IMR,  0xff  );

	return;
}

#define PORT_KEYDAT		0x0060

struct FIFO8 keyfifo;

/* 键盘中断 */
void inthandler21(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	unsigned char data, s[4];
	io_out8(PIC0_OCW2, 0x61);
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&keyfifo, data);
	return;
}

struct FIFO8 mousefifo;

void inthandler2c(int *esp)
{
	unsigned char data;
	io_out8(PIC1_OCW2, 0x64);
	io_out8(PIC0_OCW2, 0x62);
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&mousefifo, data);
	return;
}


/* 只是接收，不做其他操作 */
void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67);
	return;
}

/* 关于绘图部分的处理 */

#include <todo.h>

void init_palette(void)
{
	static unsigned char table_rgb[48] = {
		0x00, 0x00, 0x00,0xff, 0x00, 0x00,0x00, 0xff, 0x00,0xff, 0xff, 0x00,0x00, 0x00, 0xff,0xff, 0x00, 0xff,
		0x00, 0xff, 0xff,0xff, 0xff, 0xff,0xc6, 0xc6, 0xc6,0x84, 0x00, 0x00,0x00, 0x84, 0x00,0x84, 0x84, 0x00,
		0x00, 0x00, 0x84,0x84, 0x00, 0x84,0x00, 0x84, 0x84,0x84, 0x84, 0x84
	};
	unsigned char table2[216*3] ;
	int r,g,b ;
	set_palette(0, 15, table_rgb);
	for (b = 0; b < 6; b++) {
		for (g = 0; g < 6; g++) {
			for (r = 0; r < 6; r++) {
				table2[(r + g * 6 + b * 36) * 3 + 0] = r * 51;
				table2[(r + g * 6 + b * 36) * 3 + 1] = g * 51;
				table2[(r + g * 6 + b * 36) * 3 + 2] = b * 51;
			}
		}
	}
	set_palette(16, 231, table2);
	return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* 记录中断许可标志的值 */
	io_cli(); 					/* 将中断许可标志置为0,禁止中断 */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* 复原中断许可标志 */
	return;
}

// 区域绘制
void print_area(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

// 初始界面
void init_screen8(char *vram, int x, int y)
{
 	print_area(vram, x, COL8_000000,  0,     0,      x, 		y);
	print_area(vram, x, COL8_C6C6C6,  0,     0,     x/2,   y);
	// print_area(vram, x, COL8_008484,  x/2+2,   0,     x,   y);
	// print_area(vram, x, COL8_C6C6C6,  x/2+2,    y-25,   x,   y);
	// print_area(vram, x, COL8_848484,  x/2+4,    y-25+3,   x,   y);
	print_area(vram, x, COL8_000000,  3,     15,     x/2-3, y-3);
	return;
}

void print_font(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void print_string(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	/* C语言中，字符串都是以0x00结尾 */
	for (; *s != 0x00; s++) {
		print_font(vram, xsize, x, y, c, hankaku + *s * 16);
		x += 8;
	}
	return;
}

void init_mouse_cursor8(char *mouse, char bc)
{
	static char cursor[16][16] = {
		"****************",
		".*OOOOOOOOOOOO*.",
		".*OOOOOOOOOOOO*.",
		"......*O*.......",
		"......*O*.......",
		"..*OOOOOOOOOO*..",
		"..*OOOOOOOOOO*..",
		"......*OO*......",
		"......*OO*......",
		"*OOOOOOOOOOOOOO*",
		"*OOOOOOOOOOOOOO*",
		".....*OOO*......",
		".....*OOO*......",
		".....*OOO*......",
		".....*OOO*......",
		"......***......."
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}

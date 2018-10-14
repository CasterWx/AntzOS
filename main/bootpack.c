#include "todo.h"
#include <stdio.h>
#include <string.h>

extern struct FIFO8 keyfifo, mousefifo;
void enable_mouse(void);
void init_keyboard(void);
void new_pe(struct BOOTINFO *binfo);

int write_x = 55 ;
int write_y = 57 ;

char* replace_char(char s[40]){
	char *chr = "$" ;
	if((strcmp(s,"1E")==0)||(strcmp(s,"9E")==0)){
			chr = "a" ;
	}else if((strcmp(s,"30")==0)||(strcmp(s,"B0")==0)){
			chr = "b" ;
	}else if((strcmp(s,"2E")==0)||(strcmp(s,"AE")==0)){
			chr = "c" ;
	}else if((strcmp(s,"20")==0)||(strcmp(s,"A0")==0)){
			chr = "d" ;
	}else if((strcmp(s,"12")==0)||(strcmp(s,"92")==0)){
			chr = "e" ;
	}else if((strcmp(s,"21")==0)||(strcmp(s,"A1")==0)){
			chr = "f" ;
	}else if((strcmp(s,"22")==0)||(strcmp(s,"A2")==0)){
			chr = "g" ;
	}else if((strcmp(s,"23")==0)||(strcmp(s,"A3")==0)){
			chr = "h" ;
	}else if((strcmp(s,"17")==0)||(strcmp(s,"97")==0)){
		chr = "i" ;
	}else if((strcmp(s,"24")==0)||(strcmp(s,"A4")==0)){
		chr = "j" ;
	}else if((strcmp(s,"25")==0)||(strcmp(s,"A5")==0)){
		chr = "k" ;
	}else if((strcmp(s,"26")==0)||(strcmp(s,"A6")==0)){
		chr = "l" ;
	}else if((strcmp(s,"32")==0)||(strcmp(s,"B2")==0)){
		chr = "m" ;
	}else if((strcmp(s,"31")==0)||(strcmp(s,"B1")==0)){
		chr = "n" ;
	}else if((strcmp(s,"18")==0)||(strcmp(s,"98")==0)){
		chr = "o" ;
	}else if((strcmp(s,"19")==0)||(strcmp(s,"99")==0)){
		chr = "p" ;
	}else if((strcmp(s,"10")==0)||(strcmp(s,"90")==0)){
		chr = "q" ;
	}else if((strcmp(s,"13")==0)||(strcmp(s,"93")==0)){
		chr = "r" ;
	}else if((strcmp(s,"1F")==0)||(strcmp(s,"9F")==0)){
		chr = "s" ;
	}else if((strcmp(s,"14")==0)||(strcmp(s,"94")==0)){
		chr = "t" ;
	}else if((strcmp(s,"16")==0)||(strcmp(s,"96")==0)){
		chr = "u" ;
	}else if((strcmp(s,"2F")==0)||(strcmp(s,"AF")==0)){
		chr = "v" ;
	}else if((strcmp(s,"11")==0)||(strcmp(s,"91")==0)){
		chr = "w" ;
	}else if((strcmp(s,"2D")==0)||(strcmp(s,"AD")==0)){
		chr = "x" ;
	}else if((strcmp(s,"15")==0)||(strcmp(s,"95")==0)){
		chr = "y" ;
	}else if((strcmp(s,"2C")==0)||(strcmp(s,"AC")==0)){
		chr = "z" ;
	}else if((strcmp(s,"39")==0)||(strcmp(s,"B9")==0)){
		chr = " " ;
	}
	return chr ;
}

int flag = 1 ;

// 指令缓存，但是因为中断响应的时间问题，终端输入速度要非常慢
char  command[100]  = "";
void add_command(char *s)  {
/*
if(strcmp(s," ")==0){
	sprintf(command,"%s%s",command,"");
}else	if(strcmp(s,"$")){
				//忽略这种错误输入
		}else {
	}
*/
		sprintf(command,"%s%s",command,s);

}

void action_command(struct BOOTINFO *binfo){
		// action command
		// ls
		// data
		if(strcmp(command,"data")==0){
			// get new data;
			write_y += 19 ;
			putfonts8_asc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS in 2018");
		}else if(strcmp(command,"cls")==0){
			flag = 0 ;
			new_pe(binfo);
		}else if(strcmp(command,"version")==0){
			write_y += 19 ;
			putfonts8_asc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "Antz.version.1.1");
		}else if(strcmp(command,"help")==0){
			// help内容过多，显示在图形化界面区域
		}else if(sizeof(command)>=1){
				write_y += 19 ;
				putfonts8_asc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "Not Found");
		}
		// 命令缓存清除
		sprintf(command,"%s","");
}

void key(struct BOOTINFO *binfo,char s[40]){
	if((strcmp(s,"1C")==0)||(strcmp(s,"9C")==0)){
			action_command(binfo);
			write_x = 58 ;
			write_y += 19 ;
			putfonts8_asc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
	}else if((strcmp(s,"3B")==0)||(strcmp(s,"BB")==0)){  //关于F1的响应中断
			sprintf(command,"%s","");
			flag = 0 ;
			new_pe(binfo);
			putfonts8_asc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
	}else if(strcmp(s,"0E")==0){
			// 回退
			write_x -= 8 ;
			boxfill8(binfo->vram, binfo->scrnx , COL8_000000,  write_x,     write_y,     write_x+19, write_y+19);
			if(write_x<=58){
				write_x = 146 ;
				write_y -= 19 ;
			}
	}else {
			//putfonts8_asc(binfo->vram, binfo->scrnx,  write_x,  write_y, COL8_FFFFFF, s);
			putfonts8_asc(binfo->vram, binfo->scrnx,  write_x,  write_y, COL8_FFFFFF, replace_char(s));
			add_command(replace_char(s));
			write_x += 8 ;
			// 添加响应区
			//清除
			//boxfill8(binfo->vram, binfo->scrnx, COL8_008400 , 300	,240	,310	,250);
			//打印字符 Only use debug
			//putfonts8_asc(binfo->vram, binfo->scrnx,  300,  240 ,COL8_000000, s) ;
	}
	if(write_x>148){
		write_x = 58 ;
		write_y += 19 ;
		//putfonts8_asc(binfo->vram, binfo->scrnx, 4, 57, COL8_FFFFFF, "AntzOS>");
	}
	if(write_y>180){
 		new_pe(binfo);
		putfonts8_asc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
	}

}

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32], mousebuf[128];
	int mx, my, i;

	init_gdtidt();
	init_pic();
	io_sti(); /* PIC的初始化已经完成*/

	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	io_out8(PIC0_IMR, 0xf9); /* 开放PIC1和键盘中断(11111001) */
	io_out8(PIC1_IMR, 0xef); /* 开放鼠标中断(11101111) */

	init_keyboard();

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

	putfonts8_asc(binfo->vram, binfo->scrnx,  0,  0, COL8_FFFFFF, "Terminal-Antz");
	putfonts8_asc(binfo->vram, binfo->scrnx,  0,  0, COL8_000000, "Terminal-Antz");
	putfonts8_asc(binfo->vram, binfo->scrnx,  107,  0, COL8_000000, "|-|o|x|");
	putfonts8_asc(binfo->vram, binfo->scrnx, 4, 19, COL8_FFFFFF, "AntzOS> SayHello()");
	putfonts8_asc(binfo->vram, binfo->scrnx, 4, 38, COL8_FFFFFF, "Hello My AntzOs.");
	putfonts8_asc(binfo->vram, binfo->scrnx, 4, 57, COL8_FFFFFF, "AntzOS>_");


	enable_mouse();
	for (;;) {
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
			io_stihlt();
		} else {
			if (fifo8_status(&keyfifo) != 0) {

				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", i);
				//boxfill8(binfo->vram, binfo->scrnx, COL8_008484,  0, 16, 15, 31);
				//putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
				if (flag){
					key(binfo,s);
				}
				if(flag==1){
					flag = 0 ;
				}else {
					flag = 1 ;
				}
			} else if (fifo8_status(&mousefifo) != 0) {
				i = fifo8_get(&mousefifo);
				io_sti();
			}
		}
	}
}

void new_pe(struct BOOTINFO *binfo){
	write_x = 58 ;
	write_y = 19 ;
	// 右边并没有保存
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	putfonts8_asc(binfo->vram, binfo->scrnx,  0,  0, COL8_FFFFFF, "Terminal-Antz");
	putfonts8_asc(binfo->vram, binfo->scrnx,  0,  0, COL8_000000, "Terminal-Antz");
	putfonts8_asc(binfo->vram, binfo->scrnx,  107,  0, COL8_000000, "|-|o|x|");
	// 此处保留此输出，交给调用者自己
	//	putfonts8_asc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
}

#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47

void wait_KBC_sendready(void)
{
	/* 等待键盘准备 */
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}

void init_keyboard(void)
{
	/* 初始化键盘 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4

void enable_mouse(void)
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	return;
}

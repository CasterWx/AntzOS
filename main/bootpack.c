#include <todo.h>
#include <stdio.h>
#include <string.h>

extern struct FIFO8 keyfifo, mousefifo;
void init_keyboard(void);

void new_pe(struct BOOTINFO *binfo);
int write_x = 58 ;
int write_y = 57 ;

char* replace_char(char s[40]){
	char *chr = "" ;
	if (strcmp(s,"9E")==0){
			chr = "" ;
	}else if(strcmp(s,"B0")==0){
			chr = "" ;
	}else if(strcmp(s,"AE")==0){
			chr = "" ;
	}else if(strcmp(s,"A0")==0){
			chr = "" ;
	}else if(strcmp(s,"92")==0){
			chr = "" ;
	}else if(strcmp(s,"A1")==0){
			chr = "" ;
	}else if(strcmp(s,"A2")==0){
			chr = "" ;
	}else if(strcmp(s,"A3")==0){
			chr = "" ;
	}else if(strcmp(s,"97")==0){
			chr = "" ;
	}else if(strcmp(s,"A4")==0){
			chr = "" ;
	}else if(strcmp(s,"A5")==0){
			chr = "" ;
	}else if(strcmp(s,"A6")==0){
			chr = "" ;
	}else if(strcmp(s,"B2")==0){
			chr = "" ;
	}else if(strcmp(s,"B1")==0){
			chr = "" ;
	}else if(strcmp(s,"98")==0){
			chr = "" ;
	}else if(strcmp(s,"99")==0){
			chr = "" ;
	}else if(strcmp(s,"90")==0){
			chr = "" ;
	}else if(strcmp(s,"93")==0){
			chr = "" ;
	}else if(strcmp(s,"9F")==0){
			chr = "" ;
	}else if(strcmp(s,"94")==0){
			chr = "" ;
	}else if(strcmp(s,"96")==0){
			chr = "" ;
	}else if(strcmp(s,"AF")==0){
			chr = "" ;
	}else if(strcmp(s,"91")==0){
			chr = "" ;
	}else if(strcmp(s,"AD")==0){
			chr = "" ;
	}else if(strcmp(s,"95")==0){
			chr = "" ;
	}else if(strcmp(s,"AC")==0){
			chr = "" ;
	}else if(strcmp(s,"B9")==0){
			chr = "" ;
	}else	if((strcmp(s,"A7")==0)){
			chr = "" ;
	}else	if((strcmp(s,"B4")==0)){
			chr = "" ;
	}else	if((strcmp(s,"B3")==0)){
			chr = "" ;
	}else	if((strcmp(s,"9A")==0)){
			chr = "" ;
	}else	if((strcmp(s,"9B")==0)){
			chr = "" ;
	}else	if((strcmp(s,"1A")==0)){
			chr = "[" ;
	}else	if((strcmp(s,"1B")==0)){
			chr = "]" ;
	}else	if((strcmp(s,"33")==0)){
			chr = "," ;
	}else	if((strcmp(s,"27")==0)){
			chr = ";" ;
	}else	if((strcmp(s,"34")==0)){
			chr = "." ;
	}else	if((strcmp(s,"1E")==0)){
			chr = "a" ;
	}else if((strcmp(s,"30")==0)){
			chr = "b" ;
	}else if((strcmp(s,"2E")==0)){
			chr = "c" ;
	}else if((strcmp(s,"20")==0)){
			chr = "d" ;
	}else if((strcmp(s,"12")==0)){
			chr = "e" ;
	}else if((strcmp(s,"21")==0)){
			chr = "f" ;
	}else if((strcmp(s,"22")==0)){
			chr = "g" ;
	}else if((strcmp(s,"23")==0)){
			chr = "h" ;
	}else if((strcmp(s,"17")==0)){
		chr = "i" ;
	}else if((strcmp(s,"24")==0)){
		chr = "j" ;
	}else if((strcmp(s,"25")==0)){
		chr = "k" ;
	}else if((strcmp(s,"26")==0)){
		chr = "l" ;
	}else if((strcmp(s,"32")==0)){
		chr = "m" ;
	}else if((strcmp(s,"31")==0)){
		chr = "n" ;
	}else if((strcmp(s,"18")==0)){
		chr = "o" ;
	}else if((strcmp(s,"19")==0)){
		chr = "p" ;
	}else if((strcmp(s,"10")==0)){
		chr = "q" ;
	}else if((strcmp(s,"13")==0)){
		chr = "r" ;
	}else if((strcmp(s,"1F")==0)){
		chr = "s" ;
	}else if((strcmp(s,"14")==0)){
		chr = "t" ;
	}else if((strcmp(s,"16")==0)){
		chr = "u" ;
	}else if((strcmp(s,"2F")==0)){
		chr = "v" ;
	}else if((strcmp(s,"11")==0)){
		chr = "w" ;
	}else if((strcmp(s,"2D")==0)){
		chr = "x" ;
	}else if((strcmp(s,"15")==0)){
		chr = "y" ;
	}else if((strcmp(s,"2C")==0)){
		chr = "z" ;
	}else if((strcmp(s,"39")==0)){
		chr = " " ;
	}else if(strcmp(s,"9C")==0){ // 回车的弹起中断
		chr = "" ;
	}else if(strcmp(s,"BB")==0){
		chr = "" ;
	}
	return chr ;
}

// int flag = 1 ;
int x_move = 0 ;
// 指令缓存，但是因为中断响应的时间问题，终端输入速度要非常慢
char  command[100]  = "";
int command_index = 0 ;
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
		command_index++ ;
}
// vim之前保存x，y
int last_x = 0 ;
int last_y = 0 ;
void action_command(struct BOOTINFO *binfo){
		// action command
		// ls
		// data
		if(strcmp(command,"data")==0){
			// get new data;
			write_y += 19 ;
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS in 2018");
		}else if(strcmp(command,"reset")==0){
			// 重置
			write_x = 58 ;
			write_y = 19 ;
			// 右边并没有保存
			init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
			print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_FFFFFF, "Terminal-Antz");
			print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_000000, "Terminal-Antz");
			print_string(binfo->vram, binfo->scrnx,  107,  0, COL8_000000, "|-|o|x|");
		}else if(strcmp(command,"vim")==0){
			// vim edit
			// 重置右半边屏幕
			print_area(binfo->vram,binfo->scrnx,COL8_000000, 160,0,320-3,260-3);
			print_string(binfo->vram,binfo->scrnx,162,2,COL8_00FF00,"Vim :");
			x_move = 104 ;
			last_y = write_y + 19 ;
			write_x = 58 ;
			write_y = 2 ;
		}else if(strcmp(command,"playgame")==0){
			to_printf() ;
		}else if(strcmp(command,"cls")==0){
			// flag = 0 ;
			new_pe(binfo);
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "New PE:");
		}else if(strcmp(command,"version")==0){
			write_y += 19 ;
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "Antz.version.2.0");
		}else if(strcmp(command,"help")==0){
			// help内容过多，显示在图形化界面区域
			print_area(binfo->vram, binfo->scrnx , COL8_000000,  160,     0,     320-3, 260-3);
			print_string(binfo->vram, binfo->scrnx, 162, 2+19, COL8_00FF00,  "  He was stabbed in"); //21
			print_string(binfo->vram, binfo->scrnx, 161, 21+19, COL8_00FF00, "the throat. He died");
			print_string(binfo->vram, binfo->scrnx, 161, 40+19, COL8_00FF00, "almost instantly.");
			print_string(binfo->vram, binfo->scrnx, 161, 59+19, COL8_00FF00, "  Although I hadn't");
			print_string(binfo->vram, binfo->scrnx, 161, 78+19, COL8_00FF00, "seen him in more th");
			print_string(binfo->vram, binfo->scrnx, 161, 97+19, COL8_00FF00, "an ten years,  I kn");
			print_string(binfo->vram, binfo->scrnx, 161, 116+19, COL8_00FF00,"ow I will miss him");
			print_string(binfo->vram, binfo->scrnx, 161, 135+19, COL8_00FF00, "forever.");
			print_string(binfo->vram, binfo->scrnx, 220, 160+19, COL8_00FF00, "AntzOs-10/16");
		}else if(sizeof(command)>=1){
				write_y += 19 ;
				print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "Not Found");
		}
		// 命令缓存清除
		sprintf(command,"%s","");
}

void key(struct BOOTINFO *binfo,char s[40]){
	if((strcmp(s,"1C")==0)){  // enter
			if(x_move!=0){
				// 右边
				print_area(binfo->vram, binfo->scrnx ,COL8_000000,x_move + write_x,write_y,x_move+write_x+8, write_y+19);
				write_x = 58 ;
				write_y += 19 ;
			}else {
				// 左边
				print_area(binfo->vram, binfo->scrnx ,COL8_000000,x_move + write_x,write_y,x_move+write_x+8, write_y+19);
				action_command(binfo);
				write_x = 58 ;
				write_y += 19 ;
				if (x_move==0)
					print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
			}
	}else if((strcmp(s,"01")==0)){
		if (x_move!=0){
			print_area(binfo->vram, binfo->scrnx ,COL8_000000,x_move + write_x,write_y,x_move+write_x+8, write_y+19);
			x_move = 0 ;
			write_y = last_y ;
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
			write_x = 58 ;
		}
	}else if((strcmp(s,"0F")==0)){
		// 关于tab 0F 8F
		print_string(binfo->vram, binfo->scrnx, x_move+write_x, write_y, COL8_FFFFFF, " ");
		write_x += 8 ;
		border(binfo);
		print_string(binfo->vram, binfo->scrnx, x_move+write_x, write_y, COL8_FFFFFF, " ");
		write_x += 8 ;
		border(binfo);
		print_string(binfo->vram, binfo->scrnx, x_move+write_x, write_y, COL8_FFFFFF, " ");
		write_x += 8 ;
		border(binfo);
	}else if((strcmp(s,"3B")==0)){  //关于F1的响应中断
			sprintf(command,"%s","");
			// flag = 0
			x_move = 0 ;
			new_pe(binfo);
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
	}else if(strcmp(s,"0E")==0){
			// 回退
			print_area(binfo->vram, binfo->scrnx ,COL8_000000,x_move + write_x,write_y,x_move+write_x+8, write_y+19);
			int len = strlen(command);
			command[len - 1] = '\0';
			write_x -= 8 ;
			print_area(binfo->vram, binfo->scrnx , COL8_000000,  x_move + write_x,     write_y,     x_move+write_x+8, write_y+19);
			if(x_move!=0){
   				// 正在右边界
					if(write_x<=60) {
						write_x = 202 ;
						write_y -= 19 ;
					}
			}else if(x_move==0){
					// 正在左边界
					if(write_x<=4) {
						write_x = 146 ;
						write_y -= 19 ;
					}
			}

	}else {
			//putfonts8_asc(binfo->vram, binfo->scrnx,  write_x,  write_y, COL8_FFFFFF, s);
			char *in = replace_char(s) ;
			if(strcmp(in,"")==0){

			}else {
				print_area(binfo->vram, binfo->scrnx, COL8_000000, x_move+write_x, write_y, x_move+write_x+8, write_y+19);
				print_string(binfo->vram, binfo->scrnx,  x_move + write_x,  write_y, COL8_FFFFFF, in);
				add_command(in);
				write_x += 8 ;
			}
			// 添加响应区
			//清除
			//boxfill8(binfo->vram, binfo->scrnx, COL8_008400 , 300	,240	,310	,250);
			//打印字符 Only use debug
			//putfonts8_asc(binfo->vram, binfo->scrnx,  300,  240 ,COL8_000000, s) ;
	}
	border(binfo);
}
// 边界处理
void border(struct BOOTINFO *binfo){
	if (x_move==0){
		// 左边
		if(write_x>148){
			write_x = 4 ;
			write_y += 19 ;
			//putfonts8_asc(binfo->vram, binfo->scrnx, 4, 57, COL8_FFFFFF, "AntzOS>");
		}
		if(write_y>180){
	 		new_pe(binfo);
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
		}
	}else if(x_move!=0){
		// vim模式
		if(write_x>310-x_move){
			write_x = 58 ;
			write_y += 19 ;
		}
		if(write_y>180){
			write_y = 15 ;
			write_x = 58 ;
			print_area(binfo->vram,binfo->scrnx,COL8_000000, 160,0,320-3,260-3);
			print_string(binfo->vram,binfo->scrnx,162,2,COL8_00FF00,"Vim :");
		}
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
	init_pit();
	io_out8(PIC0_IMR, 0xf8); /* 开放PIC1和键盘中断(11111001) */
	io_out8(PIC1_IMR, 0xef); /* 开放鼠标中断(11101111) */

	init_keyboard();

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

	print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_FFFFFF, "Terminal-Antz");
	print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_000000, "Terminal-Antz");
	print_string(binfo->vram, binfo->scrnx,  107,  0, COL8_000000, "|-|o|x|");
	print_string(binfo->vram, binfo->scrnx, 4, 19, COL8_FFFFFF, "AntzOS> SayHello()");
	print_string(binfo->vram, binfo->scrnx, 4, 38, COL8_FFFFFF, "Hello My AntzOs.");
	print_string(binfo->vram, binfo->scrnx, 4, 57, COL8_FFFFFF, "AntzOS>");


	for (;;) {
		int t = timerctl.count/100 ;
		sprintf(s, "time:%8ds", t);
		print_area(binfo->vram, binfo->scrnx, COL8_000000,  162,     0,      315, 		16);
		print_string(binfo->vram, binfo->scrnx, 162, 0, COL8_840000, s) ;
		if (t%2==0){
			print_string(binfo->vram, binfo->scrnx, x_move+write_x, write_y, COL8_FFFFFF, "|") ;
		}else {
			print_area(binfo->vram, binfo->scrnx, COL8_000000, x_move+write_x, write_y, x_move+write_x+8, write_y+19);
		}
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
			io_stihlt();
		} else {
			if (fifo8_status(&keyfifo) != 0) {

				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", i);
					key(binfo,s);
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
	print_area(binfo->vram, binfo->scrnx, COL8_000000,  3,     15,     155 ,   260);
	print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_FFFFFF, "Terminal-Antz");
	print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_000000, "Terminal-Antz");
	print_string(binfo->vram, binfo->scrnx,  107,  0, COL8_000000, "|-|o|x|");
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

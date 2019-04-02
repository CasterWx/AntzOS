#include <todo.h>
#include <stdio.h>
#include <string.h>

extern struct FIFO8 keyfifo, mousefifo;
void init_keyboard(void);

void new_pe(struct BOOTINFO *binfo);

int write_x = 60 ;
int write_y = 57 ;
// int flag = 1 ;
int x_move = 0 ;
// 指令缓存，但是因为中断响应的时间问题，终端输入速度要非常慢
char  command[100]  = "";
int command_index = 0 ;
void add_command(char *s)  {
		sprintf(command,"%s%s",command,s);
		command_index++ ;
}

// vim输入的数据，根据回车来分隔
struct vim_input{
	char vim_char[200] ;
	int len ;
	struct vim_input * next ;
} ;

// 初始40行缓存
struct vim_input mlist[40] ;
int vim_index = 0 ;


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
			write_x = 60 ;
			write_y = 19 ;
			// 右边并没有保存
			init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
			print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_FFFFFF, "Terminal-Antz");
			print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_000000, "Terminal-Antz");
			print_string(binfo->vram, binfo->scrnx,  107,  0, COL8_000000, "|-|o|x|");
		}else if(strcmp(command,"vim")==0){
			/*
			* @author : CasterWx
			* @time : 2019/3/19/16:32
			* @content : 经过思考之后，决定将vim_input结构体来存储输入数据，在回车后接下来的数据存储到下一个节点。
			*/
			// vim edit 
			// vim启动时刷新右半区域显存
			print_area(binfo->vram, binfo->scrnx , COL8_000000,   binfo->scrnx/2 + 3, 0, binfo->scrnx-3,  binfo->scrnx-3);
			// 初始化vim输入指针
			print_string(binfo->vram,binfo->scrnx,162,2,COL8_00FF00,"Vim :");
			x_move = binfo->scrnx/2 - 52  ;
			last_y = write_y + 19 ;
			write_x = 60 ;
			write_y = 2 ;

			// 清空过去的vim输入缓存  清空vim_input
			vim_index = 0 ;
		}else if(strcmp(command,"dijkstra")==0){
			to_printf_dijkstra();
		}else if(strcmp(command,"pdd")==0){
			to_printf_pdd();
		}else if(strcmp(command,"world.execute.me")==0){
			timerctl.flag = 1 ;
			init_data();
		}else if(strcmp(command,"cls")==0){
			// flag = 0 ;
			new_pe(binfo);
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "New PE:");
		}else if(strcmp(command,"version")==0){
			write_y += 19 ;
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "Antz.version.2.0");
		}else if(strcmp(command,"help")==0){
			// help内容过多，显示在图形化界面区域
			print_area(binfo->vram, binfo->scrnx , COL8_000000,   binfo->scrnx/2 + 3, 0, binfo->scrnx-3,  binfo->scrnx-3);
			print_string(binfo->vram, binfo->scrnx, binfo->scrnx/2+5, 2+19, COL8_00FF00,  "  He was stabbed in the throat.He died"); //21
			print_string(binfo->vram, binfo->scrnx, binfo->scrnx/2+5, 21+19, COL8_00FF00, "almost instantly.Although I hadn't seen");
			print_string(binfo->vram, binfo->scrnx, binfo->scrnx/2+5, 40+19, COL8_00FF00, "him in more than ten years,  I know I");
			print_string(binfo->vram, binfo->scrnx, binfo->scrnx/2+5, 59+19, COL8_00FF00, "will miss him forever.");
			print_string(binfo->vram, binfo->scrnx, binfo->scrnx/2+5, 78+19, COL8_00FF00, "   AntzOs-10/16");
		}else if(strcmp(command,"vimshow")==0){
			// 显示vim_input中内容
			char myline[] = "line is ";
			
			sprintf(myline,"%s%4d",myline,vim_index);
			print_string(binfo->vram, binfo->scrnx, binfo->scrnx/2, binfo->scrnx/2+19, COL8_FFFFFF, myline); //21
			
			print_string(binfo->vram, binfo->scrnx, binfo->scrnx/2+5, 5, COL8_FFFFFF,  "Old Vim Log:"); //21
			
			int index_my_vim = 0 ;
			for (index_my_vim=0;index_my_vim<vim_index;index_my_vim++){
				print_string(binfo->vram, binfo->scrnx, binfo->scrnx/2+5, 31+index_my_vim*19, COL8_FFFFFF,  mlist[index_my_vim].vim_char); //21
			}
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
				write_x = 60 ;
				write_y += 19 ;
			}else {
				// 左边
				print_area(binfo->vram, binfo->scrnx ,COL8_000000,x_move + write_x,write_y,x_move+write_x+8, write_y+19);
				action_command(binfo);
				write_x = 60 ;
				write_y += 19 ;
				if (x_move==0)
					print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
			}
			vim_index ++ ;
	}else if((strcmp(s,"01")==0)){
		if (x_move!=0){
			print_area(binfo->vram, binfo->scrnx ,COL8_000000,x_move + write_x,write_y,x_move+write_x+8, write_y+19);
			x_move = 0 ;
			write_y = last_y ;
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
			write_x = 60 ;
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
			timerctl.flag = 0 ;
			sprintf(command,"%s","");
			// flag = 0
			x_move = 0 ;
			new_pe(binfo);
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
			sprintf(command,"%s","");
	}else if(strcmp(s,"0E")==0){
			// 回退
			print_area(binfo->vram, binfo->scrnx ,COL8_000000,x_move + write_x,write_y,x_move+write_x+8, write_y+19);
			int len = strlen(command);
			command[len - 1] = '\0';
			write_x -= 8 ;
			print_area(binfo->vram, binfo->scrnx , COL8_000000,  x_move + write_x,     write_y,     x_move+write_x+8, write_y+19);
			if(x_move!=0){
   				// 正在右边界
				if((x_move+write_x)<=binfo->scrnx/2) {
					write_x = binfo->scrnx - 8;
					write_y -= 19 ;
				}
				
			}else if(x_move==0){
				// 正在左边界
				if(write_x<=9) {
					write_x = binfo->scrnx/2-12 ;
					write_y -= 19 ;
				}
			}

	}else {
			//putfonts8_asc(binfo->vram, binfo->scrnx,  write_x,  write_y, COL8_FFFFFF, s);
			// 非功能键则为输出键
			char *in = replace_char(s) ;
			if(strcmp(in,"")==0){

			}else {
				print_area(binfo->vram, binfo->scrnx, COL8_000000, x_move+write_x, write_y, x_move+write_x+8, write_y+19);
				print_string(binfo->vram, binfo->scrnx,  x_move + write_x,  write_y, COL8_FFFFFF, in);

				// vim内容记录
				if(x_move!=0){ 	
					sprintf(mlist[vim_index].vim_char,"%s%s",mlist[vim_index].vim_char,in) ; // 添加进去，但是为了在输出时不超过边界，选择
				}

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
		if(write_x > binfo->scrnx/2-10){
			write_x = 4 ;
			write_y += 19 ;
			//putfonts8_asc(binfo->vram, binfo->scrnx, 4, 57, COL8_FFFFFF, "AntzOS>");
		}
		if(write_y > binfo->scrny-20){
	 		new_pe(binfo);
			print_string(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
		}
	}else if(x_move!=0){
		// vim模式
		if(write_x> binfo->scrnx-10-x_move){
			write_x = 60 ;
			write_y += 19 ;
		}
		if(write_y>binfo->scrny-20){
			write_y = 15 ;
			write_x = 60 ;
			print_area(binfo->vram, binfo->scrnx , COL8_000000,   binfo->scrnx/2 + 3, 0, binfo->scrnx-3,  binfo->scrnx-3);
			print_string(binfo->vram,binfo->scrnx,binfo->scrnx/2 + 3, 0,COL8_00FF00,"Vim :");
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
	
	while (timerctl.count/100<5)
	 	to_show();


	print_area(binfo->vram, binfo->scrnx , COL8_FFFFFF , 0 , 0 , binfo->scrnx, binfo->scrny);
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

	print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_FFFFFF, "Terminal-Antz");
	print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_000000, "Terminal-Antz");
	print_string(binfo->vram, binfo->scrnx,  binfo->scrnx/2-60,  0, COL8_000000, "|-|o|x|");

	print_string(binfo->vram, binfo->scrnx, 4, 19, COL8_FFFFFF, "AntzOS> SayHello()");
	print_string(binfo->vram, binfo->scrnx, 4, 38, COL8_FFFFFF, "Hello My AntzOs.");
	print_string(binfo->vram, binfo->scrnx, 4, 57, COL8_FFFFFF, "AntzOS>");
 // 1024x768
	for (;;) {
		int t = timerctl.count/100 ;
		sprintf(s, "runtime:%8ds", t);
		print_area(binfo->vram, binfo->scrnx, COL8_C6C6C6,  162,     0,      315, 		14);
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
	write_x = 60 ;
	write_y = 19 ;
	// 右边并没有保存
	print_area(binfo->vram, binfo->scrnx, COL8_000000,  3,     15,     binfo->scrnx/2-3 ,   binfo->scrny-3);
	print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_FFFFFF, "Terminal-Antz");
	print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_000000, "Terminal-Antz");
	print_string(binfo->vram, binfo->scrnx,  binfo->scrnx/2-60,  0, COL8_000000, "|-|o|x|");
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

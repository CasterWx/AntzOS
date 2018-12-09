# 自制操作系统Antz day11——实现shell(下)命令响应
> 我已经规范了系统代码风格，类似于按照linux分包，把各部分功能区分开了
### [Antz系统更新地址](https://www.cnblogs.com/LexMoon/category/1262287.html)

### [Linux内核源码分析地址](https://www.cnblogs.com/LexMoon/category/1267413.html)

### [Github项目地址](https://github.com/CasterWx/AntzOS)

### 在之前的任务中，我们已经通过直接操作显卡驱动完成了简单的图形化。
### 需要了解之前的部分：
>   直接操作显卡请参考day03

>   简单图形化的实现请参考day09

>   键盘按键中断响应请参考day10

>   Makefile

### 项目目录

![s](https://www.cnblogs.com/images/cnblogs_com/LexMoon/1246510/o_project.jpg)

## 一 . 如何实现命令缓存

### 在之前已经完成了键盘的响应工作，但这种响应却是有很大的问题，比如说对于一次按键的两次中断处理。后来我把两种中断都加入了响应判断中，这样就不会有之前第10天所提到的bug了。

### * 在替换函数中完成识别。修改如下
```
char* replace_char(char s[40]){
	char *chr = "$" ;
	if((strcmp(s,"1E")==0)||(strcmp(s,"9E")==0)){
			chr = "a" ;
	}else if((strcmp(s,"30")==0)||(strcmp(s,"B0")==0)){
			chr = "b" ;
	}
	... // 省略
	return chr ;
```
### * 这里是一些功能按键的识别修改
```
    // Enter  -> 回车键 响应之前缓存的命令 换行
	if((strcmp(s,"1C")==0)||(strcmp(s,"9C")==0)){
			action_command(binfo);  //响应命令
			write_x = 58 ;   //下面是换行
			write_y += 19 ;
			putfonts8_asc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
	} 
    // F1 -> 终端刷新  类似于clear命令
	else if((strcmp(s,"3B")==0)||(strcmp(s,"BB")==0)){  //关于F1的响应中断
			sprintf(command,"%s","");  // 命令缓存清空
			flag = 0 ;  // 按键模式回复默认，这个看第10天，本质目的是为了处理一次按键的两次终端
			new_pe(binfo);  // 通过操作显存直接刷新当前终端
			putfonts8_asc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
	}
	//  Backspace -> 删格键
	else if(strcmp(s,"0E")==0){
			// 回退
			write_x -= 8 ;
			boxfill8(binfo->vram, binfo->scrnx , COL8_000000,  write_x,     write_y,     write_x+19, write_y+19);
			if(write_x<=58){
				write_x = 146 ;
				write_y -= 19 ;
			}
	} 
```
### 这些就是按键识别上目前修改的内容了。

### 接下来我们要添加一个功能，在每次按键之后，不但在屏幕上显示这个键，而且将他缓存在缓冲区，在下次回车的时候进行识别，并清空。

### 先来看一个简单的demo

```
// command数组就是命令缓存区
char  command[100]  = "";

void add_command(char *s)  { 
    sprintf(command,"%s%s",command,s); 
}

void action_command(){
    // 响应
}

```

## 二 . 实现

### command就是一个命令缓存数组，每次按键之后调用add_command()命令将这次的按键保存。

### sprintf()这个函数是字符串格式化命令，主要功能是把格式化的数据写入某个字符串中。sprintf 是个变参函数。使用sprintf 对于写入buffer的字符数是没有限制的，这就存在了buffer溢出的可能性。

### 我们使用sprintf函数直接在command后面添加了s字符。

### 那么在下次按下Enter的时候，我们只需要调用action_command,并且在里面识别是什么命令，然后做出合适的响应即可。

### 来看看完整的命令缓冲区实现。

``` 
// 指令缓存，但是因为中断响应的时间问题，终端输入速度要非常慢
char  command[100]  = "";
void add_command(char *s)  {
        
    if(strcmp(s," ")==0){
	    sprintf(command,"%s%s",command,"");
    }else if(strcmp(s,"$")){
		//忽略这种错误输入
	}else {
	    sprintf(command,"%s%s",command,s);  
	} 
}

void action_command(struct BOOTINFO *binfo){
		// action command 响应命令
		// ls命令
		// data命令
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

```

### 现在使用Makefile来生成这个支持命令的img镜像。
```
    make img
```
### 使用虚拟机打开镜像，结果如下
![os](https://www.cnblogs.com/images/cnblogs_com/LexMoon/1246510/o_122.jpg)


### 终端主要代码如下：
```

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

void main(void)
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

 
	for (;;) {
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
			io_stihlt();
		} else {
			if (fifo8_status(&keyfifo) != 0) {

				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", i);
				if (flag){
					key(binfo,s);
				}
				if(flag==1){
					flag = 0 ;
				}else {
					flag = 1 ;
				}
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


```
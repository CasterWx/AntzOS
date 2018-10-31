# 自制操作系统Antz
> 我已经规范了系统代码风格，类似于按照linux分包，把各部分功能区分开了

### 显示图片
![run](https://github.com/CasterWx/AntzOS/blob/master/screen/a.gif?raw=true)

### 动态
![run](https://github.com/CasterWx/AntzOS/blob/master/screen/execute.gif?raw=true)
![antz_run](https://github.com/CasterWx/AntzOS/blob/master/screen/antz_gif.gif?raw=true)

### [Antz系统更新地址](https://www.cnblogs.com/LexMoon/category/1262287.html)

### [Linux内核源码分析地址](https://www.cnblogs.com/LexMoon/category/1267413.html)

### [Github项目地址](https://github.com/CasterWx/AntzOS)

### 在之前的工作中，AntzOS已经从单调的界面，变得逐渐拥有自己的功能了。

### 真机运行情况 ：
![os1](https://www.cnblogs.com/images/cnblogs_com/LexMoon/1246510/o_qq_pic_merged_1539834568688.jpg)

![os2](https://www.cnblogs.com/images/cnblogs_com/LexMoon/1246510/o_antzos4.jpg)

![os3](https://www.cnblogs.com/images/cnblogs_com/LexMoon/1246510/o_qq_pic_merged_1539834611072.jpg)

### 这个系统在我最初的目的中就是实现一个半图形半终端的轻巧OS。

### 完成了当前的工作后，Antz接下来需要实现的则是关于任务调度相关的。

### 目前实现的是在Terminal中对命令的响应，还有一个简易的vim，可以用于右边界面的文本编辑。

### 对于按键中断，对全键盘的响应改良之后不会出现之前说的bug，但是在shift按下时的按键模式却是有很大问题，虽然我已经想到了解决方案，不外乎给shift的按下一个flag，弹起一个flag，但这部分感觉现在实现与否都是不怎么重要，所以就先忽略这里了。

### 最近同时也在读Linux内核源码。发现其中的注释也是很有意思，甚至Linus自己写的，他也不知道这部分为什么这样写，不断尝试之后发现可以实现，他就这样用了。

### 仔细看看关于按键响应命令的实现，现在只能说很懵，又臭又长，估计随便改改我就会不知道怎么继续下一步了。（不过好在我每添加一个功能都会把整个项目备份一份）

```
void key(Binfo binfo,char s[40]){
	if((strcmp(s,"1C")==0)){  // enter
			if(x_move!=0){
				// 右边
				write_x = 58 ;
				write_y += 19 ;
			}else {
				// 左边
				action_command(binfo);
				write_x = 58 ;
				write_y += 19 ;
				if (x_move==0)
					printasc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
			}
	}else if((strcmp(s,"0F")==0)){
		// 关于tab 0F 8F
		printasc(binfo->vram, binfo->scrnx, x_move+write_x, write_y, COL8_FFFFFF, " ");
		write_x += 8 ;
		border(binfo);
		printasc(binfo->vram, binfo->scrnx, x_move+write_x, write_y, COL8_FFFFFF, " ");
		write_x += 8 ;
		border(binfo);
		printasc(binfo->vram, binfo->scrnx, x_move+write_x, write_y, COL8_FFFFFF, " ");
		write_x += 8 ;
		border(binfo);
	}else if((strcmp(s,"3B")==0)){  //关于F1的响应中断
			sprintf(command,"%s","");
			// flag = 0
			x_move = 0 ;
			new_pe(binfo);
			printasc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
	}else if(strcmp(s,"0E")==0){
			// 回退
			int len = strlen(command);
			command[len - 1] = '\0';
			write_x -= 8 ;
			flash(binfo->vram, binfo->scrnx , COL8_000000,  x_move + write_x,     write_y,     x_move+write_x+19, write_y+19);
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
			char *in = replace_char(s) ;
			if(strcmp(in,"")==0){

			}else {
				printasc(binfo->vram, binfo->scrnx,  x_move + write_x,  write_y, COL8_FFFFFF, in);
				add_command(in);
				write_x += 8 ;
			}
			// 添加响应区
			//清除
			//打印字符 Only use debug
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
		}
		if(write_y>180){
	 		new_pe(binfo);
			printasc(binfo->vram, binfo->scrnx, 4, write_y, COL8_FFFFFF, "AntzOS>");
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
			flash(binfo->vram,binfo->scrnx,COL8_000000, 160,0,320-3,260-3);
			printasc(binfo->vram,binfo->scrnx,162,2,COL8_00FF00,"Vim :");
		}
	}
}
```


### 目前的项目目录，请忽略掉md文件，这个镜像文件可以直接使用工具写入u盘启动，或者在虚拟机打开。thun.c是今天(2018年10月18日)新增的，目的是为了抽离其他c文件中的工具化函数，不然以后只是一个源码文件都会让人头疼。

```
▒▒  antz.img
▒▒  Makefile
▒▒  README.md
▒▒
▒▒▒▒asmfunc
▒▒      naskfunc.nas
▒▒
▒▒▒▒assic
▒▒      char.set
▒▒
▒▒▒▒boot
▒▒      asmhead.asm
▒▒      mbr.asm
▒▒
▒▒▒▒include
▒▒      errno.h
▒▒      float.h
▒▒      limits.h
▒▒      math.h
▒▒      stdio.h
▒▒      string.h
▒▒      todo.h
▒▒
▒▒▒▒interrupt
▒▒      int.c
▒▒
▒▒▒▒io
▒▒      fifo.c
▒▒
▒▒▒▒lgdt
▒▒      set_lgdt.c
▒▒
▒▒▒▒log
▒▒      build.log
▒▒      delete.log
▒▒
▒▒▒▒main
▒▒      bootpack.c
▒▒      README..md
▒▒      shell_1.md
▒▒      shell_2.md
▒▒
▒▒▒▒thun
▒▒      thun.c
▒▒
▒▒▒▒windows
        graphic.c

```

### Makefile非常关键，如果没有这个，怕是我只是编译链接就得花费很长时间，而且中间估计会错误频出。
> Antz_kernel = main/bootpack.obj asmfunc/naskfunc.obj assic/char.set windows/graphic.obj lgdt/set_lgdt.obj interrupt/int.obj io/fifo.obj thun/thun.obj

### 之后的添加c文件，只需要在这里预先确定路径，然后将需要实现的函数声明在include/todo.h中即可。显卡虽然实现了函数可以直接操作，但是分辨率太低，导致界面极不美观，网卡驱动暂时没有想过要写，如果要实现，恐怕得整个antz都可以让用户使用的情况下才具备条件。

### [AntzOs](https://github.com/CasterWx/AntzOS)需要更多优秀的开发者来实现，无论你是正在学习操作系统课程还是其他方面，我相信AntzOs都可以帮助你更加的深入了解计算机操作系统底层的实现。

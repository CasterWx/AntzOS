# 自制操作系统Antz day10——实现shell(上)
> 我已经规范了系统代码风格，类似于按照linux分包，把各部分功能区分开了
### [Antz系统更新地址](https://www.cnblogs.com/LexMoon/category/1262287.html)

### [Linux内核源码分析地址](https://www.cnblogs.com/LexMoon/category/1267413.html)

### [Github项目地址](https://github.com/CasterWx/AntzOS)


### 在之前的任务中，我们已经通过直接操作显卡驱动完成了简单的图形化。
### 需要了解之前的部分：
>   直接操作显卡请参考day03

>   简单图形化的实现请参考day09

>   Makefile

### 项目目录

![s](https://www.cnblogs.com/images/cnblogs_com/LexMoon/1246510/o_project.jpg)

#### console的图形化实现与规则均在main/bootpack.c中完成

#### interrupt/int.c 中实现了键盘中断处理，按键会中断两次，一次按下，一次弹起，在响应处理中，只需要处理第一次按下即可。

## 一 . 键盘按键

#### 如何来判断中断来自于键盘？(代码如下)
```
    // gdt初始化操作...
    // fifo加载操作...
	if (fifo8_status(&keyfifo) != 0) {  // True则说明中断来自于键盘
		i = fifo8_get(&keyfifo);
		io_sti();
		// i 就是中断返回的值，分析他即可得到按键信息, 在下面我把它转换为了16进制存储在了一个char array s中
	    sprintf(s, "%02X", i);
	    // 把两次中断变为一次，看下文
	}
```
#### 得到了s，就是得到了键盘按键的信息。

#### 开头说了，按下一次，会有两次中断发生，那么我们是否可以使用一个flag来区分按下和弹起呢？

```
	if (flag){
	    keyshow(); // 显示这次按键，把按下的中断当作一个键位的信息，把弹起的中断用下面flag的方法屏蔽掉
	}
	// 屏蔽
	if(flag==1){
		flag = 0 ;
	}else {
		flag = 1 ;
	}
```
#### 这是一个很拙略的实现方法，而且我测试了几次之后发现有一个bug，就是同时按下两个键位时，屏蔽的方法就会变成另一种。
#### 比如开始是用按下识别一个键位，那么同时按下两个键位之后就是以弹起的方法来识别键位了。

#### 这个情况留在之后再考虑。

## 二 . 按键识别

#### 上文中已经将按键返回的数据存储到了char数组s，只需要在屏幕上显示s的数据就可以了。

```
int write_x = 55 ; //按键显示位置的x，y坐标
int write_y = 57 ;

void key(struct BOOTINFO *binfo,char s[40]){
    //在指定位置显示数据
    showkeys(binfo->vram, binfo->scrnx,  write_x,  write_y, COL8_FFFFFF, s);
    // 显示之后光标右移
    write_x += 19 ;
    // 如果超出右边界，换行
	if(write_x>155){
		write_x = 55 ;
		write_y += 19 ;
	}
	// 如果超出下边界，刷新清理本页，开启新的一页
	if(write_y>180){
 		new_pe(binfo);
	}

}
```
### 结果：
![print](https://www.cnblogs.com/images/cnblogs_com/LexMoon/1246510/o_print.jpg)

#### 很明显，我们需要编写一种转换机制，将表示16进制的数据对应成为键盘按键。

#### 键盘上需要显示的有字母和特殊符号，还有一些功能性的按键shift，backspace等。

#### 测试记录了几个按键的按下数据
>  键盘    按下  

>   F1      3B   

>   F2      3C   

>   F3      3D   

>   F4      3E   

>   A       1E   

>   B       30   

>   Backspace  OE

>   空格      39

#### 既然已经知道了对应关系，那么很容易就可以建立一种对应。

#### 先来实现这几个特殊按键功能 

#### 我打算将 F1 实现为 clear 功能，实现页面刷新 。 Backspace 实现回退功能。Enter实现确定以及回车功能。

```
void showkey(struct BOOTINFO *binfo,char s[40]){
	// 回车键 
	if(strcmp(s,"1C")==0){ 
			write_x = 55 ;  // 光标移动至下一行起始位置。
			write_y += 19 ;
			showkeys(binfo->vram, binfo->scrnx, 0, write_y, COL8_FFFFFF, "AntzOS>");
	}
	// F1   刷新本页
	else if(strcmp(s,"3B")==0){
			new_pe(binfo);
	}
	// 空格  光标后移一位
	else if(strcmp(s,"39")==0){
			showkeys(binfo->vram, binfo->scrnx,  write_x,  write_y, COL8_FFFFFF, " ");
			write_x += 19 ;
	}
	// Backspace 删除退格
	else if(strcmp(s,"0E")==0){
			// 回退
			write_x -= 19 ;
			//重新覆盖这片区域
			area_flash(binfo->vram, binfo->scrnx , COL8_000000,  write_x,     write_y,     write_x+19, write_y+19);
	}
	// 其他按键
	else {
			showkeys(binfo->vram, binfo->scrnx,  write_x,  write_y, COL8_FFFFFF, s);
			write_x += 19 ;
	}
	if(write_x>155){
		write_x = 55 ;
		write_y += 19 ;
		//putfonts8_asc(binfo->vram, binfo->scrnx, 4, 57, COL8_FFFFFF, "AntzOS>");
	}
	if(write_y>180){
 		new_pe(binfo);
	}

}
```

#### 字母识别同理，当然可以比上面实现的更加完善更加简洁，但我仓促之下就只能先做到这一步。

## 三 . Bug引发的思考

#### 这里开始就和AntzOs实现没有多少联系了，不过在我测试按键中断时候发现了很多奇怪的小问题。

>   Caps Lock(大小写键) 是否开启并不会影响中断对你一个按键的返回信息，也就是所谓的大小写中断其实是无法区分的，那么现代系统如何区分呢？ 同理于上面我们区分按下和弹起两次中断，我们可以将Caps Lock键的状态获取到，从而对当前按键进行所谓的大小写区分。

>   按下两次导致规则置换，会不会是因为中断响应的时间导致的。

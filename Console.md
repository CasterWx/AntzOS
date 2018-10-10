# 自制操作系统Antz day10——实现shell
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
[!s](https://www.cnblogs.com/images/cnblogs_com/LexMoon/1246510/o_project.jpg)

#### console的图形化实现与规则均在main/bootpack.c中完成

#### interrupt/int.c 中实现了键盘中断处理，按键会中断两次，一次按下，一次弹起，在响应处理中，只需要处理第一次按下即可。

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


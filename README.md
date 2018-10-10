# AntzOS

## 引子
> 最近在看一些操作系统的资料，最开始的为什么是07c00h这个问题就让我对操作系统有了很大的兴趣。所以准备在看象书之余顺便写一个操作系统(Anz)。至于为什么这个系统会被叫做Antz,可以参考Antz Uhl Kone, 日语为アインズ·ウール·ゴウン ， 与之对应的还有接下来准备写的自制脚本语言AntzScript，因为准备是用Java实现解释器，所以如何把AntzScript运行在Antz上是一个很大问题（其实问题就是引入Java）。

* Antz系统更新地址： [这里是链接](https://www.cnblogs.com/LexMoon/category/1262287.html)

#### [mbr实现](https://www.cnblogs.com/LexMoon/p/antz01.html)

#### [保护模式](https://www.cnblogs.com/LexMoon/p/antz02.html)

#### [显存](https://www.cnblogs.com/LexMoon/p/antz03.html)

#### [硬盘](https://www.cnblogs.com/LexMoon/p/antz04.html)

#### [进入保护模式](https://www.cnblogs.com/LexMoon/p/antz05.html)

#### [引入c语言](https://www.cnblogs.com/LexMoon/p/antz09.html)

#### [shell实现](https://github.com/CasterWx/AntzOS/blob/master/main/Console.md)

 
## 用了一个简单的字符库

```
char.bin : char.txt Makefile
	$(MAKEFONT) char.txt char.bin

char.set : char.bin Makefile
	$(BIN2OBJ) char.bin assic/char.set _hankaku
```
### 我把char.set直接存放在了assic文件夹中，直接拿来使用，不过只是ASSIC支持的字符，中文字符库找不到


## 去除鼠标

### 在鼠标存在的情况下，移动会导致后面的界面发生变化，图形化界面虽然容易保存现场结果，但终端部分缺会失去部分显示结果


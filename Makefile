INCPATH  = include/
MAKE     = make.exe
COPY     = copy
DEL      = del
TREE     = tree /f
Antz_kernel = main/bootpack.obj asmfunc/naskfunc.obj assic/char.set windows/graphic.obj lgdt/set_lgdt.obj interrupt/int.obj io/fifo.obj thun/thun.obj task/timer.obj

default :
	$(MAKE) img

# 需要的
mbr.bin : boot/mbr.asm Makefile
	nask.exe boot/mbr.asm mbr.bin mbr.lst

asmhead.bin : boot/asmhead.asm Makefile
	nask.exe boot/asmhead.asm asmhead.bin asmhead.lst

# naskfunc.obj会放在asmfunc目录中，如果在bootpack.bim合成时候不到，not find naskfunc会报错一万次！！！2018年9月9日15:46:27
naskfunc.obj : asmfunc/naskfunc.nas Makefile
	nask.exe asmfunc/naskfunc.nas asmfunc/naskfunc.obj asmfunc/naskfunc.lst

%.gas : %.c Makefile
	cc1.exe -I include/ -Os -Wall -quiet -o $*.gas $*.c

%.nas : %.gas Makefile
	gas2nask.exe -a $*.gas $*.nas

%.obj : %.nas Makefile
	nask.exe $*.nas $*.obj $*.lst

bootpack.bim : $(Antz_kernel) Makefile
	obj2bim.exe  @include/haribote.rul out:bootpack.bim stack:3136k map:bootpack.map $(Antz_kernel)

bootpack.hrb : bootpack.bim Makefile
	bim2hrb.exe bootpack.bim bootpack.hrb 0

antz.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb antz.sys

antz.img : mbr.bin antz.sys Makefile
	edimg.exe  imgin:include/fdimg0at.tek wbinimg src:mbr.bin len:512 from:0 to:0  copy from:antz.sys to:@: imgout:antz.img

# 指令
img :
	$(MAKE) antz.img

run :
	$(MAKE) img
	$(MAKE) clean

clean :
	-$(DEL) *.bin
	-$(DEL) *.lst

	-$(DEL) asmfunc\naskfunc.lst
	-$(DEL) asmfunc\naskfunc.obj

	-$(DEL) interrupt\int.lst
	-$(DEL) interrupt\int.obj

	-$(DEL) io\fifo.lst
	-$(DEL) io\fifo.obj

	-$(DEL) lgdt\set_lgdt.lst
	-$(DEL) lgdt\set_lgdt.obj

	-$(DEL) main\bootpack.lst
	-$(DEL) main\bootpack.obj

	-$(DEL) windows\graphic.lst
	-$(DEL) windows\graphic.obj

	-$(DEL) thun\thun.lst
	-$(DEL) thun\thun.obj

	-$(DEL) task\timer.lst
	-$(DEL) task\timer.obj
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) bootpack.hrb
	-$(DEL) antz.sys

tree :
	$(TREE)

OBJS_BOOTPACK = main/bootpack.obj asmfunc/naskfunc.obj assic/char.set windows/graphic.obj lgdt/set_lgdt.obj \
		interrupt/int.obj io/fifo.obj

INCPATH  = lib/

MAKE     = make.exe -r

NASK     = nask.exe
CC1      = cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = gas2nask.exe -a
OBJ2BIM  = obj2bim.exe
MAKEFONT = makefont.exe
BIN2OBJ  = bin2obj.exe
BIM2HRB  = bim2hrb.exe
RULEFILE = $(INCPATH)haribote.rul
EDIMG    = edimg.exe
IMGTOL   = imgtol.com
COPY     = copy
DEL      = del
TREE     = tree /f
# 默认动作

default :
	$(MAKE) img

# 镜像文件生成

mbr.bin : boot/mbr.asm Makefile
	$(NASK) boot/mbr.asm mbr.bin mbr.lst

asmhead.bin : boot/asmhead.asm Makefile
	$(NASK) boot/asmhead.asm asmhead.bin asmhead.lst

# naskfunc.obj会放在asmfunc目录中，莫得在bootpack.bim合成时候瞎找，不然not find naskfunc会报错一万次！！！2018年9月9日15:46:27
naskfunc.obj : asmfunc/naskfunc.nas Makefile
	$(NASK) asmfunc/naskfunc.nas asmfunc/naskfunc.obj asmfunc/naskfunc.lst


%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

char.bin : char.txt Makefile
	$(MAKEFONT) char.txt char.bin

char.set : char.bin Makefile
	$(BIN2OBJ) char.bin assic/char.set _hankaku


bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)


# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

antz.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb antz.sys

antz.img : mbr.bin antz.sys Makefile
	$(EDIMG)   imgin:lib/fdimg0at.tek \
		wbinimg src:mbr.bin len:512 from:0 to:0 \
		copy from:antz.sys to:@: \
		imgout:antz.img

# 其他指令

img :
	$(MAKE) antz.img

run :
	$(MAKE) img
	$(MAKE) clean

install :
	$(MAKE) img
	$(IMGTOL) w a: antz.img

clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.gas
	-$(DEL) *.obj

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

	-$(DEL) bootpack.nas
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) bootpack.hrb
	-$(DEL) antz.sys

src_only :
	$(MAKE) clean
	-$(DEL) antz.img

tree :
	$(TREE)

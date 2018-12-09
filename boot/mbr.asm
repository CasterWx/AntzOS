;	MBR是BIOS接力的第一棒，在他之后，会由分区引导扇区DBR接力，至于为什么不直接给DBR。
;	那是因为BIOS大小有限，无法完成所有操作，在给DBR之前会通过MBR完成。
;   操作系统算是一个中断驱动的死循环，在不考虑中断的情况下，死循环的方法很多
;	times 510-($-$$) db 0  _OR_  JMP

CYLS	EQU		10
		ORG		0x7c00
; 为什么是7c00 : 参考我的博客 https://www.cnblogs.com/LexMoon/p/antz01.html
		; 32kb - 1kb
		JMP		begin			; 跳转到主程序

; 软盘FAT12格式专用的代码，格式固定，主要是一些信息存储
		DB		0x90
		DB		"Antz_Mbr"
		DW		512
		DB		1
		DW		1
		DB		2
		DW		224
		DW		2880
		DB		0xf0
		DW		9
		DW		18
		DW		2
		DD		0
		DD		2880
		DB		0,0,0x29
		DD		0xffffffff
		DB		"AntzUhlMBR "
		DB		"FAT12   "
		RESB	18

; MBR主体
begin:
		MOV		AX,0			; 初始化寄存器
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
		; 将ds，es和cs指向相同的地址
; mov ax,cs
; mov ds,ax
; mov es,ax
; call doSomeThing
; jmp $
		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0
		MOV		DH,0
		MOV		CL,2

error_while_loop:  ; 次数
		MOV		SI,0

retry:
		MOV		AH,0x02
		MOV		AL,1
		MOV		BX,0
		MOV		DL,0x00
		INT		0x13
		JNC		next
		ADD		SI,1
		CMP		SI,5
		JAE		error
		MOV		AH,0x00
		MOV		DL,0x00
		INT		0x13
		JMP		retry

next:
		MOV		AX,ES
		ADD		AX,0x0020
		MOV		ES,AX
		ADD		CL,1
		CMP		CL,18
		JBE		error_while_loop
		MOV		CL,1
		ADD		DH,1
		CMP		DH,2
		JB		error_while_loop
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB		error_while_loop

; 读取完毕
		MOV		[0x0ff0],CH
		JMP		0xc200

error:
		MOV		SI,msg

putloop:
		MOV		AL,[SI]
		ADD		SI,1			; 给SI加1
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 显示一个文字
		MOV		BX,15			; 指定字符颜色
		INT		0x10			; 调用显卡BIOS
		JMP		putloop

fin:
		HLT
		JMP		fin

wait_key:
		DB    0x13,0x13

msg:
		DB		0x0a, 0x0a		; 换行两次
		DB		"load error"
		DB		0x0a			; 换行
		DB		0
		RESB	0x7dfe-$
		DB		0x55, 0xaa    ; Mbr标志魔数，具体意义参考博客 https://www.cnblogs.com/LexMoon/p/antz01.html

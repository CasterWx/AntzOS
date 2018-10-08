
org  0100h

;================================================================================================
BaseOfStack		equ	0100h

BaseOfKernelFile	equ	 08000h	; KERNEL.BIN �����ص���λ�� ----  �ε�ַ
OffsetOfKernelFile	equ	     0h	; KERNEL.BIN �����ص���λ�� ---- ƫ�Ƶ�ַ

;================================================================================================

	jmp	LABEL_START		; Start

; ������ FAT12 ���̵�ͷ, ֮���԰���������Ϊ�����õ��˴��̵�һЩ��Ϣ
%include	"fat12hdr.inc"


LABEL_START:			; <--- �����￪ʼ *************
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack

	mov	dh, 0			; "Loading  "
	call	DispStr			; ��ʾ�ַ���

	; ������ A �̵ĸ�Ŀ¼Ѱ�� KERNEL.BIN
	mov	word [wSectorNo], SectorNoOfRootDirectory
	xor	ah, ah	; ��
	xor	dl, dl	; �� ������λ
	int	13h	; ��
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
	cmp	word [wRootDirSizeForLoop], 0	; ��
	jz	LABEL_NO_KERNELBIN		; �� �жϸ�Ŀ¼���ǲ����Ѿ�����, ����������ʾû���ҵ� KERNEL.BIN
	dec	word [wRootDirSizeForLoop]	; ��
	mov	ax, BaseOfKernelFile
	mov	es, ax			; es <- BaseOfKernelFile
	mov	bx, OffsetOfKernelFile	; bx <- OffsetOfKernelFile	����, es:bx = BaseOfKernelFile:OffsetOfKernelFile = BaseOfKernelFile * 10h + OffsetOfKernelFile
	mov	ax, [wSectorNo]		; ax <- Root Directory �е�ĳ Sector ��
	mov	cl, 1
	call	ReadSector

	mov	si, KernelFileName	; ds:si -> "KERNEL  BIN"
	mov	di, OffsetOfKernelFile	; es:di -> BaseOfKernelFile:???? = BaseOfKernelFile*10h+????
	cld
	mov	dx, 10h
LABEL_SEARCH_FOR_KERNELBIN:
	cmp	dx, 0					; ��
	jz	LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR	; �� ѭ����������, �����Ѿ�������һ�� Sector, ��������һ�� Sector
	dec	dx					; ��
	mov	cx, 11
LABEL_CMP_FILENAME:
	cmp	cx, 0			; ��
	jz	LABEL_FILENAME_FOUND	; �� ѭ����������, �����Ƚ��� 11 ���ַ�������, ��ʾ�ҵ�
	dec	cx			; ��
	lodsb				; ds:si -> al
	cmp	al, byte [es:di]	; if al == es:di
	jz	LABEL_GO_ON
	jmp	LABEL_DIFFERENT
LABEL_GO_ON:
	inc	di
	jmp	LABEL_CMP_FILENAME	;	����ѭ��

LABEL_DIFFERENT:
	and	di, 0FFE0h		; else��	��ʱdi��ֵ��֪����ʲô, di &= e0 Ϊ�������� 20h �ı���
	add	di, 20h			;     ��
	mov	si, KernelFileName	;     �� di += 20h  ��һ��Ŀ¼��Ŀ
	jmp	LABEL_SEARCH_FOR_KERNELBIN;   ��

LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
	add	word [wSectorNo], 1
	jmp	LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_KERNELBIN:
	mov	dh, 2			; "No KERNEL."
	call	DispStr			; ��ʾ�ַ���
%ifdef	_LOADER_DEBUG_
	mov	ax, 4c00h		; ��
	int	21h			; ��û���ҵ� KERNEL.BIN, �ص� DOS
%else
	jmp	$			; û���ҵ� KERNEL.BIN, ��ѭ��������
%endif

LABEL_FILENAME_FOUND:			; �ҵ� KERNEL.BIN ����������������
	mov	ax, RootDirSectors
	and	di, 0FFF0h		; di -> ��ǰ��Ŀ�Ŀ�ʼ

	push	eax
	mov	eax, [es : di + 01Ch]		; ��
	mov	dword [dwKernelSize], eax	; ������ KERNEL.BIN �ļ���С
	pop	eax

	add	di, 01Ah		; di -> �� Sector
	mov	cx, word [es:di]
	push	cx			; ������ Sector �� FAT �е�����
	add	cx, ax
	add	cx, DeltaSectorNo	; ��ʱ cl ������ LOADER.BIN ����ʼ������ (�� 0 ��ʼ��������)
	mov	ax, BaseOfKernelFile
	mov	es, ax			; es <- BaseOfKernelFile
	mov	bx, OffsetOfKernelFile	; bx <- OffsetOfKernelFile	����, es:bx = BaseOfKernelFile:OffsetOfKernelFile = BaseOfKernelFile * 10h + OffsetOfKernelFile
	mov	ax, cx			; ax <- Sector ��

LABEL_GOON_LOADING_FILE:
	push	ax			; ��
	push	bx			; ��
	mov	ah, 0Eh			; �� ÿ��һ���������� "Loading  " ������һ����, �γ�������Ч��:
	mov	al, '.'			; ��
	mov	bl, 0Fh			; �� Loading ......
	int	10h			; ��
	pop	bx			; ��
	pop	ax			; ��

	mov	cl, 1
	call	ReadSector
	pop	ax			; ȡ���� Sector �� FAT �е�����
	call	GetFATEntry
	cmp	ax, 0FFFh
	jz	LABEL_FILE_LOADED
	push	ax			; ���� Sector �� FAT �е�����
	mov	dx, RootDirSectors
	add	ax, dx
	add	ax, DeltaSectorNo
	add	bx, [BPB_BytsPerSec]
	jmp	LABEL_GOON_LOADING_FILE
LABEL_FILE_LOADED:

	call	KillMotor		; �ر���������

	mov	dh, 1			; "Ready."
	call	DispStr			; ��ʾ�ַ���

	jmp	$


;============================================================================
;����
;----------------------------------------------------------------------------
wRootDirSizeForLoop	dw	RootDirSectors	; Root Directory ռ�õ�������
wSectorNo		dw	0		; Ҫ��ȡ��������
bOdd			db	0		; ��������ż��
dwKernelSize		dd	0		; KERNEL.BIN �ļ���С

;============================================================================
;�ַ���
;----------------------------------------------------------------------------
KernelFileName		db	"KERNEL  BIN", 0	; KERNEL.BIN ֮�ļ���
; Ϊ�򻯴���, ����ÿ���ַ����ĳ��Ⱦ�Ϊ MessageLength
MessageLength		equ	9
LoadMessage:		db	"Loading  "
Message1		db	"Ready.   "
Message2		db	"No KERNEL"
;============================================================================

;----------------------------------------------------------------------------
; ������: DispStr
;----------------------------------------------------------------------------
; ����:
;	��ʾһ���ַ���, ������ʼʱ dh ��Ӧ�����ַ�������(0-based)
DispStr:
	mov	ax, MessageLength
	mul	dh
	add	ax, LoadMessage
	mov	bp, ax			; ��
	mov	ax, ds			; �� ES:BP = ����ַ
	mov	es, ax			; ��
	mov	cx, MessageLength	; CX = ������
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; ҳ��Ϊ0(BH = 0) �ڵװ���(BL = 07h)
	mov	dl, 0
	add	dh, 3			; �ӵ� 3 ��������ʾ
	int	10h			; int 10h
	ret
;----------------------------------------------------------------------------
; ������: ReadSector
;----------------------------------------------------------------------------
; ����:
;	������(Directory Entry �е� Sector ��)Ϊ ax �ĵ� Sector ��ʼ, �� cl �� Sector ���� es:bx ��
ReadSector:
	; -----------------------------------------------------------------------
	; �������������������ڴ����е�λ�� (������ -> ������, ��ʼ����, ��ͷ��)
	; -----------------------------------------------------------------------
	; ��������Ϊ x
	;                           �� ������ = y >> 1
	;       x           �� �� y ��
	; -------------- => ��      �� ��ͷ�� = y & 1
	;  ÿ�ŵ�������     ��
	;                   �� �� z => ��ʼ������ = z + 1
	push	bp
	mov	bp, sp
	sub	esp, 2			; �ٳ������ֽڵĶ�ջ���򱣴�Ҫ����������: byte [bp-2]

	mov	byte [bp-2], cl
	push	bx			; ���� bx
	mov	bl, [BPB_SecPerTrk]	; bl: ����
	div	bl			; y �� al ��, z �� ah ��
	inc	ah			; z ++
	mov	cl, ah			; cl <- ��ʼ������
	mov	dh, al			; dh <- y
	shr	al, 1			; y >> 1 (��ʵ�� y/BPB_NumHeads, ����BPB_NumHeads=2)
	mov	ch, al			; ch <- ������
	and	dh, 1			; dh & 1 = ��ͷ��
	pop	bx			; �ָ� bx
	; ����, "������, ��ʼ����, ��ͷ��" ȫ���õ� ^^^^^^^^^^^^^^^^^^^^^^^^
	mov	dl, [BS_DrvNum]		; �������� (0 ��ʾ A ��)
.GoOnReading:
	mov	ah, 2			; ��
	mov	al, byte [bp-2]		; �� al ������
	int	13h
	jc	.GoOnReading		; ������ȡ���� CF �ᱻ��Ϊ 1, ��ʱ�Ͳ�ͣ�ض�, ֱ����ȷΪֹ

	add	esp, 2
	pop	bp

	ret

;----------------------------------------------------------------------------
; ������: GetFATEntry
;----------------------------------------------------------------------------
; ����:
;	�ҵ�����Ϊ ax �� Sector �� FAT �е���Ŀ, �������� ax ��
;	��Ҫע������, �м���Ҫ�� FAT �������� es:bx ��, ���Ժ���һ��ʼ������ es �� bx
GetFATEntry:
	push	es
	push	bx
	push	ax
	mov	ax, BaseOfKernelFile	; ��
	sub	ax, 0100h		; �� �� BaseOfKernelFile �������� 4K �ռ����ڴ��� FAT
	mov	es, ax			; ��
	pop	ax
	mov	byte [bOdd], 0
	mov	bx, 3
	mul	bx			; dx:ax = ax * 3
	mov	bx, 2
	div	bx			; dx:ax / 2  ==>  ax <- ��, dx <- ����
	cmp	dx, 0
	jz	LABEL_EVEN
	mov	byte [bOdd], 1
LABEL_EVEN:;ż��
	xor	dx, dx			; ���� ax ���� FATEntry �� FAT �е�ƫ����. ���������� FATEntry ���ĸ�������(FATռ�ò�ֹһ������)
	mov	bx, [BPB_BytsPerSec]
	div	bx			; dx:ax / BPB_BytsPerSec  ==>	ax <- ��   (FATEntry ���ڵ����������� FAT ��˵��������)
					;				dx <- ���� (FATEntry �������ڵ�ƫ��)��
	push	dx
	mov	bx, 0			; bx <- 0	����, es:bx = (BaseOfKernelFile - 100):00 = (BaseOfKernelFile - 100) * 10h
	add	ax, SectorNoOfFAT1	; �˾�ִ��֮���� ax ���� FATEntry ���ڵ�������
	mov	cl, 2
	call	ReadSector		; ��ȡ FATEntry ���ڵ�����, һ�ζ�����, �����ڱ߽緢������, ��Ϊһ�� FATEntry ���ܿ�Խ��������
	pop	dx
	add	bx, dx
	mov	ax, [es:bx]
	cmp	byte [bOdd], 1
	jnz	LABEL_EVEN_2
	shr	ax, 4
LABEL_EVEN_2:
	and	ax, 0FFFh

LABEL_GET_FAT_ENRY_OK:

	pop	bx
	pop	es
	ret
;----------------------------------------------------------------------------


;----------------------------------------------------------------------------
; ������: KillMotor
;----------------------------------------------------------------------------
; ����:
;	�ر���������
KillMotor:
	push	dx
	mov	dx, 03F2h
	mov	al, 0
	out	dx, al
	pop	dx
	ret
;----------------------------------------------------------------------------

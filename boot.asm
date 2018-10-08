
;%define	_BOOT_DEBUG_	; �� Boot Sector ʱһ��������ע�͵�!�����д򿪺��� nasm Boot.asm -o Boot.com ����һ��.COM�ļ����ڵ���

%ifdef	_BOOT_DEBUG_
	org  0100h			; ����״̬, ���� .COM �ļ�, �ɵ���
%else
	org  07c00h			; Boot ״̬, Bios ���� Boot Sector ���ص� 0:7C00 ������ʼִ��
%endif

%ifdef	_BOOT_DEBUG_
BaseOfStack		equ	0100h	; ����״̬�¶�ջ����ַ(ջ��, ������λ�����͵�ַ����)
%else
BaseOfStack		equ	07c00h	; Boot״̬�¶�ջ����ַ(ջ��, ������λ�����͵�ַ����)
%endif

BaseOfLoader		equ	09000h	; LOADER.BIN �����ص���λ�� ----  �ε�ַ
OffsetOfLoader		equ	0100h	; LOADER.BIN �����ص���λ�� ---- ƫ�Ƶ�ַ

	jmp short LABEL_START		; Start to boot.
	nop				; ���� nop ������

; ������ FAT12 ���̵�ͷ, ֮���԰���������Ϊ�����õ��˴��̵�һЩ��Ϣ
%include	"fat12hdr.inc"

LABEL_START:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack

	; ����
	mov	ax, 0600h		; AH = 6,  AL = 0h
	mov	bx, 0700h		; �ڵװ���(BL = 07h)
	mov	cx, 0			; ���Ͻ�: (0, 0)
	mov	dx, 0184fh		; ���½�: (80, 50)
	int	10h			; int 10h

	mov	dh, 0			; "Booting  "
	call	DispStr			; ��ʾ�ַ���

	xor	ah, ah	; ��
	xor	dl, dl	; �� ������λ
	int	13h	; ��

; ������ A �̵ĸ�Ŀ¼Ѱ�� LOADER.BIN
	mov	word [wSectorNo], SectorNoOfRootDirectory
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
	cmp	word [wRootDirSizeForLoop], 0	; ��
	jz	LABEL_NO_LOADERBIN		; �� �жϸ�Ŀ¼���ǲ����Ѿ�����
	dec	word [wRootDirSizeForLoop]	; �� ����������ʾû���ҵ� LOADER.BIN
	mov	ax, BaseOfLoader
	mov	es, ax			; es <- BaseOfLoader
	mov	bx, OffsetOfLoader	; bx <- OffsetOfLoader	����, es:bx = BaseOfLoader:OffsetOfLoader
	mov	ax, [wSectorNo]	; ax <- Root Directory �е�ĳ Sector ��
	mov	cl, 1
	call	ReadSector

	mov	si, LoaderFileName	; ds:si -> "LOADER  BIN"
	mov	di, OffsetOfLoader	; es:di -> BaseOfLoader:0100 = BaseOfLoader*10h+100
	cld
	mov	dx, 10h
LABEL_SEARCH_FOR_LOADERBIN:
	cmp	dx, 0										; ��ѭ����������,
	jz	LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR	; �������Ѿ�������һ�� Sector,
	dec	dx											; ����������һ�� Sector
	mov	cx, 11
LABEL_CMP_FILENAME:
	cmp	cx, 0
	jz	LABEL_FILENAME_FOUND	; �����Ƚ��� 11 ���ַ�������, ��ʾ�ҵ�
dec	cx
	lodsb				; ds:si -> al
	cmp	al, byte [es:di]
	jz	LABEL_GO_ON
	jmp	LABEL_DIFFERENT		; ֻҪ���ֲ�һ�����ַ��ͱ����� DirectoryEntry ����
; ����Ҫ�ҵ� LOADER.BIN
LABEL_GO_ON:
	inc	di
	jmp	LABEL_CMP_FILENAME	;	����ѭ��

LABEL_DIFFERENT:
	and	di, 0FFE0h						; else ��	di &= E0 Ϊ������ָ������Ŀ��ͷ
	add	di, 20h							;     ��
	mov	si, LoaderFileName					;     �� di += 20h  ��һ��Ŀ¼��Ŀ
	jmp	LABEL_SEARCH_FOR_LOADERBIN;    ��

LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
	add	word [wSectorNo], 1
	jmp	LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_LOADERBIN:
	mov	dh, 2			; "No LOADER."
	call	DispStr			; ��ʾ�ַ���
%ifdef	_BOOT_DEBUG_
	mov	ax, 4c00h		; ��
	int	21h			; ��û���ҵ� LOADER.BIN, �ص� DOS
%else
	jmp	$			; û���ҵ� LOADER.BIN, ��ѭ��������
%endif

LABEL_FILENAME_FOUND:			; �ҵ� LOADER.BIN ����������������
	mov	ax, RootDirSectors
	and	di, 0FFE0h		; di -> ��ǰ��Ŀ�Ŀ�ʼ
	add	di, 01Ah		; di -> �� Sector
	mov	cx, word [es:di]
	push	cx			; ������ Sector �� FAT �е�����
	add	cx, ax
	add	cx, DeltaSectorNo	; ��������ʱ cl �������� LOADER.BIN ����ʼ������ (�� 0 ��ʼ��������)
	mov	ax, BaseOfLoader
	mov	es, ax			; es <- BaseOfLoader
	mov	bx, OffsetOfLoader	; bx <- OffsetOfLoader	����, es:bx = BaseOfLoader:OffsetOfLoader = BaseOfLoader * 10h + OffsetOfLoader
	mov	ax, cx			; ax <- Sector ��

LABEL_GOON_LOADING_FILE:
	push	ax			; ��
	push	bx			; ��
	mov	ah, 0Eh			; �� ÿ��һ���������� "Booting  " ������һ����, �γ�������Ч��:
	mov	al, '.'			; ��
	mov	bl, 0Fh			; �� Booting ......
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

	mov	dh, 1			; "Ready."
	call	DispStr			; ��ʾ�ַ���

; *****************************************************************************************************
	jmp	BaseOfLoader:OffsetOfLoader	; ��һ����ʽ��ת���Ѽ��ص��ڴ��е� LOADER.BIN �Ŀ�ʼ��
						; ��ʼִ�� LOADER.BIN �Ĵ���
						; Boot Sector ��ʹ�����˽���
; *****************************************************************************************************



;============================================================================
;����
;----------------------------------------------------------------------------
wRootDirSizeForLoop	dw	RootDirSectors	; Root Directory ռ�õ�������, ��ѭ���л��ݼ�����.
wSectorNo		dw	0		; Ҫ��ȡ��������
bOdd			db	0		; ��������ż��

;============================================================================
;�ַ���
;----------------------------------------------------------------------------
LoaderFileName		db	"LOADER  BIN", 0	; LOADER.BIN ֮�ļ���
; Ϊ�򻯴���, ����ÿ���ַ����ĳ��Ⱦ�Ϊ MessageLength
MessageLength		equ	9
BootMessage:		db	"Booting  "; 9�ֽ�, �������ÿո�����. ���� 0
Message1		db	"Ready.   "; 9�ֽ�, �������ÿո�����. ���� 1
Message2		db	"No LOADER"; 9�ֽ�, �������ÿո�����. ���� 2
;============================================================================


;----------------------------------------------------------------------------
; ������: DispStr
;----------------------------------------------------------------------------
; ����:
;	��ʾһ���ַ���, ������ʼʱ dh ��Ӧ�����ַ�������(0-based)
DispStr:
	mov	ax, MessageLength
	mul	dh
	add	ax, BootMessage
	mov	bp, ax			; ��
	mov	ax, ds			; �� ES:BP = ����ַ
	mov	es, ax			; ��
	mov	cx, MessageLength	; CX = ������
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; ҳ��Ϊ0(BH = 0) �ڵװ���(BL = 07h)
	mov	dl, 0
	int	10h			; int 10h
	ret


;----------------------------------------------------------------------------
; ������: ReadSector
;----------------------------------------------------------------------------
; ����:
;	�ӵ� ax �� Sector ��ʼ, �� cl �� Sector ���� es:bx ��
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
	mov	ax, BaseOfLoader	; ��
	sub	ax, 0100h		; �� �� BaseOfLoader �������� 4K �ռ����ڴ��� FAT
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
	mov	bx, 0			; bx <- 0	����, es:bx = (BaseOfLoader - 100):00 = (BaseOfLoader - 100) * 10h
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

times 	510-($-$$)	db	0	; ����ʣ�µĿռ䣬ʹ���ɵĶ����ƴ���ǡ��Ϊ512�ֽ�
dw 	0xaa55				; ������־

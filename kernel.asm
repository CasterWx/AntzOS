[section .text]

global _start

_start:	; ������������ʱ�������Ǽ��� gs ָ���Դ�
	mov	ah, 0Fh				; 0000: �ڵ�    1111: ����
	mov	al, 'K'
	mov	[gs:((80 * 1 + 39) * 2)], ax	; ��Ļ�� 1 ��, �� 39 �С�
	jmp	$

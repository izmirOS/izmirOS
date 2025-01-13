[bits 16]
print_string:
	pusha

	.loop:
	mov al, [bx]
	cmp al, 0
	je .ret
	mov ah, 0x0E
	int 0x10
	inc bx
	jmp .loop

	.ret:
	mov ah, 0x0E
	mov al, 0x0A
	int 0x10
	mov al, 0x0D
	int 0x10

	popa
	ret

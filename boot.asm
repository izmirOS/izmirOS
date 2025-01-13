[org 0x7C00]
[bits 16]

KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl
call load_boot
call execute_boot

load_boot:
	pusha
	mov bx, LOADING_BOOT_MSG
	call print_string
	mov bx, KERNEL_OFFSET
	mov dh, 15
	mov dl, [BOOT_DRIVE]
	call disk_read
	popa
	ret

execute_boot:
	mov bx, EXECUTING_BOOT_MSG
	call print_string
	call KERNEL_OFFSET
	jmp $

disk_read:
	pusha
	push dx

	mov ah, 0x02
	mov al, dh
	mov ch, 0x00
	mov dh, 0x00
	mov cl, 0x02
	int 0x13

	jc disk_read_error

	pop dx
	cmp dh, al
	jne disk_read_error

	popa
	ret

disk_read_error:
	mov bx, DISK_READ_ERROR_MSG
	call print_string
	hlt

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

BOOT_DRIVE: db 0
LOADING_BOOT_MSG: db "Loading Boot", 0
EXECUTING_BOOT_MSG: db "Executing Boot", 0
DISK_READ_ERROR_MSG: db "Disk Error", 0

times 510 - ($ - $$) db 0
dw 0xAA55

bits 16
org 0x7c00
section .text
global start
start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x7D00
    mov [bootdrive], dl

    ; Reset disk system first
    xor ax, ax      ; ah = 0 (reset disk)
    mov dl, [bootdrive]
    int 0x13
    jc disk_error

    ; Read Stage 2
    xor bx, bx
    mov es, bx
    mov bx, 0x7E00      ; ES:BX = 0000:7E00
    
    mov ah, 0x02        ; Read sectors function
    mov al, 0x01        ; Read 1 sector
    mov ch, 0x00        ; Cylinder 0
    mov cl, 0x02        ; Sector 2 (1-based)
    mov dh, 0x00        ; Head 0
    mov dl, [bootdrive] ; Drive number

    int 0x13
    jc disk_error

    sti
    jmp 0x7E00

disk_error:
    ; Print error code from ah
    push ax
    mov ah, 0x0e
    mov al, 'E'
    int 0x10
    pop ax
    
    mov al, ah      ; Error code is in ah
    add al, '0'     ; Convert to ASCII
    mov ah, 0x0e
    int 0x10
    
    cli
    hlt

bootdrive: db 0
times 510-($-$$) db 0
dw 0xaa55
bits 16
org 0x7E00

start:
    
    mov ah, 0x0e
    mov al, '>'
    int 0x10

    mov ah, 0x13
    mov al, 1
    mov bh, 0
    mov bl, 0x07
    mov cx, 14
    mov dh, 12
    mov dl, 0
    mov bp, message
    int 0x10

    cli
    hlt

message: db 'Hello from S2!', 0
times 512-($-$$) db 0
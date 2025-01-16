[bits 16]


load_idt:
    push ebp
    mov ebp, esp
    mov eax, [esp + 8]    ; Get IDT descriptor address
    lidt [eax]            ; Load IDT register
    pop ebp
    ret

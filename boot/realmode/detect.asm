[bits 16]

detect_memory:
    pusha
    mov di, 0x8000    ; Buffer address
    xor ebx, ebx      ; Start with EBX=0
    mov edx, 0x534D4150  ; 'SMAP' signature
    mov eax, 0xE820   ; Function code
    mov ecx, 24       ; Entry size
    int 0x15          ; Call BIOS
    
    ; Store entry count at known location
    mov word [0x7FF8], 1  ; Start with 1 entry
    
    .next_entry:
        add di, 24      ; Next position
        test ebx, ebx   ; Last entry?
        jz .done
        
        mov eax, 0xE820
        mov ecx, 24
        int 0x15
        jc .done
        
        ; Increment count
        inc word [0x7FF8]
        jmp .next_entry
        
    .done:
        popa
        ret

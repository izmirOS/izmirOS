[bits 32]

    ; Declare the structure we'll use to pass registers
struc registers_t
    .edi:       resd 1
    .esi:       resd 1
    .ebp:       resd 1
    .esp:       resd 1
    .ebx:       resd 1
    .edx:       resd 1
    .ecx:       resd 1
    .eax:       resd 1
    .ds:        resd 1
    .es:        resd 1
    .fs:        resd 1
    .gs:        resd 1
    .error_code: resd 1
    .eip:       resd 1
    .cs:        resd 1
    .eflags:    resd 1
    .user_esp:  resd 1
    .user_ss:   resd 1
endstruc

section .bss
    registers_storage: resb registers_t_size

section .text

global isr0
global isr1
global isr5
global isr6
global isr7
global isr8
global isr14
global isr32
global isr33
extern handle_isr0
extern handle_isr1
extern handle_isr5
extern handle_isr6
extern handle_isr7
extern handle_isr8
extern handle_isr14
extern handle_isr32
extern handle_isr33


; When CPU calls int handlers pushes EFLAGS -> CS -> EIP

isr0:
    pushad          ; Save all registers
    push ds        ; Save segments
    push es
    push fs
    push gs
    
    call handle_isr0
    
    pop gs         ; Restore segments
    pop fs
    pop es
    pop ds
    popad          ; Restore registers
    iret           ; Return (pops EIP, CS, and EFLAGS)

isr1:
    pushad          ; Save all registers
    push ds        ; Save segments
    push es
    push fs
    push gs
    
    call handle_isr1
    
    pop gs         ; Restore segments
    pop fs
    pop es
    pop ds
    popad          ; Restore registers
    iret           ; Return (pops EIP, CS, and EFLAGS)

isr5:
    pushad          ; Save all registers
    push ds        ; Save segments
    push es
    push fs
    push gs
    
    call handle_isr5
    
    pop gs         ; Restore segments
    pop fs
    pop es
    pop ds
    popad          ; Restore registers
    iret           ; Return (pops EIP, CS, and EFLAGS)

isr6:
    pushad          ; Save all registers
    push ds        ; Save segments
    push es
    push fs
    push gs
    
    call handle_isr6
    
    pop gs         ; Restore segments
    pop fs
    pop es
    pop ds
    popad          ; Restore registers
    iret           ; Return (pops EIP, CS, and EFLAGS)

isr7:
    pushad          ; Save all registers
    push ds        ; Save segments
    push es
    push fs
    push gs
    
    call handle_isr7
    
    pop gs         ; Restore segments
    pop fs
    pop es
    pop ds
    popad          ; Restore registers
    iret           ; Return (pops EIP, CS, and EFLAGS)

isr8:
    pushad          ; Save all registers
    push ds        ; Save segments
    push es
    push fs
    push gs
    
    call handle_isr8
    
    pop gs         ; Restore segments
    pop fs
    pop es
    pop ds
    popad          ; Restore registers
    iret           ; Return (pops EIP, CS, and EFLAGS)

isr14:
    ; Save all general-purpose registers
    mov [registers_storage + registers_t.edi], edi
    mov [registers_storage + registers_t.esi], esi
    mov [registers_storage + registers_t.ebp], ebp
    mov [registers_storage + registers_t.esp], esp
    mov [registers_storage + registers_t.ebx], ebx
    mov [registers_storage + registers_t.edx], edx
    mov [registers_storage + registers_t.ecx], ecx
    mov [registers_storage + registers_t.eax], eax
    
    ; Save segment registers
    mov ax, ds
    mov [registers_storage + registers_t.ds], ax
    mov ax, es
    mov [registers_storage + registers_t.es], ax
    mov ax, fs
    mov [registers_storage + registers_t.fs], ax
    mov ax, gs
    mov [registers_storage + registers_t.gs], ax
    
    ; Get the error code (pushed by CPU for page faults)
    mov eax, [esp]
    mov [registers_storage + registers_t.error_code], eax
    
    ; Get the instruction pointer and other CPU-pushed values
    mov eax, [esp + 4]  ; EIP
    mov [registers_storage + registers_t.eip], eax
    mov eax, [esp + 8]  ; CS
    mov [registers_storage + registers_t.cs], eax
    mov eax, [esp + 12] ; EFLAGS
    mov [registers_storage + registers_t.eflags], eax
    
    ; If this is a ring transition (user -> kernel), these will exist
    ; Otherwise they'll be garbage, but that's okay for kernel faults
    mov eax, [esp + 16] ; User ESP
    mov [registers_storage + registers_t.user_esp], eax
    mov eax, [esp + 20] ; User SS
    mov [registers_storage + registers_t.user_ss], eax
    
    ; Push the address of our registers structure as parameter
    push registers_storage
    call handle_isr14
    add esp, 4  ; Clean up parameter
    
    ; No need to restore segment registers if we're killing the process
    
    ; Skip over error code (already on stack from CPU)
    add esp, 4
    iret

isr32:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call handle_isr32
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret
isr33:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call handle_isr33
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret


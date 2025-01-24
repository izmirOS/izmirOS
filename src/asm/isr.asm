[bits 32]

section .text

global isr0
global isr1
global isr5
global isr6
global isr7
global isr8
global isr32
global isr33
extern handle_isr0
extern handle_isr1
extern handle_isr5
extern handle_isr6
extern handle_isr7
extern handle_isr8
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


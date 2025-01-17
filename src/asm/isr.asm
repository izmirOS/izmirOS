[bits 32]

section .text

global isr0
global isr33
extern handle_isr0'


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

isr33:
    pushad
    call handle_isr33
    popad
    iret
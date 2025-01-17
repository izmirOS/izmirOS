bits [32]
global isr0
extern handle_isr0

// When CPU calls int handlers pushes EFLAGS -> CS -> EIP

isr0:
    pushad
    call handle_isr0 
    popad
    iret


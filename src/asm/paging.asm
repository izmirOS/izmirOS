[bits 32]

section .text

global enable_32bit_paging

extern page_directory
extern init_paging_structures

enable_32bit_paging:
    pushad

    call init_paging_structures
    mov eax, page_directory
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    popad
    ret



[org 0x7C00]
[bits 16]

KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl
call load_kernel

call switch_to_pm
    
; no memory initially
load_kernel:
	pusha
	; mov bx, LOADING_BOOT_MSG
	; call print_string
	mov bx, KERNEL_OFFSET
	mov dh, 15
	mov dl, [BOOT_DRIVE]
	call disk_read
	popa
	ret

[bits 32]
%define BOOT_INFO_ADDR 0x9000
%define BOOT_MODE_32 32
%define BOOT_MODE_64 64

begin_pm:
    ;; Check if we can move from Protected Mode to Long Mode
    ;; If something went wrong (detect_lm shouldn't return at all)
    ;; we call execute_kernel in x32 Protected Mode
    call detect_lm

    mov edi, BOOT_INFO_ADDR
    xor eax, eax
    mov ecx, 10        ; clear 40 bytes
    rep stosd
    
    mov edi, BOOT_INFO_ADDR
    mov DWORD [edi + 0x20], BOOT_MODE_32
    call execute_kernel

[bits 64]
begin_lm:
    ;; In case, if detect_lm and switch_to_lm works fine, call kernel in x64 mode
    ;; Boot info for 64-bit mode was already set up in switch_to_lm
    mov rdi, BOOT_INFO_ADDR
    mov DWORD [rdi + 0x20], BOOT_MODE_64  ; Set 64-bit boot mode

    call execute_kernel

execute_kernel:
    call KERNEL_OFFSET

;; we have direct interrupts here
;; ability to use disk interrupt
;; and teletype interrupt
;; provided and only available in
;; real mode
%include "boot/realmode/disk.asm"

;; we have to define a global descriptor table
;; this provides data and code segment descriptions
;; for both the kernel and a uspace
;; we set protected mode then do a far jump 
;; into kernel code seg
;; interrupts disabled after this
%include "boot/protectedmode/gdt.asm"
%include "boot/protectedmode/switch.asm"


;; detect then
;; set up 4 level paging
;; and switch to 64-bit
%include "boot/longmode/detect.asm"
%include "boot/longmode/switch.asm"

BOOT_DRIVE: db 0
LOADING_BOOT_MSG: db "Loading Boot", 0
EXECUTING_BOOT_MSG: db "Executing Boot", 0

times 510 - ($ - $$) db 0
dw 0xAA55

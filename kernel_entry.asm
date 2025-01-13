global _start

[bits 16]
[extern kernel_main]

_start:
  call kernel_main
  jmp $

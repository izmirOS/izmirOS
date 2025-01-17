set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)


set(CMAKE_ASM_NASM_COMPILER nasm)
set(CMAKE_ASM_NASM_FLAGS "-f elf32")
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> ${CMAKE_ASM_NASM_FLAGS} -o <OBJECT> <SOURCE>")

# set(CMAKE_ASM_NASM_FLAGS "-f elf32")

set(CMAKE_C_COMPILER   i386-elf-gcc)
set(CMAKE_C_FLAGS "-O2 -g -ffreestanding -m32")

set(CMAKE_CXX_COMPILER i386-elf-g++)
set(CMAKE_CXX_FLAGS "-O2 -g -ffreestanding -m32 -fno-exceptions -fno-rtti")
# also linker: -Ttext=0x1000 -nostdlib -ffreestanding -m32 -Wl,--oformat=binary


.PHONY: all clean run debug lldb hexdump

# Default target
all: disk.img kernel

# Build the bootloader binaries
boot.bin: boot.asm
	nasm -f bin boot.asm -o boot.bin -g

boot2.bin: boot2.asm
	nasm -f bin boot2.asm -o boot2.bin -g

# Compile kernel
kernel: kernel.c linker.ld
    x86_64-elf-gcc -ffreestanding -m32 -O2 -Wall -Wextra -c kernel.c -o kernel.o
    x86_64-elf-gcc -T linker.ld -o kernel.elf -ffreestanding -m32 -nostdlib kernel.o -lgcc
    x86_64-elf-objcopy -O binary kernel.elf kernel.bin

# Create disk image
disk.img: boot.bin boot2.bin kernel
	dd if=/dev/zero of=disk.img bs=512 count=2880
	dd if=boot.bin of=disk.img bs=512 count=1 conv=notrunc
	dd if=boot2.bin of=disk.img bs=512 seek=1 conv=notrunc
	dd if=kernel.bin of=disk.img bs=512 seek=2 conv=notrunc

# Normal QEMU run
run: disk.img
	qemu-system-i386 -fda disk.img

# Run QEMU with debugger server
debug: disk.img
	qemu-system-i386 -fda disk.img -s -S

# Connect LLDB
lldb:
	lldb \
	--one-line "gdb-remote localhost:1234" \
	--one-line "settings set plugin.process.gdb-remote.target-definition-file i386.xml" \
	--one-line "break set -a 0x7c00" \
	--one-line "continue"

# Show disk contents
hexdump: disk.img
	@echo "=== First Sector (Bootloader) ==="
	@xxd -g 1 -l 512 disk.img
	@echo -e "\n=== Second Sector (Stage 2) ==="
	@xxd -g 1 -s 512 -l 512 disk.img
	@echo -e "\n=== Third Sector (Kernel) ==="
	@xxd -g 1 -s 1024 -l 512 disk.img

# Clean build artifacts
clean:
	rm -f *.bin *.o disk.img
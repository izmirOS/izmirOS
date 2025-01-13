ASM=nasm
CC=i386-elf-gcc

ASMFLAGS=-f elf32
CFLAGS=-O0 -g -ffreestanding -m32

BOOTFLAGS=-f bin
LDFLAGS=-Ttext=0x1000 -nostdlib -ffreestanding -m32 -Wl,--oformat=binary

OBJECTS=loader_entry.o loader.o
TARGET=image.bin

all: $(TARGET)

$(TARGET): boot.bin loader.bin
	cat boot.bin loader.bin > $(TARGET)

boot.bin: boot.asm
	$(ASM) $(BOOTFLAGS) $< -o $@

loader.bin: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

loader_entry.o: loader.asm
	$(ASM) $(ASMFLAGS) $< -o $@

loader.o: loader.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.bin $(TARGET)

run: $(TARGET)
	qemu-system-i386 -fda $(TARGET)

.PHONY: all clean

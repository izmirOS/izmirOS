ASM=nasm
CC=i386-elf-gcc

ASMFLAGS=-f elf32
CFLAGS=-O0 -g -ffreestanding -m32

BOOTFLAGS=-f bin
LDFLAGS=-Ttext=0x1000 -nostdlib -ffreestanding -m32 -Wl,--oformat=binary

SRC_DIR=..
BUILD_DIR=.

OBJECTS=$(BUILD_DIR)/loader_entry.o $(BUILD_DIR)/loader.o
TARGET=$(BUILD_DIR)/image.bin

all: $(TARGET)

$(TARGET): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/loader.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/loader.bin > $(TARGET)

$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot.asm
	$(ASM) $(BOOTFLAGS) $< -o $@

$(BUILD_DIR)/loader.bin: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/loader_entry.o: $(SRC_DIR)/loader.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/loader.o: $(SRC_DIR)/loader.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.bin $(TARGET)

run: $(TARGET)
	qemu-system-i386 -fda $(TARGET)

.PHONY: all clean


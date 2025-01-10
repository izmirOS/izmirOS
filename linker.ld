SECTIONS {
    . = 0x1000;
    .text : { *(.text) }
}
OUTPUT_FORMAT(binary)
void _start(void) {
    volatile char* video = (char*)0xB8000;

    const char* str = "Kernel Loaded!";
    for(int i = 0; str[i] != '\0'; i++) {
        video[i * 2] = str[i];          // Character
        video[i * 2 + 1] = 0x07;        // Light gray on black
    }
    
    while(1) {
        __asm__ volatile("hlt");
    }
}

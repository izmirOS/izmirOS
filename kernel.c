void _start(void) {
    char* video = (char*)0xb8000; // VGA text buffer
    *video = 'K';                 // Print K to show kernel loaded
    while(1);                     // Halt
}
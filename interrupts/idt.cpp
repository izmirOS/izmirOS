#include <stdint.h>

struct IDTEntry {
    uint16_t offset_low;    
    uint16_t segment;      
    uint8_t  reserved;     
    uint8_t  flags;       
    uint16_t offset_high;
}__attribute__((packed)); // exactly 8 bytes

void divide_by_zero_handler() {
    
}


int main(){

    IDTEntry interrupt_table[256]; // 8 * 256 in size

    IDTEntry div_by_zero;

    uint32_t handler_address = (uint32_t)&divide_by_zero_handler;  // Get actual address
    div_by_zero.offset_low = handler_address & 0xFFFF;              // Lower 16 bits
    div_by_zero.offset_high = (handler_address >> 16) & 0xFFFF;     // Upper 16 bits
    div_by_zero.segment = 0x08;
    
    interrupt_table[0] = div_by_zero;




    return 0;
}
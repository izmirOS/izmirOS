#pragma once

#include "../kernel/dev/vga.hpp"
#include "../shell/shell.hpp"
#include "../include/paging/paging.hpp"

#define KILL_PROCESS()                                                         \
  {                                                                            \
    __asm__ volatile("cli; hlt");                                              \
    for (;;)                                                                   \
      ;                                                                        \
  }

#define CMOS_ADDRESS 0x70
#define CMOS_READ 0x71



// this should be pulled into a class instance and cpp file
// for these:
/* ├╴W Variable 'term_instance' defined in a header file; variable definitions
 * in header files can lead to ODR violations clang-tidy
 * (misc-definitions-in-headers) [15, 16] */
/* ├╴W Function 'init_handlers' defined in a header file; function definitions
 * in header files can lead to ODR violations (fix available) clang-tidy
 * (misc-definitions-in-headers) [17, 6] */
/* ├╴W Function 'handle_isr0' defined in a header file; function definitions in
 * header files can lead to ODR violations (fix available) clang-tidy
 * (misc-definitions-in-headers) [25, 17] */
/* └╴W Function 'handle_isr33' defined in a header file; function definitions in
 * header files can lead to ODR violations (fix available) clang-tidy
 * (misc-definitions-in-headers) [32, 17] */
namespace interrupt_handlers {

vga::terminal *term_instance = nullptr;

void init_handlers(vga::terminal *term) { term_instance = term; }

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
  __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static const unsigned char scancode_s2_to_ascii[0xFF] = {
    0,        // 0x00
    0,        // 0x01
    '1',      // 0x02
    '2',      // 0x03
    '3',      // 0x04
    '4',      // 0x05
    '5',      // 0x06
    '6',      // 0x07
    '7',      // 0x08
    '8',      // 0x09
    '9',      // 0x0A
    '0',      // 0x0B
    '-',      // 0x0C
    '=',      // 0x0D
    '\b',     // 0x0E
    '\t',     // 0x0F
    'q',      // 0x10
    'w',      // 0x11
    'e',      // 0x12
    'r',      // 0x13
    't',      // 0x14
    'y',      // 0x15
    'u',      // 0x16
    'i',      // 0x17
    'o',      // 0x18
    'p',      // 0x19
    '[',      // 0x1A
    ']',      // 0x1B
    '\n',     // 0x1C
    0,        // 0x1D (Left Control)
    'a',      // 0x1E
    's',      // 0x1F
    'd',      // 0x20
    'f',      // 0x21
    'g',      // 0x22
    'h',      // 0x23
    'j',      // 0x24
    'k',      // 0x25
    'l',      // 0x26
    ';',      // 0x27
    '\'',     // 0x28
    '`',      // 0x29
    0,        // 0x2A (Left Shift)
    '\\',     // 0x2B
    'z',      // 0x2C
    'x',      // 0x2D
    'c',      // 0x2E
    'v',      // 0x2F
    'b',      // 0x30
    'n',      // 0x31
    'm',      // 0x32
    ',',      // 0x33
    '.',      // 0x34
    '/',      // 0x35
    0,        // 0x36 (Right Shift)
    '*',      // 0x37
    0,        // 0x38 (Left Alt)
    ' ',      // 0x39
    0,        // 0x3A (Caps Lock)
    0,        // 0x3B
    0,        // 0x3C
    0,        // 0x3D
    0,        // 0x3E
    0,        // 0x3F
    0,        // 0x40
    0,        // 0x41
    0,        // 0x42
    0,        // 0x43
    0,        // 0x44
    0,        // 0x45
    0,        // 0x46
    0,        // 0x47
    '7',      // 0x48 (Keypad)
    '8',      // 0x49
    '9',      // 0x4A
    '-',      // 0x4B (Keypad)
    '4',      // 0x4C
    '5',      // 0x4D
    '6',      // 0x4E
    '+',      // 0x4F (Keypad)
    '1',      // 0x50
    '2',      // 0x51
    '3',      // 0x52
    '0',      // 0x53
    '.',      // 0x54 (Keypad)
    0,        // 0x55
    0,        // 0x56
    0,        // 0x57
    0,        // 0x58
    0,        // 0x59
    0,        // 0x5A
    0,        // 0x5B
    0,        // 0x5C
    0,        // 0x5D
    0,        // 0x5E
    0,        // 0x5F
    0,        // 0x60
    0,        // 0x61
    0,        // 0x62
    0,        // 0x63
    0,        // 0x64
    0,        // 0x65
    0,        // 0x66
    0,        // 0x67
    0,        // 0x68
    0,        // 0x69
    0,        // 0x6A
    0,        // 0x6B
    0,        // 0x6C
    0,        // 0x6D
    0,        // 0x6E
    0,        // 0x6F
    0,        // 0x70
    0,        // 0x71
    0,        // 0x72
    0,        // 0x73
    0,        // 0x74
    0,        // 0x75
    0,        // 0x76
    0,        // 0x77
    0,        // 0x78
    0,        // 0x79
    0,        // 0x7A
    0,        // 0x7B
    0,        // 0x7C
    0,        // 0x7D
    0,        // 0x7E
    0,        // 0x7F
};


void to_hex_str(uint64_t value, char *buf) {
  const char hex_digits[] = "0123456789ABCDEF";
  buf[0] = '0';
  buf[1] = 'x';

  for (int i = 15; i >= 0; i--) {
    buf[2 + i] = hex_digits[value & 0xF];
    value >>= 4;
  }
  buf[18] = '\0';
}

extern "C" void handle_isr0() {
  if (term_instance) {
    term_instance->write_c_str("Division by zero error!\n");
  }
  KILL_PROCESS();
}

extern "C" void handle_isr1() {
if (term_instance) {
    term_instance->write_c_str("Debug fault!\n");
  }
  KILL_PROCESS();
}

extern "C" void handle_isr5() {
    if (term_instance) {
        term_instance->write_c_str("Bound Range Exceeded Exception!\n");
    }
    KILL_PROCESS();
}

extern "C" void handle_isr6() {
  if (term_instance) {
    term_instance->write_c_str("Invalid Opcode!\n");
  }
  KILL_PROCESS();
}

extern "C" void handle_isr7() {
  if (term_instance) {
    term_instance->write_c_str("Device not availible!\n");
  }
  KILL_PROCESS();
}

extern "C" void handle_isr8() {
  if (term_instance) {
    term_instance->write_c_str("Double fault!\n");
  }
  KILL_PROCESS();
}

typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // From pusha
    uint32_t error_code;                            // Pushed by CPU
    uint32_t eip, cs, eflags, user_esp, user_ss;    // Pushed by CPU during interrupt
} registers_t;

extern "C" void handle_isr14(registers_t* regs) {
  
  uint32_t faulting_addr;
  asm volatile("mov %%cr2, %0" : "=r"(faulting_addr));
  handle_page_fault(faulting_addr);  
}

static inline void io_wait(void)
{
    outb(0x80, 0);
}

void itoa_custom(int value, char* str, int base) {
    int i = 0;
    bool isNegative = false;

    // Handle 0 explicitly, otherwise empty string is printed
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers only if the base is 10
    if (value < 0 && base == 10) {
        isNegative = true;
        value = -value;
    }

    // Process individual digits
    while (value != 0) {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0'; // For base > 10, we use 'a' to 'f'
        value = value / base;
    }

    // Append negative sign for negative numbers
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Null-terminate string

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void wait_for_controller_ready() {
    while (inb(0x64) & 0x02) {
    }
}

// https://wiki.osdev.org/%228042%22_PS/2_Controller#Command_Register
// https://www.eecg.utoronto.ca/~pc/courses/241/DE1_SoC_cores/ps2/ps2.html
extern "C" void init_keyboard(){
    
    wait_for_controller_ready();
    outb(0x64, 0xAD);

    wait_for_controller_ready();
    outb(0x64, 0xAE);

    term_instance->write_c_str("Keyboard initialized.\n");

}

extern "C" void init_pit(uint32_t freq){
  uint32_t divisor = 1193182 / freq;
  outb(0x43, 0x36);
  outb(0x40, divisor & 0xFF);
  outb(0x40, divisor >> 8);
}

extern "C" void handle_isr32(){
  // save process
  // term_instance->write_c_str("Handling PIT.\n");
  // start new process
  outb(0x20, 0x20);
}

extern "C" void handle_isr33() {
  if (term_instance) {
    uint8_t scancode = inb(0x60);
    char buf[2] = { scancode_s2_to_ascii[scancode], '\0' };
    shell::process_keypress(scancode);
//    term_instance->write_c_str(buf);
  }
  outb(0x20, 0x20);
}

unsigned char get_RTC_register(int reg) {
      outb(CMOS_ADDRESS, reg);
      return inb(CMOS_READ);
}

unsigned char BCD_to_binary(unsigned char bcd) {
    return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

extern "C" void read_rtc(){

  unsigned char hour = BCD_to_binary(get_RTC_register(0x04));
  unsigned char last_minute = BCD_to_binary(get_RTC_register(0x02));
  unsigned char last_second = BCD_to_binary(get_RTC_register(0x00));

  unsigned char day_of_month = BCD_to_binary(get_RTC_register(0x07));
  unsigned char month = BCD_to_binary(get_RTC_register(0x08));
  unsigned char year = BCD_to_binary(get_RTC_register(0x09));

 if (term_instance) {

        char buffer[8];

        itoa_custom(hour, buffer, 10);
        term_instance->write_c_str(buffer);
        term_instance->write_c_str(":");

        itoa_custom(last_minute, buffer, 10);
        term_instance->write_c_str(buffer);
        term_instance->write_c_str(":");

        itoa_custom(last_second, buffer, 10);
        term_instance->write_c_str(buffer);
        term_instance->write_c_str(" ");


        itoa_custom(month, buffer, 10);
        term_instance->write_c_str(buffer);
        term_instance->write_c_str("/");

        itoa_custom(day_of_month, buffer, 10);
        term_instance->write_c_str(buffer);
        term_instance->write_c_str("/");

        itoa_custom(year, buffer, 10);
        term_instance->write_c_str(buffer);
        term_instance->write_c_str("\n");
    }
}

} // namespace interrupt_handlers

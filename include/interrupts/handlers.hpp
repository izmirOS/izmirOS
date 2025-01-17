#pragma once

#include "../kernel/dev/vga.hpp"

#define KILL_PROCESS()                                                         \
  {                                                                            \
    __asm__ volatile("cli; hlt");                                              \
    for (;;)                                                                   \
      ;                                                                        \
  }

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

extern "C" void handle_isr0() {
  if (term_instance) {
    term_instance->write_c_str("Division by zero error!\n");
  }
  KILL_PROCESS();
}

extern "C" void handle_isr33() {
  if (term_instance) {
    uint8_t scancode = inb(0x60);
    char buf[2] = {0};
    buf[0] = scancode;
    term_instance->write_c_str(buf);
  }
}
} // namespace interrupt_handlers

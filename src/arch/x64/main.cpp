#include "kernel/dev/vga.hpp"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern "C" void kernel_main() {
  vga::terminal term{};
  /* Newline support is left as an exercise. */
  term.write("Hello, kernel World!\n", 23);
}

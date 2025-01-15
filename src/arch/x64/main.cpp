#include "kernel/dev/vga.hpp"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern "C" void kernel_main() {
  vga::terminal term{};
  constexpr auto limit = 100;
  for (auto it = 0; it < limit; it++) {
    term.write_c_str("Hello, kernel World!\n");
  }
  term.scroll_up(5);

  /* constexpr auto limit2 = 2; */
  /* for (auto it = 0; it < limit2; it++) { */
  /*   term.write_c_str("Hello, kernel World!\n"); */
  /* } */

  /* term.scroll_down(5); */
}

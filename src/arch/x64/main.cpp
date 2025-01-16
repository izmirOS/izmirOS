#include "kernel/dev/vga.hpp"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct __attribute__((packed)) boot_info {
  uint64_t cr3;       // offset 0  - PML4
  uint64_t pdpt_phys; // offset 8  - PDPT
  uint64_t pt_phys;   // offset 16 - PT
  uint64_t flags;     // offset 24 - FLAGS
  uint32_t boot_mode; // offset 32 for boot mode
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

void to_hex_str(uint32_t value, char *buf) {
  const char hex_digits[] = "0123456789ABCDEF";
  buf[0] = '0';
  buf[1] = 'x';
  for (int i = 7; i >= 0; i--) {
    buf[2 + i] = hex_digits[value & 0xF];
    value >>= 4;
  }
  buf[10] = '\0';
}

extern "C" void kernel_main(struct boot_info *info) {
  vga::terminal term{};

  // Original code
  constexpr auto limit = 100;
  for (auto it = 0; it < limit; it++) {
    term.write_c_str("Hello, kernel World!\n");
  }
  term.scroll_up(5);

  // Print boot info
  char hex_buf[20]; // Buffer for hex string conversion

  term.write_c_str("\nBoot Info:\n");
  term.write_c_str("-----------------\n");

  term.write_c_str("CR3: ");
  to_hex_str(info->cr3, hex_buf);
  term.write_c_str(hex_buf);
  term.write_c_str("\n");

  term.write_c_str("PDPT Physical: ");
  to_hex_str(info->pdpt_phys, hex_buf);
  term.write_c_str(hex_buf);
  term.write_c_str("\n");

  term.write_c_str("PT Physical: ");
  to_hex_str(info->pt_phys, hex_buf);
  term.write_c_str(hex_buf);
  term.write_c_str("\n");

  term.write_c_str("Flags: ");
  to_hex_str(info->flags, hex_buf);
  term.write_c_str(hex_buf);
  term.write_c_str("\n");

  term.write_c_str("Boot Mode: ");
  uint32_t mode = info->boot_mode;
  to_hex_str(mode, hex_buf);
  term.write_c_str(hex_buf);
  term.write_c_str("\n");

  term.write_c_str("Boot mode offset: ");
  char offset_buf[20];
  to_hex_str(offsetof(struct boot_info, boot_mode), offset_buf);
  term.write_c_str(offset_buf);
  term.write_c_str("\n");
}

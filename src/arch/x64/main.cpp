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

typedef struct {
  uint16_t isr_low;
  uint16_t kernel_cs;
  uint8_t reserved;
  uint8_t attributes;
  uint16_t isr_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(0x10))) static idt_entry_t idt[256];

void idt_set_gate(uint8_t vector, uint32_t isr_addr, uint8_t flags) {
  idt_entry_t entry;
  entry.kernel_cs = 0x8000;
  entry.reserved = 0;
  entry.isr_low = isr_addr & 0xFFFF;
  entry.isr_low = (isr_addr >> 16) & 0xFFFF;
  entry.attributes = flags;
  idt[vector] = entry;
}

void idt_init() {

  static idtr_t idtr;
  idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
  idtr.base = (uint32_t)&idt[0];

  __asm__ volatile("lidt %0" : : "m"(idtr));
  __asm__ volatile("sti");
}

/* mov ax, 0x??  ;The descriptor of the TSS in the GDT (e.g. 0x28 if the sixths
 * entry in your GDT describes your TSS) */
/* ltr ax        ;The actual load */

extern "C" void kernel_main(struct boot_info *info) {

  // idt_init();

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

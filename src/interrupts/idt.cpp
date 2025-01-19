#include "interrupts/idt.hpp"

#define MASTER_PIC 0x0020
#define SLAVE_PIC 0x00A0

#define MASTER_COMMAND 0x0020
#define SLAVE_COMMAND 0x00A0

#define MASTER_DATA 0x0021
#define SLAVE_DATA 0x00A1

__attribute__((aligned(0x10))) static idt_entry_t idt[256];
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr33();

extern "C" void idt_set_gate(uint8_t vector, uint32_t isr_addr, uint8_t flags) {
  idt_entry_t entry;
  entry.kernel_cs = 0x08;
  entry.reserved = 0;
  entry.isr_low = isr_addr & 0xFFFF;
  entry.isr_high = (isr_addr >> 16) & 0xFFFF;
  entry.attributes = flags;
  idt[vector] = entry;
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
  __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline void io_wait(void)
{
    outb(0x80, 0);
}

// http://www.brokenthorn.com/Resources/OSDevPic.html

extern "C" void configure_pic(){

  // 0x11 is init control word that will init both PICs
  outb(MASTER_PIC, 0x11);
  outb(SLAVE_PIC, 0x11);

  // reset the offsets of PICs
  outb(0x21, 0x20);
  outb(0xA1, 0x28);

  outb(0x21, 0x04);
  outb(0xA1, 0x02);

  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  outb(0x21, 0x01);
  outb(0xA1, 0x00);


}

extern "C" void idt_init() {
  static idtr_t idtr;
  idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
  idtr.base = (uint32_t)&idt[0];
  idt_set_gate(0, (uint32_t)isr0, 0x8E);
  idt_set_gate(1, (uint32_t)isr1, 0x8E);
  idt_set_gate(5, (uint32_t)isr5, 0x8E);
  idt_set_gate(6, (uint32_t)isr6, 0x8E);
  idt_set_gate(7, (uint32_t)isr7, 0x8E);
  idt_set_gate(8, (uint32_t)isr8, 0x8E);
  idt_set_gate(33, (uint32_t)isr33, 0x8E);
  __asm__ volatile("lidt %0" : : "m"(idtr));
  __asm__ volatile("sti");
}

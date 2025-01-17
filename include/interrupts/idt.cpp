#include "idt.hpp"

__attribute__((aligned(0x10))) static idt_entry_t idt[256];
extern "C" void isr0();

extern "C" void idt_set_gate(uint8_t vector, uint32_t isr_addr, uint8_t flags) {
    idt_entry_t entry;
    entry.kernel_cs = 0x08;
    entry.reserved = 0;
    entry.isr_low = isr_addr & 0xFFFF;
    entry.isr_high = (isr_addr >> 16) & 0xFFFF;
    entry.attributes = flags;
    idt[vector] = entry;
}

extern "C" void idt_init() {
    static idtr_t idtr;
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
    idtr.base = (uint32_t)&idt[0];
    idt_set_gate(0, (uint32_t)isr0, 0x8E);
    __asm__ volatile ("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti");
}
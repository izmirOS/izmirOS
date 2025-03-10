#include "interrupts/handlers.hpp" // not causing boot loop
#include "interrupts/idt.hpp"      // not causing boot loop
#include "kernel/dev/vga.hpp"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct __attribute__((packed)) boot_info
{
  uint64_t cr3;       // offset 0  - PML4
  uint64_t pdpt_phys; // offset 8  - PDPT
  uint64_t pt_phys;   // offset 16 - PT
  uint64_t flags;     // offset 24 - FLAGS
  uint32_t boot_mode; // offset 32 for boot mode
};

void to_hex_str(uint64_t value, char *buf)
{
  const char hex_digits[] = "0123456789ABCDEF";
  buf[0] = '0';
  buf[1] = 'x';

  for (int i = 15; i >= 0; i--)
  {
    buf[2 + i] = hex_digits[value & 0xF];
    value >>= 4;
  }
  buf[18] = '\0';
}

void print_memory_map(vga::terminal &term) {
    uint16_t entry_count = *((uint16_t*)0x7FF8);
    
    // Define memory map entry structure
    struct MemoryMapEntry {
        uint64_t base_address;
        uint64_t length;
        uint32_t type;
        uint32_t acpi_extended;
    };
    
    MemoryMapEntry* mmap = (MemoryMapEntry*)0x8000;
    
    // Print header
    char addr_buf[20];
    char len_buf[20];
    char num_buf[12]; // For numbers
    
    // Determine if we have a valid memory map
    if (entry_count == 0 || entry_count > 100) {  // Sanity check
        term.write_c_str("No valid memory map found\n");
        return;
    }
    
    term.write_c_str("\nMemory Map Information:\n");
    term.write_c_str("------------------------------------\n");
    
    term.write_c_str("Found ");
    // Convert entry_count to string
    uint16_t temp = entry_count;
    int pos = 0;
    char digits[6]; // Temp buffer for digits
    
    // Extract digits in reverse
    do {
        digits[pos++] = '0' + (temp % 10);
        temp /= 10;
    } while (temp > 0);
    
    // Print digits in correct order
    while (pos > 0) {
        char c[2] = { digits[--pos], '\0' };
        term.write_c_str(c);
    }
    
    term.write_c_str(" memory map entries\n");
    
    term.write_c_str("Type | Base Address       | Length\n");
    term.write_c_str("------------------------------------\n");
    
    uint64_t total_available = 0;
    
    // Print each entry
    for (uint16_t i = 0; i < entry_count; i++) {
        // Get type string
        const char* type_str;
        switch (mmap[i].type) {
            case 1:
                type_str = "Usable ";
                total_available += mmap[i].length;
                break;
            case 2:
                type_str = "Reservd";
                break;
            case 3:
                type_str = "ACPI   ";
                break;
            case 4:
                type_str = "NVS    ";
                break;
            case 5:
                type_str = "Bad    ";
                break;
            default:
                type_str = "??     ";
                break;
        }
        
        // Convert addresses to hex strings
        to_hex_str(mmap[i].base_address, addr_buf);
        to_hex_str(mmap[i].length, len_buf);
        
        // Print the entry
        term.write_c_str(type_str);
        term.write_c_str(" | ");
        term.write_c_str(addr_buf);
        term.write_c_str(" | ");
        term.write_c_str(len_buf);
        term.write_c_str("\n");
    }
    
    // Print total available memory
    term.write_c_str("\nTotal Available Memory: ");
    
    // Convert to MB for readability
    uint64_t total_mb = total_available / (1024 * 1024);
    
    // Convert total_mb to string
    uint64_t temp_mb = total_mb;
    pos = 0;
    
    do {
        digits[pos++] = '0' + (temp_mb % 10);
        temp_mb /= 10;
    } while (temp_mb > 0);
    
    while (pos > 0) {
        char c[2] = { digits[--pos], '\0' };
        term.write_c_str(c);
    }
    
    term.write_c_str(" MB\n");
}

extern "C" void enable_32bit_paging();

extern "C" void kernel_main(struct boot_info *info)
{
  // https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.pdf
  /* mov ax, 0x??  ;The descriptor of the TSS in the GDT (e.g. 0x28 if the sixths
   * entry in your GDT describes your TSS) */
  /* ltr ax        ;The actual load */

  vga::terminal term{};
  // print_memory_map(term);
  interrupt_handlers::init_handlers(&term);
  idt_init();
  configure_pic();
  enable_32bit_paging();
  uint32_t *test_addr = (uint32_t*)0x500000;
  *test_addr = 0xDEADBEEF;
  if (*test_addr == 0xDEADBEEF) {
    term.write_c_str("Paging test passed\n");
  }



  // Print boot info
  char hex_buf[20]; // Buffer for hex string conversion

  term.write_c_str("\nBoot Info:\n");
  term.write_c_str("-----------------\n");

  term.write_c_str("CR3: ");
  to_hex_str(info->cr3, hex_buf);
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

  interrupt_handlers::init_keyboard();
  interrupt_handlers::read_rtc();


  while (1){
    __asm__ volatile("hlt");
  }
}

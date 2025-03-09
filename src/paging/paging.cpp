#include <stdint.h>
#include <paging/paging.hpp>

__attribute__((aligned(4096))) uint32_t page_directory[1024];
__attribute__((aligned(4096))) uint32_t page_table_0[1024];
__attribute__((aligned(4096))) uint32_t page_table_1[1024]; // Add more page tables

extern "C" void init_paging_structures(void) {
    // Clear page directory
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
    }
    
    // Set up first 4MB (0x00000000-0x003FFFFF)
    for (int i = 0; i < 1024; i++) {
        page_table_0[i] = (i * 4096) | 3;
    }
    
    // Set up second 4MB (0x00400000-0x007FFFFF)
    for (int i = 0; i < 1024; i++) {
        page_table_1[i] = (0x400000 + i * 4096) | 3;
    }
    
    // Link page tables to directory
    page_directory[0] = ((uint32_t)page_table_0) | 3;
    page_directory[1] = ((uint32_t)page_table_1) | 3;
}

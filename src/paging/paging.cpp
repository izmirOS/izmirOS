#include <stdint.h>
#include <string.h>
#include <paging/paging.hpp>

__attribute__((aligned(4096))) uint32_t page_directory[1024];
__attribute__((aligned(4096))) uint32_t page_table_0[1024];
__attribute__((aligned(4096))) uint32_t page_table_1[1024]; // Add more page tables

#define NUM_PAGE_FRAMES 65536
#define FRAME_SIZE 4096

uint8_t page_frame_bitmap[NUM_PAGE_FRAMES / 8];

uint32_t find_first_free_frame_bit(){

    for (int i = 0; i < NUM_PAGE_FRAMES / 8; i++){

         uint8_t byte = page_frame_bitmap[i];
        if (byte == 0xFF) {
            continue;
        }

        for (int k = 0; k < 8; k++){
            if (!(byte & (1 << k))){
                return i * 8 + k;
            }
        }
    }
    return (uint32_t)-1;
}

void bitmap_set_bit(uint8_t* bitmap, uint32_t frame_number) {
    uint32_t byte_index = frame_number / 8;
    uint32_t bit_position = frame_number % 8;
    bitmap[byte_index] |= (1 << bit_position);
}

uint32_t allocate_page_table(){

    uint32_t frame_index = find_first_free_frame_bit();
    bitmap_set_bit(page_frame_bitmap, frame_index);
    uint32_t phys_addr = frame_index * FRAME_SIZE;
    memset((void*)phys_addr, 0, FRAME_SIZE);
    return phys_addr;
}

void handle_page_table_fault(uint32_t virt_addr){
    uint32_t page_dir_index = virt_addr >> 22;
    uint32_t phys_addr = allocate_page_table();
    page_directory[page_dir_index] = phys_addr | 3;
}

void handle_page_fault(uint32_t virt_addr){

}

extern "C" void init_paging_structures(void) {
    // Clear page directory
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
    }

    bitmap_set_bit(page_frame_bitmap, (uint32_t)page_directory / FRAME_SIZE);
    
    // Set up first 4MB (0x00000000-0x003FFFFF)
    for (int i = 0; i < 1024; i++) {
        page_table_0[i] = (i * 4096) | 3;
    }

    bitmap_set_bit(page_frame_bitmap, (uint32_t)page_table_0 / FRAME_SIZE);
    
    // Set up second 4MB (0x00400000-0x007FFFFF)
    for (int i = 0; i < 1024; i++) {
        page_table_1[i] = (0x400000 + i * 4096) | 3;
    }

    bitmap_set_bit(page_frame_bitmap, (uint32_t)page_table_1 / FRAME_SIZE);
    
    // Link page tables to directory
    page_directory[0] = ((uint32_t)page_table_0) | 3;
    page_directory[1] = ((uint32_t)page_table_1) | 3;

}

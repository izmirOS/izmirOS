#include <stdint.h>
#include <paging/paging.hpp>

__attribute__((aligned(4096))) uint32_t page_directory[1024];
__attribute__((aligned(4096))) uint32_t page_table_0[1024];
__attribute__((aligned(4096))) uint32_t page_table_1[1024]; // Add more page tables
__attribute__((aligned(NUM_PAGE_FRAMES / 2))) uint8_t page_frame_bitmap[NUM_PAGE_FRAMES / 8];

uint32_t* create_page_directory(){
    uint32_t pd_frame = find_free_frame();
    set_bitmap_frame(page_frame_bitmap, pd_frame);
    uint32_t* new_page_dir = (uint32_t*)(pd_frame * FRAME_SIZE);

    new_page_dir[1023] = (uint32_t)new_page_dir | 0x3;
    return new_page_dir;
}

uint32_t find_first_free_frame(){

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

void memset(void *b, int value, int size){
	unsigned char *ptr = (unsigned char*)b;
	int len = size;
	while (len > 0){
		*ptr = (unsigned char)value;
		ptr++;
		len--;
	}
}

void set_bitmap_frame(uint8_t* bitmap, uint32_t frame_number) {
    uint32_t byte_index = frame_number / 8;
    uint32_t bit_position = frame_number % 8;
    bitmap[byte_index] |= (1 << bit_position);
}

uint32_t allocate_page(){
    uint32_t frame_index = find_first_free_frame();
    set_bitmap_frame(page_frame_bitmap, frame_index);
    uint32_t phys_addr = frame_index * FRAME_SIZE;
    // memset((void*)phys_addr, 0, FRAME_SIZE);
    return phys_addr;
}

static inline void outb(uint16_t port, uint8_t val)
{
  __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}




static inline void outs(char* error)
{
    while (*error){
        outb(0xe9, *error);
        error++;
    }
}

#define DB(x) outb(0xe9, x)
#define DHN(x) outb(0xe9, (((x & 0xF) >= 10) ? (((x & 0xF) - 10) + 'a') : ((x & 0xF) + '0')));
#define DHB(x) DHN(x >> 4) ; DHN(x);
#define DHW(x) DHB(x >> 8) ; DHB(x);
#define DHL(x) DHW(x >> 16) ; DHW(x);
#define DHQ(x) DHL(x >> 32) ; DHL(x);
#define DS(x) outs(x);



void handle_page_fault(uint32_t virt_addr){
    uint32_t page_dir_index = virt_addr >> 22;
    uint32_t page_table_index = (virt_addr >> 12) & 0x3FF;
    
    if (!(page_directory[page_dir_index] & 1)){
        uint32_t page_table_phys = allocate_page();
        page_directory[page_dir_index] = page_table_phys | 3;
    }
    
    // Access page tables through recursive mapping
    uint32_t* page_tables = (uint32_t*)(0xFFC00000);  // Base address with recursive mapping
    uint32_t* page_table = &page_tables[page_dir_index * 1024];
    
    uint32_t phys_addr = allocate_page();
    page_table[page_table_index] = phys_addr | 3;
    
    asm volatile("invlpg (%0)" : : "r" (virt_addr));
}


#define HEAP_START 0x400000

extern "C" void init_paging_structures(void) {

    memset(page_frame_bitmap, 0, NUM_PAGE_FRAMES / 8);

    

    // memset(page_frame_bitmap, 0xFF, HEAP_START / 8);

    // Clear page directory
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
    }

    set_bitmap_frame(page_frame_bitmap, (uint32_t)page_directory / FRAME_SIZE);
    
    // Set up first 4MB (0x00000000-0x003FFFFF)
    for (int i = 0; i < 1024; i++) {
        page_table_0[i] = (i * 4096) | 3;
        set_bitmap_frame(page_frame_bitmap, i);
    }

    set_bitmap_frame(page_frame_bitmap, (uint32_t)page_table_0 / FRAME_SIZE);
    
    // Set up second 4MB (0x00400000-0x007FFFFF)
    for (int i = 0; i < 1024; i++) {
        page_table_1[i] = (0x400000 + i * 4096) | 3;
        set_bitmap_frame(page_frame_bitmap, 0x400000/FRAME_SIZE + i);
    }

    set_bitmap_frame(page_frame_bitmap, (uint32_t)page_table_1 / FRAME_SIZE);
    
    // Link page tables to directory
    page_directory[0] = ((uint32_t)page_table_0) | 3;
    page_directory[1] = ((uint32_t)page_table_1) | 3;
    page_directory[1023] = ((uint32_t)page_directory) | 3;

}

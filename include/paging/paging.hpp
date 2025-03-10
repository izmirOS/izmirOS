#include <stdint.h>

extern uint32_t page_directory[1024];
extern uint32_t page_table_0[1024];
extern uint32_t page_table_1[1024];

#define NUM_PAGE_FRAMES 65536
#define FRAME_SIZE 4096
#define PAGE_SIZE 4096

extern uint8_t page_frame_bitmap[NUM_PAGE_FRAMES / 8];

uint32_t find_free_frame();
void set_bitmap_frame(uint8_t* bitmap, uint32_t frame_number);

uint32_t allocate_page_table();
uint32_t allocate_page();

void handle_page_table_fault(uint32_t virt_addr);
void handle_page_fault(uint32_t virt_addr);

void memset(void *b, int value, int size);

extern "C" void init_paging_structures(void);


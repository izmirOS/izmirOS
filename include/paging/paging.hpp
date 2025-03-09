#include <stdint.h>

extern uint32_t page_directory[1024];
extern uint32_t page_table_0[1024];

void init_paging_structurses(void);


#include <stdint.h>
#include <paging/paging.hpp>


uint8_t __attribute__((packed)) tid_bitmap[1024];
enum TaskState {
    TASK_RUNNING = 0,
    TASK_READY = 1,
    TASK_BLOCKED = 2,
    TASK_STOPPED = 3
};

struct context {
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi, ebp, esp;
    uint32_t eip;
    uint32_t eflags;
    uint32_t cs, ds, es, fs, gs, ss;
};

struct task_struct {
    uint32_t tid;
    uint32_t time_started; 
    TaskState state;
    void* page_directory;
    struct context* context;
};

struct task_struct* task_table[8192];

int update_cr3(struct task_struct* new_task) {
    uint32_t pd_addr = (uint32_t)new_task->page_directory;
    __asm__ __volatile__("mov %0, %%cr3"
        :                              
        : "r"(pd_addr)                
        : "memory");                   
    return 0;
}

int save_context();

void free_tid(int tid) {
    if (tid < 0 || tid >= 1024 * 8) {
        return;     }
    
    int byte_index = tid / 8;
    int bit_index = tid % 8;
    
    tid_bitmap[byte_index] &= ~(1 << bit_index);
}

int find_next_free_tid() {
    for (int byte_index = 0; byte_index < 1024; byte_index++) {
        if (tid_bitmap[byte_index] == 0xFF) {
            continue;
        }
        
        for (int bit_index = 0; bit_index < 8; bit_index++) {
            if ((tid_bitmap[byte_index] & (1 << bit_index)) == 0) {
                int tid = byte_index * 8 + bit_index;
                tid_bitmap[byte_index] |= (1 << bit_index);
                return tid;
            }
        }
    }
    return -1;
}

int create_task(){
    struct task_struct new_task = {0};

    new_task.tid = find_next_free_tid();
    new_task.state = TASK_READY;
    new_task.page_directory = create_page_directory();

    task_table[new_task.tid] = &new_task;
    return 1;
}

void schedule();
int kill_task();
int switch_task(); // returns tid of next task

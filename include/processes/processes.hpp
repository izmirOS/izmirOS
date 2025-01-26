#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define ERROR -1

namespace processes
{

    enum PROCESS_STATE
    {
        RUNNING = 0,
        READY = 1,
        BLOCKED = 2,
    };

    struct PCB
    {
        uint32_t pid;          // Process ID
        PROCESS_STATE state;   // Process state
        uint32_t registers[8]; // General-purpose registers (EAX, EBX, etc.)
        uint32_t eip;          // Instruction pointer
        uint32_t esp;          // Stack pointer
        uint32_t ebp;          // Base pointer
    };

    constexpr int max_processes = 16;
    PCB process_table[MAX_PROCESSES];

    uint8_t pid_bitmap[max_processes / 2] = {0};

    uint32_t clear_bitmap(uint32_t pid)
    {
        if (pid > max_processes)
        {
            return ERROR;
        }
        pid_bitmap[pid / 32] &= ~(1 << (pid % 32));

        return SUCCESS;
    }

    uint32_t add_to_bitmap()
    {
        for (size_t i = 0; i < max_processes / 2; i++)
        {
            if (!(pid_bitmap[i / 32] & (1 << (i % 32))))
            {
                pid_bitmap[i / 32] |= (1 << (i % 32));
                return i;
            }
        }
        return -1;
    }

    int create_process()
    {
        uint32_t pid = add_to_bitmap();

        if (pid == -1)
        {
            return ERROR;
        }

        PCB *process = (PCB *)malloc(sizeof(PCB));

        process->state = READY;
        process->pid = pid;
    }

    int kill_process(uint32_t pid){
        clear_bitmap(pid);
    }

    void context_switch(PCB *current)
    {
        process_table[current->pid] = *current;
        int next_pid = (current->pid + 1) % MAX_PROCESSES;
        PCB *next = &process_table[next_pid];
        *current = *next;
    }

}

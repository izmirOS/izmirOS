#include "../kernel/dev/vga.hpp" 

namespace interrupt_handlers {

    vga::terminal* term_instance = nullptr;

    void init_handlers(vga::terminal* term){
        term_instance = term;
    }
    
    extern "C" void handle_isr0() {
        if (term_instance){
            term_instance->write_c_str("Division by zero error!\n");
        }
    }
}
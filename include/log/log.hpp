#pragma once

#include "../include/kernel/dev/vga.hpp"



namespace log {

    enum LogType {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        CRITICAL
    };
    static vga::terminal* term_instance = nullptr;
    static LogType current_level = INFO;
    static const char* level_strings[] = {
        "INFO", "DEBUG", "WARNING", "ERROR", "CRITICAL"
    };

    
    void init(vga::terminal* term) {
        term_instance = term;
    }
    
    void set_level(LogType min_level) {
        current_level = min_level;
    }
    
    void write(LogType level, const char* message) {
        // Skip if below current log level
        if (level < current_level) return;
        
        // Store current color
        
        // Set color for this log level
        
        term_instance->write_c_str("[");
        term_instance->write_c_str(level_strings[level]);
        term_instance->write_c_str("]");
        term_instance->write_c_str(message);
        term_instance->write_c_str("\n");
        
        // Restore original color
    }
}



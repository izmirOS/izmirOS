#pragma once

#include <stdint.h>
#include "kernel/dev/vga.hpp"

namespace shell {

static const unsigned char scancode_s2_to_ascii[0xFF] = {
    0,        // 0x00
    0,        // 0x01
    '1',      // 0x02
    '2',      // 0x03
    '3',      // 0x04
    '4',      // 0x05
    '5',      // 0x06
    '6',      // 0x07
    '7',      // 0x08
    '8',      // 0x09
    '9',      // 0x0A
    '0',      // 0x0B
    '-',      // 0x0C
    '=',      // 0x0D
    '\b',     // 0x0E
    '\t',     // 0x0F
    'q',      // 0x10
    'w',      // 0x11
    'e',      // 0x12
    'r',      // 0x13
    't',      // 0x14
    'y',      // 0x15
    'u',      // 0x16
    'i',      // 0x17
    'o',      // 0x18
    'p',      // 0x19
    '[',      // 0x1A
    ']',      // 0x1B
    '\n',     // 0x1C
    0,        // 0x1D (Left Control)
    'a',      // 0x1E
    's',      // 0x1F
    'd',      // 0x20
    'f',      // 0x21
    'g',      // 0x22
    'h',      // 0x23
    'j',      // 0x24
    'k',      // 0x25
    'l',      // 0x26
    ';',      // 0x27
    '\'',     // 0x28
    '`',      // 0x29
    0,        // 0x2A (Left Shift)
    '\\',     // 0x2B
    'z',      // 0x2C
    'x',      // 0x2D
    'c',      // 0x2E
    'v',      // 0x2F
    'b',      // 0x30
    'n',      // 0x31
    'm',      // 0x32
    ',',      // 0x33
    '.',      // 0x34
    '/',      // 0x35
    0,        // 0x36 (Right Shift)
    '*',      // 0x37
    0,        // 0x38 (Left Alt)
    ' ',      // 0x39
    0,        // 0x3A (Caps Lock)
    0,        // 0x3B
    0,        // 0x3C
    0,        // 0x3D
    0,        // 0x3E
    0,        // 0x3F
    0,        // 0x40
    0,        // 0x41
    0,        // 0x42
    0,        // 0x43
    0,        // 0x44
    0,        // 0x45
    0,        // 0x46
    0,        // 0x47
    '7',      // 0x48 (Keypad)
    '8',      // 0x49
    '9',      // 0x4A
    '-',      // 0x4B (Keypad)
    '4',      // 0x4C
    '5',      // 0x4D
    '6',      // 0x4E
    '+',      // 0x4F (Keypad)
    '1',      // 0x50
    '2',      // 0x51
    '3',      // 0x52
    '0',      // 0x53
    '.',      // 0x54 (Keypad)
    0,        // 0x55
    0,        // 0x56
    0,        // 0x57
    0,        // 0x58
    0,        // 0x59
    0,        // 0x5A
    0,        // 0x5B
    0,        // 0x5C
    0,        // 0x5D
    0,        // 0x5E
    0,        // 0x5F
    0,        // 0x60
    0,        // 0x61
    0,        // 0x62
    0,        // 0x63
    0,        // 0x64
    0,        // 0x65
    0,        // 0x66
    0,        // 0x67
    0,        // 0x68
    0,        // 0x69
    0,        // 0x6A
    0,        // 0x6B
    0,        // 0x6C
    0,        // 0x6D
    0,        // 0x6E
    0,        // 0x6F
    0,        // 0x70
    0,        // 0x71
    0,        // 0x72
    0,        // 0x73
    0,        // 0x74
    0,        // 0x75
    0,        // 0x76
    0,        // 0x77
    0,        // 0x78
    0,        // 0x79
    0,        // 0x7A
    0,        // 0x7B
    0,        // 0x7C
    0,        // 0x7D
    0,        // 0x7E
    0,        // 0x7F
};


	vga::terminal* term_instance = nullptr;
	char input_buffer[256];
	uint32_t buffer_pos;
	
	void init_shell(vga::terminal *term) { 
		term_instance = term; 
		buffer_pos = 0;
		term_instance->write_c_str("> "); 
	}
void execute_command();

	void process_keypress(uint8_t scancode){

		if (scancode & 0x80) {
       			return; // Skip key releases
    		}

		char c = scancode_s2_to_ascii[scancode];

		if (c == 0) {
        		return;
    		}

		if (scancode == 0x1C) {
			term_instance->write_c_str("\n");
        		input_buffer[buffer_pos] = '\0'; // Null-terminate
        		execute_command(); // Process the command
        		buffer_pos = 0; // Reset buffer
        		term_instance->write_c_str("> "); // New prompt
        		return;
    		}

		char buf[2] = {c, '\0'};
        	input_buffer[buffer_pos++] = c;
        	term_instance->write_c_str(buf);

	};

	char* parse_input(){
		static char command[256];
		
		int i = 0;
    		while (i < buffer_pos && input_buffer[i] != ' ' && input_buffer[i] != '\n') {
        		command[i] = input_buffer[i];
        		i++;
    		}
		command[i] = '\0';
		return command;
	}

int my_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Check if two strings are equal (convenience function)
bool str_equals(const char* s1, const char* s2) {
    return my_strcmp(s1, s2) == 0;
}

	void execute_command(){
		char* command = parse_input();
	if (str_equals(command, "help")) {
        term_instance->write_c_str("\nHelp? I need somebody... Help! Not just anybody...\n");
        term_instance->write_c_str("(Beatles reference aside, real help coming in future update)\n");
    }	}

	
}

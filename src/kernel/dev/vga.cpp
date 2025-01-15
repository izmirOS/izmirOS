#include "kernel/dev/vga.hpp"

namespace vga {
terminal::terminal() {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color =
      entryColor(VGA_COLOR::VGA_COLOR_LIGHT_GREY, VGA_COLOR::VGA_COLOR_BLACK);
  terminal_buffer = (uint16_t *)0xB8000;

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = entry(' ', terminal_color);
    }
  }
}

void terminal::set_color(uint8_t color) { terminal_color = color; }

void terminal::put_entry_at(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = entry(c, color);
}
void terminal::put_char(char c) {
  put_entry_at(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
      terminal_row = 0;
  }
}
void terminal::write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    put_char(data[i]);
}
} // namespace vga

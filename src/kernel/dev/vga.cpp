#include "kernel/dev/vga.hpp"

namespace vga {
 static const char* level_strings[] = {
        "INFO", "DEBUG", "WARNING", "ERROR", "CRITICAL"
    };

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

void terminal::move_row(const size_t from, const size_t to) {
  for (size_t x = 0; x < VGA_WIDTH; x++) {
    terminal_buffer[to * VGA_WIDTH + x] = terminal_buffer[from * VGA_WIDTH + x];
    terminal_buffer[from * VGA_WIDTH + x] = entry(' ', terminal_color);
  }
}

void terminal::scroll_up(const size_t by) {
  for (size_t row = 0; row < VGA_HEIGHT - by; ++row) {
    move_row(row + by, row);
  }

  for (size_t row = VGA_HEIGHT - by; row < VGA_HEIGHT; ++row) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      put_entry_at(' ', terminal_color, x, row);
    }
  }

  if (terminal_row >= by) {
    terminal_row -= by;
  } else {
    terminal_row = 0;
  }
}

void terminal::scroll_down(const size_t by) {
  for (size_t row = VGA_HEIGHT - 1; row >= by; --row) {
    move_row(row - by, row);
  }

  for (size_t row = 0; row < by; ++row) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      put_entry_at(' ', terminal_color, x, row);
    }
  }

  if (terminal_row + by < VGA_HEIGHT) {
    terminal_row += by;
  } else {
    terminal_row = VGA_HEIGHT - 1;
  }
}

void terminal::put_char(char c) {
  if (c == '\n') {
    terminal_column = 0;
    if (terminal_row + 1 == VGA_HEIGHT)
      scroll_up(1);
    else
      terminal_row++;
    return;
  }

  put_entry_at(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;

    if (terminal_row + 1 == VGA_HEIGHT)
      scroll_up(1);
    else
      terminal_row++;
  }
}
void terminal::write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    put_char(data[i]);
}

void terminal::log(LogType level, const char *str){
  write_c_str("[");
  write_c_str(level_strings[level]);
  write_c_str("]: ");
  write_c_str(str);
  write_c_str("\n");
}

void terminal::write_c_str(const char *str) {
  size_t it = 0;
  while (str[it]) {
    put_char(str[it]);
    it++;
  }
}
} // namespace vga

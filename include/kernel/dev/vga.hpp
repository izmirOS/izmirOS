#pragma once

#include <stddef.h>
#include <stdint.h>

namespace vga {
/* Hardware text mode color constants. */
// scoped enums
enum class VGA_COLOR : uint8_t {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

enum LogType {
  INFO,
  DEBUG,
  WARNING,
  ERROR
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

inline uint8_t entryColor(VGA_COLOR fg, VGA_COLOR bg) {
  return static_cast<uint8_t>(fg) | (static_cast<uint8_t>(bg) << 4);
}
inline uint16_t entry(unsigned char uc, uint8_t color) {
  return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

class terminal {
private:
  size_t terminal_row;
  size_t terminal_column;
  uint8_t terminal_color;
  uint16_t *terminal_buffer;

public:
  terminal();
  void set_color(uint8_t color);
  void put_entry_at(char c, uint8_t color, size_t x, size_t y);
  void put_char(char c);
  void write(const char *data, size_t size);
  void log(LogType level, const char *str);
  void write_c_str(const char *str);
  void move_row(const size_t from, const size_t to);
  void scroll_up(const size_t by);
  void scroll_down(const size_t by);
};
} // namespace vga

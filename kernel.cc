#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

namespace VGA {
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

inline uint8_t entryColor(VGA_COLOR fg, VGA_COLOR bg) {
  return static_cast<uint8_t>(fg) | (static_cast<uint8_t>(bg) << 4);
}
inline uint16_t entry(unsigned char uc, uint8_t color) {
  return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
class Terminal {
private:
  size_t terminal_row;
  size_t terminal_column;
  uint8_t terminal_color;

  uint16_t *terminal_buffer;

public:
  Terminal() {
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
  void setColor(uint8_t color) { terminal_color = color; }

  void putEntryAt(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = entry(c, color);
  }

  void putChar(char c) {
    putEntryAt(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      if (++terminal_row == VGA_HEIGHT)
        terminal_row = 0;
    }
  }

  void write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++)
      putChar(data[i]);
  }

  void writeString(const char *str) { write(str, strlen(str)); }
};
}; // namespace VGA

extern "C" void kernel_main() {
  VGA::Terminal term{};
  /* Newline support is left as an exercise. */
  term.writeString("Hello, kernel World!\n");
}

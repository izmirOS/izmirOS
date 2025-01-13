#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// we need to port the whole stl

template <bool Cond, typename T = void> struct enable_if;
template <typename T> struct enable_if<true, T> {
  using type = T;
};

template <bool Cond, typename T = void>
using enable_if_t = enable_if<Cond, void>::type;

template <auto v> struct integral_constant {
  constexpr static auto value = v;
  using type = decltype(v);
};

using true_type = integral_constant<true>;
using false_type = integral_constant<false>;

template <bool flag> using bool_constant = integral_constant<flag>;

template <typename T, typename B> struct is_same : false_type {};
template <typename T> struct is_same<T, T> : true_type {};

template <typename T, typename B> using is_same_v = is_same<T, B>::value;

template <typename... Ts> struct void_t {
  using type = void;
};

template <typename From, typename To, typename = void>
struct is_convertible : false_type {};

template <typename From, typename To>
struct is_convertible<From, To,
                      void_t<decltype(static_cast<To>(declval<From>()))>>
    : true_type {};

template <typename From, typename To>
using is_convertible_v = is_convertible<From, To>::value;

// this needs remove const, volatile etc
template <typename T> struct is_pointer : false_type {};
template <typename T> struct is_pointer<T *> : true_type {};

template <typename T> using is_pointer_v = is_pointer<T>::value;

/* T t, T* p, void(*f)(T) */
template <typename T> struct is_integral {};

/* template <class T> */
/*     struct is_integral */
/*     : std::bool_constant < */
/*       requires(T t, T *p, void (*f)(T)) // T* parameter excludes reference
 * types */
/* { */
/*   reinterpret_cast<T>(t); // Exclude class types */
/*   f(0);                   // Exclude enumeration types */
/*   p + t; // Exclude everything not yet excluded but integral types */
/* }>{}; */

// libcpp array implementation
// aggregate value initialization works out of the box
template <typename T, size_t N> struct array {
  using value_type = T;

  using pointer = T *;
  using const_pointer = const T *;

  using reference = value_type &;
  using const_reference = const value_type &;

  using iterator = value_type *;
  using const_iterator = const value_type *;

  using size_type = size_t;
  // diff type, reverse iterator, const reverse iterator not included *yet*

  // this is what allows aggregate value init
  value_type m_vals[N ? N : 1]; // allow empty arrs

  constexpr size_type size() { return N; }
  constexpr bool empty() { return size() == 0; }

  reference operator[](size_type i) { return m_vals[i]; }
  const_reference operator[](size_type i) const { return m_vals[i]; }

  pointer data() { return m_vals; }
  // more to come
};

// this is weird will fix soon
template <size_t N> using string = array<char, N>;

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

  template <size_t N> void writeString(string<N> string) {
    write(string.data(), string.size());
  }
};
}; // namespace VGA

extern "C" void kernel_main() {
  VGA::Terminal term{};
  /* Newline support is left as an exercise. */
  term.writeString(string<23>{"Hello, kernel World!\n"});
}

// libcpp array implementation
// aggregate value initialization works out of the box
#pragma once
#include <stddef.h>

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

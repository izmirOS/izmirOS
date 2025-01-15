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

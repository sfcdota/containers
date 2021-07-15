
#ifndef SRCS_SFINAE_HPP_
#define SRCS_SFINAE_HPP_
//#include "is_integral.hpp"
namespace ft {
template<bool, class Type = void>
struct enable_if {};

template<class Type>
struct enable_if<true, Type> {
  typedef Type type;
};

template<class T>
struct is_pointer { static const bool value = false; };

template<class T>
struct is_pointer<T *> { static const bool value = true; };

template<class T>
struct is_pointer<T const *> { static const bool value = true; };

template<class T>
class DetectIterator {
 private:
  typedef char yes[1];
  typedef yes no[2];
  template<typename C>
  static yes &test(typename C::iterator_category * = 0) {};
  template<typename>
  static no &test(...) {};
 public:
  static const bool value = sizeof(test<T>(0)) == sizeof(yes) || is_pointer<T>::value;
};

template<typename T>
struct has_iterator {
  template<typename U>
  static char test(typename U::iterator * = 0) {};
  template<typename U>
  static long test(U *) {};
  static const bool value = sizeof(test<T>(0)) == 1;
};
}
#endif //SRCS_SFINAE_HPP_

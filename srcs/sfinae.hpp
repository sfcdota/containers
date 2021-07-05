
#ifndef SRCS_SFINAE_HPP_
#define SRCS_SFINAE_HPP_
#include "iterator.hpp"

template<bool, class Type = void>
struct enable_if {};

template <class Type>
struct enable_if<true, Type>
{
  typedef Type type;
};

template <class T>
struct is_pointer { static const bool value = false; };

template <class T>
struct is_pointer<T*> { static const bool value = true; };

template <class T>
class DetectIterator {
 private:
  typedef char yes[1];
  typedef yes no[2];
  template<typename C> static yes& test(typename C::value_type* = 0) {};
  template<typename> static no& test(...) {};
 public:
  static const bool value = sizeof(test<T>(0)) == sizeof(yes) || is_pointer<T>::value;
};
#endif //SRCS_SFINAE_HPP_


#ifndef SRCS_IS_INTEGRAL_HPP_
#define SRCS_IS_INTEGRAL_HPP_
#include "iterator_traits.hpp"
namespace ft {
  template<class T, T v>
  struct integral_constant {
    static const T value = v;
    typedef T value_type;
    typedef integral_constant type;
  };

  typedef integral_constant<bool, true> true_type;
  typedef integral_constant<bool, false> false_type;

  template <class T> struct is_integral_template                   : public false_type {};
  template <> struct is_integral_template<bool>                    : public true_type {};
  template <> struct is_integral_template<char>                    : public true_type {};
  template <> struct is_integral_template<char16_t>                : public true_type {};
  template <> struct is_integral_template<char32_t>                : public true_type {};
  template <> struct is_integral_template<wchar_t>                 : public true_type {};
  template <> struct is_integral_template<signed char>             : public true_type {};
  template <> struct is_integral_template<short int>               : public true_type {};
  template <> struct is_integral_template<int>                     : public true_type {};
  template <> struct is_integral_template<long  int>               : public true_type {};
  template <> struct is_integral_template<long long  int>          : public true_type {};
  template <> struct is_integral_template<unsigned char>           : public true_type {};
  template <> struct is_integral_template<unsigned short int>      : public true_type {};
  template <> struct is_integral_template<unsigned int>            : public true_type {};
  template <> struct is_integral_template<unsigned long int>       : public true_type {};
  template <> struct is_integral_template<unsigned long long int>  : public true_type {};


  template <class T> struct remove_const { typedef T type; };
  template <class T> struct remove_const<const T> { typedef T type; };

  template <class T> struct is_integral : public is_integral_template<typename remove_const<T>::type> {};

template<class T> struct is_const : public false_type {};
template<class T> struct is_const<T const> : public true_type {};



//template<class T> struct is_input_iterator: public has_iterator_category_convertible_to<T, input_iterator_tag> {};
//template<class T> struct is_forward_iterator: public has_iterator_category_convertible_to<T, forward_iterator_tag> {};
//template<class T> struct is_bidirectional_iterator: public has_iterator_category_convertible_to<T, bidirectional_iterator_tag> {};
//template<class T> struct is_random_access_iterator: public has_iterator_category_convertible_to<T, random_access_iterator_tag> {};

};


#endif //SRCS_IS_INTEGRAL_HPP_

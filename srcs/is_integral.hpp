
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

  template <class T> is_integral : public is_integral_template<typename remove_const<T>::type> {};

template<class T> struct is_const : public false_type {};
template<class T> struct is_const<T const> : public true {};

template<class T> struct is_reference : public false_type {};
template<class T> struct is_reference<T&> : public true_type {};
template<class T> struct is_reference<T&&> : public true_type {};
template<class T> struct is_array : public false_type {};
template<class T> struct is_array<T[]> : public true_type {};
template<class T, size_t N> struct is_array<T[N]> : public true_type {};
template<class T> struct is_function : public integral_constant<is_const<T> && !is_reference<T>> {};
template<class T> struct is_void_template : public false_type {};
template<> struct is_void_template<void> : public true_type {};
template<class T> struct is_void: public is_void_template<typename remove_const<T>::type> {};

namespace is_convertible_implementantion {
template <class T> void test_convert(T);
template <class From, class To, class = void> struct is_convertible_test: public false_type {};

//template <class From, class To> struct is_convertible_test<From, To, >


template<
         class T,
         bool isArray = is_array<T>::value,
         bool isFunction = is_function<T>::value,
         bool isVoid = is_void<T>::value
        >
struct is_array_function_or_void {enum {value = 0};};
template <class T> struct is_array_function_or_void<T, true, false, false> {enum {value = 1};};
template <class T> struct is_array_function_or_void<T, false, true, false> {enum {value = 2};};
template <class T> struct is_array_function_or_void<T, false, false, true> {enum {value = 3};};
}









//struct is_convertible_check
//
//template
//    <class T1, class T2,
//    unsigned T1_is_arra
//
//    <



template <class T1, class T2> struct is_convertible : public is_convertible_template<T1, T2>
{
  static const size_t check1 = is_convertible_check<T1>::v;
  static const size_t check2 = is_convertible_check<T2>::v;
};




template<class T, class U, bool = has_iterator_category<iterator_traits<T>>::value>
struct has_iterator_category_convertible_to
    : public integral_constant<bool, is_convertible<typename iterator_traits<T>::iterator_category, U>::value>

template<class T, class U> struct has_iterator_category_convertible_to<T, U, false>: public false_type {};

template<class T> struct is_input_iterator: public has_iterator_category_convertible_to<T, input_iterator_tag> {};
template<class T> struct is_forward_iterator: public has_iterator_category_convertible_to<T, forward_iterator_tag> {};
template<class T> struct is_bidirectional_iterator: public has_iterator_category_convertible_to<T, bidirectional_iterator_tag> {};
template<class T> struct is_random_access_iterator: public has_iterator_category_convertible_to<T, random_access_iterator_tag> {};

};


#endif //SRCS_IS_INTEGRAL_HPP_

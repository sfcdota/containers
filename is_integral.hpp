
#ifndef SRCS_IS_INTEGRAL_HPP_
#define SRCS_IS_INTEGRAL_HPP_
#include "iterator_traits.hpp"
//#include "iterator.hpp"
namespace ft {
  template<class T, T v>
  struct integral_constant {
    static const T value = v;
    typedef T value_type;
    typedef integral_constant type;
  };

  typedef integral_constant<bool, true> true_type;
  typedef integral_constant<bool, false> false_type;


template<class T> struct is_good_iterator : public false_type {};
template<> struct is_good_iterator <input_iterator_tag> : public true_type {};
template<> struct is_good_iterator <forward_iterator_tag> : public true_type {};
template<> struct is_good_iterator <bidirectional_iterator_tag> : public true_type {};
template<> struct is_good_iterator <random_access_iterator_tag> : public true_type {};


  template <class T> struct is_integral : public is_good_iterator<T>::type {};

};


#endif //SRCS_IS_INTEGRAL_HPP_


#ifndef SRCS_ITERATOR_TRAITS_HPP_
#define SRCS_ITERATOR_TRAITS_HPP_

namespace ft {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag           : public input_iterator_tag {};
struct bidirectional_iterator_tag     : public forward_iterator_tag {};
struct random_access_iterator_tag     : public bidirectional_iterator_tag {};


template<class Iterator>
class iterator_traits {
 public:
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
  typedef typename Iterator::iterator_category iterator_category;
};

template<class T>
class iterator_traits<T *> {
 public:
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef ft::random_access_iterator_tag iterator_category;
};
template<class T>
class iterator_traits<const T *> {
 public:
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef const T *pointer;
  typedef const T &reference;
  typedef ft::random_access_iterator_tag iterator_category;
};

template <class T>
struct has_iterator_category {
 private:
  typedef char yes[1];
  typedef yes no[2];
  template <class V> static yes& test (...);
  template <class V> static no& test(typename V::iterator_category* = 0);
 public:
  static const bool value = sizeof(test<T>(0)) == 1;
};

}
#endif //SRCS_ITERATOR_TRAITS_HPP_

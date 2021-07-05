#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include "iterator_traits.hpp"
namespace ft {

template<class Category, class T, class Distance = std::ptrdiff_t,
    class Pointer = T*, class Reference = T&>
struct iterator {
  typedef T value_type;
  typedef Distance difference_type;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef Category iterator_category;
};

template <class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
class const_bidirectional_iterator {
 public:
  typedef T         value_type;
  typedef Distance  difference_type;
  typedef Pointer   pointer;
  typedef Reference reference;
  typedef bidirectional_iterator_tag  iterator_category;
  const_bidirectional_iterator() { ptr_ = NULL; };
  const_bidirectional_iterator(const pointer & ptr) { ptr_ = ptr; };
  const_bidirectional_iterator(const const_bidirectional_iterator & in) { *this = in; };
  const_bidirectional_iterator & operator=(const const_bidirectional_iterator& in) { this->ptr_ = in.ptr_; return *this; };
  virtual ~const_bidirectional_iterator() {};
  bool operator==(const const_bidirectional_iterator &r) const { return this->ptr_ == r.ptr_; };
  bool operator!=(const const_bidirectional_iterator &r) const { return this->ptr_ != r.ptr_; };
  const reference operator*() { return (*ptr_)(); };
  const reference operator->() { return (*ptr_)(); };
  const_bidirectional_iterator & operator++() { ptr_ = ++(*ptr_); return *this; };
  const_bidirectional_iterator operator++(int) { const_bidirectional_iterator tmp(*this); ptr_ = ++(*ptr_); return tmp; };
  const_bidirectional_iterator & operator--() { ptr_ = --(*ptr_); return *this; };
  const_bidirectional_iterator operator--(int) { const_bidirectional_iterator tmp(*this); ptr_ = --(*ptr_); return tmp; };
  pointer GetPtr()  const { return ptr_; }
 protected:
  pointer ptr_;
};

template <class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
class bidirectional_iterator {
 public:
  typedef T         value_type;
  typedef Distance  difference_type;
  typedef Pointer   pointer;
  typedef Reference reference;
  typedef bidirectional_iterator_tag  iterator_category;
  bidirectional_iterator(): ptr_(NULL) {};
  explicit bidirectional_iterator(const pointer & ptr): ptr_(ptr) {};
  bidirectional_iterator(const value_type *ptr): ptr_(ptr) {};
  bidirectional_iterator(const bidirectional_iterator & in) { *this = in; };
  bidirectional_iterator & operator=(const bidirectional_iterator& in) { this->ptr_ = in.ptr_; return *this; };
  virtual ~bidirectional_iterator() {};
  bool operator==(const bidirectional_iterator &r) const { return this->ptr_ == r.ptr_; };
  bool operator!=(const bidirectional_iterator &r) const { return this->ptr_ != r.ptr_; };
  reference operator*() { return (*ptr_)(); };
  pointer operator->() { return &(*ptr_)(); };
  bidirectional_iterator & operator++() { ptr_ = ++(*ptr_); return *this; };
  bidirectional_iterator operator++(int) { bidirectional_iterator tmp(*this); ptr_ = ++(*ptr_); return tmp; };
  bidirectional_iterator & operator--() { ptr_ = --(*ptr_); return *this; };
  bidirectional_iterator operator--(int) { bidirectional_iterator tmp(*this); ptr_ = --(*ptr_); return tmp; };
  operator ft::const_bidirectional_iterator<Category, T>() { return const_bidirectional_iterator<Category, T>(ptr_); }
  pointer GetPtr()  const { return ptr_; }
 protected:
  pointer ptr_;
};

template <class Iterator>
 class reverse_iterator
     : public iterator<typename iterator_traits<Iterator>::iterator_category,
                       typename iterator_traits<Iterator>::value_type,
                       typename iterator_traits<Iterator>::difference_type,
                       typename iterator_traits<Iterator>::pointer,
                       typename iterator_traits<Iterator>::reference>
{
 public:
  typedef Iterator iterator_type;
  typedef typename iterator_traits<Iterator>::value_type         value_type;
  typedef typename iterator_traits<Iterator>::difference_type  difference_type;
  typedef typename iterator_traits<Iterator>::pointer   pointer;
  typedef typename iterator_traits<Iterator>::reference reference;

  reverse_iterator(): current() {}
  reverse_iterator(const pointer & ptr): current(ptr) {}

  template<class T>
  reverse_iterator(const reverse_iterator<T> & in): current(in.base()) {}

  template<class T>
  reverse_iterator & operator=(const reverse_iterator<T>& in) { current = in.base(); return *this; }
  Iterator base() const { return current; }
  virtual ~reverse_iterator() {};
 //   reference operator*() const { Iterator tmp = current; return *--tmp; };
  reference operator*() const { Iterator tmp = current; return *(tmp); };
  pointer operator->() const { return &operator*(); };
  reverse_iterator & operator++() { --current; return *this; };
  reverse_iterator operator++(int) { reverse_iterator tmp(*this); --current; return tmp; };
  reverse_iterator & operator--() { ++current; return *this; };
  reverse_iterator operator--(int) { reverse_iterator tmp(*this); ++current; return tmp; };
  reverse_iterator operator+(difference_type n) const { return reverse_iterator(current - n); }
  reverse_iterator& operator +=(difference_type n) { current -= n; return *this; }
  reverse_iterator operator-(difference_type n) const { return reverse_iterator(current + n); }
  reverse_iterator& operator -=(difference_type n) { current += n; return *this; }
  reference operator[](difference_type n) const { return *(*this + n); }


  pointer GetPtr()  const { return current.GetPtr(); }
 protected:
  Iterator current;
};

template <class Iterator1, class Iterator2>
bool operator<(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y) {
  return x.base() > y.base();
}

template <class Iterator1, class Iterator2>
bool operator!=(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y) {
  return x.base() != y.base();
}

template <class Iterator1, class Iterator2>
bool operator>(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y) {
  return x.base() < y.base();
}

template <class Iterator1, class Iterator2>
bool operator>=(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y) {
  return x.base() <= y.base();
}

template <class Iterator1, class Iterator2>
bool operator<=(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y) {
  return x.base() >= y.base();
}

template <class Iterator1, class Iterator2>
typename reverse_iterator<Iterator1>::difference_type
  operator-(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y) {
    return y.base() - x.base();
}

template <class Iterator>
reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& x) {
    return reverse_iterator<Iterator>(x.base - n);
}


//template <class Iterator>
//class const_reverse_iterator {
// public:
//  typedef typename Iterator::value_type         value_type;
//  typedef typename Iterator::difference_type  difference_type;
//  typedef typename Iterator::pointer   pointer;
//  typedef typename Iterator::reference reference;
//  const_reverse_iterator() { ptr_ = NULL; };
//  const_reverse_iterator(const pointer & ptr) { ptr_ = ptr; };
//  const_reverse_iterator(const const_reverse_iterator & in) { *this = in; };
//  const_reverse_iterator & operator=(const const_reverse_iterator& in) { this->ptr_ = in.ptr_; return *this; };
//  virtual ~const_reverse_iterator() {};
//  bool operator==(const const_reverse_iterator &r) const { return this->ptr_ == r.ptr_; };
//  bool operator!=(const const_reverse_iterator &r) const { return this->ptr_ != r.ptr_; };
//  const reference operator*() { return (*ptr_)(); };
//  const reference operator->() { return (*ptr_)(); };
//  const_reverse_iterator & operator++() { ptr_ = --(*ptr_); return *this; };
//  const_reverse_iterator operator++(int) { const_reverse_iterator tmp(*this); ptr_ = --(*ptr_); return tmp; };
//  const_reverse_iterator & operator--() { ptr_ = ++(*ptr_); return *this; };
//  const_reverse_iterator operator--(int) { const_reverse_iterator tmp(*this); ptr_ = ++(*ptr_); return tmp; };
//  pointer GetPtr()  const { return ptr_; }
// protected:
//  pointer ptr_;
//};


template <class InputIterator, class Distance>
void advance (InputIterator& it, Distance n) {
  for(Distance i = 0; i < n; i++)
    ++it;
}




template <class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
class const_random_access_iterator {
 public:
  typedef T         value_type;
  typedef Distance  difference_type;
  typedef Pointer   pointer;
  typedef Reference reference;
  typedef random_access_iterator_tag  iterator_category;

  const_random_access_iterator(){};
  const_random_access_iterator(pointer ptr): ptr_(ptr) {};
  const_random_access_iterator(const const_random_access_iterator& in) { *this = in; };
  const_random_access_iterator & operator=(const const_random_access_iterator & in) { ptr_ = in.ptr_; return *this;}
  virtual ~const_random_access_iterator() {};
  bool operator==(const const_random_access_iterator &r) const { return ptr_ == r.ptr_; };
  bool operator!=(const const_random_access_iterator &r) const { return ptr_ != r.ptr_; };
  const reference operator*() { return *ptr_; };
  const reference operator->() { return *ptr_; };
  const_random_access_iterator & operator++() { ++ptr_; return *this; };
  const_random_access_iterator operator++(int) { const_random_access_iterator tmp(*this); ++ptr_; return tmp; };
  const_random_access_iterator & operator--() { --ptr_; return *this; };
  const_random_access_iterator operator--(int) { const_random_access_iterator tmp(*this); --ptr_; return tmp; };

  const_random_access_iterator operator+(difference_type r) const { return random_access_iterator(ptr_ + r); };
  pointer operator+( ) const { return ptr_; }
  const_random_access_iterator operator-(difference_type r) const { return random_access_iterator(ptr_ - r); };
  difference_type operator-(const const_random_access_iterator &r) const { return ptr_ - r.ptr_; };
  bool operator>(const const_random_access_iterator &r) { return ptr_ > r.ptr_; };
  bool operator>=(const const_random_access_iterator &r) { return ptr_ >= r.ptr_; };
  bool operator<(const const_random_access_iterator &r) { return ptr_ < r.ptr_; };
  bool operator<=(const const_random_access_iterator &r) { return ptr_ <= r.ptr_; };
  const_random_access_iterator & operator+=(int r) { ptr_ += r; return *this; };
  const_random_access_iterator & operator-=(int r) { ptr_ -= r; return *this; };
  reference operator[](int n) { return *(ptr_ + n); }

//  operator ft::const_random_access_iterator<Category, T>() { return const_random_access_iterator<Category, T>(ptr_); }
  pointer GetPtr()  const { return ptr_; }
 protected:
  pointer ptr_;
};





template <class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
class random_access_iterator {
 public:
  typedef T         value_type;
  typedef Distance  difference_type;
  typedef Pointer   pointer;
  typedef Reference reference;
  typedef random_access_iterator_tag  iterator_category;

  random_access_iterator(){};
  random_access_iterator(pointer ptr): ptr_(ptr) {};
  random_access_iterator(const random_access_iterator& in) { *this = in; };
  random_access_iterator & operator=(const random_access_iterator & in) { ptr_ = in.ptr_; return *this;}
  virtual ~random_access_iterator() {};
  bool operator==(const random_access_iterator &r) const { return ptr_ == r.ptr_; };
  bool operator!=(const random_access_iterator &r) const { return ptr_ != r.ptr_; };
  reference operator*() { return *ptr_; };
  reference operator->() { return *ptr_; };
  random_access_iterator & operator++() { ++ptr_; return *this; };
  random_access_iterator operator++(int) { random_access_iterator tmp(*this); ++ptr_; return tmp; };
  random_access_iterator & operator--() { --ptr_; return *this; };
  random_access_iterator operator--(int) { random_access_iterator tmp(*this); --ptr_; return tmp; };

  random_access_iterator operator+(difference_type r) const { return random_access_iterator(ptr_ + r); };
  pointer operator+( ) const { return ptr_; }
  random_access_iterator operator-(difference_type r) const { return random_access_iterator(ptr_ - r); };
  difference_type operator-(const random_access_iterator &r) const { return ptr_ - r.ptr_; };
  bool operator>(const random_access_iterator &r) { return ptr_ > r.ptr_; };
  bool operator>=(const random_access_iterator &r) { return ptr_ >= r.ptr_; };
  bool operator<(const random_access_iterator &r) { return ptr_ < r.ptr_; };
  bool operator<=(const random_access_iterator &r) { return ptr_ <= r.ptr_; };
  random_access_iterator & operator+=(int r) { ptr_ += r; return *this; };
  random_access_iterator & operator-=(int r) { ptr_ -= r; return *this; };
  reference operator[](int n) { return *(ptr_ + n); }

  operator ft::const_random_access_iterator<Category, T>() { return const_random_access_iterator<Category, T>(ptr_); }
  pointer GetPtr()  const { return ptr_; }
 protected:
  pointer ptr_;
};


//template <class Iterator>
//class const_reverse_random_access_iterator {
// public:
//  typedef typename Iterator::value_type         value_type;
//  typedef typename Iterator::difference_type  difference_type;
//  typedef typename Iterator::pointer   pointer;
//  typedef typename Iterator::reference reference;
//
//  const_reverse_random_access_iterator(){};
//  const_reverse_random_access_iterator(pointer ptr): ptr_(ptr) {};
//  const_reverse_random_access_iterator(const const_reverse_random_access_iterator& in) { *this = in; };
//  const_reverse_random_access_iterator & operator=(const const_reverse_random_access_iterator & in) { ptr_ = in.ptr_; return *this;}
//  virtual ~const_reverse_random_access_iterator() {};
//  bool operator==(const const_reverse_random_access_iterator &r) const { return ptr_ == r.ptr_; };
//  bool operator!=(const const_reverse_random_access_iterator &r) const { return ptr_ != r.ptr_; };
//  const reference operator*() { return *ptr_; };
//  const reference operator->() { return *ptr_; };
//  const_reverse_random_access_iterator & operator++() { --ptr_; return *this; };
//  const_reverse_random_access_iterator operator++(int) { const_reverse_random_access_iterator tmp(*this); --ptr_; return tmp; };
//  const_reverse_random_access_iterator & operator--() { ++ptr_; return *this; };
//  const_reverse_random_access_iterator operator--(int) { const_reverse_random_access_iterator tmp(*this); ++ptr_; return tmp; };
//
//  const_reverse_random_access_iterator operator+(difference_type r) const { return const_reverse_random_access_iterator(ptr_ + r); };
//  pointer operator+( ) const { return ptr_; }
//  const_reverse_random_access_iterator operator-(difference_type r) const { return const_reverse_random_access_iterator(ptr_ - r); };
//  difference_type operator-(const const_reverse_random_access_iterator &r) const { return ptr_ - r.ptr_; };
//  bool operator>(const const_reverse_random_access_iterator &r) { return ptr_ > r.ptr_; };
//  bool operator>=(const const_reverse_random_access_iterator &r) { return ptr_ >= r.ptr_; };
//  bool operator<(const const_reverse_random_access_iterator &r) { return ptr_ < r.ptr_; };
//  bool operator<=(const const_reverse_random_access_iterator &r) { return ptr_ <= r.ptr_; };
//  const_reverse_random_access_iterator & operator+=(int r) { ptr_ += r; return *this; };
//  const_reverse_random_access_iterator & operator-=(int r) { ptr_ -= r; return *this; };
//  reference operator[](int n) { return *(ptr_ + n); }
//
////  operator ft::const_random_access_iterator<Category, T>() { return const_random_access_iterator<Category, T>(ptr_); }
//  pointer GetPtr()  const { return ptr_; }
// protected:
//  pointer ptr_;
//};
//
//template <class Iterator>
//class reverse_random_access_iterator {
// public:
//  typedef typename Iterator::value_type         value_type;
//  typedef typename Iterator::difference_type  difference_type;
//  typedef typename Iterator::pointer   pointer;
//  typedef typename Iterator::reference reference;
//
//  reverse_random_access_iterator(){};
//  reverse_random_access_iterator(pointer ptr): ptr_(ptr) {};
//  reverse_random_access_iterator(const reverse_random_access_iterator& in) { *this = in; };
//  reverse_random_access_iterator & operator=(const reverse_random_access_iterator & in) { ptr_ = in.ptr_; return *this;}
//  virtual ~reverse_random_access_iterator() {};
//  bool operator==(const reverse_random_access_iterator &r) const { return ptr_ == r.ptr_; };
//  bool operator!=(const reverse_random_access_iterator &r) const { return ptr_ != r.ptr_; };
//  reference operator*() { return *ptr_; };
//  reference operator->() { return *ptr_; };
//  reverse_random_access_iterator & operator++() { --ptr_; return *this; };
//  reverse_random_access_iterator operator++(int) { reverse_random_access_iterator tmp(*this); --ptr_; return tmp; };
//  reverse_random_access_iterator & operator--() { ++ptr_; return *this; };
//  reverse_random_access_iterator operator--(int) { reverse_random_access_iterator tmp(*this); ++ptr_; return tmp; };
//
//  reverse_random_access_iterator operator+(difference_type r) const { return reverse_random_access_iterator(ptr_ + r); };
//  pointer operator+( ) const { return ptr_; }
//  reverse_random_access_iterator operator-(difference_type r) const { return reverse_random_access_iterator(ptr_ - r); };
//  difference_type operator-(const reverse_random_access_iterator &r) const { return ptr_ - r.ptr_; };
//  bool operator>(const reverse_random_access_iterator &r) { return ptr_ > r.ptr_; };
//  bool operator>=(const reverse_random_access_iterator &r) { return ptr_ >= r.ptr_; };
//  bool operator<(const reverse_random_access_iterator &r) { return ptr_ < r.ptr_; };
//  bool operator<=(const reverse_random_access_iterator &r) { return ptr_ <= r.ptr_; };
//  reverse_random_access_iterator & operator+=(int r) { ptr_ += r; return *this; };
//  reverse_random_access_iterator & operator-=(int r) { ptr_ -= r; return *this; };
//  reference operator[](int n) { return *(ptr_ + n); };
//
//  operator ft::const_reverse_random_access_iterator<Iterator>() { return const_reverse_random_access_iterator<Iterator>(ptr_); }
//  pointer GetPtr()  const { return ptr_; }
// protected:
//  pointer ptr_;
//};


}



#endif
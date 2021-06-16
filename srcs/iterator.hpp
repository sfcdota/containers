#ifndef ITERATOR_HPP
#define ITERATOR_HPP

namespace ft {



template <class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
class const_bidirectional_iterator {
 public:
  typedef T         value_type;
  typedef Distance  difference_type;
  typedef Pointer   pointer;
  typedef Reference reference;
  typedef Category  iterator_category;
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
  typedef Category  iterator_category;
  bidirectional_iterator() { ptr_ = NULL; };
  explicit bidirectional_iterator(const pointer & ptr) { ptr_ = ptr; };
  bidirectional_iterator(const value_type *ptr) { ptr_ = ptr; };
  bidirectional_iterator(const bidirectional_iterator & in) { *this = in; };
  bidirectional_iterator & operator=(const bidirectional_iterator& in) { this->ptr_ = in.ptr_; return *this; };
  virtual ~bidirectional_iterator() {};
  bool operator==(const bidirectional_iterator &r) const { return this->ptr_ == r.ptr_; };
  bool operator!=(const bidirectional_iterator &r) const { return this->ptr_ != r.ptr_; };
  reference operator*() { return (*ptr_)(); };
  reference operator->() { return (*ptr_)(); };
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
class reverse_iterator {
 public:
  typedef typename Iterator::value_type         value_type;
  typedef typename Iterator::difference_type  difference_type;
  typedef typename Iterator::pointer   pointer;
  typedef typename Iterator::reference reference;
  reverse_iterator() { ptr_ = NULL; };
  reverse_iterator(const pointer & ptr) { ptr_ = ptr; };
  reverse_iterator(const reverse_iterator & in) { *this = in; };
  reverse_iterator & operator=(const reverse_iterator& in) { this->ptr_ = in.ptr_; return *this; };
  virtual ~reverse_iterator() {};
  bool operator==(const reverse_iterator &r) const { return this->ptr_ == r.ptr_; };
  bool operator!=(const reverse_iterator &r) const { return this->ptr_ != r.ptr_; };
  reference operator*() { return (*ptr_)(); };
  reference operator->() { return (*ptr_)(); };
  reverse_iterator & operator++() { ptr_ = --(*ptr_); return *this; };
  reverse_iterator operator++(int) { reverse_iterator tmp(*this); ptr_ = --(*ptr_); return tmp; };
  reverse_iterator & operator--() { ptr_ = ++(*ptr_); return *this; };
  reverse_iterator operator--(int) { reverse_iterator tmp(*this); ptr_ = ++(*ptr_); return tmp; };
  pointer GetPtr()  const { return ptr_; }
 protected:
  pointer ptr_;
};


template <class Iterator>
class const_reverse_iterator {
 public:
  typedef typename Iterator::value_type         value_type;
  typedef typename Iterator::difference_type  difference_type;
  typedef typename Iterator::pointer   pointer;
  typedef typename Iterator::reference reference;
  const_reverse_iterator() { ptr_ = NULL; };
  const_reverse_iterator(const pointer & ptr) { ptr_ = ptr; };
  const_reverse_iterator(const const_reverse_iterator & in) { *this = in; };
  const_reverse_iterator & operator=(const const_reverse_iterator& in) { this->ptr_ = in.ptr_; return *this; };
  virtual ~const_reverse_iterator() {};
  bool operator==(const const_reverse_iterator &r) const { return this->ptr_ == r.ptr_; };
  bool operator!=(const const_reverse_iterator &r) const { return this->ptr_ != r.ptr_; };
  const reference operator*() { return (*ptr_)(); };
  const reference operator->() { return (*ptr_)(); };
  const_reverse_iterator & operator++() { ptr_ = --(*ptr_); return *this; };
  const_reverse_iterator operator++(int) { const_reverse_iterator tmp(*this); ptr_ = --(*ptr_); return tmp; };
  const_reverse_iterator & operator--() { ptr_ = ++(*ptr_); return *this; };
  const_reverse_iterator operator--(int) { const_reverse_iterator tmp(*this); ptr_ = ++(*ptr_); return tmp; };
  pointer GetPtr()  const { return ptr_; }
 protected:
  pointer ptr_;
};


template <class InputIterator, class Distance>
void advance (InputIterator& it, Distance n) {
  for(Distance i = 0; i < n; i++)
    ++it;
}





  template <class T, class Pointer = T*, class Reference = T&>
  class random_access_iterator: public bidirectional_iterator<T, Pointer, Reference>{
   public:
    random_access_iterator(): bidirectional_iterator<T, Pointer, Reference>() {};
    random_access_iterator(const random_access_iterator& in): bidirectional_iterator<T, Pointer, Reference>(in) {};
    virtual ~random_access_iterator() {};
    random_access_iterator operator+(int r) { return this->ptr_ + r; };
    random_access_iterator operator-(int r) { return this->ptr_ - r; };
    random_access_iterator operator+(const random_access_iterator &r) { return this->ptr_ + r.ptr_; };
    random_access_iterator operator-(const random_access_iterator &r) { return this->ptr_ - r.ptr_; };
    bool operator>(const random_access_iterator &r) { return this->ptr_ > r.ptr_; };
    bool operator>=(const random_access_iterator &r) { return this->ptr_ >= r.ptr_; };
    bool operator<(const random_access_iterator &r) { return this->ptr_ < r.ptr_; };
    bool operator<=(const random_access_iterator &r) { return this->ptr_ <= r.ptr_; };
    random_access_iterator & operator+=(int r) { return this->ptr_ += r; };
    random_access_iterator & operator-=(int r) { return this->ptr_ -= r; };
    T& operator[](int n) { return *(this->ptr_ + n); }
  };
}



#endif
#ifndef VECTOR_HPP
#define VECTOR_HPP
#include "iterator.hpp"
#include "Utils.hpp"
#include "sfinae.hpp"
#include <cmath>

namespace ft {
  template<class T, class Alloc = std::allocator<T> >
  class vector {
   public:
    typedef T value_type;
    typedef Alloc allocator_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef ft::random_access_iterator<bidirectional_iterator_tag, value_type> iterator;
    typedef ft::random_access_iterator<bidirectional_iterator_tag, value_type> const_iterator;
    typedef ft::reverse_iterator<iterator> reverse_iterator;
    typedef ft::const_reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename iterator::difference_type difference_type;
    typedef size_t size_type;

    explicit vector(const allocator_type &alloc = allocator_type()) :
        allocator_(alloc), array_(allocator_.allocate(1)), capacity_(0), size_(0) {};

    explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type()) :
        allocator_(alloc), array_(allocator_.allocate(1)), capacity_(0), size_(0) { assign(n, val); }

    template<class InputIterator>
    vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(),
           typename enable<DetectIterator<InputIterator>::value, InputIterator>::type* = 0):
        allocator_(alloc), array_(allocator_.allocate(1)), capacity_(0), size_(0) { assign(first, last); }
    vector(const vector &x): allocator_(x.allocator_), array_(allocator_.allocate(1)), capacity_(0), size_(0) { *this = x; }
    ~vector() { clear(); };
    vector &operator=(const vector &x) { assign(x.begin(), x.end()); return *this; }
    iterator begin() { return iterator(array_); }
    const_iterator begin() const { return const_iterator(array_); }
    iterator end() { return iterator(array_ + size_); }
    const_iterator end() const { return const_iterator(array_ + size_); }
    reverse_iterator rbegin() { return reverse_iterator(array_ + size_ - 1); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(array_ + size_ - 1); }
    reverse_iterator rend() { return reverse_iterator(array_ - 1); }
    const_reverse_iterator rend() const { return const_reverse_iterator(array_ - 1); }
    size_type size() const { return size_; }
    size_type max_size() const { return allocator_.max_size; }

    void resize(size_type n, value_type val = value_type())
    { if (n < size_) erase(begin() + n - 1, begin() + size_); else insert(begin() + size_, begin() + n, val); }

    size_type capacity() const { return capacity_; }
    bool empty() const { return size_; }

    void reserve(size_type n) {
      if (n <= capacity_)
        return;
      pointer tmp = allocator_.allocate(n);
      for (size_type i = 0; i < size_; i++)
        tmp[i] = array_[i];
      allocator_.deallocate(array_, capacity_);
      array_ = tmp;
      capacity_ = n;
    };

    reference operator[](size_type n) { return array_[n]; }
    const_reference operator[](size_type n) const { return array_[n]; }

    reference at(size_type n)
    { if (n < 0 || n >= size_) throw std::out_of_range("out of range"); return array_[n]; }

    const_reference at(size_type n) const
    { if (n < 0 || n >= size_) throw std::out_of_range("out of range"); return array_[n]; }

    reference front() { return *array_; }
    const_reference front() const { return *array_; }
    reference back() { return *(array_ + size_ - 1); }
    const_reference back() const { return *(array_ + size_ - 1); }

    template<class InputIterator>
    void assign(InputIterator first, InputIterator last,
                typename enable<DetectIterator<InputIterator>::value, InputIterator>::type* = 0)
    { insert(begin(), first, last); };

    void assign(size_type n, const value_type &val) { insert(begin(), n, val); };
    void push_back(const value_type &val) { insert(end(), val); };
    void pop_back() { erase(end() - 1); };
    iterator insert(iterator position, const value_type &val) { insert(position, 1, val); return position - 1;};
    void insert(iterator position, size_type n, const value_type &val) {
      if (!n)
        return;
      size_type target_size = size_ + n;
      size_type index = position - begin();
      if (!capacity_)
        reserve(1);
      for (size_type i = 1; i <= static_cast<size_type>(floor(target_size / capacity_)); i *= 2)
        reserve(capacity_ * 2);
      for(size_type i = 0; i < n; ++i) {
        if (size_)
          array_[size_ + i] = array_[index + i];
        allocator_.construct(array_ + index + i, val);
      }
      size_ = target_size;
    };
    template<class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last,
                typename enable<DetectIterator<InputIterator>::value, InputIterator>::type* = 0) {
      InputIterator tmp = first;
      size_type n = 0;
      while (tmp++ != last)
        n++;
      if (!n)
        return;
      pointer target_ptr = position.GetPtr();
      insert(position, n, value_type()); //call to default-allocated insert(pos, val)
      for(pointer ptr = target_ptr - n; ptr != target_ptr; ++first)
        allocator_.construct(ptr++, *first);
    };
    iterator erase(iterator position) { return erase(position, position + 1); };

    iterator erase(iterator first, iterator last) {
      size_type n = last - first;
      if (!n)
        return last;
      pointer tmp = last.GetPtr();
      for(size_type i = 0; i < n; i++) {
        ft::swap(*(tmp - n), *tmp);
        allocator_.destroy(tmp++);
      }
      size_ -= n;
      return last;
    };
    void clear() { erase(begin(), end()); };

    allocator_type get_allocator() const { return allocator_; };

    friend bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    friend bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    friend bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    friend bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    friend bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    friend bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

   private:
    allocator_type allocator_;
    pointer array_;
    size_type capacity_;
    size_type size_;
  };
}
#endif

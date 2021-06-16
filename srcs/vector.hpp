#ifndef VECTOR_HPP
#define VECTOR_HPP

template <class T, class Alloc = allocator<T>>
class vector {
  typedef T value_type;
  typedef Alloc allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef ft::bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type>*> iterator;
  typedef ft::const_bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type>*> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::const_reverse_iterator<const_iterator> const_reverse_iterator;
  typedef typename iterator::difference_type difference_type;
  typedef size_t size_type;
 public:
  explicit vector(const allocator_type& alloc = allocator_type());
  explicit vector(size_type n, const value_type& val = value_type(),
                  const allocator_type& alloc = allocator_type());
  template<class InputIterator>
      vector(InputIterator first, InputIterator last,
             const allocator_type& alloc = allocator_type());
  vector(const vector& x);
  ~vector();
  vector& operator=(const vector&x);
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;
  size_type size() const;
  size_type max_size const;
  void resize(size_type n, value_type val = value_type());
  size_type capacity() const;
  bool empty() const;
  void reverse(size_type n);
  void shrink_to_fit();
  reference operator[](size_type n);
  const_reference operator[](size_type n) const;
  reference at(size_type n);
  const_reference at(size_type n) const;
  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;
  template<class InputIterator>
      void assign(InputIterator first, InputIterator last);
  void assign(size_type n, const value_type& val);
  void push_back (const value_type& val);
  void pop_back();
  iterator insert(iterator position, const value_type& val);
  void insert(iterator position, size_type n, const value_type& val);
  template<class InputIterator>
      void insert(iterator position, InputIterator first, InputIterator last);
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  void clear();
 private:
};
#endif

#ifndef LIST_HPP
#define LIST_HPP

template <class T, class Alloc = allocator<T>> class List {
  typedef value_type T;
  typedef allocator_type allocator<value_type>;
  typedef reference &value_type;
  typedef const_reference const &value_type;
  typedef pointer *value_type;
  typedef const_pointer const *value_type;
  typedef iterator ft::iterator<value_type>;
  typedef const_iterator ft::iterator<value_type>;
  typedef reverse_iterator ft::iterator<value_type>;
  typedef const_reverse_iterator ft::iterator<value_type>;

 public:
  //Member functions
  explicit List(const allocator_type& alloc = allocator_type());
  explicit List(size_type n, const value_type& val = value_type(),
                const allocator_type& alloc = allocator_type());
  template <class InputIterator>
      List(InputIterator first, InputIterator last,
           const allocator_type& alloc = allocator_type());
  ~List();
  List & operator=(const List& x);

  //Iterators
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;

  //Capacity
  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  //Element access
  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  //Modifiers
  template <class InputIterator>
      void assign(InputIterator first, InputIterator last);
  void assign(size_type n, const value_type& val);
  void push_front(const value_type& val);
  void pop_front();
  void push_back(const value_type& val);
  void pop_back();
  iterator insert(iterator position, const value_type& val);
  void insert(iterator position, size_type n, const value_type& val);
  template <class InputIterator>
      void insert(iterator position, InputIterator first, InputIterator last);
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  void swap(list& x);
  void resize(size_type n, value_type val = value_type());
  void clear();








 private:

};
#endif

//#ifndef MAP_HPP
//#define MAP_HPP
//
//template<
//    class Key,
//    class T,
//    class Compare = std::less<Key>,
//    class Alloc = allocator<std::pair<const Key, T>
//    >
//class map {
//  typedef key_type Key;
//  typedef mapped_type T;
//  typedef value_type std::pair<const key_type, mapped_type>;
//  typedef key_compare std::less<key_type>
//  ////////////////////typedef value_compare nullptr;
//  typedef allocator_type allocator<value_type>;
//  typedef reference &value_type;
//  typedef const_reference &value_type;
//  typedef pointer *value_type;
//  typedef const_pointer const *value_type;
//  typedef iterator ft::iterator;
//  typedef const_iterator ft::iterator;
//  typedef reverse_iterator ft::iterator;
//  typedef const_reverse_iterator ft::iterator;
//  typedef difference_type ptrdiff_t;
//  typedef size_type size_t;
// public:
//    explicit map(const key_compare& comp = key_compare(),
//                 const allocator_type& alloc = allocator_type());
//    template <class InputIterator>
//        map (InputIterator fisrt, InputIterator last,
//             const key_compare& comp = key_compare(),
//             const allocator_type& alloc = allocator_type());
//    map(cosnt map& x);
//    ~map();
//    map& operator=(const map& x);
//    iterator begin();
//    const_iterator begin() const;
//    iterator end();
//    const_iterator end() const;
//    reverse_iterator rbegin();
//    const_reverse_iterator rbegin() const;
//    reverse_iterator rend();
//    const_reverse_iterator rend() const;
//    bool empty() const;
//    size_type size() const;
//    size_type max_size() const;
//    mapped_type& operator[] (const key_type& k);
//    pair<iterator, bool> insert (const value_type& val);
//    iterator insert (iterator position, const value_type& val);
//    template <class InputIterator>
//        void insert (InputIterator first, InputIterator last);
//    void erase(iterator position);
//    size_type erase(const key_type& k);
//    void erase(iterator first, iterator last);
//    void swap(map& xx);
//    void clear();
//    key_compare key_comp() const;
//    value_compare value_comp() const;
//    iterator find(const key_type& k);
//    const_iterator find(const key_type& k) const;
//    size_type count (const key_type& k) const;
//    iterator lower_bound (const key_type& k);
//    const_iterator lower_bound (const key_type& k) const;
//    iterator upper_bound (const key_type& k);
//    const_iterator upper_bound (const key_type& k) const;
//    pair<const_iterator, const_iterator> equal_range (const key_Type& k) const;
//    pair<iterator, iterator> equal_range (const key_type& k);
// private:
//};
//#endif

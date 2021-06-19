#ifndef MAP_HPP
#define MAP_HPP

template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Alloc = allocator<std::pair<const Key, T>
    >
class map {
 public:
  typedef Key key_type ;
  typedef T mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef std::less<key_type> key_compare;
  ////////////////////typedef value_compare nullptr;
  typedef allocator<value_type> allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef ft::bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type>*> iterator;
  typedef ft::const_bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type>*> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::const_reverse_iterator<const_iterator> const_reverse_iterator;
  typedef difference_type ptrdiff_t;
  typedef size_type size_t;



 private:
  template<class Type>
  struct Node {
    Type data;
    Node* prev;
    Node* next;
    Node* operator++() { return this->next; };
    Node* operator--() { return this->prev; };
    Node& operator=(Node *in) {
      if (in != this) {
        this->prev = in->prev;
        this->next = in->next;
        this->data = in->data;
      }
      return *this;
    };
    Type & operator()() { return this->data; };
    Node(): prev(this), next(this) { };
    Node(const Type & data, Node *prev, Node *next): data(data), prev(prev), next(next) { };
  };


 public:
    explicit map(const key_compare& comp = key_compare(),
                 const allocator_type& alloc = allocator_type());
    template <class InputIterator>
        map (InputIterator fisrt, InputIterator last,
             const key_compare& comp = key_compare(),
             const allocator_type& alloc = allocator_type());
    map(cosnt map& x);
    ~map();
    map& operator=(const map& x);
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    bool empty() const;
    size_type size() const;
    size_type max_size() const;
    mapped_type& operator[] (const key_type& k);
    pair<iterator, bool> insert (const value_type& val);
    iterator insert (iterator position, const value_type& val);
    template <class InputIterator>
        void insert (InputIterator first, InputIterator last);
    void erase(iterator position);
    size_type erase(const key_type& k);
    void erase(iterator first, iterator last);
    void swap(map& xx);
    void clear();
    key_compare key_comp() const;
    value_compare value_comp() const;
    iterator find(const key_type& k);
    const_iterator find(const key_type& k) const;
    size_type count (const key_type& k) const;
    iterator lower_bound (const key_type& k);
    const_iterator lower_bound (const key_type& k) const;
    iterator upper_bound (const key_type& k);
    const_iterator upper_bound (const key_type& k) const;
    pair<const_iterator, const_iterator> equal_range (const key_Type& k) const;
    pair<iterator, iterator> equal_range (const key_type& k);

};
#endif

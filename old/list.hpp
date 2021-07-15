#ifndef LIST_HPP
#define LIST_HPP
#include "../sfinae.hpp"
#include "../Utils.hpp"

namespace ft {
template<class T, class Alloc = std::allocator<T> >
class list {
 private:
  template<class Type>
  struct Node {
    Type data;
    Node *prev;
    Node *next;
    Node *operator++() { return this->next; };
    Node *operator--() { return this->prev; };
    Node &operator=(Node *in) {
      if (in != this) {
        this->prev = in->prev;
        this->next = in->next;
        this->data = in->data;
      }
      return *this;
    };
    Type &operator()() { return this->data; };
    Node() : prev(this), next(this) {};
    Node(const Type &data, Node *prev, Node *next) : data(data), prev(prev), next(next) {};
  };

 public:
  typedef T value_type;
  typedef Alloc allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef list_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type>*> iterator;
  typedef const_list_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type>*> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef typename iterator::difference_type difference_type;
  typedef size_t size_type;

 private:
  typedef Node<value_type>* node_ptr;
  typename Alloc::template rebind<Node<value_type> >::other	allocator_;
  node_ptr end_;
  size_type size_;

  node_ptr NewNode(const_reference data, node_ptr pos) {
    node_ptr new_node = allocator_.allocate(1);
    allocator_.construct(new_node, Node<value_type>(data, pos->prev, pos));
    pos->prev->next = new_node;
    pos->prev = new_node;
    size_++;
    return (new_node);
  }

  node_ptr NewNode() {
    node_ptr new_node = allocator_.allocate(1);
    new_node->prev = new_node;
    new_node->next = new_node;
    return (new_node);
  }

  void DeleteNode(node_ptr ptr) {
    ptr->prev->next = ptr->next;
    ptr->next->prev = ptr->prev;
    allocator_.destroy(ptr);
    allocator_.deallocate(ptr, 1);
    size_--;
  }

 public:
  //Member functions
  explicit list(const allocator_type &alloc = allocator_type()): allocator_(alloc), end_(NewNode()), size_() {};

  explicit list(size_type n, const value_type &val = value_type(), const allocator_type& alloc = allocator_type()):
      allocator_(alloc), end_(NewNode()), size_()
  { assign(n, val); }

  template<class InputIterator>
  list(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
       typename enable_if<DetectIterator<InputIterator>::value, InputIterator>::type* = 0):
      allocator_(alloc), end_(NewNode()), size_()
  { assign(first, last); }

  list (const list& x):allocator_(x.allocator_), end_(NewNode()), size_() { *this = x; };

  list &operator=(const list &x) { assign(x.begin(), x.end()); return *this; }
  ~list() {
    clear();
    allocator_.deallocate(end_, 1);
  };

  //Iterators
  iterator begin() { return iterator(end_->next); };
  const_iterator begin() const { return const_iterator(end_->next); };
  iterator end() { return iterator(end_); };
  const_iterator end() const { return const_iterator(end_); };
  reverse_iterator rbegin() { return reverse_iterator(end_->prev); };
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end_->prev); };
  reverse_iterator rend() { return reverse_iterator(end_); };
  const_reverse_iterator rend() const { return const_reverse_iterator(end_); };

  //Capacity
  bool empty() const { return !size_; };
  size_type size() const { return size_; };
  size_type max_size() const { return allocator_.max_size(); } ;

  //Element access
  reference front() { return end_->next->data; };
  const_reference front() const { return end_->next->data; };
  reference back() { return end_->prev->data; };
  const_reference back() const { return end_->prev->data; };

  //Modifiers
  template<class InputIterator>
  void assign(InputIterator first, InputIterator last,
              typename enable_if<DetectIterator<InputIterator>::value, InputIterator>::type* = 0)
  { clear(); insert(iterator(end_), first, last); };

  void assign(size_type n, const value_type &val) { clear(); insert(iterator(end_), n, val); };

  void push_front(const value_type &val) { NewNode(val, end_->next); };
  void pop_front() { DeleteNode (end_->next); };
  void push_back(const value_type &val) { NewNode(val, end_); };
  void pop_back() { DeleteNode(end_->prev); };
  iterator insert(iterator position, const value_type &val) { return iterator(NewNode(val, position.GetPtr())); }
  void insert(iterator position, size_type n, const value_type &val) { while(n--) NewNode(val, position.GetPtr()); };

  template<class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last,
              typename enable_if<DetectIterator<InputIterator>::value, InputIterator>::type* = 0)
    { for(; first != last; ++first) NewNode(*first, position.GetPtr()); };

  iterator erase(iterator position) {
    node_ptr tmp(position.GetPtr()->next);
    DeleteNode(position.GetPtr());
    return iterator(tmp);
  };

  iterator erase(iterator first, iterator last) { while(first != last) erase(first++); return last; };


  void swap(list<value_type, Alloc> &x) {
    node_ptr tmp_end = x.end_;
    size_type tmp_size = x.size_;
    x.end_ = end_;
    x.size_ = size_;
    end_ = tmp_end;
    size_ = tmp_size;
  };

  void resize(size_type n, value_type val = value_type()) {
    while(size_ > n) pop_back();  while (n > size_) push_back(val);
  };

  void clear() {
    while(size_)
      pop_back();
  };



  void splice (iterator position, list& x) { splice(position, x, x.begin(), x.end()); };

  void splice (iterator position, list& x, iterator i) { insert(position, *i); x.erase(i); };

  void splice (iterator position, list& x, iterator first, iterator last) {
    insert(position, first, last);
    x.erase(first, last);
  };

  void remove (const value_type& val) {
    for(iterator it = begin(); it != end(); ++it)
      while (it != end() && *it == val)
        it = erase(it);
  };

  template <class Predicate>
  void remove_if (Predicate pred) {
    for(iterator it = begin(); it != end(); ++it)
      while (it != end() && pred(*it))
        it = erase(it);
  };


  void unique() {
    iterator tmp = begin();
    for(iterator it = ++begin(); it != end(); ++it) {
      while(it != end() && *tmp == *it)
        it = erase(it);
      ++tmp;
    }
  };

  template <class BinaryPredicate>
  void unique (BinaryPredicate binary_pred) {
    iterator tmp = begin();
    for(iterator it = ++begin(); it != end() && tmp != end(); ++it) {
      while (it != end() && binary_pred(*it, *tmp))
        it = erase(it);
      ++tmp;
    }
  };

  void merge (list& x) {
    if (&x == this)
      return;
    iterator it = begin();
    iterator xit = x.begin();
    for(; it != end() && xit != x.end(); ++it)
      if (*xit < *it) {
        insert(it, *xit);
        xit = x.erase(xit);
      }
    splice(it, x, xit, x.end());
  };

  template <class Compare>
  void merge (list& x, Compare comp) {
    if (&x == this)
      return;
    iterator it = begin();
    iterator xit = x.begin();
    for(; it != end() && xit != x.end(); ++it)
      if (comp(*xit, *it)) {
        insert(it, *xit);
        xit = x.erase(xit);
      }
    splice(it, x, xit, x.end());
  }

  void sort() {
    for(iterator i = begin(); i != end(); ++i)
      for(iterator j = begin(); j != end(); ++j)
        if (*i < *j)
          ft::swap(*i, *j);
  };

  template <class Compare>
  void sort (Compare comp) {
    for(iterator i = begin(); i != end(); ++i)
      for(iterator j = begin(); j != end(); ++j)
        if (comp(*i, *j))
          ft::swap(*i, *j);
  };

  void reverse() {
    for(iterator itb = begin(), ite = --end(); itb != ite; ++itb, --ite)
      ft::swap(*itb, *ite);
  };

  friend bool operator==(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs) {
    if (lhs.size_ != rhs.size_)
      return false;
    bool res = true;
    for(const_iterator lit = lhs.begin(), rit = rhs.begin(); res && lit != lhs.end() && rit != rhs.end(); ++lit, ++rit)
      res = *lit == *rit;
    return res;
  };

  friend bool operator!=(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs) { return !(lhs == rhs); }

  friend bool operator<  (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
  { return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }


  friend bool operator<= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs) { return !(rhs < lhs); }


  friend bool operator>  (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs) { return rhs < lhs; };


  friend bool operator>= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs) { return !(lhs < rhs); }



  friend void swap (list<T,Alloc>& x, list<T,Alloc>& y) { x.swap(y); };



};
}
#endif

#ifndef MAP_HPP
#define MAP_HPP
#include <cmath>
#include "Utils.hpp"
namespace ft {

  template<class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
  class map {
   public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef ft::pair<const key_type, mapped_type> value_type;
    typedef Compare key_compare;

    class value_compare: public binary_function<value_type, value_type, bool>  {   // in C++98, it is required to inherit binary_function<value_type,value_type,bool>
      friend class map;
     protected:
      key_compare comp;
      value_compare (key_compare c) : comp(c) {}  // constructed with map's comparison object
     public:
      bool operator() (const value_type& x, const value_type& y) const
      { return comp(x.first, y.first); }
    };

    typedef typename Alloc::template rebind<Node<value_type> >::other allocator_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef ft::bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type> * >
        iterator;
    typedef ft::const_bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type> * >
        const_iterator;
    typedef ft::reverse_iterator<iterator> reverse_iterator;
    typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef ptrdiff_t difference_type ;
    typedef size_t size_type;

   private:
    allocator_type allocator_;
    key_compare kcomp;
    typedef Node<T>* nodeptr;
    nodeptr end_;
    Tree<T, Compare, Alloc> tree_;


   public:
    explicit map(const key_compare &comp = key_compare(),
                 const allocator_type &alloc = allocator_type()): allocator_(alloc), kcomp(comp) {

    }
    template<class InputIterator>
    map(InputIterator first, InputIterator last,
        const key_compare &comp = key_compare(),
        const allocator_type &alloc = allocator_type()):allocator_(alloc), kcomp(comp) {
      insert(first, last);
    }
    map(const map &x) {
      *this = x;
    }
    ~map() {};
    map &operator=(const map &x) {
      this->kcomp = x.kcomp;
    }
    iterator begin() {

    }
    const_iterator begin() const {

    }
    iterator end() {

    }
    const_iterator end() const {

    }
    reverse_iterator rbegin() {

    }
    const_reverse_iterator rbegin() const {

    }
    reverse_iterator rend() {

    }
    const_reverse_iterator rend() const;
    bool empty() const;
    size_type size() const;
    size_type max_size() const;
    mapped_type &operator[](const key_type &k);
    pair<iterator, bool> insert(const value_type &val);
    iterator insert(iterator position, const value_type &val);
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last);
    void erase(iterator position);
    size_type erase(const key_type &k);
    void erase(iterator first, iterator last);
    void swap(map &xx);
    void clear();
    key_compare key_comp() const;
    value_compare value_comp() const;
    iterator find(const key_type &k);
    const_iterator find(const key_type &k) const;
    size_type count(const key_type &k) const;
    iterator lower_bound(const key_type &k);
    const_iterator lower_bound(const key_type &k) const;
    iterator upper_bound(const key_type &k);
    const_iterator upper_bound(const key_type &k) const;
    pair<const_iterator, const_iterator> equal_range(const key_type &k) const;
    pair<iterator, iterator> equal_range(const key_type &k);

  };
}
#endif

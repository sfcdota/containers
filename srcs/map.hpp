#ifndef MAP_HPP
#define MAP_HPP
#include <cmath>
#include "Utils.hpp"
#include "iterator.hpp"
#include "tree.hpp"
#include "sfinae.hpp"
namespace ft {

  template<class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
  class map {

   public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef ft::pair<const key_type, mapped_type> value_type;
    typedef Compare key_compare;
    typedef Alloc allocator_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;

    class value_compare: public binary_function<value_type, value_type, bool>  {   // in C++98, it is required to inherit binary_function<value_type,value_type,bool>
      friend class map;
     protected:
      key_compare comp;
      value_compare (key_compare c) : comp(c) {}  // constructed with map's comparison object
     public:
      bool operator() (const value_type& x, const value_type& y) const
      { return comp(x.first, y.first); }
    };

   public:
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef ft::bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type> * > iterator;
    typedef ft::const_bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type> * >  const_iterator;
    typedef ft::reverse_iterator<iterator> reverse_iterator;
    typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;

   private:
    Alloc allocator_;
    tree<value_type, value_compare, allocator_type> tree_;
    typedef value_compare vc;
//    value_compare vc;
   public:
    explicit map(const key_compare &comp = key_compare(),
                 const allocator_type &alloc = allocator_type()): allocator_(alloc), tree_(vc(comp), allocator_) { }
    template<class InputIterator>
    map(InputIterator first, InputIterator last,
        const key_compare &comp = key_compare(),
        const allocator_type &alloc = allocator_type(),
        typename enable_if<true, has_iterator_category<InputIterator> >
//                              is_input_iterator<InputIterator>::value &&
//                              !is_forward_iterator<InputIterator>::value &&
//                              is_constructioble<value_type, typename iterator_traits<InputIterator>::reference>::value,
                           ::type* = 0): tree_(vc(comp)),  allocator_(alloc) {
      insert(first, last);
    }
    map(const map &x): tree_(x.tree_) {
      insert(x.begin(), x.end());
    }
    ~map() {};
    map &operator=(const map &x) {
      tree_ = x.tree_;
      return *this;
    }
    iterator begin() { return iterator(tree_.tree_min()); };
    const_iterator begin() const { return iterator(tree_.tree_min()); };
    iterator end() { return iterator(tree_.tree_max()); };
    const_iterator end() const { return iterator(tree_.tree_max()); };
    reverse_iterator rbegin() { return iterator(tree_.tree_max()); };
    const_reverse_iterator rbegin() const { return iterator(tree_.tree_max()); };
    reverse_iterator rend() { return iterator(tree_.tree_min()); };
    const_reverse_iterator rend() const { return iterator(tree_.tree_min()); };
    bool empty() const { return tree_.size() == 0; }
    size_type size() const { return tree_.size(); }
    size_type max_size() const { return tree_.max_size(); }
    mapped_type &operator[](const key_type &k) { return tree_.GetNode(k)->data; }
    pair<iterator, bool> insert(const value_type &val) { return tree_.insert(val); }
    iterator insert(iterator position, const value_type &val) { return iterator(tree_.insert(position.ptr_, val)); }
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last,
                typename enable_if<DetectIterator<T>::value,
//                              is_input_iterator<InputIterator>::value &&
//                              !is_forward_iterator<InputIterator>::value &&
//                              is_constructioble<value_type, typename iterator_traits<InputIterator>::reference>::value,
                                   InputIterator>::type* = 0) {
      for(iterator it = end(); first != last; ++first)
        insert(it.ptr_, *first);
    }
    void erase(iterator position) { tree_.erase(position.ptr_); }

    //todo size_type return!
    size_type erase(const key_type &k) { return tree_.erase(k); }
    void erase(iterator first, iterator last) { return tree_.erase(first.ptr_, last.ptr_); }
    void swap(map &xx) { tree_.swap(xx.tree_); }
    void clear() { tree_.clear(); };
    key_compare key_comp() const { return tree_.value_comp().key_comp(); }
    value_compare value_comp() const { return value_compare(tree_.value_comp().key_comp()); }
    iterator find(const key_type &k) { return tree_.find(k); }
    const_iterator find(const key_type &k) const { return tree_.find(k); }
    size_type count(const key_type &k) const { return tree_.count(k); }
    iterator lower_bound(const key_type &k) { return tree_.lower_bound(k); }
    const_iterator lower_bound(const key_type &k) const { return lower_bound(k); }
    iterator upper_bound(const key_type &k) { return upper_bound(k); }
    const_iterator upper_bound(const key_type &k) const { return upper_bound(k); }
    pair<const_iterator, const_iterator> equal_range(const key_type &k) const { return tree_.equal_range(k); }
    pair<iterator, iterator> equal_range(const key_type &k) { return tree_.equal_range(k); }
    allocator_type get_allocator() const { return allocator_type(tree_.alloc()); }
  };
}
#endif

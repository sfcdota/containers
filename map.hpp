#ifndef MAP_HPP
#define MAP_HPP
#include <cmath>
#include "Utils.hpp"
#include "iterator.hpp"
//#include "tree.hpp"
#include "sfinae.hpp"
namespace ft {

  template<class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
  class map {

   public:
    typedef Key key_type;
    typedef T mapped_type;
//    typedef ft::pair<key_type, mapped_type> value_type;
    typedef ft::pair<const key_type, mapped_type> value_type;
    typedef Compare key_compare;


    class value_compare  {   // in C++98, it is required to inherit binary_function<value_type,value_type,bool>
//     friend class map;
     public:
      key_compare comp;
      value_compare (key_compare c) : comp(c) {}  // constructed with map's comparison object
     public:

      bool operator() (const value_type& x, const value_type& y) const
      { return comp(x.first, y.first); }

      bool operator()(const value_type & x, const key_type & y) const
      { return comp(x.first, y); }

      bool operator()(const key_type & x, const value_type & y) const
      { return comp(x, y.first); }
    };
   private:
    enum Color {
      red,
      black
    };
    template<class V>
    class Node {
     public:
      typedef V value_type;
      value_type data;
      Node *parent;
      Node* end;
      Color color;
      Node *left;
      Node *right;
      Node *operator++() {
        Node* n = this;
        if (n->right)
          return min(n->right);
        while (n && !is_left(n))
          n = n->parent;
        return n && n->parent ? n->parent : end; // if NULL then n was the most right (max) Node of the tree
      }
      Node *operator--() {
        Node* n = this;
        if (n->left)
          return max(n->left);
        while (n && is_left(n))
          n = n->parent;
        return n && n->parent ? n->parent : end;
      }
      Node &operator=(Node *in) {
        if (in != this) {
          this->left = in->left;
          this->right = in->right;
          this->data = in->data;
          this->color = in->color;
          this->end = in->end;
        }
        return *this;
      }
      bool is_left(Node* n) { return n->parent && n->parent->left == n; }
      Node* min() {
        return min(this);
      }
      Node* min(Node* n) {
        while(n && n->left)
          n = n->left;
        return n;
      }
      Node* max() {
        return max(this);
      }
      Node* max(Node* n) {
        while(n && n->right)
          n = n->right;
        return n;
      }
      Node* get_root() {
        Node* n = this;
        while(n && n->parent)
          n = n->parent;
        return n;
      }
      value_type &operator()() { return this->data; };
      Node(): left(NULL), right(NULL), parent(NULL), color() {};
      Node(Node* end, const value_type &data, Node* parent = NULL, Color color = red, Node *prev = NULL, Node *next = NULL)
          : data(data),  parent(parent),   end(end), color(color),left(prev),right(next) {};
      ~Node() {};
    };



   public:
    typedef typename Alloc::template rebind<Node<value_type> >::other allocator_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef ft::map_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type>*> iterator;
    typedef ft::const_map_iterator< bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type>*> const_iterator;
    typedef ft::reverse_iterator<iterator> reverse_iterator;
    typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;

   private:
    allocator_type allocator_;
    std::allocator<value_type > value_allocator_;
    typedef Node<value_type>* nodeptr;
    nodeptr root_;
    nodeptr end_;
    size_type size_;
    key_compare comp_;
    value_compare vc;
    nodeptr tree_min(nodeptr n) const{
      return n->min();
    }
    nodeptr tree_min() const {
      return end_->right->min();
    }
    nodeptr tree_max(nodeptr n) const{
      return n->max();
    }
    nodeptr tree_max() const{
      return end_->right->max();
    }
    void printBT(const std::string& prefix, nodeptr node, bool isLeft)
    {
      std::cout << prefix;
      std::cout << (isLeft ? "├──" : "└──" );
      if (node) {
        std::cout << (node->color == black ? "b" : "r") << node->data << std::endl;
        if (node->left || node->right) { // print NIL on child only if second exists
          printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
          printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
      }
      else
        std::cout << "NIL" << std::endl;
    }
   public:
    void printBT(nodeptr node = NULL)
    {
      printBT("", node ? node : root_, false);
    }
   private:
    nodeptr GetRoot(nodeptr n) const {
      return n->get_root();
    }


    template<class V> nodeptr GetNode(const V & data) {
      nodeptr p = root_;
      while (p)
        if (vc(data, p->data))
          p = p->left;
        else if (vc(p->data, data))
          p = p->right;
        else
          return p;
      return p;
    }

    nodeptr GetNode(const key_type & data) const {
      nodeptr p = GetRoot(end_->left);
      while (p)
        if (vc(data, p->data))
          p = p->left;
        else if (vc(p->data, data))
          p = p->right;
        else
          return p;
      return p;
    }


    template<class V> value_type GetNodeValue(const V & data) {
      return GetNode(data)->data.second;
    }
    nodeptr NewNode() {
      nodeptr new_node = allocator_.allocate(1);
      new_node->left = new_node;
      new_node->right = new_node;
      return new_node;
    }
    nodeptr NewNode(const value_type &Data, nodeptr parent = NULL, Color color = red, nodeptr prev = NULL, nodeptr next = NULL) {
      nodeptr tmp = allocator_.allocate(1);
      allocator_.construct(tmp, Node<value_type>(end_, Data, parent, color, prev, next));
      size_++;
      return tmp;
    }
    void DeleteNode(nodeptr & n) {
      if (n != end_)
        allocator_.destroy(n);
      allocator_.deallocate(n, 1);
      n = NULL;
      size_--;
    }
    nodeptr Grandparent(nodeptr n) { return n && n->parent ? n->parent->parent : NULL; }
    nodeptr Uncle(nodeptr n) {
      nodeptr g = Grandparent(n);
      if (!g)
        return NULL;
      return n->parent == g->left ? g->right : g->left;
    }
    void rotate_left(nodeptr n) {
      nodeptr pivot = n->right;
      if (n == root_)
        root_ = pivot;
      pivot->parent = n->parent;
      if (n->parent) {
        if (n->parent->left == n)
          n->parent->left = pivot;
        else
          n->parent->right = pivot;
      }
      n->right = pivot->left;
      if (pivot->left)
        pivot->left->parent = n;
      n->parent = pivot;
      pivot->left = n;
    }
    void rotate_right(nodeptr n) {
      nodeptr pivot = n->left;
      if (n == root_)
        root_ = pivot;
      pivot->parent = n->parent;
      if (n->parent) {
        if (n->parent->left == n)
          n->parent->left = pivot;
        else
          n->parent->right = pivot;
      }
      n->left = pivot->right;
      if (pivot->right)
        pivot->right->parent = n;
      n->parent = pivot;
      pivot->right = n;
    }
    bool IsLeftChild(nodeptr n) { return n->parent && n->parent->left == n; }


    template<class Comp>
    nodeptr insert_node(const value_type & data, Comp compare_, bool & t) {
      if (!root_) {
        root_ = NewNode(data, NULL, black);
        update_end_(root_->max(), root_->min());
        return root_;
      }
      else {
        nodeptr cur = root_;
        nodeptr tmp = root_;
        for (; cur ;(cur = compare_(data, cur->data) ? cur->left : cur->right)) {
          if (cur) {
            tmp = cur;
            if (!compare_(data, tmp->data) && !compare_(tmp->data, data)) {
              t = false;
              return tmp;
            }
          }
        }
        cur = NewNode(data, tmp);
        if (compare_(data, tmp->data))
          tmp->left = cur;
        else
          tmp->right = cur;
        FixInsertion(cur);
        update_end_(root_->max(), root_->min());
        return cur;
      }
    }
    void FixInsertion(nodeptr n) {
      nodeptr p;
      nodeptr unc;
      bool parentIsLeft;
      while ((p = n->parent) && p->color == red) {
        if ((unc = Uncle(n)) && unc->color == red) {
          p->color = black;
          unc->color = black;
          p->parent->color = red;
          n = p->parent;
        }
        else {
          parentIsLeft = IsLeftChild(p);
          if (parentIsLeft && !IsLeftChild(n)) {
            rotate_left(n->parent);
            n = n->left;
          }
          else if (!parentIsLeft && IsLeftChild(n)) {
            rotate_right(n->parent);
            n = n->right;
          }
          p = n->parent;
          nodeptr gp = Grandparent(n);
          p->color = black;
          gp->color = red;
          if (IsLeftChild(n) && IsLeftChild(p))
            rotate_right(gp);
          else
            rotate_left(gp);

        }
      }
      if (p)
        root_ = GetRoot(root_);
      root_->color = black;
    }


    void delete_node_(nodeptr p) {
      if (!p)
        return;
      nodeptr y = NULL, x = NULL;
      if (!p->left || !p->right)
        y = p;
      else {
        y = p->right;
        while (y->left)
          y = y->left;
      }
      if (y->left)
        x = y->left;
      else
        x = y->right;
      if (x)
        x->parent = y->parent;
      if (y->parent) {
        if (IsLeftChild(y))
          y->parent->left = x;
        else
          y->parent->right = x;
      } else
        root_ = x;
      bool y_is_black = y->color == black;
      if (y != p) {
        y->parent = p->parent;
        if (p->parent) {
          if (IsLeftChild(p)) {
            p->parent->left = y;
          }
          else
            p->parent->right = y;
        }
        if (p->left)
          p->left->parent = y;
        if (p->right)
          p->right->parent = y;
//        value_allocator_.construct(&(p->data), y->data);
//        const_cast<key_type &>(p->data.first) = y->data.first;
        y->color = p->color;
        y->left = p->left;
        y->right = p->right;
        //p->data = y->data;
        if (root_ == p)
          root_ = y;
      }
      if (y_is_black)
        FixDeleting(x);
      DeleteNode( y != p ? p : y);
      root_ = GetRoot(root_);
      update_end_(root_->max(), root_->min());
    }
    nodeptr delete_node(const value_type & data) {
      return delete_node_(GetNode(data));
    }
    nodeptr sibling(nodeptr n) {
      if (!n->parent)
        return NULL;
      return IsLeftChild(n) ? n->parent->right : n->parent->left;
    }
    void FixDeleting(nodeptr n) {
      nodeptr s;
      while(n && n->parent != NULL && n->color == black) {
        s = sibling(n);
        if (IsLeftChild(n)) {
          if (s->color == red) {
            s->color = black;
            s->parent->color = red;
            rotate_left(n->parent);
            s = n->parent->right;
          }
          if (s->left && s->left->color == black &&
              s->right && s->right->color == black) {
            s->color = red;
            n = n->parent;
          }
          else {
            if (s->right->color == black) {
              s->left->color = black;
              s->color = red;
              rotate_right(s);
              s = n->parent->right;
            }
            s->color = n->parent->color;
            n->parent->color = black;
            s->right->color = black;
            rotate_left(n->parent);
            n = root_;
          }
        }
        else {
          if (s->color == red) {
            s->color = black;
            n->parent->color = red;
            rotate_right(n->parent);
            s = n->parent->left;
          }
          if (s->left && s->left->color == black &&
              s->right && s->right->color == black) {
            s->color = red;
            n = n->parent;
          }
          else {
            if (s->left->color == black) {
              s->right->color = black;
              s->color = red;
              rotate_left(s);
              s = n->parent->left;
            }
            s->color = n->parent->color;
            n->parent->color = black;
            s->left->color = black;
            rotate_right(n->parent);
            n = root_;
          }
        }
      }
      if (n)
        n->color = black;
    }
    int GetHeight(nodeptr n) {
      return n ? ft::max(GetHeight(n->left), GetHeight(n->right)) + 1 : 0;
    }


    void delete_tree_elems(nodeptr root) {
      if (root) {
        delete_tree_elems(root->left);
        delete_tree_elems(root->right);
        DeleteNode(root);
//        if (root->left)
//          root->left = NULL;
//        if (root->right)
//          root->right = NULL;
        root = NULL;
      }
    }

    void delete_tree() {
      delete_tree_elems(root_);
      root_ = NULL;
      update_end_(end_, end_);
    }

    void update_end_(nodeptr left, nodeptr right) {
      if (!left && !right) {
        end_->left = end_;
        end_->right = end_;
      }
      else {
        end_->left = left;
        end_->right = right;
      }

    }
//    nodeptr get_node_by_key(const key_type &k) {
//      nodeptr p = GetRoot(end_->left);
//      while (p)
//        if (Compare()(k, p->data.first))
//          p = p->left;
//        else if (Compare()(p->data.first,k))
//          p = p->right;
//        else
//          break;
//      return p;
//    }
//
//    mapped_type get_value_by_key(const key_type &k) {
//      return get_node_by_key(k)->data.second;
//    }


    size_t count_equal_key_nodes(const key_type &data) const {
      size_t i = 0;
      for (nodeptr r = GetNode(data); r && !vc(r->data, data) && !vc(data, r->data); r = r->right)
        i++;
      return i;
    }


    nodeptr lower_bound_node(const key_type & data) {
      nodeptr i = end_->right;
      for(; i != end_ && vc(i->data, data); i =++(*i));
      return i;
    }



    nodeptr upper_bound_node(const key_type & data) {
      nodeptr i = end_->right;
      for(; i != end_ && !vc(data,i->data); i =++(*i));
      return i;
    }

//    value_compare vc;
   public:
    explicit map(const key_compare &comp = key_compare(),
      const allocator_type &alloc = allocator_type()): allocator_(alloc), root_(NULL), size_(0), comp_(comp), vc(comp_) {
      end_ = NewNode();
      end_->parent = NULL;
      end_->left = end_;
      end_->right = end_;
    };

    template<class InputIterator>
    map(InputIterator first, InputIterator last,
        const key_compare &comp = key_compare(),
        const allocator_type &alloc = allocator_type(),
        typename enable_if<true, has_iterator_category<InputIterator> >::type* = 0)
        : allocator_(alloc), root_(NULL), size_(0), comp_(comp), vc(comp_) {
        end_ = NewNode();
        end_->parent = NULL;
        end_->left = end_;
        end_->right = end_;
      insert(first, last);
    }

    map(const map &x): allocator_(x.allocator_), root_(NULL), size_(0), comp_(x.comp_), vc(comp_) {
      end_ = NewNode();
      end_->parent = NULL;
      end_->left = end_;
      end_->right = end_;
      insert(x.begin(), x.end());
    }

    /*
     *     typedef value_compare vc;
    node_allocator allocator_;
    nodeptr end_;
    size_type size_;
     */

    //todo delete end_
    ~map() { clear(); if (end_) DeleteNode(end_); };
    map &operator=(const map &x) {
      if (this != &x) {
        clear();
        vc = x.vc;
        insert(x.begin(), x.end());
      }
      return *this;
    }

    iterator begin() { return iterator(end_->right); };
    const_iterator begin() const { return const_iterator(end_->right); };
    iterator end() { return iterator(end_); };
    const_iterator end() const { return const_iterator(end_); };
    reverse_iterator rbegin() { return reverse_iterator(end_->left); };
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end_->left); };
    reverse_iterator rend() { return reverse_iterator(end_); };
    const_reverse_iterator rend() const { return const_reverse_iterator(end_); };
    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    size_type max_size() const { return allocator_.max_size(); }
    mapped_type &operator[](const key_type &k)
        { return (*((this->insert(ft::make_pair(k,mapped_type()))).first)).second; }
    pair<iterator, bool> insert(const value_type &val) {
      bool t = true;
      return ft::make_pair(iterator(insert_node(val, vc, t)), t);
    }
    iterator insert(iterator position, const value_type &val) { (void)position; return iterator(insert(val).first); }
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) {
      for(iterator it = end(); first != last; ++first)
        insert(*first);
    }
    void erase(iterator position) { delete_node_(position.GetPtr()); }

    //todo size_type return!
    size_type erase(const key_type &k) {
      nodeptr p = GetNode(k);
      bool deleted = p;
      delete_node_(p);
      return deleted;
      //allocator destroy <-
    }
    void erase(iterator first, iterator last) {
      while(first != last) {
        iterator tmp = first++;
        erase(tmp);
      }
    }
    void swap(map &xx) {
      ft::swap(this->end_, xx.end_);
      ft::swap(this->size_, xx.size_);
      ft::swap(this->vc, xx.vc);
    }
    void clear() { delete_tree(); };
    key_compare key_comp() const { return comp_; }
    value_compare value_comp() const { return value_compare(comp_); }
    iterator find(const key_type &k) { return iterator(GetNode<const key_type>(k)); }
    const_iterator find(const key_type &k) const { return const_iterator(GetNode(k)); }
    size_type count(const key_type &k) const { return count_equal_key_nodes(k); }
    iterator lower_bound(const key_type &k) { return iterator(lower_bound_node(k)); }
    const_iterator lower_bound(const key_type &k) const { return const_iterator(lower_bound_node(k)); }
    iterator upper_bound(const key_type &k) { return iterator(upper_bound_node(k)); }
    const_iterator upper_bound(const key_type &k) const { return const_iterator(upper_bound_node(k)); }
    pair<const_iterator, const_iterator> equal_range(const key_type &k) const {
//      nodeptr right = GetNode<key_type, key_compare>(k);
//      if (!right)
//        return NULL;
//      nodeptr left = right;
//      nodeptr tmp;
//      while(!key_comp()(left->data, k) && !key_comp()(k, left->data)) {
//        tmp = left;
//        left = left->left;
//      }
//      if (!left)
//        left = tmp;
//
//      while(!key_comp()(right->data, k) && !key_comp()(k, right->data)) {
//        tmp = right;
//        right = right->right;
//      }
//      if (!right)
//        right = tmp;
      return make_pair(lower_bound(k), upper_bound(k));
    }
    pair<iterator, iterator> equal_range(const key_type &k) {
//      nodeptr right = GetNode(k);
//      if (!right)
//        return make_pair(iterator(end_), iterator(end_));
//      nodeptr left = right;
//      nodeptr tmp;
//      while(!vc(left->data, k) && !vc(k, left->data)) {
//        tmp = left;
//        left = left->left;
//      }
//      if (!left)
//        left = tmp;
//
//      while(!vc(right->data, k) && !vc(k, right->data)) {
//        tmp = right;
//        right = right->right;
//      }
//      if (!right)
//        right = tmp;
      return make_pair(lower_bound(k), upper_bound(k));
    }
    allocator_type get_allocator() const { return allocator_; }



    friend bool operator== ( const map<Key,T,Compare,Alloc>& lhs,
                      const map<Key,T,Compare,Alloc>& rhs ) {
      if (lhs.size_ != rhs.size_)
        return false;
      bool res = true;
      for(const_iterator lit = lhs.begin(), rit = rhs.begin(); res && lit != lhs.end() && rit != rhs.end(); ++lit, ++rit)
        res = *lit == *rit;
      return res;
    }
    friend bool operator!= ( const map<Key,T,Compare,Alloc>& lhs,
                      const map<Key,T,Compare,Alloc>& rhs ) { return !(lhs == rhs); };
    friend bool operator<  ( const map<Key,T,Compare,Alloc>& lhs,
                      const map<Key,T,Compare,Alloc>& rhs ) { return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); };
    friend bool operator<= ( const map<Key,T,Compare,Alloc>& lhs,
                      const map<Key,T,Compare,Alloc>& rhs ) { return !(rhs < lhs); }
    friend bool operator>  ( const map<Key,T,Compare,Alloc>& lhs,
                      const map<Key,T,Compare,Alloc>& rhs ) { return rhs < lhs; };
    friend bool operator>= ( const map<Key,T,Compare,Alloc>& lhs,
                      const map<Key,T,Compare,Alloc>& rhs ) { return !(lhs < rhs); }

    friend  void swap (map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y) { x.swap(y); }
  };
}
#endif

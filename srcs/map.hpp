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
   private:
    enum Color {
      red,
      black
    };
    class Node {
     public:
      value_type data;
      Node* tree_end_pointer;
      Node *parent;
      Color color;
      Node *left;
      Node *right;
      Node *operator++() {
        Node* n = this;
        if (n->right)
          return min(n->right);
        while (!is_left(n))
          n = n->parent;
        return n->parent ? n->parent : tree_end_pointer; // if NULL then n was the most right (max) Node of the tree
      }
      Node *operator--() {
        Node* n = this;
        if (n->left)
          return max(n->left);
        while (is_left(n))
          n = n->parent;
        return n->parent ? n->parent : tree_end_pointer;
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
        Node* n = this;
        return min(n);
      }
      Node* min(Node* n) {
        while(n && n->left)
          n = n->left;
        return n;
      }
      Node* max() {
        Node* n = this;
        return max(n);
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
      Node(Node* end_tree_pointer, const value_type &data, Node* parent = NULL, Color color = red, Node *prev = NULL, Node *next = NULL)
          : tree_end_pointer(end_tree_pointer), data(data), parent(parent), color(color), left(prev), right(next) {};
      ~Node() {};
    };



   public:
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef ft::map_iterator<bidirectional_iterator_tag, value_type> iterator;
    typedef ft::map_iterator<bidirectional_iterator_tag, value_type>  const_iterator;
    typedef ft::reverse_iterator<iterator> reverse_iterator;
    typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;

   private:
    typedef typename Alloc::template rebind<Node>::other node_allocator;
    typedef Node* nodeptr;
    typedef value_compare vc;
    node_allocator allocator_;
    nodeptr root_;
    nodeptr begin_;
    nodeptr end_;
    size_type size_;
    key_compare comp_;

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
        if (node->left || node->right) {
          printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
          printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
      }
      else
        std::cout << "NIL" << std::endl;
    }
    void printBT(nodeptr node)
    {
      printBT("", node, false);
    }
    nodeptr GetRoot(nodeptr n) {
      return n->get_root();
    }
    template<class V, class comp> nodeptr GetNode(const V & data) {
      nodeptr p = GetRoot(end_->left);
      while (p)
        if (comp()(data, p->data))
          p = p->left;
        else if (comp()(p->data, data))
          p = p->right;
        else
          return;
      return p;
    }
    template<class V, class comp> value_type GetNodeValue(const V & data) {
      return GetNode<V, comp>(data)->data.second;
    }
    nodeptr NewNode() {
      nodeptr new_node = allocator_.allocate(1);
      new_node->left = new_node;
      new_node->right = new_node;
      return new_node;
    }
    nodeptr NewNode(nodeptr endpointer, const value_type &Data, nodeptr parent = NULL, Color color = red, nodeptr prev = NULL, nodeptr next = NULL) {
      nodeptr tmp = allocator_.allocate(1);
      allocator_.construct(endpointer, tmp, nodeelem(Data, parent, color, prev, next));
      size_++;
      return tmp;
    }
    void DeleteNode(nodeptr n) {
      allocator_.destroy(n);
      allocator_.deallocate(n, 1);
      size_--;
    }
    nodeptr Grandparent(nodeptr n) { return n && n->parent ? n->parent->parent : NULL; }
    nodeptr Uncle(nodeptr n) {
      nodeptr g = Grandparent(n);
      if (!g)
        return NULL;
      return n->parent == g->left ? g->right : g->left;
    }
    void rotate_left(nodeptr n, nodeptr & root) {
      nodeptr pivot = n->right;
      if (n == root)
        root = pivot;
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
    void rotate_right(nodeptr n, nodeptr & root) {
      nodeptr pivot = n->left;
      if (n == root)
        root = pivot;
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


    nodeptr insert_node(const value_type & data, Compare compare_, bool & t) {
      if (!root_) {
        root_ = NewNode(end_, data, NULL, black);
        update_end_(root_->min(), root_->max());
        return root_;
      }
      else {
        nodeptr cur = root_;
        nodeptr tmp = root_;
        while ((cur = compare_()(data, cur->data) ? cur->left : cur->right))
          tmp = cur;
        if (!compare_()(data, tmp->data) && !compare_()(tmp->data, data)) {
          t = true;
          return tmp;
        }
        cur = NewNode(end_, data, tmp);
        if (compare_()(data, tmp->data))
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
      if (p) {
        root_ = GetRoot(root_);
        root_->color = black;
      }
    }


    nodeptr delete_node_(nodeptr p) {
      nodeptr retval = p;
      ++retval;
      if (!p)
        return retval;
      nodeptr src = p;
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
      if (y != p)
        p->data = y->data;
      if (y->color == black)
        FixDeleting(x);
      DeleteNode(src);
      update_end_(root_->max(), root_->min());
      return retval;
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
      while(n->color == black && n != root_) {
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
      n->color = black;
    }
    int GetHeight(nodeptr n) {
      return n ? ft::max(GetHeight(n->left), GetHeight(n->right)) + 1 : 0;
    }
    void delete_node_data(nodeptr node) {
      allocator_.destroy(node);
    }

    void delete_tree_elems(nodeptr root) {
      if (root) {
        delete_tree_elems(root->left);
        delete_tree_elems(root->right);
        delete_node_data(root);
        if (root->left)
          root->left = NULL;
        if (root->right)
          root->right = NULL;
        root = NULL;
      }
    }

    void delete_tree() {
      delete_tree_elems(root_);
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

    template<class V, class comp>
    size_t count_equal_key_nodes(const V &data) {
      size_t i = 0;
      for (nodeptr r = GetNode<V, comp>(data); r && !comp()(r->data, data) && !comp()(data, r->data); r = r->right)
        i++;
      return i;
    }

    template<class V, class comp>
    nodeptr lower_bound_node(const V & data) {
      nodeptr i = end_->right;
      for(; comp()(i->data, data); ++i);
      return i;
    }


    template<class V, class comp>
    nodeptr upper_bound_node(const V & data) {
      nodeptr i = end_->right;
      for(; !comp()(data,i->data); ++i);
      return i;
    }

//    value_compare vc;
   public:
    explicit map(const key_compare &comp = key_compare(),
      const allocator_type &alloc = allocator_type()): allocator_(alloc), size_(0) {
      value_comp() = comp;
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
        : allocator_(alloc), vc(comp), size_(0) {
        end_->parent = NULL;
        end_->left = end_;
        end_->right = end_;
      insert(first, last);
    }

    map(const map &x): allocator_(x.allocator_), vc(x.value_comp()), size_(0) {
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
      if (*this != x) {
        clear();
        value_compare() = x.value_compare();
        for(nodeptr i = x.end_->right; i != x.end_; i++)
          insert(x->data);
      }
      return *this;
    }

    iterator begin() { return iterator(end_->right); };
    const_iterator begin() const { return const_iterator(end_->right); };
    iterator end() { return iterator(end_->left); };
    const_iterator end() const { return const_iterator(end_->left); };
    reverse_iterator rbegin() { return iterator(end_->left); };
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end_->left); };
    reverse_iterator rend() { return iterator(end_->right); };
    const_reverse_iterator rend() const { return const_reverse_iterator(end_->right); };
    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    size_type max_size() const { return allocator_.max_size(); }
    mapped_type &operator[](const key_type &k) { return (*((this->insert(make_pair(k,mapped_type()))).first)).second; }
    pair<iterator, bool> insert(const value_type &val) {
      bool t = false;
      make_pair(insert_node(val, GetRoot(end_->right), t), t);
    }
    iterator insert(iterator position, const value_type &val) { (void)position; return iterator(insert(val)); }
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
    void erase(iterator position) { erase(position.ptr_, position.ptr_->right); }

    //todo size_type return!
    size_type erase(const key_type &k) {
      size_t c = 0;
      for (nodeptr p = GetNode<k, key_compare>(k); p; c++)
        delete_node_(p);
      return c;
      //allocator destroy <-
    }
    void erase(iterator first, iterator last) { return erase(first.ptr_, last.ptr_); }
    void swap(map &xx) {
      ft::swap(this->end_, xx.end_);
      ft::swap(this->size_, xx.size_);
      ft::swap(this->value_comp(), xx.value_comp());
    }
    void clear() { delete_tree(); };
    key_compare key_comp() const { return value_comp().key_comp(); }
    value_compare value_comp() const { return value_compare(comp_); }
    iterator find(const key_type &k) { return iterator(get_node_by_key(k)); }
    const_iterator find(const key_type &k) const { return const_iterator(get_node_by_key(k)); }
    size_type count(const key_type &k) const { return count_equal_key_nodes(k); }
    iterator lower_bound(const key_type &k) { return iterator(lower_bound_node<key_type, key_compare>(k)); }
    const_iterator lower_bound(const key_type &k) const { return const_iterator(lower_bound_node<key_type, key_compare>(k)); }
    iterator upper_bound(const key_type &k) { return iterator(upper_bound_node<key_type, key_compare>(k)); }
    const_iterator upper_bound(const key_type &k) const { return const_iterator(upper_bound_node<key_type, key_compare>(k)); }
    pair<const_iterator, const_iterator> equal_range(const key_type &k) const {
      nodeptr right = GetNode<key_type, key_compare>(k);
      if (!right)
        return NULL;
      nodeptr left = right;
      nodeptr tmp;
      while(!key_comp()(left->data, k) && !key_comp()(k, left->data)) {
        tmp = left;
        left = left->left;
      }
      if (!left)
        left = tmp;

      while(!key_comp()(right->data, k) && !key_comp()(k, right->data)) {
        tmp = right;
        right = right->right;
      }
      if (!right)
        right = tmp;
      return make_pair(const_iterator(left), const_iterator(right));
    }
    pair<iterator, iterator> equal_range(const key_type &k) {
      nodeptr right = GetNode<key_type, key_compare>(k);
      if (!right)
        return NULL;
      nodeptr left = right;
      nodeptr tmp;
      while(!key_comp()(left->data, k) && !key_comp()(k, left->data)) {
        tmp = left;
        left = left->left;
      }
      if (!left)
        left = tmp;

      while(!key_comp()(right->data, k) && !key_comp()(k, right->data)) {
        tmp = right;
        right = right->right;
      }
      if (!right)
        right = tmp;
      return make_pair(iterator(left), iterator(right));
    }
    allocator_type get_allocator() const { return allocator_; }
  };
}
#endif

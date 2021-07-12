
#ifndef SRCS_TREE_HPP_
#define SRCS_TREE_HPP_
#include <iomanip>

namespace ft {
  enum Color {
    red,
    black
  };

  template<class T>
  class Node {
   public:
    typedef T value_type;
    value_type data;
    Node *parent;
    Node *&end;
    Color color;
    Node *left;
    Node *right;
    Node *operator++() {
      Node* n = this;
      if (n->right)
        return min(n->right);
      while (!is_left(n))
        n = n->parent;
      return n->parent; // if NULL then n was the most right (max) Node of the tree
    }
    Node *operator--() {
      Node* n = this;
      if (n->left)
        return max(n->left);
      while (is_left(n))
        n = n->parent;
      return n->parent;
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
      while(n && n->left)
        n = n->left;
      return n;
    }
    Node* max() {
      Node* n = this;
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
    Node(const value_type &data, Node* parent = NULL, Color color = red, Node*&end = NULL, Node *prev = NULL, Node *next = NULL)
        : data(data), parent(parent), color(color), end(end), left(prev), right(next) {};
    ~Node() {};
  };

  template<class T, class Compare = ft::less<T>, class Alloc = std::allocator<T> >
  class tree {
   public:
    typedef T value_type;
    typedef Node<value_type> nodeelem;
    typedef Node<value_type> *nodeptr;
    typedef Alloc allocator_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef ft::bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type> * >
        iterator;
    typedef ft::const_bidirectional_iterator<bidirectional_iterator_tag, value_type, std::ptrdiff_t, Node<value_type> * >
        const_iterator;
    typedef ft::reverse_iterator<iterator> reverse_iterator;
    typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef Compare value_compare;

   private:
    nodeptr root;
    size_t size_;
    nodeptr end;
    value_compare compare_;
    typename Alloc::template rebind<Node<value_type> >::other allocator_;

   private:
    nodeptr NewNode() {
      nodeptr new_node = allocator_.allocate(1);
      new_node->left = new_node;
      new_node->right = new_node;
      return new_node;
    }
    nodeptr NewNode(const value_type &Data,
                    nodeptr parent = NULL,
                    Color color = red,
                    nodeptr prev = NULL,
                    nodeptr next = NULL) {
      nodeptr tmp = allocator_.allocate(1);
      allocator_.construct(tmp, nodeelem(Data, parent, color, end, prev, next));
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

    void rotate_left(nodeptr n) {
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

    void rotate_right(nodeptr n) {
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
        root = GetRoot(root);
        root->color = black;
      }
    }


    nodeptr sibling(nodeptr n) {
      if (!n->parent)
        return NULL;
      return IsLeftChild(n) ? n->parent->right : n->parent->left;
    }

    void FixDeleting(nodeptr n) {
      nodeptr s;
      while(n->color == black && n != root) {
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
            n = root;
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
            n = root;
          }
        }
      }
      n->color = black;
    }

    void delete_node_data(nodeptr node) {
      allocator_.destroy(node);
    }

    void delete_tree(nodeptr root_) {
      if (root_) {
        delete_tree(root_->left);
        delete_tree(root_->right);
        delete_node_data(root_);
        if (root_->left)
          root_->left = NULL;
        if (root_->right)
          root_->right = NULL;
        root_ = NULL;
      }
    }

   public:

    tree():root() {}
    explicit tree(const allocator_type& alloc): allocator_(alloc), end(NewNode()) { }
    tree(const value_compare & comp, const allocator_type& alloc = allocator_type()):end(NewNode()), compare_(comp), allocator_(alloc)   {}
    tree & operator=(const tree & x){
      if (this != x) {
        clear();
        value_compare() = x.value_compare();
        for(nodeptr i = x.tree_min(); i != NULL; i++)
          insert(x->data);
        this->size_ = x.size_;
      }
      return *this;
    }

    explicit tree(const tree &x) {
      *this = x;
    }

    size_t size()
    {
      return size_;
    }

    ~tree() {
      clear();
    }

    nodeptr min() {
      nodeptr p = root;
      return p->min();
    }

    nodeptr max() {
      nodeptr p = root;
      return p->max();
    }

    iterator tree_min(nodeptr n) {
      return iterator(n->min());
    }

    iterator tree_min() {
      return iterator(root->min());
    }

    iterator tree_max(nodeptr n) {
      return iterator(n->max());
    }

    iterator tree_max() {
      return iterator(root->max());
    }




//    int GetLevel(nodeptr n) {
//      int i = 0;
//      while (n->parent) {
//        n = n->parent;
//        i++;
//      }
//      return i;
//    }


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

    int GetHeight(nodeptr n) {
      return n ? ft::max(GetHeight(n->left), GetHeight(n->right)) + 1 : 0;
    }




    nodeptr GetRoot(nodeptr n) {
      return n->get_root();
    }

    pair<iterator, bool> insert(const value_type & data) {
      nodeptr kek = max();
      if (kek)
        kek->right = NULL;
      if (!root)
        return (root = NewNode(data, NULL, black));
      else {
        nodeptr cur = root;
        nodeptr tmp = root;
        while ((cur = compare_()(data, cur->data) ? cur->left : cur->right))
          tmp = cur;
        if (!compare_()(data, tmp->data) && !compare_()(tmp->data, data)) {
          kek->right = end;
          return make_pair(iterator(tmp), false);
        }
        cur = NewNode(data, tmp);
        if (compare_()(data, tmp->data))
          tmp->left = cur;
        else
          tmp->right = cur;
        FixInsertion(cur);
        kek = max();
        kek->right = end;
        end->left = kek;
        return make_pair(iterator(cur), true);
      }
    }

    iterator insert(nodeptr pos, const value_type & data) {
      (void)pos;
      return insert(data).first;
    }


    nodeptr GetNode(const value_type & data) {
      nodeptr p = root;
      while (p && p->data != data)
        if (compare_()(data, p->data))
          p = p->left;
        else
          p = p->right;
      return p;
    }

    void del (const value_type & data) {
      nodeptr p = GetNode(data);
      if (!p)
        return;
      nodeptr src = p;
      max()->right = NULL;
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
        root = x;
      if (y != p)
        p->data = y->data;
      if (y->color == black)
        FixDeleting(x);
      DeleteNode(src);
      nodeptr kek = max();
      if (kek)
        kek->right = end;
      end->left = kek;
      //allocator destroy <-
    }




    void clear() {
      delete_tree(root);
    }


  };
}
#endif //SRCS_TREE_HPP_

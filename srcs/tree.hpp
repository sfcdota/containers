
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
    Color color;
    Node *left;
    Node *right;
    Node *operator++() { return this->right; };
    Node *operator--() { return this->left; };
    Node &operator=(Node *in) {
      if (in != this) {
        this->left = in->left;
        this->right = in->right;
        this->data = in->data;
        this->color = in->color;
      }
      return *this;
    };
    value_type &operator()() { return this->data; };
    Node() : left(NULL), right(NULL), parent(NULL), color() {};
    Node(const value_type &data, Node* parent = NULL, Color color = red, Node *prev = NULL, Node *next = NULL)
        : data(data), parent(parent), color(color), left(prev), right(next) {};
    ~Node() {};
  };

  template<class T, class Compare = ft::less<T>, class Alloc = std::allocator<T> >
  class Tree {
   public:
    typedef T value_type;
    typedef ft::less<T> less;
    typedef Node<value_type> nodeelem;
    typedef Node<value_type> *nodeptr;
    typename Alloc::template rebind<Node<value_type> >::other allocator_;

   private:
    nodeptr root;

   public:
    Tree() : root(NULL) {}

    nodeptr NewNode(const value_type &Data,
                    nodeptr parent = NULL,
                    Color color = red,
                    nodeptr prev = NULL,
                    nodeptr next = NULL) {
      nodeptr tmp = allocator_.allocate(1);
      allocator_.construct(tmp, nodeelem(Data, parent, color, prev, next));
      return tmp;
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

    nodeptr tree_min(nodeptr n) {
      while (n && n->left)
        n = n->left;
      return n;
    }

    nodeptr tree_max(nodeptr n) {
      while(n && n->right)
        n = n->right;
      return n;
    }

//    nodeptr next_node(nodeptr n) {
//      if (IsLeftChild(n)) {
//        if (n->right) {
//          return tree_min(n->right);
//        }
//        return n->parent;
//      } else if (n->right)
//        return tree_min(n->right);
//      else {
//        while (n->parent && n->parent->data < n->data)
//          n = n->parent;
//        return n->parent; // if NULL then n was the most right (max) Node of the tree
//      }
//    }

    nodeptr next_node(nodeptr n) {
      if (n->right)
        return tree_min(n->right);
      while (!IsLeftChild(n))
          n = n->parent;
      return n->parent; // if NULL then n was the most right (max) Node of the tree
    }


    nodeptr prev_node(nodeptr n) {
      if (n->left)
        return tree_max(n->left);
      while (IsLeftChild(n))
        n = n->parent;
      return n->parent;
    }



    int GetLevel(nodeptr n) {
      int i = 0;
      while (n->parent) {
        n = n->parent;
        i++;
      }
      return i;
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

    int GetHeight(nodeptr n) {
      return n ? ft::max(GetHeight(n->left), GetHeight(n->right)) + 1 : 0;
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


    nodeptr GetRoot(nodeptr n) {
      if (!n)
        return NULL;
      while (n->parent)
        n = n->parent;
      return n;
    }

    nodeptr insert(const value_type & data = value_type()) {
      if (!root)
        return (root = NewNode(data, NULL, black));
      else {
        nodeptr cur = root;
        nodeptr tmp = root;
        while ((cur = less()(data, cur->data) ? cur->left : cur->right))
          tmp = cur;
        if (!less(data, tmp->data) && !less(tmp->data, data)) {
          return tmp;
        }
        cur = NewNode(data, tmp);
        if (less()(data, cur->data))
          tmp->left = cur;
        else
          tmp->right = cur;
        FixInsertion(cur);
        return cur;
      }
    }

    nodeptr sibling(nodeptr n) {
      if (!n->parent)
        return NULL;
      return IsLeftChild(n) ? n->parent->right : n->parent->left;
    }

    void replace_node (nodeptr n, nodeptr child) {
      child->parent = n->parent;
      if (IsLeftChild(n))
        n->parent->left = child;
      else
        n->parent->right = child;
    }

    bool is_leaf(nodeptr n) {
      return !n || (!n->left && !n->right);
    }

    void delete_one_child (nodeptr n) {
      nodeptr child = is_leaf(n->right) ? n->left : n->right;
      replace_node(n, child);
      if (n->color == black) {
        if (child->color == red)
          child->color = black;
        else
          delete_node(child);
      }
      allocator_.destroy(n);
      n = NULL;
    }

    void delete_node(nodeptr n) {
      if(!n->parent)
        return;
      nodeptr s;
      while (n->parent) {
        s = sibling(n);
        if (s->color == red) {
          s->parent->color = red;
          s->color = black;
          if (IsLeftChild(n))
            rotate_left(n->parent);
          else
            rotate_right(n->parent);
        }

        if (n->parent->color == black &&
            s->color == black &&
            s->left->color == black &&
            s->right->color == black) {
          s->color = red;
          n = n->parent;
        } else
          break;
      }
      if (n->parent->color == red &&
          s->color == black &&
          s->left->color == black &&
          s->right->color == black) {
        s->color = red;
        n->parent->color = black;
      } else if (s->color == black) {
        if (IsLeftChild(n) &&
            s->left->color == red &&
            s->right->color == black) {
          s->color = red;
          s->left->color = black;
          rotate_right(s);
        } else if (!IsLeftChild(n) &&
            s->left->color == black &&
            s->right->color == red) {
          s->color = red;
          s->right->color = black;
          rotate_left(s);
        }
      }
      s->color = n->parent->color;
      n->parent->color = black;
      if (IsLeftChild(n)) {
        s->right->color = black;
        rotate_left(n->parent);
      } else {
        s->left->color = black;
        rotate_right(n->parent);
      }
    }


    nodeptr GetNode(value_type & data) {
      nodeptr p = root;
      while (p && p->data != data)
        if (less()(data, p->data))
          p = p->left;
        else
          p = p->right;
      return p;
    }

    void del (value_type & data) {
      nodeptr p = root;
      while (p->data != data)
        if (data < p->data)
          p = p->left;
        else
          p = p->right;
      if (!p->left && !p->right) {
        if (p == root)
          root = NULL;
        else {
          if (IsLeftChild(p))
            p->parent->left = NULL;
          else
            p->parent->right = NULL;
        }
        allocator_.destroy(p);
        return;
      }
      nodeptr y = NULL, q = NULL;
      if (p->left && !p->right) {//only one child
        p->left->parent = p->parent;
        if (IsLeftChild(p))
          p->parent->left = p->left;
        else
          p->parent->right = p->left;
      }
      else if (!p->left && p->right) {
        p->right->parent = p->parent;
        if (IsLeftChild(p))
          p->parent->left = p->right;
        else
          p->parent->right = p->right;
      }
      else {
        nodeptr next = next_node(p);
        if (next->right)
          next->right->parent = next->parent;
        if (next == root)
          root = next->right;
        else {
          if (IsLeftChild(next))
            next->parent->left = next->right;
          else
            next->parent->right = next->right;
        }
        if (next != p) {
          p->color = next->color;
          p->data = next->data;
        }
        if (next->color == black)
          FixDeleting();
      }
    }

    void FixDeleting(nodeptr n) {
      nodeptr s;
      while(n->color == black && n != root) {
        s = sibling(n);
        if (IsLeftChild(n)) {
          if (s->color == red) {
            s->color = black;
            s->parent = red;
            rotate_left(s->parent);
          }
          if (s->left && s->left->color == black &&
              s->right && s->right->color == black) {
            s->color = red;
          }
          else {
            if (s->right->color == black) {
              s->left->color = black;
              s->color = red;
              rotate_right(s);
            }
            s->color = s->parent->color;
            s->parent = black;
            s->right = black;
            rotate_left(s->parent);
            n = root;
          }
        }
        else {
          if (s->color == red) {
            s->color = black;
            s->parent->color = red;
            rotate_right(n->parent);
          }
          if (s->left && s->left->color == black &&
              s->right && s->right->color == black) {
            s->color = red;
          }
          else {
            if (s->left->color == black) {
              s->right->color = black;
              s->color = red;
              rotate_left(s);
            }
            s = n->parent;
            n->parent->color = black;
            s->left = black;
            rotate_right(n->parent);
            n = root;
          }
        }
      }
      n->color = black;
      root->color = black;
    }
  };
}
#endif //SRCS_TREE_HPP_

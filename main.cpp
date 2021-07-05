#include <iostream>
#include "srcs/iterator.hpp"
#include "srcs/list.hpp"
#include <list>
#include <vector>
#include "srcs/vector.hpp"
#include "srcs/tree.hpp"
#include <map>
using namespace ft;

// comparison, not case sensitive.


int main ()
{
//  srand(time(NULL));
  srand(1337);
  std::map
  Tree<int> tree;
  for(int i = 0; i <5; i++) {
    int elem = rand() % 50;
    tree.insert(elem);
    std::cout << "new elem = " << elem << std::endl;
  }
  std::cout << "Node height = " << tree.GetHeight(tree.root) << std::endl;
//  std::cout << "Next same level" << tree.NextNodeSameLevel(tree.root->left)->data << std::endl;
  tree.printBT(tree.root);
  tree.delete_one_child(tree.root);
  tree.printBT(tree.root);
  return 0;
}
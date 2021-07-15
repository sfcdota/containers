#include <iostream>
#if STANDARD //CREATE A REAL STL EXAMPLE
#include <stack>
#define version "standard STL version"
namespace ft = std;
#else
#include "../stack.hpp"
#define version "ft STL version"
#endif

void stack_constructor() {
  std::deque<int> mydeque (3,100); // deque with 3 elements
  std::vector<int > myvector (2,200); // vector with 2 elements

  ft::stack<int, std::deque<int> > first; // empty stack
  ft::stack<int, std::deque<int> > second (mydeque); // stack initialized to copy of deque

  ft::stack<int,std::vector<int> > third; // empty stack using vector
  ft::stack<int,std::vector<int> > fourth (myvector);

  std::cout << "size of first: " << first.size() << '\n';
  std::cout << "size of second: " << second.size() << '\n';
  std::cout << "size of third: " << third.size() << '\n';
  std::cout << "size of fourth: " << fourth.size() << '\n';
}

void stack_empty() {
  ft::stack<int> mystack;
  int sum (0);

  for (int i=1;i<=10;i++) mystack.push(i);

  while (!mystack.empty())
  {
    sum += mystack.top();
    mystack.pop();
  }

  std::cout << "total: " << sum << '\n';
}

void stack_pop() {
  ft::stack<int> mystack;

  for (int i=0; i<5; ++i) mystack.push(i);

  std::cout << "Popping out elements...";
  while (!mystack.empty())
  {
    std::cout << ' ' << mystack.top();
    mystack.pop();
  }
  std::cout << '\n';
}

void stack_push() {
  ft::stack<int> mystack;

  for (int i=0; i<5; ++i) mystack.push(i);

  std::cout << "Popping out elements...";
  while (!mystack.empty())
  {
    std::cout << ' ' << mystack.top();
    mystack.pop();
  }
  std::cout << '\n';
}

void stack_size() {
  ft::stack<int> myints;
  std::cout << "0. size: " << myints.size() << '\n';

  for (int i=0; i<5; i++) myints.push(i);
  std::cout << "1. size: " << myints.size() << '\n';

  myints.pop();
  std::cout << "2. size: " << myints.size() << '\n';
}

void stack_top() {
  ft::stack<int> mystack;

  mystack.push(10);
  mystack.push(20);

  mystack.top() -= 5;

  std::cout << "mystack.top() is now " << mystack.top() << '\n';
}

int main() {
  stack_constructor();
  stack_empty();
  stack_pop();
  stack_push();
  stack_size();
  stack_top();
}
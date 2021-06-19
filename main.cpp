#include <iostream>
#include "srcs/iterator.hpp"
#include "srcs/list.hpp"
#include <list>
#include <vector>
#include "srcs/vector.hpp"

using namespace ft;

// comparison, not case sensitive.


int main ()
{
  ft::vector<int> myvector (3,100);
  ft::vector<int>::iterator it;

  it = myvector.begin();
  it = myvector.insert ( it , 200 );
  myvector.print();
  myvector.insert (it,2,300);
  myvector.print();

//  // "it" no longer valid, get a new one:
//  it = myvector.begin();
//
//  ft::vector<int> anothervector (2,400);
//  myvector.insert (it+2,anothervector.begin(),anothervector.end());
//
//  int myarray [] = { 501,502,503 };
//  myvector.insert (myvector.begin(), myarray, myarray+3);
//
//  std::cout << "myvector contains:";
//  for (it=myvector.begin(); it<myvector.end(); it++)
//    std::cout << ' ' << *it;
//  std::cout << '\n';

  return 0;

  return 0;
}
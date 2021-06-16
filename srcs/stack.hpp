#ifndef STACK_HPP
#define STACK_HPP
template <class T, class Container = deque<T>>
class Stack {
  typename value_type T;
  typename container_type Container;
  typename size_type size_t;
 public:
  explicit Stack(const container_type& ctnr = container_type());
  bool empty() const;
  size_type size() const;
  value_type& top();
  const value_type& top() const;
  void push(const value_type& val);
  void pop();
 private:

};
#endif

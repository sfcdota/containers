//
// Created by sfcdo on 01.05.2021.
//

#ifndef QUEUE_HPP
#define QUEUE_HPP

template <class T, class Container = deque<T>> class Queue {
  typedef value_type T;
  typedef container_type Container;
  typedef size_type size_t;
 public:
  explicit Queue(const container_type& ctnr = container_type());
  bool empty() const;
  size_type size() const;
  value_type& front();
  const value_type& front() const;
  value_type& back();
  const value_type& back() const;
  void push(const value_type& val);
  void pop();
 private:

};
#endif

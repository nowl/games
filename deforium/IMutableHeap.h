#ifndef __MUTABLE_HEAP_H__
#define __MUTABLE_HEAP_H__

#include <ostream>

template<class T, class Container, class Compare, class Equal>
class IMutableHeap {
public:
  virtual ~IMutableHeap() {};

  virtual bool empty() const = 0;
  virtual T pop() = 0;
  virtual void push(const T& value) = 0;
  virtual void remove(const T& value) = 0;  
  virtual bool find(const T& value, T& result) = 0;

public:
  // ugly friending code
  template<class U, class V, class W, class X>
  friend std::ostream& 
  operator<<(std::ostream& out, const IMutableHeap<U,V,W,X> &heap);
};

#endif	// __MUTABLE_HEAP_H__

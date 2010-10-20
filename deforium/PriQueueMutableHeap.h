#ifndef __PRI_QUEUE_MUTABLE_HEAP_H__
#define __PRI_QUEUE_MUTABLE_HEAP_H__

#include <boost/shared_ptr.hpp>
#include <queue>

#include "IMutableHeap.h"

#include <vector>
#include <ostream>

template<class T, 
	 class Container=std::vector<T>, 
	 class Compare=std::less<T>,
	 class Equal=std::less<T> >
class PriQueueMutableHeap : public IMutableHeap<T, Container, Compare, Equal> {

private:
  struct HackedPriorityQueue : public std::priority_queue<T, Container, Compare> {
    Container& getUnderlyingContainer() {
      return this->c;
    }

    const Container& getUnderlyingContainer() const {
      return this->c;
    }
  };

public:
  PriQueueMutableHeap() {}
  virtual ~PriQueueMutableHeap() {}
  
  virtual bool empty() const {
    return _queue.empty();
  }

  virtual T pop() {
    T result = _queue.top();
    _queue.pop();
    return result;
  }

  virtual void push(const T& value) {
    _queue.push(value);
  }

  virtual void remove(const T& value) {
    Container &c = _queue.getUnderlyingContainer();
    typename Container::iterator iter = c.begin();
    for(; iter != c.end(); iter++)
      if(_equal(*iter, value)) {
	c.erase(iter);
	break;
      }
  }
  
  virtual bool find(const T& value, T& result) {
    bool found = false;
    Container &c = _queue.getUnderlyingContainer();
    typename Container::iterator iter = c.begin();
    for(; iter != c.end(); iter++)
      if(_equal(*iter, value)) {
	result = *iter;
	found = true;
	break;
      }
    
    return found;
  }
 
private:
  HackedPriorityQueue _queue;
  Equal _equal;

  //Compare _comparison;
};

#endif	// __PRI_QUEUE_MUTABLE_HEAP_H__

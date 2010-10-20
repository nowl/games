#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <vector>

class Random {
public:
  typedef unsigned long int u4;

  static void init();
  static void init(u4 seed);
  static u4 val();
  static float floatVal();

  /* Returns a random integer equal to or above min and less than max.
   */
  static int intValMinMax(int min, int max);
  
  template< class T >
  static const T& choice(const std::vector<T> choices) {
    int number = choices.size();
    int choice = intValMinMax(0, number);
    return choices[choice];
  }
 
private:
  struct ranctx { u4 a; u4 b; u4 c; u4 d; };
  static ranctx mRanCtx;

  static inline u4 rot(u4 x, u4 k) {
    return (x<<k) | (x>>(32-k));
  }  
};

#endif	// __RANDOM_H__

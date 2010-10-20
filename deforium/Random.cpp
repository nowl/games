#include <ctime>
#include <climits>

#include "Random.h"

using namespace std;

Random::ranctx Random::mRanCtx;

void Random::init() {
  init(time(NULL));
}

void Random::init(u4 seed) {
  u4 i;
  mRanCtx.a = 0xf1ea5eed, mRanCtx.b = mRanCtx.c = mRanCtx.d = seed;
  for (i=0; i<20; ++i) {
    val();
  }
}

Random::u4 Random::val() {
  u4 e = mRanCtx.a - rot(mRanCtx.b, 27);
  mRanCtx.a = mRanCtx.b ^ rot(mRanCtx.c, 17);
  mRanCtx.b = mRanCtx.c + mRanCtx.d;
  mRanCtx.c = mRanCtx.d + e;
  mRanCtx.d = e + mRanCtx.a;
  return mRanCtx.d;

}

float Random::floatVal() {
  u4 v = val();
  return static_cast<float>(v)/ULONG_MAX;
}

int Random::intValMinMax(int min, int max) {
  int span = max - min + 1;
  u4 v = val() % span;
  return v + min;
}

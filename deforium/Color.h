#ifndef __COLOR_H__
#define __COLOR_H__

class Color {
public:
  Color() : mR(0), mG(0), mB(0) {};
  Color(double r, double g, double b)
    : mR(r), mG(g), mB(b), mAlpha(0), mLimit(0) {};
  Color(double r, double g, double b, double alpha)
    : mR(r), mG(g), mB(b), mAlpha(alpha), mLimit(0) {};
  Color(int r, int g, int b, int alpha, unsigned int limit) 
    : mR((double)r/limit), mG((double)g/limit), 
      mB((double)b/limit), mAlpha((double)alpha/limit), 
      mLimit(limit) {};
  
  double getRed() const { return mR; }
  double getGreen() const { return mG; }
  double getBlue() const { return mB; }
  double getAlpha() const { return mAlpha; }

  int getRedAsInt() const { return mR*mLimit; }
  int getGreenAsInt() const { return mG*mLimit; }
  int getBlueAsInt() const { return mB*mLimit; }
  int getAlphaAsInt() const { return mAlpha*mLimit; }
  
private:
  double mR, mG, mB, mAlpha;
  unsigned int mLimit;
};

#endif	// __COLOR_H__

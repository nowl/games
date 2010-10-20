#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Vector.h"

template<class T, int M, int N>
class Matrix2D {};

template<class T>
class Matrix2D<T, 2, 4> {
public:
	Matrix2D(const T& n11,
			 const T& n21,
			 const T& n12,
			 const T& n22,
			 const T& n13,
			 const T& n23,
			 const T& n14,
			 const T& n24)
	{
		_vals[0] = n11;
		_vals[1] = n21;
		_vals[2] = n12;
		_vals[3] = n22;
		_vals[4] = n13;
		_vals[5] = n23;
		_vals[6] = n14;
		_vals[7] = n24;
	}

	Vector<T, 2> operator*(const Vector<T, 4>& vect) const {
		return Vector<T, 2>(_vals[0]*vect[0] + _vals[2]*vect[1] +
							_vals[4]*vect[2] + _vals[6]*vect[3],
							_vals[1]*vect[0] + _vals[3]*vect[1] +
							_vals[5]*vect[2] + _vals[7]*vect[3]);
							   
	}

	T& operator[](int n) { return _vals[n]; }
	const T& operator[](int n) const { return _vals[n]; }

private:
	T _vals[8];
};

typedef Matrix2D<float, 2, 4> Matrix2Df24;

#endif	// __MATRIX_H__

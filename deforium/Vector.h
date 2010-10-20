#ifndef __VECTOR_H__
#define __VECTOR_H__

template<class T, int N>
class Vector;

template<class T>
class Vector<T, 4> {
public:
	Vector(const T& n1, const T& n2, const T& n3, const T& n4)
	{
		_vals[0] = n1;
		_vals[1] = n2;
		_vals[2] = n3;
		_vals[3] = n4;
	}

	T& operator[](int n) { return _vals[n]; }
	const T& operator[](int n) const { return _vals[n]; }

	Vector operator+(const Vector<T,4>& vect) const {
		return Vector(_vals[0] + vect[0],
					  _vals[1] + vect[1],
					  _vals[2] + vect[2],
					  _vals[3] + vect[3]);
	}
	Vector operator-(const Vector<T,4>& vect) const {
		return Vector(_vals[0] - vect[0],
					  _vals[1] - vect[1],
					  _vals[2] - vect[2],
					  _vals[3] - vect[3]);
	}
	Vector operator*(const T& val) const {
		return Vector(_vals[0] * val,
					  _vals[1] * val,
					  _vals[2] * val,
					  _vals[3] * val);
	}
	Vector operator/(const T& val) const {
		return Vector(_vals[0] / val,
					  _vals[1] / val,
					  _vals[2] / val,
					  _vals[3] / val);
	}
	const Vector& operator+=(const Vector<T,4>& vect) {
		_vals[0] += vect[0];
		_vals[1] += vect[1];
		_vals[2] += vect[2];
		_vals[3] += vect[3];
		
		return *this;
	}
	const Vector& operator-=(const Vector<T,4>& vect) {
		_vals[0] -= vect[0];
		_vals[1] -= vect[1];
		_vals[2] -= vect[2];
		_vals[3] -= vect[3];
		
		return *this;
	}
	const Vector& operator*=(const T& val) {
		_vals[0] *= val;
		_vals[1] *= val;
		_vals[2] *= val;
		_vals[3] *= val;
		
		return *this;
	}
	const Vector& operator/=(const T& val) {
		_vals[0] /= val;
		_vals[1] /= val;
		_vals[2] /= val;
		_vals[3] /= val;
		
		return *this;
	}

	
private:
	T _vals[4];
};

template<class T>
class Vector<T, 2> {
public:
	Vector() { _vals[0] = 0; _vals[1] = 0; }
	
	Vector(const T& n1, const T& n2)
	{
		_vals[0] = n1;
		_vals[1] = n2;
	}

	virtual ~Vector() {}

	const T operator[](int n) const { return _vals[n]; }
	bool operator==(const Vector<T,2>& vect) const {
		return _vals[0] == vect[0] && _vals[1] == vect[1];
	}
    bool operator!=(const Vector<T,2>& vect) const {
		return _vals[0] != vect[0] || _vals[1] != vect[1];
	}
	Vector operator+(const Vector<T,2>& vect) const {
		return Vector(_vals[0] + vect[0],
					  _vals[1] + vect[1]);
	}
	Vector operator-(const Vector<T,2>& vect) const {
		return Vector(_vals[0] - vect[0],
					  _vals[1] - vect[1]);
	}
	Vector operator*(const T& val) const {
		return Vector(_vals[0] * val,
					  _vals[1] * val);
	}
	Vector operator/(const T& val) const {
		return Vector(_vals[0] / val,
					  _vals[1] / val);
	}
	const Vector& operator+=(const Vector<T,2>& vect) {
		_vals[0] += vect[0];
		_vals[1] += vect[1];
		
		return *this;
	}
	const Vector& operator-=(const Vector<T,2>& vect) {
		_vals[0] -= vect[0];
		_vals[1] -= vect[1];
		
		return *this;
	}
	const Vector& operator*=(const T& val) {
		_vals[0] *= val;
		_vals[1] *= val;
		
		return *this;
	}
	const Vector& operator/=(const T& val) {
		_vals[0] /= val;
		_vals[1] /= val;
		
		return *this;
	}

	
private:
	T _vals[2];
};
	
typedef Vector<float, 4> Vector4f;
typedef Vector<float, 2> Vector2f;

#endif	// __VECTOR_H__

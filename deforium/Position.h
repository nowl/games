#ifndef __POSITION_H__
#define __POSITION_H__

#include <boost/functional/hash.hpp>

#include "Vector.h"

struct Position : public Vector2f {
	Position() : Vector2f() {}
	Position(float x, float y) : Vector2f(x, y) {}
	Position(const Vector2f& vect) : Vector2f(vect) {}

	float getX() const { return (*this)[0]; }
	float getY() const { return (*this)[1]; }
};

struct position_hash : std::unary_function<Position, std::size_t>
{
	std::size_t operator()(const Position& p) const
    {
		std::size_t seed = 0;
		boost::hash_combine(seed, p.getX());
		boost::hash_combine(seed, p.getY());
        return seed;
    }
};


#endif	// __POSITION_H__

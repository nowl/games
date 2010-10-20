#ifndef __OBJECT_TRANSFORM_H__
#define __OBJECT_TRANSFORM_H__

#include "Matrix.h"

/** Object transform methods. The purpose of this class is to provide
 * transformation methods to move a position from the world to the
 * screen and visa-versa.
 */
class ObjectTransform {
public:
	ObjectTransform(const Matrix2Df24& transformWorldToScreen,
					const Matrix2Df24& transformScreenToWorld)
		: _transformWtoS(transformWorldToScreen),
		  _transformStoW(transformScreenToWorld)		  
	{}

	virtual ~ObjectTransform() {}
	
	/** This will transform a point from world to screen coordinates.
	 */
	Vector2f transformWorldToScreen(const Vector2f& pos) const {
		return _transformWtoS * Vector4f(pos[0], pos[1], 1, 1);
	}

	/** This will transform a point from screen to world coordinates.
	 */
	Vector2f transformScreenToWorld(const Vector2f& pos) const {
		return _transformStoW * Vector4f(pos[0], pos[1], 1, 1);
	}

    Matrix2Df24 getWorldTransform() const {
        return _transformWtoS;
    }
	
private:
	Matrix2Df24 _transformWtoS, _transformStoW;
};

/** Simple Independent Scale and/or Translate Object Transform. If the
 * transform is an independent scale and or transform only then this
 * will determine the inverse automatically.
 */
class SSTObjectTransform : public ObjectTransform {
public:
	SSTObjectTransform(const Matrix2Df24& transformWorldToScreen)
		: ObjectTransform(transformWorldToScreen,
						  Matrix2Df24(1/transformWorldToScreen[0],
									  0,
									  0,
									  1/transformWorldToScreen[3],
									  -transformWorldToScreen[4] / 
									  transformWorldToScreen[0],
									  0,
									  0,
									  -transformWorldToScreen[7] / 
									  transformWorldToScreen[3]))
	{}
};

#endif	// __OBJECT_TRANSFORM_H__

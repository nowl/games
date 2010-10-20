#ifndef __RENDERABLE_H__
#define __RENDERABLE_H__

#include <boost/foreach.hpp>

#include "IRenderable.h"
#include "Childable.h"

class GameState;

/** Abstract class that is both IRenderable and Childable. Classes
 * derived from this class should override the render method but are
 * able to contain children of the given derived type.
 */
template<class T>
class Renderable : public IRenderable,
				   virtual public Childable<T>
{
public:
    virtual ~Renderable() {}

	/** Default render method calls render on each child of this
	 * object. */
	virtual void render(GraphicsEngine *graphicsEngine, 
                        GameState *gameState,
                        double interpolation);
};



template<class T>
void Renderable<T>::render(GraphicsEngine *graphicsEngine, 
						   GameState *gameState,
						   double interpolation)
{
	// For each child of this node call the render method on each
	// one.
	BOOST_FOREACH(T* child, Childable<T>::getChildren()) {
		child->render(graphicsEngine, gameState, interpolation);
	}
}

#endif  // __RENDERABLE_H__

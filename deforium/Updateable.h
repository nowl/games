#ifndef __UPDATEABLE_H__
#define __UPDATEABLE_H__

#include <boost/foreach.hpp>

#include "IUpdateable.h"
#include "Childable.h"

/** Abstract class that is both IUpdateable and Childable. Classes
 * derived from this class should override the update method but are
 * able to contain children of the given derived type.
 */
template<class T>
class Updateable : public IUpdateable,
				   virtual public Childable<T>
{
public:
    virtual ~Updateable() {}

	/** Default update method calls update on each child of this
	 * object. */
	virtual void update(GameState *gameState);
};



template<class T>
void Updateable<T>::update(GameState *gameState)
{
	// For each child of this node call the update method on each one.
	BOOST_FOREACH(T* child, Childable<T>::getChildren()) {
		child->update(gameState);
	}
}

#endif  // __UPDATEABLE_H__

#ifndef __IRENDERABLE_H__
#define __IRENDERABLE_H__

#include "GraphicsEngine.h"
#include "GameState.h"

/** Interface defining a renderable type. A class that should be
 * rendered to the screen should derive from this class and override
 * the virtual render method.
 */
class IRenderable {
public:
    virtual ~IRenderable() {}

	/** Renders an object to the screen. Uses graphicsEngine to render
	 * the derived object to the screen. Access is given to gameState
	 * in order to access other objects if needed.
	 * @param graphicsEngine a pointer to the underlying graphics engine
	 * @param gameState a pointer to the global gamestate
	 * @param interpolation a double between 0 and 1 representing proximity to next tick
	 */	 
    virtual void render(GraphicsEngine *graphicsEngine, 
                        GameState *gameState,
                        double interpolation) = 0;
};

#endif  // __IRENDERABLE_H__

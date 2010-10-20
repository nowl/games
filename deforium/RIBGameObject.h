#ifndef __RIB_GAME_OBJECT_H__
#define __RIB_GAME_OBJECT_H__

#include <vector>

#include "GameObject.h"
#include "GraphicsAsset.h"

/* Rotating Image Backed Game Object. This object is meant to have a
 * cycling group of images that can be drawn to a location on the
 * screen.
 */
class RIBGameObject : public GameObject {
public:
	/** Constructor.
	 * @param name the name of the object
	 * @param layer the render layer
	 * @param position the initial world position of this object
	 * @param imageResourceNames the resource names of the images
	 * @param waitTime number of ticks to wait to change image
	 */
    RIBGameObject(const std::string& name,
				  const int layer,
				  const Position& position,
				  const std::vector<std::string>& imageResourceNames,
				  unsigned int waitTime);
    
    virtual ~RIBGameObject() {};

	virtual void specificRender(GraphicsEngine *graphicsEngine, 
								GameState *gameState,
								double interpolation);
	
	virtual void specificUpdate(GameState *gameState);

private:
	std::vector<GraphicsAsset*> _imageAssets;
	unsigned int _waitTime;
	unsigned int _currentTime;
	unsigned int _currentObject;
};

#endif	// __RIB_GAME_OBJECT_H__

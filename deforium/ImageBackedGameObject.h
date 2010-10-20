#ifndef __IMAGE_BACKED_GAME_OBJECT_H__
#define __IMAGE_BACKED_GAME_OBJECT_H__

#include <vector>

#include "GameObject.h"
#include "GraphicsAsset.h"

/** A GameObject that contains a renderable image object. The image
 * will be rendered to the world position passed to the constructor
 * (or set with setWorldPosition).
 */
class ImageBackedGameObject : public GameObject {
public:
	/** Constructor.
	 * @param name the name of the object
	 * @param layer the render layer
	 * @param position the initial world position of this object
	 * @param imageResourceName the GlobalGraphicsCache image name
	 */
    ImageBackedGameObject(const std::string& name,
						  const int layer,
						  const Position& position,
						  const std::string& imageResourceName);

	virtual ~ImageBackedGameObject();
	
    virtual void specificRender(GraphicsEngine *graphicsEngine,
								GameState *gameState,
								double interpolation);
private:
    Log *_log;
	GraphicsAsset* _imageAsset;
};

#endif  // __IMAGE_BACKED_GAME_OBJECT_H__

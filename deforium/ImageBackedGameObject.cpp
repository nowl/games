#include <iostream>

#include "GraphicsCache.h"
#include "ImageBackedGameObject.h"

using namespace std;
using namespace boost;

ImageBackedGameObject::
ImageBackedGameObject(const std::string& name,
					  const int layer,
					  const Position& position,
					  const std::string& imageResourceName)
	: GameObject(name, layer),
      _log(Log::getLogger("ImageBackedGameObject")),
	  _imageAsset(GlobalGraphicsCache.get(imageResourceName,
										  GraphicsCache::IMAGE_GRAPHICS_ASSET))
{
	setWorldPosition(position);
}

ImageBackedGameObject::~ImageBackedGameObject() {}

void
ImageBackedGameObject::specificRender(GraphicsEngine *graphicsEngine,
									  GameState *gameState,
									  double interpolation)
{
	Position pos = getScreenPosition();

    graphicsEngine->drawImage(_imageAsset->getContents(), 
							  pos.getX(),
							  pos.getY());
}

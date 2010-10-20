#include <boost/foreach.hpp>

#include "GraphicsCache.h"
#include "RIBGameObject.h"

using namespace std;
using namespace boost;

RIBGameObject::RIBGameObject(const string& name,
							 const int layer,
							 const Position& position,
							 const vector<string>& imageResourceNames,
							 unsigned int waitTime)
	: GameObject(name, layer),
	  _waitTime(waitTime),
	  _currentTime(0),
	  _currentObject(0)
{
	setWorldPosition(position);

	BOOST_FOREACH(string imageName, imageResourceNames)
		_imageAssets.push_back(GlobalGraphicsCache.get(imageName,
													   GraphicsCache::IMAGE_GRAPHICS_ASSET));
}

void
RIBGameObject::specificRender(GraphicsEngine *graphicsEngine,
							  GameState *gameState,
							  double interpolation)
{
	Position pos = getScreenPosition();

    graphicsEngine->drawImage(_imageAssets[_currentObject]->getContents(), 
							  pos.getX(),
							  pos.getY());
}


void
RIBGameObject::specificUpdate(GameState *gameState)
{
	if(++_currentTime == _waitTime) {
		_currentObject  = (_currentObject + 1) % _imageAssets.size();
        _currentTime = 0;
	}

	if(_currentObject >= _imageAssets.size())
		_currentObject = 0;
}

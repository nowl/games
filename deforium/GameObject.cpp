#include <boost/foreach.hpp>

#include "GameObject.h"

using namespace std;
using namespace boost;

GameObject::GameObject(const std::string& name, const int layer)
    : _unitSize(32),
      _log(Log::getLogger("GameObject")),
      _name(name),
      _layer(layer),
      _visible(true),
      _objectTransform(SSTObjectTransform(Matrix2Df24(1, 0, 0, 1, 0, 0, 0, 0))),
      _screenPositionStale(true),
      _clipWidth(0)
{}

GameObject::~GameObject() {}

const ObjectTransform&
GameObject::getObjectTransform()
{
	return _objectTransform;
}

void 
GameObject::setObjectTransform(const ObjectTransform& transform)
{
	_objectTransform = transform;
	
	// changing this will modify the screen position, set delayed
	// update of this
	_screenPositionStale = true;
	
	// This should propogate to all child GameObjects
	BOOST_FOREACH(GameObject* child, getChildren())
		child->setObjectTransform(transform);
}

string
GameObject::getName() const
{ 
	return _name;
}
	
void
GameObject::render(GraphicsEngine *graphicsEngine, 
				   GameState *gameState,
				   double interpolation)
{
	// render if the layer is the one being rendered and the object is
	// currently visible
	if(_layer == gameState->renderLayer && getVisible()) {
        if(_clipWidth != 0)
            graphicsEngine->setClipRect(_clipULX, _clipULY, _clipWidth, _clipHeight);

		specificRender(graphicsEngine, gameState, interpolation);
    }
	
	// render all children
	Renderable<GameObject>::render(graphicsEngine, gameState, interpolation);
}

void
GameObject::update(GameState *gameState)
{
	specificUpdate(gameState);

	Updateable<GameObject>::update(gameState);
}

const Position&
GameObject::getWorldPosition()
{
	return _worldPosition;
}

void
GameObject::setWorldPosition(const Position& position)
{
	_worldPosition = position;
	
	// set delayed update of the screen position
	_screenPositionStale = true;
}

Position
GameObject::getScreenPosition()
{
	if(_screenPositionStale) {
		_screenPosition = _objectTransform.transformWorldToScreen(_worldPosition);
		_screenPositionStale = false;
	}

	return _screenPosition;
}

void
GameObject::specificRender(GraphicsEngine *graphicsEngine, 
						   GameState *gameState,
						   double interpolation)
{}

void
GameObject::specificUpdate(GameState *gameState)
{}

void
GameObject::setVisible(bool value)
{
	_visible = value;

	// propagate this to all children
	BOOST_FOREACH(GameObject* child, getChildren())
		child->setVisible(value);
}

bool
GameObject::getVisible() const
{
	return _visible;
}

void
GameObject::addChild(GameObject *child)
{
	child->setObjectTransform(_objectTransform);
	
	Childable<GameObject>::addChild(child);
}

void
GameObject::setClipping(const Extents& extents)
{
    _clipULX = extents.getMinX();
    _clipULY = extents.getMinY();
    _clipWidth = extents.getWidth();
    _clipHeight = extents.getHeight();
}

int
GameObject::getLayer()
{
	return _layer;
}

Extents
GameObject::getBBox()
{
    return Extents(getWorldPosition().getX(),
                   getWorldPosition().getY(),
                   _unitSize,
                   _unitSize);
}

void
GameObject::collideWithObject(GameState *gameState,
                              GameObject *gameObject)
{}

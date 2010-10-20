#include <boost/bind.hpp>
#include <boost/signal.hpp>

#include "MUGameObject.h"
#include "ObjectTransform.h"

using namespace boost;

MUGameObject::MUGameObject(GameObject *go,
						   Map* map,
						   MapView* mapView)
	: GameObject(go->getName() + " delegate", -1),
	  _go(go),
	  _map(map),
	  _mapView(mapView)
{
	addChild(go);

	// hook up slots
	function<void (const Position&)> func;
	func = boost::bind(&MUGameObject::mapChange, this, _1);
	signal<void (const Position&)>& sig = _map->getChangeSignal();
	_mapConnection = sig.connect(func);

    function<void (const Extents&)> func2;
    func2 = bind(&MUGameObject::mapViewChange, this, _1);
    signal<void (const Extents&)>& sig2 = _mapView->getChangeSignal();
    _mapViewConnection = sig2.connect(func2);

}
	
MUGameObject::~MUGameObject() { 
	_map->getChangeSignal().disconnect(_mapConnection);
    _mapView->getChangeSignal().disconnect(_mapViewConnection);
	
	delete _go;
}

void
MUGameObject::mapChange(const Position& position)
{
	// called when the underlying map changes, position will be the
	// location of the change
	// TODO: finish this
}

void
MUGameObject::mapViewChange(const Extents& extents)
{
	// called when the map view changes
	// TODO: finish this

	float x = extents.getMinX();
	float y = extents.getMinY();
	setObjectTransform(SSTObjectTransform(Matrix2Df24(1, 0, 0, 1, -x, 0, 0, -y)));
}

void
MUGameObject::collideWithObject(GameState *gameState,
                                GameObject *gameObject)
{
    getChildren()[0]->collideWithObject(gameState, gameObject);
}

Extents
MUGameObject::getBBox()
{
    return getChildren()[0]->getBBox();
}

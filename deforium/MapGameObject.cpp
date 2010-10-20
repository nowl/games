#include <boost/bind.hpp>

#include "MapGameObject.h"
#include "ImageBackedGameObject.h"
#include "ImpassableTerrain.h"

using namespace std;
using namespace boost;

MapGameObject::MapGameObject(const std::string& name,
                             int layer,
                             const Position& position,
                             int width,
                             int height,
                             Map *map,
                             MapView *mapView,
							 int unitSize)
    : GameObject(name, layer),
      _log(Log::getLogger("MapGameObject")),
      _map(map),
      _mapView(mapView),
      _scrollingLeft(false),
      _scrollingRight(false),
      _scrollingUp(false),
      _scrollingDown(false),
      _scrollsRemainingRight(0),
      _scrollsRemainingLeft(0),
      _scrollsRemainingUp(0),
      _scrollsRemainingDown(0),
	  _unitSize(unitSize)
{
    setWorldPosition(position);

    _mapExtents = Extents(position.getX(), position.getY(), width, height);

	// add map terrains to this object
	_buildMapTerrains();
}

void
MapGameObject::_freeObjects()
{
	BOOST_FOREACH(MUGameObject *go, _objects)
        delete go;
	_objects.clear();
}

MapGameObject::~MapGameObject()
{
    _freeObjects();
}

void
MapGameObject::addObject(GameObject* object)
{
    GameObject *go = new MUGameObject(object, _map, _mapView);
	_objects.push_back(dynamic_cast<MUGameObject*>(go));
	addChild(go);
}

void
MapGameObject::specificUpdate(GameState *gameState)
{       
	if(_scrollingLeft || _scrollingUp || _scrollingRight || _scrollingDown) {
		const int scrollAmount = 4;

		Extents e = _mapView->getExtents();
        Extents newE(e);
		if(_scrollingLeft) newE.setMinX(newE.getMinX() - scrollAmount);
        if(! _mapView->setExtents(newE)) {
            _scrollingLeft = false;
            newE = e;
        }
		if(_scrollingUp) newE.setMinY(newE.getMinY() - scrollAmount);
        if(! _mapView->setExtents(newE)) {
            _scrollingUp = false;
            newE = e;
        }
		if(_scrollingRight) newE.setMinX(newE.getMinX() + scrollAmount);
        if(! _mapView->setExtents(newE)) {
            _scrollingRight = false;
            newE = e;
        }
		if(_scrollingDown) newE.setMinY(newE.getMinY() + scrollAmount);
		if(! _mapView->setExtents(newE)) _scrollingDown = false;
	}
	
	if(--_scrollsRemainingRight <= 0) _scrollingRight = false;
	if(--_scrollsRemainingLeft <= 0) _scrollingLeft = false;
	if(--_scrollsRemainingUp <= 0) _scrollingUp = false;
	if(--_scrollsRemainingDown <= 0) _scrollingDown = false;
}

static const char *normalTerrainName = "terrains/Normal.jpg";

static const char *impassableTerrainNames[] = {
	"terrains/Impassable1.jpg",
	"terrains/Impassable2.jpg",
	"terrains/Impassable3.jpg",
	"terrains/Impassable4.jpg"};

static const char *gateTerrainName[] = {
    "terrains/Gate-anim0.jpg",
    "terrains/Gate-anim1.jpg",
    "terrains/Gate-anim2.jpg",
    "terrains/Gate-anim3.jpg"};

void
MapGameObject::_buildMapTerrains()
{
	int width = _map->getWidth();
	int height = _map->getHeight();
	for(int h=0; h<height; h++)
		for(int w=0; w<width; w++) {
            int cost = static_cast<int>(_map->getCost(MapElement(w,h)));
			_buildMapTerrain(Position(w*_unitSize, h*_unitSize), cost);
        }
}

void
MapGameObject::_buildMapTerrain(const Position& pos, int cost)
{
    stringstream terrainName;
    terrainName << "map terrain at (" << pos.getX() << "," << pos.getY() << ")";
    GameObject *terrain;
    
    if(cost==0) {
        // a basic normal terrain
        string terrName = normalTerrainName;
		terrain = new ImageBackedGameObject(terrainName.str(), 
											getLayer(),
											pos,
											terrName);
    } else if(cost == 1000) {
        // a basic impassable terrain
        int num = sizeof(impassableTerrainNames)
			/sizeof(impassableTerrainNames[0]);
        
        vector<string> assets;
        for(int i=0; i<num; i++)
			assets.push_back(impassableTerrainNames[i]);

		terrain = new ImpassableTerrain(terrainName.str(),
                                        getLayer(),
                                        pos,
                                        assets,
                                        25);
    } else if(cost == 98 || cost == 99) {
        // a gate animation
		int num = sizeof(gateTerrainName)
			/sizeof(gateTerrainName[0]);
        
		vector<string> assets;
        for(int i=0; i<num; i++)
			assets.push_back(gateTerrainName[i]);

		terrain = new RIBGameObject(terrainName.str(),
									getLayer(),
									pos,
									assets,
									25);
    }

    addObject(terrain);
}

void
MapGameObject::runCollisions(GameState *gameState,
                             GameObject* object)
{
    BOOST_FOREACH(GameObject* child, getChildren())
		child->collideWithObject(gameState, object);
}

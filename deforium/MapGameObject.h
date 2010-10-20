#ifndef __MAPGAMEOBJECT_H__
#define __MAPGAMEOBJECT_H__

#include <iostream>
#include <boost/bind.hpp>
#include <boost/signal.hpp>

#include "MUGameObject.h"
#include "map.h"
#include "MapView.h"

/** This class serves to tie together the map model and view and also
 * serves as a container for all of the map-unit game objects.
 */
class MapGameObject : public GameObject {
public:
    enum ScrollType {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    MapGameObject(const std::string& name,
                  int layer,
                  const Position& position,
                  int width,
                  int height,
                  Map *map,
                  MapView *mapView,
				  int unitSize);

    /** Destructor. Disconnects signals. */
    virtual ~MapGameObject();

    /** Wraps the passed in game object into a MUGameObject and adds
     * it as a child of this MapGameObject.
     */
    void addObject(GameObject* object);

	virtual void specificUpdate(GameState *gameState);

    void setScrollNTicks(ScrollType type, int numTicks) {
        switch(type) {
        case LEFT:
            _scrollingLeft = true;
            _scrollsRemainingLeft = numTicks;
            break;
        case RIGHT:
            _scrollingRight = true;
            _scrollsRemainingRight = numTicks;
            break;
        case UP:
            _scrollingUp = true;
            _scrollsRemainingUp = numTicks;
            break;
        case DOWN:
            _scrollingDown = true;
            _scrollsRemainingDown = numTicks;
            break;
        }        
    }

    /** Checks for collisions with all objects contained in this
     * object.
     */
    void runCollisions(GameState* gameState,
                       GameObject* object);

    Extents getExtents() const {
        return _mapExtents;
    }
    
private:
    Log* _log;
	Map *_map;
    MapView *_mapView;

	bool _scrollingLeft, _scrollingRight, _scrollingUp, _scrollingDown;
    int _scrollsRemainingRight, _scrollsRemainingLeft;
    int _scrollsRemainingUp, _scrollsRemainingDown;
	int _unitSize;

    Extents _mapExtents;

	std::vector< MUGameObject* > _objects;

	void _buildMapTerrains();
	void _buildMapTerrain(const Position& pos, int cost);

	void _freeObjects();
};

#endif	// __MAPGAMEOBJECT_H__

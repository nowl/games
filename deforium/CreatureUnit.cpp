#include <iostream>
#include <boost/any.hpp>

#include "map.h"
#include "CreatureUnit.h"
#include "Random.h"
#include "EventProcessor.h"
#include "astar.h"

using namespace std;
using namespace boost;

void CreatureUnit::_buildNewPath()
{    
    int mapX = getWorldPosition().getX() / 32;
    int mapY = getWorldPosition().getY() / 32;
    
    int newX, newY;
    do {
        newX = Random::intValMinMax(0, _map->getWidth()-1);
        newY = Random::intValMinMax(0, _map->getHeight()-1);
    } while(_map->getCost(MapElement(newX, newY)) != 1000);

    _path = astar::best_path(*_map, 
                             MapElement(mapX, mapY),
                             MapElement(newX, newY));
}

static int findX(const MapElement& start, 
                 const MapElement& end)
{
    if(end.x > start.x)
        return 1;
    else if(end.x < start.x)
        return -1;
    else
        return 0;
}


static int findY(const MapElement& start, 
                 const MapElement& end)
{
    if(end.y > start.y)
        return 1;
    else if(end.y < start.y)
        return -1;
    else
        return 0;
}


void CreatureUnit::specificUpdate(GameState *gameState)
{
    _energy += Random::floatVal();

    if(_energy > .30) {
        if(_path.size() == 0) {
            _buildNewPath();
            _moves = 0;
            _currentMapPos = _path[0];
            _path.erase(_path.begin());
        }

        int moveX = findX(_currentMapPos, _path[0]);
        int moveY = findY(_currentMapPos, _path[0]);
        
        Position pos = getWorldPosition() + Position(moveX, moveY);

        setWorldPosition(pos);
        
        _moves++;
        if(_moves == 32) {
            _currentMapPos = _path[0];
            _path.erase(_path.begin());
            _moves = 0;
        }

        _energy = 0;
    }
}

void
CreatureUnit::collideWithObject(GameState *gameState,
                                GameObject *gameObject)
{
	if(gameObject->getName() == "player")
	{
        Extents bbox = gameObject->getBBox();
        if(coarseBBIntersect(getBBox(), bbox)) {
            vector< any > data;
            gameObject->processEvent(Event(Event::ENEMY_COLLISION, data),
									 gameState);
        }
    }
}

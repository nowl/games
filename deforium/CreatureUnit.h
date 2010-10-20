#ifndef __CREATUREUNIT_H__
#define __CREATUREUNIT_H__

#include <iostream>

#include "ImageBackedGameObject.h"

class CreatureUnit : public ImageBackedGameObject {
public:
	CreatureUnit(const std::string& name,
                 int layer,
				 const Position& position,
                 const std::string& imageResourceName,
                 Map *map)
        : ImageBackedGameObject(name, layer, position, imageResourceName),
        _energy(0),
        _changePathProb(0.01),
        _moves(0),
        _map(map)
        {}
	
	virtual ~CreatureUnit() {};
	
    virtual void specificUpdate(GameState *gameState);

    virtual void collideWithObject(GameState *gameState,
                                   GameObject *gameObject);

private:
    float _energy;
    float _changePathProb;
    int _moves;
    std::vector< MapElement > _path;
    MapElement _currentMapPos;
    Map *_map;

    void _buildNewPath();
};

#endif  // __CREATUREUNIT_H__

#ifndef __IMPASSABLE_TERRAIN_H__
#define __IMPASSABLE_TERRAIN_H__

#include "EventProcessor.h"
#include "RIBGameObject.h"

class ImpassableTerrain : public RIBGameObject {
public:
    ImpassableTerrain(const std::string& name,
                      const int layer,
                      const Position& position,
                      const std::vector<std::string>& imageResourceNames,
                      unsigned int waitTime)
        : RIBGameObject(name, layer, position,
                        imageResourceNames,
                        waitTime)
    {}
    
    virtual ~ImpassableTerrain() {};

    virtual void collideWithObject(GameState *gameState,
                                   GameObject *gameObject)
    {    
		Extents bbox = gameObject->getBBox();
		if(coarseBBIntersect(getBBox(), bbox)) {
			std::vector< boost::any > data;
			gameObject->processEvent(Event(Event::WALL_COLLISION, data),
									 gameState);
		}
    }
};

#endif	// __IMPASSABLE_TERRAIN_H__

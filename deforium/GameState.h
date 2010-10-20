#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

//#include <map>
//#include <ostream>
//#include <string>
#include <vector>
#include <tr1/unordered_map>

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

#include "Position.h"
#include "Extents.h"

class GameObject;

class GameState : private boost::noncopyable {
public:
    GameState();

	bool mainLoopRunning;

	std::vector<GameObject*> renderList;

    unsigned int normalUnitSize;

    Position offsetInEffect;
    Extents screenClipInEffect;

    unsigned int tickCount;
    
	int minRenderLayer, renderLayer, maxRenderLayer;
    
    boost::any& operator[](const std::string& resourceName) {
        return _res[resourceName];
    }

	//std::string toString() const;

private:
    std::tr1::unordered_map<std::string, boost::any> _res;

};
/*
std::ostream& operator<<(std::ostream& out, 
						 const GameState& gameState);
*/
//extern GameState GlobalGameState;

#endif  // __GAMESTATE_H__

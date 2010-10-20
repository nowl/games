#include "EventProcessor.h"
#include "GameObject.h"
#include "Log.h"

#include <boost/foreach.hpp>

#include <vector>

using namespace std;

static bool
processEventAux(GameObject* obj, GameState* gameState, const IEvent& event)
{
	vector<GameObject*> children = obj->getChildren();

	bool result = false;
	BOOST_FOREACH(GameObject *child, children) {
		result = processEventAux(child, gameState, event);
		if(result)
			return true;
	}

	return obj->processEvent(event, gameState);
}

bool processEventGlobal(GameState& gameState, const IEvent& event)
{
	BOOST_FOREACH(GameObject *gu, gameState.renderList) {
		if(processEventAux(gu, &gameState, event))
			return true;
	}
	
	return false;		
}
